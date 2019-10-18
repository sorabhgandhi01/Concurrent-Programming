## LAB1 of Concurrent Programming Course

### INTRODUCTION

    This is an application developed to parallely sort an input file with a list of integer elements and it exports the sorted list into an output file or prints it on the console. This application can parallely sort the list using merge and bucket sort individually.


### CODE ORGANIZATION


    lab0
    |
    |----main.c		-   This is the main application file that contains the sequential flow operations
	|						and the API declaration and definition for bucket sort 
    |----main.c		-   This is the main application file that contains the sequential flow of operations
    |
    |----helper.c	-   This file contains definition of all the helper functions to parse the command 
    |                       line argument, read the elements from file and populate sorted elements to the file
    |
    |----helper.h	-   This file contains declaration of all the helper functions
    |
    |----mergesort.c-   This file contains the implementation of the merge sort algorithm
    |
    |----mergesort.h-   This file contains the definition of merge sort functions
    |
    |----Makefile	-   This file contains the project build and cleanup script


### BUILD INSTRUCTION AND RUN STEPS

    1.) Go to the project directory using the command - cd lab1
    2.) Run "make" inorder to build the project
    3.) Run the executable - mysort with appropriate command-line arguments
    4.) Run "make clean" inorder to clean the directory and remove all the object files and exe


### Parallelization strategy for Merge Sort


    1.) Divide the input elements in equal halves based on number of threads being used.

	2.) Calculate the start and end array value for each thread and store it in a structure.

	3.) Create thread with merge function handler and the populated structure as an argument.

	4.) The thread handler function will perform merge sort in their respective sub-array.

	5.) Then join all the threads and merge all the sub-array to the main array. 

### Parallelization strategy for Bucket Sort

	1.) Have the number of buckets equal to number of threads

	2.) Calculate the maximum element in the array and based of that calculate range of each bucket

	3.) Divide the input elements in equal halves based on number of bucket being used and store in the
		argument structure which will be passed to the thread handler.

	4.) Create thread with bucket function handler and the populated structure as an argument.

	5.) The thread handler function will assign the elements in their sub-array to an appropriate bucket

	6.) Then join all the threads and merge all the sub-array to the main array.

### Locks and Barrier performance stats on counter application

    |  Locks | Run Time  |  L1 cache hit rate | Brance Prediction hit rate | page-fault count |
    |--------|-----------|--------------------|----------------------------|------------------|
    | tas    | 0.007997  |  96.29%            | 1.36%                      |    236           |
    | ttas   | 0.000802  |  94.74%            | 2.12%                      |    238           |
    | mcs    | 40.79373  |  99.99%            | 99.99%                     |    375           |
    | pthread| 0.261842  |  96.29%            | 97.92%                     |    231           |

    | Barrier| Run Time  |  L1 cache hit rate | Brance Prediction hit rate | page-fault count |
    |--------|-----------|--------------------|----------------------------|------------------|
    |sense   |170.149125 |  99.99%            | 99.99%                     |    232           |
    |pthread | 0.261842  |  87.86%            | 97.92%                     |    228           |


### Locks and Barrier performance stats on bucket sort application

    |  Locks | Run Time  |  L1 cache hit rate | Brance Prediction hit rate | page-fault count |
    |--------|-----------|--------------------|----------------------------|------------------|
    | tas    | 0.554375  |  98.46%            | 99.59%                     |    8424          |
    | ttas   | 0.492622  |  99.26%            | 99.73%                     |    8421          |
    | mcs    | 0.589436  |  99.40%            | 99.83%                     |    13519         |
    | pthread| 0.848636  |  94.54%            | 98.89%                     |    8426          |
    

### Observation

    1.)

    2.) 
