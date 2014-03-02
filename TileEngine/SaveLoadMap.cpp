#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "Types.h"
	#include "SaveLoadMap.h"
	#include "OverHead.h"
	#include "FileMan.h"
	#include "Tactical Save.h"
	#include "Debug.h"
	#include "WorldMan.h"
	#include "StrategicMap.h"
	#include "Campaign Types.h"
	#include "RenderWorld.h"
	#include "Render Fun.h"
	#include "fov.h"
	#include "WorldDef.h"
	#include "Exit Grids.h"
	#include "Message.h"
	#include "GameSettings.h"
	#include "Smell.h"
#endif

#include <vfs/Core/vfs.h>
//SB: make size of gpRevealedMap dependable from variable tactical map dimensions
#define			NUM_REVEALED_BYTES			(WORLD_MAX/8)

extern BOOLEAN gfLoadingExitGrids;

BOOLEAN			gfApplyChangesToTempFile = FALSE;

//	There are 3200 bytes, and each bit represents the revelaed status.
//	3200 bytes * 8 bits = 25600 map elements
UINT8				*gpRevealedMap;

#ifdef JA2UB
//Ja25
void AddRemoveExitGridToUnloadedMapTempFile( UINT32 usGridNo, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ );
#endif

void RemoveSavedStructFromMap( INT32 uiMapIndex, UINT16 usIndex );
void AddObjectFromMapTempFileToMap( INT32 uiMapIndex, UINT16 usIndex );
void AddBloodOrSmellFromMapTempFileToMap( MODIFY_MAP *pMap );
// sevenfm
void AddMineFlagFromMapTempFileToMap( MODIFY_MAP *pMap );
void SetSectorsRevealedBit( UINT32	usMapIndex );
void SetMapRevealedStatus();
void DamageStructsFromMapTempFile( MODIFY_MAP * pMap );
BOOLEAN ModifyWindowStatus( INT32 uiMapIndex );
//ppp

struct ModifiedMapFile
{
	struct Key
	{
		UINT32  uiType;
		INT16   sMapX;
		INT16   sMapY;
		INT8    bMapZ;
	} key;
	CHAR8 szMapName[128];
	HWFILE hFileHandle;

	ModifiedMapFile(UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ)
	{
		this->key.uiType   = uiType;
		this->key.sMapX    = sMapX;
		this->key.sMapY    = sMapY;
		this->key.bMapZ    = bMapZ;
		this->szMapName[0] = 0;
		this->hFileHandle  = NULL;
	}

	ModifiedMapFile(UINT32 uiType, STR pMapName, INT16 sMapX, INT16 sMapY, INT8 bMapZ)
	{
		this->key.uiType  = uiType;
		this->key.sMapX   = sMapX;
		this->key.sMapY   = sMapY;
		this->key.bMapZ   = bMapZ;
		strncpy(this->szMapName, pMapName, _countof(this->szMapName));
		this->szMapName[_countof(this->szMapName) - 1] = 0;
		this->hFileHandle = NULL;
	}

	~ModifiedMapFile()
	{
		if (this->hFileHandle != NULL)
		{
			FileClose(this->hFileHandle);
			this->hFileHandle = NULL;
		}
	}

	BOOLEAN IsOpen() const
	{
		return (this->hFileHandle == NULL) ? FALSE : TRUE;
	}

	BOOLEAN Open()
	{
		if ( hFileHandle == NULL && szMapName[0] != 0)
		{
			// we assume that we 'own' the file
			hFileHandle = FileOpen( szMapName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );

			//Move to the end of the file
			FileSeek( hFileHandle, 0, FILE_SEEK_FROM_END );
		}
		return IsOpen();
	}

	void Delete()
	{
		Close();

		if (szMapName[0] != 0)
		{
			FileDelete(szMapName);
		}
	}

	BOOLEAN Write( PTR pDest, UINT32 uiBytesToWrite )
	{
		UINT32 uiNumBytesWritten = 0;
		if (!IsOpen()) return FALSE;

		if ( !FileWrite( this->hFileHandle, pDest, uiBytesToWrite, &uiNumBytesWritten ) )
		{
			Close();
			return FALSE;
		}
		if (uiBytesToWrite != uiNumBytesWritten)
		{
			Close();
			return FALSE;
		}
		return TRUE;
	}

	void Close()
	{
		if (this->hFileHandle != NULL)
		{
			FileClose(this->hFileHandle);
			this->hFileHandle = NULL;
		}
	}
};

