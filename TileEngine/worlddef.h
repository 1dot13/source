#ifndef __WORLDDEF_H
#define __WORLDDEF_H

#include "structure.h"
#include "Handle Items.h"


#define WORLD_TILE_X		40
#define WORLD_TILE_Y		20
#define CELL_X_SIZE					10
#define CELL_Y_SIZE					10
//<SB> variable map size
extern INT32 guiWorldCols;
extern INT32 guiWorldRows;

#define OLD_WORLD_COLS			160
#define OLD_WORLD_ROWS			160
#define OLD_WORLD_MAX			25600

#define WORLD_COLS			guiWorldCols
#define WORLD_ROWS			guiWorldRows
#define WORLD_COORD_COLS			(WORLD_COLS*CELL_X_SIZE)
#define WORLD_COORD_ROWS			(WORLD_ROWS*CELL_Y_SIZE)
#define WORLD_MAX					(WORLD_COLS*WORLD_ROWS)

// WANNE - BMP: The maximum value WORLD_MAX can have. DONE!
// CptMoore - DC: added MAX values here so it is known that display cover depends on them
#define WORLD_COLS_MAX			2000
#define WORLD_ROWS_MAX			2000
#define MAX_ALLOWED_WORLD_MAX		WORLD_COLS_MAX*WORLD_ROWS_MAX // (2000 cols x 2000 rows)

//</SB>

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

// SB: new map version, with map dimensions added
#define MAJOR_MAP_VERSION		7.0		
//Current minor map version updater.
#define MINOR_MAP_OVERHEATING		28		// 27 -> 28: Flugente:: increased to 28 because of included weapon overheated and tripwire feature. See ObjectClass for the new Tags!
//#define MINOR_MAP_VERSION		29		// 28 -> 29: increased range of roomnumbers to full UINT16 by DBrot
#define MINOR_MAP_REPAIR_SYSTEM		30		// 30 -> 29: Flugente:: necessary change due to repair system
#define MINOR_MAP_ITEMFLAG64		31		// 30 -> 31: Flugente: changed sObjectFlag from INT32 to UINT64

#define MINOR_MAP_VERSION		MINOR_MAP_ITEMFLAG64

//dnl ch33 230909
#define VANILLA_MAJOR_MAP_VERSION 5.00
#define VANILLA_MINOR_MAP_VERSION 25

#define WORLD_BASE_HEIGHT			0
#define WORLD_CLIFF_HEIGHT		80
 
//A macro that actually memcpy's over data and increments the pointer automatically
//based on the size.  Works like a FileRead except with a buffer instead of a file pointer.
//Used by LoadWorld() and child functions.
#include <memory.h>
//SB: fix macro syntax flaw
//#define  LOADDATA( dst, src, size ) memcpy( dst, src, size ); src += size
#define  LOADDATA( dst, src, size ) { memcpy( dst, src, size ); src += size; }
#define  SKIPDATA( dst, src, size ) { dst = src + size; }//dnl ch81 051213

#define LANDHEAD							0
#define MAXDIR								8

// Defines for shade levels
#define DEFAULT_SHADE_LEVEL		4
#define MIN_SHADE_LEVEL				4
#define MAX_SHADE_LEVEL				15


// DEFINES FOR LEVELNODE FLAGS
#define LEVELNODE_SOLDIER										0x00000001
#define LEVELNODE_UNUSED2										0x00000002
#define LEVELNODE_MERCPLACEHOLDER						0x00000004
#define LEVELNODE_SHOW_THROUGH							0x00000008
#define LEVELNODE_NOZBLITTER								0x00000010
#define LEVELNODE_CACHEDANITILE							0x00000020
#define LEVELNODE_ROTTINGCORPSE							0x00000040
#define LEVELNODE_BUDDYSHADOW								0x00000080
#define LEVELNODE_HIDDEN										0x00000100
#define LEVELNODE_USERELPOS									0x00000200
#define LEVELNODE_DISPLAY_AP								0x00000400
#define LEVELNODE_ANIMATION									0x00000800
#define LEVELNODE_USEABSOLUTEPOS						0x00001000
#define LEVELNODE_REVEAL										0x00002000
#define	LEVELNODE_REVEALTREES								0x00004000
#define	LEVELNODE_USEBESTTRANSTYPE					0x00008000
#define	LEVELNODE_USEZ											0x00010000
#define	LEVELNODE_DYNAMICZ									0x00020000
#define	LEVELNODE_UPDATESAVEBUFFERONCE			0x00040000
#define LEVELNODE_ERASEZ										0x00080000
#define LEVELNODE_WIREFRAME									0x00100000
#define LEVELNODE_ITEM											0x00200000
#define LEVELNODE_IGNOREHEIGHT							0x00400000
#define LEVELNODE_DYNAMIC										0x02000000
#define LEVELNODE_LASTDYNAMIC								0x04000000
#define LEVELNODE_PHYSICSOBJECT							0x08000000
#define LEVELNODE_NOWRITEZ									0x10000000
#define LEVELNODE_MULTITILESOLDIER					0x20000000
#define LEVELNODE_EXITGRID									0x40000000
#define LEVELNODE_CAVE											0x80000000


