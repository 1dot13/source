#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "tiledef.h"
	#include "worlddef.h"
#endif

#include "GameSettings.h"

INT32 giNumberOfTiles; //Madd: new global variables to allow for a variable number of Pitems
INT32 giNumberOfTileTypes; 

INT16 gOpenDoorList[20] =
{
	FIRSTDOOR1,
	SECONDDOOR1,
	THIRDDOOR1,
	FOURTHDOOR1,
	FIRSTDOOR6,
	SECONDDOOR6,
	THIRDDOOR6,
	FOURTHDOOR6,
	FIRSTDOOR11,
	SECONDDOOR11,
	THIRDDOOR11,
	FOURTHDOOR11,
	FIRSTDOOR16,
	SECONDDOOR16,
	THIRDDOOR16,
	FOURTHDOOR16,
	-1
};

INT16 gOpenDoorShadowList[20] =
{
	FIRSTDOORSH1,
	SECONDDOORSH1,
	THIRDDOORSH1,
	FOURTHDOORSH1,
	FIRSTDOORSH6,
	SECONDDOORSH6,
	THIRDDOORSH6,
	FOURTHDOORSH6,
	FIRSTDOORSH11,
	SECONDDOORSH11,
	THIRDDOORSH11,
	FOURTHDOORSH11,
	FIRSTDOORSH16,
	SECONDDOORSH16,
	THIRDDOORSH16,
	FOURTHDOORSH16,
	-1
};

INT16 gClosedDoorList[20] =
{
	FIRSTDOOR5,
	SECONDDOOR5,
	THIRDDOOR5,
	FOURTHDOOR5,
	FIRSTDOOR10,
	SECONDDOOR10,
	THIRDDOOR10,
	FOURTHDOOR10,
	FIRSTDOOR15,
	SECONDDOOR15,
	THIRDDOOR15,
	FOURTHDOOR15,
	FIRSTDOOR20,
	SECONDDOOR20,
	THIRDDOOR20,
	FOURTHDOOR20,
	-1
};

INT16 gClosedDoorShadowList[20] =
{
	FIRSTDOORSH5,
	SECONDDOORSH5,
	THIRDDOORSH5,
	FOURTHDOORSH5,
	FIRSTDOORSH10,
	SECONDDOORSH10,
	THIRDDOORSH10,
	FOURTHDOORSH10,
	FIRSTDOORSH15,
	SECONDDOORSH15,
	THIRDDOORSH15,
	FOURTHDOORSH15,
	FIRSTDOORSH20,
	SECONDDOORSH20,
	THIRDDOORSH20,
	FOURTHDOORSH20,
	-1
};



// REVERSE BUDDIES FROM SHADOW BACK TO STRUCT
INT16 gReverseShadowBuddys[] =
{
	FIRSTCLIFFSHADOW,			FIRSTCLIFFSHADOW1,			FIRSTCLIFF1,

	FIRSTSHADOW,					FIRSTSHADOW1,						FIRSTOSTRUCT1,
	SECONDSHADOW,					SECONDSHADOW1,					SECONDOSTRUCT1,
	THIRDSHADOW,					THIRDSHADOW1,						THIRDOSTRUCT1,
	FOURTHSHADOW,					FOURTHSHADOW1,					FOURTHOSTRUCT1,
	FIFTHSHADOW,					FIFTHSHADOW1,						FIFTHOSTRUCT1,
	SIXTHSHADOW,					SIXTHSHADOW1,						SIXTHOSTRUCT1,
	SEVENTHSHADOW,				SEVENTHSHADOW1,					SEVENTHOSTRUCT1,
	EIGHTSHADOW,					EIGHTSHADOW1,						EIGHTOSTRUCT1,

	FIRSTFULLSHADOW,			FIRSTFULLSHADOW1,				FIRSTFULLSTRUCT1,
	SECONDFULLSHADOW,			SECONDFULLSHADOW1,			SECONDFULLSTRUCT1,
	THIRDFULLSHADOW,			THIRDFULLSHADOW1,				THIRDFULLSTRUCT1,
	FOURTHFULLSHADOW,			FOURTHFULLSHADOW1,			FOURTHFULLSTRUCT1,

	FIRSTDOORSHADOW,			FIRSTDOORSH1,						FIRSTDOOR1,
	SECONDDOORSHADOW,			SECONDDOORSH1,					SECONDDOOR1,
	THIRDDOORSHADOW,			THIRDDOORSH1,						THIRDDOOR1,
	FOURTHDOORSHADOW,			FOURTHDOORSH1,					FOURTHDOOR1,

	//FENCE
	FENCESHADOW,					FENCESHADOW1,						FENCESTRUCT1,

	// VEHICLES
	FIRSTVEHICLESHADOW,		FIRSTVEHICLESHADOW1,		FIRSTVEHICLE1,
	SECONDVEHICLESHADOW,	SECONDVEHICLESHADOW1,		SECONDVEHICLE1,

	//DebrisSTRUCT
	FIRSTDEBRISSTRUCTSHADOW,		FIRSTDEBRISSTRUCTSHADOW1,				FIRSTDEBRISSTRUCT1,
	SECONDDEBRISSTRUCTSHADOW,		SECONDDEBRISSTRUCTSHADOW1,			SECONDDEBRISSTRUCT1,

	NINTHOSTRUCTSHADOW,					NINTHOSTRUCTSHADOW1,						NINTHOSTRUCT1,
	TENTHOSTRUCTSHADOW,					TENTHOSTRUCTSHADOW1,						TENTHOSTRUCT1,

	FIRSTLARGEEXPDEBRISSHADOW,	FIRSTLARGEEXPDEBRISSHADOW1,			FIRSTLARGEEXPDEBRIS1,
	SECONDLARGEEXPDEBRISSHADOW,	SECONDLARGEEXPDEBRISSHADOW1,			SECONDLARGEEXPDEBRIS1,

	-1
};


