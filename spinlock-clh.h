#ifndef _SPINLOCK_CLH_H_
#define _SPINLOCK_CLH_H_

#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#ifndef cpu_relax
#define cpu_relax() __asm__ __volatile__("pause" ::: "memory")
#endif

typedef struct clh_node {
    _Atomic(bool) locked;
    char pad[64 - sizeof(_Atomic(bool))]; // avoid false sharing
} clh_node_t;

typedef struct {
    _Atomic(clh_node_t *) tail;
} spinlock;

/* Thread-local nodes */
static __thread clh_node_t *my_node = NULL;
static __thread clh_node_t *my_prev = NULL;

/* Allocate a node (not aligned) */
static inline clh_node_t *alloc_node(void) {
    clh_node_t *n = malloc(sizeof(clh_node_t));
    if (!n) {
        perror("malloc");
        abort();
    }
    atomic_store(&n->locked, false);
    return n;
}

/* Ensure tail is initialized */
static inline void ensure_tail(spinlock *lock) {
    clh_node_t *old_tail = atomic_load_explicit(&lock->tail, memory_order_acquire);
    if (!old_tail) {
        clh_node_t *dummy = alloc_node();
        clh_node_t *expected = NULL;
        if (!atomic_compare_exchange_strong(&lock->tail, &expected, dummy)) {
            free(dummy); // another thread already initialized
        }
    }
}

/* Lock */
static inline void spin_lock(spinlock *lock) {
    if (!my_node)
        my_node = alloc_node();

    atomic_store_explicit(&my_node->locked, true, memory_order_relaxed);

    ensure_tail(lock); // make sure tail is not NULL

    my_prev = atomic_exchange_explicit(&lock->tail, my_node, memory_order_acq_rel);

    while (atomic_load_explicit(&my_prev->locked, memory_order_acquire))
        cpu_relax();
}

/* Unlock */
static inline void spin_unlock(spinlock *lock) {
    atomic_store_explicit(&my_node->locked, false, memory_order_release);
    my_node = my_prev; // reuse previous node for next lock
}

#endif /* _SPINLOCK_CLH_H_ */
