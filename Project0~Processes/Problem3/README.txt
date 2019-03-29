-----------
File list
------------
problem3_driver.c	The driver program which handles distribution
problem3_procons.c	The producer-consumer program which specify a shared memory 				segment and able to read to/ write from that segment
p3_SS			Screenshot representation of Problem 3

----------------------
Command Line Arguments
----------------------
gcc -o exe file.c ...	in order to compile the file.c and obtain executable file
-lrt			producer consumer program requires lrt library



-------------------------
Design Decisions & Issues
-------------------------

$  	File must be compiled beforehand the working process.	
$  	Producer consumer program handles with memory management
$	Driver program handles parallel programming through establishing 		parent-	children relationship
$ 	Producer consumer program handles with producing and consuming seperately
$ 	In order to have a better randomization srand is used, and seed is 		arranged with respect to the current process
$	Driver program requires a message, the name of the executable file, and 	number of child processes.
$ 	Driver program may take as many as command line paramaters more than 3, and 		except first two parameters, remaining ones are dedicated to the message. 		User can enter a multiple-word message.
$ 	The swap process is only involves alpha-numeric characters, hence word 		lengths never change (space as non alpha numerical)
$	The child's id determines which process it must handle. For ID = 1, 		producer part must produce the resource, for ID <= total number the game 		continues as expected, for ID = total number, the shared memory is unlinked.
$	The driver program implicitly executes the producer consumer program via 		executable file, and informs producer consumer program about the message, 		the current child ID, and the total number of children.
$	The shuffle/swap part of the producer consumer program is a randomized 		algorithm for swapping operations. Since processes must be continued in 	parallel randomization is handled with current children ID which seems the 		only relaible way to obtain better pseudo-randomized values.
$	The randomization process is crucial, since the rand and srand functions 		may provides a very limited randomness which is observed as the same series 		of swaps and returning back to original message.

------------------------
Notes
------------------------
$	Please check screenshot of the results.
$ 	The compilation process is a bit confusing. Please follow this path:

	@
	>>gcc -o driver driver.
	>>driver executable N msg
	
$	If user would like to obtain executable this file must follow this path:

	@
	>>gcc -o exe procons.c -lrt

$	if user would like to test the executable file must follow this path:

	...after obtaining the executable file
	@
	>>executable msg currentChild totalChild

	***which under following parameter constraints:
	*currentChild =1 (without production it would be a shared memory error)
	*msg must be one argument	
	