// SHADOW BUDDIES FROM STRUCT FORWARD TO SHADOW
INT16 gForwardShadowBuddys[] =
{
	FIRSTCLIFF,						FIRSTCLIFF1,							FIRSTCLIFFSHADOW1,

	FIRSTOSTRUCT,					FIRSTOSTRUCT1,						FIRSTSHADOW1,
	SECONDOSTRUCT,				SECONDOSTRUCT1,						SECONDSHADOW1,
	THIRDOSTRUCT,					THIRDOSTRUCT1,						THIRDSHADOW1,
	FOURTHOSTRUCT,				FOURTHOSTRUCT1,						FOURTHSHADOW1,
	FIFTHOSTRUCT,					FIFTHOSTRUCT1,						FIFTHSHADOW1,
	SIXTHOSTRUCT,					SIXTHOSTRUCT1,						SIXTHSHADOW1,
	SEVENTHOSTRUCT,				SEVENTHOSTRUCT1,					SEVENTHSHADOW1,
	EIGHTOSTRUCT,					EIGHTOSTRUCT1,						EIGHTSHADOW1,

	FIRSTFULLSTRUCT,			FIRSTFULLSTRUCT1,				FIRSTFULLSHADOW1,
	SECONDFULLSTRUCT,			SECONDFULLSTRUCT1,			SECONDFULLSHADOW1,
	THIRDFULLSTRUCT,			THIRDFULLSTRUCT1,				THIRDFULLSHADOW1,
	FOURTHFULLSTRUCT,			FOURTHFULLSTRUCT1,			FOURTHFULLSHADOW1,

	FIRSTDOOR,						FIRSTDOOR1,								FIRSTDOORSH1,
	SECONDDOOR,						SECONDDOOR1,							SECONDDOORSH1,
	THIRDDOOR,						THIRDDOOR1,								THIRDDOORSH1,
	FOURTHDOOR,						FOURTHDOOR1,							FOURTHDOORSH1,

	//FENCE
	FENCESTRUCT,					FENCESTRUCT1,							FENCESHADOW1,

	// VEHICLE
	FIRSTVEHICLE,					FIRSTVEHICLE1,						FIRSTVEHICLESHADOW1,
	SECONDVEHICLE,				SECONDVEHICLE1,						SECONDVEHICLESHADOW1,

	FIRSTDEBRISSTRUCT,		FIRSTDEBRISSTRUCT1,				FIRSTDEBRISSTRUCTSHADOW1,
	SECONDDEBRISSTRUCT,		SECONDDEBRISSTRUCT1,			SECONDDEBRISSTRUCTSHADOW1,

	NINTHOSTRUCT,					NINTHOSTRUCT1,						NINTHOSTRUCTSHADOW1,
	TENTHOSTRUCT,					TENTHOSTRUCT1,						TENTHOSTRUCTSHADOW1,

	FIRSTLARGEEXPDEBRIS,	FIRSTLARGEEXPDEBRIS1,			FIRSTLARGEEXPDEBRISSHADOW1,
	SECONDLARGEEXPDEBRIS,	SECONDLARGEEXPDEBRIS1,		SECONDLARGEEXPDEBRISSHADOW1,

	-1

};

// Global variable used to initialize tile database with full tile spec
UINT8	gFullBaseTileValues[] =

{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // First Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Second Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Third Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Forth Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Fifth Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Sixth Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Seventh Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Water1 Texture
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1 // Water2 Texture

};


