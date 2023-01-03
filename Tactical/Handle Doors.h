#ifndef _DOORS_H
#define _DOORS_H

#define HANDLE_DOOR_OPEN				1
#define HANDLE_DOOR_EXAMINE			2
#define HANDLE_DOOR_LOCKPICK		3
#define HANDLE_DOOR_FORCE				4
#define HANDLE_DOOR_LOCK				5
#define HANDLE_DOOR_UNLOCK			6
#define HANDLE_DOOR_EXPLODE			7
#define HANDLE_DOOR_UNTRAP			8
#define HANDLE_DOOR_CROWBAR			9

extern BOOLEAN gfSetPerceivedDoorState;


BOOLEAN HandleOpenableStruct( SOLDIERTYPE *pSoldier, INT32 sGridNo, STRUCTURE *pStructure );

void InteractWithOpenableStruct( SOLDIERTYPE *pSoldier, STRUCTURE *pStructure, UINT8 ubDirection, BOOLEAN fDoor );

void InteractWithClosedDoor( SOLDIERTYPE *pSoldier, UINT8 ubHandleCode );

void SetDoorString( INT32 sGridNo );

void HandleDoorChangeFromGridNo( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fNoAnimations );



#endif