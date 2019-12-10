#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *sfp;
	int i = 0;

	sfp = fopen("range_1000_highcontention.txt", "w+");

	if (sfp == NULL) {
		printf("Failed to create a file\n");
		exit(0);
	}

	for (i = 0; i < 1000; i++)
	{
		fprintf(sfp, "%d %d\n", 0, 999);
	}

	fclose(sfp);

	return 0;
}