// THE FIRST FEW ( 4 ) bits are flags which are saved in the world
#define MAPELEMENT_REDUNDENT						0x0001
#define MAPELEMENT_REEVALUATE_REDUNDENCY			0x0002
#define MAPELEMENT_ENEMY_MINE_PRESENT				0x0004
#define MAPELEMENT_PLAYER_MINE_PRESENT				0x0008
#define	MAPELEMENT_STRUCTURE_DAMAGED				0x0010
#define MAPELEMENT_REEVALUATEBLOOD					0x0020
#define MAPELEMENT_INTERACTIVETILE					0x0040
#define MAPELEMENT_RAISE_LAND_START					0x0080
#define MAPELEMENT_REVEALED							0x0100
#define MAPELEMENT_RAISE_LAND_END					0x0200
#define MAPELEMENT_REDRAW							0x0400
#define MAPELEMENT_REVEALED_ROOF					0x0800
#define MAPELEMENT_MOVEMENT_RESERVED				0x1000
#define MAPELEMENT_RECALCULATE_WIREFRAMES			0x2000
#define MAPELEMENT_ITEMPOOL_PRESENT					0x4000
#define MAPELEMENT_REACHABLE						0x8000

#define MAPELEMENT_EXT_SMOKE						0x0001 //0x01
#define MAPELEMENT_EXT_TEARGAS						0x0002 //0x02
#define MAPELEMENT_EXT_MUSTARDGAS					0x0004 //0x04
#define MAPELEMENT_EXT_DOOR_STATUS_PRESENT			0x0008 //0x08
#define MAPELEMENT_EXT_RECALCULATE_MOVEMENT			0x0010 //0x10
#define MAPELEMENT_EXT_NOBURN_STRUCT				0x0020 //0x20
#define MAPELEMENT_EXT_ROOFCODE_VISITED				0x0040 //0x40
#define MAPELEMENT_EXT_CREATUREGAS	 				0x0080 //0x80
#define MAPELEMENT_EXT_BURNABLEGAS					0x0100 //0x100
#define MAPELEMENT_EXT_CLIMBPOINT					0x0200 //0x200
#define MAPELEMENT_EXT_SIGNAL_SMOKE					0x0400 //0x400	// added by Flugente
#define MAPELEMENT_EXT_DEBRIS_SMOKE					0x0800 //0x800	// added by Flugente
#define MAPELEMENT_EXT_FIRERETARDANT_SMOKE			0x1000 //0x800	// added by Flugente

#define FIRST_LEVEL 0
#define SECOND_LEVEL 1

//#define ANY_SMOKE_EFFECT		( MAPELEMENT_EXT_CREATUREGAS | MAPELEMENT_EXT_SMOKE | MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS )
#define ANY_SMOKE_EFFECT		( MAPELEMENT_EXT_CREATUREGAS | MAPELEMENT_EXT_SMOKE | MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS | MAPELEMENT_EXT_BURNABLEGAS | MAPELEMENT_EXT_SIGNAL_SMOKE | MAPELEMENT_EXT_DEBRIS_SMOKE | MAPELEMENT_EXT_FIRERETARDANT_SMOKE )


// WDS - Clean up inventory handling
struct LEVELNODE
{
	struct LEVELNODE				*pNext;
	UINT32											uiFlags;							// flags struct

	UINT8												ubSumLights;					// LIGHTING INFO
	UINT8												ubMaxLights;					// MAX LIGHTING INFO

