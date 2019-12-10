#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <iostream>
#include <bits/stdc++.h> 

using namespace std; 

typedef struct noderw 
{
	int key;
	int value;
	struct noderw *right;
	struct noderw *left;
	pthread_rwlock_t lock;
}bst_noderw;

typedef struct Testrw 
{ 
   int start_key;
   int end_key;
   bst_noderw *node;
}rangerw; 

void put_node_rw(bst_noderw *root, int key, int value, int thread_num);
bst_noderw *get_node_rw(bst_noderw *root, int key);
void range_querry_rw(bst_noderw *root, int start_key, int end_key, int tid);
void get_nodes_inrange_rw(bst_noderw *root, int start_key, int end_key, int tid);
void print_tree(bst_noderw *root);
void free_tree(bst_noderw *root);