struct ltMMF
{
	bool compare(const ModifiedMapFile::Key& s1, const ModifiedMapFile::Key& s2) const
	{
		int diff = s1.uiType - s2.uiType;
		if (diff == 0)
		{
			diff = s1.sMapX - s2.sMapX;
			if (diff == 0)
			{
				diff = s1.sMapY - s2.sMapY;
				if (diff == 0)
				{
					diff = s1.bMapZ - s2.bMapZ;
				}
			}
		}
		return diff < 0;
	}
	bool operator()(const ModifiedMapFile::Key& s1, const ModifiedMapFile::Key& s2) const { return compare(s1, s2); }
	bool operator()(const ModifiedMapFile::Key* s1, const ModifiedMapFile::Key* s2) const { return compare(*s1, *s2); }
	bool operator()(const ModifiedMapFile::Key& s1, const ModifiedMapFile::Key* s2) const { return compare(s1, *s2); }
	bool operator()(const ModifiedMapFile::Key* s1, const ModifiedMapFile::Key& s2) const { return compare(*s1, s2); }
};

typedef std::map<ModifiedMapFile::Key, ModifiedMapFile, ltMMF> ModifiedMapFileSet;
ModifiedMapFileSet g_mapFileSet;
BOOLEAN g_useSaveCache;

void ClearTempFileSets()
{
	//for (ModifiedMapFileSet::iterator itr = g_mapFileSet.begin(), end = g_mapFileSet.end(); itr != end; ++itr )
	//{
	//}
	// destructor will close any open file handles
	g_mapFileSet.clear();
}

BOOLEAN EnableModifiedFileSetCache(BOOLEAN value)
{
	BOOLEAN previousValue = g_useSaveCache;
	if (g_useSaveCache != value)
	{
		ClearTempFileSets();
	}
	g_useSaveCache = value;
	return previousValue;
}

bool TryGetModifiedMapFile( UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ, ModifiedMapFile** ppMMF )
{
	if (ppMMF == NULL)
		return false;

	ModifiedMapFile key(uiType, sMapX, sMapY, bMapZ);
	ModifiedMapFileSet::iterator itr = g_mapFileSet.find(key.key);
	if (itr == g_mapFileSet.end())
	{
		*ppMMF = NULL;
		return false;
	}
	*ppMMF = &(itr->second);
	return true;
}

ModifiedMapFile& GetOrCreateModifiedMapFile(UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ)
{
	ModifiedMapFile* pResult = NULL;
	if ( TryGetModifiedMapFile(uiType, sMapX, sMapY, bMapZ, &pResult) )
		return *pResult;

	ModifiedMapFile key(uiType, sMapX, sMapY, bMapZ);
	ModifiedMapFileSet::iterator itr = g_mapFileSet.insert(g_mapFileSet.end(), std::make_pair(key.key, key));
	pResult = &(itr->second);
	
	GetMapTempFileName( pResult->key.uiType, pResult->szMapName, pResult->key.sMapX, pResult->key.sMapY, pResult->key.bMapZ );

	return *pResult;
}

void	ApplyMapChangesToMapTempFile( BOOLEAN fAddToMap )
{
	gfApplyChangesToTempFile = fAddToMap;
}


BOOLEAN SaveModifiedMapStructToMapTempFile( MODIFY_MAP *pMap, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	if (g_useSaveCache)
	{
		ModifiedMapFile& rMMF = GetOrCreateModifiedMapFile(SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, sSectorX, sSectorY, bSectorZ);
		if( !rMMF.Open() )
		{
			//Error opening map modification file
			return( FALSE );
		}

		if ( !rMMF.Write( pMap, sizeof( MODIFY_MAP ) ) )
		{
			return( FALSE );
		}

		SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS );

		return( TRUE );
	}
	else
	{
		CHAR8		zMapName[ 128 ];
		HWFILE	hFile;
		UINT32	uiNumBytesWritten;

		//Convert the current sector location into a file name
		//	GetMapFileName( sSectorX, sSectorY, bSectorZ, zTempName, FALSE );

		//add the 'm' for 'Modifed Map' to the front of the map name
		//	sprintf( zMapName, "%s\\m_%s", MAPS_DIR, zTempName);

		GetMapTempFileName( SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );

		//Open the file for writing, Create it if it doesnt exist
		hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
		if( hFile == 0 )
		{
			//Error opening map modification file
			return( FALSE );
		}

		//Move to the end of the file
		FileSeek( hFile, 0, FILE_SEEK_FROM_END );


		FileWrite( hFile, pMap, sizeof( MODIFY_MAP ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( MODIFY_MAP ) )
		{
			//Error Writing size of array to disk
			FileClose( hFile );
			return( FALSE );
		}

		FileClose( hFile );

		SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS );

		return( TRUE );
	}
}



