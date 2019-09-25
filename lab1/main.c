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

/* System Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

/* Own Headers */
#include "helper.h"
#include "mysort.h"

struct task {

	int task_no;
	int task_low;
	int task_high;
	int *list;
};

void *merge_sort123(void *arg)
{
    struct task *tsk = arg;
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
	struct task *tsk;

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
    struct task tsklist[arg_handler_t.thread];

	int len = (array_size / arg_handler_t.thread);
	int i = 0, low = 0;

    /*Read integers from the file and store in local array*/
    if (insert_elements_to_array(arg_handler_t, list) != 0) {
        exit(-1);
    }

	if (arg_handler_t->algos == 0) {

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
		for (int i = 0; i < arg_handler_t.thread; i++) {
			tsk = &tsklist[i];
			pthread_create(&threads[i], NULL, merge_sort123, tsk);
		}

    		// joining all 4 threads
		for (int i = 0; i < arg_handler_t.thread; i++) {
			pthread_join(threads[i], NULL);
		}

		struct task *tskm = &tsklist[0];
		for (int i = 1; i < arg_handler_t.thread; i++) {
			struct task *tsk = &tsklist[i];
			merge(tsk->list, tskm->task_low, tsk->task_low - 1, tsk->task_high);
		}

	} else {

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
