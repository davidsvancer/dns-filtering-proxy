// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dynamic_array.c
 * @author David Švancer
 *
 * @brief Implementation of dynamic array.
 */

#include "../include/dynamic_array.h"

int Array_Init(Dynamic_String * element, int initial_capacity) {

    element->string_array = malloc(initial_capacity * sizeof(char));
    if (element->string_array == NULL) {
        
        Log_Info("ERROR: Dynamic array memory allocation failed");
        return FUNCTION_ERROR;
    }
    element->capacity = initial_capacity;
    element->string_length = 0;
    return FUNCTION_SUCCESS;
}

int Array_Realloc(Dynamic_String * array) {

    array->string_array = realloc(array->string_array, array->capacity * 2 * sizeof(char));
    if (array->string_array == NULL) {

        Log_Info("ERROR: Dynamic array memory reallocation failed");
        return FUNCTION_ERROR;
    }
    array->capacity = array->capacity * 2;
    return FUNCTION_SUCCESS;
}

int Array_AddChar(Dynamic_String * array, int element) {  

    int result;
    if (array->string_length >= array->capacity) {

        result = Array_Realloc(array);
        if (result == FUNCTION_ERROR) {

            return FUNCTION_ERROR;
        }
    }
    array->string_array[array->string_length] = element;
    array->string_length++;
    return FUNCTION_SUCCESS;
}

void Array_Remove_Last_Element(Dynamic_String * array) {

    if(array->string_length > 0) {

        array->string_array[array->string_length-1] = '\0';
        array->string_length--;
    }
}

int Array_Terminate(Dynamic_String * array) {

    int result;
    if (array->string_length >= array->capacity) {

        result = Array_Realloc(array);
        if (result == FUNCTION_ERROR) {
            return FUNCTION_ERROR;
        }
    }
    array->string_array[array->string_length] = '\0';
    return FUNCTION_SUCCESS;
}

void Array_Free(Dynamic_String * array) {

    free(array->string_array);
    array->string_array = NULL;
    array->capacity = 0;
    array->string_length = 0;
}
