// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file arg_parse.c
 * @author David Švancer
 *
 * @brief Implementation of argument parsing.
 */

#include "../include/arg_parse.h"

int Parse_Arguments(int argc, char **argv, Parameters * param) {

    param->file = NULL;
    param->server = NULL;
    param->verbose = false;
    param->port = -1;

    if ((!strcmp(argv[1], "-h") || (!strcmp(argv[1], "-help"))) && argc == 2)
    {
        printf("Usage:\n\ndns -s server [-p port] -f filter_file [-v]\n\n" 
                "server - IP adrress/hostname\n"
                "port - port number\n"
                "filter_file - file with not desired hostnames\n"
                "-v - verbose, prints log\n"
                "-h/-help - prints help\n");
            
        return FUNCTION_PRINT_HELP;
    }
    
    for (int i = 1; i < argc; i++) {

        if (!strcmp(argv[i], "-s")) {
        
            if ((i + 1) >= argc || param->server != NULL) {

                fprintf(stderr, "%s", "Wrong program arguments, try [-h|-help] for usage help!\n");
                return FUNCTION_ERROR;
            }
            if ((param->server = malloc(strlen(argv[i+1]) + 1)) == NULL) {

                fprintf(stderr, "%s", "Failed to allocate memory!\n");
                return FUNCTION_ERROR;
            }
            strcpy(param->server, argv[i+1]);
            i++;

        } else if (!strcmp(argv[i], "-p")) {

            if ((i + 1) >= argc || param->port != -1) {

                fprintf(stderr, "%s", "Wrong program arguments, try [-h|-help] for usage help!\n");
                return FUNCTION_ERROR;
            }
            if ((param->port = atoi(argv[i+1])) <= 0) {

                fprintf(stderr, "%s", "Wrong port number\n!");
                return FUNCTION_ERROR;
            }
            i++;

        } else if (!strcmp(argv[i], "-f")) {

            if ((i + 1) >= argc || param->file != NULL) {

                fprintf(stderr, "%s", "Wrong program arguments, try [-h|-help] for usage help!\n");
                return FUNCTION_ERROR;
            }
            if ((param->file = malloc(strlen(argv[i+1]) + 1)) == NULL) {

                fprintf(stderr, "%s", "Failed to allocate memory!\n");
                return FUNCTION_ERROR;
            }
            strcpy(param->file, argv[i+1]);
            i++;

        } else if (!strcmp(argv[i], "-v")) {

            if(param->verbose) {

                fprintf(stderr, "%s", "Wrong program arguments, try [-h|-help] for usage help!\n");
                return FUNCTION_ERROR;
            }
            param->verbose = true;

        } else {

            fprintf(stderr, "%s", "Wrong program arguments, try [-h|-help] for usage help!\n");
        }
    }

    if(param->port == -1) param->port = 53; // Default

    return FUNCTION_SUCCESS;
}
