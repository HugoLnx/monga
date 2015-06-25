#include <stdio.h>

typedef struct stNodeList tpList;

tpList *createList();
void addLast(tpList *pList, void *val);
int goNext(tpList *pList);
int goPrevious(tpList *pList);
void *getCurrentValue(tpList *pList);
void resetList(tpList *pList);
int getListSize(tpList *pList);
