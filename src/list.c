#include <stdio.h>
#include "list.h"
#include <stdlib.h>

struct stNodeList {
  struct stNode *pHead;
  struct stNode *pLast;
  struct stNode *pCurrent;
  int count;
};

typedef struct stNode {
  void *pValue;
  struct stNode *pNext;
  struct stNode *pPrevious;
} tpNode;

tpList *createList() {
	tpList *pList = (tpList*) malloc(sizeof(tpList));
  pList->pHead = NULL;
  pList->pLast = NULL;
  pList->pCurrent = NULL;
  pList->count = 0;
  return pList;
}

void addLast(tpList *pList, void *val) {
  tpNode *pOldLast = pList->pLast;

  tpNode *pNewNode = (tpNode*) malloc(sizeof(tpNode));
  pNewNode->pValue = val;
  pNewNode->pNext = NULL;
  pNewNode->pPrevious = pList->pLast;
  pList->pLast = pNewNode;

  if(pOldLast == NULL) {
    pList->pHead = pNewNode;
  } else {
    pOldLast->pNext = pNewNode;
  }
  pList->count++;
}

int goNext(tpList *pList){
  if(pList->pCurrent == NULL) {
    pList->pCurrent = pList->pHead;
  } else {
    pList->pCurrent = pList->pCurrent->pNext;
  }
  if(pList->pCurrent == NULL) {
    return 0;
  } else {
    return 1;
  }
}

int goPrevious(tpList *pList){
  if(pList->pCurrent == NULL) {
    pList->pCurrent = pList->pLast;
  } else {
    pList->pCurrent = pList->pCurrent->pPrevious;
  }
  if(pList->pCurrent == NULL) {
    return 0;
  } else {
    return 1;
  }
}

void *getCurrentValue(tpList *pList){
  return pList->pCurrent->pValue;
}

void resetList(tpList *pList) {
  pList->pCurrent = NULL;
}

int getListSize(tpList *pList) {
  return pList->count;
}
