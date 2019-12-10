#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <iostream> 
#include <algorithm> 
  
using namespace std; 

int main(int argc, char **argv)
{
	if (argc != 8) {
		printf("Invalid Argument\n");
		printf("Usage: ./file [insert filename] [Number of elements to insert] [Max limit] [search filename] [Number of elements to search] [range filename] [Number of range querry]\n");
		exit(0);
	}

	char *ifilename = argv[1];
	int icount = atoi(argv[2]);
	int max_limit = atoi(argv[3]);
	char *sfilename = argv[4];
	int scount = atoi(argv[5]);
	char *rfilename = argv[6];
	int rcount = atoi(argv[7]);

	int i = 0;

	FILE *ifp, *sfp, *rfp;
	
	srand(time(NULL));

	int temp[icount];

	ifp = fopen(ifilename, "w+");

	if (ifp == NULL) {
		printf("Failed to create a file\n");
		exit(0);
	}

	for (i = 0; i < icount; i++)
	{
		temp[i] = (int)(random() % max_limit);
		fprintf(ifp, "%d %d\n", temp[i], i);
	}

	fclose(ifp);

	
	sfp = fopen(sfilename, "w+");

	if (sfp == NULL) {
		printf("Failed to create a file\n");
		exit(0);
	}

	for (i = 0; i < scount; i++)
	{
		fprintf(sfp, "%d\n", temp[i]);
	}

	fclose(sfp);

	sort(temp, (temp + icount));

	rfp = fopen(rfilename, "w+");

	if (rfp == NULL) {
		printf("Failed to create a file\n");
		exit(0);
	}

	for (i = 0; i < rcount-1; i++)
	{
		fprintf(sfp, "%d %d\n", temp[i], temp[i + 1]);
	}

	fclose(sfp);

	return 0;
}
