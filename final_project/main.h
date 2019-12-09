#define MAX_THREAD (10)

vector <range> querry[2];

struct thread_info
{
	int task_no;
	int task_len;
	int task_key;
	char filename[64];
};

bst_node *g_root;
pthread_mutex_t bst_lock;