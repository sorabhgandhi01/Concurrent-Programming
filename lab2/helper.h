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

    char *lock;			//lock name

    int f_size;         //Size of the file

	int thread;			//Number of threads

	bool barrier;		//0 for sense and 1 for pthread

	bool algos;			// 0 for merge and 1 for bucket sort

    bool print_on_console; //Indicator to print output

    bool is_barrier_set;	//Barrier set indicator

    bool is_lock_set;		//Lock set indicator
};


struct arg_count_handler{

    char name[16];      //Authors name

    char *ofile;        //Output file name

    char *lock;			//lock name

	int thread;			//Number of threads

	int iteration;		//Number of iteration

	bool barrier;		//0 for sense and 1 for pthread

    bool print_on_console; //Indicator to print output

    bool is_barrier_set;	//Barrier set indicator

    bool is_lock_set;		//Lock set indicator
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
arg_count_parser
-----------------------------------------------------------------------------------------------------
*   @\brief This function parses the runtime command line arguments and populates the content in 
*           the argument handler structure.
*
*   @\param argc            [INPUT]     Count of total number of arguments
*
*           argv            [INPUT]     2d char array containing all arguments
*
*           arg_handler_t   [OUTPUT]    Pointer to the arg_count_handler structure
*
*   @\return    On Sucess it returns 0, if the input arg contains '--name' then it returns 1
*               On Failure it returns -1;
*
*           
*/
int arg_count_parser(int argc, char **argv, struct arg_count_handler *arg_handler_t);

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


int insert_count_to_file(struct arg_count_handler arg_handler_t, int count);


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