	union 
	{
		struct LEVELNODE				*pPrevNode;					// FOR LAND, GOING BACKWARDS POINTER
		STRUCTURE										*pStructureData;		// STRUCTURE DATA
		INT32												iPhysicsObjectID;		// ID FOR PHYSICS ITEM
		INT32												uiAPCost;						// FOR AP DISPLAY
//SB: change packed exitgrid for EXITGRID *
//		INT32												iExitGridInfo;
//		void *											pExitGridInfo;//dnl ch86 190214
	}; // ( 4 byte union )

	union 
	{
		struct
		{
			UINT16										usIndex;							// TILE DATABASE INDEX
			INT16											sCurrentFrame;				// Stuff for animated tiles for a given tile location ( doors, etc )
		};

		SOLDIERTYPE									*pSoldier;							// POINTER TO SOLDIER

	}; // ( 4 byte union )

	union 
	{
		// Some levelnodes can specify relative X and Y values!
		struct
		{
			INT16											sRelativeX;							// Relative position values
			INT16											sRelativeY;							// Relative position values
		};

		// Some can contains index values into dead corpses
		//struct
		//{
			INT32											iCorpseID;							// Index into corpse ID
		//};

		//struct
		//{
			UINT32										uiAnimHitLocationFlags;	// Animation profile flags for soldier placeholders ( prone merc hit location values )
		//};

		// Some can contains index values into animated tile data
		//struct
		//{
			struct TAG_anitile				*pAniTile;
		//};

		// Can be an item pool as well...
		//struct
		//{
			ITEM_POOL										*pItemPool;					// ITEM POOLS
		//};


	};

	INT16													sRelativeZ;							// Relative position values
 	UINT8													ubShadeLevel;						// LIGHTING INFO
 	UINT8													ubNaturalShadeLevel;		// LIGHTING INFO
	UINT8													ubFakeShadeLevel;				// LIGHTING INFO

}; // LEVELNODE;

 
#define		LAND_START_INDEX									1
#define		OBJECT_START_INDEX								2
#define		STRUCT_START_INDEX								3
#define		SHADOW_START_INDEX								4
#define		MERC_START_INDEX									5
#define		ROOF_START_INDEX									6
#define		ONROOF_START_INDEX								7
#define		TOPMOST_START_INDEX								8

#define pLandHead pLevelNodes[ 0 ]
#define pLandStart pLevelNodes[ 1 ]
#define pObjectHead pLevelNodes[ 2 ]
#define pStructHead pLevelNodes[ 3 ]
#define pShadowHead pLevelNodes[ 4 ]
#define pMercHead pLevelNodes[ 5 ]
#define pRoofHead pLevelNodes[ 6 ]
#define pOnRoofHead pLevelNodes[ 7 ]
#define pTopmostHead pLevelNodes[ 8 ]

typedef struct
{
	//union 
	//{
		//struct
		//{
			//LEVELNODE								*pLandHead;							//0
			//LEVELNODE								*pLandStart;						//1

			//LEVELNODE								*pObjectHead;						//2

			//LEVELNODE								*pStructHead;						//3

			//LEVELNODE								*pShadowHead;						//4

			//LEVELNODE								*pMercHead;							//5

			//LEVELNODE								*pRoofHead;							//6

			//LEVELNODE								*pOnRoofHead;						//7

			//LEVELNODE								*pTopmostHead;					//8
		//};

		LEVELNODE									*pLevelNodes[ 9 ];
	//};

	STRUCTURE									*pStructureHead;				
	STRUCTURE									*pStructureTail;

	UINT16										uiFlags;
	UINT16										ubExtFlags[2];
	UINT16										sSumRealLights[1];
	UINT8										sHeight;
	UINT8										ubAdjacentSoldierCnt;
	UINT8										ubTerrainID;

 	UINT8										ubReservedSoldierID;
	UINT8										ubBloodInfo;
	UINT8										ubSmellInfo;
} MAP_ELEMENT;


// World Data
extern MAP_ELEMENT			*gpWorldLevelData;

