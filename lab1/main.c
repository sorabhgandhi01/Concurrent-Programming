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
#include "mysort.h"

using namespace std;

struct task {

	int task_no;
	int task_low;
	int task_high;
	int *list;
};

struct bucket_task {

	int task_no;
	int task_div;
	int task_size;
	int *list;
};

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

//function declaration
int get_bucket_range(int arr[], int size, int thread);
void *bucketSort(void *arg);



vector <multiset <int32_t> > B;

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
  
	//insert element into bucket
	for (i = 0; i < btsk->task_size; i++) {
		j = floor( btsk->list[i] / btsk->task_div );

		if (btsk->task_no == j)
		{
			pthread_mutex_lock(&lock1);
			B[j].insert((btsk->list)[i]);
			pthread_mutex_unlock(&lock1);
		}
	}

	return 0;
}

void *merge_sort123(void *arg)
{
    struct task *tsk = (struct task *)arg;
    int low;
    int high;

    // calculating low and high
    low = tsk->task_low;
    high = tsk->task_high;

    // evaluating mid point
    int mid = low + (high - low) / 2;

    if (low < high) {
        mergesort(tsk->list, low, mid);
        mergesort(tsk->list, mid + 1, high);
        merge(tsk->list, low, mid, high);
    }

    return 0;
}

int main (int argc, char **argv)
{
    struct arg_handler arg_handler_t = {"Sorabh Gandhi"};

    int ret_status = arg_parser(argc, argv, &arg_handler_t);    //parse the input argument

    /*If the arguments are incorrect*/
    if (ret_status == -1) {
        exit(-1);
    }

    /*If the '--name' flag is used*/
    if (ret_status == 1) {
        exit(0);
    }

    int array_size = ((arg_handler_t.f_size));  //Calculate the number of elements in array
    int list[array_size];   //Initialize an array with set number of elements

	pthread_t threads[arg_handler_t.thread];

    /*Read integers from the file and store in local array*/
    if (insert_elements_to_array(arg_handler_t, list) != 0) {
        exit(-1);
    }

	if (arg_handler_t.algos == 0) {
		printf("Executing merge sort\n");
		struct task *tsk;
		struct task tsklist[arg_handler_t.thread];
		int len = (array_size / arg_handler_t.thread);
		int i = 0, low = 0;

		for (i = 0; i < arg_handler_t.thread; i++, low += len) {

			tsk = &tsklist[i];
			tsk->task_no = i;
			tsk->list = list;

			tsk->task_low = low;
			tsk->task_high = low + len - 1;

			if (i == (arg_handler_t.thread - 1)) {
				tsk->task_high = array_size - 1;
			}
		}


		// creating 4 threads
		for (i = 0; i < arg_handler_t.thread; i++) {
			tsk = &tsklist[i];
			pthread_create(&threads[i], NULL, merge_sort123, tsk);
		}

    		// joining all 4 threads
		for (i = 0; i < arg_handler_t.thread; i++) {
			pthread_join(threads[i], NULL);
		}

		struct task *tskm = &tsklist[0];
		for (i = 1; i < arg_handler_t.thread; i++) {
			struct task *tsk = &tsklist[i];
			merge(tsk->list, tskm->task_low, tsk->task_low - 1, tsk->task_high);
		}

	} else {
		printf("Executing bucket sort\n");
		int i = 0, k = 0;
		struct bucket_task btsk[arg_handler_t.thread];
		int divider = get_bucket_range(list, array_size, arg_handler_t.thread);
		B.resize(arg_handler_t.thread);

		for (i = 0; i < arg_handler_t.thread; i++) {
			
			btsk[i].task_div = divider;
			btsk[i].task_no = i;
			btsk[i].task_size = array_size;
			btsk[i].list = list;

			if ((pthread_create(&threads[i], NULL, bucketSort, (void *)&btsk[i])) != 0)
			{
				printf("Error on creating the thread\n");
				exit(1);
			} else {
				printf("Creating thread %d\n", i);
			}

		}

		for (i = 0; i < arg_handler_t.thread; i++) {
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

	}

#if 0 
    //sort the input array
    mergesort(list, 0, (array_size - 1));
#endif

    //Check if output is required to print on console or in a file
    if (!(arg_handler_t.print_on_console))
    {
        insert_elements_to_file(arg_handler_t, list);
    } else {
        print_to_console(arg_handler_t, list);
    }

    return 0;
}
