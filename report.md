The 3 additional Spinlocks implemented are using TTAS, MCS and CLH. 
This report evaluates the performance of several spinlock implementations under increasing contention.
The implementations tested were:
xchg spinlock (test-and-set)
cmpxchg spinlock (test-and-test-and-set + CAS)
pthread mutex
TTAS spinlock (test-and-test-and-set)
MCS queue lock
CLH queue lock

Each lock was measured across multiple thread counts (1, 2, 4, 8, 16, 32 threads), and each configuration repeated 3 times.

All measurements reported are execution times, so the lower the number the better is the performance.


These are the results of the different implementations of the Spinlock, when run on the server:

test spin lock using xchg
0.155387        0.155628        0.155004
0.961436        0.996251        0.810389
1.598935        1.605300        1.631620
4.732090        4.238891        4.793531
5.803589        6.377814        5.719715
9.113031        8.697902        9.296665
test spin lock using cmpxchg
0.155020        0.155670        0.155539
1.681890        1.541725        1.719819
6.505092        4.075729        5.400482
9.955215        8.166711        9.251940
15.869756       19.250058       17.760354
36.977554       34.773812       31.703513
test spin lock using pthread
0.350088        0.350117        0.350159
1.960579        1.773110        1.758490
2.241034        2.469371        2.337041
2.159848        2.328471        2.305002
2.247970        2.357152        2.289418
2.337092        2.182577        2.495651
test spin lock using TTAS
0.161128        0.161736        0.160956
0.162045        0.227969        0.170235
0.202198        0.199329        0.202541
0.214259        0.207306        0.213325
0.224139        0.213323        0.213263
0.212009        0.199487        0.211265
test spin lock using MCS
0.390296        0.390278        0.390301
3.290272        3.376521        3.836703
5.460017        5.533237        5.485636
5.777955        5.986474        5.829523
5.372987        5.268137        5.297912
5.347665        5.091129        4.881858
test spin lock using CLH
0.189614        0.189608        0.189555
2.548774        2.890246        2.744171
5.591129        5.586189        5.368817
6.088027        7.098582        7.168744
5.529627        5.879223        6.415572
6.274452        6.001291        6.153327



Clearly TTAS performs the best. An exponential backoff, during which the CPU sleeps plays a big role in the superior performance.
Queue-based locks (MCS and CLH) were better than TAS/CAS but worse than TTAS on this hardware.
cmpxchg spinlock showed extremely poor scalability, taking too long after 4–8 threads.

xchg: simple, but does poorly as threads increases. this is because of high contention and the atomoic operation causes 
multiple cache invalidations. 

cmpxchg: showed extremely poor scalability, taking too long after 4–8 threads. this is because of the expensive cache
invalidation. 

pthread_mutex: has low contention and decent performance, as it uses sleep/wakeup.

ttas: spins locally and doesn't perform the expensive atomic write operation unless it reads teh lock is free.
      this reduces cache invalidations and contention.

MCS: scalable queue lock, but overhead of node management affects performance.

CLH: slower than MCS even though they are similar sue to its higher overhead.

TTAS is the most favourable as it is not very complex so overhead is less and scales well.

