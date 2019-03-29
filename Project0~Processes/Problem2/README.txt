-----------
File list
------------
problem2.c		Solution for Problem 2
p2_SS			Screenshot representation of Problem 2

----------------------
Command Line Arguments
----------------------
gcc -o exe file.c ...	in order to compile the file.c and obtain executable file 				with given parameters


-------------------------
Design Decisions & Issues
-------------------------

$  	File must be compiled beforehand the working process.	
$  	For this part of the assignment the program is designed to create 2*N pipes 		between the parent and the children in order to establish communication.
$ 	The program is able to distribute the data fairly between children, for 	further info please check the code segment related with determination of 		the partition minimum and maximum index
$ 	M > N > 0, and for an efficient process N<<100, for a fair distribution 	M<100
------------------------
Notes
------------------------
$	Please check screenshot of the results.
	
