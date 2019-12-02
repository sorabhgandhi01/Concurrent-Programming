#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_THREAD (10)

struct thread_info
{
	int task_no;
	int task_len;
	int task_key;
	char filename[64];
};

typedef struct node 
{
	int key;
	int value;
	struct node *right;
	struct node *left;
	pthread_mutex_t lock;
}bst_node;

bst_node *g_root = NULL;
pthread_mutex_t bst_lock;

void print_nodes_inhirarchy(bst_node *root, int end_key);

bst_node *create_node(int key, int value)
{
	bst_node *node = (bst_node *)malloc(sizeof(bst_node));

	node->key = key;
	node->value = value;
	node->left = NULL;
	node->right = NULL;

	pthread_mutex_init(&node->lock, NULL);

	return node;
}

void update_value(bst_node *node, int value)
{
	node->value = value;
}

void put_node(bst_node *root, int key, int value, int thread_num)
{
	if (root == NULL)
	{
		pthread_mutex_lock(&bst_lock);
		if (g_root == NULL)
		{
			g_root = create_node(key, value);
			printf("Root node initialized with key %d and value %d\n", g_root->key, g_root->value);
			pthread_mutex_unlock(&bst_lock);
			return;
		}

		pthread_mutex_lock(&g_root->lock);
		root = g_root;
		pthread_mutex_unlock(&bst_lock);
	}

	if (key < root->key)
	{
		if (root->left == NULL)
		{
			root->left = create_node(key, value);
			pthread_mutex_unlock(&root->lock);
		} else {
			pthread_mutex_lock(&root->left->lock);
			pthread_mutex_unlock(&root->lock);
			put_node(root->left, key, value, thread_num);
		}
	}
	else if (key > root->key)
    {
        if (root->right == NULL)
        {
            root->right = create_node(key, value);
            pthread_mutex_unlock(&root->lock);
        } else {
            pthread_mutex_lock(&root->right->lock);
            pthread_mutex_unlock(&root->lock);
            put_node(root->right, key, value, thread_num);
        }
    } else {
		if (key == root->key)
			update_value(root, value);
		pthread_mutex_unlock(&root->lock);
	}
}

bst_node *get_node(bst_node *root, int key)
{
	if (root == NULL)
	{
		pthread_mutex_lock(&bst_lock);
		if (g_root == NULL)
		{
			//printf("Searched failed for node with key %d\n", key);
			pthread_mutex_unlock(&bst_lock);
			return NULL;
		}

		pthread_mutex_lock(&g_root->lock);
		root = g_root;
		pthread_mutex_unlock(&bst_lock);
	}

	if (key == root->key)
	{
		pthread_mutex_unlock(&root->lock);
		return root;
	}
	else if (key < root->key)
	{
		if (root->left == NULL)
		{
			//printf("Searched failed for node with key %d\n", key);
			pthread_mutex_unlock(&root->lock);
			return NULL;
		} else {
			pthread_mutex_lock(&root->left->lock);
			pthread_mutex_unlock(&root->lock);
			get_node(root->left, key);
		}
	}
	else if (key > root->key)
    {
        if (root->right == NULL)
        {
            //printf("Searched failed for node with key %d\n", key);
            pthread_mutex_unlock(&root->lock);
            return NULL;
        } else {
            pthread_mutex_lock(&root->right->lock);
            pthread_mutex_unlock(&root->lock);
            get_node(root->right, key);
        }
    } else {
		pthread_mutex_unlock(&root->lock);
	}
}

void range_querry(bst_node *root, int start_key, int end_key)
{
	bst_node *node = get_node(root, start_key);

	if (node == NULL) {
		printf("Invalid Query. Node with key %d is not present in the tree\n", start_key);
	} else {
		pthread_mutex_lock(&bst_lock);
		pthread_mutex_lock(&node->lock);
		pthread_mutex_unlock(&bst_lock);
		printf("Range Query result for key %d to %d:\n", start_key, end_key);
		print_nodes_inhirarchy(node, end_key);
	}
}

