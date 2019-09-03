
#ifndef __HELPER_H__
#define __HELPER_H__


struct arg_handler{

    char name[16];      //Authors name

    char *ifile;        //Input file name

    char *ofile;        //Output file name

    int f_size;         //Size of the file

    bool print_on_console; //Indicator to print output
};

int arg_parser(int argc, char **argv, struct arg_handler *arg_handler_t);
int insert_elements_to_array(struct arg_handler arg_handler_t, int list[]);
int insert_elements_to_file(struct arg_handler arg_handler_t, int list[]);


#endif