STR gTileSurfaceName[NUMBEROFTILETYPES] =
{
"TEXTURE1",
"TEXTURE2",
"TEXTURE3",
"TEXTURE4",
"TEXTURE5",
"TEXTURE6",
"TEXTURE7",
"WATER1",
"DEEP WATER",
"FIRSTCLIFFHANG",
"FIRSTCLIFF",
"FIRSTCLIFFSHADOW",
"OSTRUCT1",
"OSTRUCT2",
"OSTRUCT3",
"OSTRUCT4",
"OSTRUCT5",
"OSTRUCT6",
"OSTRUCT7",
"OSTRUCT8",
"OFSTRUCT1",
"OFSTRUCT2",
"PLACEHOLDER1",
"PLACEHOLDER2",

"SHADOW1",
"SHADOW2",
"SHADOW3",
"SHADOW4",
"SHADOW5",
"SHADOW6",
"SHADOW7",
"SHADOW8",
"FSHADOW1",
"FSHADOW2",
"PLACEHOLDER3",
"PLACEHOLDER4",

"WALL1",
"WALL2",
"WALL3",
"WALL4",
"DOOR1",
"DOOR2",
"DOOR3",
"DOOR4",
"DOORSH1",
"DOORSH2",
"DOORSH3",
"DOORSH4",
"SLANTFLATPEICE",
"ANOTHERDEBRIS",
"ROADPIECES",
"WINDOW4",
"DECORATIONS1",
"DECORATIONS2",
"DECORATIONS3",
"DECORATIONS4",
"WALLDECAL1",
"WALLDECAL2",
"WALLDECAL3",
"WALLDECAL4",
"FLOOR1",
"FLOOR2",
"FLOOR3",
"FLOOR4",
"ROOF1",
"ROOF2",
"ROOF3",
"ROOF4",
"SROOF1",
"SROOF2",
"ONROOF1",
"ONROOF2",
"MOCKF1",

"ISTRUCT1",
"ISTRUCT2",
"ISTRUCT3",
"ISTRUCT4",

"FIRSTCISTRCUT",

"FIRSTROAD",

"ROCKS",
"WOOD",
"WEEDS",
"GRASS",
"SAND",
"MISC",

"ANIOSTRUCT",
"FENCESTRUCT",
"FENCESHADOW",

"FIRSTVEHICLE",
"SECONDVEHICLE",
"FIRSTVEHICLESHADOW",
"SECONDVEHICLESHADOW",
"MISC2",
"FIRSTDEBRISSTRUCT",
"SECONDDEBRISSTRUCT",
"FIRSTDEBRISSTRUCTSHADOW",
"SECONDDEBRISSTRUCTSHADOW",
"NINTHOSTRUCT",
"TENTHOSTRUCT",
"NINTHOSTRUCTSHADOW",
"TENTHOSTRUCTSHADOW",

"FIRSTEXPLODEDEBRIS",
"SECONDEXPLODEDEBRIS",
"FIRSTLARGEEXPLODEDEBRIS",
"SECONDLARGEEXPLODEDEBRIS",
"FIRSTLARGEEXPLODEDEBRISSHADOW",
"SECONDLARGEEXPLODEDEBRISSHADOW",

"FIFTHISTRUCT",
"SIXTHISTRUCT",
"SEVENTHISTRUCT",
"EIGHTISTRUCT",

"FIRSTHIGHROOF",
"SECONDHIGHROOF",

"WALLDECAL5",
"WALLDECAL6",
"WALLDECAL7",
"WALLDECAL8",

"HUMANBLOOD",
"CREATUREBLOOD",
"FIRSTSWITCHES",

// ABSOLUTELY NO STUFF PAST HERE!
// CAN BE SAVED IN THE MAP DIRECTLY!
"REVEALEDSLANTROOF",
"1stREVEALEDHIGHROOF",
"2ndREVEALEDHIGHROOF",

"GUNS",
"ITEMS",
"ITEMS2",

"GLASSSHATTER",
"ITEMS3",
"BODYBLOW",

"EXITTEXTURE",
"FOOTPRINTS",
"POINTERS",
"POINTERS2",
"POINTERS3",

"GOODRUN",
"GOODWALK",
"GOODSWAT",
"GOODSCOOT",
"CONFIRMMOVE",
"VEHICLEMOVE",
"ACTIONTWO",
"BADMARKER",
"GRING",
"ROTATINGKEY",
"SELRING",
"SPECIAL",
"BULLET",
"MISS1",
"MISS2",
"MISS3",
"WIREFRAME"
};


