#include "fine_lock_bst.h"
#include <stdbool.h>

vector <range> querry[2];

bst_node *g_root;
pthread_mutex_t bst_lock;

void BSTtoArray(bst_node *root, int A[])
{
    static int pos = 0;
    if(root == NULL) return;

    BSTtoArray(root->left, A);
    A[pos++] = root->key;
    BSTtoArray(root->right, A);

}

bool test_put_function()
{
	int i = 0;
	for (i = 0; i < 50; i++) {
		put_node(NULL, i, (2*i), 0);
	}

	int *A = new int[50];
    BSTtoArray(g_root, A);

    for (i = 0; i < 50; i++) {
    	assert(i == A[i]);
	}
	free(A);
	return true;
}

bool test_get_function()
{
	int i = 0;
	bst_node *node;

	for (i = 0; i < 50; i++) {
		node = get_node(NULL, i);
		if (node == NULL)
			return false;
		assert(node->value == (2*i));
	}
	return true;
}

bool test_range_function()
{
	range_querry(g_root, 0, 49, 0);
	int i = 0;
	for (i = 0; i < 50; i++) {
    	assert(i == querry[0][i].node->key);
	}

	return true;
}

int main()
{
	if (test_put_function() == true) {
		printf("Put function test passed\n");
	}

	if (test_get_function() == true) {
		printf("Get function test passed\n");
	}

	if (test_range_function() == true) {
		printf("Range Querry function test passed\n");
	}

	return 0;
}