The objective of this assignment, originally created by Dr. Nikolopoulos, is to implement mutual exclusion locks that are efficient and scalable. 

**Logistics**
You need access to the Computer Science (CS) rlogin server cluster for this assignment. You need  CS credentials to access the system (by performing an ssh to rlogin.cs.vt.edu). If you are a Computer Science student, you should already have CS credentials. If you are an ECE student, you should create an account by [following this link](https://admin.cs.vt.edu/create.pl). **The username will match the user's Virginia Tech username exactly. The password can and should be different from the user's Virginia Tech password.** Password policies are available here: [Password Requirements](https://wiki.cs.vt.edu/index.php/Password_Requirements).

You will submit the assignment using the course [GitHub Classroom](https://classroom.github.com/classrooms/239649709-virginia-tech-cs-ece-5510-classroom-8e49b1). Your instructor will provide you with a link to access your repo and submit the assignment. Your repository should include everything the assignment asks of you, including the code, report with correctness and performance measurements (in markup language), and a comprehensive README file that will allow us to quickly compile, run, test, and experimentally evaluate your code.

We chose the source code language to C for this assignment, as writing the code in C allows you to learn and understand better the semantics of hardware instructions for synchronization, as well as their implications on the safety, liveness, and performance properties of synchronization algorithms.

**Objectives**
We provide you with three implementations of spin locks. The first implementation uses the native POSIX threads library mutual exclusion lock. The second and third implementation use a basic spinlock using a test-and-set operation, which is implemented in inline x86 Assembly using the xchg (second implementation) and cmpxchg (third implementation) instructions, respectively.

We are asking you to provide three additional lock implementations:
1. A test-and-test-and-set (TTAS) lock spin lock that uses backoff.  **(30% of your grade).**

2. The MCS queue lock -- MCS stands for John Mellor-Crummery and Michael Scott, the two researchers who invented this algorithm.  **(30% of your grade)**.

3. The CLH lock -- where CLH is for Craig, Landin, and Hagersten.  **(30% of your grade).**

We also ask you to provide a report with your lock performance measurements as a markdown file with this directory **(10% of your grade)**.

The repository comes with a test to measure the performance and check the correctness of your locks. The test checks the correctness and measures the performance of the locks on up to 32 cores. You will run all your experiments on the Computer Science Department rlogin cluster servers.

**Constraints**
1. You may use the provided inline x86 assembly for xchg and cmpxchg instructions or the stdatomic.h library for atomics only. You are not allowed to build your locks on top of an existing threads library mutex primitive.
2. Your implementations must use the provided test code, do not create your own. Extend the provided Makefile and test code to run your lock implementations as well as the three provided implementations.

**References**
Chapter 7 of your textbook covers the spinlock algorithms we consider in this assignment, including references to seminal papers describing the most successful and widely used algorithms. We particularly wish to draw your attention to the classic paper of John Mellor-Crummey and Michael Scott (reference [124], work that has also earned the authors the Edsger W. Dijkstra Prize in Distributed Computing), an earlier classic work by Larry Rudolph and Zary Sgeall on the TTAS lock (reference [150]), and an also classic but largely overlooked technical report by Travis Craig on queue lock s(references [32]). References [37], [121], and [144] might be good starting points for thinking about how to implement more efficient locks.