BOOLEAN LoadAllMapChangesFromMapTempFileAndApplyThem( )
{
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;
	UINT32	uiNumBytesRead;
	UINT32	uiFileSize;
	UINT32	uiNumberOfElements;
	UINT32	uiNumberOfElementsSavedBackToFile = 0;	// added becuase if no files get saved back to disk, the flag needs to be erased
	UINT32	cnt;
	MODIFY_MAP *pMap;
	MODIFY_MAP *pTempArrayOfMaps=NULL;
	UINT16	usIndex;

	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'm' for 'Modifed Map' to the front of the map name
//	sprintf( zMapName, "%s\\m_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );


	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists, its no problem.
		return( TRUE );
	}

	//Open the file for reading
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		return( FALSE );
	}


	//Get the size of the file
	uiFileSize = FileGetSize( hFile );


	//Allocate memory for the buffer
	pTempArrayOfMaps = (MODIFY_MAP *) MemAlloc( uiFileSize );
	if( pTempArrayOfMaps == NULL )
	{
		Assert( 0 );
		return( TRUE );
	}

	//Read the map temp file into a buffer
	FileRead( hFile, pTempArrayOfMaps, uiFileSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiFileSize )
	{
		FileClose( hFile );
		return( FALSE );
	}

	//Close the file
	FileClose( hFile );

	//Delete the file
	FileDelete( zMapName );

	// Begin save cache
	EnableModifiedFileSetCache(TRUE);

	uiNumberOfElements = uiFileSize / sizeof( MODIFY_MAP );

	for( cnt=0; cnt< uiNumberOfElements; cnt++ )
	{
		pMap = &pTempArrayOfMaps[ cnt ];

		//Switch on the type that should either be added or removed from the map
		switch( pMap->ubType )
		{
			//If we are adding to the map
			case SLM_LAND:
				break;
			case SLM_OBJECT:
				GetTileIndexFromTypeSubIndex( pMap->usImageType, pMap->usSubImageIndex, &usIndex );

				AddObjectFromMapTempFileToMap( pMap->usGridNo, usIndex );

				// Save this struct back to the temp file
				SaveModifiedMapStructToMapTempFile( pMap, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

				//Since the element is being saved back to the temp file, increment the #
				uiNumberOfElementsSavedBackToFile++;

				break;
			case SLM_STRUCT:
				GetTileIndexFromTypeSubIndex( pMap->usImageType, pMap->usSubImageIndex, &usIndex );

				AddStructFromMapTempFileToMap( pMap->usGridNo, usIndex );

				// Save this struct back to the temp file
				SaveModifiedMapStructToMapTempFile( pMap, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

				//Since the element is being saved back to the temp file, increment the #
				uiNumberOfElementsSavedBackToFile++;
				break;
			case SLM_SHADOW:
				break;
			case SLM_MERC:
				break;
			case SLM_ROOF:
				break;
			case SLM_ONROOF:
				break;
			case SLM_TOPMOST:
				break;


			//Remove objects out of the world
			case SLM_REMOVE_LAND:
				break;
			case SLM_REMOVE_OBJECT:
				break;
			case SLM_REMOVE_STRUCT:

				// ATE: OK, dor doors, the usIndex can be varied, opened, closed, etc
				// we MUSTR delete ANY door type on this gridno
				// Since we can only have one door per gridno, we're safe to do so.....
				if ( pMap->usImageType >= FIRSTDOOR && pMap->usImageType <= FOURTHDOOR )
				{
					// Remove ANY door...
					RemoveAllStructsOfTypeRange( pMap->usGridNo, FIRSTDOOR, FOURTHDOOR );
				}
				else
				{
					GetTileIndexFromTypeSubIndex( pMap->usImageType, pMap->usSubImageIndex, &usIndex );
					RemoveSavedStructFromMap( pMap->usGridNo, usIndex );
				}

				// Save this struct back to the temp file
				SaveModifiedMapStructToMapTempFile( pMap, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

				//Since the element is being saved back to the temp file, increment the #
				uiNumberOfElementsSavedBackToFile++;
				break;
			case SLM_REMOVE_SHADOW:
				break;
			case SLM_REMOVE_MERC:
				break;
			case SLM_REMOVE_ROOF:
				break;
			case SLM_REMOVE_ONROOF:
				break;
			case SLM_REMOVE_TOPMOST:
				break;


			case SLM_BLOOD_SMELL:
				AddBloodOrSmellFromMapTempFileToMap( pMap );
				break;
// sevenfm
			case SLM_MINE_PRESENT:
				AddMineFlagFromMapTempFileToMap( pMap );
				break;
			case SLM_DAMAGED_STRUCT:
				DamageStructsFromMapTempFile( pMap );
				break;

			case SLM_EXIT_GRIDS:
				{
					EXITGRID ExitGrid;
					gfLoadingExitGrids = TRUE;
					ExitGrid.usGridNo = (UINT32)pMap->usHiExitGridNo << 16 | pMap->usSubImageIndex;//dnl ch86 260214 was ExitGrid.usGridNo = pMap->usSubImageIndex;
					ExitGrid.ubGotoSectorX = (UINT8) pMap->usImageType;
					ExitGrid.ubGotoSectorY = (UINT8) ( pMap->usImageType >> 8 ) ;
					ExitGrid.ubGotoSectorZ = pMap->ubExtra;

					AddExitGridToWorld( pMap->usGridNo, &ExitGrid );
					gfLoadingExitGrids = FALSE;

					// Save this struct back to the temp file
					SaveModifiedMapStructToMapTempFile( pMap, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

					//Since the element is being saved back to the temp file, increment the #
					uiNumberOfElementsSavedBackToFile++;
				}
				break;

			case SLM_OPENABLE_STRUCT:
				SetOpenableStructStatusFromMapTempFile( pMap->usGridNo, (BOOLEAN)pMap->usImageType );
				break;

			case SLM_WINDOW_HIT:
				if ( ModifyWindowStatus( pMap->usGridNo ) )
				{
					// Save this struct back to the temp file
					SaveModifiedMapStructToMapTempFile( pMap, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

					//Since the element is being saved back to the temp file, increment the #
					uiNumberOfElementsSavedBackToFile++;
				}
				break;
#ifdef JA2UB				
			case SLM_REMOVE_EXIT_GRID:
				//Remove the exit grid
				RemoveExitGridFromWorld( pMap->usGridNo );

				// Save this struct back to the temp file
				SaveModifiedMapStructToMapTempFile( pMap, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
				break;
#endif
			default:
				AssertMsg( 0, "ERROR!	Map Type not in switch when loading map changes from temp file");
				break;
		}

	}

	//if no elements are saved back to the file, remove the flag indicating that there is a temp file
	if( uiNumberOfElementsSavedBackToFile == 0 )
	{
		ReSetSectorFlag( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS );
	}
#ifndef USE_VFS
	FileClose( hFile );
#else
	// FileClose( hFile );
	// file already deleted. can't close it anymore (handle is invalid)
#endif

	//Free the memory used for the temp array
	MemFree( pTempArrayOfMaps );
	pTempArrayOfMaps = NULL;

	// Flush any open file sets
	EnableModifiedFileSetCache(FALSE);

	return( TRUE );
}








void AddStructToMapTempFile( INT32 uiMapIndex, UINT16 usIndex )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;


	if( !gfApplyChangesToTempFile )
		return;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
//	Map.usIndex		= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;

	Map.ubType		= SLM_STRUCT;

	SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
}


void AddStructFromMapTempFileToMap( INT32 uiMapIndex, UINT16 usIndex )
{
	AddStructToTailCommon( uiMapIndex, usIndex, TRUE );
}


void AddObjectToMapTempFile( INT32 uiMapIndex, UINT16 usIndex )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;

	if( !gfApplyChangesToTempFile )
		return;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
//	Map.usIndex		= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;

	Map.ubType		= SLM_OBJECT;

	SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
}


void AddObjectFromMapTempFileToMap( INT32 uiMapIndex, UINT16 usIndex )
{
	AddObjectToHead( uiMapIndex, usIndex );
}

void AddRemoveObjectToMapTempFile( INT32 uiMapIndex, UINT16 usIndex )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;

	if( !gfApplyChangesToTempFile )
		return;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
//	Map.usIndex		= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;

	Map.ubType		= SLM_REMOVE_OBJECT;

	SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
}


void RemoveStructFromMapTempFile( INT32 uiMapIndex, UINT16 usIndex )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;

	if( !gfApplyChangesToTempFile )
		return;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo	= uiMapIndex;
//	Map.usIndex			= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;

	Map.ubType			= SLM_REMOVE_STRUCT;

	SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

}


void RemoveSavedStructFromMap( INT32 uiMapIndex, UINT16 usIndex )
{
	RemoveStruct( uiMapIndex, usIndex );
}


// sevenfm
void SaveMineFlagFromMapToTempFile()
{
	MODIFY_MAP Map;
	INT32	cnt;	

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		if( gpWorldLevelData[cnt].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT )
		{	
			memset( &Map, 0, sizeof( MODIFY_MAP ) );
			Map.usGridNo	= cnt;
			Map.ubType			= SLM_MINE_PRESENT;
			SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		}
	}
}
void SaveBloodSmellAndRevealedStatesFromMapToTempFile()
{
	MODIFY_MAP Map;
	INT32	cnt;
	STRUCTURE * pStructure;


	gpRevealedMap = (UINT8 *) MemAlloc( NUM_REVEALED_BYTES );
	if( gpRevealedMap == NULL )
		AssertMsg( 0, "Failed allocating memory for the revealed map" );
	memset( gpRevealedMap, 0, NUM_REVEALED_BYTES );

	//Loop though all the map elements
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		//if there is either blood or a smell on the tile, save it
		if( gpWorldLevelData[cnt].ubBloodInfo || gpWorldLevelData[cnt].ubSmellInfo )
		{
			memset( &Map, 0, sizeof( MODIFY_MAP ) );


			// Save the BloodInfo in the bottom byte and the smell info in the upper byte
			Map.usGridNo	= cnt;
//			Map.usIndex			= gpWorldLevelData[cnt].ubBloodInfo | ( gpWorldLevelData[cnt].ubSmellInfo << 8 );
			Map.usImageType = gpWorldLevelData[cnt].ubBloodInfo;
			Map.usSubImageIndex = gpWorldLevelData[cnt].ubSmellInfo;


			Map.ubType			= SLM_BLOOD_SMELL;

			//Save the change to the map file
			SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		}


		//if the element has been revealed
		if( gpWorldLevelData[cnt].uiFlags & MAPELEMENT_REVEALED )
		{
			SetSectorsRevealedBit( cnt );
		}

		//if there is a structure that is damaged
		if( gpWorldLevelData[cnt].uiFlags & MAPELEMENT_STRUCTURE_DAMAGED )
		{
			STRUCTURE * pCurrent;

			pCurrent =	gpWorldLevelData[cnt].pStructureHead;

			pCurrent = FindStructure( cnt, STRUCTURE_BASE_TILE );

			//loop through all the structures and add all that are damaged
			while( pCurrent )
			{
				//if the structure has been damaged
				if( pCurrent->ubHitPoints < pCurrent->pDBStructureRef->pDBStructure->ubHitPoints )
				{
					UINT8	ubBitToSet = 0x80;
					UINT8	ubLevel=0;

					if( pCurrent->sCubeOffset != 0 )
						ubLevel |= ubBitToSet;

					memset( &Map, 0, sizeof( MODIFY_MAP ) );

					// Save the Damaged value
					Map.usGridNo	= cnt;
//					Map.usIndex			= StructureFlagToType( pCurrent->fFlags ) | ( pCurrent->ubHitPoints << 8 );
					Map.usImageType = StructureFlagToType( pCurrent->fFlags );
					Map.usSubImageIndex = pCurrent->ubHitPoints;


					Map.ubType			= SLM_DAMAGED_STRUCT;
					Map.ubExtra			= pCurrent->ubWallOrientation | ubLevel;

					//Save the change to the map file
					SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
				}

				pCurrent = FindNextStructure( pCurrent, STRUCTURE_BASE_TILE );
			}
		}

		pStructure = FindStructure( cnt, STRUCTURE_OPENABLE );

		//if this structure
		if( pStructure )
		{
			// if the current structure has an openable structure in it, and it is NOT a door
			if( !( pStructure->fFlags & STRUCTURE_ANYDOOR ) )
			{
				BOOLEAN			fStatusOnTheMap;

				fStatusOnTheMap = ( ( pStructure->fFlags & STRUCTURE_OPEN ) != 0 );

				AddOpenableStructStatusToMapTempFile( cnt, fStatusOnTheMap );
			}
		}
	}
}


