// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file linked_list.h
 * @author David Švancer
 *
 * @brief Header file for linked list and its methods. 
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

typedef struct Node{
    char * hostname;
    struct Node * next;
} Node;

/**
 * @brief Function for adding new element to the list.
 * @param[in] list Linked list where will be adding new element.
 * @param[in] element Element, which we want to add
 * @return Returns FUNCTION_SUCCES if everything was okay, otherwisw FUNCTION_ERROR if adding or allocating memory failed
 */
int Add_To_List(Node ** list, char * element);

/**
 * @brief Function for linked list allocated dynamic memory free
 * @param[in] list Linked list to be disposed
 */
void List_Dispose(Node ** list);

/**
 * @brief Function prints all linked list elements, each on new line
 * @param[in] list Linked list, which element will be printed
 */
void List_Print_All_Elements(Node * list);

#endif
