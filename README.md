to compile, use the command:
gcc -O2 -pthread test-spinlock.c -DTTAS -o .\test-spinlock-ttas
Replace DTTAS with DXCHG, DPTHREAD, DCMPXCHG, DMCS and DCLH as needed to get the respective output and change th eoutput file correctly.
Run shell script to observe peroformance on the server.
