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
 * @\brief	This file contains the application code for parallel tree
 * @\date	12/05/2019
 *
 */

#include "fine_lock_bst.h"
#include "rw_lock_bst.h"
#include "helper.h"

#define MAX_THREAD (10)
 
vector <range> querry[2];
vector <rangerw> querryrw[2];

struct thread_info
{
	int task_no;
	int task_len;
	int task_key;
	char filename[64];
};

bst_node *g_root;
bst_noderw *g_rw_root;
pthread_mutex_t bst_lock;
pthread_rwlock_t bst_rwlock;

struct timespec start, end1;

void *put_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
	int key = 0, value = 0, i = 0;

	FILE *fp = fopen(th_info->filename, "r");
	
	if (fp == NULL) {
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (fp))) {
		fscanf(fp, "%d %d\n", &key, &value);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(fp, "%d %d\n", &key, &value);
		put_node(NULL, key, value, th_info->task_no);
	}	

	fclose(fp);	

	return 0;
}

void *get_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
	bst_node *node;
	int key = 0, i = 0;

	FILE *fp = fopen(th_info->filename, "r");
	
	if (fp == NULL) {
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (fp))) {
		fscanf(fp, "%dn", &key);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(fp, "%d\n", &key);

		node = get_node(NULL, key);
		if (node == NULL) {
			printf("Node with key %d not found\n", key);
		} else {
			printf("Node with key %d contains value %d\n", node->key, node->value);
		}	
		node = NULL;
	}

    fclose(fp);	

    return 0;
}

void *range_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
    int start_key = 0, end_key = 0, i = 0;

    FILE *fp = fopen(th_info->filename, "r");

    if (fp == NULL) {
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (fp))) {
		fscanf(fp, "%d %d\n", &start_key, &end_key);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(fp, "%d %d\n", &start_key, &end_key);
		if (end_key > start_key) {
			//printf("Invoking range query result for %d to %d\n", start_key, end_key);
			get_nodes_inrange(NULL, start_key, end_key, th_info->task_no);
		}
	}

	fclose(fp);

	return 0;
}

void *rw_put_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
	int key = 0, value = 0, i = 0;

	FILE *fp = fopen(th_info->filename, "r");
	
	if (fp == NULL) {
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (fp))) {
		fscanf(fp, "%d %d\n", &key, &value);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(fp, "%d %d\n", &key, &value);
		put_node_rw(NULL, key, value, th_info->task_no);
	}	

	fclose(fp);	

	return 0;
}

void *rw_get_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
	bst_noderw *node;
	int key = 0, i = 0;

	FILE *fp = fopen(th_info->filename, "r");
	
	if (fp == NULL) {
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (fp))) {
		fscanf(fp, "%dn", &key);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(fp, "%d\n", &key);

		node = get_node_rw(NULL, key);
		if (node == NULL) {
			printf("Node with key %d not found\n", key);
		} else {
			printf("Node with key %d contains value %d\n", node->key, node->value);
		}	
		node = NULL;
	}

    fclose(fp);	

    return 0;
}

void *rw_range_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;
    int start_key = 0, end_key = 0, i = 0;

    FILE *fp = fopen(th_info->filename, "r");

    if (fp == NULL) {
		printf("Error in opening the file\n");
		exit(0);
	}

	while ((i < th_info->task_key) && (!feof (fp))) {
		fscanf(fp, "%d %d\n", &start_key, &end_key);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(fp, "%d %d\n", &start_key, &end_key);
		if (end_key > start_key) {
			//printf("Invoking range query result for %d to %d\n", start_key, end_key);
			get_nodes_inrange_rw(NULL, start_key, end_key, th_info->task_no);
		}
	}

	fclose(fp);	

	return 0;
}

