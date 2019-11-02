LAB0 of Concurrent Programming Course
-------------------------------------

INTRODUCTION
------------
    This is an application developed to sort an input file with a list of integer elements and it exports the 
    sorted list into an output file or prints it on the console.

CODE ORGANIZATION
-----------------

    lab0
    |
    |----main.c     -   This is the main application file that contains the sequential flow of operations
    |
    |----helper.c   -   This file contains definition of all the helper functions to parse the command 
    |                   line argument, read the elements from file and populate sorted elements to the file
    |
    |----helper.h   -   This file contains declaration of all the helper functions
    |
    |----mysort.c   -   This file contains the implementation of the merge sort algorithm
    |
    |----mysort.h   -   This file contains the definition of merge sort functions
    |
    |----Makefile   -   This file contains the project build and cleanup script


BUILD INSTRUCTION AND RUN STEPS
-------------------------------
    1.) Go to the project directory using the command - cd lab0
    2.) Run "make" inorder to build the project
    3.) Run the executable - mysort with appropriate command-line arguments
    4.) Run "make clean" inorder to clean the directory and remove all the object files and exe


CHOICE OF ALGORITHM
-------------------

    I have choose merge sort for this application due to the following reasons,

    1.) MergeSort is stable by design, equal elements keep their original order.

    2.) MergeSort is well suited to be implemented parallel (multithreading).

    3.) MergeSort uses (about 30%) less comparisons than QuickSort. This is an often overlooked 
    advantage, because a comparison can be quite expensive.

    Apart from all this, Merge sort has a guaranteed upper limit of time complexity O(nlogn).

PARALLELIZATION STRATEGIES
--------------------------
	Used OpenMP API to parallelize the merge sort algorithm. I have called merge sort in two
	different OpenMP sections and thus this creates two different threads. Finally in the
	main thread, the merge operation merges the two output of the merge sort into a final
	sorted list.
