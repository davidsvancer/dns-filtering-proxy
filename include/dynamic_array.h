// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dynamic_array.h
 * @author David Švancer
 *
 * @brief Header file for dynamic array implementation and its methods.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"


typedef struct Dynamic_String{
    char * string_array;
    int capacity;
    int string_length;
} Dynamic_String;

/**
 * @brief Function for initialization of a dynamic string array.
 * @param[in] element Pointer to the dynamic string.
 * @param[in] initial_capacity The initial capacity of the string array.
 * @return Returns FUNCTION_SUCCESS on success, FUNCTION_ERROR on failure.
 */
int Array_Init(Dynamic_String * element, int initial_capacity);

/**
 * @brief Function to reallocate memory for a dynamic string array when capacity is exceeded.
 * @param[in] array Pointer to the dynamic string.
 * @return Returns FUNCTION_SUCCESS on success, FUNCTION_ERROR on failure.
 */
int Array_Realloc(Dynamic_String * array);

/**
 * @brief Function to add a single character to a dynamic string array.
 * @param[in] array Pointer to the dynamic string.
 * @param[in] element The character to be add.
 * @return Returns FUNCTION_SUCCESS on success, FUNCTION_ERROR on failure.
 */
int Array_AddChar(Dynamic_String * array, int element);

/**
 * @brief Function to terminate a dynamic string array with a null character.
 * @param[in] array Pointer to the dynamic string.
 * @return Returns FUNCTION_SUCCESS on success, FUNCTION_ERROR on failure.
 */
int Array_Terminate(Dynamic_String * array);

/**
 * @brief Function to free the memory used by a dynamic string.
 * @param[in] array Pointer to the dynamic string.
 */
void Array_Free(Dynamic_String * array);

/**
 * @brief Function to remove the last character from a dynamic string array.
 * @param[in] array Pointer to the dynamic string.
 */
void Array_Remove_Last_Element(Dynamic_String * array);

#endif