// Flugente: this stuff is only ever used in AStar pathing and is a unnecessary waste of resources otherwise, so I'm putting an end to this
#ifdef USE_ASTAR_PATHS
// World Movement Costs
//UINT8						gubWorldMovementCosts[ WORLD_MAX ][MAXDIR][2];
//ddd Tables to track tactical value of grid tiles
extern BOOLEAN						gubWorldTileInLight[ MAX_ALLOWED_WORLD_MAX ];
extern BOOLEAN						gubIsCorpseThere[ MAX_ALLOWED_WORLD_MAX ]; //Tracks position of corpses for AI avoidance
extern INT32						gubMerkCanSeeThisTile[ MAX_ALLOWED_WORLD_MAX ]; //Tracks visibility my mercs
//ddd
#endif

extern UINT8 (*gubWorldMovementCosts)[MAXDIR][2];//dnl ch43 260909

//dnl ch44 290909 Translation routine
class MAPTRANSLATION
{
private:
	BOOLEAN fTrn;
	INT32 iTrnFromRows;
	INT32 iTrnFromCols;
	INT32 iTrnToRows;
	INT32 iTrnToCols;
	INT32 iResizeTrnFromRows;
	INT32 iResizeTrnFromCols;
	INT32 iResizeTrnToRows;
	INT32 iResizeTrnToCols;

public:
	MAPTRANSLATION();
	~MAPTRANSLATION();

	void DisableTrn(void){ fTrn = FALSE; }
	void GetTrnCnt(INT32& cnt);
	void GetTrnXY(INT16& x, INT16& y);
	BOOLEAN IsTrn(void){ return(fTrn); }
	BOOLEAN SetTrnPar(INT32 iFromRows, INT32 iFromCols, INT32 iToRows, INT32 iToCols);
	//dnl ch45 011009
	void ResizeTrnCfg(INT32 iFromRows, INT32 iFromCols, INT32 iToRows, INT32 iToCols);
	void ResizeTrnCnt(INT32& cnt);
};
extern MAPTRANSLATION gMapTrn;

extern UINT8		gubCurrentLevel;
extern INT32		giCurrentTilesetID;

extern HVOBJECT		hRenderVObject;
extern UINT32		gSurfaceMemUsage;

extern CHAR8		gzLastLoadedFile[ 260 ];

extern INT16		gsRecompileAreaTop;
extern INT16		gsRecompileAreaLeft;
extern INT16		gsRecompileAreaRight;
extern INT16		gsRecompileAreaBottom;

// Functions
BOOLEAN InitializeWorld( );
void DeinitializeWorld( );

void BuildTileShadeTables( );
void DestroyTileShadeTables( );


void TrashWorld(void);
void TrashMapTile(INT32 MapTile);
BOOLEAN NewWorld( INT32 nMapRows,  INT32 nMapCols );

BOOLEAN SaveWorld(const STR8 puiFilename, FLOAT dMajorMapVersion=MAJOR_MAP_VERSION, UINT8 ubMinorMapVersion=MINOR_MAP_VERSION);//dnl ch33 150909
BOOLEAN LoadWorld(const STR8 puiFilename, FLOAT* pMajorMapVersion=NULL, UINT8* pMinorMapVersion=NULL);//dnl ch44 290909

void CompileWorldMovementCosts(void);//dnl ch56 151009
void RecompileLocalMovementCosts( INT32 sCentreGridNo );
void RecompileLocalMovementCostsFromRadius( INT32 sCentreGridNo, INT8 bRadius );


BOOLEAN LoadMapTileset( INT32 iTilesetID );
BOOLEAN SaveMapTileset( INT32 iTilesetID );

void SetLoadOverrideParams( BOOLEAN fForceLoad, BOOLEAN fForceFile, CHAR8 *zLoadName );

void CalculateWorldWireFrameTiles( BOOLEAN fForce );
void RemoveWorldWireFrameTiles( );
void RemoveWireFrameTiles( INT32 sGridNo );


LEVELNODE *GetAnimProfileFlags( INT32 sGridNo, UINT16 *usFlags, SOLDIERTYPE **ppTargSoldier, LEVELNODE *pGivenNode );

void ReloadTileset( UINT8 ubID );

BOOLEAN FloorAtGridNo( INT32 iMapIndex );
BOOLEAN DoorAtGridNo( INT32 iMapIndex );
BOOLEAN GridNoIndoors( INT32 iMapIndex );


BOOLEAN OpenableAtGridNo( INT32 iMapIndex );

void RecompileLocalMovementCostsInAreaWithFlags( void );
void AddTileToRecompileArea( INT32 sGridNo );

void SetWorldSize(INT32 nWorldRows, INT32 nWorldCols);

#endif
