#include <stdio.h>

typedef struct stNodeList nodeList;

nodeList *createNode(void *value);
void add(nodeList * headList, void *val);
void *removeLast(nodeList * headList);
void *getValue(nodeList **headList, int index);
int getSize(nodeList *headList);