#include <stdio.h>
#include "list.h"
#include <stdlib.h>

struct stNodeList {
    void *value;
    int size;
    struct stNodeList *next;
};

nodeList *createNode(void *value){
	nodeList * node = NULL;
	node = malloc(sizeof(nodeList));
	if (node == NULL) {
	    return NULL;
	}

	node->value = value;
	node->size = 1;
	node->next = NULL;
	return node;
}

void add(nodeList * headList, void *val) {
    nodeList * current = headList;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(nodeList));
    current->next->value = val;
    current->size++;
    current->next->next = NULL;
}

void *removeLast(nodeList * headList) {
    void *nodeVal = NULL;
	nodeList * current = headList;

    if (headList->next == NULL) {
        nodeVal = headList->value;
        headList->size--;
        headList->next = NULL;
        free(headList);
        return nodeVal;
    }

    while (current->next != NULL) {
        current = current->next;
    }

   	headList->size--;
    nodeVal = current->value;
    free(current);

    return nodeVal;
}

void *getValue(nodeList **headList, int index){
	nodeList *current = (*headList);
	int count = 0;
	if (index == 0){
		return current->value;
	}
	else{
		for (count = 0; count < index; count++){
			current = current->next;
		}
		return current->value;
	}
}

int getSize(nodeList *headList){
	return headList->size;
}