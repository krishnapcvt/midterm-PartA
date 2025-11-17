#ifndef _SPINLOCK_MCS_H_
#define _SPINLOCK_MCS_H_

#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>

#define cpu_relax() asm volatile("pause\n": : :"memory")


typedef struct mcs_node {
    _Atomic(struct mcs_node *) next;
    _Atomic(bool) locked;     // If true, spin
} mcs_node_t;

typedef struct {
    _Atomic(mcs_node_t *) tail;
} spinlock;

#define SPINLOCK_INITIALIZER { .tail = NULL }

// thread-local node
static __thread mcs_node_t my_mcs_node;


static inline void spin_lock(spinlock *lock)
{
    mcs_node_t *prev;

    my_mcs_node.next = NULL;
    atomic_store_explicit(&my_mcs_node.locked, true, memory_order_relaxed);

    /* Atomically swap tail with my node */
    prev = atomic_exchange_explicit(&lock->tail, &my_mcs_node,
                                    memory_order_acq_rel);

    if (prev != NULL) {
        /* Link into queue */
        atomic_store_explicit(&prev->next, &my_mcs_node, memory_order_release);

        /* Spin until predecessor clears the flag */
        while (atomic_load_explicit(&my_mcs_node.locked, memory_order_acquire))
            cpu_relax();
    }
}

static inline void spin_unlock(spinlock *lock)
{
    mcs_node_t *next =
        atomic_load_explicit(&my_mcs_node.next, memory_order_acquire);

    if (next == NULL) {
        /* Try to reset tail to NULL */
        if (atomic_compare_exchange_strong_explicit(
                &lock->tail, & (mcs_node_t *){ &my_mcs_node }, NULL,
                memory_order_acq_rel, memory_order_acquire))
        {
            return; // No successor: lock released
        }

        /* Wait for successor to appear */
        while ((next = atomic_load_explicit(&my_mcs_node.next, memory_order_acquire)) == NULL)
            cpu_relax();
    }

    /* Hand off to next waiter */
    atomic_store_explicit(&next->locked, false, memory_order_release);
    my_mcs_node.next = NULL;
}

#endif /* _SPINLOCK_MCS_H_ */
