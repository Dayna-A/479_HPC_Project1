# Project 1 - Variant of Leader Election Ring Topology
A project for CPSC 479 High Performance Computing at CSUF

## Created by 
[Dayna Anderson](dayna.anderson@csu.fullerton.edu)

[Roman Belaire](romanbelaire@csu.fullerton.edu)

[Oscar Olazabal](omolazabal@csu.fullerton.edu)

## Requirements

### Two Leader Election Problem in a Ring:

The leader election problem is the process of designating a single node in a distributed network of autonomous nodes as the organizer of some task. Each node need to agree on whom among them will be the leader node, and then the leader needs to be announced. Our version of the problem will select two leaders which we can call president and vice-president. The president will be the largest even value and the vice president will be the largest odd value. The two elections can run concurrently (by sending/receiving two values in the same MPI send) or separately (sending one message for president-even value and another message for vice-president-odd value).

In the leader selection, each process generates a single random integer value and does some computations on it before sending results on to the next node:
* The value needs to be positive.
* The value needs to be greater than 10 and less than 100
* The value gets concatenated with the process rank and then the resulting value gets concatenated, such that two random values that are equal become distinct. For example, if the process of rank 0 has the resulting value 15, then the new value becomes 1015. Another example: if process of rank 2 has the resulting value of 78 then the new value becomes 1278.

### Concurrent two-leader election algorithm:

Process 0 sends its computer value to the left neighbor (clockwise). That value can be even or odd. If the value is even, then process 0 adds a second value of 1; if the value is odd, then process 0 adds a second value of 0. The two values are sent to process 1.

On receiving the value, process 1 compares:
* its value with the odd one if it has an odd value or 
* its value with the even one, if it has an even value. 

The largest value and the other unchanged value are sent further to process 2. Process 2 does the same check. The two values received at process 0 will be the largest even and odd generated values in the ring and will be declared by process 0 as the two leaders.
Run your program with N identical copies. You can choose the value of N to be between 6 and 20.

## Solution Pseudocode
```c++
MPI_INIT
k = MPI_RANK
s = MPI_SIZE
array​[​4​]
if​ k is ​not​​ 0​:
    receive ​array​ from process k​-1
else​:
    // Contents of array:
    // 0 - even value
    // 1 - odd value
    // 2 - rank of even value
    // 3 - rank of odd value
array​[​4​] = {​1000​, ​1001​, ​0​, ​0​}
    
N = positive number in range (​10​, ​100​)
N = concat(concat(​1​, rank), N)

if​ N%​2​ is ​1 ​​and​ N%​100​ >= ​array​[​1​]%​100​:
    array​[​1​] = N
    array​[​3​] = rank
else​​if​ N is N%​100​ >= ​array​[​0​]%​100​:
    array​[​0​] = N
    array​[​2​] = rank

send ​array​ from process k to k+​1

if​ k is ​0​:
    receive value from process s​-1
    president = ​array​[​2​]
    president_value = ​array​[​0​]%​100
    vice_president = ​array​[​3​]
    vice_president_value = ​array​[​1​]%​100
    
MPI_FINALIZE
```