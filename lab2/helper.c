/**
 * @\file   helper.c
 * @\author Sorabh Gandhi
 * @\brief  This file contains the definition all the application specific helper functions
 * @\date   09/05/2019
 *
 */

/* System Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <getopt.h>

/* Own Headers */
#include "helper.h"


/**
-----------------------------------------------------------------------------------------------------
is_textfile
-----------------------------------------------------------------------------------------------------
*   @\brief This function checks if the input file is a text file with .txt extention or not
*
*   @\param argv    Input filename
*
*   @\return    On Success it returns 'True', else it returns 'False' 
*
*/
bool is_textfile (char *argv)
{
    int str_len = strlen(argv);
    char fname[str_len];
    strcpy(fname, argv);

    char *token = strtok(fname, ".txt");
    if ((strcmp(token, argv)) == 0) {
        return false;
    }

    return true;
}


/**
-----------------------------------------------------------------------------------------------------
arg_parser
-----------------------------------------------------------------------------------------------------
*   @\brief This function parses the runtime command line arguments and populates the content in 
*           the argument handler structure.
*
*   @\param argc            [INPUT]     Count of total number of arguments
*
*           argv            [INPUT]     2d char array containing all arguments
*
*           arg_handler_t   [OUTPUT]    Pointer to the arg_handler structure
*
*   @\return    On Sucess it returns 0, if the input arg contains '--name' then it returns 1
*               On Failure it returns -1;
*
*           
*/
int arg_parser(int argc, char **argv, struct arg_handler *arg_handler_t)
{

	int c;
	char algo[256];
	char barrier[256];

    static struct option opt[] =
    {
        {"name", no_argument, 0, 'n'},
        {"object", required_argument, 0, 'o'},
        {"thread", optional_argument, 0, 't'},
        {"alg", required_argument, 0, 'a'},
        {"bar", required_argument, 0, 'b'},
        {"lock", required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };

	if ((strcmp(argv[1], "--name")) != 0) {
		arg_handler_t->ifile = argv[1];
    	printf("The input filename is %s\n", arg_handler_t->ifile);
	}

	arg_handler_t->print_on_console = false;
	arg_handler_t->is_barrier_set = false;
    arg_handler_t->is_lock_set = false;

    while ((c = getopt_long(argc, argv, "n:o:t:a:", opt, 0)) != -1)
    {

        switch (c)
        {
            case 'n':
				printf("Name = %s\n", arg_handler_t->name);
                return 1;

            case 'o':
				arg_handler_t->ofile = optarg;
				arg_handler_t->print_on_console = true;
                printf("The output filename is %s\n", arg_handler_t->ofile);
                break;

            case 't':
				arg_handler_t->thread = atoi(optarg);
				printf("The number of thread is %d\n", arg_handler_t->thread);
                break;

            case 'a':
				strcpy(algo, optarg);
				if (strcmp(algo, "bucket") == 0) {
					arg_handler_t->algos = 1;
				} else {
					arg_handler_t->algos = 0;
				}
				printf("Algorithm used is %d\n", arg_handler_t->algos);
                break;

            case 'l':
                arg_handler_t->lock = optarg;
                arg_handler_t->is_lock_set = true;
                printf("The lock used is %s\n", arg_handler_t->lock);
                break;

            case 'b':
                strcpy(barrier, optarg);
                if (strcmp(barrier, "sense") == 0) {
                    arg_handler_t->barrier = 0;
                } else {
                    arg_handler_t->barrier = 1;
                }
                arg_handler_t->is_barrier_set = true;
                printf("Barrier used is %d\n", arg_handler_t->barrier);
                break;

            default:
                printf("No args provided\n");
                return -1;
        }

    }

	char *buffer = NULL;
	size_t temp = 0;
	int element = 0;
	FILE *ptr;
            
	ptr = fopen(arg_handler_t->ifile, "r");

	if (ptr == NULL) {
		printf("Error in opening the input file\n");
		return -1;
	}

    while (getline(&buffer, &temp, ptr) != -1)
    {
		element++;
	}

	arg_handler_t->f_size = element;
	printf("Total number of elements = %d\n", element);

	free(buffer);
	fclose(ptr);

    return 0;
}


/**
-----------------------------------------------------------------------------------------------------
arg_count_parser
-----------------------------------------------------------------------------------------------------
*   @\brief This function parses the runtime command line arguments and populates the content in 
*           the argument handler structure.
*
*   @\param argc            [INPUT]     Count of total number of arguments
*
*           argv            [INPUT]     2d char array containing all arguments
*
*           arg_handler_t   [OUTPUT]    Pointer to the arg_handler structure
*
*   @\return    On Sucess it returns 0, if the input arg contains '--name' then it returns 1
*               On Failure it returns -1;
*
*           
*/
int arg_count_parser(int argc, char **argv, struct arg_count_handler *arg_handler_t)
{

    int c;
    char barrier[256];

    static struct option opt[] =
    {
        {"name", no_argument, 0, 'n'},
        {"object", required_argument, 0, 'o'},
        {"thread", optional_argument, 0, 't'},
        {"iteration", required_argument, 0, 'i'},
        {"bar", required_argument, 0, 'b'},
        {"lock", required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };

    arg_handler_t->print_on_console = false;
    arg_handler_t->is_barrier_set = false;
    arg_handler_t->is_lock_set = false;

    while ((c = getopt_long(argc, argv, "n:o:t:i:", opt, 0)) != -1)
    {

        switch (c)
        {
            case 'n':
                printf("Name = %s\n", arg_handler_t->name);
                return 1;

            case 'o':
                arg_handler_t->ofile = optarg;
                arg_handler_t->print_on_console = true;
                printf("The output filename is %s\n", arg_handler_t->ofile);
                break;

            case 't':
                arg_handler_t->thread = atoi(optarg);
                printf("The number of thread is %d\n", arg_handler_t->thread);
                break;

            case 'i':
                arg_handler_t->iteration = atoi(optarg);
                printf("The number of iteration is %d\n", arg_handler_t->iteration);
                break;

            case 'l':
                arg_handler_t->lock = optarg;
                arg_handler_t->is_lock_set = true;
                printf("The lock used is %s\n", arg_handler_t->lock);
                break;

            case 'b':
                strcpy(barrier, optarg);
                if (strcmp(barrier, "sense") == 0) {
                    arg_handler_t->barrier = 0;
                } else {
                    arg_handler_t->barrier = 1;
                }
                arg_handler_t->is_barrier_set = true;
                printf("Barrier used is %d\n", arg_handler_t->barrier);
                break;

            default:
                printf("No args provided\n");
                return -1;
        }

    }

    return 0;
}

/**
-----------------------------------------------------------------------------------------------------
insert_elements_to_array
-----------------------------------------------------------------------------------------------------
*   @\brief This function reads all the elements from the input file and copies to the array
*
*   @\param arg_handler_t   [input]     Pointer to the arg_handler structure
*           
*           list            [output]    Integer array that will be populated with list of elements
*
*   @\return    On Sucess it returns 0;
*               On Failure it returns -1.
*
*
*/
int insert_elements_to_array(struct arg_handler arg_handler_t, int list[])
{
    int i = 0;
    FILE *fp;

    //printf("The input filename is %s and size = %d\n", arg_handler_t.ifile, arg_handler_t.f_size);
    fp = fopen(arg_handler_t.ifile, "r");

    if (fp == NULL) {
        printf("Error in opening the file\n");
        return -1;
    }

    for (i = 0; i < (arg_handler_t.f_size); i++)
    {
        fscanf(fp, "%d", &list[i]);
    }

    fclose(fp);

    return 0;
}


/**
-----------------------------------------------------------------------------------------------------
insert_elements_to_file
-----------------------------------------------------------------------------------------------------
*   @\brief This function reads all the elements from the input array and copies it to the output file
*
*   @\param arg_handler_t   [input]     Pointer to the arg_handler structure
*
*           list            [input]     Integer array with list of elements
*
*   @\return    On Sucess it returns 0;
*               On Failure it returns -1.
*
*
*/
int insert_elements_to_file(struct arg_handler arg_handler_t, int list[])
{
    int i = 0;
    FILE *fp;

    fp = fopen(arg_handler_t.ofile, "w+");

    if (fp == NULL) {
        printf("Error in opening the file\n");
        return -1;
    }

    for (i = 0; i < (arg_handler_t.f_size); i++)
    {
        fprintf(fp, "%d\n", list[i]);
    }

    fclose(fp);

    return 0;
}


int insert_count_to_file(struct arg_count_handler arg_handler_t, int count)
{
    FILE *fp;

    fp = fopen(arg_handler_t.ofile, "w+");

    if (fp == NULL) {
        printf("Error in opening the file\n");
        return -1;
    }

    fprintf(fp, "%d\n", count);

    fclose(fp);

    return 0;
}


/**
-----------------------------------------------------------------------------------------------------
print_to_console
-----------------------------------------------------------------------------------------------------
*   @\brief This function prints all the elements of the array to stdout
*
*   @\param list            [input]     Integer array with list of elements
*
*   @\return    void
*
*
*/

void print_to_console (struct arg_handler arg_handler_t, int list[])
{
    int j = 0;
    int elements = (arg_handler_t.f_size);
    
    printf("The sorted array is\n");

    for (j = 0; j < elements; j++)
    {
        printf("%d\n", list[j]);
    }
}
