#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/stat.h>

#include "helper.h"

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


int arg_parser(int argc, char **argv, struct arg_handler *arg_handler_t)
{
    if ((argc < 1) || (argc > 4)) {
        printf("Usage -> ./sort [--name] inputfile.txt -o outputfile.txt\n");
        return -1;
    }

    if ((strcmp(argv[1], "--name")) == 0) {

        printf("Name = %s\n", arg_handler_t->name);
        
        return 1;

    } else {
        
        if(is_textfile(argv[1]) == false) {
            printf("Enter a valid input filename with '.txt' as the extension\n");
            return -1;
        } else {
            struct stat st;
            arg_handler_t->ifile = argv[1];
            stat(arg_handler_t->ifile, &st);
            arg_handler_t->f_size = st.st_size;

            printf("The input filename is %s and size = %d\n", arg_handler_t->ifile, arg_handler_t->f_size);
        }
    }


    if ((argc == 3) || (argc == 4)) {

        if ((strcmp(argv[2], "-o") == 0) || (strcmp(argv[3], "-o") == 0) || (strcmp(argv[2], "-O") == 0) || (strcmp(argv[3], "-O") == 0)) {

            if (is_textfile(argv[3]) == true) {
                
                arg_handler_t->ofile = argv[3];
                arg_handler_t->print_on_console = false;
                printf("The output filename is %s\n", arg_handler_t->ofile);
            } else {
                printf("Enter a valid output filename with '.txt' as the extension\n");
                return -1;
            }

        } else {
            printf("Enter a vaid flag - '-o' to specify the output flag\n");
            return -1;
        }

    } else {

        arg_handler_t->print_on_console = true;
    }

    return 0;
}


int insert_elements_to_array(struct arg_handler arg_handler_t, int list[])
{
    int i = 0;
    FILE *fp;

    printf("The input filename is %s and size = %d\n", arg_handler_t.ifile, arg_handler_t.f_size);
    fp = fopen(arg_handler_t.ifile, "r");

    if (fp == NULL) {
        printf("Error in opening the file\n");
        return -1;
    }

    for (i = 0; i < (arg_handler_t.f_size / 2); i++)
    {
        fscanf(fp, "%d", &list[i]);
    }

    fclose(fp);

    return 0;
}


int insert_elements_to_file(struct arg_handler arg_handler_t, int list[])
{
    int i = 0;
    FILE *fp;

    fp = fopen(arg_handler_t.ofile, "w+");

    if (fp == NULL) {
        printf("Error in opening the file\n");
        return -1;
    }

    for (i = 0; i < (arg_handler_t.f_size / 2); i++)
    {
        fprintf(fp, "%d\n", list[i]);
    }

    fclose(fp);

    return 0;
}