// The BloodInfo is saved in the bottom byte and the smell info in the upper byte
void AddBloodOrSmellFromMapTempFileToMap( MODIFY_MAP *pMap )
{
	gpWorldLevelData[ pMap->usGridNo ].ubBloodInfo = (UINT8)pMap->usImageType;

	//if the blood and gore option IS set, add blood
	if( gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
	{
		// Update graphics for both levels...
		gpWorldLevelData[ pMap->usGridNo ].uiFlags |= MAPELEMENT_REEVALUATEBLOOD;
		UpdateBloodGraphics( pMap->usGridNo, 0 );
		gpWorldLevelData[ pMap->usGridNo ].uiFlags |= MAPELEMENT_REEVALUATEBLOOD;
		UpdateBloodGraphics( pMap->usGridNo, 1 );
	}

	gpWorldLevelData[ pMap->usGridNo ].ubSmellInfo = (UINT8)pMap->usSubImageIndex;
}

//sevenfm
void AddMineFlagFromMapTempFileToMap( MODIFY_MAP *pMap )
{
	gpWorldLevelData[ pMap->usGridNo ].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;
}


BOOLEAN SaveRevealedStatusArrayToRevealedTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;
	UINT32	uiNumBytesWritten;

	Assert( gpRevealedMap != NULL );

	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'v' for 'reVeiled Map' to the front of the map name
//	sprintf( zMapName, "%s\\v_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_REVEALED_STATUS_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );


	//Open the file for writing, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}


	//Write the revealed array to the Revealed temp file
	FileWrite( hFile, gpRevealedMap, NUM_REVEALED_BYTES, &uiNumBytesWritten );
	if( uiNumBytesWritten != NUM_REVEALED_BYTES )
	{
		//Error Writing size of array to disk
		FileClose( hFile );
		return( FALSE );
	}

	FileClose( hFile );

	SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_REVEALED_STATUS_TEMP_FILE_EXISTS );

	MemFree( gpRevealedMap );
	gpRevealedMap = NULL;

	return( TRUE );
}




