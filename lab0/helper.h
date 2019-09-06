/**
 * @\file   helper.h
 * @\author Sorabh Gandhi
 * @\brief  This file contains the declaration all the application specific helper functions
 * @\date   09/05/2019
 *
 */

#ifndef __HELPER_H__
#define __HELPER_H__


struct arg_handler{

    char name[16];      //Authors name

    char *ifile;        //Input file name

    char *ofile;        //Output file name

    int f_size;         //Size of the file

    bool print_on_console; //Indicator to print output
};


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
int arg_parser(int argc, char **argv, struct arg_handler *arg_handler_t);


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
int insert_elements_to_array(struct arg_handler arg_handler_t, int list[]);


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
int insert_elements_to_file(struct arg_handler arg_handler_t, int list[]);


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
void print_to_console (struct arg_handler arg_handler_t, int list[]);

#endif
