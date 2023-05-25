# buddy_system

Replicating simple buddy system, memory management and allocation algorithm

3 commands: 'a' for allocation 'f' for free and 'q' to quit
Need to put number after 'a' and 'f' for space you want to allocate/free

How to run (command line):
gcc buddy.cc
./a.out

Enviroment:
g++ 9.4.0 (might not work in gcc)
Ubuntu 9.4.0-1ubuntu1~20.04.1

Result:
tam7777@DESKTOP-0I82836 ~/dev/school/OS/buddy_system$ g++ buddy.cc
tam7777@DESKTOP-0I82836 ~/dev/school/OS/buddy_system$ ./a.out

(splitting 0/64)
(splitting 0/32)
(splitting 0/16)
(splitting 0/8)
How many blocks do you want to allocate/fee?
a 16
Blockss 16-32 allocated:
--------|--------|################|--------------------------------|
How many blocks do you want to allocate/fee?
a 4
Blocks 0-3 allocated:
####|----|--------|################|--------------------------------|
How many blocks do you want to allocate/fee?
a 31
Blockss 32-63 allocated:
####|----|--------|################|###############################-|
How many blocks do you want to allocate/fee?
f 16
Merging to the left node
mergering (8/8) and (16/16) 01
####|----|------------------------|###############################-|
How many blocks do you want to allocate/fee?
q