BOOLEAN LoadRevealedStatusArrayFromRevealedTempFile()
{
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;
	UINT32	uiNumBytesRead;



	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'v' for 'reVeiled Map' to the front of the map name
//	sprintf( zMapName, "%s\\v_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_REVEALED_STATUS_TEMP_FILE_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists, its no problem.
		return( TRUE );
	}

	//Open the file for reading
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		return( FALSE );
	}

	//Allocate memory
	Assert( gpRevealedMap == NULL );
	gpRevealedMap = (UINT8 *) MemAlloc( NUM_REVEALED_BYTES );
	if( gpRevealedMap == NULL )
		AssertMsg( 0, "Failed allocating memory for the revealed map" );
	memset( gpRevealedMap, 0, NUM_REVEALED_BYTES );



	// Load the Reveal map array structure
	FileRead( hFile, gpRevealedMap, NUM_REVEALED_BYTES, &uiNumBytesRead );
	if( uiNumBytesRead != NUM_REVEALED_BYTES )
	{
		return( FALSE );
	}

	FileClose( hFile );


	//Loop through and set the bits in the map that are revealed
	SetMapRevealedStatus();

	MemFree( gpRevealedMap );
	gpRevealedMap = NULL;



	return( TRUE );
}

void SetSectorsRevealedBit( UINT32	usMapIndex )
{
	UINT16	usByteNumber;
	UINT8		ubBitNumber;

	usByteNumber = usMapIndex / 8;
	ubBitNumber  = usMapIndex % 8;

	gpRevealedMap[ usByteNumber ] |= 1 << ubBitNumber;
}



