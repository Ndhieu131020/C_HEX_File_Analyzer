#include "Linked_list.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
void pushNodeHead(t_collection *c, hexRecordTypedef *info)
{
    struct t_node *newNode =  (struct t_node *)malloc(sizeof(struct t_node));

    newNode->hexRecord.recordLength = info->recordLength;
    newNode->hexRecord.address[0] = info->address[0];

    newNode->next = c->head;
    c->head = newNode;
}