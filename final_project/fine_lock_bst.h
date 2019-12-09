#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <iostream>
#include <bits/stdc++.h> 

using namespace std; 

typedef struct node 
{
	int key;
	int value;
	struct node *right;
	struct node *left;
	pthread_mutex_t lock; //pthread_rwlock_t
}bst_node;

typedef struct Test 
{ 
   int start_key;
   int end_key;
   bst_node *node;
}range; 

void put_node(bst_node *root, int key, int value, int thread_num);
bst_node *get_node(bst_node *root, int key);
void range_querry(bst_node *root, int start_key, int end_key, int tid);
void get_nodes_inrange(bst_node *root, int start_key, int end_key, int tid);
void print_tree(bst_node *root);
void free_tree(bst_node *root);