void SetMapRevealedStatus()
{
	UINT32	usByteCnt;
	UINT8		ubBitCnt;
	UINT32	usMapIndex;

	if( gpRevealedMap == NULL )
		AssertMsg( 0, "gpRevealedMap is NULL.	DF 1" );


	ClearSlantRoofs( );

	//Loop through all bytes in the array
	for( usByteCnt=0; usByteCnt< 3200; usByteCnt++)
	{
		//loop through all the bits in the byte
		for( ubBitCnt=0; ubBitCnt<8; ubBitCnt++)
		{
			usMapIndex = ( usByteCnt * 8 ) + ubBitCnt;

			if( gpRevealedMap[ usByteCnt ] & ( 1 << ubBitCnt ) )
			{
				gpWorldLevelData[ usMapIndex ].uiFlags |= MAPELEMENT_REVEALED;
				SetGridNoRevealedFlag( usMapIndex );
			}
			else
			{
				gpWorldLevelData[ usMapIndex ].uiFlags &= (~MAPELEMENT_REVEALED );
			}
		}
	}

	ExamineSlantRoofFOVSlots( );

}




void DamageStructsFromMapTempFile( MODIFY_MAP * pMap )
{
	STRUCTURE *pCurrent=NULL;
	INT8			bLevel;
	UINT8			ubWallOrientation;
	UINT8			ubBitToSet = 0x80;
	UINT8			ubType=0;


	//Find the base structure
	pCurrent = FindStructure( pMap->usGridNo, STRUCTURE_BASE_TILE );

	if( pCurrent == NULL )
		return;

	bLevel = pMap->ubExtra & ubBitToSet;
	ubWallOrientation = pMap->ubExtra & ~ubBitToSet;
	ubType = (UINT8) pMap->usImageType;


	//Check to see if the desired strucure node is in this tile
	pCurrent = FindStructureBySavedInfo( pMap->usGridNo, ubType, ubWallOrientation, bLevel );

	if( pCurrent != NULL )
	{
		//Assign the hitpoints
		pCurrent->ubHitPoints = (UINT8)( pMap->usSubImageIndex );

		gpWorldLevelData[ pCurrent->sGridNo ].uiFlags |= MAPELEMENT_STRUCTURE_DAMAGED;
	}
}


//////////////


void AddStructToUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
//	Map.usIndex		= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;


	Map.ubType		= SLM_STRUCT;

	SaveModifiedMapStructToMapTempFile( &Map, sSectorX, sSectorY, ubSectorZ );
}

void AddObjectToUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
//	Map.usIndex		= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;

	Map.ubType		= SLM_OBJECT;

	SaveModifiedMapStructToMapTempFile( &Map, sSectorX, sSectorY, ubSectorZ );
}


void RemoveStructFromUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo	= uiMapIndex;
//	Map.usIndex			= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;

	Map.ubType			= SLM_REMOVE_STRUCT;

	SaveModifiedMapStructToMapTempFile( &Map, sSectorX, sSectorY, ubSectorZ );
}


void AddRemoveObjectToUnLoadedMapTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ  )
{
	MODIFY_MAP Map;
	UINT32	uiType;
	UINT16	usSubIndex;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
//	Map.usIndex		= usIndex;
	Map.usImageType = (UINT16)uiType;
	Map.usSubImageIndex = usSubIndex;

	Map.ubType		= SLM_REMOVE_OBJECT;

	SaveModifiedMapStructToMapTempFile( &Map, sSectorX, sSectorY, ubSectorZ );
}


