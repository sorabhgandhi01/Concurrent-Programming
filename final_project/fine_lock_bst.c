#include "fine_lock_bst.h"

extern bst_node *g_root;
extern pthread_mutex_t bst_lock; //rw_lock
extern vector <range> querry[2];

//create a new bst node
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

//update the value of the given node
void update_value(bst_node *node, int value)
{
	node->value = value;
}

//insert the key-value pair in the tree
void put_node(bst_node *root, int key, int value, int thread_num)
{
	if (root == NULL)
	{
		pthread_mutex_lock(&bst_lock);  //rwlock_wrlock
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

//obtain the node from the tree based on the key
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
		return NULL;
	}

	//return NULL;
}

void range_querry(bst_node *root, int start_key, int end_key, int tid)
{
	if (root == NULL) {
		return;
	}

	bst_node *start_node = get_node(NULL, start_key);
	bst_node *end_node = get_node(NULL, end_key);

	if (start_node == NULL) {
		printf("Invalid Query. Node with key %d is not present in the tree\n", start_key);
	} else if (end_node == NULL) {
		printf("Invalid Query. Node with key %d is not present in the tree\n", end_key);
	} else {
		pthread_mutex_lock(&bst_lock);
		pthread_mutex_lock(&root->lock);
		pthread_mutex_unlock(&bst_lock);
		//printf("Range query result for %d to %d\n", start_key, end_key);
		get_nodes_inrange(root, start_key, end_key, tid);
	}
}

//fetch all the nodes in the given range
void get_nodes_inrange(bst_node *root, int start_key, int end_key, int tid)
{
	if (root == NULL)
	{
		pthread_mutex_lock(&bst_lock);
		if (g_root == NULL)
		{
			//printf("Searched failed for node with key %d\n", key);
			pthread_mutex_unlock(&bst_lock);
			return;
		}

		pthread_mutex_lock(&g_root->lock);
		root = g_root;
		pthread_mutex_unlock(&bst_lock);
	}

	if (start_key < root->key)
	{
		if (root->left != NULL)
		{
			pthread_mutex_lock(&root->left->lock);
			pthread_mutex_unlock(&root->lock);
			get_nodes_inrange(root->left, start_key, end_key, tid);
			pthread_mutex_lock(&root->lock);
		} 
		
	}

	if ((start_key <= root->key) && (end_key >= root->key))
	{
		//printf("-->%d  %d to %d\n",tid, root->key, root->value);
		querry[tid].push_back({start_key, end_key, root});
	}
	
	if (end_key > root->key)
    {
        if (root->right != NULL)
        {
        	pthread_mutex_lock(&root->right->lock);
        	pthread_mutex_unlock(&root->lock);
        	get_nodes_inrange(root->right, start_key, end_key, tid);
        	pthread_mutex_lock(&root->lock);
        } 
    }

    pthread_mutex_unlock(&root->lock);

}

void print_tree(bst_node *root)
{
	if (root == NULL)
		return;

	print_tree(root->left);
	printf("%d %d\n", root->key, root->value);
	print_tree(root->right);

}


void free_tree(bst_node *root)
{
	if (root != NULL) {
        free_tree(root->right);
        free_tree(root->left);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
 }