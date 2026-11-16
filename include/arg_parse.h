// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file arg_parse.h
 * @author David Švancer
 *
 * @brief Header file for argument parsing implementation.
 */
#ifndef ARG_PARSE_H
#define ARG_PARSE_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include <stdlib.h>

typedef struct Parameters{
    int port;
    char * server;              // IP/hostname
    char * file;          
    bool verbose;               
} Parameters;

/**
 * @brief Function to parse input arguments.
 * @param[in] argc Argument count.
 * @param[in] argv Double pointer for input arguments
 * @param[in,out] param Pointer where will be parsed values stored in.
 * @return Returns FUNCTION_PRINT_HELP if argument was -h|-help, FUNCTION_SUCCESS if parse was successfull, FUNCTION_ERROR if error occured.
 */
int Parse_Arguments(int argc, char **argv, Parameters * param); 

#endif
