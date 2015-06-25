#include <stdio.h>
#include "list.h"
#include <stdlib.h>

struct LIS_stList {
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

LIS_tpList *LIS_create() {
	LIS_tpList *pList = (LIS_tpList*) malloc(sizeof(LIS_tpList));
  pList->pHead = NULL;
  pList->pLast = NULL;
  pList->pCurrent = NULL;
  pList->count = 0;
  return pList;
}

void LIS_addLast(LIS_tpList *pList, void *val) {
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

int LIS_goNext(LIS_tpList *pList){
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

int LIS_goPrevious(LIS_tpList *pList){
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

void *LIS_getCurrentValue(LIS_tpList *pList){
  return pList->pCurrent->pValue;
}

void LIS_reset(LIS_tpList *pList) {
  pList->pCurrent = NULL;
}

int LIS_getCount(LIS_tpList *pList) {
  return pList->count;
}
