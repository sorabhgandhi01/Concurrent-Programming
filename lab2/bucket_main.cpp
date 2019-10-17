/***************************************************************************************************
MIT License

Copyright (c) 2018 Sorabh Gandhi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************************************/

/**
 * @\file	main.c
 * @\author	Sorabh Gandhi
 * @\brief	This file contains the application code for lab0 exercise 
 * @\date	09/05/2019
 *
 */

/* C++ headers */
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set> 
#include <iterator> 

/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

/* Own Headers */
#include "helper.h"
#include "lock.h"

using namespace std;


struct bucket_task {

	int task_no;
	int task_div;
	int task_size;
	int *list;
};

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

pthread_barrier_t bar;
struct timespec start, end1;
struct arg_handler arg_handler_t = {"Sorabh Gandhi"};

int TEST_NUM = 0;
vector <multiset <int32_t> > B;
char lockname[] = "pthread";
MCSLock my_mcs_lock;
atomic<Node*> tail{NULL};

//function declaration
int get_bucket_range(int arr[], int size, int thread);
void *bucketSort(void *arg);

const int NUM_FUNCS = 8;
void (*funcs_lock[NUM_FUNCS])()  = {
    tas_lock,
    ttas_lock,
    ticket_lock,
    pthread_lock,
    tas_unlock,
    ttas_unlock,
    ticket_unlock,
    pthread_unlock
};

const char* func_names_lock[NUM_FUNCS/2] = {
    "tas",
    "ttas",
    "ticket",
    "pthread"
};

int get_bucket_range(int arr[], int size, int thread)
{
    int max = 0, bucket = 0, divider = 0;

	bucket = thread;

    //find max
    max = *max_element(arr, arr + size);
    divider = ceil(float(max + 1) / bucket);

    return divider;
}

void *bucketSort(void *arg) {
   
	struct bucket_task *btsk = (struct bucket_task *) arg;
	int i = 0, j = 0;

	void (*lock)() = funcs_lock[TEST_NUM];
    void (*unlock)() = funcs_lock[TEST_NUM + 4];

	pthread_barrier_wait(&bar);
    if(btsk->task_no == 0){
    	if ((strcmp(func_names_lock[TEST_NUM], "pthread") == 0)) {
    		if (pthread_mutex_init(&lock1, NULL) != 0) {
    			printf("Mutex init failed\n");
    		}
    	}
        clock_gettime(CLOCK_MONOTONIC,&start);
    }
    pthread_barrier_wait(&bar);

	printf("Executing thread %d\n", (btsk->task_no + 1));
  
	//insert element into bucket
	for (i = 0; i < btsk->task_size; i++) {
		j = floor( btsk->list[i] / btsk->task_div );

		if ((arg_handler_t.is_lock_set) && (strcmp(arg_handler_t.lock, "mcs") == 0)) {
			Node *mynode = new Node;
			my_mcs_lock.acquire(mynode);
			B[j].insert((btsk->list)[i]);
			my_mcs_lock.release(mynode);
		}
		else {
			lock();
			B[j].insert((btsk->list)[i]);
			unlock();
		}
	}

	pthread_barrier_wait(&bar);

	return 0;
}

int main (int argc, char **argv)
{
    
    int i = 0, k = 0;
    int ret_status = arg_parser(argc, argv, &arg_handler_t);    //parse the input argument

    /*If the arguments are incorrect*/
    if (ret_status == -1) {
        exit(-1);
    }

    /*If the '--name' flag is used*/
    if (ret_status == 1) {
        exit(0);
    }

    if ((arg_handler_t.is_barrier_set) && (arg_handler_t.is_lock_set)) {
    	printf("Invard arg: Both lock and barrier is set\n");
    	exit(0);
    }
    else if (arg_handler_t.is_lock_set) {
    	for (i = 0; (i < (NUM_FUNCS/2)); i++) {
    		if (strcmp(arg_handler_t.lock, func_names_lock[i]) == 0) {
    			TEST_NUM = i;
    		}
    	}
    }
    else {
    	arg_handler_t.lock = lockname;
    	TEST_NUM = 3;

    }

    int array_size = ((arg_handler_t.f_size));  //Calculate the number of elements in array
    int list[array_size];   //Initialize an array with set number of elements

	pthread_t threads[arg_handler_t.thread];

    /*Read integers from the file and store in local array*/
    if (insert_elements_to_array(arg_handler_t, list) != 0) {
        exit(-1);
    }

	//Assign one thread if the coomand line thread input is 0 or less than 0
	if (arg_handler_t.thread <= 0) {
		arg_handler_t.thread = 1;
	}

	printf("Executing bucket sort\n");
	struct bucket_task btsk[arg_handler_t.thread];
	int divider = get_bucket_range(list, array_size, arg_handler_t.thread);
	B.resize(arg_handler_t.thread);
	int len = (array_size / arg_handler_t.thread);
	int m = 0;

	pthread_barrier_init(&bar, NULL, arg_handler_t.thread);

	for (i = 0; i < arg_handler_t.thread; i++) {

		m = i*len;

		if (i == (arg_handler_t.thread - 1)) {

			btsk[i].task_div = divider;
            btsk[i].task_no = i;
            btsk[i].task_size = (array_size - m);
            btsk[i].list = &list[m];
				
		} else {
		
			btsk[i].task_div = divider;
			btsk[i].task_no = i;
			btsk[i].task_size = len;
			btsk[i].list = &list[m];
		}

		if ((pthread_create(&threads[i], NULL, bucketSort, (void *)&btsk[i])) != 0)
		{
			printf("Error on creating the thread\n");
			exit(1);
		} else {
			printf("Creating thread %d\n", (i+1));
		}

	}

	for (i = 0; i < arg_handler_t.thread; i++) {
		printf("Joining thread %d\n", (i+1));
        pthread_join(threads[i], NULL);
    }

	int vec_len = (int)B.size();

	for (i = 0; i < vec_len; i++) {
		for (std::multiset<int>::iterator j = B[i].begin(); j != B[i].end(); ++j)
		{
			list[k] = *j;
			k++;
		}
	}
	clock_gettime(CLOCK_MONOTONIC,&end1);

	pthread_barrier_destroy(&bar);

	if ((arg_handler_t.is_lock_set) && (strcmp(arg_handler_t.lock, "pthread") == 0)) {
		pthread_mutex_destroy(&lock1);
	}
	

    //Check if output is required to print on console or in a file
    if ((arg_handler_t.print_on_console))
    {
        insert_elements_to_file(arg_handler_t, list);
    } else {
        print_to_console(arg_handler_t, list);
    }

	unsigned long long elapsed_ns;
	elapsed_ns = (end1.tv_sec-start.tv_sec)*1000000000 + (end1.tv_nsec-start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);

    return 0;
}