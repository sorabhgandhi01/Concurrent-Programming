
#include "fine_lock_bst.h"
#include "helper.h"

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
		//printf("Invoking range query result for %d to %d\n", start_key, end_key);
		range_querry(g_root, start_key, end_key, th_info->task_no);
	}

	fclose(fp);	
}

//./tree -i test.txt -s search.txt -r range.txt -t 10 --lock=rw_lock
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
	pthread_mutex_init(&bst_lock, NULL);
	pthread_t th[arg.thread];
	int i = 0;
	
	int length = (arg.total_insert_keys / (arg.thread - 4));
	int m = 0;

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
	int j = 0;

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

	printf("\n\n Range querry result:\n");
	for (i = 0; i < 2; i++) {
		for (j = 0; j < querry[i].size(); j++) {
			printf("Range Querry by thread %d for %d to %d = %d\n",i, querry[i][j].start_key, querry[i][j].end_key, querry[i][j].node->key);
		}
	}

	printf("\nInorder Tree\n");
	print_tree(g_root);
	free_tree(g_root);
	printf("\n");

	return 0;
}
