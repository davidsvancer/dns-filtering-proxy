// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file linked_list.c
 * @author David Švancer
 *
 * @brief Implementation of linked list and its methods. 
 */


#include "../include/linked_list.h"

int Add_To_List(Node ** list, char * element) {

    Node * new_elem = malloc(sizeof(Node));
    if (new_elem == NULL) {

        Log_Info("ERROR: Failed to allocate memory");
        return FUNCTION_ERROR;
    } 
    new_elem->hostname = malloc(strlen(element) + 1);
    if (new_elem->hostname == NULL) {

        Log_Info("ERROR: Failed to allocate memory");
        free(new_elem);
        return FUNCTION_ERROR;
    }

    strcpy(new_elem->hostname, element);
    new_elem->next = NULL;
    if(*list == NULL) {
        
        *list = new_elem;
    } else {

        Node * current = *list;
        while(current->next != NULL) {

            current = current->next;
        } 

        current->next = new_elem;
    }
    return FUNCTION_SUCCESS;
}

void List_Dispose(Node **list)
{
    if (list == NULL || *list == NULL) return;

    Node *current = *list;
    while (current != NULL) {

        Node *next = current->next;
        free(current->hostname);
        free(current);
        current = next;
    }
    *list = NULL; 
}


void List_Print_All_Elements(Node * list) {

    Node * tmp = list;
    printf("Printing list of elements\n");
    while(tmp != NULL) {

        printf("%s\n", tmp->hostname);
        tmp = tmp->next;
    }
}
