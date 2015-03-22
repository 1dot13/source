#ifndef		_SAVELOADMAP__H_
#define		_SAVELOADMAP__H_

#include "WorldDef.h"
#include "Exit Grids.h"

//Used for the ubType in the MODIFY_MAP struct	
enum
{
	SLM_NONE,

	//Adding a map graphic
	SLM_LAND,
	SLM_OBJECT,
	SLM_STRUCT,
	SLM_SHADOW,
	SLM_MERC,										//Should never be used
	SLM_ROOF,
	SLM_ONROOF,
	SLM_TOPMOST,								//Should never be used

	// For Removing
	SLM_REMOVE_LAND,
	SLM_REMOVE_OBJECT,
	SLM_REMOVE_STRUCT,
	SLM_REMOVE_SHADOW,
	SLM_REMOVE_MERC,										//Should never be used
	SLM_REMOVE_ROOF,
	SLM_REMOVE_ONROOF,
	SLM_REMOVE_TOPMOST,								//Should never be used

	//Smell, or Blood is used
	SLM_BLOOD_SMELL,

	// Damage a particular struct
	SLM_DAMAGED_STRUCT,

	//Exit Grids
	SLM_EXIT_GRIDS,

	// State of Openable structs
	SLM_OPENABLE_STRUCT,

	// Modify window graphic & structure 
	SLM_WINDOW_HIT,
#ifdef JA2UB	
	SLM_REMOVE_EXIT_GRID,
#endif
	// sevenfm
	SLM_MINE_PRESENT,
	SLM_REMOVE_MINE_PRESENT,	// silversurfer: we need this to get rid of the mine flag otherwise any tile that ever had a mine on it will forever be flagged with MAPELEMENT_PLAYER_MINE_PRESENT
};

typedef struct//dnl ch86 250214
{
	INT32 usGridNo;				// The gridno the graphic will be applied to
	UINT16 usImageType;			// graphic index
	UINT16 usSubImageIndex;		// ExitGrid low WORD of usGridno is stored here
	UINT8 ubType;				// the layer it will be applied to
	UINT8 ubExtra;				// Misc. variable used to strore arbritary values
	UINT16 usHiExitGridNo;		// ExitGrid.usGridno is store in usSubImageIndex which is not enough for big maps so high WORD goes here just to preserve compatibility
} MODIFY_MAP;

// Call this function, to set whether the map changes will be added to the	map temp file
void	ApplyMapChangesToMapTempFile( BOOLEAN fAddToMap );

BOOLEAN SaveModifiedMapStructToMapTempFile( MODIFY_MAP *pMap, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );


//Applies a change TO THE MAP TEMP file
void AddStructToMapTempFile( INT32 iMapIndex, UINT16 usIndex );


//Applies a change TO THE MAP from the temp file
void AddStructFromMapTempFileToMap( INT32 iMapIndex, UINT16 usIndex );


void AddObjectToMapTempFile( INT32 uiMapIndex, UINT16 usIndex );


BOOLEAN LoadAllMapChangesFromMapTempFileAndApplyThem( );


void RemoveStructFromMapTempFile( INT32 uiMapIndex, UINT16 usIndex );

// Flugente: permanently remove other tiles
void RemoveRoofFromMapTempFile( INT32 uiMapIndex, UINT16 usIndex );
void RemoveOnRoofFromMapTempFile( INT32 uiMapIndex, UINT16 usIndex );

void AddRemoveObjectToMapTempFile( INT32 uiMapIndex, UINT16 usIndex );

void SaveBloodSmellAndRevealedStatesFromMapToTempFile();

// sevenfm
void SaveMineFlagFromMapToTempFile();
void RemoveMineFlagFromMapTempFile( INT32 usGridNo);

BOOLEAN SaveRevealedStatusArrayToRevealedTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

BOOLEAN LoadRevealedStatusArrayFromRevealedTempFile();


void AddRemoveObjectToUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  );
void RemoveStructFromUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  );
void AddObjectToUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  );
void AddStructToUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  );

//Adds the exit grid to
void AddExitGridToMapTempFile( INT32 usGridNo, EXITGRID *pExitGrid, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ );


//This function removes a struct with the same MapIndex and graphic index from the given sectors temp file
BOOLEAN RemoveGraphicFromTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ );


void SetOpenableStructStatusFromMapTempFile( INT32 uiMapIndex, BOOLEAN fOpened );
void AddOpenableStructStatusToMapTempFile( INT32 uiMapIndex, BOOLEAN fOpened );

void AddWindowHitToMapTempFile( INT32 uiMapIndex );

BOOLEAN ChangeStatusOfOpenableStructInUnloadedSector( UINT16 usSectorX, UINT16 usSectorY, INT8 bSectorZ, INT32 usGridNo, BOOLEAN fChangeToOpen );

#ifdef JA2UB
//ja25 ub
void AddRemoveExitGridToUnloadedMapTempFile( UINT32 usGridNo, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ );
#endif

#endif
