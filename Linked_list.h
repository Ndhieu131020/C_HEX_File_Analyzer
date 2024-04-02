#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Linked_list.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/


struct t_node{
    hexRecordTypedef hexRecord;
    struct t_node *next;
};

typedef struct{
    struct t_node* head;
}t_collection;

/*******************************************************************************
 * API
 ******************************************************************************/
#endif /* _LINKED_LIST_H_ */
