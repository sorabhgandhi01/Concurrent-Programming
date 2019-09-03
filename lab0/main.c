#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "helper.h"
#include "mysort.h"

int main (int argc, char **argv)
{
    struct arg_handler arg_handler_t = {"Sorabh Gandhi"};
    int j = 0;

    int ret_status = arg_parser(argc, argv, &arg_handler_t);

    /*If the arguments are incorrect*/
    if (ret_status == -1) {
        exit(-1);
    }

    /*If the '--name' flag is used*/
    if (ret_status == 1) {
        exit(0);
    }

    int array_size = ((arg_handler_t.f_size) / 2);  //Calculate the number of elements in array
    int list[array_size];   //Initialize an array with set number of elements

    /*Read integers from the file and store in local array*/
    if (insert_elements_to_array(arg_handler_t, list) != 0) {
        exit(-1);
    }
 
    //sort the input array
    mergesort(list, 0, (array_size - 1));

    if (!(arg_handler_t.print_on_console))
    {
        insert_elements_to_file(arg_handler_t, list);
    } else {

    for (j = 0; j < array_size; j++)
    {
        printf("%d\n", list[j]);
    }
    }


    return 0;
}
