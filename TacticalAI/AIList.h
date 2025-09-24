#include "Overhead Types.h"

typedef struct AILIST
{
	SoldierID		ubID;
	INT8				bPriority;
	UINT8			ubUnused;
	struct AILIST *pNext;
} AILIST;

#define MAX_AI_PRIORITY 100

extern void ClearAIList( void );
extern AILIST * CreateNewAIListEntry( UINT16 ubNewEntry, SoldierID ubID, INT8 bAlertStatus );
extern BOOLEAN InsertIntoAIList( SoldierID ubID, INT8 bAlertStatus );
extern SoldierID RemoveFirstAIListEntry( void );
extern BOOLEAN BuildAIListForTeam( INT8 bTeam );
extern BOOLEAN MoveToFrontOfAIList( SoldierID ubID );
