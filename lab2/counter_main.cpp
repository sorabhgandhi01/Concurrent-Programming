/***************************************************************************************************
MIT License

Copyright (c) 2019 Sorabh Gandhi

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
 * @\brief	This file contains the application code for lab2 counter exercise
 * @\date	09/05/2019
 *
 */


/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

/* Own Headers */
#include "helper.h"
#include "lock.h"

using namespace std;

typedef struct barrier 
{
	atomic<int> cnt;
	atomic<int> sense;
	int N;
}bar_t;

struct timespec start, end1;
struct arg_count_handler arg_handler_t = {"Sorabh Gandhi"};
bar_t sense_barrier;
pthread_barrier_t bar;

extern pthread_mutex_t mutex_lock;
extern pthread_barrier_t pth_barr;

int count = 0;
int TEST_NUM = 0;

void sense_rev_barrier ()
{
	thread_local bool my_sense = 0;
	if (my_sense == 0) {
		my_sense = 1;
	} else {
		my_sense = 0;
	}

	int cnt_cpy = sense_barrier.cnt.fetch_add(1);
	if (cnt_cpy == arg_handler_t.thread) {
		sense_barrier.cnt.store(0, memory_order_relaxed);
		sense_barrier.sense.store(my_sense);
	} else {
		while (sense_barrier.sense.load() != my_sense);
	}
}

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

const int NUM_BAR = 2;
void (*funcs_bar[NUM_BAR])()  = {
    sense_rev_barrier,
    pthread_barrier
};

const char* func_names_bar[NUM_BAR] = {
    "sense",
    "pthread"
};

void (*barrier)() = NULL;

void* thread_main(void* args){
    int tid = *((int *)args);

    void (*lock)() = funcs_lock[TEST_NUM];
    void (*unlock)() = funcs_lock[TEST_NUM + 4];
    
    pthread_barrier_wait(&bar);
    if(tid == 0) {
    	if ((arg_handler_t.is_lock_set) && (strcmp(func_names_lock[TEST_NUM], "pthread") == 0)) {
    		if (pthread_mutex_init(&mutex_lock, NULL) != 0) {
    			printf("Mutex init failed\n");
    		}
    	}
        clock_gettime(CLOCK_MONOTONIC, &start);
    }
    pthread_barrier_wait(&bar);
   	
    int i = 0;
    for (i = 0; i < (arg_handler_t.iteration * arg_handler_t.thread); i++)
    {
    	if ((i % arg_handler_t.thread) == tid) {
    		if (arg_handler_t.is_lock_set) {
    			lock();
    			count++;
    			unlock();
    		} else {
    			count++;
    		}

    		if (arg_handler_t.is_barrier_set) {
    			barrier();
    		}
    	}
    }

    pthread_barrier_wait(&bar);
    if(tid == 0){
        clock_gettime(CLOCK_MONOTONIC,&end1);
    }

    return 0;
}


int main (int argc, char **argv)
{
    
    int ret_status = arg_count_parser(argc, argv, &arg_handler_t);
    int i = 0;
    /*If the arguments are incorrect*/
    if (ret_status == -1) {
        exit(-1);
    }

    /*If the '--name' flag is used*/
    if (ret_status == 1) {
        exit(0);
    }

    if (arg_handler_t.thread < 1) {
    	arg_handler_t.thread = 1;
    }

    if (arg_handler_t.iteration < 1) {
    	arg_handler_t.iteration = 1;
    }

	if (arg_handler_t.is_barrier_set == arg_handler_t.is_lock_set) {
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
    else if (arg_handler_t.is_barrier_set) {
    	if (arg_handler_t.barrier) {
    			pthread_barrier_init(&pth_barr, NULL, arg_handler_t.thread);
    			barrier = funcs_bar[1];
    	} else {
    		barrier = funcs_bar[0];
    	}
    }

    pthread_t threads[arg_handler_t.thread];
    int thread_id[arg_handler_t.thread];
    pthread_barrier_init(&bar, NULL, arg_handler_t.thread);

	// creating 4 threads
	for (i = 0; i < arg_handler_t.thread; i++) {
		thread_id[i] = i;
		if ((pthread_create(&threads[i], NULL, thread_main, &thread_id[i])) != 0) {
			printf("Error on creating the thread\n");
            exit(1);
		} else {
			printf("Creating thread %d\n", (i+1));
		}
	}

	// joining all 4 threads
	for (i = 0; i < arg_handler_t.thread; i++) {
		printf("Joining thread %d\n", (i+1));
		pthread_join(threads[i], NULL);
	}

	    //Check if output is required to print on console or in a file
    if ((arg_handler_t.print_on_console))
    {
        insert_count_to_file(arg_handler_t, count);
    } else {
        printf("Count - %d\n", count);
    }

	unsigned long long elapsed_ns;
    elapsed_ns = (end1.tv_sec - start.tv_sec)*1000000000 + (end1.tv_nsec - start.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);
    double elapsed_s = ((double)elapsed_ns)/1000000000.0;
    printf("Elapsed (s): %lf\n",elapsed_s);


	pthread_barrier_destroy(&bar);
	if ((arg_handler_t.is_barrier_set) && (arg_handler_t.barrier)) {
		pthread_barrier_destroy(&pth_barr);
	}
	if ((arg_handler_t.is_lock_set) && (strcmp(arg_handler_t.lock, "pthread") == 0)) {
		pthread_mutex_destroy(&mutex_lock);
	}

    return 0;
}