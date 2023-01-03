#ifndef __Encyclopedia_Data_new_H
#define __Encyclopedia_Data_new_H

#include "Item Types.h"

#ifdef ENCYCLOPEDIA_WORKS

#define ENC_NUM_DATAPAGES MAXITEMS

extern void GameInitEncyclopediaData_NEW(  );
extern void HandleEncyclopediaData_NEW(  );
extern void RenderEncyclopediaData_NEW(  );
extern BOOLEAN EnterEncyclopediaData_NEW(  );
extern BOOLEAN ExitEncyclopediaData_NEW(  );

#endif

#endif