UINT16	gNumTilesPerType[ NUMBEROFTILETYPES ] =
{
	FIRSTTEXTURE35	 - FIRSTTEXTURE1 + 1,
	SECONDTEXTURE35	- SECONDTEXTURE1 + 1,
	THIRDTEXTURE35	 - THIRDTEXTURE1 + 1,
	FOURTHTEXTURE35	- FOURTHTEXTURE1 + 1,
	FIFTHTEXTURE35	 - FIFTHTEXTURE1 + 1,
	SIXTHTEXTURE37		- SIXTHTEXTURE1 + 1,
	SEVENTHTEXTURE49	- SEVENTHTEXTURE1 + 1,
	REGWATERTEXTURE50	- REGWATERTEXTURE1 + 1,
	DEEPWATERTEXTURE37 - DEEPWATERTEXTURE1 + 1,
	FIRSTCLIFFHANG17	- FIRSTCLIFFHANG1 + 1,
	FIRSTCLIFF17			- FIRSTCLIFF1 + 1,
	FIRSTCLIFFSHADOW17 - FIRSTCLIFFSHADOW1 + 1,	// Med hill

	FIRSTOSTRUCT12		- FIRSTOSTRUCT1 + 1,
	SECONDOSTRUCT12		- SECONDOSTRUCT1 + 1,
	THIRDOSTRUCT12		- THIRDOSTRUCT1 + 1,
	FOURTHOSTRUCT12	- FOURTHOSTRUCT1 + 1,	 // Fourth OSTRUCT
	FIFTHOSTRUCT12		- FIFTHOSTRUCT1 + 1,		// Fifth OSTRUCT
	SIXTHOSTRUCT12		- SIXTHOSTRUCT1 + 1,		// Sixth OSTRUCT
	SEVENTHOSTRUCT12	- SEVENTHOSTRUCT1 + 1,	// Seventh OSTRUCT
	EIGHTOSTRUCT12		- EIGHTOSTRUCT1 + 1,	 // Eigth OSTRUCT
	FIRSTFULLSTRUCT12	- FIRSTFULLSTRUCT1 + 1,
	SECONDFULLSTRUCT12 - SECONDFULLSTRUCT1 + 1,
	THIRDFULLSTRUCT2	- THIRDFULLSTRUCT1 + 1,
	FOURTHFULLSTRUCT2 - FOURTHFULLSTRUCT1 + 1,

	FIRSTSHADOW12			- FIRSTSHADOW1 + 1,
	SECONDSHADOW12	 - SECONDSHADOW1 + 1,
	THIRDSHADOW12			- THIRDSHADOW1 + 1,
	FOURTHSHADOW12		- FOURTHSHADOW1 + 1,
	FIFTHSHADOW12			- FIFTHSHADOW1 + 1,
	SIXTHSHADOW12			- SIXTHSHADOW1 + 1,
	SEVENTHSHADOW12	- SEVENTHSHADOW1 + 1,
	EIGHTSHADOW12			- EIGHTSHADOW1 + 1,
	FIRSTFULLSHADOW12	- FIRSTFULLSHADOW1 + 1,
	SECONDFULLSHADOW12 - SECONDFULLSHADOW1 + 1,
	THIRDFULLSHADOW2	- THIRDFULLSHADOW1 + 1,
	FOURTHFULLSHADOW2 - FOURTHFULLSHADOW1 + 1,


	FIRSTWALL65				- FIRSTWALL1 + 1,
	SECONDWALL65			- SECONDWALL1 + 1,
	THIRDWALL65				- THIRDWALL1 + 1,
	FOURTHWALL65			- FOURTHWALL1 + 1,

	FIRSTDOOR20				- FIRSTDOOR1 + 1,
	SECONDDOOR20			- SECONDDOOR1 + 1,
	THIRDDOOR20				- THIRDDOOR1 + 1,
	FOURTHDOOR20			- FOURTHDOOR1 + 1,

	FIRSTDOORSH20			- FIRSTDOORSH1 + 1,
	SECONDDOORSH20		- SECONDDOORSH1 + 1,
	THIRDDOORSH20			- THIRDDOORSH1 + 1,
	FOURTHDOORSH20		- FOURTHDOORSH1 + 1,

	SLANTROOFCEILING2	- SLANTROOFCEILING1 + 1,
	ANOTHERDEBRIS10		- ANOTHERDEBRIS1 + 1,
	ROADPIECES400			- ROADPIECES001 + 1,
	FOURTHWINDOW2			- FOURTHWINDOW1 + 1,

	FIRSTDECORATIONS10	- FIRSTDECORATIONS1 + 1,
	SECONDDECORATIONS10 - SECONDDECORATIONS1 + 1,
	THIRDDECORATIONS10	- THIRDDECORATIONS1 + 1,
	FOURTHDECORATIONS10 - FOURTHDECORATIONS1 + 1,

	FIRSTWALLDECAL10	- FIRSTWALLDECAL1 + 1,
	SECONDWALLDECAL10	- SECONDWALLDECAL1 + 1,
	THIRDWALLDECAL10	- THIRDWALLDECAL1 + 1,
	FOURTHWALLDECAL10	- FOURTHWALLDECAL1 + 1,

	FIRSTFLOOR8				- FIRSTFLOOR1 + 1,
	SECONDFLOOR8			- SECONDFLOOR1 + 1,
	THIRDFLOOR8				- THIRDFLOOR1 + 1,
	FOURTHFLOOR8			- FOURTHFLOOR1 + 1,

	FIRSTROOF14 			- FIRSTROOF1 + 1,
	SECONDROOF14 			- SECONDROOF1 + 1,
	THIRDROOF14 			- THIRDROOF1 + 1,
	FOURTHROOF14 			- FOURTHROOF1 + 1,
	FIRSTSLANTROOF20	- FIRSTSLANTROOF1 + 1,
	SECONDSLANTROOF20	- SECONDSLANTROOF1 + 1,

	FIRSTONROOF12			- FIRSTONROOF1 + 1,
	SECONDONROOF12		- SECONDONROOF1 + 1,

	1,

	FIRSTISTRUCT24		- FIRSTISTRUCT1 + 1,
	SECONDISTRUCT24		- SECONDISTRUCT1 + 1,
	THIRDISTRUCT24		- THIRDISTRUCT1 + 1,
	FOURTHISTRUCT24		- FOURTHISTRUCT1 + 1,
	FIRSTCISTRUCT24		- FIRSTCISTRUCT1 + 1,

	FIRSTROAD35				- FIRSTROAD1 + 1,

	DEBRISROCKS10			- DEBRISROCKS1 + 1,
	DEBRISWOOD10			- DEBRISWOOD1 + 1,
	DEBRISWEEDS10		- DEBRISWEEDS1 + 1,
	DEBRISGRASS10		- DEBRISGRASS1 + 1,
	DEBRISSAND10		- DEBRISSAND1 + 1,
	DEBRISMISC10		- DEBRISMISC1 + 1,

	ANIOSTRUCT20		- ANIOSTRUCT1 + 1,
	FENCESTRUCT23		- FENCESTRUCT1 + 1,
	FENCESHADOW23		- FENCESHADOW1 + 1,
	FIRSTVEHICLE12					- FIRSTVEHICLE1 + 1,
	SECONDVEHICLE12					- SECONDVEHICLE1 + 1,
	FIRSTVEHICLESHADOW12		- FIRSTVEHICLESHADOW1 + 1,
	SECONDVEHICLESHADOW12		- SECONDVEHICLESHADOW1 + 1,
	DEBRIS2MISC10		- DEBRIS2MISC1 + 1,

	FIRSTDEBRISSTRUCT10						- FIRSTDEBRISSTRUCT1 + 1,
	SECONDDEBRISSTRUCT10					- SECONDDEBRISSTRUCT1 + 1,
	FIRSTDEBRISSTRUCTSHADOW10			- FIRSTDEBRISSTRUCTSHADOW1 + 1,
	SECONDDEBRISSTRUCTSHADOW10		- SECONDDEBRISSTRUCTSHADOW1 + 1,

	NINTHOSTRUCT12								- NINTHOSTRUCT1 + 1,
	TENTHOSTRUCT12								- TENTHOSTRUCT1 + 1,
	NINTHOSTRUCTSHADOW12					- NINTHOSTRUCTSHADOW1 + 1,
	TENTHOSTRUCTSHADOW12					- TENTHOSTRUCTSHADOW1 + 1,

	FIRSTEXPLDEBRIS40							- FIRSTEXPLDEBRIS1 + 1,
	SECONDEXPLDEBRIS40						- SECONDEXPLDEBRIS1 + 1,
	FIRSTLARGEEXPDEBRIS10					- FIRSTLARGEEXPDEBRIS1 + 1,
	SECONDLARGEEXPDEBRIS10				- SECONDLARGEEXPDEBRIS1 + 1,
	FIRSTLARGEEXPDEBRISSHADOW10		- FIRSTLARGEEXPDEBRISSHADOW1 + 1,
	SECONDLARGEEXPDEBRISSHADOW10	- SECONDLARGEEXPDEBRISSHADOW1 + 1,

	FIFTHISTRUCT24								- FIFTHISTRUCT1 + 1,
	SIXTHISTRUCT24								- SIXTHISTRUCT1 + 1,
	SEVENTHISTRUCT24							- SEVENTHISTRUCT1 + 1,
	EIGHTISTRUCT24								- EIGHTISTRUCT1 + 1,

	FIRSTHIGHROOF15								- FIRSTHIGHROOF1 + 1,
	SECONDHIGHROOF15							- SECONDHIGHROOF1 + 1,

	FIFTHWALLDECAL10						- FIFTHWALLDECAL1 + 1,
	SIXTHWALLDECAL10						- SIXTHWALLDECAL1 + 1,
	SEVENTHWALLDECAL10						- SEVENTHWALLDECAL1 + 1,
	EIGTHWALLDECAL10						- EIGTHWALLDECAL1 + 1,

	HUMANBLOOD16									- HUMANBLOOD1 + 1,
	CREATUREBLOOD16								- CREATUREBLOOD1 + 1,
	FIRSTSWITCHES21								- FIRSTSWITCHES1 + 1,

	// NO SAVED STUFF AFTER HERE!
	REVEALEDSLANTROOFS8						- REVEALEDSLANTROOFS1 + 1,
	FIRSTREVEALEDHIGHROOFS11			- FIRSTREVEALEDHIGHROOFS1 + 1,
	SECONDREVEALEDHIGHROOFS11			- SECONDREVEALEDHIGHROOFS1 + 1,

	GUN5000							- GUN1 + 1,
	P1ITEM5000					- P1ITEM1 + 1,
	P2ITEM5000					- P2ITEM1 + 1,

	WINDOWSHATTER20	- WINDOWSHATTER1 + 1,
	P3ITEM5000				- P3ITEM1 + 1,
	BODYEXPLOSION15		- BODYEXPLOSION1 + 1,

	EXITTEXTURE35			- EXITTEXTURE1 + 1,
	FOOTPRINTS80			- FOOTPRINTS1 + 1,

	FIRSTPOINTERS24		- FIRSTPOINTERS1 + 1,
	SECONDPOINTERS8		- SECONDPOINTERS1 + 1,
	THIRDPOINTERS3		- THIRDPOINTERS1 + 1,

	GOODRUN11					- GOODRUN1 + 1,
	GOODWALK11				- GOODWALK1 + 1,
	GOODSWAT11				- GOODSWAT1 + 1,
	GOODPRONE11				- GOODPRONE1 + 1,
	CONFIRMMOVE11			- CONFIRMMOVE1 + 1,
	VEHICLEMOVE10			- VEHICLEMOVE1 + 1,
	ACTIONTWO11				- ACTIONTWO1 + 1,
	BADMARKER11				- BADMARKER1 + 1,
	GOODRING10				- GOODRING1 + 1,
	ROTATINGKEY8			- ROTATINGKEY1 + 1,
	SELRING10				- SELRING1 + 1,
	SPECIALTILE_COVER_5		- SPECIALTILE_MAPEXIT + 1,
	BULLETTILE2				- BULLETTILE1 + 1,

	FIRSTMISS5				- FIRSTMISS1 + 1,
	SECONDMISS5				- SECONDMISS1 + 1,
	THIRDMISS14				- THIRDMISS1 + 1,
	WIREFRAMES15			- WIREFRAMES1 + 1,

//MM: New item tileslots start here
	P4ITEM5000				- P4ITEM1 + 1,
	P5ITEM5000				- P5ITEM1 + 1,
	P6ITEM5000				- P6ITEM1 + 1,
	P7ITEM5000				- P7ITEM1 + 1,
	P8ITEM5000				- P8ITEM1 + 1,
	P9ITEM5000				- P9ITEM1 + 1,
	P10ITEM5000				- P10ITEM1 + 1

};

