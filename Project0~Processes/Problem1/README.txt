-----------
File list
------------
problem1_a.c		Solution for part a (fork())
problem1_b.c		Solution for part b (exec())
problem1_c.c		Solution for part c (zombie/orphan processes)
pc_SS			Screenshot representation of part c

----------------------
Command Line Arguments
----------------------
-&			in order to run the program in the background (part_b)
ps -f			in order to observe currently working processes (part_b&c)
gcc -o exe file.c	in order to compile the file.c and obtain executable file

-------------------------
Design Decisions & Issues
-------------------------

$  Each part is dedicated to individual c files with respect to assigned part name. 
$  Files must be compiled beforehand the working process.	
$  For this part of the assignment the codes are not neither complex nor intense.
	
------------------------
Notes
------------------------
$  User must be quick in order to observe zombie processes. Zombie process          would wait for 5 seconds before termination.
$  The order of the written output in the part a does not have to be in level order.