//./tree -i test.txt -s search.txt -r range.txt -t 10 --lock=rw_lock
//./tree -i test_files/insert_10000.txt -s test_files/search_10000_highcontention.txt -r test_files/range_10000_highcontention.txt -t 6 --lock=rw_lock
int main(int argc, char **argv)
{
	struct arg_handler arg;
	arg_parser(argc, argv, &arg);

	if (arg.total_insert_keys < arg.thread) {
		arg.thread = (arg.total_insert_keys / 2);
	}

	if (arg.thread < 6) {
		arg.thread = 6;
	}

	struct thread_info th_info[arg.thread];
	pthread_t th[arg.thread];
	int i = 0;
	
	int length = (arg.total_insert_keys / (arg.thread - 4));
	int m = 0, j = 0;

	if (arg.is_rw_lock_set != true) {

		pthread_mutex_init(&bst_lock, NULL);
		clock_gettime(CLOCK_MONOTONIC,&start);

		for (i = 0; i < (arg.thread - 4); i++) {

			m = i*length;
			strcpy(th_info[i].filename, arg.ifile);

			if (i == (arg.thread - 5)) {
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_insert_keys - m);
				th_info[i].task_key = m;
			} else {
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			if (pthread_create(&th[i], NULL, put_handler, (void *)&th_info[i]) != 0) 
			{
				printf("Error on creating the thread\n");
				exit(0);
			} else {
				//printf("Create new insert thread %d with length %d and start key %d\n", th_info[i].task_no, th_info[i].task_len, th_info[i].task_key);
			}
		}

		length = (arg.total_search_keys/ 2);
		m = 0;
		j = 0;

		for (; i < (arg.thread - 2); i++) {
			strcpy(th_info[i].filename, arg.sfile);
			m = j*length;

			if (i == (arg.thread - 3)) {
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_search_keys - m);
				th_info[i].task_key = m;
			} else {
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}


			if (pthread_create(&th[i], NULL, get_handler, (void *)&th_info[i]) != 0) {
					printf("Error on creating the thread\n");
					exit(0);
			} else {
				//printf("Create new search thread %d with length %d and start key %d\n", th_info[i].task_no, th_info[i].task_len, th_info[i].task_key);
			}
			j++;
		}

		length = (arg.range_querries/ 2);
		m = 0;
		j = 0;
		for (; i < (arg.thread); i++) {

			strcpy(th_info[i].filename, arg.rfile);
			m = j*length;

			if (i == (arg.thread - 1)) {
				th_info[i].task_no = j;
				th_info[i].task_len = (arg.range_querries - m);
				th_info[i].task_key = m;
			} else {
				th_info[i].task_no = j;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			if (pthread_create(&th[i], NULL, range_handler, (void *)&th_info[i]) != 0) {
					printf("Error on creating the thread\n");
					exit(0);
			} else {
				//printf("Create new range querry thread %d with length %d and start key %d\n", th_info[i].task_no, th_info[i].task_len, th_info[i].task_key);
			}
			j++;
		}

		for (i = 0; i < arg.thread; i++) {
			pthread_join(th[i], NULL);
		}

		clock_gettime(CLOCK_MONOTONIC,&end1);

		pthread_mutex_destroy(&bst_lock);

		printf("\n\n Range querry result:\n");
		for (i = 0; i < 2; i++) {
			for (j = 0; j < querry[i].size(); j++) {
				printf("Range Querry by thread %d for %d to %d = %d\n",i, querry[i][j].start_key, querry[i][j].end_key, querry[i][j].node->key);
			}
		}

		printf("\nInorder Tree\n");
		print_tree(g_root);
		free_tree(g_root);

		unsigned long long elapsed_ns;
		elapsed_ns = (end1.tv_sec-start.tv_sec)*1000000000 + (end1.tv_nsec-start.tv_nsec);
		printf("Elapsed (ns): %llu\n",elapsed_ns);
		double elapsed_s = ((double)elapsed_ns)/1000000000.0;
		printf("Elapsed (s): %lf\n",elapsed_s);

	} else {

		pthread_rwlock_init(&bst_rwlock, NULL);
		clock_gettime(CLOCK_MONOTONIC,&start);
		for (i = 0; i < (arg.thread - 4); i++) {

			m = i*length;
			strcpy(th_info[i].filename, arg.ifile);

			if (i == (arg.thread - 5)) {
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_insert_keys - m);
				th_info[i].task_key = m;
			} else {
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			if (pthread_create(&th[i], NULL, rw_put_handler, (void *)&th_info[i]) != 0) 
			{
				printf("Error on creating the thread\n");
				exit(0);
			} else {
				//printf("Create new insert thread %d with length %d and start key %d\n", th_info[i].task_no, th_info[i].task_len, th_info[i].task_key);
			}
		}

		length = (arg.total_search_keys/ 2);
		m = 0;
		j = 0;

		for (; i < (arg.thread - 2); i++) {
			strcpy(th_info[i].filename, arg.sfile);
			m = j*length;

			if (i == (arg.thread - 3)) {
				th_info[i].task_no = i;
				th_info[i].task_len = (arg.total_search_keys - m);
				th_info[i].task_key = m;
			} else {
				th_info[i].task_no = i;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}


			if (pthread_create(&th[i], NULL, rw_get_handler, (void *)&th_info[i]) != 0) {
					printf("Error on creating the thread\n");
					exit(0);
			} else {
				//printf("Create new search thread %d with length %d and start key %d\n", th_info[i].task_no, th_info[i].task_len, th_info[i].task_key);
			}
			j++;
		}

		length = (arg.range_querries/ 2);
		m = 0;
		j = 0;
		for (; i < (arg.thread); i++) {

			strcpy(th_info[i].filename, arg.rfile);
			m = j*length;

			if (i == (arg.thread - 1)) {
				th_info[i].task_no = j;
				th_info[i].task_len = (arg.range_querries - m);
				th_info[i].task_key = m;
			} else {
				th_info[i].task_no = j;
				th_info[i].task_len = length;
				th_info[i].task_key = m;
			}

			if (pthread_create(&th[i], NULL, rw_range_handler, (void *)&th_info[i]) != 0) {
					printf("Error on creating the thread\n");
					exit(0);
			} else {
				//printf("Create new range querry thread %d with length %d and start key %d\n", th_info[i].task_no, th_info[i].task_len, th_info[i].task_key);
			}
			j++;
		}

		for (i = 0; i < arg.thread; i++) {
			pthread_join(th[i], NULL);
		}

		clock_gettime(CLOCK_MONOTONIC,&end1);

		pthread_rwlock_destroy(&bst_rwlock);

		printf("\n\n Range querry result:\n");
		for (i = 0; i < 2; i++) {
			for (j = 0; j < querryrw[i].size(); j++) {
				printf("Range Querry by thread %d for %d to %d = %d\n",i, querryrw[i][j].start_key, querryrw[i][j].end_key, querryrw[i][j].node->key);
			}
		}

		printf("\nInorder Tree\n");
		print_tree(g_rw_root);
		free_tree(g_rw_root);
		unsigned long long elapsed_ns;
		elapsed_ns = (end1.tv_sec-start.tv_sec)*1000000000 + (end1.tv_nsec-start.tv_nsec);
		printf("Elapsed for rw (ns): %llu\n",elapsed_ns);
		double elapsed_s = ((double)elapsed_ns)/1000000000.0;
		printf("Elapsed for rw(s): %lf\n",elapsed_s);
	}
	
	printf("\n");

	return 0;
}