UINT8	gTileTypeLogicalHeight[ NUMBEROFTILETYPES ] =
{
	2,						// First texture
	2,						// Second texture
	2,						// Third texture
	2,						// Forth texture
	2,						// Fifth texture
	2,						// Sixth texture
	2,						// Seventh texture
	10,						// First water
	10							// Second water
};


void	SetSpecificDatabaseValues( UINT16 usType, UINT16 uiDatabaseElem, TILE_ELEMENT *TileElement, BOOLEAN fUseRaisedObjectType, INT32 iTilesetID )
{
	UINT8 ubLoop;
	INT16						sIndexDiff;
	UINT32					cnt;

	// SETUP BUDDYS FOR SHADOWS
	cnt = 0;
	while( gReverseShadowBuddys[ cnt ] != -1 )
	{
		// IF WE ARE A SHADOW TYPE
		if ( usType == gReverseShadowBuddys[ cnt ] )
		{
			TileElement->sBuddyNum = gReverseShadowBuddys[ cnt + 2 ] + ( uiDatabaseElem - gReverseShadowBuddys[ cnt + 1 ] );

			// Check flags and made the same, take from buddy's
			TileElement->uiFlags |= gTileDatabase[ TileElement->sBuddyNum ].uiFlags;
		}

		cnt+=3;
	};

	// SETUP BUDDYS THAT GO FROM STRUCT TO SHADOW
	cnt = 0;
	while( gForwardShadowBuddys[ cnt ] != -1 )
	{
		// IF WE ARE A SHADOW TYPE
		if ( usType == gForwardShadowBuddys[ cnt ] )
		{
			TileElement->sBuddyNum = gForwardShadowBuddys[ cnt + 2 ] + ( uiDatabaseElem - gForwardShadowBuddys[ cnt + 1 ] );
			// Set flag indicating such
			TileElement->uiFlags |= HAS_SHADOW_BUDDY;
		}
		cnt+=3;
	};



	if ( uiDatabaseElem >= FIRSTDOOR1 &&
			uiDatabaseElem <= FOURTHDOORSH20 )
	{
		// Door anims
		// Open
		cnt = 0;
		while( gOpenDoorList[ cnt ] != -1 )
		{
			// IF WE ARE A SHADOW TYPE
			if ( uiDatabaseElem == gOpenDoorList[ cnt ] )
			{
				// Allocate Animated tile data
				AllocateAnimTileData( TileElement, 5 );

				TileElement->pAnimData->bCurrentFrame = 0;
				for (ubLoop = 0; ubLoop < TileElement->pAnimData->ubNumFrames; ubLoop++)
				{
					TileElement->pAnimData->pusFrames[ubLoop] = uiDatabaseElem + ubLoop;
				}
			}
			cnt++;
		};
		// Open Shadow
		cnt = 0;
		while( gOpenDoorShadowList[ cnt ] != -1 )
		{
			// IF WE ARE A SHADOW TYPE
			if ( uiDatabaseElem == gOpenDoorShadowList[ cnt ] )
			{
				// Allocate Animated tile data
				AllocateAnimTileData( TileElement, 5 );

				TileElement->pAnimData->bCurrentFrame = 0;
				for (ubLoop = 0; ubLoop < TileElement->pAnimData->ubNumFrames; ubLoop++)
				{
					TileElement->pAnimData->pusFrames[ubLoop] = uiDatabaseElem + ubLoop;
				}
			}
			cnt++;
		};

		// Closed
		cnt = 0;
		while( gClosedDoorList[ cnt ] != -1 )
		{
			// IF WE ARE A SHADOW TYPE
			if ( uiDatabaseElem == gClosedDoorList[ cnt ] )
			{
				// Allocate Animated tile data
				AllocateAnimTileData( TileElement, 5 );

				TileElement->pAnimData->bCurrentFrame = 0;
				for (ubLoop = 0; ubLoop < TileElement->pAnimData->ubNumFrames; ubLoop++)
				{
					TileElement->pAnimData->pusFrames[ubLoop] = uiDatabaseElem - ubLoop;
				}
			}
			cnt++;
		};
		// Open Shadow
		cnt = 0;
		while( gClosedDoorShadowList[ cnt ] != -1 )
		{
			// IF WE ARE A SHADOW TYPE
			if ( uiDatabaseElem == gClosedDoorShadowList[ cnt ] )
			{
				// Allocate Animated tile data
				AllocateAnimTileData( TileElement, 5 );

				TileElement->pAnimData->bCurrentFrame = 0;
				for (ubLoop = 0; ubLoop < TileElement->pAnimData->ubNumFrames; ubLoop++)
				{
					TileElement->pAnimData->pusFrames[ubLoop] = uiDatabaseElem - ubLoop;
				}
			}
			cnt++;
		};
	}

	if ( uiDatabaseElem == FIRSTMISS1 )
	{
		// Allocate Animated tile data
		AllocateAnimTileData( TileElement, 5 );

		TileElement->pAnimData->bCurrentFrame = 0;
		for (ubLoop = 0; ubLoop < TileElement->pAnimData->ubNumFrames; ubLoop++)
		{
			TileElement->pAnimData->pusFrames[ubLoop] = uiDatabaseElem + ubLoop;
		}
	}

	if ( uiDatabaseElem >= FIRSTMISS1 &&
			uiDatabaseElem <= FIRSTMISS5 )
	{
		TileElement->uiFlags |= DYNAMIC_TILE;
	}


	if ( uiDatabaseElem == WINDOWSHATTER1 || uiDatabaseElem == WINDOWSHATTER6 || uiDatabaseElem == WINDOWSHATTER11 || uiDatabaseElem == WINDOWSHATTER16 )
	{
		// Allocate Animated tile data
		AllocateAnimTileData( TileElement, 5 );

		TileElement->pAnimData->bCurrentFrame = 0;
		for (ubLoop = 0; ubLoop < TileElement->pAnimData->ubNumFrames; ubLoop++)
		{
			TileElement->pAnimData->pusFrames[ubLoop] = uiDatabaseElem + ubLoop;
		}
	}

	if ( uiDatabaseElem >= WINDOWSHATTER1 &&
			uiDatabaseElem <= WINDOWSHATTER20 )
	{
		TileElement->uiFlags |= DYNAMIC_TILE;
	}

//	if ( usType == FIRSTEXPLOSION )
//	{
//		TileElement->uiFlags |= DYNAMIC_TILE;
//	}

	if ( usType == BODYEXPLOSION )
	{
		TileElement->uiFlags |= DYNAMIC_TILE;
	}

	if ( usType == FIRSTROAD )
	{
		TileElement->uiFlags |= ROAD_TILE;
	}

	// SET FLAGS FOR OBJECTS PEICES WHICH USE PROPER Z
	if ( fUseRaisedObjectType )
	{
		TileElement->uiFlags |= OBJECTLAYER_USEZHEIGHT;
	}


	// OFFSET SLANTED ROOFS TO HEIGHT OF WALLS
	if ( uiDatabaseElem >= FIRSTROOF1 &&
			uiDatabaseElem <= SECONDSLANTROOF20 )
	{
		//TileElement->sOffsetHeight = WALL_HEIGHT;
	}

	// Set flag for full 3d tiles, as well as the dynamic flag for the folliage
	if ( uiDatabaseElem >= FIRSTFULLSTRUCT1 &&
			uiDatabaseElem <= SECONDFULLSTRUCT12 )
	{
		//TileElement->uiFlags |= FULL3D_TILE;
		sIndexDiff = uiDatabaseElem - gTileTypeStartIndex[ usType ];

		// Every second set as dynamic
		if ( ( ( sIndexDiff ) % 3 ) == 1 )
		{
			TileElement->uiFlags |= DYNAMIC_TILE;
		}

		// Set every first as full tile
		if ( ( ( sIndexDiff ) % 3 ) == 0 )
		{
			TileElement->uiFlags |= FULL3D_TILE;
		}

	}

	//Ignore height for cliffs ie: if we rasie the land, do not offset the cliff
	if ( uiDatabaseElem >= FIRSTCLIFFHANG1 &&
			uiDatabaseElem <= FIRSTCLIFFSHADOW17 )
	{
		if ( usType == FIRSTCLIFFHANG )
		{
			TileElement->uiFlags |= CLIFFHANG_TILE;
		}
		TileElement->uiFlags |= IGNORE_WORLD_HEIGHT;

	}

	if ( uiDatabaseElem >= FIRSTWALL1 &&
			uiDatabaseElem <= FOURTHWALL65 )
	{
		TileElement->uiFlags |= WALL_TILE;
	}

	// Set a-frames heigher!
	if ( uiDatabaseElem >= FIRSTWALL1 &&
			uiDatabaseElem <= FOURTHWALL47 )
	{

		// Set these ones higher ( for roof pieces )
		if ( uiDatabaseElem	>= ( gTileTypeStartIndex[ usType ] + WALL_AFRAME_START ) &&
				uiDatabaseElem	<= ( gTileTypeStartIndex[ usType ] + WALL_AFRAME_END ) )
		{
			//TileElement->sOffsetHeight = WALL_HEIGHT;
			TileElement->uiFlags |= AFRAME_TILE;
		}


	}

	// Set UI Elements to be dynamic
	if ( uiDatabaseElem >= FOOTPRINTS1 &&
			uiDatabaseElem <= THIRDPOINTERS2 )
	{
		TileElement->uiFlags |= DYNAMIC_TILE;
		TileElement->uiFlags |= OBJECTLAYER_USEZHEIGHT;
	}

	// Set UI Elements to use object z level
	if ( usType >= FOOTPRINTS &&
			usType <= LASTUIELEM )
	{
		TileElement->uiFlags |= OBJECTLAYER_USEZHEIGHT;
	}

	// Set UI Elements to use object z level
	if ( usType >= HUMANBLOOD &&
			usType <= CREATUREBLOOD )
	{
		TileElement->uiFlags |= OBJECTLAYER_USEZHEIGHT;
	}


	// Set UI Elements to use object z level
	if ( usType >= GUNS &&
			usType <= P2ITEMS )
	{
		TileElement->uiFlags |= OBJECTLAYER_USEZHEIGHT;
	}
	if ( usType == P3ITEMS )
	{
		TileElement->uiFlags |= OBJECTLAYER_USEZHEIGHT;
	}

	if ( usType >= FIRSTEXPLDEBRIS && usType <= SECONDEXPLDEBRIS )
	{
		TileElement->uiFlags |= OBJECTLAYER_USEZHEIGHT;
	}

	if ( uiDatabaseElem >= FIRSTDOOR1 &&
			uiDatabaseElem <= FOURTHDOORSH20 )
	{
		//TileElement->uiFlags |= DYNAMIC_TILE;
	}

	// Set UI Elements to be dynamic
	if ( uiDatabaseElem == MOCKFLOOR1 )
	{
		TileElement->uiFlags |= DYNAMIC_TILE;

	}


	if ( usType == BULLETTILE )
	{
		TileElement->uiFlags |= DYNAMIC_TILE;
	}

	if ( usType == WIREFRAMES )
	{
		//TileElement->uiFlags |= DYNAMIC_TILE;
	}

	// Set full tile flag for floors
	if ( uiDatabaseElem >= FIRSTFLOOR1 &&
			uiDatabaseElem <= FOURTHFLOOR8 )
	{
		TileElement->ubFullTile = 1;
	}

	if ( uiDatabaseElem >= FIRSTTEXTURE1 &&
			uiDatabaseElem <= DEEPWATERTEXTURE10 )
	{
		// Set tile 'fullness' attribute
		TileElement->ubFullTile = gFullBaseTileValues[ uiDatabaseElem ];
	}

	if ( usType >= FIRSTONROOF && usType <= SECONDONROOF )
	{
		// Set height!
		//TileElement->sOffsetHeight = WALL_HEIGHT;
	}

	// anv: ok, as far as I understand here, specific elements of water surface are set to FLAT_GROUND instead of LOW_WATER (edges technically are part of water surface, but mercs can walk on them)
	// E50 || uiDatabaseElem == REGWATERTEXTURE || seems to a bug, REGWATERTEXTURE is a tile type, not tile element and would overwrite part of first surface instead
	//

	if ( uiDatabaseElem >= REGWATERTEXTURE18 && uiDatabaseElem <= REGWATERTEXTURE50 ||
			uiDatabaseElem == REGWATERTEXTURE12 || uiDatabaseElem == REGWATERTEXTURE14 || uiDatabaseElem == REGWATERTEXTURE16 )
	{
		TileElement->ubTerrainID			= FLAT_GROUND;
	}
	else if( uiDatabaseElem >= REGWATERTEXTURE1 && uiDatabaseElem <= REGWATERTEXTURE18 )
	{
		// we set properties for water edges, but need to disable them for proper water tiles
		if(gGameExternalOptions.fAdditionalTileProperties)
		{
			TileElement->bWoodCamoAffinity = 0;
			TileElement->bDesertCamoAffinity = 0;
			TileElement->bUrbanCamoAffinity = 0;
			TileElement->bSnowCamoAffinity = 0;
			TileElement->bSoundModifier = 0;
			TileElement->bCamoStanceModifer= 0;
			TileElement->bStealthDifficultyModifer = 0;
			TileElement->bTrapBonus = 0;
			TileElement->uiAdditionalFlags = 0;
		}
	}

	if ( ( usType >= FIRSTROOF && usType <= SECONDSLANTROOF ) || usType == FIRSTHIGHROOF || usType == SECONDHIGHROOF )
	{
		TileElement->uiFlags |= ROOF_TILE;
	}
}


// Madd: sets the giNumberOfTiles variable
void SetNumberOfTiles()
{
	giNumberOfTiles = P4ITEM1 + ((gGameExternalOptions.ubNumPItems - 3) * 5000);
	giNumberOfTileTypes = P4ITEMS + (gGameExternalOptions.ubNumPItems - 3);
}