void AddExitGridToMapTempFile( INT32 usGridNo, EXITGRID *pExitGrid, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ )
{
	MODIFY_MAP Map;

	if( !gfApplyChangesToTempFile )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Called AddExitGridToMapTempFile() without calling ApplyMapChangesToMapTempFile()" );
		return;
	}

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = usGridNo;
//	Map.usIndex		= pExitGrid->ubGotoSectorX;

	Map.usImageType = pExitGrid->ubGotoSectorX | ( pExitGrid->ubGotoSectorY << 8 );
	Map.usSubImageIndex = pExitGrid->usGridNo;
	Map.usHiExitGridNo = pExitGrid->usGridNo >> 16;//dnl ch86 260214

	Map.ubExtra		= pExitGrid->ubGotoSectorZ;
	Map.ubType		= SLM_EXIT_GRIDS;

	SaveModifiedMapStructToMapTempFile( &Map, sSectorX, sSectorY, ubSectorZ );
}

BOOLEAN RemoveGraphicFromTempFile( INT32 uiMapIndex, UINT16 usIndex, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ )
{
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;
	UINT32	uiNumBytesRead;
	MODIFY_MAP *pTempArrayOfMaps=NULL;
	MODIFY_MAP *pMap;
	UINT32	uiFileSize;
	UINT32	uiNumberOfElements;
	BOOLEAN	fRetVal=FALSE;
	UINT32	uiType;
	UINT16	usSubIndex;
	UINT32	cnt;


	//Convert the current sector location into a file name
//	GetMapFileName( sSectorX, sSectorY, ubSectorZ, zTempName, FALSE );

	//add the 'm' for 'Modifed Map' to the front of the map name
//	sprintf( zMapName, "%s\\m_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, ubSectorZ );

	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists,
		return( FALSE );
	}


	//Open the file for writing, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}

	//Get the size of the temp file
	uiFileSize = FileGetSize( hFile );

	//Allocate memory for the buffer
	pTempArrayOfMaps = (MODIFY_MAP *) MemAlloc( uiFileSize );
	if( pTempArrayOfMaps == NULL )
	{
		Assert( 0 );
		return( FALSE );
	}

	//Read the map temp file into a buffer
	FileRead( hFile, pTempArrayOfMaps, uiFileSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiFileSize )
	{
		FileClose( hFile );
		return( FALSE );
	}

	//Close the file
	FileClose( hFile );

	//Delete the file
	FileDelete( zMapName );

	//Get the number of elements in the file
	uiNumberOfElements = uiFileSize / sizeof( MODIFY_MAP );

	//Get the image type and subindex
	GetTileType( usIndex, &uiType );
	GetSubIndexFromTileIndex( usIndex, &usSubIndex );

	for( cnt=0; cnt< uiNumberOfElements; cnt++ )
	{
		pMap = &pTempArrayOfMaps[ cnt ];

		//if this is the peice we are looking for
		if( pMap->usGridNo == uiMapIndex && pMap->usImageType == uiType && pMap->usSubImageIndex == usSubIndex )
		{
			//Do nothin
			fRetVal = TRUE;
		}
		else
		{
			//save the struct back to the temp file
			SaveModifiedMapStructToMapTempFile( pMap, sSectorX, sSectorY, ubSectorZ );
		}
	}

	return( fRetVal );
}



void AddOpenableStructStatusToMapTempFile( INT32 uiMapIndex, BOOLEAN fOpened )
{
	MODIFY_MAP Map;

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
	Map.usImageType = fOpened;

	Map.ubType = SLM_OPENABLE_STRUCT;

	SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
}

