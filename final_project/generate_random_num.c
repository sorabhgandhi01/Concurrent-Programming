#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

int main(int argc, char **argv)
{
	if (argc != 4) {
		printf("Invalid Argument\n");
		exit(0);
	}

	char *filename = argv[1];
	int count = atoi(argv[2]);
	int max_limit = atoi(argv[3]);
	int i = 0;
	
	srand(time(NULL));

	FILE *fp;

	fp = fopen(filename, "w+");

	if (fp == NULL) {
		printf("Failed to create a file\n");
		exit(0);
	}

	for (i = 0; i < count; i++)
	{
		fprintf(fp, "%d %d\n", i, i);
	}

	fclose(fp);

	return 0;
}
