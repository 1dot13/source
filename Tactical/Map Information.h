#ifndef __MAP_INFORMATION_H
#define __MAP_INFORMATION_H

#include "types.h"

extern FLOAT gdMajorMapVersion;
extern UINT8 gubMinorMapVersion;

enum //for use with MAPCREATE_STRUCT.ubEditorSmoothingType
{
	SMOOTHING_NORMAL,
	SMOOTHING_BASEMENT,
	SMOOTHING_CAVES
};

//dnl ch42 250909
// WANNE - BMP: DONE!
typedef struct
{
	//These are the mandatory entry points for a map.	If any of the values are -1, then that means that
	//the point has been specifically not used and that the map is not traversable to or from an adjacent 
	//sector in that direction.	The >0 value points must be validated before saving the map.	This is 
	//done by simply checking if those points are sittable by mercs, and that you can plot a path from 
	//these points to each other.	These values can only be set by the editor : mapinfo tab
	INT16 sNorthGridNo;
	INT16 sEastGridNo;
	INT16 sSouthGridNo;
	INT16 sWestGridNo;
	//This contains the number of individuals in the map.
	//Individuals include NPCs, enemy placements, creatures, civilians, rebels, and animals.
	UINT8 ubNumIndividuals;
	UINT8 ubMapVersion;
	UINT8 ubRestrictedScrollID;
	UINT8 ubEditorSmoothingType;	//normal, basement, or caves
	INT16 sCenterGridNo;
	INT16 sIsolatedGridNo;
	INT8 bPadding[83];	//I'm sure lots of map info will be added
}_OLD_MAPCREATE_STRUCT; //99 bytes

class MAPCREATE_STRUCT
{
public:
	INT32 sNorthGridNo;
	INT32 sEastGridNo;
	INT32 sSouthGridNo;
	INT32 sWestGridNo;
	INT32 sCenterGridNo;
	INT32 sIsolatedGridNo;
	UINT16 ubNumIndividuals;
	UINT8 ubMapVersion;
	UINT8 ubRestrictedScrollID;
	UINT8 ubEditorSmoothingType;
public:
	MAPCREATE_STRUCT& operator=(const _OLD_MAPCREATE_STRUCT& src);
	BOOLEAN Load(INT8** hBuffer, FLOAT dMajorMapVersion);
	BOOLEAN Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
};

extern MAPCREATE_STRUCT gMapInformation;

void LoadMapInformation(INT8** hBuffer, FLOAT dMajorMapVersion);
void SaveMapInformation(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);

void ValidateAndUpdateMapVersionIfNecessary();
BOOLEAN ValidateEntryPointGridNo( INT32 *sGridNo );

extern BOOLEAN gfWorldLoaded;

#endif
