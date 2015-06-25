#include <stdio.h>

typedef struct LIS_stList LIS_tpList;

LIS_tpList *LIS_create();
void LIS_addLast(LIS_tpList *pList, void *val);
int LIS_goNext(LIS_tpList *pList);
int LIS_goPrevious(LIS_tpList *pList);
void *LIS_getCurrentValue(LIS_tpList *pList);
void LIS_reset(LIS_tpList *pList);
int LIS_getCount(LIS_tpList *pList);