void AddWindowHitToMapTempFile( INT32 uiMapIndex )
{
	MODIFY_MAP Map;

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = uiMapIndex;
	Map.ubType = SLM_WINDOW_HIT;

	SaveModifiedMapStructToMapTempFile( &Map, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
}

BOOLEAN ModifyWindowStatus( INT32 uiMapIndex )
{
	STRUCTURE *		pStructure;

	pStructure = FindStructure( uiMapIndex, STRUCTURE_WALLNWINDOW );
	if (pStructure)
	{
		SwapStructureForPartner( uiMapIndex, pStructure );		
		return( TRUE );
	}
	// else forget it, window could be destroyed
	return( FALSE );
}

void SetOpenableStructStatusFromMapTempFile( INT32 uiMapIndex, BOOLEAN fOpened )
{
	STRUCTURE * pStructure;
	STRUCTURE * pBase;
	BOOLEAN			fStatusOnTheMap;
	ITEM_POOL			*pItemPool;
  INT32 sBaseGridNo = uiMapIndex;

	pStructure = FindStructure( uiMapIndex, STRUCTURE_OPENABLE );

	if( pStructure == NULL )
	{
//		ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"SetOpenableStructStatusFromMapTempFile( %d, %d ) failed to find the openable struct.	DF 1.", uiMapIndex, fOpened );
		return;
	}

	fStatusOnTheMap = ( ( pStructure->fFlags & STRUCTURE_OPEN ) != 0 );

	if( fStatusOnTheMap != fOpened )
	{
		// Adjust the item's gridno to the base of struct.....
		pBase = FindBaseStructure( pStructure );

		// Get LEVELNODE for struct and remove!
	if ( pBase )
	{
		sBaseGridNo = pBase->sGridNo;
	}

		if(SwapStructureForPartnerWithoutTriggeringSwitches( uiMapIndex, pStructure ) == NULL )
		{
			//an error occured
		}


		// Adjust visiblity of any item pools here....
	// ATE: Nasty bug here - use base gridno for structure for items!
	// since items always drop to base gridno in AddItemToPool
		if ( GetItemPoolFromGround( sBaseGridNo, &pItemPool ) )
		{
			if ( fOpened )
			{
				// We are open, make un-hidden if so....
				SetItemPoolVisibilityOn( pItemPool, ANY_VISIBILITY_VALUE, FALSE );
			}
			else
			{
				// Make sure items are hidden...
				SetItemPoolVisibilityHidden( pItemPool );
			}

		}

	}
}




BOOLEAN ChangeStatusOfOpenableStructInUnloadedSector( UINT16 usSectorX, UINT16 usSectorY, INT8 bSectorZ, INT32 usGridNo, BOOLEAN fChangeToOpen )
{
//	STRUCTURE * pStructure;
//	MODIFY_MAP Map;
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;
	UINT32	uiNumBytesRead;
	UINT32	uiNumBytesWritten;
	UINT32	uiFileSize;
	UINT32	uiNumberOfElements;
	UINT32	cnt;
	MODIFY_MAP *pMap;
	MODIFY_MAP *pTempArrayOfMaps=NULL;
//	UINT16	usIndex;


	//Convert the current sector location into a file name
//	GetMapFileName( usSectorX, usSectorY, bSectorZ, zTempName, FALSE );

	//add the 'm' for 'Modifed Map' to the front of the map name
//	sprintf( zMapName, "%s\\m_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, zMapName, usSectorX, usSectorY, bSectorZ );

	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists, its no problem.
		return( TRUE );
	}

	//Open the file for reading
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		return( FALSE );
	}

	//Get the size of the file
	uiFileSize = FileGetSize( hFile );


	//Allocate memory for the buffer
	pTempArrayOfMaps = (MODIFY_MAP *) MemAlloc( uiFileSize );
	if( pTempArrayOfMaps == NULL )
	{
		Assert( 0 );
		return( TRUE );
	}

	//Read the map temp file into a buffer
	FileRead( hFile, pTempArrayOfMaps, uiFileSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiFileSize )
	{
		FileClose( hFile );
		MemFree( pTempArrayOfMaps);
		return( FALSE );
	}

	//Close the file
	FileClose( hFile );

	//Delete the file
	FileDelete( zMapName );

	uiNumberOfElements = uiFileSize / sizeof( MODIFY_MAP );

	//loop through all the array elements to
	for( cnt=0; cnt< uiNumberOfElements; cnt++ )
	{
		pMap = &pTempArrayOfMaps[ cnt ];

		//if this element is of the same type
		if( pMap->ubType == SLM_OPENABLE_STRUCT )
		{
			//if its on the same gridno
			if( pMap->usGridNo == usGridNo )
			{
				//Change to the desired settings
				pMap->usImageType = fChangeToOpen;

				break;
			}
		}
	}

	//Open the file for writing
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		MemFree( pTempArrayOfMaps);
		return( FALSE );
	}

	//Write the map temp file into a buffer
	FileWrite( hFile, pTempArrayOfMaps, uiFileSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiFileSize )
	{
		FileClose( hFile );
		MemFree( pTempArrayOfMaps);
		return( FALSE );
	}

	FileClose( hFile );

	MemFree( pTempArrayOfMaps);

	return( TRUE );
}

#ifdef JA2UB
//JA25
void AddRemoveExitGridToUnloadedMapTempFile( UINT32 usGridNo, INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ )
{
	MODIFY_MAP Map;

	if( !gfApplyChangesToTempFile )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Called AddRemoveExitGridToUnloadedMapTempFile() without calling ApplyMapChangesToMapTempFile()" );
		return;
	}

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		return;

	memset( &Map, 0, sizeof( MODIFY_MAP ) );

	Map.usGridNo = usGridNo;

	Map.ubType		= SLM_REMOVE_EXIT_GRID;

	SaveModifiedMapStructToMapTempFile( &Map, sSectorX, sSectorY, ubSectorZ );
}
#endif
