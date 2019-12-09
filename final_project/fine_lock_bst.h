#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <bits/stdc++.h> 

typedef struct node 
{
	int key;
	int value;
	struct node *right;
	struct node *left;
	pthread_mutex_t lock;
}bst_node;

typedef struct Test 
{ 
   int start_key;
   int end_key;
   bst_node *node;
}range; 

void put_node(bst_node *root, int key, int value, int thread_num);
bst_node *get_node(bst_node *root, int key);
void range_querry(bst_node *root, int start_key, int end_key);
void print_nodes_inhirarchy(bst_node *root, int end_key);
void print_tree(bst_node *root);