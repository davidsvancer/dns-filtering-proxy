// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file utils.h
 * @author David Švancer
 *
 * @brief Header file for utils methods.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "dns.h"
#include "linked_list.h"
#include "dynamic_array.h"

typedef struct Thread_Arguments Thread_Arguments;
typedef struct Header Header;
typedef struct Question Question;
typedef struct Parameters Parameters;
typedef struct Socket Socket;

/**
 * @brief Function for DNS name decompression.
 * @param[in] packet The packet array to read from.
 * @param[in] decompressed_name Buffer where the decompressed domain name will be stored.
 * @param[in] index The starting index in the packet for decompression.
 * @return Returns the index, where should we continue reading from or FUNCTION_ERROR on failure.
 */
int DNS_Name_Decompression(unsigned char packet[], char * decompressed_name, int index);

/**
 * @brief Function to check if the given byte is a pointer.
 * @param[in] byte The byte to check.
 * @return Returns true if the byte is a pointer, otherwise false.
 */
bool DNS_Is_Pointer(unsigned char byte);

/**
 * @brief Function to check if the given byte is a label.
 * @param[in] byte The byte to check.
 * @return Returns true if the byte is a label, otherwise false.
 */
bool DNS_Is_Label(unsigned char byte);

/**
 * @brief Function to get the offset (was pointer).
 * @param[in] char_1 The first byte of the pointer.
 * @param[in] char_2 The second byte of the pointer.
 * @return Returns offset value.
 */
int DNS_Pointer_Offset(unsigned char char_1, unsigned char char_2);

/**
 * @brief Function to get the lenght (was label).
 * @param[in] char_l The byte representing the label length.
 * @return Returns the length of the label.
 */
int DNS_Label_Length(unsigned char char_l);

/**
 * @brief Function to read domain names from a file and store them in a linked list.
 *        Ignore lines starting with '#' and empty lines.
 * @param[in] file_name File name.
 * @param[in] list Linked list where the domains will be stored.
 * @return Returns FUNCTION_SUCCESS on success, FUNCTION_ERROR on failure.
 */
int Read_File(char * file_name, Node ** list);

/**
 * @brief Function to check if a domain is valid. (Is not in blocked domains)
 * @param[in] domain_list The linked list of blocked domains.
 * @param[in] domain The domain name to validate.
 * @return Returns true if the domain is valid (not blocked), otherwise false.
 */
bool Is_Valid_Domain(Node * domain_list, char * domain);

/**
 * @brief Function to free memory and exit thread.
 * @param[in] thread_args thread_argument pointer to be freed.
 * @param[in] header header pointer to be freed.
 * @param[in] question question pointer to be freed.
 */
void Clean_Memory_And_Exit_Thread(Thread_Arguments * thread_args, Header * header, Question * question);


/**
 * @brief Function to free memory
 * @param[in] client_socket Pointer to client socket to be freed.
 * @param[in] server_socket Pointer to server socket to be freed.
 * @param[in] param Pointer to parameter to be freed.
 * @param[in] list Pointer to linked list to be disposed.
 */
void Clean_Memory(Parameters * param, Node ** list, Socket * client_socket, Socket * server_socket);

#endif 


