#ifndef __AIMSORT_H_
#define __AIMSORT_H_

extern UINT8			gubCurrentSortMode;
extern UINT8			gubCurrentListMode;



#define		AIM_ASCEND									13
#define		AIM_DESCEND									14


void GameInitAimSort();
BOOLEAN EnterAimSort();
void ExitAimSort();
void HandleAimSort();
void RenderAimSort();

#endif



 