void print_nodes_inhirarchy(bst_node *root, int end_key)
{
	if (root == NULL)
	{
		printf("Invalid Query. Node with key %d is not present in the tree\n", end_key);
		pthread_mutex_unlock(&root->lock);
		return;
	}

	if (end_key == root->key)
	{
		printf("Node with key %d contains value %d\n", root->key, root->value);
		pthread_mutex_unlock(&root->lock);
		return;
	}
	else if (end_key < root->key)
	{
		if (root->left == NULL)
		{
			printf("Invalid Query. Node with key %d is not present in the tree\n", end_key);
			pthread_mutex_unlock(&root->lock);
			return;
		} else {
			pthread_mutex_lock(&root->left->lock);
			printf("Node with key %d contains value %d\n", root->key, root->value);
			pthread_mutex_unlock(&root->lock);
			print_nodes_inhirarchy(root->left, end_key);
		}
	}
	else if (end_key > root->key)
    {
        if (root->right == NULL)
        {
            printf("Invalid Query. Node with key %d is not present in the tree\n", end_key);
            pthread_mutex_unlock(&root->lock);
            return;
        } else {
            pthread_mutex_lock(&root->right->lock);
            printf("Node with key %d contains value %d\n", root->key, root->value);
            pthread_mutex_unlock(&root->lock);
            print_nodes_inhirarchy(root->right, end_key);
        }
    } else {
		pthread_mutex_unlock(&root->lock);
	}
}

void print_tree(bst_node *root)
{
	if (root == NULL)
		return;

	print_tree(root->left);
	printf("%d %d\n", root->key, root->value);
	print_tree(root->right);

}


void *thread_handler(void *arg)
{
	struct thread_info *th_info = (struct thread_info *)arg;

	FILE *fp = fopen("test.txt", "r");
	
	if (fp == NULL){
		printf("Error in opening the file\n");
		exit(0);
	}

	int key = 0, value = 0, i = 0;

	while ((i < th_info->task_key) && (!feof (fp))) {
		fscanf(fp, "%d %d\n", &key, &value);
		i++;
	}

	for (i = 0; i < th_info->task_len; i++)
	{
		fscanf(fp, "%d %d\n", &key, &value);
		put_node(NULL, key, value, th_info->task_no);
	}
		
}


void *thread_handler3(void *arg)
{
    int i = 0, j = 80;
	bst_node *node;

    for (i = 0; i <= 7; i++)
    {
        node = get_node(NULL, j);
		if (node == NULL) {
			printf("Node with key %d not found\n", j);
		} else {
			printf("Node with key %d contains value %d\n", j, node->value);
		}	
		node = NULL;
        j = j - 10;
    }
    
}

void *thread_handler4(void *arg)
{
    int i = 0, j = 80, z = 100;
	bst_node *node;

    for (i = 0; i <= 7; i++)
    {
        range_querry(NULL, z, j);
        j = j - 10;
        z = z + 10;
    }
    
}

int main()
{
	struct thread_info th_info[MAX_THREAD];
	pthread_mutex_init(&bst_lock, NULL);
	pthread_t th[MAX_THREAD];
	int i = 0;
	int count = 10000;
	int length = (count / MAX_THREAD);
	int m = 0;

	for (i = 0; i < MAX_THREAD; i++) {

		m = i*length;

		if (i == (MAX_THREAD - 1)) {
			th_info[i].task_no = i;
			th_info[i].task_len = (count - m);
			th_info[i].task_key = m;
		} else {
			th_info[i].task_no = i;
			th_info[i].task_len = length;
			th_info[i].task_key = m;
		}

		if (pthread_create(&th[i], NULL, thread_handler, (void *)&th_info[i]) != 0)
		{
			printf("Error on creating the thread\n");
			exit(0);
		} else {
			printf("Create new thread %d with length %d and start key %d\n", th_info[i].task_no, th_info[i].task_len, th_info[i].task_key);
		}
	}

	for (i = 0; i < MAX_THREAD; i++) {
		pthread_join(th[i], NULL);
	}

	print_tree(g_root);
	printf("\n");
}
