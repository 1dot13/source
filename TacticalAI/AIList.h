#include "Types.h"

typedef struct AILIST
{
	UINT16							ubID;
	INT8							bPriority;
	UINT8							ubUnused;
	struct AILIST *		pNext;
} AILIST;

#define MAX_AI_PRIORITY 100

extern void ClearAIList( void );
extern AILIST * CreateNewAIListEntry( UINT16 ubNewEntry, UINT16 ubID, INT8 bAlertStatus );
extern BOOLEAN InsertIntoAIList( UINT16 ubID, INT8 bAlertStatus );
extern UINT16 RemoveFirstAIListEntry( void );
extern BOOLEAN BuildAIListForTeam( INT8 bTeam );
extern BOOLEAN MoveToFrontOfAIList( UINT16 ubID );
