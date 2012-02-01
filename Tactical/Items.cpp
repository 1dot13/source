#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "items.h"
	#include "Action Items.h"
	#include "weapons.h"
	#include "Interface Cursors.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Handle UI.h"
	#include "Animation Control.h"
	#include "points.h"
	#include "Sound Control.h"
	#include "Sys globals.h"
	#include "Isometric Utils.h"
	#include "Animation Data.h"
	#include "worldman.h"
	#include "Random.h"
	#include "Campaign.h"
	#include "interface.h"
	#include "interface panels.h"
	#include "explosion control.h"
	#include "Keys.h"
	#include "faces.h"

	#include "wcheck.h"
	#include "soldier profile.h"
	#include "SkillCheck.h"
	#include "los.h"
	#include "message.h"
	#include "text.h"

	#include "fov.h"
	#include "MessageBoxScreen.h"

	#include "PathAIDebug.h"
	#include "Interface Control.h"
	#include "ShopKeeper Interface.h"
	#include "Cursors.h"

	#include "GameSettings.h"
	#include "environment.h"
	#include "Auto Resolve.h"
	#include "Interface Items.h"
	#include "Campaign Types.h"
	#include "history.h"
	#include "Game Clock.h"
	#include "strategicmap.h"
	#include "Inventory Choosing.h"
	#include "Soldier macros.h"
	#include "Smell.h"
	#include "lighting.h"
	#include "utilities.h"
	#include "english.h"
	#include "debug control.h"

	#include "math.h"
#endif

#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define ANY_MAGSIZE 65535

void SetNewItem( SOLDIERTYPE *pSoldier, UINT8 ubInvPos, BOOLEAN fNewItem );

extern	SOLDIERTYPE *gpItemDescSoldier;
extern BOOLEAN			fShowMapInventoryPool;
extern UINT32 guiCurrentItemDescriptionScreen;
extern BOOLEAN AutoPlaceObjectInInventoryStash( OBJECTTYPE *pItemPtr, INT32 sGridNo );

UINT16 OldWayOfCalculatingScopeBonus(SOLDIERTYPE *pSoldier);
// weight units are 100g each

////////////////////////////////////////////////////////////////////////////
//ATE: When adding new items, make sure to update text.c with text description
///////////////////////////////////////////////////////////////////////////

INVTYPE Item[MAXITEMS]; //=
//{
////  							CLASS								SOUND			GRPH	GRA-			PER
////CLASS						INDEX		CURSOR			TYPE			TYPE	PHIC	WT	PCKT	PRICE COOL	DESCRIPTION							REL		REPAIR	FLAGS
////---------				-----		-------     -------		----	--	  --  ----  -----	----	-----------							---		------		-----
//{	IC_PUNCH,					0,			PUNCHCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			ITEM_NOT_BUYABLE , 0, "Nothing"},
////---WEAPONS---
//// NB For convenience, in accessing the Weapons table, the class index
//// of a weapon must be equal to its position in the Item table
//{	IC_GUN,			 			1,			TARGETCURS,	CONDBUL,	0,		1,		6,	1,		 350,	2,		/* Glock 17        */	+2,		+2,			IF_STANDARD_GUN },
//{	IC_GUN,			 			2,			TARGETCURS,	CONDBUL,	0,		2,		6,	1,		 480,	2,		/* Glock 18        */	+1,		+1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			3,			TARGETCURS,	CONDBUL,	0,		3,		11,	1,		 450,	2,		/* Beretta 92F     */	-1,		-1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			4,			TARGETCURS,	CONDBUL,	0,		4,		11,	1,		 470,	2,		/* Beretta 93R     */	-2,		-2,			IF_STANDARD_GUN },
//{	IC_GUN,			 			5,			TARGETCURS,	CONDBUL,	0,		5,		11,	1,		 250,	1,		/* .38 S&W Special */	+4,		+4,			IF_STANDARD_GUN },
//{	IC_GUN,			 			6,			TARGETCURS,	CONDBUL,	0,		6,		10,	1,		 300,	1,		/* .357 Barracuda  */	+3,		+3,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			7,			TARGETCURS,	CONDBUL,	0,		7,		17,	1,		 300,	1,		/* .357 DesertEagle*/ -1,		-1,			IF_STANDARD_GUN },
//{	IC_GUN,			 			8,			TARGETCURS,	CONDBUL,	0,		8,		11,	1,		 400,	2,		/* .45 M1911			 */  0,		0,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			9,			TARGETCURS,	CONDBUL,	0,		9,		21,	0,		 980,	3,		/* H&K MP5K      	 */	-1,		0,			IF_STANDARD_GUN},
//{	IC_GUN,						10,			TARGETCURS,	CONDBUL,	0,		10,		28,	0,		1170,	4,		/* .45 MAC-10	     */	-2,		-1,			IF_STANDARD_GUN },
//
//{	IC_GUN,						11,			TARGETCURS,	CONDBUL,	0,		11,		48,	0,		 700,	3,		/* Thompson M1A1   */	+3,		-3,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						12,			TARGETCURS,	CONDBUL,	0,		12,		26,	0,		1330,	5,		/* Colt Commando   */	 0,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						13,			TARGETCURS,	CONDBUL,	0,		13,		28,	0,		1000,	4,		/* H&K MP53		 		 */	-1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						14,			TARGETCURS,	CONDBUL,	0,		14,		39,	0,		1180,	4,		/* AKSU-74         */ -2,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST  },
//{	IC_GUN,						15,			TARGETCURS,	CONDBUL,	0,		15,		28,	0,		2750,	10,		/* 5.7mm FN P90    */ -2,		-4,			IF_STANDARD_GUN  },
//{	IC_GUN,						16,			TARGETCURS,	CONDBUL,	0,		16,		19,	0,		 620,	3,		/* Type-85         */ -4,		+2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						17,			TARGETCURS,	CONDBUL,	0,		17,		39,	0,		1350,	5,		/* SKS             */ -4,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						18,			TARGETCURS,	CONDBUL,	0,		18,		43,	0,		1930,	6,		/* Dragunov        */ +2,		+2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST  },
//{	IC_GUN,						19,			TARGETCURS,	CONDBUL,	0,		19,		55,	0,		1950,	6,		/* M24             */ +4,		+4,			IF_TWOHANDED_GUN  },
//{	IC_GUN,						20,			TARGETCURS,	CONDBUL,	0,		20,		36,	0,		2380,	8,		/* Steyr AUG       */ +1,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//
//{	IC_GUN,						21,			TARGETCURS,	CONDBUL,	0,		21,		41,	0,		1620,	6,		/* H&K G41         */ +1,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						22,			TARGETCURS,	CONDBUL,	0,		22,		29,	0,		1100,	4,		/* Ruger Mini-14   */  0,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						23,			TARGETCURS,	CONDBUL,	0,		23,		36,	0,		2680,	8,		/* C-7             */ -1,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						24,			TARGETCURS,	CONDBUL,	0,		24,		36,	0,		1970,	7,		/* FA-MAS          */ -2,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						25,			TARGETCURS,	CONDBUL,	0,		25,		36,	0,		1830,	6,		/* AK-74           */ -1,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						26,			TARGETCURS,	CONDBUL,	0,		26,		43,	0,		1450,	5,		/* AKM             */ +2,		+2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						27,			TARGETCURS,	CONDBUL,	0,		27,		29,	0,		2120,	7,		/* M-14            */ +1,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						28,			TARGETCURS,	CONDBUL,	0,		28,		43,	0,		2680,	8,		/* FN-FAL          */	 0,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						29,			TARGETCURS,	CONDBUL,	0,		29,		44,	0,		1570,	5,		/* H&K G3A3        */ +1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						30,			TARGETCURS,	CONDBUL,	0,		30,		38,	0,		2530,	8,		/* H&K G11         */	+3,		-4,			IF_TWOHANDED_GUN },
//
//{	IC_GUN,						31,			TARGETCURS,	CONDBUL,	0,		31,		36,	0,		 670,	3,		/* Remington M870  */	+3,		+3,			IF_TWOHANDED_GUN },
//{	IC_GUN,						32,			TARGETCURS,	CONDBUL,	0,		32,		38,	0,		 980,	4,		/* SPAS-15         */ -2,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						33,			TARGETCURS,	CONDBUL,	0,		33,		41,	0,		2900,	9,		/* CAWS            */	-3,		-3,			IF_TWOHANDED_GUN },
//{	IC_GUN,						34,			TARGETCURS,	CONDBUL,	0,		34,		68,	0,		3100,	10,		/* FN Minimi       */	-1,		-2,			IF_TWOHANDED_GUN },
//{	IC_GUN,						35,			TARGETCURS,	CONDBUL,	0,		35,		48,	0,		3180,	10,		/* RPK-74          */ -1,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						36,			TARGETCURS,	CONDBUL,	0,		36,		93,	0,		3420,	10,		/* H&K 21E         */	+2,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_BLADE,					37,			KNIFECURS,  0,			1,		79,		9,	2,		 100,	2,		/* combat knife    */	+2,		+2,			IF_STANDARD_BLADE},
//{	IC_THROWING_KNIFE,38,					TARGETCURS, 0,			1,		53,		1,	4,		  50,	3,		/* throwing knife  */	-1,		-1,			IF_STANDARD_BLADE},
//{	IC_THROWN,				39,				TOSSCURS,	0,			1,		57,		5,	2,		   0,	0,		/* rock            */ 0,		0,			ITEM_NOT_BUYABLE},
//{	IC_LAUNCHER,			40,				TRAJECTORYCURS,	0,		0,		37,		26,	0,		 900,	7,		/* grenade launcher*/	0,		-1,			IF_TWOHANDED_GUN },
//
//{	IC_LAUNCHER,			41,				TRAJECTORYCURS,	0,		0,		0,		77,	0,		1800,	10,		/* mortar */					0,		-2,			IF_TWOHANDED_GUN},
//{	IC_THROWN,				42,				TOSSCURS,		0,		1,		60,		4,	3,		 0,		0,		/* another rock    */	0,		0,			ITEM_NOT_BUYABLE},
//{	IC_BLADE,					43,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* yng male claws */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					44,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* yng fem claws  */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					45,			KNIFECURS,	0,			0,		0,		0,	1,		   0,	0,		/* old male claws */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					46,			KNIFECURS,	0,			0,		0,		0,	1,		   0,	0,		/* old fem claws  */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_TENTACLES,			47,				KNIFECURS,	0,			0,		0,		0,	1,		   0,	0,		/* queen tentacles*/	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_GUN,						48,			TARGETCURS,	0,			0,		0,		0,	1,		   0,	0,		/* queen spit */			0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_PUNCH,					49,			PUNCHCURS,	0,			1,		102,	1,	4,		  20,	2,		/* brass knuckles */	0,		0,			IF_STANDARD_BLADE },
//{	IC_LAUNCHER,			50,				INVALIDCURS,0,			0,		39,		13,	0,		 500,	8,		/* underslung g.l.*/	0,		0,			IF_STANDARD_GUN},
//
//{	IC_GUN,						51,			TARGETCURS,	0,			0,		38,		21,	0,		 500,	9,		/* rocket Launcher*/	0,		-3,			IF_TWOHANDED_GUN },	// now repairable
//{	IC_BLADE,					52,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* bloodcat claws*/		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					53,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* bloodcat bite */		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					54,			KNIFECURS,	0,			0,		41,		13,	0,		 200,	3,		/* machete */					0,		+3,			IF_STANDARD_BLADE},
//{	IC_GUN,						55,			TARGETCURS,	0,			0,		45,		40,	0,		5000,	10,		/* rocket rifle */		0,		-5,			IF_TWOHANDED_GUN | ITEM_ELECTRONIC },
//{	IC_GUN,						56,			TARGETCURS,	0,			0,		40,		12,	0,		1000,	0,		/* Automag III */			0,		-2,			IF_STANDARD_GUN },
//{	IC_GUN,						57,			TARGETCURS,	0,			0,		0,		0,	0,		   0,	0,		/* infant spit */			0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_GUN,						58,			TARGETCURS,	0,			0,		0,		0,	0,		   0,	0,		/* yng male spit */		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//
//{	IC_GUN,						59,			TARGETCURS,	0,			0,		0,		0,	0,		   0,	0,		/* old male spit */		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_GUN,						60,			TARGETCURS,	0,			0,		37,		26,	0,		   0,	0,		/* tank cannon  */		0,		0,			ITEM_NOT_BUYABLE },
//
//{	IC_GUN,						61,			TARGETCURS,		0,		0,		46,		12,	1,		 500,	5,		/* dart gun */				0,		+3,			IF_STANDARD_GUN },
//{	IC_THROWING_KNIFE,62,					TARGETCURS,		0,		1,		95,		1,	4,		  50,	0,		/*bloody throw.knife*/0,		+4,			IF_STANDARD_BLADE | ITEM_NOT_BUYABLE },
//{	IC_GUN,						63,			TARGETCURS,		0,		0,		48,		18,	0,		   0,	0,		/* flamethrower */		0,		0,			IF_STANDARD_GUN },
//{	IC_PUNCH,					64,			PUNCHCURS,		0,		1,		85,		30,	0,		  40,	1,		/* Crowbar       */		0,		-4,			ITEM_METAL | ITEM_DAMAGEABLE },
//{	IC_GUN,						65,			TARGETCURS,		0,		0,		45,		40,	0,		10000,	10,		/* rocket rifle */		0,		-5,			IF_TWOHANDED_GUN | ITEM_ELECTRONIC },
//// MADD MARKER
//{	IC_GUN,					66,				TARGETCURS,	CONDBUL,	0,		55,		122,0,		   6200,	10,		/* Barrett */				+1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					67,				TARGETCURS,	CONDBUL,	0,		56,		25,	0,		   4100,	9,		/* VAL Silent */				-3,		-3,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					68,				TARGETCURS,	CONDBUL,	0,		57,		80,	0,		   2700,	8,		/* PSG */				+4,		+4,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					69,				TARGETCURS,	CONDBUL,	0,		58,		24,	0,		   2100,	8,		/* TAR 21 */				+3,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
////MAX WEAPONS spot
//{	IC_NONE,					0,			INVALIDCURS,	0,				0,		0,	0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////---AMMO---
//{	IC_AMMO,					0,			INVALIDCURS,	0,		1,		32,		2,	8,		  15,	2,		/* CLIP9_15 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					1,			INVALIDCURS,	0,		1,		35,		3,	4,		  30,	4,		/* CLIP9_30 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					2,			INVALIDCURS,	0,		1,		33,		2,	8,		  45,	4,		/* CLIP9_15_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					3,			INVALIDCURS,	0,		1,		36,		3,	4,		  90,	6,		/* CLIP9_30_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					4,			INVALIDCURS,	0,		1,		34,		2,	8,		  30,	3,		/* CLIP9_15_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					5,			INVALIDCURS,	0,		1,		37,		3,	4,		  60,	5,		/* CLIP9_30_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					6,			INVALIDCURS,	0,		1,		24,		1,	8,		   5,	1,		/* CLIP38_6 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					7,			INVALIDCURS,	0,		1,		25,		1,	8,		  15,	3,		/* CLIP38_6_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					8,			INVALIDCURS,	0,		1,		26,		1,	8,		  10,	2,		/* CLIP38_6_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					9,			INVALIDCURS,	0,		1,		14,		2,	8,		  10,	2,		/* CLIP45_7 */				0,		0,			IF_STANDARD_CLIP},
//
//{	IC_AMMO,					10,			INVALIDCURS,	0,		1,		 4,		10,	4,		  45,	3,		/* CLIP45_30 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					11,			INVALIDCURS,	0,		1,		15,		2,	8,		  45,	4,		/* CLIP45_7_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					12,			INVALIDCURS,	0,		1,		 5,		10,	4,		 135,	5,		/* CLIP45_30_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					13,			INVALIDCURS,	0,		1,		16,		2,	8,		  30,	3,		/* CLIP45_7_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					14,			INVALIDCURS,	0,		1,		 6,		10,	4,		  90,	4,		/* CLIP45_30_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					15,			INVALIDCURS,	0,		1,		11,		1,	8,		  10,	1,		/* CLIP357_6 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					16,			INVALIDCURS,	0,		1,		17,		3,	8,			15,	1,		/* CLIP357_9 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					17,			INVALIDCURS,	0,		1,		12,		1,	8,		  30,	3,		/* CLIP357_6_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					18,			INVALIDCURS,	0,		1,		18,		3,	8,		  45,	3,		/* CLIP357_9_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					19,			INVALIDCURS,	0,		1,		13,		1,	8,		  20,	2,		/* CLIP357_6_HP */		0,		0,			IF_STANDARD_CLIP},
//
//{	IC_AMMO,					20,			INVALIDCURS,	0,		1,		19,		3,	8,		  30,	2,		/* CLIP357_9_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					21,			INVALIDCURS,	0,		1,		 9,		6,	4,		 150,	5,		/* CLIP545_30_AP */		0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					22,			INVALIDCURS,	0,		1,		10,		6,	4,		 100,	4,		/* CLIP545_30_HP */		0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					23,			INVALIDCURS,	0,		1,		 7,		5,	4,		 150,	4,		/* CLIP556_30_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					24,			INVALIDCURS,	0,		1,		 8,		5,	4,		 100,	3,		/* CLIP556_30_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					25,			INVALIDCURS,	0,		1,		22,		3,	6,		  60,	6,		/* CLIP762W_10_AP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					26,			INVALIDCURS,	0,		1,		29,		8,	4,		 180,	4,		/* CLIP762W_30_AP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					27,			INVALIDCURS,	0,		1,		23,		3,	6,		  40,	5,		/* CLIP762W_10_HP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					28,			INVALIDCURS,	0,		1,		30,		8,	4,		 120,	3,		/* CLIP762W_30_HP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					29,			INVALIDCURS,	0,		1,		 3,		1,	6,		  30,	7,		/* CLIP762N_5_AP */		0,		0,			IF_STANDARD_CLIP},
//
//{	IC_AMMO,					30,			INVALIDCURS,	0,		1,		27,		8,	4,		 120,	6,		/* CLIP762N_20_AP */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					31,			INVALIDCURS,	0,		1,		 2,		1,	6,		  20,	6,		/* CLIP762N_5_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					32,			INVALIDCURS,	0,		1,		28,		8,	4,		  80,	5,		/* CLIP762N_20_HP */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					33,			INVALIDCURS,	0,		1,		31,		5,	4,		 700,	8,		/* CLIP47_50_SAP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					34,			INVALIDCURS,	0,		1,		20,		9 ,	4,		 750,	9,		/* CLIP57_50_SAP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					35,			INVALIDCURS,	0,		1,		21,		9,	4,		 500,	9,		/* CLIP57_50_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					36,			INVALIDCURS,	0,		2,		22,		5,	6,		  20,	3,		/* CLIP12G_7 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					37,			INVALIDCURS,	0,		2,		 4,		5,	6,		  20,	3,		/* CLIP12G_7_BUCKSHOT */	0,		0,	IF_STANDARD_CLIP},
//{	IC_AMMO,					38,			INVALIDCURS,	0,		1,		 0,		10,	6,		 300,	9,		/* CLIPCAWS_10_SAP */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					39,			INVALIDCURS,	0,		1,		 1,		10,	6,		 300,	9,		/* CLIPCAWS_10_FLECH */		0,		0,	IF_STANDARD_CLIP},
//
//{	IC_AMMO,					40,			INVALIDCURS,	0,		1,		110,	10,	4,		 500,	9,		/* CLIPROCKET_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					41,			INVALIDCURS,	0,		1,		115,	10,	4,		 500,	9,		/* CLIPROCKET_HE */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					42,			INVALIDCURS,	0,		1,		114,	10,	4,		 500,	9,		/* CLIPROCKET_HEAT */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					43,			INVALIDCURS,	0,		1,		119,	1,	8,		  10,	4,		/* sleep dart */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					44,			INVALIDCURS,	0,		0,		49,		8,	4,		   0,	0,		/* flameThrwr clip */	0,		0,			IF_STANDARD_CLIP },
//// MADD MARKER
//{	IC_AMMO,					45,			INVALIDCURS,	0,		1,		138,	9,	4,		   300,	10,		/* CLIP50_11 */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					46,			INVALIDCURS,	0,		1,		139,	4,	4,		   50,	9,		/* CLIP9H_20 */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					47,			INVALIDCURS,	0,		1,		140,	8,	4,		   50,	6,		/* CLIP9_50 */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					48,			INVALIDCURS,	0,		1,		141,	8,	4,		   50,	7,		/* CLIP9_50_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					49,			INVALIDCURS,	0,		1,		142,	8,	4,		   60,	7,		/* CLIP9_50_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//
//{	IC_AMMO,					50,			INVALIDCURS,	0,		1,		143,	15,	1,		   375,	9,		/* DRUM545_75_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					51,			INVALIDCURS,	0,		1,		144,	15,	1,		   250,	9,		/* DRUM545_75_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					52,			INVALIDCURS,	0,		1,		145,	30,	1,		   1000,9,		/* BELT556_200_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					53,			INVALIDCURS,	0,		1,		146,	30,	1,		   650,	9,		/* BELT556_200_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					54,			INVALIDCURS,	0,		1,		153,	40,	1,		   600, 9,		/* BELT762N_100_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					55,			INVALIDCURS,	0,		1,		154,	40,	1,		   400,	9,		/* BELT762N_100_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					56,			INVALIDCURS,	0,		1,		147,	3,	8,		   300,	6,		/* CLIP57_20_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					57,			INVALIDCURS,	0,		1,		148,	3,	8,		   200,	6,		/* CLIP57_20_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////---EXPLOSIVES---
//
//{	IC_GRENADE,	 			0,				TOSSCURS,		0,		1,		38,		6,	4,		 100,	5,		/* stun grenade				*/	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE },
//{	IC_GRENADE,	 			1,				TOSSCURS,		0,		1,		48,		6,	4,		 120,	4,		/* tear gas grenade   */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,	 			2,				TOSSCURS,		0,		1,		41,		6,	4,		 500,	7, 		/* mustard gas grenade*/	0,		-3,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,	 			3,				TOSSCURS,		0,		1,		50,		3,	6,		 150,	5,		/* mini hand grenade  */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,	 			4,				TOSSCURS,		0,		1,		49,		6,	4,		 200,	6,		/* reg hand grenade   */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			5,			INVALIDCURS,	0,		2,		 3,		11,	2,		 400,	7,		/* RDX                */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			6,			INVALIDCURS,	0,		2,		 0,		11,	1,		 500,	4,		/* TNT (="explosives")*/	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			7,			INVALIDCURS,	0,		2,		23,		11,	1,		1000,	8,		/* HMX (=RDX+TNT)     */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			8,			INVALIDCURS,	0,		1,		45,		11,	1,		 750,	7,		/* C1  (=RDX+min oil) */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			9,			INVALIDCURS,	0,		1,		40,		41,	2,		 400,	9,		/* mortar shell       */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//
//{	IC_BOMB,					10,			BOMBCURS,		0,		1,		46,		8,	1,		 300,	5,		/* mine               */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE },
//{	IC_BOMB,					11,			INVALIDCURS,	0,		1,		44,		11,	1,		1500,	9,		/* C4  ("plastique")  */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,					12,			BOMBCURS,		0,		1,		42,		4,	2,		   0,	0,		/* trip flare				  */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE | ITEM_REPAIRABLE },
//{	IC_BOMB,					13,			BOMBCURS,		0,		1,		43,		4,	2,		   0,	0,		/* trip klaxon        */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE | ITEM_REPAIRABLE },
//{	IC_BOMB,					14,			INVALIDCURS,	0,		1,		107,	2,	4,		 250,	5,		/* shaped charge			*/	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				15,				TOSSCURS,		0,		2,		24,		1,	6,		  50,	3,		/* break light (flare)*/	0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_GRENADE,				16,				INVALIDCURS,	0,		1,		97,		10,	4,		 400,	8,		/* 40mm HE grenade		*/	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				17,				INVALIDCURS,	0,		1,		111,	10,	4,		 250,	6,		/* 40mm tear gas grnd */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				18,				INVALIDCURS,	0,		1,		113,	10,	4,		 200,	5,		/* 40mm stun grenade  */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				19,				INVALIDCURS,	0,		1,		112,	10,	4,		 100,	7,		/* 40mm smoke grenade */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//
//{	IC_GRENADE,				20,				TOSSCURS,		0,		1,		98,		6,	4,		  50,	3,		/* smoke hand grenade */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_BOMB,					21,			INVALIDCURS,	0,		1,		40,		41,	8,		 450,	0,		/* tank shell       */		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR },
//{	IC_BOMB,					22,			INVALIDCURS,	0,		1,		40,		41,	2,		 450,	0,		/* fake struct ignite*/		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE },
//{	IC_GRENADE,				23,				TOSSCURS,		0,		2,		37,		6,	4,		  50,	0,		/* creature cocktail*/		0,		0,			ITEM_DAMAGEABLE | ITEM_METAL },
//{	IC_BOMB,					24,			INVALIDCURS,	0,		1,		40,		41,	2,		 450,	0,		/* fake struct xplod*/		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE },
//{	IC_BOMB,					25,			INVALIDCURS,	0,		1,		40,		41,	2,		 450,	0,		/* fake vehicle xplod*/		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE },
//{	IC_GRENADE,	 			26,				TOSSCURS,		0,		1,		48,		6,	4,		   0,	0,		/* BIG tear gas grenade*/	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE | ITEM_NOT_BUYABLE },
//{	IC_GRENADE,				27,				INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* small creature gas */	0,		0,			0},
//{	IC_GRENADE,				28,				INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* big creature gas */		0,		0,			0},
//{	IC_GRENADE,				29,				INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* very sm creat gas */	  0,		0,			0},
//
////---ARMOUR---
//{ IC_ARMOUR,			 0,				INVALIDCURS,	COND,		1,		66,		20,	0,		 300, 2,		/* Flak jacket     */ 0,		+2,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			 1,				INVALIDCURS,	COND,		2,		18,		22,	0,		 350, 0,		/* Flak jacket w X */ 0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 2,				INVALIDCURS,	COND,		2,		11,		18,	0,		 400, 0,		/* Flak jacket w Y */ 0,		+3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 3,				INVALIDCURS,	COND,		1,		64,		32,	0,		 500, 4,		/* Kevlar jacket   */ 0,		0,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			 4,				INVALIDCURS,	COND,		2,		16,		35,	0,		 600, 0,		/* Kevlar jack w X */ 0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 5,				INVALIDCURS,	COND,		2,		 9,		29,	0,		 700, 0,		/* Kevlar jack w Y */ 0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 6,				INVALIDCURS,	COND,		1,		65,		32,	0,		1000, 8,		/* Spectra jacket  */ 0,		-2,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			 7,				INVALIDCURS,	COND,		2,		17,		35,	0,		1100, 0,		/* Spectra jack w X*/ 0,		-3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 8,				INVALIDCURS,	COND,		2,		10,		29,	0,		1200, 0,		/* Spectra jack w Y*/ 0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 9,				INVALIDCURS,	COND,		1,		67,		39,	0,		 650,	5,		/* Kevlar leggings */	0,		0,			IF_STANDARD_ARMOUR},
//
//{ IC_ARMOUR,			10,				INVALIDCURS,	COND,		2,		19,		43,	0,		 800,	0,		/* Kevlar legs w X */	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			11,				INVALIDCURS,	COND,		2,		12,		35,	0,		 950,	0,		/* Kevlar legs w Y */	0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			12,				INVALIDCURS,	COND,		1,		68,		39,	0,		 900,	8,		/* Spectra leggings*/	0,		-2,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			13,				INVALIDCURS,	COND,		2,		20,		43,	0,		1100,	0,		/* Spectra legs w X*/	0,		-3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			14,				INVALIDCURS,	COND,		2,		13,		35,	0,		1300,	0,		/* Spectra legs w Y*/	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			15,				INVALIDCURS,	COND,		1,		61,		14,	0,		  50,	2,		/* Steel helmet    */	0,		+2,			IF_STANDARD_ARMOUR | ITEM_METAL},
//{ IC_ARMOUR,			16,				INVALIDCURS,	COND,		1,		63,		14,	0,		 200,	4,		/* Kevlar helmet   */	0,		0,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			17,				INVALIDCURS,	COND,		2,		15,		15,	0,		 250,	0,		/* Kevlar helm w X */	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			18,				INVALIDCURS,	COND,		2,		 8,		13,	0,		 300,	0,		/* Kevlar helm w Y */	0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			19,				INVALIDCURS,	COND,		1,		62,		14,	0,		 450,	7,		/* Spectra helmet  */	0,		-2,			IF_STANDARD_ARMOUR},
//
//{ IC_ARMOUR,			20,				INVALIDCURS,	COND,		2,		14,		15,	0,		 550,	0,		/* Spectra helm w X*/	0,		-3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			21,				INVALIDCURS,	COND,		2,		 7,		13,	0,		 650,	0,		/* Spectra helm w Y*/	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			22,				INVALIDCURS,	COND,		1,		81,		12,	2,		 250,	5,		/* Ceramic plates  */ 0,		-4,			(IF_STANDARD_ARMOUR | ITEM_ATTACHMENT) & (~ITEM_REPAIRABLE) },
//{ IC_ARMOUR,			23,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Infant crt hide */ 0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			24,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Yng male hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			25,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Old male hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			26,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Queen cret hide */ 0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			27,				INVALIDCURS,	COND,		1,		96,		20,	0,		 200,	2,		/* Leather jacket  */ 0,		+4,			IF_STANDARD_ARMOUR },
//// NOTE: THE FOLLOWING ITEM'S PRICE VALUE IS IN DIALOGUE AND SHOULD NOT BE CHANGED
//{ IC_ARMOUR,			28,				INVALIDCURS,	COND,		1,		116,	20,	0,		 950,	3,		/* L jacket w kev  */ 0,		+2,			IF_STANDARD_ARMOUR },
//{ IC_ARMOUR,			29,				INVALIDCURS,	COND,		1,		117,	20,	0,		1200,	0,		/* L jacket w kev 18*/0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//
//{ IC_ARMOUR,			30,				INVALIDCURS,	COND,		1,		118,	20,	0,		1500,	0,		/* L jacket w kev c*/ 0,		+3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{	IC_ARMOUR,			31,				INVALIDCURS,	COND,		1,		0,		0,	0,		   0,	0,		/* yng fem hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{	IC_ARMOUR,			32,				INVALIDCURS,	COND,		1,		0,		0,	0,		   0,	0,		/* old fem hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{	IC_ARMOUR,			33,				INVALIDCURS,	COND,		2,		25,		3,	1,		  10,	1,		/* t-shirt */					0,		0,			ITEM_DAMAGEABLE | ITEM_SHOW_STATUS | ITEM_UNAERODYNAMIC},
//{	IC_ARMOUR,			33,				INVALIDCURS,	COND,		2,		34,		3,	1,		  10,	1,		/* t-shirt D. rules*/	0,		0,			ITEM_DAMAGEABLE | ITEM_SHOW_STATUS | ITEM_UNAERODYNAMIC},
//{ IC_ARMOUR,			34,				INVALIDCURS,	COND,		1,		137,		32,	0,		 700, 6,		/* Kevlar2 jacket  */ 0,		-1,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			35,				INVALIDCURS,	COND,		2,		40,		35,	0,		 800, 0,		/* Kevlar2 jack w X*/ 0,		-2,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			36,				INVALIDCURS,	COND,		2,		41,		29,	0,		 900, 0,		/* Kevlar2 jack w Y*/ 0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{	IC_NONE,				0,			INVALIDCURS,	0,			0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,			0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////---MISC---
//{ IC_MEDKIT,			0,				AIDCURS,		USAGE,		1,		73,		5,	4,		 100,	1,		/* First aid kit */		0,		0,			IF_STANDARD_KIT},
//{ IC_MEDKIT,			0,				AIDCURS,		USAGE,		1,		86,		18,	0,		 300,	1,		/* Medical Kit   */		0,		0,			IF_STANDARD_KIT | ITEM_METAL},
//{ IC_KIT,					0,			REPAIRCURS,		COND,			2,		21,		50, 0,		 250,	1,		/* Tool Kit	     */		0,		0,			IF_STANDARD_KIT | ITEM_METAL},
//{ IC_KIT,					0,			INVALIDCURS,	COND,			1,		78,		3,	1,		 250,	3,	  /* Locksmith kit */		0,		-2,			IF_STANDARD_KIT | ITEM_METAL | ITEM_REPAIRABLE},
//{ IC_KIT,					0,			INVALIDCURS,	COND,			1,		58,		1,	4,		 250,	5,	  /* Camouflage kit*/		0,		0,			IF_STANDARD_KIT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		52,		5,	4,		 300,	4,		/* Silencer      */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		76,		9,	4,		 500,	5,		/* Sniper scope  */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		69,		5,	2,		  50,	3,		/* Bipod         */		0,		+5,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		77,		9,	1,		 400,	7,		/* Extended ear	 */		0,		-3,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		74,		9,	1,		 800,	7,		/* Night goggles */		0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		55,		2,	4,		 150,	3,		/* Sun goggles	 */		0,		+3,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE },
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,	  75,		9,	1,		 100,	4,		/* Gas mask   	 */		0,		+1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE },
//{	IC_KIT,					0,			INVALIDCURS,	0,				2,		 5,	 10,	4,		  10,	1,		/* Canteen       */		0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		80,	 10,	1,		 200,	4,		/* Metal detector*/		0,		-2,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		 6,		1,	4,		 900,	7,		/* Compound 18	 */		0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		42,		1,	4,		2500,	0,		/* Jar/QueenBlood*/		0,		0,			ITEM_DAMAGEABLE | ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		 2,		1,	4,		 500,	1,		/* Jar/Elixir    */		0,		0,			ITEM_DAMAGEABLE },
//{	IC_MONEY,				0,			INVALIDCURS,	0,				2,		 1,		1,	1,		   0,	0,		/* Money         */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{ IC_MISC,				0,				JARCURS,			COND,			1,		71,		1,	2,			10, 1,		/* Glass jar		 */		0,		0,			ITEM_DAMAGEABLE},
//
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		72,		5,	2,			50, 1,		/* Jar/CreatureBlood*/0,		0,			ITEM_DAMAGEABLE | ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		1,	8,		 150,	4,		/* Adren Booster */		0,		0,			ITEM_DAMAGEABLE | ITEM_UNAERODYNAMIC},
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		47,		1,	4,		 100, 3,		/* Detonator     */		0,		+1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ATTACHMENT | ITEM_METAL},
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		47,		1,	4,		 200, 6,		/* Rem Detonator */		0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ATTACHMENT | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		93,		1,	8,		   0,	0,		/* Videotape     */		0,		0,			ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		94,		1,	8,		   0,	0,		/* Deed          */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		56,		1,	1,		   0,	0,		/* Letter				 */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		59,		1,	1,		   0,	0,		/* Diskette	  	 */		0,		0,			ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	124,		0,	1,		3000,	0,		/* Chalice	     */		0,		0,			ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	120,		1,	4,		  50,	0,		/* Bloodcat claws*/		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	121,		1,	4,		 100,	0,		/* Bloodcat teeth*/		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		9,	 60,	0,		 400, 0,		/* Bloodcat pelt */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		54,		0,	99,		   0,	0,		/* Switch        */		0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		47,		0,	99,		   0,	0,		/* Action item   */		0,		0,			ITEM_NOT_BUYABLE },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		1,	6,		 300,	6,		/* Regen Booster */		0,		0,			ITEM_DAMAGEABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		0,	99,		   0,	0,		/* syringe 3     */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		0,	99,		   0,	0,		/* syringe 4     */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		0,	99,		   0,	0,		/* syringe 5     */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		72,		5,	2,			10, 1,		/* Jar/Human Blood*/	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		0,		0,	0,		   0,	0,		/* ownership     */		0,		0,			ITEM_NOT_BUYABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		51,		4,	4,		 750,	7,		/* Laser scope   */		0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			REMOTECURS,		0,				1,		54,		9,	4,		 400,	6,		/* Remote bomb trig*/	0,		-2,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			WIRECUTCURS,	0,				1,		88,		4,	2,		  20,	2,		/* Wirecutters   */		0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		90,		9,	4,		  30,	2,		/* Duckbill      */		0,		+5,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		106,	20,	1,		  30,	1,		/* Alcohol  */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		74,		11,	1,		1500,	9,		/* UV goggles */			0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		44,		21,	0,		  30,	0,		/* discarded LAW*/		0,		0,			IF_TWOHANDED_GUN | ITEM_NOT_BUYABLE },
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		0,		40,	0,		   0,	0,		/* head - generic */	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		1,		40,	0,		   0,	0,		/* head - Imposter*/	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		2,		40,	0,		   0,	0,		/* head - T-Rex */		0,		0,			ITEM_DAMAGEABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		3,		40,	0,		   0,	0,		/* head - Slay */			0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		4,		40,	0,		   0,	0,		/* head - Druggist */	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		5,		40,	0,		   0,	0,		/* head - Matron */		0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		6,		40,	0,		   0,	0,		/* head - Tiffany */	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		100,	12,	1,		  20,	1,		/* wine     */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		101,	4,	4,		  10,	1,		/* beer		  */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		99,		0,	2,		  20,	3,		/* pornos   */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		43,		20,	0,		 900,	6,		/* video camera */		0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC  },
//{	IC_FACE,				0,			INVALIDCURS,	0,				0,		42,		5,	1,		2500,	0,		/* robot remote */		0,		-5,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC  },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		103, 20,	0,		 500,	0,		/* creature claws */	0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		7,	 40,	0,		 250,	0,		/* creature flesh */	0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		104, 10,	0,		1000,	0,		/* creature organ */	0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			REMOTECURS,		0,				1,		54,		9,	4,		 400,	6,		/* Remote trigger*/		0,		-2,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		47,		2,	8,		 500,	2,		/* gold watch */			0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		136, 100,	0,		 200,	2,		/* golf clubs */			0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_FACE,				0,			INVALIDCURS,	0,				3,		11,		5,	1,		 100,	1,		/* walkman */					0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		8,	 50,	0,		 300,	2,		/* portable tv */			0,		-3,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC },
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		10,	 10,	1,		  30,	1,		/* cigars */					0,		0,			ITEM_DAMAGEABLE },
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
//{	IC_KEY,					0,			INVALIDCURS,	0,				1,		82,		1,	8,		   0,	0,		/* dull gold key */		0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					1,			INVALIDCURS,	0,				1,		83,		1,	8,		   0,	0,		/* silver key */			0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					2,			INVALIDCURS,	0,				1,		84,		1,	8,		   0,	0,		/* diamond-shpd key */0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					3,			INVALIDCURS,	0,				1,		87,		1,	8,		   0,	0,		/* bright gold key */	0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					4,			INVALIDCURS,	0,				1,		91,		1,	8,		   0,	0,		/* gold key */				0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					5,			INVALIDCURS,	0,				1,		92,		1,	8,		   0,	0,		/* small gold key */	0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					6,			INVALIDCURS,	0,				1,		108,	1,	8,		   0,	0,		/* electronic */			0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_KEY,					7,			INVALIDCURS,	0,				1,		109,	1,	8,		   0,	0,		/* passcard       */	0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					8,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					9,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		149,	4,	4,		 100,	3,		/* Flash Suppressor */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_LAUNCHER,			282,		TARGETCURS,		0,				0,		68,		79,	0,		 1500,	10,		/* rpg 7 */					0,		-3,				IF_TWOHANDED_GUN },
//{	IC_GRENADE,				30,			INVALIDCURS,	0,				1,		150,	21,	0,		 1200,	10,		/* HE rpg ammo	*/	0,		0,					ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				31,			INVALIDCURS,	0,				1,		152,	21,	0,		 2000,	10,		/* AP rpg ammo	*/	0,		0,					ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				32,			INVALIDCURS,	0,				1,		151,	21,	0,		 1000,	10,		/* Frag rpg ammo	*/	0,		0,					ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		155,	4,	4,		 1200,	6,		/* Reflex scoped */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		156,	2,	4,		 1200,	6,		/* Reflex unscoped */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
////{	IC_KEY,					10,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					11,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					12,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					13,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					14,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					15,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					16,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					17,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					18,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					19,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//
//{	IC_KEY,					20,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					21,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					22,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					23,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					24,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					25,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					26,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					27,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					28,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					29,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//
//{	IC_KEY,					30,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					31,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		31,		4,	0,		 150,	2,		/* silver platter */	0,		0,			ITEM_DAMAGEABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		30,		1,	6,		   5,	1,		/* duct tape */				0,		0,			ITEM_DAMAGEABLE | ITEM_HIDDEN_ADDON },
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		28,		3,	1,		  20,	0,		/* aluminum rod */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		32,		1,	8,		   0,	0,		/* spring */					0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_UNAERODYNAMIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		33,		4,	1,		  25,	0,		/* a. rod & spring */	0,		0,			ITEM_NOT_BUYABLE | ITEM_REPAIRABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_INSEPARABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	 133,		4,	1,		  20,	0,		/* steel rod */				0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	 132,		2,	6,		   5,	3,		/* quick glue */			0,		0,			ITEM_DAMAGEABLE  | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	 134,		6,	1,		 150,	0,		/* gun barrel xtndr */0,		0,			ITEM_NOT_BUYABLE | ITEM_REPAIRABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_INSEPARABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		35,		1,	8,		   0,	0,		/* string */					0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC  | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		27,		1,	1,		   0,	0,		/* tin can */					0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC  | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			TINCANCURS,		0,				2,		36,		2,	4,		   0,	0,		/* string & tin can */0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	122,		3,	6,		   5,	0,		/* marbles */					0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	127,		6,	1,		 200,	6,		/* lame boy */				0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	128,		1,	8,		   5,	1,		/* copper wire */			0,		0,			ITEM_METAL | ITEM_HIDDEN_ADDON },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	126,		7,	1,		  50,	0,		/* display unit */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	130,		1,	1,		 100,	5,		/* fumble pak */			0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	131,		1,	2,		  10,	5,		/* xray bulb */				0,		0,			ITEM_DAMAGEABLE | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,	 29,		1,	8,		   1,	1,		/* chewing gum */			0,		0,			ITEM_DAMAGEABLE | ITEM_HIDDEN_ADDON},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	129, 		3,	1,		 100,	0,		/* flash device */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,	 26,		1,	6,		   5,	1,		/* batteries */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	123,		1,	8,		   0,	0,		/* elastic */					0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			REMOTECURS,		0,				1,	125,	 10,	1,		2500,	0,		/* xray device */			0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC},
//{	IC_MONEY,				0,			INVALIDCURS,	0,				2,	 38,		2,	1,		 100,	0,		/* silver   */				0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MONEY,				0,			INVALIDCURS,	0,				1,	135,		2,	1,		 300,	0,		/* gold			*/				0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_KIT,					0,			REFUELCURS,	  0,				2,	 39,	 20,	0,		 250,	0,		/* gas can */					0,		0,			ITEM_DAMAGEABLE},
//
//{	IC_GUN,					328,			TARGETCURS,	CONDBUL,	0,		50,		22,	0,		900,	8,		/* M900         */	+1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					329,			TARGETCURS,	CONDBUL,	0,		51,		10,	0,		400,	6,		/* M950         */	+1,		-1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					330,			TARGETCURS,	CONDBUL,	0,		52,		22,	0,		900,	7,		/* M960A         */	+1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST  },
//{	IC_GUN,					331,			TARGETCURS,	CONDBUL,	0,		53,		19,	1,		300,	5,		/* Micro Uzi         */	+1,		-1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					332,			TARGETCURS,	CONDBUL,	0,		54,		38,	0,		1300,	6,		/* Enfield         */	0,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					333,			TARGETCURS,	CONDBUL,	0,		59,		25,	0,		900,	4,		/* MP5A2         */	0,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					334,			TARGETCURS,	CONDBUL,	0,		60,		29,	0,		1000,	4,		/* MP5SD         */	0,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					335,			TARGETCURS,	CONDBUL,	0,		61,		29,	0,		1600,	7,		/* MP5N         */	+2,		0,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					336,			TARGETCURS,	CONDBUL,	0,		62,		22,	0,		1300,	6,		/* UMP45         */	0,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					337,			TARGETCURS,	CONDBUL,	0,		63,		6,	1,		1000,	7,		/* FIVE7         */	+1,		0,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					338,			TARGETCURS,	CONDBUL,	0,		64,		7,	1,		400,	4,		/* p7m8         */	+2,		+1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					339,			TARGETCURS,	CONDBUL,	0,		65,		33,	0,		2500,	9,		/* g36k         */	+3,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					340,			TARGETCURS,	CONDBUL,	0,		66,		28,	0,		2400,	9,		/* g36c         */	+3,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					341,			TARGETCURS,	CONDBUL,	0,		67,		64,	0,		3000,	10,		/* MSG90A1 */			+4,		+4,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					342,			TARGETCURS,	CONDBUL,	0,		69,		32,	0,		1400,	7,		/* BENNELLI */			+2,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					343,			TARGETCURS,	CONDBUL,	0,		70,		34,	0,		2200,	8,		/* AK103 */			-1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
//};


// NB hack:  if an item appears in this array with an item class of IC_MISC,
// it is a slot used for noting the skill check required for a merge or multi-item attachment

AttachmentInfoStruct AttachmentInfo[MAXITEMS+1];// =
//{
//	{ SILENCER,									IC_GUN,			NO_CHECK,																			0 },
//	{ SNIPERSCOPE,							IC_GUN,			NO_CHECK,																			0 },
//	{ LASERSCOPE,								IC_GUN,			NO_CHECK,																			0 },
//	{ BIPOD,										IC_GUN,			NO_CHECK,																			0 },
//	{ UNDER_GLAUNCHER,					IC_GUN,			NO_CHECK,																			0 },
//	{ DUCKBILL,									IC_GUN,			NO_CHECK,																			0 },
//	{	SPRING_AND_BOLT_UPGRADE,	IC_GUN,			ATTACHING_SPECIAL_ITEM_CHECK,									0 },
//	{ GUN_BARREL_EXTENDER,			IC_GUN,			ATTACHING_SPECIAL_ITEM_CHECK,								  0 },
//	{ DETONATOR,								IC_BOMB,		ATTACHING_DETONATOR_CHECK,										0 },
//	{ REMDETONATOR,							IC_BOMB,		ATTACHING_REMOTE_DETONATOR_CHECK,						-10 },
//	{ REMDETONATOR,							IC_BOMB,		ATTACHING_REMOTE_DETONATOR_CHECK,						-10 },
//	{ XRAY_BULB,								IC_NONE,		ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK,		-15 },
//	{ COPPER_WIRE,							IC_NONE,		ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK,		+20 },
//	{ CERAMIC_PLATES,						IC_ARMOUR,	NO_CHECK,																			0 },
//	{ FLASH_SUPPRESSOR,									IC_GUN,			NO_CHECK,																			0 },
//	{ REFLEX_SCOPED,									IC_GUN,			NO_CHECK,																			0 },
//	{ REFLEX_UNSCOPED,									IC_GUN,			NO_CHECK,																			0 },
//	{ 0,												0 }
//};

//WarmSteel - For the New Attachment System
AttachmentSlotStruct AttachmentSlots[MAXITEMS+1];
ItemReplacementStruct ItemReplacement[MAXATTACHMENTS];

UINT16 Attachment[MAXATTACHMENTS][4];// =
//{
//	{SILENCER, GLOCK_17},
//	{SILENCER, GLOCK_18},
//	{SILENCER, BERETTA_92F},
//	{SILENCER, BERETTA_93R},
//	{SILENCER, SW38},
//	{SILENCER, BARRACUDA},
//	{SILENCER, DESERTEAGLE},
//	{SILENCER, M1911},
//	{SILENCER, MP5K},
//	{SILENCER, MAC10},
//	{SILENCER, THOMPSON},
//	{SILENCER, P90},
//	{SILENCER, M960},
//	{SILENCER, MICRO_UZI},
//	{SILENCER, M950},
//	{SILENCER, MP5},
//	{SILENCER, MP5N},
//	{SILENCER, FIVE7},
//	{SILENCER, P7M8},
//	{SILENCER, MSG90A1},
//
//	{SNIPERSCOPE, COMMANDO},
//	{SNIPERSCOPE, SKS},
//	{SNIPERSCOPE, DRAGUNOV},
//	{SNIPERSCOPE, M24},
//	{SNIPERSCOPE, AUG},
//	{SNIPERSCOPE, G41},
//	{SNIPERSCOPE, MINI14},
//	{SNIPERSCOPE, C7},
//	{SNIPERSCOPE, FAMAS},
//	{SNIPERSCOPE, AK74},
//	{SNIPERSCOPE, AKM},
//	{SNIPERSCOPE, M14},
//	{SNIPERSCOPE, FNFAL},
//	{SNIPERSCOPE, G3A3},
//	{SNIPERSCOPE, G11},
//	{SNIPERSCOPE, M900},
//	{SNIPERSCOPE, BARRETT},
//	{SNIPERSCOPE, PSG},
//	{SNIPERSCOPE, VAL_SILENT},
//	{SNIPERSCOPE, TAR21},
//	{SNIPERSCOPE, ENFIELD},
//	{SNIPERSCOPE, G36K},
//	{SNIPERSCOPE, G36C},
//	{SNIPERSCOPE, MSG90A1},
//	{SNIPERSCOPE, AK103},
//
//	{LASERSCOPE, GLOCK_17},
//	{LASERSCOPE, GLOCK_18},
//	{LASERSCOPE, BERETTA_92F},
//	{LASERSCOPE, BERETTA_93R},
//	{LASERSCOPE, SW38},
//	{LASERSCOPE, BARRACUDA},
//	{LASERSCOPE, DESERTEAGLE},
//	{LASERSCOPE, M1911},
//	{LASERSCOPE, MP5K},
//	{LASERSCOPE, MAC10},
//	{LASERSCOPE, THOMPSON},
//	{LASERSCOPE, COMMANDO},
//	{LASERSCOPE, MP53},
//	{LASERSCOPE, AKSU74},
//	{LASERSCOPE, P90},
//	{LASERSCOPE, TYPE85},
//	{LASERSCOPE, SKS},
//	{LASERSCOPE, DRAGUNOV},
//	{LASERSCOPE, M24},
//	{LASERSCOPE, AUG},
//	{LASERSCOPE, G41},
//	{LASERSCOPE, MINI14},
//	{LASERSCOPE, C7},
//	{LASERSCOPE, FAMAS},
//	{LASERSCOPE, AK74},
//	{LASERSCOPE, AKM},
//	{LASERSCOPE, M14},
//	{LASERSCOPE, FNFAL},
//	{LASERSCOPE, G3A3},
//	{LASERSCOPE, G11},
//	{LASERSCOPE, M870},
//	{LASERSCOPE, SPAS15},
//	{LASERSCOPE, CAWS},
//	{LASERSCOPE, MINIMI},
//	{LASERSCOPE, RPK74},
//	{LASERSCOPE, HK21E},
//	{LASERSCOPE, AUTOMAG_III},
//	{LASERSCOPE, MICRO_UZI},
//	{LASERSCOPE, M900},
//	{LASERSCOPE, M950},
//	{LASERSCOPE, M960},
//	{LASERSCOPE, BARRETT},
//	{LASERSCOPE, PSG},
//	{LASERSCOPE, VAL_SILENT},
//	{LASERSCOPE, TAR21},
//	{LASERSCOPE, ENFIELD},
//	{LASERSCOPE, MP5},
//	{LASERSCOPE, MP5N},
//	{LASERSCOPE, UMP45},
//	{LASERSCOPE, MP5SD},
//	{LASERSCOPE, FIVE7},
//	{LASERSCOPE, P7M8},
//	{LASERSCOPE, G36K},
//	{LASERSCOPE, G36C},
//	{LASERSCOPE, MSG90A1},
//	{LASERSCOPE, BENNELLI},
//	{LASERSCOPE, AK103},
//
//	{BIPOD,	SKS},
//	{BIPOD,	DRAGUNOV},
//	{BIPOD,	M24},
//	{BIPOD,	AUG},
//	{BIPOD,	G41},
//	{BIPOD,	MINI14},
//	{BIPOD,	C7},
//	{BIPOD,	FAMAS},
//	{BIPOD,	AK74},
//	{BIPOD,	AKM},
//	{BIPOD,	M14},
//	{BIPOD,	FNFAL},
//	{BIPOD,	G3A3},
//	{BIPOD,	G11},
//	{BIPOD,	CAWS},
//	{BIPOD,	MINIMI},
//	{BIPOD,	RPK74},
//	{BIPOD,	HK21E},
//	{BIPOD, M900},
//	{BIPOD, BARRETT},
//	{BIPOD, PSG},
//	{BIPOD, VAL_SILENT},
//	{BIPOD, TAR21},
//	{BIPOD, ENFIELD},
//	{BIPOD, MSG90A1},
//	{BIPOD, AK103},
//
//	{DUCKBILL, M870},
//	{DUCKBILL, SPAS15},
//	{DUCKBILL, CAWS},
//	{DUCKBILL, BENNELLI},
//
//	{UNDER_GLAUNCHER, COMMANDO},
//	{UNDER_GLAUNCHER, AKSU74},
//	{UNDER_GLAUNCHER, AUG},
//	{UNDER_GLAUNCHER, G41},
//	{UNDER_GLAUNCHER, MINI14},
//	{UNDER_GLAUNCHER, C7},
//	{UNDER_GLAUNCHER, FAMAS},
//	{UNDER_GLAUNCHER, AK74},
//	{UNDER_GLAUNCHER, AKM},
//	{UNDER_GLAUNCHER, M14},
//	{UNDER_GLAUNCHER, FNFAL},
//	{UNDER_GLAUNCHER, G3A3},
//	{UNDER_GLAUNCHER, TAR21},
//	{UNDER_GLAUNCHER, ENFIELD},
//	{UNDER_GLAUNCHER, M900},
//	{UNDER_GLAUNCHER, G11},
//	{UNDER_GLAUNCHER, SKS},
//	{UNDER_GLAUNCHER, G36K},
//	{UNDER_GLAUNCHER, G36C},
//	{UNDER_GLAUNCHER, AK103},
//
//	{SPRING_AND_BOLT_UPGRADE, GLOCK_17},
//	{SPRING_AND_BOLT_UPGRADE, GLOCK_18},
//	{SPRING_AND_BOLT_UPGRADE, BERETTA_92F},
//	{SPRING_AND_BOLT_UPGRADE, BERETTA_93R},
//	{SPRING_AND_BOLT_UPGRADE, SW38},
//	{SPRING_AND_BOLT_UPGRADE, BARRACUDA},
//	{SPRING_AND_BOLT_UPGRADE, DESERTEAGLE},
//	{SPRING_AND_BOLT_UPGRADE, M1911},
//	{SPRING_AND_BOLT_UPGRADE, MP5K},
//	{SPRING_AND_BOLT_UPGRADE, MAC10},
//	{SPRING_AND_BOLT_UPGRADE, THOMPSON},
//	{SPRING_AND_BOLT_UPGRADE, COMMANDO},
//	{SPRING_AND_BOLT_UPGRADE, MP53},
//	{SPRING_AND_BOLT_UPGRADE, AKSU74},
//	{SPRING_AND_BOLT_UPGRADE, P90},
//	{SPRING_AND_BOLT_UPGRADE, TYPE85},
//	{SPRING_AND_BOLT_UPGRADE, SKS},
//	{SPRING_AND_BOLT_UPGRADE, DRAGUNOV},
//	{SPRING_AND_BOLT_UPGRADE, M24},
//	{SPRING_AND_BOLT_UPGRADE, AUG},
//	{SPRING_AND_BOLT_UPGRADE, G41},
//	{SPRING_AND_BOLT_UPGRADE, MINI14},
//	{SPRING_AND_BOLT_UPGRADE, C7},
//	{SPRING_AND_BOLT_UPGRADE, FAMAS},
//	{SPRING_AND_BOLT_UPGRADE, AK74},
//	{SPRING_AND_BOLT_UPGRADE, AKM},
//	{SPRING_AND_BOLT_UPGRADE, M14},
//	{SPRING_AND_BOLT_UPGRADE, FNFAL},
//	{SPRING_AND_BOLT_UPGRADE, G3A3},
//	{SPRING_AND_BOLT_UPGRADE, G11},
//	{SPRING_AND_BOLT_UPGRADE, M870},
//	{SPRING_AND_BOLT_UPGRADE, SPAS15},
//	{SPRING_AND_BOLT_UPGRADE, CAWS},
//	{SPRING_AND_BOLT_UPGRADE, MINIMI},
//	{SPRING_AND_BOLT_UPGRADE, RPK74},
//	{SPRING_AND_BOLT_UPGRADE, HK21E},
//	{SPRING_AND_BOLT_UPGRADE, AUTOMAG_III},
//	{SPRING_AND_BOLT_UPGRADE, MICRO_UZI},
//	{SPRING_AND_BOLT_UPGRADE, M900},
//	{SPRING_AND_BOLT_UPGRADE, M950},
//	{SPRING_AND_BOLT_UPGRADE, M960},
//	{SPRING_AND_BOLT_UPGRADE, BARRETT},
//	{SPRING_AND_BOLT_UPGRADE, PSG},
//	{SPRING_AND_BOLT_UPGRADE, VAL_SILENT},
//	{SPRING_AND_BOLT_UPGRADE, TAR21},
//	{SPRING_AND_BOLT_UPGRADE, ENFIELD},
//	{SPRING_AND_BOLT_UPGRADE, MP5},
//	{SPRING_AND_BOLT_UPGRADE, MP5N},
//	{SPRING_AND_BOLT_UPGRADE, UMP45},
//	{SPRING_AND_BOLT_UPGRADE, MP5SD},
//	{SPRING_AND_BOLT_UPGRADE, FIVE7},
//	{SPRING_AND_BOLT_UPGRADE, P7M8},
//	{SPRING_AND_BOLT_UPGRADE, G36K},
//	{SPRING_AND_BOLT_UPGRADE, G36C},
//	{SPRING_AND_BOLT_UPGRADE, MSG90A1},
//	{SPRING_AND_BOLT_UPGRADE, BENNELLI},
//	{SPRING_AND_BOLT_UPGRADE, AK103},
//
//	{GUN_BARREL_EXTENDER, GLOCK_17},
//	{GUN_BARREL_EXTENDER, GLOCK_18},
//	{GUN_BARREL_EXTENDER, BERETTA_92F},
//	{GUN_BARREL_EXTENDER, BERETTA_93R},
//	{GUN_BARREL_EXTENDER, SW38},
//	{GUN_BARREL_EXTENDER, BARRACUDA},
//	{GUN_BARREL_EXTENDER, DESERTEAGLE},
//	{GUN_BARREL_EXTENDER, M1911},
//	{GUN_BARREL_EXTENDER, MP5K},
//	{GUN_BARREL_EXTENDER, MAC10},
//	{GUN_BARREL_EXTENDER, THOMPSON},
//	{GUN_BARREL_EXTENDER, COMMANDO},
//	{GUN_BARREL_EXTENDER, MP53},
//	{GUN_BARREL_EXTENDER, AKSU74},
//	{GUN_BARREL_EXTENDER, P90},
//	{GUN_BARREL_EXTENDER, TYPE85},
//	{GUN_BARREL_EXTENDER, SKS},
//	{GUN_BARREL_EXTENDER, DRAGUNOV},
//	{GUN_BARREL_EXTENDER, M24},
//	{GUN_BARREL_EXTENDER, AUG},
//	{GUN_BARREL_EXTENDER, G41},
//	{GUN_BARREL_EXTENDER, MINI14},
//	{GUN_BARREL_EXTENDER, C7},
//	{GUN_BARREL_EXTENDER, FAMAS},
//	{GUN_BARREL_EXTENDER, AK74},
//	{GUN_BARREL_EXTENDER, AKM},
//	{GUN_BARREL_EXTENDER, M14},
//	{GUN_BARREL_EXTENDER, FNFAL},
//	{GUN_BARREL_EXTENDER, G3A3},
//	{GUN_BARREL_EXTENDER, G11},
//	{GUN_BARREL_EXTENDER, M870},
//	{GUN_BARREL_EXTENDER, SPAS15},
//	{GUN_BARREL_EXTENDER, CAWS},
//	{GUN_BARREL_EXTENDER, MINIMI},
//	{GUN_BARREL_EXTENDER, RPK74},
//	{GUN_BARREL_EXTENDER, HK21E},
//	{GUN_BARREL_EXTENDER, AUTOMAG_III},
//	{GUN_BARREL_EXTENDER, MICRO_UZI},
//	{GUN_BARREL_EXTENDER, M900},
//	{GUN_BARREL_EXTENDER, M950},
//	{GUN_BARREL_EXTENDER, M960},
//	{GUN_BARREL_EXTENDER, BARRETT},
//	{GUN_BARREL_EXTENDER, PSG},
//	{GUN_BARREL_EXTENDER, VAL_SILENT},
//	{GUN_BARREL_EXTENDER, TAR21},
//	{GUN_BARREL_EXTENDER, ENFIELD},
//	{GUN_BARREL_EXTENDER, MP5},
//	{GUN_BARREL_EXTENDER, MP5N},
//	{GUN_BARREL_EXTENDER, UMP45},
//	{GUN_BARREL_EXTENDER, MP5SD},
//	{GUN_BARREL_EXTENDER, FIVE7},
//	{GUN_BARREL_EXTENDER, P7M8},
//	{GUN_BARREL_EXTENDER, G36K},
//	{GUN_BARREL_EXTENDER, G36C},
//	{GUN_BARREL_EXTENDER, MSG90A1},
//	{GUN_BARREL_EXTENDER, BENNELLI},
//	{GUN_BARREL_EXTENDER, AK103},
//
//	{FLASH_SUPPRESSOR, MP5K},
//	{FLASH_SUPPRESSOR, MAC10},
//	{FLASH_SUPPRESSOR, THOMPSON},
//	{FLASH_SUPPRESSOR, COMMANDO},
//	{FLASH_SUPPRESSOR, MP53},
//	{FLASH_SUPPRESSOR, AKSU74},
//	{FLASH_SUPPRESSOR, P90},
//	{FLASH_SUPPRESSOR, TYPE85},
//	{FLASH_SUPPRESSOR, SKS},
//	{FLASH_SUPPRESSOR, DRAGUNOV},
//	{FLASH_SUPPRESSOR, M24},
//	{FLASH_SUPPRESSOR, AUG},
//	{FLASH_SUPPRESSOR, G41},
//	{FLASH_SUPPRESSOR, MINI14},
//	{FLASH_SUPPRESSOR, C7},
//	{FLASH_SUPPRESSOR, FAMAS},
//	{FLASH_SUPPRESSOR, AK74},
//	{FLASH_SUPPRESSOR, AKM},
//	{FLASH_SUPPRESSOR, M14},
//	{FLASH_SUPPRESSOR, FNFAL},
//	{FLASH_SUPPRESSOR, G3A3},
//	{FLASH_SUPPRESSOR, G11},
//	{FLASH_SUPPRESSOR, MINIMI},
//	{FLASH_SUPPRESSOR, RPK74},
//	{FLASH_SUPPRESSOR, HK21E},
//	{FLASH_SUPPRESSOR, MICRO_UZI},
//	{FLASH_SUPPRESSOR, M900},
//	{FLASH_SUPPRESSOR, M950},
//	{FLASH_SUPPRESSOR, M960},
//	{FLASH_SUPPRESSOR, BARRETT},
//	{FLASH_SUPPRESSOR, PSG},
//	{FLASH_SUPPRESSOR, VAL_SILENT},
//	{FLASH_SUPPRESSOR, TAR21},
//	{FLASH_SUPPRESSOR, ENFIELD},
//	{FLASH_SUPPRESSOR, MP5},
//	{FLASH_SUPPRESSOR, MP5N},
//	{FLASH_SUPPRESSOR, UMP45},
//	{FLASH_SUPPRESSOR, MP5SD},
//	{FLASH_SUPPRESSOR, G36K},
//	{FLASH_SUPPRESSOR, G36C},
//	{FLASH_SUPPRESSOR, MSG90A1},
//	{FLASH_SUPPRESSOR, AK103},
//
//	{REFLEX_SCOPED, GLOCK_17},
//	{REFLEX_SCOPED, GLOCK_18},
//	{REFLEX_SCOPED, BERETTA_92F},
//	{REFLEX_SCOPED, BERETTA_93R},
//	{REFLEX_SCOPED, SW38},
//	{REFLEX_SCOPED, BARRACUDA},
//	{REFLEX_SCOPED, DESERTEAGLE},
//	{REFLEX_SCOPED, M1911},
//	{REFLEX_SCOPED, MP5K},
//	{REFLEX_SCOPED, MAC10},
//	{REFLEX_SCOPED, THOMPSON},
//	{REFLEX_SCOPED, COMMANDO},
//	{REFLEX_SCOPED, MP53},
//	{REFLEX_SCOPED, AKSU74},
//	{REFLEX_SCOPED, P90},
//	{REFLEX_SCOPED, TYPE85},
//	{REFLEX_SCOPED, SKS},
//	{REFLEX_SCOPED, AUG},
//	{REFLEX_SCOPED, G41},
//	{REFLEX_SCOPED, MINI14},
//	{REFLEX_SCOPED, C7},
//	{REFLEX_SCOPED, FAMAS},
//	{REFLEX_SCOPED, AK74},
//	{REFLEX_SCOPED, AKM},
//	{REFLEX_SCOPED, M14},
//	{REFLEX_SCOPED, FNFAL},
//	{REFLEX_SCOPED, G3A3},
//	{REFLEX_SCOPED, G11},
//	{REFLEX_SCOPED, M870},
//	{REFLEX_SCOPED, SPAS15},
//	{REFLEX_SCOPED, CAWS},
//	{REFLEX_SCOPED, MINIMI},
//	{REFLEX_SCOPED, RPK74},
//	{REFLEX_SCOPED, HK21E},
//	{REFLEX_SCOPED, AUTOMAG_III},
//	{REFLEX_SCOPED, MICRO_UZI},
//	{REFLEX_SCOPED, M900},
//	{REFLEX_SCOPED, M950},
//	{REFLEX_SCOPED, M960},
//	{REFLEX_SCOPED, TAR21},
//	{REFLEX_SCOPED, ENFIELD},
//	{REFLEX_SCOPED, MP5},
//	{REFLEX_SCOPED, MP5N},
//	{REFLEX_SCOPED, UMP45},
//	{REFLEX_SCOPED, MP5SD},
//	{REFLEX_SCOPED, FIVE7},
//	{REFLEX_SCOPED, P7M8},
//	{REFLEX_SCOPED, G36K},
//	{REFLEX_SCOPED, G36C},
//	{REFLEX_SCOPED, BENNELLI},
//	{REFLEX_SCOPED, AK103},
//
//	{REFLEX_UNSCOPED, GLOCK_17},
//	{REFLEX_UNSCOPED, GLOCK_18},
//	{REFLEX_UNSCOPED, BERETTA_92F},
//	{REFLEX_UNSCOPED, BERETTA_93R},
//	{REFLEX_UNSCOPED, SW38},
//	{REFLEX_UNSCOPED, BARRACUDA},
//	{REFLEX_UNSCOPED, DESERTEAGLE},
//	{REFLEX_UNSCOPED, M1911},
//	{REFLEX_UNSCOPED, MP5K},
//	{REFLEX_UNSCOPED, MAC10},
//	{REFLEX_UNSCOPED, THOMPSON},
//	{REFLEX_UNSCOPED, COMMANDO},
//	{REFLEX_UNSCOPED, MP53},
//	{REFLEX_UNSCOPED, AKSU74},
//	{REFLEX_UNSCOPED, P90},
//	{REFLEX_UNSCOPED, TYPE85},
//	{REFLEX_UNSCOPED, SKS},
//	{REFLEX_UNSCOPED, DRAGUNOV},
//	{REFLEX_UNSCOPED, M24},
//	{REFLEX_UNSCOPED, AUG},
//	{REFLEX_UNSCOPED, G41},
//	{REFLEX_UNSCOPED, MINI14},
//	{REFLEX_UNSCOPED, C7},
//	{REFLEX_UNSCOPED, FAMAS},
//	{REFLEX_UNSCOPED, AK74},
//	{REFLEX_UNSCOPED, AKM},
//	{REFLEX_UNSCOPED, M14},
//	{REFLEX_UNSCOPED, FNFAL},
//	{REFLEX_UNSCOPED, G3A3},
//	{REFLEX_UNSCOPED, G11},
//	{REFLEX_UNSCOPED, M870},
//	{REFLEX_UNSCOPED, SPAS15},
//	{REFLEX_UNSCOPED, CAWS},
//	{REFLEX_UNSCOPED, MINIMI},
//	{REFLEX_UNSCOPED, RPK74},
//	{REFLEX_UNSCOPED, HK21E},
//	{REFLEX_UNSCOPED, AUTOMAG_III},
//	{REFLEX_UNSCOPED, MICRO_UZI},
//	{REFLEX_UNSCOPED, M900},
//	{REFLEX_UNSCOPED, M950},
//	{REFLEX_UNSCOPED, M960},
//	{REFLEX_UNSCOPED, BARRETT},
//	{REFLEX_UNSCOPED, PSG},
//	{REFLEX_UNSCOPED, VAL_SILENT},
//	{REFLEX_UNSCOPED, TAR21},
//	{REFLEX_UNSCOPED, ENFIELD},
//	{REFLEX_UNSCOPED, MP5},
//	{REFLEX_UNSCOPED, MP5N},
//	{REFLEX_UNSCOPED, UMP45},
//	{REFLEX_UNSCOPED, MP5SD},
//	{REFLEX_UNSCOPED, FIVE7},
//	{REFLEX_UNSCOPED, P7M8},
//	{REFLEX_UNSCOPED, G36K},
//	{REFLEX_UNSCOPED, G36C},
//	{REFLEX_UNSCOPED, MSG90A1},
//	{REFLEX_UNSCOPED, BENNELLI},
//	{REFLEX_UNSCOPED, AK103},
//
//	{DETONATOR, TNT},
//	{DETONATOR, HMX},
//	{DETONATOR, C1},
//	{DETONATOR, C4},
//
//	{REMDETONATOR, TNT},
//	{REMDETONATOR, HMX},
//	{REMDETONATOR, C1},
//	{REMDETONATOR, C4},
//
//	{CERAMIC_PLATES, FLAK_JACKET},
//	{CERAMIC_PLATES, FLAK_JACKET_18},
//	{CERAMIC_PLATES, FLAK_JACKET_Y},
//	{CERAMIC_PLATES, KEVLAR_VEST},
//	{CERAMIC_PLATES, KEVLAR_VEST_18},
//	{CERAMIC_PLATES, KEVLAR_VEST_Y},
//	{CERAMIC_PLATES, SPECTRA_VEST},
//	{CERAMIC_PLATES, SPECTRA_VEST_18},
//	{CERAMIC_PLATES, SPECTRA_VEST_Y},
//	{CERAMIC_PLATES, KEVLAR2_VEST},
//	{CERAMIC_PLATES, KEVLAR2_VEST_18},
//	{CERAMIC_PLATES, KEVLAR2_VEST_Y},
//
//	{SPRING,					ALUMINUM_ROD},
//	{QUICK_GLUE,			STEEL_ROD},
//	{DUCT_TAPE,				STEEL_ROD},
//	{XRAY_BULB,				FUMBLE_PAK},
//	{CHEWING_GUM,			FUMBLE_PAK},
//	{BATTERIES,				XRAY_DEVICE},
//	{COPPER_WIRE,			LAME_BOY},
//	{COPPER_WIRE,			GOLDWATCH},
//	{0, 0}
//};

UINT16 Launchable[MAXITEMS+1][2];// =
//{
//	{GL_HE_GRENADE, GLAUNCHER},
//	{GL_HE_GRENADE, UNDER_GLAUNCHER},
//	{GL_TEARGAS_GRENADE, GLAUNCHER},
//	{GL_TEARGAS_GRENADE, UNDER_GLAUNCHER},
//	{GL_STUN_GRENADE, GLAUNCHER},
//	{GL_STUN_GRENADE, UNDER_GLAUNCHER},
//	{GL_SMOKE_GRENADE, GLAUNCHER},
//	{GL_SMOKE_GRENADE, UNDER_GLAUNCHER},
//	{MORTAR_SHELL, MORTAR},
//	{TANK_SHELL, TANK_CANNON},
//	{RPG_HE_ROCKET, RPG7},
//	{RPG_AP_ROCKET, RPG7},
//	{RPG_FRAG_ROCKET, RPG7},
//	{0, 0}
//};

UINT16 CompatibleFaceItems[MAXITEMS+1][2];// =
//{
//	{EXTENDEDEAR,			NIGHTGOGGLES},
//	{EXTENDEDEAR,			UVGOGGLES},
//	{EXTENDEDEAR,			SUNGOGGLES},
//	{EXTENDEDEAR,			GASMASK},
//	{EXTENDEDEAR,			NOTHING},
//	{WALKMAN,					NIGHTGOGGLES},
//	{WALKMAN,					UVGOGGLES},
//	{WALKMAN,					SUNGOGGLES},
//	{WALKMAN,					GASMASK},
//	{WALKMAN,					NOTHING},
//
//	{NIGHTGOGGLES,		EXTENDEDEAR},
//	{NIGHTGOGGLES,		WALKMAN},
//	{NIGHTGOGGLES,			ROBOT_REMOTE_CONTROL},
//	{NIGHTGOGGLES,		GASMASK},
//	{NIGHTGOGGLES,		NOTHING},
//	{SUNGOGGLES,			EXTENDEDEAR},
//	{SUNGOGGLES,			WALKMAN},
//	{SUNGOGGLES,			GASMASK},
//	{SUNGOGGLES,			ROBOT_REMOTE_CONTROL},
//	{SUNGOGGLES,			NOTHING},
//	{UVGOGGLES,				EXTENDEDEAR},
//	{UVGOGGLES,				WALKMAN},
//	{UVGOGGLES,				GASMASK},
//	{UVGOGGLES,			ROBOT_REMOTE_CONTROL},
//	{UVGOGGLES,				NOTHING},
//	{GASMASK,				EXTENDEDEAR},
//	{GASMASK,				WALKMAN},
//	{GASMASK,			NIGHTGOGGLES},
//	{GASMASK,			UVGOGGLES},
//	{GASMASK,			SUNGOGGLES},
//	{GASMASK,			ROBOT_REMOTE_CONTROL},
//	{GASMASK,					NOTHING},
//
//	{ROBOT_REMOTE_CONTROL,			NIGHTGOGGLES},
//	{ROBOT_REMOTE_CONTROL,			UVGOGGLES},
//	{ROBOT_REMOTE_CONTROL,			SUNGOGGLES},
//	{ROBOT_REMOTE_CONTROL,			GASMASK},
//	{ROBOT_REMOTE_CONTROL, NOTHING},
//	{0,								0},
//};


UINT16 Merge[MAXITEMS+1][6];// =
//{ // first item			second item						resulting item,					merge type
//	{FIRSTAIDKIT,			FIRSTAIDKIT,					FIRSTAIDKIT,						COMBINE_POINTS},
//	{MEDICKIT,				MEDICKIT,							MEDICKIT,								COMBINE_POINTS},
//	{CANTEEN,				CANTEEN,							CANTEEN,								COMBINE_POINTS},// Madd Combine canteens
//	{LOCKSMITHKIT,		LOCKSMITHKIT,					LOCKSMITHKIT,						COMBINE_POINTS},
//	{TOOLKIT,					TOOLKIT,							TOOLKIT,								COMBINE_POINTS},
//	{GAS_CAN,					GAS_CAN,							GAS_CAN,								COMBINE_POINTS},
//	{CAMOUFLAGEKIT,		CAMOUFLAGEKIT,				CAMOUFLAGEKIT,					COMBINE_POINTS},
//	{BEER,						BEER,									BEER,										COMBINE_POINTS},
//	{WINE,						WINE,									WINE,										COMBINE_POINTS},
//	{ALCOHOL,					ALCOHOL,							ALCOHOL,								COMBINE_POINTS},
//
//	{COMPOUND18,			FLAK_JACKET,							FLAK_JACKET_18,							TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR_VEST,							KEVLAR_VEST_18,							TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR2_VEST,							KEVLAR2_VEST_18,						TREAT_ARMOUR},
//	{COMPOUND18,			SPECTRA_VEST,							SPECTRA_VEST_18,						TREAT_ARMOUR},
//	{COMPOUND18,			LEATHER_JACKET_W_KEVLAR,	LEATHER_JACKET_W_KEVLAR_18,	TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR_LEGGINGS,					KEVLAR_LEGGINGS_18,					TREAT_ARMOUR},
//	{COMPOUND18,			SPECTRA_LEGGINGS,					SPECTRA_LEGGINGS_18,				TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR_HELMET,						KEVLAR_HELMET_18,						TREAT_ARMOUR},
//	{COMPOUND18,			SPECTRA_HELMET,						SPECTRA_HELMET_18,					TREAT_ARMOUR},
//	{COMPOUND18,			FLAK_JACKET_Y,						NOTHING,										DESTRUCTION},
//	{COMPOUND18,			KEVLAR_VEST_Y,						NOTHING,										DESTRUCTION},
//	{COMPOUND18,			SPECTRA_VEST_Y,						NOTHING,										DESTRUCTION},
//	{COMPOUND18,			LEATHER_JACKET_W_KEVLAR_Y,NOTHING,										DESTRUCTION},
//	{COMPOUND18,			KEVLAR_LEGGINGS_Y,				NOTHING,										DESTRUCTION},
//	{COMPOUND18,			SPECTRA_LEGGINGS_Y,				NOTHING,										DESTRUCTION},
//	{COMPOUND18,			KEVLAR_HELMET_Y,					NOTHING,										DESTRUCTION},
//	{COMPOUND18,			SPECTRA_HELMET_Y,					NOTHING,										DESTRUCTION},
//
//	{JAR_QUEEN_CREATURE_BLOOD,	FLAK_JACKET,					FLAK_JACKET_Y,					TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_VEST,					KEVLAR_VEST_Y,					TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_VEST,					SPECTRA_VEST_Y,					TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	LEATHER_JACKET_W_KEVLAR,	LEATHER_JACKET_W_KEVLAR_Y,	TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR2_VEST,					KEVLAR2_VEST_Y,				TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_LEGGINGS,			KEVLAR_LEGGINGS_Y,			TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_LEGGINGS,			SPECTRA_LEGGINGS_Y,			TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_HELMET,				KEVLAR_HELMET_Y,				TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_HELMET,				SPECTRA_HELMET_Y,				TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	FLAK_JACKET_18,				NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_VEST_18,				NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR2_VEST_18,			NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_VEST_18,			NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	LEATHER_JACKET_W_KEVLAR_18,	NOTHING,					DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_LEGGINGS_18,		NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_LEGGINGS_18,	NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_HELMET_18,			NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_HELMET_18,		NOTHING,								DESTRUCTION},
//
//	{RDX,							TNT,									HMX,										EXPLOSIVE},
//	{RDX,							C1,										C4,											EXPLOSIVE},
//	{TNT,							RDX,									HMX,										EXPLOSIVE},
//	{C1,							RDX,									C4,											EXPLOSIVE},
//
//	{STRING,					TIN_CAN,							STRING_TIED_TO_TIN_CAN,	EASY_MERGE},
//	{TIN_CAN,					STRING,								STRING_TIED_TO_TIN_CAN,	EASY_MERGE},
//
//	{FLASH_DEVICE,		DISPLAY_UNIT,					XRAY_DEVICE,						ELECTRONIC_MERGE},
//	{DISPLAY_UNIT,		FLASH_DEVICE,					XRAY_DEVICE,						ELECTRONIC_MERGE},
//
//	{0, 0, 0, 0}
//};


ComboMergeInfoStruct AttachmentComboMerge[MAXITEMS+1];// =
//{
//	// base item							attach 1								attach 2						 result
//	{ALUMINUM_ROD,						{SPRING,								NOTHING},						SPRING_AND_BOLT_UPGRADE	},
//	{STEEL_ROD,								{QUICK_GLUE,						DUCT_TAPE},					GUN_BARREL_EXTENDER },
//	{FUMBLE_PAK,							{XRAY_BULB,							CHEWING_GUM},				FLASH_DEVICE },
//	{LAME_BOY,								{COPPER_WIRE,						NOTHING},						DISPLAY_UNIT },
//	{GOLDWATCH,						{COPPER_WIRE,								NOTHING},						DETONATOR},
//	{NOTHING,									{NOTHING,								NOTHING},						NOTHING },
//};


UINT16 IncompatibleAttachments[MAXATTACHMENTS][2];// =
//{
//	{BIPOD,UNDER_GLAUNCHER},
//	{UNDER_GLAUNCHER,BIPOD},
//	{DETONATOR,REMDETONATOR},
//	{REMDETONATOR,DETONATOR},
//	{SNIPERSCOPE,REFLEX_SCOPED},
//	{REFLEX_SCOPED,SNIPERSCOPE},
//	{REFLEX_SCOPED,REFLEX_UNSCOPED},
//	{REFLEX_UNSCOPED,REFLEX_SCOPED},
//	{SILENCER,FLASH_SUPPRESSOR},
//	{FLASH_SUPPRESSOR,SILENCER},
//	{LASERSCOPE,REFLEX_UNSCOPED},
//	{REFLEX_UNSCOPED,LASERSCOPE},
//};

UINT16 ReplacementGuns[][2] =
{
	{ BARRACUDA,		DESERTEAGLE	},
	{ M1911,				GLOCK_17		},
	{ GLOCK_18,			BERETTA_93R	},
	{ BERETTA_92F,	GLOCK_17		},
	{ TYPE85,				BERETTA_93R	},
	{	THOMPSON,			MP5K				},
	{	MP53,					MP5K				},
	{ SPAS15,				M870				},
	{ AKSU74,				MAC10				},
	{ SKS,					MINI14			},
	{	AKM,					G41					},
	{ G3A3,					G41					},
	{ AK74,					G41					},
	{	DRAGUNOV,			M24					},
	{ FAMAS,				M14					},
	{ AUG,					C7					},
	{ RPK74,				MINIMI			},
	{ HK21E,				MINIMI			},
	{ 0,						0						}
};

UINT16 ReplacementAmmo[][2] =
{
	{ CLIP545_30_AP,		CLIP556_30_AP  },
	{ CLIP545_30_HP,		CLIP556_30_HP  },
	{ CLIP762W_10_AP,		CLIP762N_5_AP  },
	{ CLIP762W_30_AP,		CLIP762N_20_AP },
	{ CLIP762W_10_HP,		CLIP762N_5_HP  },
	{ CLIP762W_30_HP,		CLIP762N_20_HP },
	{ 0,								0							 }
};

// CHRISL: Structure Definitions for new inventory system items.
std::vector<LBETYPE> LoadBearingEquipment;
//LBETYPE LoadBearingEquipment[MAXITEMS+1];
//LBETYPE LoadBearingEquipment[] =
//{
//	//	Index	Class						Pocket Types---------------------------------
//	{	0,		/*Blank Entry*/			0,	{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	1,		/*Default Thigh Pack*/	0,	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	2,		/*Default Vest Pack*/	1,	{1,	1,	0,	0,	0,	0,	0,	0,	2,	0,	0,	0} },
//	{	3,		/*Default Combat Pack*/	2,	{1,	1,	0,	0,	2,	0,	0,	0,	0,	0,	0,	0} },
//	{	4,		/*Default Back Pack*/	3,	{1,	1,	0,	0,	0,	0,	0,	0,	3,	3,	0,	0} },
//	{	5,		/*6P Backpack*/			3,	{1,	1,	1,	1,	0,	0,	0,	0,	3,	3,	0,	0} },
//	{	6,		/*6P Combat Pack*/		2,	{1,	1,	1,	1,	2,	2,	0,	0,	0,	0,	0,	0} }
//};

std::vector<POCKETTYPE> LBEPocketType;
//TODO make the indices of this a define, because I do not know what is a large pocket (I guess 3)

//POCKETTYPE LBEPocketType[MAXITEMS+1]; //= 
//{
//	{	/* Blank Entry */			0,	0,	0,	{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Small General Pocket */	1,	0,	0,	{4,	3,	2,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Med General Pocket */	2,	0,	0,	{7,	6,	5,	4,	3,	2,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Lg General Pocket */		3,	0,	0,	{10,9,	8,	7,	6,	5,	4,	3,	2,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Gun Sling */				4,	0,	1,	{0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Knife Pocket */			5,	0,	1,	{0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} }
//};

BOOLEAN ItemIsLegal( UINT16 usItemIndex, BOOLEAN fIgnoreCoolness )
{
	if ( Item[usItemIndex].ubCoolness == 0 && !fIgnoreCoolness )
		return FALSE;

	//if the user has selected the reduced gun list
	if( !gGameOptions.fGunNut )
	{
		//if the item is a gun, or ammo
		//if( (Item[ usItemIndex ].usItemClass == IC_GUN) || (Item[ usItemIndex ].usItemClass == IC_AMMO )) //Madd: restriction removed
		{
			// and the item is only available with the extended guns
			if( Item[usItemIndex].biggunlist )
			{
				return(FALSE);
			}
		}
	}

	if ( gGameOptions.ubGameStyle != STYLE_SCIFI && Item[usItemIndex].scifi )
	{
		return FALSE;
	}

	// CHRISL: Restrict system specific items
	if( (UsingNewInventorySystem() == true) )
	{
		if(Item[usItemIndex].ItemSize == 99)
			return FALSE;
	}
	else
	{
		if(Item[usItemIndex].usItemClass == IC_LBEGEAR || Item[usItemIndex].newinv)
			return FALSE;
	}

	if(UsingNewAttachmentSystem()==true && Item[usItemIndex].ubAttachmentSystem == 1){
		return FALSE;
	} else if(UsingNewAttachmentSystem()==false && Item[usItemIndex].ubAttachmentSystem == 2){
		return FALSE;
	}
	return(TRUE);
}

// also used for ammo
BOOLEAN ExtendedGunListGun( UINT16 usGun )
{
//	return( (Item[ usGun ].fFlags & ITEM_BIGGUNLIST) != 0 );
	return( (Item[ usGun ].biggunlist ) != 0 );
}

UINT16 StandardGunListReplacement( UINT16 usGun )
{
	UINT8 ubLoop;

	if ( ExtendedGunListGun( usGun ) )
	{
		ubLoop = 0;
		while ( ReplacementGuns[ ubLoop ][ 0 ] != 0 )
		{
			if ( ReplacementGuns[ ubLoop ][ 0 ] == usGun )
			{
				return( ReplacementGuns[ ubLoop ][ 1 ] );
			}
			ubLoop++;
		}
		// ERROR!
		AssertMsg( 0, String( "Extended gun with no replacement %d, CC:0", usGun ) );
		return( NOTHING );
	}
	else
	{
		return( NOTHING );
	}
}

UINT16 StandardGunListAmmoReplacement( UINT16 usAmmo )
{
	UINT8 ubLoop;

	if ( ExtendedGunListGun( usAmmo ) )
	{
		ubLoop = 0;
		while ( ReplacementAmmo[ ubLoop ][ 0 ] != 0 )
		{
			if ( ReplacementAmmo[ ubLoop ][ 0 ] == usAmmo )
			{
				return( ReplacementAmmo[ ubLoop ][ 1 ] );
			}
			ubLoop++;
		}
		// ERROR!
		AssertMsg( 0, String( "Extended gun with no replacement %d, CC:0", usAmmo ) );
		return( NOTHING );
	}
	else
	{
		return( NOTHING );
	}
}

BOOLEAN WeaponInHand( SOLDIERTYPE * pSoldier )
{
	if ( Item[pSoldier->inv[HANDPOS].usItem].usItemClass & (IC_WEAPON | IC_THROWN) && pSoldier->inv[HANDPOS].exists() == true)
	{
		if (Item[pSoldier->inv[HANDPOS].usItem].fingerprintid )
		{
			if (pSoldier->inv[HANDPOS][0]->data.ubImprintID != NO_PROFILE)
			{
				if (pSoldier->ubProfile != NO_PROFILE && pSoldier->ubProfile != MADLAB )
				{
					if (pSoldier->inv[HANDPOS][0]->data.ubImprintID != pSoldier->ubProfile)
					{
						return( FALSE );
					}
				}
				else
				{
					if (pSoldier->inv[HANDPOS][0]->data.ubImprintID != (NO_PROFILE + 1) )
					{
						return( FALSE );
					}
				}
			}
		}
		if (pSoldier->inv[HANDPOS][0]->data.gun.bGunStatus >= USABLE)
		{
			return( TRUE );
		}
	}
	// return -1 or some "broken" value if weapon is broken?
	return( FALSE );
}

bool FitsInSmallPocket(OBJECTTYPE* pObj)
{
	if (UsingNewInventorySystem() == true) {
		return true;
	}
	return Item[pObj->usItem].ubPerPocket != 0;
}

bool IsBackpackSlot(INT8 bSlot)
{
	std::vector<INT8> pocketKey;
	GetLBESlots(BPACKPOCKPOS, pocketKey);

	for(UINT32 loop = 0; loop < pocketKey.size(); loop++)
	{
		if(pocketKey[loop] == bSlot)
			return true;
	}

	return false;
}

// CHRISL: New definition for this function so that we can look at soldiers LBE pockets.
UINT8 ItemSlotLimit( OBJECTTYPE * pObject, INT16 bSlot, SOLDIERTYPE *pSoldier, BOOLEAN cntAttach )
{
	UINT8	ubSlotLimit;
	UINT8	pIndex;
	UINT16	usItem, iSize;
	UINT16	sSize = 0;

	//doesn't matter what inventory method we are using
	usItem = pObject->usItem;
	
	// WANNE: This is the problem, why silver nuggets don't stuck, because it is not a MONEY item!!
	// We have to check the item class (IC_MONEY) and not the item index!!!
	//if (usItem == MONEY) 
	//{
	//	//need to have money "stackable" in all slots to trick it into merging
	//	return 2;
	//}

	// WANNE: This fixes the stacking problem of silver nuggets!!
	if (Item[usItem].usItemClass == IC_MONEY)
	{
		//need to have money "stackable" in all slots to trick it into merging
		return 2;
	}

	//doesn't matter what inventory method we are using, "body" slots always have a capacity of 1
	if(bSlot < BODYPOSFINAL)
		return 1;

	ubSlotLimit = Item[usItem].ubPerPocket;
	if ( ubSlotLimit > MAX_OBJECTS_PER_SLOT ) {
		ubSlotLimit = MAX_OBJECTS_PER_SLOT;
	}
	if (bSlot == STACK_SIZE_LIMIT) {
		//if it is stack size limit we want it to be a big slot or a vehicle slot
		if (UsingNewInventorySystem() == false)
			return (max(1, ubSlotLimit));
		else if(pSoldier != NULL && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
			return (max(1, LBEPocketType[VEHICLE_POCKET_TYPE].ItemCapacityPerSize[__min(gGameExternalOptions.guiMaxItemSize,Item[pObject->usItem].ItemSize)])); //JMich
		else
			return (max(1, min(255,LBEPocketType[VEHICLE_POCKET_TYPE].ItemCapacityPerSize[__min(34,Item[pObject->usItem].ItemSize)]*4)));
	}

	if (UsingNewInventorySystem() == false) {
		if (ubSlotLimit == 0 && bSlot < BIGPOCKFINAL) {
			return 1;
		}
		if (bSlot >= BIGPOCKFINAL && ubSlotLimit > 1)
		{
			ubSlotLimit /= 2;
		}
		return( ubSlotLimit );
	}


	//UsingNewInventorySystem == true
	if (pSoldier != NULL && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE)) {
		pIndex = VEHICLE_POCKET_TYPE;
	}
	// IC Group Slots
	else if (bSlot == GUNSLINGPOCKPOS) {
		pIndex = GUNSLING_POCKET_TYPE;
	}
	else if (bSlot == KNIFEPOCKPOS) {
		pIndex = KNIFE_POCKET_TYPE;
	}
	else {
		Assert(icLBE[bSlot] != -1 && icDefault[bSlot] != -1 && icPocket[bSlot] != -1);
		//find the class of the LBE, then find what size the pocket of the slot in the LBE is
		if (pSoldier == NULL || pSoldier->inv[icLBE[bSlot]].exists() == false) {
			pIndex = LoadBearingEquipment[Item[icDefault[bSlot]].ubClassIndex].lbePocketIndex[icPocket[bSlot]];
		}
		else {
			pIndex = LoadBearingEquipment[Item[pSoldier->inv[icLBE[bSlot]].usItem].ubClassIndex].lbePocketIndex[icPocket[bSlot]];
		}
	}

	//We need to actually check the size of the largest stored item as well as the size of the current item
	if(cntAttach == TRUE)
	{
		iSize = CalculateItemSize(pObject);
		if(pSoldier != NULL && pSoldier->inv[bSlot].usItem == pObject->usItem)
		{
			sSize = CalculateItemSize(&pSoldier->inv[bSlot]);
			if(LBEPocketType[pIndex].ItemCapacityPerSize[sSize] < LBEPocketType[pIndex].ItemCapacityPerSize[iSize])
				iSize = sSize;
		}
	}
	else
		iSize = Item[pObject->usItem].ItemSize;
	iSize = __min(iSize,34);
	ubSlotLimit = LBEPocketType[pIndex].ItemCapacityPerSize[iSize];

	//this could be changed, we know guns are physically able to stack
	//if ( iSize < 10 && ubSlotLimit > 1)
	//	ubSlotLimit = 1;

	if(LBEPocketType[pIndex].pRestriction != 0 && !(LBEPocketType[pIndex].pRestriction & Item[usItem].usItemClass)) {
		return 0;
	}

	return( ubSlotLimit );
}

UINT32 MoneySlotLimit( INT8 bSlot )
{
	if ( bSlot >= BIGPOCKFINAL )	/* CHRISL */
	{
		return( MAX_MONEY_PER_SLOT / 2 );
	}
	else
	{
		return( MAX_MONEY_PER_SLOT );
	}
}

INT8 FindBestWeaponIfCurrentIsOutOfRange(SOLDIERTYPE * pSoldier, INT8 bCurrentWeaponIndex, UINT16 bWantedRange)
{
	//assuming current weapon is in the handpos
	if (GunRange(&pSoldier->inv[bCurrentWeaponIndex], pSoldier) >= bWantedRange)  // SANDRO - added argument
	{
		//our current weapon is good enough
		return( bCurrentWeaponIndex );
	}

	UINT16 range;
	UINT16 bestRange = 0;
	INT8 bestWeaponThatMeetsRange = 0;
	INT8 secondBestWeapon = 0;
	//search for weapons that meet the range, then sort by damage.
	//if there are no weapons that meet the range, then use the longest range we can find
	for (INT8 bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		//if this is a weapon
		if (Item[pSoldier->inv[bLoop].usItem].usItemClass & (IC_WEAPON | IC_THROWN) && pSoldier->inv[bLoop].exists() == true)
		{
			range = GunRange(&pSoldier->inv[bLoop], pSoldier);  // SANDRO - added argument
			if (range >= bWantedRange)
			{
				if (bestWeaponThatMeetsRange == 0)
				{
					bestWeaponThatMeetsRange = bLoop;
				}
				else if (GetDamage(&pSoldier->inv[bLoop]) > GetDamage(&pSoldier->inv[bestWeaponThatMeetsRange]))
				{
					//does this weapon have more damage?
					bestWeaponThatMeetsRange = bLoop;
				}
			}
			else if (range > bestRange)
			{
				//weapon does not meet range, but it could be better anyways
				bestRange = range;
				secondBestWeapon = bLoop;
			}
			else if (range == bestRange)
			{
				//weapon ties with secondBestWeapon's range
				if (secondBestWeapon == 0)
				{
					//this if can happen if range of bLoop is 0!
					secondBestWeapon = bLoop;
				}
				else if (GetDamage(&pSoldier->inv[bLoop]) > GetDamage(&pSoldier->inv[secondBestWeapon]))
				{
					secondBestWeapon = bLoop;
				}
			}
		}
	}

	if (bestWeaponThatMeetsRange)
	{
		return bestWeaponThatMeetsRange;
	}
	else if (secondBestWeapon)
	{
		return secondBestWeapon;
	}
	return( bCurrentWeaponIndex );
}

INT8 FindMetalDetector( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].metaldetector && pSoldier->inv[bLoop].exists() == true)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindLockBomb( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].lockbomb && pSoldier->inv[bLoop].exists() == true)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindUsableObj( SOLDIERTYPE * pSoldier, UINT16 usItem )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if ( pSoldier->inv[bLoop].usItem == usItem
			&& pSoldier->inv[bLoop].exists() == true
			&& pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}


INT8 FindObjExcludingSlot( SOLDIERTYPE * pSoldier, UINT16 usItem, INT8 bExcludeSlot )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (bLoop == bExcludeSlot)
		{
			continue;
		}
		if (pSoldier->inv[bLoop].usItem == usItem && pSoldier->inv[bLoop].exists() == true)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindObj( SOLDIERTYPE * pSoldier, UINT16 usItem, INT8 bLower, INT8 bUpper )
{
	INT8	bLoop;

	for (bLoop = bLower; bLoop < bUpper; bLoop++)
	{
		//CHRISL: If in NIV, in combat and backpack is closed, don't look inside
		if(UsingNewAttachmentSystem() == true && (gTacticalStatus.uiFlags & INCOMBAT) && IsBackpackSlot(bLoop) == true && pSoldier->flags.ZipperFlag == FALSE)
			continue;

		//CHRISL: If we check exists() then we can't search for an empty pocket with this function, which is done.
		if (pSoldier->inv[bLoop].usItem == usItem/* && pSoldier->inv[bLoop].exists() == true*/)
		{
			return( bLoop );
		}
	}
	return( ITEM_NOT_FOUND );
}

INT8 FindObjInObjRange( SOLDIERTYPE * pSoldier, UINT16 usItem1, UINT16 usItem2 )
{
	INT8		bLoop;
	UINT16	usTemp;

	if (usItem1 > usItem2 )
	{
		// swap the two...
		usTemp = usItem2;
		usItem2 = usItem1;
		usItem1 = usTemp;
	}

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		usTemp = pSoldier->inv[bLoop].usItem;
		if ( usTemp >= usItem1 && usTemp <= usItem2 && pSoldier->inv[bLoop].exists() == true)
		{
			return( bLoop );
		}
	}

	return( ITEM_NOT_FOUND );
}


INT8 FindObjClass( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].usItemClass & usItemClass && pSoldier->inv[bLoop].exists() == true)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindAIUsableObjClass( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass )
{
	// finds the first object of the specified class which does NOT have
	// the "unusable by AI" flag set.

	// uses & rather than == so that this function can search for any weapon
	INT8 bLoop;

	// This is for the AI only so:

	// Do not consider tank cannons or rocket launchers to be "guns"

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( (Item[pSoldier->inv[bLoop].usItem].usItemClass & usItemClass) && !(pSoldier->inv[bLoop].fFlags & OBJECT_AI_UNUSABLE) && (pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE ) )
			{
				if ( usItemClass == IC_GUN && EXPLOSIVE_GUN( pSoldier->inv[bLoop].usItem ) )
				{
					continue;
				}
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

INT8 FindAIUsableObjClassWithin( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass, INT8 bLower, INT8 bUpper )
{
	INT8 bLoop;

	// This is for the AI only so:
	// Do not consider tank cannons or rocket launchers to be "guns"

	for (bLoop = bLower; bLoop < bUpper; bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( (Item[pSoldier->inv[bLoop].usItem].usItemClass & usItemClass) && !(pSoldier->inv[bLoop].fFlags & OBJECT_AI_UNUSABLE) && (pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE ) )
			{
				if ( usItemClass == IC_GUN && EXPLOSIVE_GUN( pSoldier->inv[bLoop].usItem ) )
				{
					continue;
				}
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

INT8 FindEmptySlotWithin( SOLDIERTYPE * pSoldier, INT8 bLower, INT8 bUpper )
{
	INT8	bLoop;

	for (bLoop = bLower; bLoop < bUpper; bLoop++)
	{
		// CHRISL: Only look at valid pockets
		if((UsingNewInventorySystem() == false) && !oldInv[bLoop])
			continue;
		if((pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) && !vehicleInv[bLoop])
			continue;
		if (pSoldier->inv[bLoop].exists() == false)
		{
			if (bLoop == SECONDHANDPOS && Item[pSoldier->inv[HANDPOS].usItem].twohanded )
			{
				continue;
			}
			else
			{
				return( bLoop );
			}
		}
	}
	return( ITEM_NOT_FOUND );
}

BOOLEAN GLGrenadeInSlot(SOLDIERTYPE *pSoldier, INT8 bSlot )
{
	if (pSoldier->inv[bSlot].exists() == true) {
		if (Item[pSoldier->inv[bSlot].usItem].glgrenade)
			return TRUE;
	}
	//switch (pSoldier->inv[bSlot].usItem)
	//{
	//	case GL_HE_GRENADE:
	//	case GL_TEARGAS_GRENADE:
	//	case GL_STUN_GRENADE:
	//	case GL_SMOKE_GRENADE:
	//		return(TRUE);
	//	default:
	//		return(FALSE);
	//}
		return FALSE;
}

// for grenade launchers
INT8 FindGLGrenade( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (GLGrenadeInSlot( pSoldier, bLoop ))
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindThrowableGrenade( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;
	BOOLEAN fCheckForFlares = FALSE;

	// JA2Gold: give some priority to looking for flares when at night
	// this is AI only so we can put in some customization for night
	if (GetTimeOfDayAmbientLightLevel() == NORMAL_LIGHTLEVEL_NIGHT)
	{
		if (pSoldier->stats.bLife > (pSoldier->stats.bLifeMax / 2))
		{
			fCheckForFlares = TRUE;
		}
	}
	if (fCheckForFlares)
	{
		// Do a priority check for flares first
		for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
		{
			if (pSoldier->inv[bLoop].exists() == true) {
				if ( Item[pSoldier->inv[ bLoop ].usItem].flare )
				{
					return( bLoop );
				}
			}
		}
	}

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( (Item[ pSoldier->inv[ bLoop ].usItem ].usItemClass & IC_GRENADE) && // Try this check instead, to avoid tossing RPG rounds     !GLGrenadeInSlot( pSoldier, bLoop ) &&
				GetLauncherFromLaunchable( pSoldier->inv[ bLoop ].usItem) == NOTHING )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

INT16 FindAttachmentSlot( OBJECTTYPE* pObj, UINT16 usItem, UINT8 subObject)
{
	if(UsingNewAttachmentSystem()==false || pObj->exists() == false)
		return -1;

	UINT8 loop = 0;
	for(attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end(); iter++, loop++){
		if(iter->exists() && iter->usItem == usItem)
			return loop;
	}

	return -1;
}

OBJECTTYPE* FindAttachment( OBJECTTYPE * pObj, UINT16 usItem, UINT8 subObject )
{
	if (pObj->exists() == true) {
		for (attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end(); ++iter) {
			if (iter->usItem == usItem && iter->exists())
			{
				return &(*iter);
			}
		}
	}
	return( 0 );
}

OBJECTTYPE* FindAttachmentByClass( OBJECTTYPE * pObj, UINT32 uiItemClass, UINT8 subObject )
{
	if (pObj->exists() == true) {
		for (attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end(); ++iter) {
			if (Item[iter->usItem].usItemClass == uiItemClass && iter->exists())
			{
				return &(*iter);
			}
		}
	}
	return( 0 );
}

INT8 FindLaunchable( SOLDIERTYPE * pSoldier, UINT16 usWeapon )
{
	INT8	bLoop;
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindLaunchable: weapon=%d",usWeapon));
	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( ValidLaunchable( pSoldier->inv[ bLoop ].usItem , usWeapon ) )
			{
				DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindLaunchable: returning slot %d",bLoop));
				return( bLoop );
			}
		}
	}

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindLaunchable: item not found"));
	return( ITEM_NOT_FOUND );
}

INT8 FindNonSmokeLaunchable( SOLDIERTYPE * pSoldier, UINT16 usWeapon )
{
	INT8	bLoop;
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindNonSmokeLaunchable: weapon=%d",usWeapon));
	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( ValidLaunchable( pSoldier->inv[ bLoop ].usItem , usWeapon ) && Explosive[Item[pSoldier->inv[ bLoop ].usItem].ubClassIndex].ubType != EXPLOSV_SMOKE )
			{
				DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindNonSmokeLaunchable: returning slot %d",bLoop));
				return( bLoop );
			}
		}
	}

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindNonSmokeLaunchable: item not found, so find items including smoke"));
	return( FindLaunchable(pSoldier,usWeapon) );
}

OBJECTTYPE* FindLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon )
{
	if (pObj->exists() == true) {
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (ValidLaunchable( iter->usItem, usWeapon) && iter->exists())
			{
				return &(*iter);
			}
		}
	}

	return( 0 );
}

//CHRISL: This function attempts to find a legal, single-shot grenade item based on the illegal, multi-shot one we send it
UINT16 FindLegalGrenade(UINT16 usItem)
{
	UINT16	newItem = 0;
	UINT16	usClass = Item[usItem].ubClassIndex;
	if(UsingNewAttachmentSystem()==false)
		return usItem;
	for(UINT16 loop = 0; loop < MAXITEMS+1; loop++){
		if(loop == usClass)
			continue;
		if(Explosive[usClass].ubType == Explosive[loop].ubType && Explosive[usClass].ubDamage == Explosive[loop].ubDamage
			&& Explosive[usClass].ubStunDamage == Explosive[loop].ubStunDamage && Explosive[usClass].ubRadius == Explosive[loop].ubRadius
			&& Explosive[usClass].ubVolume == Explosive[loop].ubVolume && Explosive[usClass].ubVolatility == Explosive[loop].ubVolatility
			&& Explosive[usClass].ubAnimationID == Explosive[loop].ubAnimationID && Explosive[usClass].ubDuration == Explosive[loop].ubDuration
			&& Explosive[usClass].ubStartRadius == Explosive[loop].ubStartRadius && Explosive[loop].ubMagSize == 1){
				newItem = loop;
				break;
		}
		if(Explosive[loop].uiIndex == 0 && loop > 0)
			break;
	}
	if(newItem > 0){
		for(UINT16 loop = 1; loop < MAXITEMS+1; loop++){
			if(Item[loop].uiIndex == 0)
				break;
			if(Item[loop].usItemClass & IC_GRENADE && Item[loop].ubClassIndex == newItem){
				return Item[loop].uiIndex;
			}
		}
	}
	return usItem;
}

OBJECTTYPE* FindNonSmokeLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon )
{

	if (pObj->exists() == true) {
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (ValidLaunchable( iter->usItem, usWeapon) && Explosive[Item[iter->usItem].ubClassIndex].ubType != EXPLOSV_SMOKE && iter->exists())
			{
				return &(*iter);
			}
		}
	}

	return( FindLaunchableAttachment(pObj,usWeapon) );
}


//Simple check to see if the item has any attachments
BOOLEAN ItemHasAttachments( OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier, UINT8 iter )
{
	if (pObj->exists() == true) {
		if(pSoldier != NULL){
			for (iter = 0; iter != pObj->objectStack.size(); ++iter) {
				if((*pObj)[iter]->AttachmentListSize() > 0){
					return TRUE;
				}
			}
		}
		else{
			if((*pObj)[iter]->AttachmentListSize() > 0){
				return TRUE;
			}
		}
	}
	return FALSE;
}

// Determine if it is possible to add this attachment to the CLASS of this item
// (i.e. to any item in the class)
BOOLEAN ValidAttachmentClass( UINT16 usAttachment, UINT16 usItem )
{
	INT32 iLoop = 0;
	while( 1 )
	{
		// see comment for AttachmentInfo array for why we skip IC_NONE
		if ( AttachmentInfo[ iLoop ].uiItemClass != IC_NONE )
		{
			if ( AttachmentInfo[ iLoop ].usItem == usAttachment )
			{
				if ( AttachmentInfo[ iLoop ].uiItemClass == Item[ usItem ].usItemClass )
				{
					return( TRUE );
				}
			}
		}
		iLoop++;
		if (AttachmentInfo[iLoop].usItem == 0)
		{
			// end of the array
			break;
		}
	}
	return( FALSE );
}

INT8 GetAttachmentInfoIndex( UINT16 usItem )
{
	INT32 iLoop = 0;

	while( 1 )
	{
		if ( AttachmentInfo[ iLoop ].usItem == usItem )
		{
			return( (INT8) iLoop );
		}
		iLoop++;
		if (AttachmentInfo[iLoop].usItem == 0)
		{
			// end of the array
			break;
		}
	}
	return( -1 );
}

//Determine if it is possible to add this attachment to the item.
BOOLEAN ValidAttachment( UINT16 usAttachment, UINT16 usItem, UINT8 * pubAPCost )
{
	INT32 iLoop = 0;
	if (pubAPCost) {
		*pubAPCost = (UINT8)APBPConstants[AP_RELOAD_GUN]; //default value
	}

	// look for the section of the array pertaining to this attachment...
	while( 1 )
	{
		if (Attachment[iLoop][0] == usAttachment)
		{
			break;
		}
		iLoop++;
		if (Attachment[iLoop][0] == 0)
		{
			// the proposed item cannot be attached to anything!
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (Attachment[iLoop][1] == usItem)
		{
			if((UsingNewAttachmentSystem()==false && Attachment[iLoop][3] != 1) || UsingNewAttachmentSystem()==true) {
				if (pubAPCost) {
					*pubAPCost = (UINT8)Attachment[iLoop][2]; //Madd: get ap cost of attaching items :)
				}
				break;
			}
		}
		iLoop++;
		if (Attachment[iLoop][0] != usAttachment)
		{
			// the proposed item cannot be attached to the item in question
			return( FALSE );
		}
	}
	return( TRUE );
}

BOOLEAN ValidAttachment( UINT16 usAttachment, OBJECTTYPE * pObj, UINT8 * pubAPCost, UINT8 subObject, std::vector<UINT16> usAttachmentSlotIndexVector )
{
	if (pObj->exists() == false) {
		return FALSE;
	}

	if(UsingNewAttachmentSystem()==true)
	{
		UINT16 usSlotIndex = 0;
		BOOLEAN foundValidAttachment = FALSE;
		UINT16 usLoop = 0;

		//It's possible we've entered this function without being passed the usAttachmentSlotIndexVector parameter
		if(usAttachmentSlotIndexVector.empty())
			usAttachmentSlotIndexVector = GetItemSlots(pObj);

		//Still no slots means nothing will ever be valid
		if(usAttachmentSlotIndexVector.empty())
			return FALSE;

		//Check if the attachment is valid with the main item
		foundValidAttachment = (ValidAttachment(usAttachment, pObj->usItem, pubAPCost) || ValidLaunchable(usAttachment, pObj->usItem));

		//Loop through all attachment points on the main item
		for(attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end() && !foundValidAttachment; ++iter)
		{
			if(iter->exists())
				foundValidAttachment = (ValidAttachment(usAttachment, iter->usItem, pubAPCost) || ValidLaunchable(usAttachment, iter->usItem));
		}

		return ( foundValidAttachment );
	}
	else
	{
		return( ValidAttachment(usAttachment, pObj->usItem, pubAPCost) );
	}
}

UINT8 AttachmentAPCost( UINT16 usAttachment, UINT16 usItem, SOLDIERTYPE * pSoldier ) // SANDRO - added argument
{
	UINT8 ubAPCost;

	ValidAttachment( usAttachment, usItem, &ubAPCost);

	// SANDRO - STOMP traits - Ambidextrous attaching objects speed bonus
	if ( pSoldier != NULL )
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		{
			ubAPCost = (UINT8)((ubAPCost * (100 - gSkillTraitValues.ubAMAttachingItemsAPsReduction) / 100) + 0.5);
		}
	}

	return ubAPCost;
}

//Also need one with pObj, for the one with usItem is not always correct.
UINT8 AttachmentAPCost( UINT16 usAttachment, OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier, UINT8 subObject, std::vector<UINT16> usAttachmentSlotIndexVector )
{
	UINT8 ubAPCost;

	ValidAttachment(usAttachment, pObj, &ubAPCost, subObject, usAttachmentSlotIndexVector);

	// SANDRO - STOMP traits - Ambidextrous attaching objects speed bonus
	if ( pSoldier != NULL )
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		{
			ubAPCost = (UINT8)((ubAPCost * (100 - gSkillTraitValues.ubAMAttachingItemsAPsReduction) / 100) + 0.5);
		}
	}

	return ubAPCost;
}

//Determine if this slot can receive this attachment.  This is different, in that it may
//be possible to have this attachment on this item, but may already have an attachment
//in the slot we're trying to attach to.
BOOLEAN ValidItemAttachmentSlot( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment, BOOLEAN fDisplayMessage, UINT8 subObject, INT16 slotCount, BOOLEAN fIgnoreAttachmentInSlot, OBJECTTYPE ** ppAttachInSlot, std::vector<UINT16> usAttachmentSlotIndexVector)
{
	BOOLEAN		fSimilarItems = FALSE, fSameItem = FALSE;
	UINT16		usSimilarItem = NOTHING;
	INT16		ubSlotIndex = 0;
	INT32		iLoop2 = 0;
	INT16		sTimesToRun = 0;
	UINT8		curSlot = 0;
	BOOLEAN		foundValidAttachment = FALSE;

	if (pObj->exists() == false) {
		return FALSE;
	}

	//It's possible we could get here without being sent the usAttachmentSlotIndexVector parameter
	if(usAttachmentSlotIndexVector.empty())
		usAttachmentSlotIndexVector = GetItemSlots(pObj, subObject);

	//No slots means nothing will ever be valid, also a slotCount outside this vector will never be valid either.
	if(usAttachmentSlotIndexVector.empty() || (INT16)usAttachmentSlotIndexVector.size() <= slotCount)
		return FALSE;

	//Search for incompatible attachments
	for(int i = 0;i<sizeof(IncompatibleAttachments);i++)
	{
		if ( FindAttachment(pObj, usAttachment, subObject) != 0 && !(Item[usAttachment].nasAttachmentClass & AC_GRENADE) && !(Item[usAttachment].nasAttachmentClass & AC_ROCKET))
		{//Search for identical attachments unless we're dealing with rifle grenades
			fSameItem = TRUE;
			break;
		}
		if ( IncompatibleAttachments[i][0] == NONE )
			break;
		if ( IncompatibleAttachments[i][0] == usAttachment && FindAttachment (pObj,IncompatibleAttachments[i][1],subObject) != 0 )
		{
			fSimilarItems = TRUE;
			usSimilarItem = IncompatibleAttachments[i][1];
			break;
		}
	}

	//Do we want to check all attachment slots or just the one in slotcount?
	if(slotCount == -1){
		//Loop through slots
		for(UINT8 curSlot = 0; curSlot < (*pObj)[subObject]->attachments.size() && !foundValidAttachment; curSlot++){
			//Any attachment that is already in this slot will go here.
			OBJECTTYPE * pAttachment;

			//Get the current attachment in the slot we're looking at.
			pAttachment = (*pObj)[subObject]->GetAttachmentAtIndex(curSlot);
			ubSlotIndex = usAttachmentSlotIndexVector[curSlot];

			//WarmSteel - does this particular slot already hold an item?
			if(ppAttachInSlot && pAttachment->exists() )
				*ppAttachInSlot = pAttachment;

			//Search for any valid attachments in this slot
			//CHRISL: Valid attachments are determined by the old "ValidItemAttachment" function and comparing the attachment class of the item and slot
			if(AttachmentSlots[ubSlotIndex].nasAttachmentClass & Item[usAttachment].nasAttachmentClass &&
				(ValidItemAttachment(pObj,usAttachment,fAttemptingAttachment,fDisplayMessage,subObject,usAttachmentSlotIndexVector) ||
				ValidLaunchable(usAttachment, GetAttachedGrenadeLauncher(pObj)) ||
				ValidLaunchable(usAttachment, pObj->usItem)))
			{
				foundValidAttachment = TRUE;
			}
		}
	} else {

		OBJECTTYPE * pAttachment;

		pAttachment = (*pObj)[subObject]->GetAttachmentAtIndex((UINT8)slotCount);

		ubSlotIndex = usAttachmentSlotIndexVector[slotCount];

		//WarmSteel - does this particular slot already hold an item? :( If we have a pAttachInSlot we're trying to switch, so then it doesn't matter.
		if(!fIgnoreAttachmentInSlot && pAttachment->exists() && fAttemptingAttachment && (!ppAttachInSlot || Item[pAttachment->usItem].inseparable)){
			//If we have a parameter to return pAttachment to, store it, else the item does not attach to this slot.
			fSimilarItems = TRUE;
			usSimilarItem = pAttachment->usItem;
		} else {
			//CHRISL: This should allow attachment swapping even if our attachments can't normally be on the weapon at the same time.
			if(slotCount != -1 && pAttachment->exists() && usSimilarItem == pAttachment->usItem && FindAttachmentSlot(pObj, pAttachment->usItem, subObject) == slotCount)
				fSimilarItems = FALSE;

			//If we have an item to return the existing attachment to.
			if(ppAttachInSlot && pAttachment->exists())
				*ppAttachInSlot = pAttachment;

			//Search for any valid attachments in this slot
			//CHRISL: Valid attachments are determined by the old "ValidItemAttachment" function and comparing the attachment class of the item and slot
			if(AttachmentSlots[ubSlotIndex].nasAttachmentClass & Item[usAttachment].nasAttachmentClass &&
				(ValidItemAttachment(pObj,usAttachment,FALSE,FALSE,subObject,usAttachmentSlotIndexVector) ||
				ValidLaunchable(usAttachment, GetAttachedGrenadeLauncher(pObj)) ||
				ValidLaunchable(usAttachment, pObj->usItem)))
			{
				foundValidAttachment = TRUE;
			}
		}
	}

	if(fAttemptingAttachment){
		if (fSimilarItems)
		{
			if(fDisplayMessage) ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_CANT_USE_TWO_ITEMS ], ItemNames[ usSimilarItem ], ItemNames[ usAttachment ] );
			return( FALSE );
		} 
		else if (fSameItem)
		{
			if (fDisplayMessage) ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_ATTACHMENT_ALREADY ] );
			return( FALSE );
		}
		else if ( !foundValidAttachment && fDisplayMessage && !ValidMerge( usAttachment, pObj->usItem ) )
		{
			//We don't want a message if we might be merging this little thingey later.
			//well, maybe the player thought he could
			CHAR16	zTemp[ 100 ];

			swprintf( zTemp, Message[ STR_CANNOT_ATTACH_SLOT ], ItemNames[ usAttachment ] );
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
		}
	}

	return( foundValidAttachment );
}

//Determine if this item can receive this attachment.  This is different, in that it may
//be possible to have this attachment on this item, but may already have an attachment on
//it which doesn't work simultaneously with the new attachment (like a silencer and duckbill).
BOOLEAN ValidItemAttachment( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment, BOOLEAN fDisplayMessage, UINT8 subObject, std::vector<UINT16> usAttachmentSlotIndexVector )
{
	BOOLEAN		fSameItem = FALSE, fSimilarItems = FALSE;
	UINT16		usSimilarItem = NOTHING;

	if (pObj->exists() == false) {
		return FALSE;
	}
	if ( !ValidAttachment( usAttachment, pObj, NULL, subObject, usAttachmentSlotIndexVector ) )
	{
		// check for an underslung grenade launcher attached to the gun
		if ( (IsGrenadeLauncherAttached ( pObj, subObject ) ) && ValidLaunchable( usAttachment, GetAttachedGrenadeLauncher( pObj ) ) )
		{
			return ( TRUE );
			/*
			if ( fAttemptingAttachment )
			{
				// if there is no other grenade attached already, then we can attach it
				if (FindAttachmentByClass( pObj, IC_GRENADE) != ITEM_NOT_FOUND)
				{
					return( FALSE );
				}
				// keep going, it can be attached to the grenade launcher
			}
			else
			{
				// logically, can be added
				return( TRUE );
			}
			*/
		}
		else
		{
			if ( fAttemptingAttachment && ValidAttachmentClass( usAttachment, pObj->usItem ) )
			{
				// well, maybe the player thought he could
				CHAR16	zTemp[ 100 ];

				swprintf( zTemp, Message[ STR_CANT_ATTACH ], ItemNames[ usAttachment ], ItemNames[ pObj->usItem ] );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
			}

			return( FALSE );
		}
	}
	// special conditions go here
	// can't have two of the same attachment on an item
	/*
	if (FindAttachment( pObj, usAttachment ) != 0)
	{
		fSameItem = TRUE;
	}
	*/

	for(int i = 0;i<sizeof(IncompatibleAttachments);i++)
	{
		if ( FindAttachment(pObj, usAttachment, subObject) != 0 )
		{
			fSameItem = TRUE;
			break;
		}

		if ( IncompatibleAttachments[i][0] == NONE )
			break;

		if ( IncompatibleAttachments[i][0] == usAttachment && FindAttachment (pObj,IncompatibleAttachments[i][1],subObject) != 0 )
		{
			fSimilarItems = TRUE;
			usSimilarItem = IncompatibleAttachments[i][1];
			break;
		}
	}


	//// special code for items which won't attach if X is present
	//switch( usAttachment )
	//{
	//	case BIPOD:
	//		if ( FindAttachment( pObj, UNDER_GLAUNCHER) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = UNDER_GLAUNCHER;
	//		}
	//		break;
	//	case UNDER_GLAUNCHER:
	//		if ( FindAttachment( pObj, BIPOD ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = BIPOD;
	//		}
	//		break;
	///*
	//	case LASERSCOPE:
	//		if (FindAttachment( pObj, SNIPERSCOPE ) != 0)
	//		{
	//			return( FALSE );
	//		}
	//		break;
	//	case SNIPERSCOPE:
	//		if (FindAttachment( pObj, LASERSCOPE ) != 0)
	//		{
	//			return( FALSE );
	//		}
	//		break;
	//		*/
	//	case DETONATOR:
	//		if( FindAttachment( pObj, REMDETONATOR ) != 0 )
	//		{
	//			fSameItem = TRUE;
	//		}
	//		break;
	//	case REMDETONATOR:
	//		if( FindAttachment( pObj, DETONATOR ) != 0 )
	//		{
	//			fSameItem = TRUE;
	//		}
	//		break;
	//	case SNIPERSCOPE:
	//		if( FindAttachment( pObj, REFLEX_SCOPED ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = REFLEX_SCOPED;
	//		}
	//		break;
	//	case REFLEX_SCOPED:
	//		if( FindAttachment( pObj, SNIPERSCOPE ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = SNIPERSCOPE;
	//		}
	//		if( FindAttachment( pObj, REFLEX_UNSCOPED ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = REFLEX_UNSCOPED;
	//		}
	//		break;
	//	case REFLEX_UNSCOPED:
	//		if( FindAttachment( pObj, REFLEX_SCOPED ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = REFLEX_SCOPED;
	//		}
	//		break;
	//	case SILENCER:
	//		if( FindAttachment( pObj, FLASH_SUPPRESSOR  ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = FLASH_SUPPRESSOR;
	//		}
	//		break;
	//	case FLASH_SUPPRESSOR:
	//		if( FindAttachment( pObj, SILENCER  ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = SILENCER;
	//		}
	//		break;
	//}

	if (fAttemptingAttachment)
	{
		if (fSameItem)
		{
			if (fDisplayMessage) ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_ATTACHMENT_ALREADY ] );
			return( FALSE );
		}
		else if (fSimilarItems)
		{
			if (fDisplayMessage) ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_CANT_USE_TWO_ITEMS ], ItemNames[ usSimilarItem ], ItemNames[ usAttachment ] );
			return( FALSE );
		}
	}

	return( TRUE );
}

//Determines if it is possible to equip this weapon with this ammo.
BOOLEAN ValidAmmoType( UINT16 usItem, UINT16 usAmmoType )
{
	if (Item[usItem].usItemClass == IC_GUN && Item[usAmmoType].usItemClass == IC_AMMO)
	{
		if (Weapon[usItem].ubCalibre == Magazine[Item[usAmmoType].ubClassIndex].ubCalibre)
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN CompatibleFaceItem( UINT16 usItem1, UINT16 usItem2 )
{
	INT32 iLoop = 0;

	//Madd: skip this function if either item is nothing
	//this will let us trim some lines from compatiblefaceitems.xml
	if ( usItem1 == NONE || usItem2 == NONE )
		return TRUE;

	// look for the section of the array pertaining to this attachment...
	while( 1 )
	{
		if (CompatibleFaceItems[iLoop][0] == usItem1)
		{
			break;
		}
		iLoop++;
		if (CompatibleFaceItems[iLoop][0] == 0)
		{
			// the proposed item cannot fit with anything!
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (CompatibleFaceItems[iLoop][1] == usItem2)
		{
			break;
		}
		iLoop++;
		if (CompatibleFaceItems[iLoop][0] != usItem1)
		{
			// the proposed item cannot be attached to the item in question
			return( FALSE );
		}
	}
	return( TRUE );
}


//Determines if this item is a two handed item.
BOOLEAN TwoHandedItem( UINT16 usItem )
{
//	if (Item[usItem].fFlags & ITEM_TWO_HANDED)
	if (Item[usItem].twohanded )
	{
		return( TRUE );
	}
	return FALSE;
}

BOOLEAN ValidLaunchable( UINT16 usLaunchable, UINT16 usItem )
{
	INT32 iLoop = 0;
	//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d",usLaunchable,usItem));
	// look for the section of the array pertaining to this launchable item...
	while( 1 )
	{
		if (Launchable[iLoop][0] == usLaunchable)
		{
			break;
		}
		iLoop++;
		if (Launchable[iLoop][0] == 0)
		{
			// the proposed item cannot be attached to anything!
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d, return FALSE, launchable not found",usLaunchable,usItem));
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (Launchable[iLoop][1] == usItem)
		{
			break;
		}
		iLoop++;
		if (Launchable[iLoop][0] != usLaunchable)
		{
			// the proposed item cannot be attached to the item in question
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d, return FALSE, item not found",usLaunchable,usItem));
			return( FALSE );
		}
	}
	//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d, return TRUE",usLaunchable,usItem));
	return( TRUE );
}

BOOLEAN ValidItemLaunchable( OBJECTTYPE * pObj, UINT16 usAttachment )
{
	if (pObj->exists() == false) {
		return FALSE;
	}
	if ( !ValidLaunchable( usAttachment, pObj->usItem ) )
	{
		return( FALSE );
	}
	// if we can find another of the same class as the attachment, it's not possible
	if ( FindAttachmentByClass( pObj, Item[ usAttachment ].usItemClass ) != 0 )
	{
		return( FALSE );
	}
	return( TRUE );
}


UINT16 GetLauncherFromLaunchable( UINT16 usLaunchable )
{
	INT32 iLoop = 0;
	// look for the section of the array pertaining to this launchable item...
	while( 1 )
	{
		if (Launchable[iLoop][0] == usLaunchable)
		{
			break;
		}
		iLoop++;
		if (Launchable[iLoop][0] == 0)
		{
			// the proposed item cannot be attached to anything!
			return( NOTHING );
		}
	}

	return( Launchable[iLoop][1] );
}



BOOLEAN EvaluateValidMerge( UINT16 usMerge, UINT16 usItem, UINT16 * pusResult, UINT16 * pusResult2, UINT8 * pubType, UINT8 * pubAPCost )
{
	// NB "usMerge" is the object being merged with (e.g. compound 18)
	// "usItem" is the item being merged "onto" (e.g. kevlar vest)
	INT32 iLoop = 0;

	//CHRISL: Update this so we can also merge IC_MONEY like wallets and nuggets.
	if (usMerge == usItem && (Item[ usItem ].usItemClass == IC_AMMO || Item[ usItem ].usItemClass == IC_MONEY))
	{
		*pusResult = usItem;
		*pubType = COMBINE_POINTS;
		return( TRUE );
	}
	// look for the section of the array pertaining to this Merge...
	while( 1 )
	{
		if (Merge[iLoop][0] == usMerge)
		{
			break;
		}
		iLoop++;
		if (Merge[iLoop][0] == 0)
		{
			// the proposed item cannot be merged with anything!
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (Merge[iLoop][1] == usItem)
		{
			break;
		}
		iLoop++;
		if (Merge[iLoop][0] != usMerge)
		{
			// the proposed item cannot be merged with the item in question
			return( FALSE );
		}
	}

	//WarmSteel - Return false if the results aren't valid.
	if( !ItemIsLegal(Merge[iLoop][2], TRUE) && !ItemIsLegal(Merge[iLoop][3], TRUE) ){
		return( FALSE );
	}

	*pusResult = Merge[iLoop][2];
	*pusResult2 = Merge[iLoop][3];
	*pubType = (UINT8) Merge[iLoop][4];
	*pubAPCost = (UINT8) Merge[iLoop][5];
	return( TRUE );
}

BOOLEAN ValidMerge( UINT16 usMerge, UINT16 usItem )
{
	UINT16	usIgnoreResult, usIgnoreResult2;
	UINT8		ubIgnoreType, ubIgnoreAPCost;
	return( EvaluateValidMerge( usMerge, usItem, &usIgnoreResult, &usIgnoreResult2, &ubIgnoreType, &ubIgnoreAPCost ) );
}

int GetPocketSizeByDimensions(int sizeX, int sizeY)
{
	static const UINT8 cisPocketSize[6][4] =
	{
		11, 12, 13, 14,
		15, 16, 17, 18,
		19, 20, 21, 22,
		23, 24, 25, 26,
		27, 28, 29, 30,
		31, 32, 33, 34
	};
	return cisPocketSize[sizeX][sizeY];
}

void GetPocketDimensionsBySize(int pocketSize, int& sizeX, int& sizeY)
{
	static const UINT8 cisPocketSize[6][4] =
	{
		11, 12, 13, 14,
		15, 16, 17, 18,
		19, 20, 21, 22,
		23, 24, 25, 26,
		27, 28, 29, 30,
		31, 32, 33, 34
	};

	for(sizeX=0; sizeX<6; sizeX++)
	{
		for(sizeY=0; sizeY<4; sizeY++)
		{
			if(pocketSize == cisPocketSize[sizeX][sizeY])
			{
				return;
			}
		}
	}
}


// CHRISL: New function to dynamically modify ItemSize based on attachments, stack size, etc
UINT16 CalculateItemSize( OBJECTTYPE *pObject )
{
	UINT16		iSize;
	UINT16		currentSize = 0;
	UINT32		cisIndex;

	// Determine default ItemSize based on item and attachments
	cisIndex = pObject->usItem;
	iSize = Item[cisIndex].ItemSize;
	if(iSize>34)
		iSize = 34;

	//for each object in the stack, hopefully there is only 1
	for (int numStacked = 0; numStacked < pObject->ubNumberOfObjects; ++numStacked) {
		//some weapon attachments can adjust the ItemSize of a weapon
		if(iSize<10) {
			for (attachmentList::iterator iter = (*pObject)[numStacked]->attachments.begin(); iter != (*pObject)[numStacked]->attachments.end(); ++iter) {
				if (iter->exists() == true) {
					iSize += Item[iter->usItem].itemsizebonus;
					// CHRISL: This is to catch things if we try and reduce ItemSize when we're already at 0
					if(iSize > 34 || iSize < 0)
						iSize = 0;
					if(iSize > 9)
						iSize = 9;
				}
			}
		}

		// LBENODE has it's ItemSize adjusted based on what it's storing
		if(pObject->IsActiveLBE(numStacked) == true) {
			LBENODE* pLBE = pObject->GetLBEPointer(numStacked);
			if(pLBE)
			{
				//start by determining the equivalent number of "small" pockets that this LBENODE has access to.  This
				//	is based on the pType value in Pockets.xml with 1=Small, 2=Medium and 3=Large
				UINT16	totalPocketValue = 0;
				FLOAT	percentOfItemUsed = 0;
				UINT16	pIndex, testSize, maxSize;
				UINT8	pocketCapacity, numberOfSizeIncrements;
				FLOAT	currentPocketPercent, currentPocketPartOfTotal;
				for(unsigned int x = 0; x < pLBE->inv.size(); x++)
				{
					if(LoadBearingEquipment[Item[pObject->usItem].ubClassIndex].lbePocketIndex[x] != 0)
					{
						pIndex = LoadBearingEquipment[Item[pObject->usItem].ubClassIndex].lbePocketIndex[x];
						totalPocketValue += LBEPocketType[pIndex].pType;
					}
				}
				//Now, look through each active pocket
				for(unsigned int x = 0; x < pLBE->inv.size(); x++)
				{
					if(pLBE->inv[x].exists() == true)
					{
						pIndex = LoadBearingEquipment[Item[pObject->usItem].ubClassIndex].lbePocketIndex[x];
						testSize = CalculateItemSize(&pLBE->inv[x]);
						pocketCapacity = LBEPocketType[pIndex].ItemCapacityPerSize[testSize];
						currentPocketPercent = (FLOAT)pLBE->inv[x].ubNumberOfObjects / (FLOAT)pocketCapacity;
						currentPocketPartOfTotal = (FLOAT)LBEPocketType[pIndex].pType / (FLOAT)totalPocketValue;
						percentOfItemUsed += currentPocketPartOfTotal * currentPocketPercent;
					}
				}
				maxSize = max(iSize, LoadBearingEquipment[Item[pObject->usItem].ubClassIndex].lbeFilledSize);
				//Now, determine the increments between initial ItemSize and Filled Size, and adjust iSize by percentOfItemUsed
				if(percentOfItemUsed != 0)
				{
					numberOfSizeIncrements = LoadBearingEquipment[Item[pObject->usItem].ubClassIndex].lbeFilledSize - Item[pObject->usItem].ItemSize;
					testSize = (UINT16)((numberOfSizeIncrements * percentOfItemUsed) + .5);
					currentSize = __max(iSize + testSize, currentSize);
					currentSize = __min(currentSize, maxSize);
				}
#if 0
//old method
				UINT16	newSize, testSize, maxSize;
				UINT8	cnt=0;
				newSize = 0;
				maxSize = max(iSize, LoadBearingEquipment[Item[pObject->usItem].ubClassIndex].lbeFilledSize);
				// Look for the ItemSize of the largest item in this LBENODE
				for(unsigned int x = 0; x < pLBE->inv.size(); ++x)
				{
					if(pLBE->inv[x].exists() == true)
					{
						testSize = CalculateItemSize(&(pLBE->inv[x]));
						//Now that we have the size of one item, we want to factor in the number of items since two
						//	items take up more space then one.
						testSize = testSize + pLBE->inv[x].ubNumberOfObjects - 1;
						testSize = min(testSize,34);
						//We also need to increase the size of guns so they'll fit with the rest of our calculations.
						if(testSize < 5)
							testSize += 10;
						if(testSize < 10)
							testSize += 18;
						//Finally, we want to factor in multiple pockets.  We'll do this by counting the number of filled
						//	pockets, then add this count total to our newSize when everything is finished.
						cnt++;
						newSize = max(testSize, newSize);
					}
				}
				//Add the total number of filled pockets to our NewSize to account for multiple pockets being used
				newSize += cnt;
				newSize = min(newSize,34);
				// If largest item is smaller then LBE, don't change ItemSize
				if(newSize > 0 && newSize < iSize) {
					iSize = iSize;
				}
				// if largest item is larget then LBE but smaller then max size, partially increase ItemSize
				else if(newSize >= iSize && newSize < maxSize) {
					iSize = newSize;
				}
				// if largest item is larger then max size, reset ItemSize to max size
				else if(newSize >= maxSize) {
					iSize = maxSize;
				}
#endif
			}
		}
	}
	//Finally, set the new iSize value
	iSize = __max(iSize, currentSize);

	return(iSize);
}

UINT16 CalculateAmmoWeight( UINT16 usGunAmmoItem, UINT16 ubShotsLeft )
{
	if( 0 == usGunAmmoItem ) /* Sergeant_Kolja: 2007-06-11, Fix for Creature Spit. This has no Ammo, so the old code calculated accidentally -1.6 resulting in 0xFFFF */
	{
		DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "'no ammo weight' FIX for Creatures\r\n" );
		return 0;
	}

	//Temporary calculation for minWeight. 0.2*ubWeight rounded correctly
	UINT32 uiMinWeight = (UINT32)((Item[usGunAmmoItem].ubWeight / 5.0) + 0.5);
	if( uiMinWeight < 1 || uiMinWeight > Item[usGunAmmoItem].ubWeight)
	{
		uiMinWeight = 1;
	}

	double weight = 0.0;

	if(ubShotsLeft > 0)
	{
		if( uiMinWeight == Item[usGunAmmoItem].ubWeight )
		{
			weight += (double)uiMinWeight;
		}
		else
		{
			weight += (double)uiMinWeight + (( (double)ubShotsLeft / (double)Magazine[ Item[usGunAmmoItem].ubClassIndex ].ubMagSize) * ( (double)Item[usGunAmmoItem].ubWeight - (double)uiMinWeight ));
		}
	}
	weight += 0.5; //Pulmu:To round correctly
	return (UINT16)weight;
	//Pulmu end
}

/*CHRISL: Change to a 16bit integer for a max weight of 6553.5kg.  Also changed to account for
new inventory system. */
UINT16 CalculateObjectWeight( OBJECTTYPE *pObject )
{
	if (pObject->exists() == false || pObject->ubNumberOfObjects == 0) {
		return 0;
	}

	UINT16 weight = 0;
	INVTYPE * pItem;
	pItem = &(Item[ pObject->usItem ]);
	//ADB it is much easier and faster to calculate ammo in a lump rather than accumulate a stack's weight
	if ( pItem->usItemClass == IC_AMMO)//Pulmu: added weight allowance for ammo not being full
	{
		if ( gGameExternalOptions.fAmmoDynamicWeight == TRUE ) {
			for( int cnt = 0; cnt < pObject->ubNumberOfObjects; cnt++ )
			{
				weight += CalculateAmmoWeight(pObject->usItem, (*pObject)[cnt]->data.ubShotsLeft);
			}
		}
		else {
			// Start with base weight
			weight = pItem->ubWeight;
			//multiply by the number of objects, can be 0
			weight *= pObject->ubNumberOfObjects;
		}
	}
	else {
		for (int x = 0; x < pObject->ubNumberOfObjects; ++x) {
			weight += pObject->GetWeightOfObjectInStack(x);
		}
	}

	return( weight );
}

UINT16 OBJECTTYPE::GetWeightOfObjectInStack(unsigned int index)
{
	//Item does not exist
	if( index >= ubNumberOfObjects )
	{
		return 0;
	}

	INVTYPE * pItem = &(Item[ usItem ]);

	// Start with base weight
	UINT16 weight = pItem->ubWeight;
	if ( pItem->usItemClass != IC_AMMO )
	{
		// Are we looking at an LBENODE item?  New inventory only.
		if(pItem->usItemClass == IC_LBEGEAR && IsActiveLBE(index) && (UsingNewInventorySystem() == true))
		{
			LBENODE* pLBE = GetLBEPointer(index);
			if (pLBE) {
				for ( unsigned int subObjects = 0; subObjects < pLBE->inv.size(); subObjects++)
				{
					if (pLBE->inv[subObjects].exists() == true)
					{
						weight += CalculateObjectWeight(&(pLBE->inv[subObjects]));
					}
				}
			}
			//do not search for attachments to an LBE
			return weight;
		}

		// account for any attachments
		for (attachmentList::iterator iter = (*this)[index]->attachments.begin(); iter != (*this)[index]->attachments.end(); ++iter) {
			if(iter->exists()){
				weight += CalculateObjectWeight(&(*iter));
			}
		}

		// add in weight of ammo
		if (Item[ usItem ].usItemClass == IC_GUN && (*this)[index]->data.gun.ubGunShotsLeft > 0)
		{
			if( gGameExternalOptions.fAmmoDynamicWeight == TRUE )
			{
				weight += CalculateAmmoWeight((*this)[index]->data.gun.usGunAmmoItem, (*this)[index]->data.gun.ubGunShotsLeft);
			}
			else
			{
				weight += Item[ (*this)[index]->data.gun.usGunAmmoItem ].ubWeight;
			}
		}
	}
	else if ( pItem->usItemClass == IC_AMMO && gGameExternalOptions.fAmmoDynamicWeight == TRUE )//Pulmu: added weight allowance for ammo not being full
	{
		weight = CalculateAmmoWeight(usItem, (*this)[index]->data.ubShotsLeft);
	}
	return weight;
}

UINT32 CalculateCarriedWeight( SOLDIERTYPE * pSoldier )
{
	UINT32	uiTotalWeight = 0;
	UINT32	uiPercent;
	UINT8		ubLoop;
	UINT8		ubStrengthForCarrying;

	//Pulmu: Changes for dynamic ammo weight
	for( ubLoop = 0; ubLoop < pSoldier->inv.size(); ubLoop++)
	{
		//ADB the weight of the object is already counting stacked objects, attachments, et al
		uiTotalWeight += CalculateObjectWeight(&pSoldier->inv[ubLoop]);
	}
	// for now, assume soldiers can carry 1/2 their strength in KGs without penalty.
	// instead of multiplying by 100 for percent, and then dividing by 10 to account
	// for weight units being in 10ths of kilos, not kilos... we just start with 10 instead of 100!
	ubStrengthForCarrying = EffectiveStrength( pSoldier );
	if ( ubStrengthForCarrying > 80 )
	{
		ubStrengthForCarrying += (ubStrengthForCarrying - 80);
	}

	// SANDRO - STOMP traits - Bodybuilding carry weight bonus
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, BODYBUILDING_NT ) )
	{
		ubStrengthForCarrying = (ubStrengthForCarrying * (100 + gSkillTraitValues.ubBBCarryWeightBonus) / 100); // plus one third
	}

	// for now, assume soldiers can carry 1/2 their strength in KGs without penalty.
	// instead of multiplying by 100 for percent, and then dividing by 10 to account
	// for weight units being in 10ths of kilos, not kilos... we just start with 10 instead of 100!

	// HEADROCK HAM 3: STR required per 1/2 kilo has been externalized. Can someone tidy this up though? The
	// formula works great, but it's damn ugly now.
	uiPercent = (UINT32)(((FLOAT)10 * (FLOAT)gGameExternalOptions.iStrengthToLiftHalfKilo) * uiTotalWeight) / ( ubStrengthForCarrying / 2 );

	return( uiPercent );

}

void DeleteObj(OBJECTTYPE * pObj )
{
	pObj->initialize();
}

void SwapObjs( OBJECTTYPE * pObj1, OBJECTTYPE * pObj2 )
{
	//cannot use gTempObject
	OBJECTTYPE Temp (*pObj1 );
	*pObj1 = *pObj2;
	*pObj2 = Temp;
}

//ADB these 2 functions were created because the code calls SwapObjs all over the place
//but never handles the effects of that swap!
void SwapObjs(SOLDIERTYPE* pSoldier, int leftSlot, int rightSlot, BOOLEAN fPermanent)
{
	SwapObjs(&pSoldier->inv[ leftSlot ], &pSoldier->inv[ rightSlot ]);

	if (fPermanent)
	{
		//old usItem for the left slot is now stored in the right slot, and vice versa
		HandleTacticalEffectsOfEquipmentChange(pSoldier, leftSlot, pSoldier->inv[ rightSlot ].usItem, pSoldier->inv[ leftSlot ].usItem);
		HandleTacticalEffectsOfEquipmentChange(pSoldier, rightSlot, pSoldier->inv[ leftSlot ].usItem, pSoldier->inv[ rightSlot ].usItem);
	}
}

void SwapObjs(SOLDIERTYPE* pSoldier, int slot, OBJECTTYPE* pObject, BOOLEAN fPermanent)
{
	SwapObjs(&pSoldier->inv[ slot ], pObject);

	if (fPermanent)
	{
		HandleTacticalEffectsOfEquipmentChange(pSoldier, slot, pObject->usItem, pSoldier->inv[ slot ].usItem);
	}
}

void DamageObj( OBJECTTYPE * pObj, INT8 bAmount, UINT8 subObject )
{
	//usually called from AttachObject, where the attachment is known to be a single item,
	//and the attachment is only being attached to the top of the stack
	if (bAmount >= (*pObj)[subObject]->data.objectStatus)
	{
		(*pObj)[subObject]->data.objectStatus = 1;
	}
	else
	{
		(*pObj)[subObject]->data.objectStatus -= bAmount;
	}
}

void DistributeStatus(OBJECTTYPE* pSourceObject, OBJECTTYPE* pTargetObject, INT16 bMaxPoints)
{
	INT16 bPointsToMove;
	for ( int bLoop = pSourceObject->ubNumberOfObjects - 1; bLoop >= 0; bLoop-- )
	{
		StackedObjectData* pSource = (*pSourceObject)[ bLoop ];
		if ( pSource->data.objectStatus > 0 )
		{
			// take the points here and distribute over the lower #d items
			int bLoop2;
			if (pSourceObject == pTargetObject) {
				//we are averaging out the same object
				bLoop2 = bLoop - 1;
			}
			else {
				//we are moving from the cursor object to this one
				bLoop2 = pTargetObject->ubNumberOfObjects - 1;
			}

			//for (; bLoop2 >= 0; bLoop2-- )
			for(int i = 0; i<=bLoop2; i++)
			{
				StackedObjectData* pDest = (*pTargetObject)[ i ];
				if ( pDest->data.objectStatus < bMaxPoints )
				{
					bPointsToMove = bMaxPoints - pDest->data.objectStatus;
					bPointsToMove = __min( bPointsToMove, pSource->data.objectStatus );

					pDest->data.objectStatus += bPointsToMove;
					pSource->data.objectStatus -= bPointsToMove;

					if ( pSource->data.objectStatus == 0 )
					{
						StackedObjects::iterator iter = pSourceObject->objectStack.begin();
						for (int x = 0; x < bLoop; ++x) {
							++iter;
						}
						pSourceObject->objectStack.erase(iter);
						pSourceObject->ubNumberOfObjects--;
						//ADB ubWeight has been removed, see comments in OBJECTTYPE
						//pSourceObject->ubWeight = CalculateObjectWeight(pSourceObject);
						// done!
						break;
					}
				}
			}
		}
	}
}

BOOLEAN PlaceObjectAtObjectIndex( OBJECTTYPE * pSourceObj, OBJECTTYPE * pTargetObj, UINT8 ubIndex, UINT32 ubCap )
{
	if (pSourceObj->usItem != pTargetObj->usItem)
	{
		return( TRUE );
	}
	if (ubIndex < pTargetObj->ubNumberOfObjects)
	{
		// swap
		//std::swap??
		StackedObjectData data = *((*pSourceObj)[0]);

		*((*pSourceObj)[0]) = *((*pTargetObj)[ubIndex]);
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//pSourceObj->ubWeight = CalculateObjectWeight(pSourceObj);

		*((*pTargetObj)[ubIndex]) = data;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//pTargetObj->ubWeight = CalculateObjectWeight(pTargetObj);
		return( TRUE );
	}
	else
	{
		// add to end
		pTargetObj->AddObjectsToStack( *pSourceObj, 1, NULL, NUM_INV_SLOTS, ubCap );
		if (pSourceObj->exists() == true) {
			return( TRUE );
		}
		else {
			return FALSE;
		}
	}
}

#define RELOAD_NONE 0
#define RELOAD_PLACE 1
#define RELOAD_SWAP 2
#define RELOAD_TOPOFF 3
#define RELOAD_AUTOPLACE_OLD 4

BOOLEAN ReloadGun( SOLDIERTYPE * pSoldier, OBJECTTYPE * pGun, OBJECTTYPE * pAmmo, UINT32 subObject )
{
	UINT16			ubBulletsToMove;
	INT16			bAPs;
	UINT16			usReloadSound;
	BOOLEAN			fSameAmmoType;
	BOOLEAN			fSameMagazineSize;
	BOOLEAN			fReloadingWithStack;
	BOOLEAN			fEmptyGun;
	BOOLEAN			fEnoughAPs;
	INT8			bReloadType;
	UINT16			usNewAmmoItem;
	UINT16			usLargestMag;
	UINT32			ammoObject = 0;

	bAPs = 0;

	if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		//CHRISL: Alter this so we treat clip fed weapons differently from weapons that load with loose rounds
		bAPs = GetAPsToReloadGunWithAmmo( pSoldier, pGun, pAmmo );
		if ( !EnoughPoints( pSoldier, bAPs, 0,TRUE ) )
		{
			return( FALSE );
		}
	}

	if ( Item[ pGun->usItem ].usItemClass == IC_LAUNCHER || Item[pGun->usItem].cannon )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("ReloadGun: Loading launcher - new ammo type = %d, weight = %d", pAmmo->usItem, CalculateObjectWeight(pAmmo) ) );
		(*pGun)[subObject]->data.gun.usGunAmmoItem = pAmmo->usItem;
		if ( pGun->AttachObject( pSoldier, pAmmo ) == FALSE )
		{
			(*pGun)[subObject]->data.gun.usGunAmmoItem = NONE;
			// abort
			return( FALSE );
		}
	}
	else
	{
		fEmptyGun = ((*pGun)[subObject]->data.gun.ubGunShotsLeft == 0);
		fReloadingWithStack = (pAmmo->ubNumberOfObjects > 1);
		fSameAmmoType = ( (*pGun)[subObject]->data.gun.ubGunAmmoType == Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType );
		fSameMagazineSize = ( Magazine[ Item[ pAmmo->usItem ].ubClassIndex ].ubMagSize == GetMagSize( pGun, subObject));
		fEnoughAPs = EnoughPoints( pSoldier, GetAPsToReloadGunWithAmmo( pSoldier, pGun, pAmmo, FALSE ), 0,FALSE );

		if (fEmptyGun)
		{
			bReloadType = RELOAD_PLACE;
		}
		else
		{
			// record old ammo
			CreateAmmo((*pGun)[subObject]->data.gun.usGunAmmoItem, &gTempObject, (*pGun)[subObject]->data.gun.ubGunShotsLeft);

			if (fSameMagazineSize)
			{
				if (fSameAmmoType)
				{
					if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) && Weapon[pGun->usItem].swapClips )
					{
						bReloadType = RELOAD_SWAP;
					}
					else
					{
						bReloadType = RELOAD_TOPOFF;
					}
				}
				else
				{
					if (!fReloadingWithStack)
					{
						bReloadType = RELOAD_SWAP;
					}
					else
					{
						bReloadType = RELOAD_AUTOPLACE_OLD;
					}
				}
			}
			else  // diff sized magazines
			{
				if (fSameAmmoType)
				{
					bReloadType = RELOAD_TOPOFF;
				}
				else
				{
					bReloadType = RELOAD_AUTOPLACE_OLD;
				}
			}
		}

		//CHRISL: We need to verify that the passed subObject actually exists.  We could get passed an empty subObject if we
		//	reload a stack of weapons from the stack popup.  If this is the case, reset subObject to the last item in the
		//	stack
		if(subObject >= pGun->ubNumberOfObjects)
			subObject = ammoObject = pGun->ubNumberOfObjects-1;

		//CHRISL: If reloading with a stack, we probably want the item with the most ammo still in it
		if (fReloadingWithStack)
		{
			usLargestMag = (*pAmmo)[ammoObject]->data.ubShotsLeft;
			for(int i = 0; i<pAmmo->ubNumberOfObjects; i++)
			{
				if((*pAmmo)[i]->data.ubShotsLeft == Magazine[Item[pAmmo->usItem].ubClassIndex].ubMagSize)
				{
					ammoObject = i;
					break;
				}
				else if((*pAmmo)[i]->data.ubShotsLeft > usLargestMag)
				{
					ammoObject = i;
				}
			}
		}

		if (fSameMagazineSize)
		{
			// record new ammo item for gun
			usNewAmmoItem = pAmmo->usItem;

			if (bReloadType == RELOAD_TOPOFF)
			{
				ubBulletsToMove = __min( (*pAmmo)[ammoObject]->data.ubShotsLeft, GetMagSize(pGun) - (*pGun)[subObject]->data.gun.ubGunShotsLeft );
			}
			else
			{
				ubBulletsToMove = (*pAmmo)[ammoObject]->data.ubShotsLeft;
			}

		}
		else if (Magazine[Item[pAmmo->usItem].ubClassIndex].ubMagSize > GetMagSize(pGun, subObject))
		{
//MADD MARKER
			//usNewAmmoItem = pAmmo->usItem - 1;
			usNewAmmoItem = FindReplacementMagazine(Weapon[pGun->usItem].ubCalibre ,GetMagSize(pGun, subObject),Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType);
			if (bReloadType == RELOAD_TOPOFF)
			{
				ubBulletsToMove = __min( (*pAmmo)[ammoObject]->data.ubShotsLeft, GetMagSize(pGun, subObject) - (*pGun)[subObject]->data.gun.ubGunShotsLeft );
			}
			else
			{
				ubBulletsToMove = __min( (*pAmmo)[ammoObject]->data.ubShotsLeft, GetMagSize(pGun, subObject) );
			}
		}
		else // mag is smaller than weapon mag
		{
//MADD MARKER
			//usNewAmmoItem = pAmmo->usItem + 1;
			usNewAmmoItem = FindReplacementMagazine(Weapon[pGun->usItem].ubCalibre ,GetMagSize(pGun, subObject),Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType);
			if (bReloadType == RELOAD_TOPOFF)
			{
				ubBulletsToMove = __min( (*pAmmo)[ammoObject]->data.ubShotsLeft, GetMagSize(pGun, subObject) - (*pGun)[subObject]->data.gun.ubGunShotsLeft );
			}
			else
			{
				ubBulletsToMove = __min( (*pAmmo)[ammoObject]->data.ubShotsLeft, GetMagSize(pGun, subObject));
			}
		}

		//CHRIS: This should reset the number of bullest moved to what we can actually afford when loading loose rounds
		if(Weapon[pGun->usItem].swapClips == 0 && (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT))
		{
			if(fEnoughAPs)
			{
				bAPs = GetAPsToReloadGunWithAmmo( pSoldier, pGun, pAmmo, FALSE );
			}
			else
			{
				//how many can we reload?  remember, bAPs assumes 1 round at this stage
				bAPs = GetAPsToReloadGunWithAmmo( pSoldier, pGun, pAmmo, 2 );
				for(int i = 0; i<ubBulletsToMove; i++)
				{
					if(EnoughPoints(pSoldier, (bAPs+(i*APBPConstants[AP_RELOAD_LOOSE])), 0, FALSE) == FALSE)
					{
						ubBulletsToMove = i+1;
						break;
					}
				}
				bAPs = GetAPsToReloadGunWithAmmo( pSoldier, pGun, pAmmo );
			}
		}

		switch( bReloadType )
		{

			case RELOAD_PLACE:
				(*pGun)[subObject]->data.gun.ubGunShotsLeft = ubBulletsToMove;
				(*pGun)[subObject]->data.gun.ubGunAmmoType = Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType;
				(*pGun)[subObject]->data.gun.usGunAmmoItem = usNewAmmoItem;
				break;

			case RELOAD_SWAP:
				(*pGun)[subObject]->data.gun.ubGunShotsLeft = ubBulletsToMove;
				(*pGun)[subObject]->data.gun.ubGunAmmoType = Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType;
				(*pGun)[subObject]->data.gun.usGunAmmoItem = usNewAmmoItem;
				if (fReloadingWithStack)
				{
					// add to end of stack
					pAmmo->AddObjectsToStack( gTempObject, 1 );
				}
				else
				{
					// Copying the old ammo to the cursor in turnbased could screw up for the player
					// (suppose his inventory is full!)

					//ADB copying the old ammo to the cursor at any time will screw it up if the cursor ammo is a stack!
					if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) && !EnoughPoints( pSoldier, (INT8) (bAPs + GetBasicAPsToPickupItem( pSoldier )), 0, FALSE ) // SANDRO
						|| pAmmo->ubNumberOfObjects > 1)
					{
						// try autoplace
						if ( !AutoPlaceObject( pSoldier, &gTempObject, FALSE ) )
						{
							// put it on the ground
							AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );
						}
						// delete the object now in the cursor
						pAmmo->RemoveObjectsFromStack(1);
					}
					else
					{
						// copy the old ammo to the cursor
						*pAmmo = gTempObject;
					}
				}
				break;
			case RELOAD_AUTOPLACE_OLD:
				if ( !AutoPlaceObject( pSoldier, &gTempObject, TRUE ) )
				{
					// error msg!
					return( FALSE );
				}
				// place first ammo in gun
				(*pGun)[subObject]->data.gun.ubGunShotsLeft = ubBulletsToMove;
				(*pGun)[subObject]->data.gun.ubGunAmmoType = Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType;
				(*pGun)[subObject]->data.gun.usGunAmmoItem = usNewAmmoItem;

				break;

			case RELOAD_TOPOFF:
				// ADD that many bullets to gun
				(*pGun)[subObject]->data.gun.ubGunShotsLeft += ubBulletsToMove;
				break;

		}

		// CHRISL: If we don't completely reload a SwapClips==0 weapon, set the "reload" flag
		if(Weapon[pGun->usItem].swapClips == 0 && GetMagSize(pGun, subObject) != (*pGun)[0]->data.gun.ubGunShotsLeft)
		{
			(*pGun)[subObject]->data.gun.ubGunState |= GS_WEAPON_BEING_RELOADED;
			(*pGun)[subObject]->data.gun.ubGunState &= ~GS_CARTRIDGE_IN_CHAMBER;
		}
		else
		{
			(*pGun)[subObject]->data.gun.ubGunState &= ~GS_WEAPON_BEING_RELOADED;
			(*pGun)[subObject]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;
		}

		if ( ! ( bReloadType == RELOAD_SWAP && !fReloadingWithStack ) )
		{
			// remove # of bullets, delete 1 object if necessary

			(*pAmmo)[ammoObject]->data.ubShotsLeft -= ubBulletsToMove;
			if ((*pAmmo)[ammoObject]->data.ubShotsLeft == 0)
			{
				pAmmo->RemoveObjectAtIndex(ammoObject);
				//pAmmo->RemoveObjectsFromStack(1);
			}

		}
	}

	// OK, let's play a sound of reloading too...
	// If this guy is visible...
	if ( pSoldier->bVisible != -1 )
	{
		// Play some effects!
		usReloadSound	= Weapon[ pGun->usItem ].sReloadSound;

		if ( usReloadSound != 0 && !IsAutoResolveActive() )
		{
			PlayJA2Sample( usReloadSound, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		}
	}

	if (pSoldier->bTeam == gbPlayerNum)
	{
		// spit out a message if this is one of our folks reloading
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_PLAYER_RELOADS], pSoldier->name );
	}

	DeductPoints( pSoldier, bAPs, 0 );
	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//pGun->ubWeight = CalculateObjectWeight( pGun );

	if ( (*pGun)[subObject]->data.gun.bGunAmmoStatus >= 0 )
	{
		// make sure gun ammo status is 100, if gun isn't jammed
		(*pGun)[subObject]->data.gun.bGunAmmoStatus = 100;
	}

	//CHRISL: Move this towards the top so that we can leave this flag off if we're in the middle of reloading
	//(*pGun)[subObject]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER; // Madd: reloading should automatically put cartridge in chamber

	return( TRUE );
}

BOOLEAN EmptyWeaponMagazine( OBJECTTYPE * pWeapon, OBJECTTYPE *pAmmo, UINT32 subObject )
{
	UINT16 usReloadSound;

	CHECKF( pAmmo != NULL );

	if ( (*pWeapon)[subObject]->data.gun.ubGunShotsLeft > 0 )
	{
		CreateAmmo((*pWeapon)[subObject]->data.gun.usGunAmmoItem, pAmmo, (*pWeapon)[subObject]->data.gun.ubGunShotsLeft);

		(*pWeapon)[subObject]->data.gun.ubGunShotsLeft		= 0;
		(*pWeapon)[subObject]->data.gun.ubGunAmmoType	  = 0;
		// HEADROCK HAM 3.5: Leaving the ammo inside the gun causes EDB stats to display values as though the magazine
		// still gives some effects (like autopen reduction, range bonus, etcetera). I'm going to try to work around
		// this issue.
		(*pWeapon)[subObject]->data.gun.usGunAmmoItem = 0; // leaving the ammo item the same for auto-reloading purposes

		// Play some effects!
		usReloadSound	= Weapon[ pWeapon->usItem ].sReloadSound;

		if ( usReloadSound != 0 )
		{
			PlayJA2Sample( usReloadSound, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		}

		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//pWeapon->ubWeight = CalculateObjectWeight( pWeapon );
		// Pulmu bugfix:
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//pAmmo->ubWeight = CalculateObjectWeight( pAmmo );
		// Pulmu end:
		return( TRUE );
	}
	else
	{
		// HEADROCK HAM 3.5: Clear the ammo type and magazine on player command. This will remove all bonuses by
		// the ammo and allow viewing the gun's normal stats. It will also change the weapon's ammocolor back to grey.
		(*pWeapon)[subObject]->data.gun.ubGunAmmoType = 0;
		(*pWeapon)[subObject]->data.gun.usGunAmmoItem = 0;
		//CHRISL: Clear the contents of pAmmo just in case
		pAmmo->initialize();
		return( FALSE );
	}
}

// HEADROCK HAM 3.3: Added an additional argument which helps the program pick a magazine
// that matches the ammotype currently used in the weapon. This makes for much smarter
// ammo selection.
INT8 FindAmmo( SOLDIERTYPE * pSoldier, UINT8 ubCalibre, UINT16 ubMagSize, UINT8 ubAmmoType, INT8 bExcludeSlot )
{
	INT8				bLoop;
	INT8				capLoop = 0;
	UINT16				curCap = 0, stackCap = 0;
	BOOLEAN				found = FALSE;
	INVTYPE *		pItem;

	//CHRISL: Update this to search for the largest appropriate mag if ubMagSize = ANY_MAGSIZE

	for (bLoop = HANDPOS; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		//CHRISL: If in NIV, in combat and backpack is closed, don't look inside
		if(UsingNewAttachmentSystem() == true && (gTacticalStatus.uiFlags & INCOMBAT) && IsBackpackSlot(bLoop) == true && pSoldier->flags.ZipperFlag == FALSE)
			continue;

		if (pSoldier->inv[bLoop].exists() == true) {
			if (bLoop == bExcludeSlot)
			{
				continue;
			}
			pItem = &(Item[pSoldier->inv[bLoop].usItem]);
			if (pItem->usItemClass == IC_AMMO)
			{
				if (Magazine[pItem->ubClassIndex].ubCalibre == ubCalibre)
				{
					if(ubMagSize != ANY_MAGSIZE && Magazine[pItem->ubClassIndex].ubMagSize == ubMagSize)
					{
						found = TRUE;
						// looking for specific size.  return if found
						// Find fullest mag
						for(int i = 0; i<pSoldier->inv[bLoop].ubNumberOfObjects; i++)
						{
							stackCap = __max(stackCap, pSoldier->inv[bLoop][i]->data.ubShotsLeft);
						}
						// If found a similar-sized magazine to the best one found so far, but this new one
						// has the same ammotype as specified in the arguments, then this is a better choice!
						if(stackCap > curCap || 
							(stackCap == curCap && Magazine[pItem->ubClassIndex].ubAmmoType == ubAmmoType))
						{
							curCap = stackCap;
							capLoop = bLoop;
						}
						//return( bLoop );
					}
					else if(ubMagSize == ANY_MAGSIZE)
					{
						found = TRUE;
						// looking for any mag size.  find the largest
						if(Magazine[pItem->ubClassIndex].ubMagSize > curCap)
						{
							curCap = Magazine[pItem->ubClassIndex].ubMagSize;
							capLoop = bLoop;
						}
					}
				}
			}
		}
	}
	if(found == TRUE)
		return( capLoop );
	return( NO_SLOT );
}


INT8 FindAmmoToReload( SOLDIERTYPE * pSoldier, INT8 bWeaponIn, INT8 bExcludeSlot )
{
	OBJECTTYPE *	pObj;
	INT8					bSlot;

	if (pSoldier == NULL)
	{
		return( NO_SLOT );
	}
	pObj = &(pSoldier->inv[bWeaponIn]);

//<SB> manual recharge
	if ((*pObj)[0]->data.gun.ubGunShotsLeft && !((*pObj)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
		return bWeaponIn;
//</SB>

	if ( Item[pObj->usItem].usItemClass == IC_GUN && !Item[pObj->usItem].cannon )
	{
		// look for same ammo as before
		//bSlot = FindObjExcludingSlot( pSoldier, (*pObj)[0]->data.gun.usGunAmmoItem, bExcludeSlot );
		//if (bSlot != NO_SLOT)
		//{
			// reload using this ammo!
		//	return( bSlot );
		//}
		// look for any ammo that matches which is of the same calibre and magazine size
		bSlot = FindAmmo( pSoldier, Weapon[pObj->usItem].ubCalibre, GetMagSize(pObj), GetAmmoType(pObj), bExcludeSlot );
		if (bSlot != NO_SLOT)
		{
			return( bSlot );
		}
		else
		{
			// look for any ammo that matches which is of the same calibre (different size okay)
			return( FindAmmo( pSoldier, Weapon[pObj->usItem].ubCalibre, ANY_MAGSIZE, GetAmmoType(pObj), bExcludeSlot ) );
		}
	}
	else
	{
		for(int i=0;i<MAXITEMS;i++)
		{
			if ( Launchable[i][0] == 0 && Launchable[i][1] == 0)
				break;

			if ( pObj->usItem == Launchable[i][1] )
			{
				bSlot = FindObj( pSoldier, Launchable[i][0] );

				if ( bSlot != NO_SLOT )
					return bSlot;
			}
		}
		return NO_SLOT;

		//switch( pObj->usItem )
		//{
		//	case RPG7: // TODO: madd fix this
		//		return( FindObjInObjRange( pSoldier, RPG_HE_ROCKET, RPG_FRAG_ROCKET ) );
		//	case MORTAR:
		//		return( FindObj( pSoldier, MORTAR_SHELL ) );
		//	case TANK_CANNON:
		//		return( FindObj( pSoldier, TANK_SHELL ) );
		//	case GLAUNCHER:
		//	case UNDER_GLAUNCHER:
		//		return( FindObjInObjRange( pSoldier, GL_HE_GRENADE, GL_SMOKE_GRENADE ) );
		//	default:
		//		return( NO_SLOT );
		//}
	}
}

BOOLEAN AutoReload( SOLDIERTYPE * pSoldier )
{
	OBJECTTYPE *pObj, *pObj2;
	INT8		bSlot;
	INT16 bAPCost;
	BOOLEAN		fRet;

	CHECKF( pSoldier );
	pObj = &(pSoldier->inv[HANDPOS]);

//<SB> manual recharge
	if ((*pObj)[0]->data.gun.ubGunShotsLeft && !((*pObj)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
	{
		(*pObj)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;
		(*pObj)[0]->data.gun.ubGunState &= ~GS_WEAPON_BEING_RELOADED;

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// STOMP traits - SANDRO
		if ( gGameOptions.fNewTraitSystem )
		{
			// Sniper trait makes chambering a round faster
			if (( Weapon[Item[(pObj)->usItem].ubClassIndex].ubWeaponType == GUN_SN_RIFLE || Weapon[Item[(pObj)->usItem].ubClassIndex].ubWeaponType == GUN_RIFLE ) && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ))
				DeductPoints(pSoldier, ((Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually * (100 - gSkillTraitValues.ubSNChamberRoundAPsReduction * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )))/100), 0);
			// Ranger trait makes pumping shotguns faster
			else if (( Weapon[Item[(pObj)->usItem].ubClassIndex].ubWeaponType == GUN_SHOTGUN ) && HAS_SKILL_TRAIT( pSoldier, RANGER_NT ))
				DeductPoints(pSoldier, ((Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually * (100 - gSkillTraitValues.ubRAPumpShotgunsAPsReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT )))/100), 0);
			else
				DeductPoints(pSoldier, Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually, 0);
		}
		else
		{
			DeductPoints(pSoldier, Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually, 0);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////

		PlayJA2Sample( Weapon[ Item[pObj->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			pObj2 = &(pSoldier->inv[SECONDHANDPOS]);

			if ((*pObj2)[0]->data.gun.ubGunShotsLeft && !((*pObj2)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
			{
				(*pObj2)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;
				PlayJA2Sample( Weapon[ Item[pObj2->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
			}
		}

		return TRUE;
	}
	else
	{
		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			pObj2 = &(pSoldier->inv[SECONDHANDPOS]);

			if ((*pObj2)[0]->data.gun.ubGunShotsLeft && !((*pObj2)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
			{
				(*pObj2)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;

				////////////////////////////////////////////////////////////////////////////////////////////////////////
				// STOMP traits - SANDRO
				if ( gGameOptions.fNewTraitSystem )
				{
					// Sniper trait makes chambering a round faster
					if (( Weapon[Item[(pObj2)->usItem].ubClassIndex].ubWeaponType == GUN_SN_RIFLE || Weapon[Item[(pObj2)->usItem].ubClassIndex].ubWeaponType == GUN_RIFLE ) && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ))
						DeductPoints(pSoldier, ((Weapon[Item[(pObj2)->usItem].ubClassIndex].APsToReloadManually * (100 - gSkillTraitValues.ubSNChamberRoundAPsReduction * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )))/100), 0);
					// Ranger trait makes pumping shotguns faster
					else if (( Weapon[Item[(pObj2)->usItem].ubClassIndex].ubWeaponType == GUN_SHOTGUN ) && HAS_SKILL_TRAIT( pSoldier, RANGER_NT ))
						DeductPoints(pSoldier, ((Weapon[Item[(pObj2)->usItem].ubClassIndex].APsToReloadManually * (100 - gSkillTraitValues.ubRAPumpShotgunsAPsReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT )))/100), 0);
					else
						DeductPoints(pSoldier, Weapon[Item[(pObj2)->usItem].ubClassIndex].APsToReloadManually, 0);
				}
				else
				{
					DeductPoints(pSoldier, Weapon[Item[(pObj2)->usItem].ubClassIndex].APsToReloadManually, 0);
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////

				PlayJA2Sample( Weapon[ Item[pObj2->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

				return TRUE;
			}
		}
	}
//</SB>

	if (Item[pObj->usItem].usItemClass == IC_GUN || Item[pObj->usItem].usItemClass == IC_LAUNCHER)
	{
		bSlot = FindAmmoToReload( pSoldier, HANDPOS, NO_SLOT );
		if (bSlot != NO_SLOT)
		{
			// reload using this ammo!
			fRet = ReloadGun( pSoldier, pObj, &(pSoldier->inv[bSlot]) );
			// if we are valid for two-pistol shooting (reloading) and we have enough APs still
			// then do a reload of both guns!
			if ( (fRet == TRUE) && pSoldier->IsValidSecondHandShotForReloadingPurposes( ) )
			{
				pObj = &(pSoldier->inv[SECONDHANDPOS]);
				bSlot = FindAmmoToReload( pSoldier, SECONDHANDPOS, NO_SLOT );
				if (bSlot != NO_SLOT)
				{
					// ce would reload using this ammo!
					bAPCost = GetAPsToReloadGunWithAmmo( pSoldier, pObj, &(pSoldier->inv[bSlot] ) );
					if ( EnoughPoints( pSoldier, (INT16) bAPCost, 0, FALSE ) )
					{
						// reload the 2nd gun too
						fRet = ReloadGun( pSoldier, pObj, &(pSoldier->inv[bSlot]) );
					}
					else
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_RELOAD_ONLY_ONE_GUN ], pSoldier->name );
					}
				}
			}

			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			return( fRet );
		}
	}

	// couldn't reload
	return( FALSE );
}

INT8 GetAttachmentComboMerge( OBJECTTYPE * pObj, UINT8 subObject )
{
	INT8		bIndex = 0;
	INT8		bAttachLoop;

	/* check the whole Array of possible Attachements, while there are still entries ... */
	while( AttachmentComboMerge[ bIndex ].usItem != NOTHING )
	{
		/* if we found our current Object as "basic hand" item, then
     * we have found at least ONE entry of our item (may be there are more)
     */
		OBJECTTYPE* pAttachment = 0;
		if ( pObj->usItem == AttachmentComboMerge[ bIndex ].usItem )
		{
			// search for all the appropriate attachments
		  /* every ComboMerge must have at least one attachments Field */
			for ( bAttachLoop = 0; bAttachLoop < 2; bAttachLoop++ )
			{
				/* if the none of both Fields contains anything, do not merge */
				if ( AttachmentComboMerge[ bIndex ].usAttachment[ bAttachLoop ] == NOTHING )
				{
					continue;
				}

			/* 2007-05-27, Sgt_Kolja:
			 * do not return, but break the inner loop moved away, otherwise
			 * we can only have ONE attachmentCombo per basic item. F.I. if we want
			 * to make a Dart gun from Dart pistol by adding (a buttstock and) wheter a
			 * steel tube /or/ a Gun Barrel Extender, the old code wouldn't work for
			 * the Gun Barel Extender, since it would never been tested.
			 */
				pAttachment = FindAttachment( pObj, AttachmentComboMerge[ bIndex ].usAttachment[ bAttachLoop ], subObject );
				if ( pAttachment->exists() == false )
				{
					// didn't find something required
					break;
				}
			}
				// found everything required?
		  /* 2007-05-27, Sgt_Kolja: Not-found-condition moved from above, otherwise we can only have ONE attachmentCombo per basic item */
			//WarmSteel - Added check to see if the resulting item is valid.
			if ( pAttachment->exists() && ItemIsLegal(AttachmentComboMerge[bIndex].usResult, TRUE) )
			{
				return( bIndex );
			}
		} /* end-if-this-is-our-item */

		/* try next Attachment Order */
		bIndex++;
	}

	return( -1 );
}



void PerformAttachmentComboMerge( OBJECTTYPE * pObj, INT8 bAttachmentComboMerge )
{
	INT8		bAttachLoop;
	UINT32	uiStatusTotal = 0;
	INT8		bNumStatusContributors = 0;

	// This object has been validated as available for an attachment combo merge.
	// - find all attachments in list and destroy them
	// - status of new object should be average of items including attachments
	// - change object

	for ( bAttachLoop = 0; bAttachLoop < 2; bAttachLoop++ )
	{
		if ( AttachmentComboMerge[ bAttachmentComboMerge ].usAttachment[ bAttachLoop ] == NOTHING )
		{
			continue;
		}

		OBJECTTYPE* pAttachment = FindAttachment( pObj, AttachmentComboMerge[ bAttachmentComboMerge ].usAttachment[ bAttachLoop ] );
		AssertMsg( pAttachment != 0, String( "Attachment combo merge couldn't find a necessary attachment" ) );

		uiStatusTotal += (*pAttachment)[0]->data.objectStatus;
		bNumStatusContributors++;

		pObj->RemoveAttachment(pAttachment);
	}

	uiStatusTotal += (*pObj)[0]->data.objectStatus;
	bNumStatusContributors++;

	pObj->usItem = AttachmentComboMerge[ bAttachmentComboMerge ].usResult;
	RemoveProhibitedAttachments(NULL, pObj, pObj->usItem);
	(*pObj)[0]->data.objectStatus = (INT8) (uiStatusTotal / bNumStatusContributors );
}

BOOLEAN OBJECTTYPE::AttachObjectOAS( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound, UINT8 subObject )
{
	//CHRISL: This makes it impossible to add attachments to objects in a stack.  Let's remove this and make this possible.
	//if ( this->ubNumberOfObjects > 1 )
	//{
	//	return( FALSE );
	//}
	if (pAttachment->exists() == false) {
		return FALSE;
	}
	if ((*this)[subObject]->attachments.size() >= OLD_MAX_ATTACHMENTS_101) {
		return FALSE;
	}
	//CHRISL: If we're adding a loaded UGL, then we have to make sure there are actually 2 open attachment slots instead of 1
	if(Item[pAttachment->usItem].grenadelauncher && (*pAttachment)[0]->attachments.size() > 0) {
		if ((*this)[subObject]->attachments.size() >= (OLD_MAX_ATTACHMENTS_101-1))
			return FALSE;
	}

	static OBJECTTYPE attachmentObject;

	UINT16		usResult, usResult2, ubLimit;
	INT8		bLoop;
	UINT8		ubType, ubAPCost;
	INT32		iCheckResult;
	INT8		bAttachInfoIndex = -1, bAttachComboMerge;
	BOOLEAN		fValidLaunchable = FALSE;
	UINT16		oldMagSize = 0;

	//CHRISL: This is so we can try to determine if the removed attachment altered our mag size.
	if(Item[this->usItem].usItemClass == IC_GUN)
		oldMagSize = GetMagSize(this);

	bool canOnlyAttach1 = false;

	//if this is an attachment or ammo for a launchable item
	fValidLaunchable = ValidLaunchable( pAttachment->usItem, this->usItem );
	//CHRISL: If we don't want to play the sound, it's a good bet we don't want to display any messages either
	if ( fValidLaunchable || ValidItemAttachment( this, pAttachment->usItem, TRUE, playSound, subObject ) )
	{
		//if there is already an attachment of the same type, we want to try swapping / replacing it
		OBJECTTYPE* pAttachmentPosition = 0;

		// find an attachment position...
		// second half of this 'if' is for attaching GL grenades to a gun w/attached GL
		if ( fValidLaunchable || (Item[pAttachment->usItem].glgrenade && FindAttachmentByClass(this, IC_LAUNCHER, subObject) != 0 ) )
		{
			canOnlyAttach1 = true;
			// try replacing if possible
			pAttachmentPosition = FindAttachmentByClass( this, Item[ pAttachment->usItem ].usItemClass, subObject );
		}
		else
		{
			// try replacing if possible
			pAttachmentPosition = FindAttachment( this, pAttachment->usItem, subObject );
		}

		if ( pSoldier )
		{
			//did the soldier damage it?
			bAttachInfoIndex = GetAttachmentInfoIndex( pAttachment->usItem );
			// in-game (not behind the scenes) attachment
			if ( bAttachInfoIndex != -1 && AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheck != NO_CHECK )
			{
				iCheckResult = SkillCheck( pSoldier, AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheck, AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod );
				if (iCheckResult < 0)
				{
					// the attach failure damages both items
					DamageObj( this, (INT8) -iCheckResult, subObject );
					DamageObj( pAttachment, (INT8) -iCheckResult, subObject );

					// there should be a quote here!
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
					if ( gfInItemDescBox )
					{
						DeleteItemDescriptionBox();
					}
					return( FALSE );
				}
			}

			if ( ValidItemAttachment( this, pAttachment->usItem, TRUE, TRUE, subObject  ) && playSound ) // not launchable
			{
				// attachment sounds
				if ( Item[ this->usItem ].usItemClass & IC_WEAPON )
				{
					PlayJA2Sample( ATTACH_TO_GUN, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else if ( Item[ this->usItem ].usItemClass & IC_ARMOUR )
				{
					PlayJA2Sample( ATTACH_CERAMIC_PLATES, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else if ( Item[ this->usItem ].usItemClass & IC_BOMB )
				{
					PlayJA2Sample( ATTACH_DETONATOR, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
			}
		}

		if (pAttachmentPosition) {
			if (canOnlyAttach1 == true) {
				if(pAttachment->ubNumberOfObjects > 1)
				{
					//We don't want to inadvertantly load two rounds during a swap so don't allow a swap if we're holding a stack
					return(FALSE);
				}
				//we have requested to swap the attachment on the cursor with the current attachment
				SwapObjs(pAttachmentPosition,pAttachment);
				return(TRUE);
			}
		}

		//ADB moved after skill check!
		//lalien: added to make a GL/RL work when reloaded manually
		// the AP costs for reloading GL/RL will be taken from weapons.xml ( wrong place!!! the AP's are deducted in DeleteItemDescriptionBox() )
		//if (pAttachmentPosition) {
		if(pAttachmentPosition || (pAttachmentPosition == NULL && (*this)[subObject]->attachments.size() < OLD_MAX_ATTACHMENTS_101)){
			//we know we are replacing this attachment
			if ( Item[ this->usItem ].usItemClass == IC_LAUNCHER || Item[this->usItem].cannon )
			{
				if ( fValidLaunchable )
				{
					(*this)[subObject]->data.gun.usGunAmmoItem = pAttachment->usItem;
					//we have reloaded a launchable, so the ammo is gone from the original object
				}
			}
		}

		//unfortunately must come before possible attachment swap
		if (Item[pAttachment->usItem].grenadelauncher )
		{
			// transfer any attachments from the grenade launcher to the gun
			(*this)[subObject]->attachments.splice((*this)[subObject]->attachments.begin(), (*pAttachment)[0]->attachments,
				(*pAttachment)[0]->attachments.begin(), (*pAttachment)[0]->attachments.end());
		}

		if (pAttachmentPosition) {
			//we are swapping the attachments, and we know we do NOT need to worry about attachment stack size
			//CHRISL: Actually, we do need to worry about attachment stack size since we might have a stack in our cursor.
			//	Rather then doing a simple swap, try moving the existing attachment to our cursor stack, then attach one item
			//	from the cursor stack.

			pAttachment->AddObjectsToStack(*pAttachmentPosition,-1,pSoldier,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT);
			pAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1);
			*pAttachmentPosition = attachmentObject;
			//backup the original attachment
			//attachmentObject = *pAttachmentPosition;

			//place the new one
			//*pAttachmentPosition = *pAttachment;

			//whatever pAttachment pointed to is now the original attachment
			//*pAttachment = attachmentObject;
		}
		else {
			//it's a new attachment
			if (canOnlyAttach1 == true) {
				//we only placed one of the stack, pAttachment could have any number of objects
				if (pAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1) == 0) {
					(*this)[subObject]->attachments.push_back(attachmentObject);
				}
			}
			else {
				//pAttachment could have any number of objects, they have all been moved over
				//CHRISL: This doesn't work.  What if we have a stack of objects in the cursor?  We don't want the whole
				//	stack to be attached
				//(*this)[subObject]->attachments.push_back(*pAttachment);
				//DeleteObj(pAttachment);
				if (pAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1) == 0) {
					(*this)[subObject]->attachments.push_back(attachmentObject);
				}
			}
		}

		//Ammo might have changed after adding an attachment.
		if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[subObject]->data.gun.usGunAmmoItem != NONE && (*this)[subObject]->data.gun.ubGunShotsLeft > 0 && oldMagSize != GetMagSize(this, subObject) )
		{
			if ( (*this)[subObject]->data.gun.ubGunShotsLeft > GetMagSize(this, subObject) )
			{ //Too many rounds, eject ammo
				EjectAmmoAndPlace(pSoldier, this, subObject);
			}
			//CHRISL: We should update the usGunAmmoItem if we've changed the ammo capacity
			if((*this)[subObject]->data.gun.usGunAmmoItem != NONE){
				UINT16 usNewAmmoItem;
				usNewAmmoItem = FindReplacementMagazine(Weapon[this->usItem].ubCalibre ,GetMagSize(this, subObject),Magazine[Item[(*this)[subObject]->data.gun.usGunAmmoItem].ubClassIndex].ubAmmoType);
				(*this)[subObject]->data.gun.usGunAmmoItem = usNewAmmoItem;
			}
		}

		if(Item[this->usItem].usItemClass == IC_GUN && oldMagSize != GetMagSize(this, subObject)){
			fInterfacePanelDirty = DIRTYLEVEL2;
			RenderBulletIcon(this, subObject);
		}

		// Check for attachment merge combos here
		//CHRISL: Only do this if we're looking at a single item.  Don't try a combo merge when dealing with a stack
		bAttachComboMerge = GetAttachmentComboMerge( this, subObject );
		if ( bAttachComboMerge != -1 )
		{
			if(this->ubNumberOfObjects == 1)
			{
				PerformAttachmentComboMerge( this, bAttachComboMerge );
				if ( bAttachInfoIndex != -1 && AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod < 20 )
				{
					StatChange( pSoldier, MECHANAMT, (INT8) ( 20 - AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod ), FALSE );
					StatChange( pSoldier, WISDOMAMT, (INT8) ( 20 - AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod ), FALSE );
				}
			}
			//CHRISL: If we're looking at a stack, we need to unattach the object we just attached
			else if(this->RemoveAttachment(&attachmentObject, pAttachmentPosition, subObject))
			{
				pAttachment->AddObjectsToStack(attachmentObject, -1, pSoldier, NUM_INV_SLOTS, MAX_OBJECTS_PER_SLOT);
			}
		}

		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = CalculateObjectWeight( this );
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//pAttachment->ubWeight = CalculateObjectWeight(pAttachment);

		if ( pSoldier != NULL )
			ApplyEquipmentBonuses(pSoldier);

		return( TRUE );
	}
	// check for merges
	else if (EvaluateValidMerge( pAttachment->usItem, this->usItem, &usResult, &usResult2, &ubType, &ubAPCost ))
	{
		//CHRISL: we don't want to try to merge IC_LBEGEAR items that are currently storing items.
		if(Item[this->usItem].usItemClass == IC_LBEGEAR)
		{
			if(this->IsActiveLBE(subObject) == true)
				return( FALSE );
		}
		//CHRISL: We don't want to do any merges if we're looking at a stack of items, with the exception of combines.
		if ( this->ubNumberOfObjects > 1 && ubType != COMBINE_POINTS )
		{
			return( FALSE );
		}
		if ( ubType != COMBINE_POINTS )
		{
			if ( pSoldier )
			{
				if ( !EnoughPoints( pSoldier, ubAPCost, 0, TRUE ) )
				{
					return( FALSE );
				}

				//lalien: don't charge AP's for reloading a GL/RL ( wrong place!!! the AP's are deducted in DeleteItemDescriptionBox() )
				//if ( IsAGLorRL == FALSE )
				//{
					DeductPoints( pSoldier, ubAPCost, 0 );
				//}
			}
		}

		switch( ubType )
		{
			case USE_ITEM:
			case USE_ITEM_HARD:
				// the merge will combine the two items
				if ( pSoldier )
				{
					if ( ubType == USE_ITEM_HARD )
					{
						// requires a skill check, and gives experience
						iCheckResult = SkillCheck( pSoldier, ATTACHING_SPECIAL_ITEM_CHECK, -30 );
						if (iCheckResult < 0)
						{
							// could have a chance of detonation
							// for now, damage both objects
							DamageObj( this, (INT8) -iCheckResult );
							DamageObj( pAttachment, (INT8) -iCheckResult );
							pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
							return( FALSE );
						}
						StatChange( pSoldier, MECHANAMT, 25, FALSE );
						StatChange( pSoldier, WISDOMAMT, 5, FALSE );

						// SANDRO - merc records - merging items
						if ( pSoldier->ubProfile != NO_PROFILE )
							gMercProfiles[ pSoldier->ubProfile ].records.usItemsCombined++;
					}

					//Madd: note that use_item cannot produce two different items!!! so it doesn't use usResult2

					//Madd: unload guns after merge if ammo caliber or mag size don't match
					if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[0]->data.gun.usGunAmmoItem != NONE && (*this)[0]->data.gun.ubGunShotsLeft > 0 )
					{
						if ( Item[usResult].usItemClass != IC_GUN || Weapon[Item[usResult].ubClassIndex].ubCalibre != Weapon[Item[this->usItem].ubClassIndex].ubCalibre || (*this)[0]->data.gun.ubGunShotsLeft > Weapon[Item[usResult].ubClassIndex].ubMagSize )
						{ // item types/calibers/magazines don't match, spit out old ammo
							EjectAmmoAndPlace(pSoldier, this);
						}
					}

					RemoveProhibitedAttachments(pSoldier, this, usResult);

					this->usItem = usResult;
					//AutoPlaceObject( pAttachment );
					//ADB ubWeight has been removed, see comments in OBJECTTYPE
					//this->ubWeight = CalculateObjectWeight( this );
					if (pSoldier->bTeam == gbPlayerNum)
					{
						pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
					}
					ApplyEquipmentBonuses(pSoldier);
					return TRUE;
				}
				else
					return FALSE;
			case COMBINE_POINTS:
				// transfer points...
				UINT32 combinedAmount;
				UINT32 attachmentAmount;
				//CHRISL: Treat differently if we're combining money items (other then MONEY)
				if(Item[this->usItem].usItemClass != IC_MONEY)
				{
					if ( Item[ this->usItem ].usItemClass == IC_AMMO )
					{
						ubLimit = Magazine[ Item[ this->usItem ].ubClassIndex ].ubMagSize;
					}
					else
					{
						ubLimit = 100;
					}

					// count down through # of attaching items and add to status of item in position 0
					for (bLoop = 0; bLoop < pAttachment->ubNumberOfObjects; )
					{
						//ADB need to watch for overflow here (thus UINT32), and need to cast to UINT8 before adding
						combinedAmount = (UINT16)(*this)[subObject]->data.objectStatus + (UINT16)(*pAttachment)[0]->data.objectStatus;
						if (combinedAmount <= ubLimit)
						{
							// consume this one totally and continue
							(*this)[subObject]->data.objectStatus = combinedAmount;
							pAttachment->RemoveObjectsFromStack(1);
						}
						else
						{
							// add part of this one and then we're done
							attachmentAmount = (UINT16)(*pAttachment)[0]->data.objectStatus;
							attachmentAmount -= (ubLimit - (UINT16)(*this)[subObject]->data.objectStatus);
							(*pAttachment)[0]->data.objectStatus = attachmentAmount;
							if ((*pAttachment)[0]->data.ubShotsLeft == 0) {
								pAttachment->RemoveObjectsFromStack(1);
							}
							(*this)[subObject]->data.ubShotsLeft = ubLimit;
							break;
						}
					}
				}
				else
				{
					ubLimit = MAX_MONEY_PER_SLOT;
					combinedAmount = (*this)[subObject]->data.money.uiMoneyAmount + (*pAttachment)[0]->data.money.uiMoneyAmount;
					if (combinedAmount <= ubLimit)
					{
						(*this)[subObject]->data.money.uiMoneyAmount = combinedAmount;
						pAttachment->RemoveObjectsFromStack(1);
					}
					else
					{
						attachmentAmount = (*pAttachment)[0]->data.money.uiMoneyAmount;
						attachmentAmount -= (ubLimit - (*this)[subObject]->data.money.uiMoneyAmount);
						(*pAttachment)[0]->data.money.uiMoneyAmount = attachmentAmount;
						if((*pAttachment)[0]->data.money.uiMoneyAmount == 0)
						{
							pAttachment->RemoveObjectsFromStack(1);
						}
						(*this)[subObject]->data.money.uiMoneyAmount = ubLimit;
						break;
					}
				}
				break;
			case DESTRUCTION:
				// the merge destroyed both items!
				this->RemoveObjectsFromStack(1);
				pAttachment->RemoveObjectsFromStack(1);
				if ( pSoldier )
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
				}
				break;
			case ELECTRONIC_MERGE:
				if ( pSoldier )
				{
					iCheckResult = SkillCheck( pSoldier, ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK, -30 );
					if ( iCheckResult < 0 )
					{
						DamageObj( this, (INT8) -iCheckResult );
						DamageObj( pAttachment, (INT8) -iCheckResult );
						pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
						return( FALSE );
					}
					// grant experience! ... SANDRO - so what?! Grant them already!
					StatChange( pSoldier, MECHANAMT, 30, FALSE );
					StatChange( pSoldier, WISDOMAMT, 10, FALSE );
					
					// SANDRO - merc records - merging items
					if ( pSoldier->ubProfile != NO_PROFILE )
						gMercProfiles[ pSoldier->ubProfile ].records.usItemsCombined++;
				}
				// fall through
			case EXPLOSIVE:
				if ( ubType == EXPLOSIVE ) /// coulda fallen through
				{
					if (pSoldier)
					{
						// requires a skill check, and gives experience
						iCheckResult = SkillCheck( pSoldier, ATTACHING_DETONATOR_CHECK, -30 );
						if (iCheckResult < 0)
						{
							// could have a chance of detonation
							// for now, damage both objects
							DamageObj( this, (INT8) -iCheckResult );
							DamageObj( pAttachment, (INT8) -iCheckResult );
							pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
							return( FALSE );
						}
						StatChange( pSoldier, EXPLODEAMT, 25, FALSE );
						StatChange( pSoldier, WISDOMAMT, 5, FALSE );

						// SANDRO - merc records - merging items
						if ( pSoldier->ubProfile != NO_PROFILE )
							gMercProfiles[ pSoldier->ubProfile ].records.usItemsCombined++;
					}
				}
				// fall through
			default:
				// the merge will combine the two items
				//Madd: usResult2 only works for standard merges->item1 + item2 = item3 + item4

				//Madd: unload guns after merge if ammo caliber or mag size don't match
				if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[subObject]->data.gun.usGunAmmoItem != NONE && (*this)[subObject]->data.gun.ubGunShotsLeft > 0 )
				{
					if ( Item[usResult].usItemClass != IC_GUN || Weapon[Item[usResult].ubClassIndex].ubCalibre != Weapon[Item[this->usItem].ubClassIndex].ubCalibre || (*this)[subObject]->data.gun.ubGunShotsLeft > Weapon[Item[usResult].ubClassIndex].ubMagSize )
					{ // item types/calibers/magazines don't match, spit out old ammo
						EjectAmmoAndPlace(pSoldier, this);
					}
				}

				RemoveProhibitedAttachments(pSoldier, this, usResult);

				this->usItem = usResult;
				if ( ubType != TREAT_ARMOUR )
				{
					(*this)[subObject]->data.objectStatus = ((*this)[subObject]->data.objectStatus + (*pAttachment)[0]->data.objectStatus) / 2;
				}

				//ADB ubWeight has been removed, see comments in OBJECTTYPE
				//this->ubWeight = CalculateObjectWeight( this );

				if ( usResult2 != NOTHING )
				{
					//Madd: usResult2 is what the original attachment/source item turns into

					//Madd: unload guns after merge if ammo caliber or mag size don't match
					if ( Item[pAttachment->usItem].usItemClass == IC_GUN && (*pAttachment)[0]->data.gun.usGunAmmoItem != NONE && (*pAttachment)[0]->data.gun.ubGunShotsLeft > 0 )
					{
						if ( Item[usResult2].usItemClass != IC_GUN || Weapon[Item[usResult2].ubClassIndex].ubCalibre != Weapon[Item[pAttachment->usItem].ubClassIndex].ubCalibre || (*pAttachment)[0]->data.gun.ubGunShotsLeft > Weapon[Item[usResult2].ubClassIndex].ubMagSize )
						{ // item types/calibers/magazines don't match, spit out old ammo
							EjectAmmoAndPlace(pSoldier, pAttachment);
						}
					}

					RemoveProhibitedAttachments(pSoldier, pAttachment, usResult2);

					pAttachment->usItem = usResult2;
					if ( ubType != TREAT_ARMOUR )
					{
						(*pAttachment)[0]->data.objectStatus = ((*pAttachment)[0]->data.objectStatus + (*this)[subObject]->data.objectStatus) / 2;
					}
					//ADB ubWeight has been removed, see comments in OBJECTTYPE
					//pAttachment->ubWeight = CalculateObjectWeight( pAttachment );
				}
				else
					pAttachment->RemoveObjectsFromStack(1);

				if (pSoldier && pSoldier->bTeam == gbPlayerNum)
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
				}
				break;
			}
			if ( pSoldier != NULL )
				ApplyEquipmentBonuses(pSoldier);
			return( TRUE );
	}
	return( FALSE );
}

BOOLEAN OBJECTTYPE::AttachObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound, UINT8 subObject, INT32 iItemPos, BOOLEAN fRemoveProhibited, std::vector<UINT16> usAttachmentSlotIndexVector )
{	
	if(UsingNewAttachmentSystem()==true){
		return( this->AttachObjectNAS(pSoldier, pAttachment, playSound, subObject, iItemPos, fRemoveProhibited, usAttachmentSlotIndexVector ) );
	} else {
		return( this->AttachObjectOAS( pSoldier, pAttachment, playSound, subObject) );
	}
}


BOOLEAN OBJECTTYPE::AttachObjectNAS( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound, UINT8 subObject, INT32 iItemPos, BOOLEAN fRemoveProhibited, std::vector<UINT16> usAttachmentSlotIndexVector )
{

	if (pAttachment->exists() == false) {
		return FALSE;
	}

	UINT16		usResult, usResult2, ubLimit;
	INT8		bLoop;
	UINT8		ubType, ubAPCost;
	INT32		iCheckResult;
	INT8		bAttachInfoIndex = -1, bAttachComboMerge;
	BOOLEAN		fValidLaunchable = FALSE;
	BOOLEAN		fValidItemAttachment = FALSE;
	INT32		curSlot = 0;
	UINT16		oldMagSize = 0;

	//if there is already an attachment of the same type, we want to try swapping / replacing it
	OBJECTTYPE* pAttachmentPosition = 0;
	//OBJECTTYPE* pAttachInSlot = 0;

	static OBJECTTYPE attachmentObject;

	//CHRISL: This is so we can try to determine if the removed attachment altered our mag size.
	if(Item[this->usItem].usItemClass == IC_GUN)
		oldMagSize = GetMagSize(this);

	//if this is an attachment or ammo for a launchable item
	fValidLaunchable = ValidLaunchable( pAttachment->usItem, this->usItem );

	//WarmSteel - if uiItemPos is -1, we're not checking for a specific slot, but scrolling through them all.
	if(iItemPos == -1){

		//WarmSteel - repeat this for every slot
		for(curSlot = 0; curSlot < (INT32)(*this)[0]->attachments.size(); curSlot++)
		{	
			fValidItemAttachment = ValidItemAttachmentSlot( this, pAttachment->usItem, TRUE, FALSE, subObject, curSlot, 0, 0, usAttachmentSlotIndexVector);

			if(fValidItemAttachment){
				//WarmSteel - apparently we found a attachable slot, exiting here will remember the curSlot
				break;
			}
		}

		//WarmSteel - if we didn't find any good slot, always just attach to the first slot. (because some functions will overrule this one).
		//It's basically just a general check so that we NEVER try to attach outside the attachment list range.
		if(curSlot == (*this)[subObject]->attachments.size())
			curSlot = 0;

	} else {
		//WarmSteel - We know in what slot we're trying to put this, so lets see if we can.
		curSlot = iItemPos;
		fValidItemAttachment = ValidItemAttachmentSlot( this, pAttachment->usItem, TRUE, playSound, subObject, curSlot, FALSE, &pAttachmentPosition, usAttachmentSlotIndexVector);
		//pAttachmentPosition = pAttachInSlot;
		// try replacing if possible
	}

	//CHRISL: If we don't want to play the sound, it's a good bet we don't want to display any messages either
	if (/*fValidLaunchable ||*/ fValidItemAttachment )
	{
		if ( pSoldier )
		{
			//did the soldier damage it?
			bAttachInfoIndex = GetAttachmentInfoIndex( pAttachment->usItem );
			// in-game (not behind the scenes) attachment
			if ( bAttachInfoIndex != -1 && AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheck != NO_CHECK )
			{
				iCheckResult = SkillCheck( pSoldier, AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheck, AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod );
				if (iCheckResult < 0)
				{
					// the attach failure damages both items
					DamageObj( this, (INT8) -iCheckResult, subObject );
					DamageObj( pAttachment, (INT8) -iCheckResult, subObject );

					// there should be a quote here!
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
					if ( gfInItemDescBox )
					{
						//DeleteItemDescriptionBox();
					}
					return( FALSE );
				}
			}

			if (fValidItemAttachment && playSound ) // not launchable
			{
				// attachment sounds
				if ( Item[ this->usItem ].usItemClass & IC_WEAPON )
				{
					PlayJA2Sample( ATTACH_TO_GUN, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else if ( Item[ this->usItem ].usItemClass & IC_ARMOUR )
				{
					PlayJA2Sample( ATTACH_CERAMIC_PLATES, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else if ( Item[ this->usItem ].usItemClass & IC_BOMB )
				{
					PlayJA2Sample( ATTACH_DETONATOR, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
			}
		}

		/*	//NAS doesn't care about single attachments.
		if (pAttachmentPosition) {
			if (canOnlyAttach1 == true) {
				if(pAttachment->ubNumberOfObjects > 1)
				{
					//We don't want to inadvertantly load two rounds during a swap so don't allow a swap if we're holding a stack
					return(FALSE);
				}
				//we have requested to swap the attachment on the cursor with the current attachment
				SwapObjs(pAttachmentPosition,pAttachment);
				return(TRUE);
			}
		}*/

		//ADB moved after skill check!
		//lalien: added to make a GL/RL work when reloaded manually
		// the AP costs for reloading GL/RL will be taken from weapons.xml ( wrong place!!! the AP's are deducted in DeleteItemDescriptionBox() )

		//we know we are replacing this attachment
		if ( Item[ this->usItem ].usItemClass == IC_LAUNCHER || Item[this->usItem].cannon )
		{
			if ( fValidLaunchable )
			{
				(*this)[subObject]->data.gun.usGunAmmoItem = pAttachment->usItem;
				//we have reloaded a launchable, so the ammo is gone from the original object
			}
		}

		/* //WarmSteel - splice won't work with attachments anymore, because of the null attachments. Replaced this function somewhere lower, because it needs to be done after the attaching of the GL.
		//Unfortunately this must come before any attachment swap
		if (Item[pAttachment->usItem].grenadelauncher && gGameOptions.ubAttachmentSystem == ATTACHMENT_OLD)
		{
			// transfer any attachments from the grenade launcher to the gun
			(*this)[subObject]->attachments.splice((*this)[subObject]->attachments.begin(), (*pAttachment)[0]->attachments,
				(*pAttachment)[0]->attachments.begin(), (*pAttachment)[0]->attachments.end());
		}*/

		//WarmSteel - Attachment swapping prevents multiple objects that are the same to be attached.
		//This takes away freedom, which is bad. This is why I turned it off in NAS (If you want to turn it back on, it also doesn't work properly because it needs to swap xml slot indexes aswell).
		if (pAttachmentPosition->exists()) {
			//we are swapping the attachments, and we know we do NOT need to worry about attachment stack size
			//CHRISL: Actually, we do need to worry about attachment stack size since we might have a stack in our cursor.
			//	Rather then doing a simple swap, try moving the existing attachment to our cursor stack, then attach one item
			//	from the cursor stack.

			//CHRISL: Try to replace illegal IC_GRENADE items with legal ones
			if(!ItemIsLegal(pAttachment->usItem) && Item[pAttachment->usItem].usItemClass & IC_GRENADE){
				OBJECTTYPE*	newAttachment = new OBJECTTYPE;
				UINT16		newItem;
				newItem = FindLegalGrenade(pAttachment->usItem);
				if(newItem != pAttachment->usItem){
					CreateItem(newItem, 100, newAttachment);
					(*pAttachment)[0]->data.objectStatus -= (INT16)(100/Weapon[this->usItem].ubMagSize);
					if((*pAttachment)[0]->data.objectStatus <= 0 )
						pAttachment->RemoveObjectsFromStack(1);
					newAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1);
				} else {
					pAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1);
				}
			} else {
				pAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1);
			}
			if(pAttachment->ubNumberOfObjects > 0){
				if (pSoldier) {
					if ( !AutoPlaceObject( pSoldier, pAttachmentPosition, FALSE ) )
					{   // put it on the ground
						AddItemToPool( pSoldier->sGridNo, pAttachmentPosition, 1, pSoldier->pathing.bLevel, 0 , -1 );
					}
				}
			} else {
				pAttachment->AddObjectsToStack(*pAttachmentPosition,-1,pSoldier,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT);
			}
			*pAttachmentPosition = attachmentObject;

			(*this)[subObject]->AddAttachmentAtIndex(curSlot, attachmentObject);

			//backup the original attachment
			//attachmentObject = *pAttachmentPosition;

			//place the new one
			//*pAttachmentPosition = *pAttachment;

			//whatever pAttachment pointed to is now the original attachment
			//*pAttachment = attachmentObject;
		}
		else {
			//it's a new attachment

			//pAttachment could have any number of objects, they have all been moved over
			//CHRISL: This doesn't work.  What if we have a stack of objects in the cursor?  We don't want the whole
			//	stack to be attached
			//(*this)[subObject]->attachments.push_back(*pAttachment);
			//DeleteObj(pAttachment);
			//CHRISL: Try to replace illegal IC_GRENADE items with legal ones
			if(!ItemIsLegal(pAttachment->usItem) && Item[pAttachment->usItem].usItemClass & IC_GRENADE){
				OBJECTTYPE*	newAttachment = new OBJECTTYPE;
				UINT16		newItem;
				newItem = FindLegalGrenade(pAttachment->usItem);
				if(newItem != pAttachment->usItem){
					CreateItem(newItem, 100, newAttachment);
					(*pAttachment)[0]->data.objectStatus -= (INT16)(100/Weapon[this->usItem].ubMagSize);
					if((*pAttachment)[0]->data.objectStatus <= 0 )
						pAttachment->RemoveObjectsFromStack(1);
					newAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1);
				} else {
					pAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1);
				}
			} else {
				pAttachment->MoveThisObjectTo(attachmentObject,1,pSoldier,NUM_INV_SLOTS,1);
			}

				//WarmSteel - Because we want every attachment to stay in place in NAS, we must first delete the "null" attachment, then insert the new one.
				//We know this attachment slot has a non-null object, otherwise ValidItemAttachmentSlot would not have returned true and we would not be here.
			(*this)[subObject]->AddAttachmentAtIndex(curSlot, attachmentObject);
		}


		//Ammo might have changed after adding an attachment.
		if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[subObject]->data.gun.usGunAmmoItem != NONE && (*this)[subObject]->data.gun.ubGunShotsLeft > 0 && oldMagSize != GetMagSize(this, subObject) )
		{
			if ( (*this)[subObject]->data.gun.ubGunShotsLeft > GetMagSize(this, subObject) )
			{ //Too many rounds, eject ammo
				EjectAmmoAndPlace(pSoldier, this, subObject);
			}
			//CHRISL: We should update the usGunAmmoItem if we've changed the ammo capacity
			if((*this)[subObject]->data.gun.usGunAmmoItem != NONE){
				UINT16 usNewAmmoItem;
				usNewAmmoItem = FindReplacementMagazine(Weapon[this->usItem].ubCalibre ,GetMagSize(this, subObject),Magazine[Item[(*this)[subObject]->data.gun.usGunAmmoItem].ubClassIndex].ubAmmoType);
				(*this)[subObject]->data.gun.usGunAmmoItem = usNewAmmoItem;
			}
		}

		if(Item[this->usItem].usItemClass == IC_GUN && oldMagSize != GetMagSize(this, subObject)){
			fInterfacePanelDirty = DIRTYLEVEL2;
			RenderBulletIcon(this, subObject);
		}

		//WarmSteel - If we have just attached a UGL, remove it's grenade and put it on the gun itself
		//CHRISL: This section of code is also needed it we add any attachment that changes the valid attachments our item can use, so we should run it whenever we
		//	add an attachment in NAS
		//if (FindAttachment_GrenadeLauncher(this)->exists() && attachmentObject.exists())
		if (attachmentObject.exists() && attachmentObject[0]->attachments.size() > 0 && FindAttachment(this, attachmentObject.usItem, subObject)->exists())
		{
			//Make sure it's actually on that gun..
			//if(FindAttachment_GrenadeLauncher(this)->usItem == attachmentObject.usItem){
			if(FindAttachment(this, attachmentObject.usItem, subObject)->usItem == attachmentObject.usItem){
				// transfer the grenade from the grenade launcher to the gun

				//To know wether this grenade is valid, we need to correct the slots, because they may have changed when attaching the UGL
				//Have to do it here, because from now on we don't need attachmentObject anymore.
				//CHRISL: Because we want this function to be useful for more then just UGLs, and because there is the probability that the attaching item will
				//	give us more attachment points, we need to just remove all attachments, tack them to the end of the attachmentList, and then go through the
				//	RemoveProhibitedAttachments function to force everything to store in the correct slot
				OBJECTTYPE * pGrenade = FindAttachment(this, attachmentObject.usItem, subObject);
				for(attachmentList::iterator iter = (*pGrenade)[0]->attachments.begin(); iter != (*pGrenade)[0]->attachments.end(); ++iter){
					if(iter->exists()){
						(*this)[subObject]->attachments.push_back((*iter));
						iter = (*pGrenade)[0]->RemoveAttachmentAtIter(iter);
						fRemoveProhibited = TRUE;
					}
				}
//				if(fRemoveProhibited){
//					RemoveProhibitedAttachments(pSoldier, this, this->usItem);
//				}

/*				CHRISL: None of this is needed.  Instead we strip any attachments from attachmentObject in the above loop.  
				for(INT16 sSlot = 0; sSlot < (INT16) (*this)[subObject]->attachments.size(); sSlot++){
					OBJECTTYPE * pGrenade = FindAttachmentByClass(FindAttachment_GrenadeLauncher(this), IC_GRENADE, subObject);
					if(pGrenade->exists()){
						if(ValidItemAttachmentSlot(this, pGrenade->usItem, 1, 0, 0, sSlot)){
							OBJECTTYPE tempObject;
							FindAttachment_GrenadeLauncher(this)->RemoveAttachment(pGrenade,&tempObject,0,0,1,0);
							(*this)[subObject]->AddAttachmentAtIndex((UINT8) sSlot, tempObject);
						}
					}	
				}*/
			}
		}

		//Check if any of the attached items has changed the slots (including the UGL grenade we just added).
		if(fRemoveProhibited){
			RemoveProhibitedAttachments(pSoldier, this, this->usItem);
		}
		// Check for attachment merge combos here
		//CHRISL: Only do this if we're looking at a single item.  Don't try a combo merge when dealing with a stack
		bAttachComboMerge = GetAttachmentComboMerge( this, subObject );
		if ( bAttachComboMerge != -1 )
		{
			if(this->ubNumberOfObjects == 1)
			{
				PerformAttachmentComboMerge( this, bAttachComboMerge );
				if ( bAttachInfoIndex != -1 && AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod < 20 )
				{
					StatChange( pSoldier, MECHANAMT, (INT8) ( 20 - AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod ), FALSE );
					StatChange( pSoldier, WISDOMAMT, (INT8) ( 20 - AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod ), FALSE );
				}
			}
			//CHRISL: If we're looking at a stack, we need to unattach the object we just attached
			else if(this->RemoveAttachment(&attachmentObject, pAttachmentPosition, subObject, pSoldier))
			{
				pAttachment->AddObjectsToStack(attachmentObject, -1, pSoldier, NUM_INV_SLOTS, MAX_OBJECTS_PER_SLOT);
			}
		}

		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = CalculateObjectWeight( this );
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//pAttachment->ubWeight = CalculateObjectWeight(pAttachment);

		if ( pSoldier != NULL )
			ApplyEquipmentBonuses(pSoldier);

		return( TRUE );
	}
	// check for merges
	else if (EvaluateValidMerge( pAttachment->usItem, this->usItem, &usResult, &usResult2, &ubType, &ubAPCost ))
	{
		//CHRISL: we don't want to try to merge IC_LBEGEAR items that are currently storing items.
		if(Item[this->usItem].usItemClass == IC_LBEGEAR)
		{
			if(this->IsActiveLBE(subObject) == true)
				return( FALSE );
		}
		//CHRISL: We don't want to do any merges if we're looking at a stack of items, with the exception of combines.
		if ( this->ubNumberOfObjects > 1 && ubType != COMBINE_POINTS )
		{
			return( FALSE );
		}
		if ( ubType != COMBINE_POINTS )
		{
			if ( pSoldier )
			{
				if ( !EnoughPoints( pSoldier, ubAPCost, 0, TRUE ) )
				{
					return( FALSE );
				}

				//lalien: don't charge AP's for reloading a GL/RL ( wrong place!!! the AP's are deducted in DeleteItemDescriptionBox() )
				//if ( IsAGLorRL == FALSE )
				//{
					DeductPoints( pSoldier, ubAPCost, 0 );
				//}
			}
		}

		switch( ubType )
		{
			case USE_ITEM:
			case USE_ITEM_HARD:
				// the merge will combine the two items
				if ( pSoldier )
				{
					if ( ubType == USE_ITEM_HARD )
					{
						// requires a skill check, and gives experience
						iCheckResult = SkillCheck( pSoldier, ATTACHING_SPECIAL_ITEM_CHECK, -30 );
						if (iCheckResult < 0)
						{
							// could have a chance of detonation
							// for now, damage both objects
							DamageObj( this, (INT8) -iCheckResult );
							DamageObj( pAttachment, (INT8) -iCheckResult );
							pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
							return( FALSE );
						}
						StatChange( pSoldier, MECHANAMT, 25, FALSE );
						StatChange( pSoldier, WISDOMAMT, 5, FALSE );
					}


					//Madd: note that use_item cannot produce two different items!!! so it doesn't use usResult2

					//Madd: unload guns after merge if ammo caliber or mag size don't match
					if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[0]->data.gun.usGunAmmoItem != NONE && (*this)[0]->data.gun.ubGunShotsLeft > 0 )
					{
						if ( Item[usResult].usItemClass != IC_GUN || Weapon[Item[usResult].ubClassIndex].ubCalibre != Weapon[Item[this->usItem].ubClassIndex].ubCalibre || (*this)[0]->data.gun.ubGunShotsLeft > Weapon[Item[usResult].ubClassIndex].ubMagSize )
						{ // item types/calibers/magazines don't match, spit out old ammo
							EjectAmmoAndPlace(pSoldier, this);
						}
					}

					UINT16 usOldItem = this->usItem;
					this->usItem = usResult;

					//WarmSteel - Replaced this with one that also checks default attachments, otherwise you could not replace built-in bonuses with default inseperable attachments.
					//RemoveProhibitedAttachments(pSoldier, this, usResult);
					ReInitMergedItem(pSoldier, this, usOldItem);

					//AutoPlaceObject( pAttachment );
					//ADB ubWeight has been removed, see comments in OBJECTTYPE
					//this->ubWeight = CalculateObjectWeight( this );
					if (pSoldier->bTeam == gbPlayerNum)
					{
						pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
					}
					ApplyEquipmentBonuses(pSoldier);
					return TRUE;
				}
				else
					return FALSE;
			case COMBINE_POINTS:
				// transfer points...
				UINT32 combinedAmount;
				UINT32 attachmentAmount;
				//CHRISL: Treat differently if we're combining money items (other then MONEY)
				if(Item[this->usItem].usItemClass != IC_MONEY)
				{
					if ( Item[ this->usItem ].usItemClass == IC_AMMO )
					{
						ubLimit = Magazine[ Item[ this->usItem ].ubClassIndex ].ubMagSize;
					}
					else
					{
						ubLimit = 100;
					}

					// count down through # of attaching items and add to status of item in position 0
					for (bLoop = 0; bLoop < pAttachment->ubNumberOfObjects; )
					{
						//ADB need to watch for overflow here (thus UINT32), and need to cast to UINT8 before adding
						combinedAmount = (UINT16)(*this)[subObject]->data.objectStatus + (UINT16)(*pAttachment)[0]->data.objectStatus;
						if (combinedAmount <= ubLimit)
						{
							// consume this one totally and continue
							(*this)[subObject]->data.objectStatus = combinedAmount;
							pAttachment->RemoveObjectsFromStack(1);
						}
						else
						{
							// add part of this one and then we're done
							attachmentAmount = (UINT16)(*pAttachment)[0]->data.objectStatus;
							attachmentAmount -= (ubLimit - (UINT16)(*this)[subObject]->data.objectStatus);
							(*pAttachment)[0]->data.objectStatus = attachmentAmount;
							if ((*pAttachment)[0]->data.ubShotsLeft == 0) {
								pAttachment->RemoveObjectsFromStack(1);
							}
							(*this)[subObject]->data.ubShotsLeft = ubLimit;
							break;
						}
					}
				}
				else
				{
					ubLimit = MAX_MONEY_PER_SLOT;
					combinedAmount = (*this)[subObject]->data.money.uiMoneyAmount + (*pAttachment)[0]->data.money.uiMoneyAmount;
					if (combinedAmount <= ubLimit)
					{
						(*this)[subObject]->data.money.uiMoneyAmount = combinedAmount;
						pAttachment->RemoveObjectsFromStack(1);
					}
					else
					{
						attachmentAmount = (*pAttachment)[0]->data.money.uiMoneyAmount;
						attachmentAmount -= (ubLimit - (*this)[subObject]->data.money.uiMoneyAmount);
						(*pAttachment)[0]->data.money.uiMoneyAmount = attachmentAmount;
						if((*pAttachment)[0]->data.money.uiMoneyAmount == 0)
						{
							pAttachment->RemoveObjectsFromStack(1);
						}
						(*this)[subObject]->data.money.uiMoneyAmount = ubLimit;
						break;
					}
				}
				break;
			case DESTRUCTION:
				// the merge destroyed both items!
				this->RemoveObjectsFromStack(1);
				pAttachment->RemoveObjectsFromStack(1);
				if ( pSoldier )
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
				}
				break;
			case ELECTRONIC_MERGE:
				if ( pSoldier )
				{
					iCheckResult = SkillCheck( pSoldier, ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK, -30 );
					if ( iCheckResult < 0 )
					{
						DamageObj( this, (INT8) -iCheckResult );
						DamageObj( pAttachment, (INT8) -iCheckResult );
						pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
						return( FALSE );
					}
					// grant experience!
				}
				// fall through
			case EXPLOSIVE:
				if ( ubType == EXPLOSIVE ) /// coulda fallen through
				{
					if (pSoldier)
					{
						// requires a skill check, and gives experience
						iCheckResult = SkillCheck( pSoldier, ATTACHING_DETONATOR_CHECK, -30 );
						if (iCheckResult < 0)
						{
							// could have a chance of detonation
							// for now, damage both objects
							DamageObj( this, (INT8) -iCheckResult );
							DamageObj( pAttachment, (INT8) -iCheckResult );
							pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
							return( FALSE );
						}
						StatChange( pSoldier, EXPLODEAMT, 25, FALSE );
						StatChange( pSoldier, WISDOMAMT, 5, FALSE );
					}
				}
				// fall through
			default:
				// the merge will combine the two items
				//Madd: usResult2 only works for standard merges->item1 + item2 = item3 + item4

				//Madd: unload guns after merge if ammo caliber or mag size don't match
				if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[subObject]->data.gun.usGunAmmoItem != NONE && (*this)[subObject]->data.gun.ubGunShotsLeft > 0 )
				{
					if ( Item[usResult].usItemClass != IC_GUN || Weapon[Item[usResult].ubClassIndex].ubCalibre != Weapon[Item[this->usItem].ubClassIndex].ubCalibre || (*this)[subObject]->data.gun.ubGunShotsLeft > Weapon[Item[usResult].ubClassIndex].ubMagSize )
					{ // item types/calibers/magazines don't match, spit out old ammo
						EjectAmmoAndPlace(pSoldier, this);
					}
				}

				UINT16 usOldItem = this->usItem;
				this->usItem = usResult;

				//WarmSteel - Replaced this with one that also checks default attachments, otherwise you could not replace built-in bonuses with default inseperable attachments.
				//RemoveProhibitedAttachments(pSoldier, this, usResult);
				ReInitMergedItem(pSoldier, this, usOldItem);

				if ( ubType != TREAT_ARMOUR )
				{
					(*this)[subObject]->data.objectStatus = ((*this)[subObject]->data.objectStatus + (*pAttachment)[0]->data.objectStatus) / 2;
				}

				//ADB ubWeight has been removed, see comments in OBJECTTYPE
				//this->ubWeight = CalculateObjectWeight( this );

				if ( usResult2 != NOTHING )
				{
					//Madd: usResult2 is what the original attachment/source item turns into

					//Madd: unload guns after merge if ammo caliber or mag size don't match
					if ( Item[pAttachment->usItem].usItemClass == IC_GUN && (*pAttachment)[0]->data.gun.usGunAmmoItem != NONE && (*pAttachment)[0]->data.gun.ubGunShotsLeft > 0 )
					{
						if ( Item[usResult2].usItemClass != IC_GUN || Weapon[Item[usResult2].ubClassIndex].ubCalibre != Weapon[Item[pAttachment->usItem].ubClassIndex].ubCalibre || (*pAttachment)[0]->data.gun.ubGunShotsLeft > Weapon[Item[usResult2].ubClassIndex].ubMagSize )
						{ // item types/calibers/magazines don't match, spit out old ammo
							EjectAmmoAndPlace(pSoldier, pAttachment);
						}
					}

					pAttachment->usItem = usResult2;

					RemoveProhibitedAttachments(pSoldier, pAttachment, usResult2);

					if ( ubType != TREAT_ARMOUR )
					{
						(*pAttachment)[0]->data.objectStatus = ((*pAttachment)[0]->data.objectStatus + (*this)[subObject]->data.objectStatus) / 2;
					}
					//ADB ubWeight has been removed, see comments in OBJECTTYPE
					//pAttachment->ubWeight = CalculateObjectWeight( pAttachment );
				}
				else
					pAttachment->RemoveObjectsFromStack(1);

				if (pSoldier && pSoldier->bTeam == gbPlayerNum)
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
				}
				break;
			}
			if ( pSoldier != NULL )
				ApplyEquipmentBonuses(pSoldier);
			return( TRUE );
	}
	return( FALSE );
}

//CHRISL: Use this function to sort through Attachments.xml and Launchables.xml
UINT32 SetAttachmentSlotsFlag(OBJECTTYPE* pObj){
	UINT32		uiSlotFlag = 0;
	UINT32		uiLoop = 0;
	UINT32		fItem;

	if(pObj->exists()==false)
		return 0;
	while(1)
	{
		fItem = 0;
		if (Attachment[uiLoop][1] == pObj->usItem){
			fItem = Attachment[uiLoop][0];
		}
		if (Launchable[uiLoop][1] == pObj->usItem ){
			fItem = Launchable[uiLoop][0];
		}
		if(fItem && ItemIsLegal(fItem, TRUE)){	// We've found a valid attachment.  Set the nasAttachmentSlots flag appropriately
			uiSlotFlag |= Item[fItem].nasAttachmentClass;
		}
		uiLoop++;
		if (Attachment[uiLoop][0] == 0 && Launchable[uiLoop][0] == 0){
			// No more attachments to search
			break;
		}
	}
	return uiSlotFlag;

}

//WarmSteel - This function returns the available item slot indexes in a vector.
std::vector<UINT16> GetItemSlots(OBJECTTYPE* pObj, UINT8 subObject, BOOLEAN fAttachment){
	std::vector<UINT16> tempItemSlots;
	std::vector<UINT16> tempAttachmentSlots;
	std::vector<UINT16>	tempSlots;
	UINT8				numSlots = 0;
	UINT16				magSize = 0;
	UINT32				fItemSlots = 0;
	UINT128				fItemLayout = 0;

	if(UsingNewAttachmentSystem()==false || !pObj->exists())
		return tempItemSlots;

	//CHRISL: We no longer need the ItemSlotAssign.xml file but we do still need to figure out which slots an item can have
	//Start by searching Attachments.xml and Launchables.xml for valid attachments for the primary object
	fItemSlots = SetAttachmentSlotsFlag(pObj);
	fItemLayout = Item[pObj->usItem].nasLayoutClass;
	if(Item[pObj->usItem].grenadelauncher || Item[pObj->usItem].rocketlauncher)
		magSize = GetMagSize(pObj);

	//Next, let's figure out which slots the item gives us access to
	if(fItemSlots){	//We don't need to do anything if the item gets no slots
		for(UINT8 sClass = 0; sClass < 32; sClass++){	//go through each attachment class and find the slots the item should have
			UINT32 uiClass = (UINT32)pow((double)2, (int)sClass);
			UINT32 slotSize = tempItemSlots.size();
			if(fItemSlots & uiClass){	//don't bother with this slot if it's not a valid class
				for(UINT32 sCount = 1; sCount < MAXITEMS+1; sCount++){
					if(AttachmentSlots[sCount].uiSlotIndex == 0)
						break;
					if(AttachmentSlots[sCount].nasAttachmentClass & uiClass && AttachmentSlots[sCount].nasLayoutClass & fItemLayout){	//found a slot
						if(magSize > 0 && AttachmentSlots[sCount].fMultiShot)
							magSize--;
						else if(AttachmentSlots[sCount].fMultiShot)
							continue;
						tempItemSlots.push_back(AttachmentSlots[sCount].uiSlotIndex);
					}
				}
				if(slotSize == tempItemSlots.size()){	//we didn't find a layout specific slot so try to find a default layout slot
					for(UINT32 sCount = 1; sCount < MAXITEMS+1; sCount++){
						if(AttachmentSlots[sCount].uiSlotIndex == 0)
							break;
						if(AttachmentSlots[sCount].nasAttachmentClass & uiClass && AttachmentSlots[sCount].nasLayoutClass == 1){	//found a default slot
								tempItemSlots.push_back(AttachmentSlots[sCount].uiSlotIndex);
						}
					}
				}
			}
		}
	}

	//Now that we've setup tempItemSlots for the main item, let's look at the individual attachments
	for(attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end(); ++iter){
		if(iter->exists() && (*iter)[0]->attachments.size() > 0){
			OBJECTTYPE* pAttachment = &(*iter);
			tempSlots = GetItemSlots(pAttachment,0,TRUE);
			for(UINT8 x = 0; x < tempSlots.size(); x++)
				tempAttachmentSlots.push_back(tempSlots[x]);
		}
	}

	//Now that we have tempAttachmentSlots, put it all together, assuming we're woking on the main item
	if(!fAttachment){
		if(tempAttachmentSlots.size() > 0){	//Add attachmentSlots to itemSlots
			for(UINT8 attachSlot = 0; attachSlot < tempAttachmentSlots.size(); attachSlot++){
				tempItemSlots.push_back(tempAttachmentSlots[attachSlot]);
			}
		}
		tempSlots = tempItemSlots;
		for(std::vector<UINT16>::iterator iter1 = tempSlots.begin(); iter1 != tempSlots.end(); ++iter1){
			BOOLEAN fSlotDuplicated = FALSE;
			for(std::vector<UINT16>::iterator iter = tempItemSlots.begin(); iter != tempItemSlots.end();){
				UINT16 i1 = *iter1;
				UINT16 i = *iter;
				if(i1 == i && !fSlotDuplicated){
					fSlotDuplicated = TRUE;
					++iter;
					continue;
				}
				else if(i1 == i && fSlotDuplicated){
					iter = tempItemSlots.erase(iter);
					continue;
				}
				else
					++iter;
			}
		}
		//If we still have no attachment slots, look through Merges.xml to see if we have merges and use the 4 default slots if we do
		//Should we throw on default slots regardless of whether there is an entry in Merges?  Ammo doesn't have Merges entries but we still need slots for them.
		if(tempItemSlots.size() == 0){
//			INT32 iLoop = 0;
//			while( 1 ){
//				if (Merge[iLoop][1] == pObj->usItem)
				{
					tempItemSlots.push_back(1);
					tempItemSlots.push_back(2);
					tempItemSlots.push_back(3);
					tempItemSlots.push_back(4);
//					break;
				}
//				iLoop++;
//				if (Merge[iLoop][1] == 0)
//				{
//					break;
//				}
//			}
		}
	}

	return tempItemSlots;
}

void InitItemAttachments(OBJECTTYPE* pObj){

	if(UsingNewAttachmentSystem()==false)
		return;

	//pObj->usAttachmentSlotIndexVector = GetItemSlots(pObj);
	(*pObj)[0]->attachments.resize((GetItemSlots(pObj).size()));
}
/*
//WarmSteel - Changed this function for NAS, because when the slots change many items will become invalid IN THAT SLOT, but not on the weapon.
To fix this we re-attach all invalid attachments. It also checks the slots that are on the gun.
IMPORTANT: If you use AttachObject/RemoveAttachment with fRemoveProhibited TRUE in this function, I will hunt you down and smite you.
*/
void RemoveProhibitedAttachments(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, UINT16 usItem, BOOLEAN fOnlyRemoveWhenSlotsChange)
{
	if(!pObj->exists())
		return;

	if(UsingNewAttachmentSystem()==true){
		UINT32				curSlot = 0;
		BOOLEAN				fCanAttach = FALSE;
		BOOLEAN				fRemoveProhibitedAttachments = FALSE;
		BOOLEAN				fDoneRemovingProhibited = FALSE;
		attachmentList		tempAttachList;
		std::vector<UINT16> tempItemSlots;
		std::vector<UINT16>	usAttachmentSlotIndexVector = GetItemSlots(pObj);

		//Get the item slots this item SHOULD have (but may not have right now), also counting any slots that were added or removed.
		//tempItemSlots = GetItemSlots(pObj);

		//Check if the slots have changed.
		if(fOnlyRemoveWhenSlotsChange){
			if(usAttachmentSlotIndexVector.size() != (*pObj)[0]->attachments.size()){
				fRemoveProhibitedAttachments = TRUE;
			} else {
				for(attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end() && !fRemoveProhibitedAttachments; ++iter, ++curSlot){
					if(iter->exists()){
						UINT16 slotIndex = usAttachmentSlotIndexVector[curSlot];
						UINT16 sItem = iter->usItem;
						if(!(AttachmentSlots[slotIndex].nasAttachmentClass & Item[sItem].nasAttachmentClass)){
							fRemoveProhibitedAttachments = TRUE;
						}
					}
				}
			}
		}

		if(fRemoveProhibitedAttachments){
			UINT16 usInfiniteLoopCount = 0;
			//CHRISL: There is the chance that we could have attachments in slots that are no longer valid, but if we arbitrarily resize the attachmentList
			//	we could actually lose attachments.  So let's just remove all attachments and replace them in correct locations
			for(attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					tempAttachList.push_back((*iter));
					iter = (*pObj)[0]->RemoveAttachmentAtIter(iter);
				}
			}
			//pObj->usAttachmentSlotIndexVector = tempItemSlots;
			//Keep checking till slots stop changing.
			while(!fDoneRemovingProhibited){
				//Surely 500 tries is enough to fix ANY item...
				AssertMsg(usInfiniteLoopCount < 500, "There was an inifinite loop while removing prohibited attachments");

				//Resize the object to the correct size
				(*pObj)[0]->attachments.resize(usAttachmentSlotIndexVector.size());

				//Start by trying to re-attach inseperable items.  They take precedence over items that can normally be removed
				for (attachmentList::iterator iter = tempAttachList.begin(); iter != tempAttachList.end();) {
					if(Item[iter->usItem].inseparable && ValidItemAttachmentSlot(pObj, iter->usItem, TRUE, FALSE, 0, -1, 0, NULL, usAttachmentSlotIndexVector)){
						//This seems to be rather valid. Can't be 100% sure though.
						if(pObj->AttachObject(NULL, &(*iter), FALSE, 0, -1, FALSE, usAttachmentSlotIndexVector)){
							//Ok now we can be sure, lets remove this object so we don't try to drop it later.
							iter = tempAttachList.erase(iter);
						} else {
							 ++iter;
						}
					} else {
						++iter;
					}
				}
				//Try to attach all the other attachments that didn't fit their current slot.
				for (attachmentList::iterator iter = tempAttachList.begin(); iter != tempAttachList.end();) {
					if(ValidItemAttachmentSlot(pObj, iter->usItem, TRUE, FALSE, 0, -1, 0, NULL, usAttachmentSlotIndexVector)){
						//This seems to be rather valid. Can't be 100% sure though.
						if(pObj->AttachObject(NULL, &(*iter), FALSE, 0, -1, FALSE, usAttachmentSlotIndexVector)){
							//Ok now we can be sure, lets remove this object so we don't try to drop it later.
							iter = tempAttachList.erase(iter);
						} else {
							 ++iter;
						}
					} else {
						++iter;
					}
				}

				tempItemSlots = GetItemSlots(pObj);

				//assume all attachments are replaced and we're good to break the look
				fDoneRemovingProhibited = TRUE;

				//Check if the slots have changed after possibly removing attachments.
				if(tempItemSlots.size() != usAttachmentSlotIndexVector.size()){
					//Changed, we need to correct again.
					fDoneRemovingProhibited = FALSE;
				} else if(!tempItemSlots.empty()){
					for (UINT16 cnt = 0; cnt < tempItemSlots.size(); ++cnt) {
						//If these slots don't match, something has changed, keep checking.
						if(tempItemSlots[cnt] != usAttachmentSlotIndexVector[cnt]){
							fDoneRemovingProhibited = FALSE;
							break;
						}
					}
				}

				if(usInfiniteLoopCount > 10){	//run through the loop 10 times before we drop items.
					//Anything that's still in our tempAttachList couldn't be attached, and should be dropped.
					for (attachmentList::iterator iter = tempAttachList.begin(); iter != tempAttachList.end();) {
						if (iter->exists()) {
							if ( pSoldier && AutoPlaceObject( pSoldier, &(*iter), FALSE ) )
							{
								iter = tempAttachList.erase(iter);
							} else {	// put it on the ground
								INT8 pathing = (pSoldier?pSoldier->pathing.bLevel:0);
								INT32 sGridNo = (pSoldier?pSoldier->sGridNo:0);
								AutoPlaceObjectToWorld(pSoldier, &(*iter), TRUE);
								iter = tempAttachList.erase(iter);
								/*if(guiCurrentItemDescriptionScreen == MAP_SCREEN && fShowMapInventoryPool){
									AutoPlaceObjectInInventoryStash(&(*iter), sGridNo);
									//AddItemToPool( sGridNo, &(*iter), 1, pathing, WORLD_ITEM_REACHABLE, 0 );
									iter = tempAttachList.erase(iter);
								} else {
									AddItemToPool( sGridNo, &(*iter), 1, pathing, WORLD_ITEM_REACHABLE, 0 );
									iter = tempAttachList.erase(iter);
								}*/
							}
						} else {
							++iter;
						}
					}
				}
				if(!tempAttachList.empty()){
					fDoneRemovingProhibited = FALSE;
				}

				usAttachmentSlotIndexVector = GetItemSlots(pObj);
				usInfiniteLoopCount++;
			}
		}
	} else {
		UINT16 x = 0;
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end();) {
			if(!iter->exists()){
				//WarmSteel - erase null objects, we don't need them without the NAS.
				iter = (*pObj)[0]->attachments.erase(iter);
			} else {
				if ( (!ValidAttachment(iter->usItem, pObj) && !ValidLaunchable(iter->usItem, usItem) && !ValidLaunchable(iter->usItem, GetAttachedGrenadeLauncher(pObj)) ) || x >= OLD_MAX_ATTACHMENTS_101)
				{
					if ( !Item[iter->usItem].inseparable )
					{
						if (pSoldier) {
							if ( !AutoPlaceObject( pSoldier, &(*iter), FALSE ) )
							{   // put it on the ground
								AddItemToPool( pSoldier->sGridNo, &(*iter), 1, pSoldier->pathing.bLevel, 0 , -1 );
							}
						}
					}
					pObj->RemoveAttachment(&(*iter),0 ,0 , pSoldier, TRUE, FALSE);
					iter = (*pObj)[0]->attachments.begin();
					x = 0;
				} else {
					++x;
					++iter; 
				}
			}
		}
	}
	return;
}

//WarmSteel - Needed for merges, because the new items don't get their new default attachments.
//This does the same as RemoveProhibitedAttachments but is a bit more thorough at it.
//This is at the risk that items move around if more than one slot is valid, but since this is used for "new" guns after merges, that's acceptable.
void ReInitMergedItem(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, UINT16 usOldItem){

	attachmentList		tempAttachList;
	attachmentList		tempSlotChangingAttachList;
	BOOLEAN fFoundDefaultAttachment = FALSE;
	UINT8 slotCount;
	std::vector<UINT16>	usAttachmentSlotIndexVector = GetItemSlots(pObj);

	if( !(*pObj)[0]->attachments.empty() ){
		//Have to take all attachments off, because of possible incompatibilities with the default attachments (can't NOT attach a default attachment because some stupid item already attached is incompatible with it).

		//Delete all default inseperable attachments from the old gun.
		//This is safe UNLESS the inseparable default attachments is not the only attachment of the same kind on the gun.
		//I can't think of any reason why this would happen, and if it does the worst that can happen is your attachment disappearing.
		for(slotCount = 0; slotCount < (*pObj)[0]->attachments.size(); slotCount++){
			UINT16 usAttach = (*pObj)[0]->GetAttachmentAtIndex(slotCount)->usItem;
			if(Item[usAttach].inseparable){
				for(UINT16 cnt = 0; cnt < MAX_DEFAULT_ATTACHMENTS && Item[usOldItem].defaultattachments[cnt] != 0; cnt++){
					if(Item[usOldItem].defaultattachments[cnt] == usAttach){
						(*pObj)[0]->RemoveAttachmentAtIndex(slotCount);
						break;
					}
				}
			}
		}
		slotCount = 0;
		//Put all other attachments into a temporary storage, so we can try to re-attach later.
		for(attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); iter++, slotCount++) {
			if(iter->exists()){
				UINT16 cnt = 0;
				tempAttachList.push_back((*iter));
			}
		}
	}
	//clear the attachment list, we've saved the attachments somewhere safe now.
	(*pObj)[0]->attachments.clear();

	//Make sure the attachment slot data is correct.
	//std::vector<UINT16> tempItemSlots = GetItemSlots(pObj);
	(*pObj)[0]->attachments.resize(usAttachmentSlotIndexVector.size());
	//pObj->usAttachmentSlotIndexVector = tempItemSlots;

	//Now add all default attachments, but add them with the same status as the gun. We don't want to make repairing guns easy :)
	for(UINT16 cnt = 0; cnt < MAX_DEFAULT_ATTACHMENTS && Item[pObj->usItem].defaultattachments[cnt] != 0; cnt++){
		//Only add inseparable default attachments, because they are likely "part" of the gun.
		if(Item[Item[pObj->usItem].defaultattachments[cnt]].inseparable){
			static OBJECTTYPE defaultAttachment;
			CreateItem(Item [ pObj->usItem ].defaultattachments[cnt],(*pObj)[0]->data.objectStatus,&defaultAttachment);
			AssertMsg(pObj->AttachObject(NULL,&defaultAttachment, FALSE, 0, -1, FALSE), "A default attachment could not be attached after merging, this should not be possible.");
		}
	}

	//First re-attach any slot-changing attachments.
	for (attachmentList::iterator iter = tempSlotChangingAttachList.begin(); iter != tempSlotChangingAttachList.end();) {
		if( ValidItemAttachmentSlot(pObj, iter->usItem, TRUE, FALSE )){
			//This seems to be rather valid. Can't be 100% sure though.
			if(pObj->AttachObject(NULL, &(*iter), FALSE)){
				//Ok now we can be sure, lets remove this object so we don't try to drop it later.
				iter = tempSlotChangingAttachList.erase(iter);
			} else {
				++iter;
			}
		} else {
			++iter;
		}
	}

	//Time to re-attach the other attachments, if we can. I am the king of copy pasta.
	for (attachmentList::iterator iter = tempAttachList.begin(); iter != tempAttachList.end();) {
		if( ValidItemAttachmentSlot(pObj, iter->usItem, TRUE, FALSE)){
			//This seems to be rather valid. Can't be 100% sure though.
			if(pObj->AttachObject(NULL, &(*iter), FALSE)){
				//Ok now we can be sure, lets remove this object so we don't try to drop it later.
				iter = tempAttachList.erase(iter);
			} else {
				++iter;
			}
		} else {
			++iter;
		}
	}

	//drop all items we couldn't re-attach.
	for (attachmentList::iterator iter = tempSlotChangingAttachList.begin(); iter != tempSlotChangingAttachList.end(); ++iter) {
		if ( !Item[iter->usItem].inseparable )
		{//WarmSteel - Couldn't re-attach this item, try to drop it.
			if (pSoldier) {
				if ( !AutoPlaceObject( pSoldier, &(*iter), FALSE ) )
				{   // put it on the ground
					AddItemToPool( pSoldier->sGridNo, &(*iter), 1, pSoldier->pathing.bLevel, 0 , -1 );
				}
			}
		}
	}
	//and the rest too
	for (attachmentList::iterator iter = tempAttachList.begin(); iter != tempAttachList.end(); ++iter) {
		if ( !Item[iter->usItem].inseparable )
		{//WarmSteel - Couldn't re-attach this item, try to drop it.
			if (pSoldier) {
				if ( !AutoPlaceObject( pSoldier, &(*iter), FALSE ) )
				{   // put it on the ground
					AddItemToPool( pSoldier->sGridNo, &(*iter), 1, pSoldier->pathing.bLevel, 0 , -1 );
				}
			}
		}
	}
}

void EjectAmmoAndPlace(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, UINT8 subObject)
{
	CreateAmmo((*pObj)[subObject]->data.gun.usGunAmmoItem, &gTempObject, (*pObj)[subObject]->data.gun.ubGunShotsLeft);
	(*pObj)[subObject]->data.gun.ubGunShotsLeft = 0;
	(*pObj)[subObject]->data.gun.usGunAmmoItem = NONE;
	// HEADROCK HAM 3.5: Clear ammo type
	(*pObj)[subObject]->data.gun.ubGunAmmoType = NONE;
	if ( pSoldier )
	{
		AutoPlaceObjectAnywhere( pSoldier, &gTempObject, FALSE );
//		if ( !AutoPlaceObject( pSoldier, &gTempObject, FALSE ) )
//		{   // put it on the ground
//			AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );
//		}
	}
	return;
}

/* CHRISL: This function is edited to handle the new inventory system when we have an item in our cursor.
Not only do we have to hatch out pockets that the item won't fit in, we also have to hatch out pockets that
our current LBE gear haven't activated.  We'll also need to display the number of items of the type currently
held in the cursor that each active pocket can hold.*/
extern BOOLEAN CompatibleAmmoForGun( OBJECTTYPE *pTryObject, OBJECTTYPE *pTestObject );
BOOLEAN CanItemFitInVehicle( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement )
{
	if((UsingNewInventorySystem() == false) || !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
		return(FALSE);
	if(!vehicleInv[bPos])
		return(FALSE);

	UINT8					ubSlotLimit;
	ubSlotLimit = ItemSlotLimit( pObj, bPos, pSoldier );

	if ( ubSlotLimit == 0 )
		return ( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) );

	return( TRUE );
}


BOOLEAN CanItemFitInPosition( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement )
{
	UINT8					ubSlotLimit, lbePocket=1;
	INT8					bNewPos=ITEM_NOT_FOUND;
	UINT32					pRestrict=0;

	// CHRISL: Only check valid pockets
	if((UsingNewInventorySystem() == false) && !oldInv[bPos])
		return(FALSE);
	if((pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) && UsingNewInventorySystem() == true)
		return(CanItemFitInVehicle(pSoldier, pObj, bPos, fDoingPlacement));

	ubSlotLimit = ItemSlotLimit( pObj, bPos, pSoldier );

	switch( bPos )
	{
		case SECONDHANDPOS:
			if (Item[pSoldier->inv[HANDPOS].usItem].twohanded )
			{
				return( FALSE );
			}
			break;
		case HANDPOS:
			if (Item[ pObj->usItem ].twohanded )
			{
				if (pSoldier->inv[HANDPOS].exists() == true && pSoldier->inv[SECONDHANDPOS].exists() == true)
				{
					// two items in hands; try moving the second one so we can swap
					if (FitsInSmallPocket(&pSoldier->inv[SECONDHANDPOS]) == true)
					{
						bNewPos = FindEmptySlotWithin( pSoldier, BIGPOCKSTART, NUM_INV_SLOTS );
					}
					else
					{
						bNewPos = FindEmptySlotWithin( pSoldier, BIGPOCKSTART, MEDPOCKFINAL );
					}

					if (bNewPos == NO_SLOT)
					{
						// nowhere to put second item
						return( FALSE );
					}

					if ( fDoingPlacement )
					{
						// otherwise move it, forget about bNewPos!
						PlaceInAnyPocket(pSoldier, &pSoldier->inv[SECONDHANDPOS], FALSE);
					}
				}
			}
			break;
		case VESTPOS:
		case HELMETPOS:
		case LEGPOS:
			if (Item[pObj->usItem].usItemClass != IC_ARMOUR)
			{
				return( FALSE );
			}
			switch (bPos)
			{
				case VESTPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_VEST)
					{
						return( FALSE );
					}
					break;
				case HELMETPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_HELMET)
					{
						return( FALSE );
					}
					break;
				case LEGPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_LEGGINGS)
					{
						return( FALSE );
					}
					break;
				default:
					break;
			}
			break;
		case HEAD1POS:
		case HEAD2POS:
			if (Item[pObj->usItem].usItemClass != IC_FACE)
			{
				return( FALSE );
			}
			break;
		case VESTPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != VEST_PACK)
			{
				return( FALSE );
			}
			break;
		case LTHIGHPOCKPOS:
		case RTHIGHPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != THIGH_PACK)
			{
				return( FALSE );
			}
			break;
		case CPACKPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != COMBAT_PACK)
			{
				return( FALSE );
			}
			if(pSoldier->inv[BPACKPOCKPOS].exists() == true)
			{
				//DBrot: changed to bitwise comparison
				if(((LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo & LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeCombo) == 0) ||
				LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo == 0)
				{
					return( FALSE );
				}
			}
			break;
		case BPACKPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != BACKPACK)
			{
				return( FALSE );
			}
			// Removed backpack/gunsling restrictions
			//if(pSoldier->inv[GUNSLINGPOCKPOS].exists() == true)
			//	return( FALSE );
			if(pSoldier->inv[CPACKPOCKPOS].exists() == true)
			{
				//DBrot: changed to bitwise comparison
				if(((LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo & LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeCombo) ==0)||
				LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo == 0)
				{
					return( FALSE );
				}
			}
			break;
		case GUNSLINGPOCKPOS:	// Gun Sling
			//if (Item[pObj->usItem].usItemClass != IC_GUN && Item[pObj->usItem].usItemClass != IC_BLADE && Item[pObj->usItem].usItemClass != IC_LAUNCHER)
			if(pObj->usItem == MONEY)
				return( FALSE );
			if(Item[pObj->usItem].usItemClass == IC_AMMO || Item[pObj->usItem].usItemClass == IC_GRENADE)
				return(CompatibleAmmoForGun(pObj, &pSoldier->inv[GUNSLINGPOCKPOS]) || ValidAttachment(pObj->usItem, &(pSoldier->inv[GUNSLINGPOCKPOS]) ) || ValidLaunchable(pObj->usItem, pSoldier->inv[GUNSLINGPOCKPOS].usItem));
			//recalc slot limit to exclude ItemSize attachment modifiers
			ubSlotLimit = ItemSlotLimit( pObj, bPos, pSoldier, FALSE );
			// Removed backpack/gunsling restrictions
			//if(pSoldier->inv[BPACKPOCKPOS].exists() == true)
			//	return(CompatibleAmmoForGun(pObj, &pSoldier->inv[GUNSLINGPOCKPOS]));
			break;
		case KNIFEPOCKPOS:	// Knife sheath
			if(pObj->usItem == MONEY)
				return( FALSE );
			if (Item[pObj->usItem].usItemClass != IC_BLADE && Item[pObj->usItem].usItemClass != IC_THROWING_KNIFE )
				return(CompatibleAmmoForGun(pObj, &pSoldier->inv[KNIFEPOCKPOS]) || ValidAttachment(pObj->usItem, &(pSoldier->inv[KNIFEPOCKPOS]) ) || ValidLaunchable(pObj->usItem, pSoldier->inv[KNIFEPOCKPOS].usItem));
			break;
		// IC Pockets
		case BIGPOCK1POS:
		case BIGPOCK2POS:
		case BIGPOCK3POS:
		case BIGPOCK4POS:
		case BIGPOCK5POS:
		case BIGPOCK6POS:
		case BIGPOCK7POS:
		case MEDPOCK1POS:
		case MEDPOCK2POS:
		case MEDPOCK3POS:
		case MEDPOCK4POS:
		case SMALLPOCK1POS:
		case SMALLPOCK2POS:
		case SMALLPOCK3POS:
		case SMALLPOCK4POS:
		case SMALLPOCK5POS:
		case SMALLPOCK6POS:
		case SMALLPOCK7POS:
		case SMALLPOCK8POS:
		case SMALLPOCK9POS:
		case SMALLPOCK10POS:
		case SMALLPOCK11POS:
		case SMALLPOCK12POS:
		case SMALLPOCK13POS:
		case SMALLPOCK14POS:
		case SMALLPOCK15POS:
		case SMALLPOCK16POS:
		case SMALLPOCK17POS:
		case SMALLPOCK18POS:
		case SMALLPOCK19POS:
		case SMALLPOCK20POS:
		case SMALLPOCK21POS:
		case SMALLPOCK22POS:
		case SMALLPOCK23POS:
		case SMALLPOCK24POS:
		case SMALLPOCK25POS:
		case SMALLPOCK26POS:
		case SMALLPOCK27POS:
		case SMALLPOCK28POS:
		case SMALLPOCK29POS:
		case SMALLPOCK30POS:
			if((UsingNewInventorySystem() == true))
			{
				if(icLBE[bPos] == BPACKPOCKPOS && (!(pSoldier->flags.ZipperFlag) || (pSoldier->flags.ZipperFlag && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND)) && (gTacticalStatus.uiFlags & INCOMBAT))
					return( FALSE );
				lbePocket = (pSoldier->inv[icLBE[bPos]].exists() == false) ? LoadBearingEquipment[Item[icDefault[bPos]].ubClassIndex].lbePocketIndex[icPocket[bPos]] : LoadBearingEquipment[Item[pSoldier->inv[icLBE[bPos]].usItem].ubClassIndex].lbePocketIndex[icPocket[bPos]];
				pRestrict = LBEPocketType[lbePocket].pRestriction;
				if(pRestrict != 0)
					if(!(pRestrict & Item[pObj->usItem].usItemClass))
						lbePocket = 0;
			}
			break;
		default:
			break;
	}

	if((UsingNewInventorySystem() == false))
	{
		if (ubSlotLimit == 0 && bPos >= SMALLPOCKSTART )
		{
			// doesn't fit!
			return( FALSE );
		}
	}
	else
	{
		// CHRISL: lbePocket==0 means pocket disabled.  ubSlotLimit==0 means pocket can't hold item
		if ( lbePocket == 0 || ubSlotLimit == 0 )
			return ( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) || ValidAttachment(pObj->usItem, &(pSoldier->inv[bPos]) ) || ValidLaunchable(pObj->usItem, pSoldier->inv[bPos].usItem) );

		// CHRISL: Adjust parameters to include the new inventory system
		if (ubSlotLimit == 0 && bPos >= BIGPOCKFINAL )
			return( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) || ValidAttachment(pObj->usItem, &(pSoldier->inv[bPos]) ) || ValidLaunchable(pObj->usItem, pSoldier->inv[bPos].usItem) );
	}

	return( TRUE );
}

//CHRISL: Wrote this function to try and clean up possible problems relating to the 16bit change for ammo capacity
void CleanUpItemStats( OBJECTTYPE * pObj )
{
	UINT16		magSize;

	if(Item[pObj->usItem].usItemClass == IC_GUN)
	{
		magSize = GetMagSize(pObj);
		if((*pObj)[0]->data.gun.ubGunShotsLeft > magSize)
		{
			(*pObj)[0]->data.gun.ubGunShotsLeft = magSize;
		}
		return;
	}
	if(Item[pObj->usItem].usItemClass == IC_AMMO)
	{
		magSize = Magazine[Item[pObj->usItem].ubClassIndex].ubMagSize;
		if((*pObj)[0]->data.ubShotsLeft > magSize)
		{
			(*pObj)[0]->data.ubShotsLeft = magSize;
		}
		return;
	}
}

BOOLEAN FreeUpSlotIfPossibleThenPlaceObject( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj )
{
	//this gets called if something doesn't fit in bPos, which can happen if something is there
	//or if it simply doesn't fit, if it doesn't fit return false to prevent recursion

	// try autoplacing item in bSlot elsewhere, excluding the slot it came from, then do a placement
	if ( pSoldier->inv[bPos].exists() == true && AutoPlaceObject( pSoldier, &(pSoldier->inv[bPos]), FALSE , bPos) )
	{
		//the old object has been placed somewhere, it's safe to place this one
		return( PlaceObject( pSoldier, bPos, pObj ) );
	}
	return( FALSE );
}

// CHRISL: Use to find best pocket to store item in.  Could probably be merged with FitsInSmallPocket
INT32 PickPocket(SOLDIERTYPE *pSoldier, UINT8 ppStart, UINT8 ppStop, UINT16 usItem, UINT8 iNumber, UINT8 * cap, int bExcludeSlot)
{
	UINT16	pIndex=0;
	INT32	pocket=0;
	UINT8	capacity=254;

	for(UINT32 uiPos=ppStart; uiPos<ppStop; uiPos++){
		if(pSoldier->inv[icLBE[uiPos]].exists() == false){
			pIndex=LoadBearingEquipment[Item[icDefault[uiPos]].ubClassIndex].lbePocketIndex[icPocket[uiPos]];
		}
		else {
			pIndex=LoadBearingEquipment[Item[pSoldier->inv[icLBE[uiPos]].usItem].ubClassIndex].lbePocketIndex[icPocket[uiPos]];
		}
		// Here's were we get complicated.  We should look for the smallest pocket all items can fit in
		if(LBEPocketType[pIndex].ItemCapacityPerSize[Item[usItem].ItemSize] >= iNumber &&
			LBEPocketType[pIndex].ItemCapacityPerSize[Item[usItem].ItemSize] < capacity &&
			pSoldier->inv[uiPos].exists() == false && uiPos != bExcludeSlot) {
				if((LBEPocketType[pIndex].pRestriction != 0 && (LBEPocketType[pIndex].pRestriction & Item[usItem].usItemClass)) ||
					LBEPocketType[pIndex].pRestriction == 0) {
						capacity = LBEPocketType[pIndex].ItemCapacityPerSize[Item[usItem].ItemSize];
						pocket = uiPos;
				}
		}
	}
	if(pocket!=0){
		*cap=capacity;
		return pocket;
	}
	else{
		*cap=254;
		return -1;
	}
}

BOOLEAN PlaceObject( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj, BOOLEAN fNewItem )
{
	if (PlaceObject(pSoldier, bPos, pObj) == TRUE) {
		SetNewItem(pSoldier, bPos, fNewItem);
		return TRUE;
	}
	return FALSE;
}

BOOLEAN PlaceObject( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj )
{
	// returns object to have in hand after placement... same as original in the
	// case of error

	UINT8					ubSlotLimit;
	OBJECTTYPE *	pInSlot;
	BOOLEAN				fObjectWasRobotRemote = FALSE;

	if ( Item[pObj->usItem].robotremotecontrol )
	{
		fObjectWasRobotRemote = TRUE;
	}

	//CHRISL: Failsafe to try and clean up ammo capacity problems
	CleanUpItemStats(pObj);

	if ( !CanItemFitInPosition( pSoldier, pObj, bPos, TRUE ) )
	{
		return( FALSE );
	}

	// If the position is either head slot, then the item must be IC_FACE (checked in
	// CanItemFitInPosition).
	if ( bPos == HEAD1POS )
	{
		if ( !CompatibleFaceItem( pObj->usItem, pSoldier->inv[ HEAD2POS ].usItem ) )
		{
			CHAR16	zTemp[ 150 ];

			swprintf( zTemp, Message[ STR_CANT_USE_TWO_ITEMS ], ItemNames[ pObj->usItem ], ItemNames[ pSoldier->inv[ HEAD2POS ].usItem ] );
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
			return( FALSE );
		}
	}
	else if ( bPos == HEAD2POS )
	{
		if ( !CompatibleFaceItem( pObj->usItem, pSoldier->inv[ HEAD1POS ].usItem ) )
		{
			CHAR16	zTemp[ 150 ];

			swprintf( zTemp, Message[ STR_CANT_USE_TWO_ITEMS ], ItemNames[ pObj->usItem ], ItemNames[ pSoldier->inv[ HEAD1POS ].usItem ] );
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
			return( FALSE );
		}
	}

	if ( Item[ pObj->usItem ].usItemClass == IC_KEY && pSoldier->flags.uiStatusFlags & SOLDIER_PC )
	{
		if ( KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound == 0 )
		{
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound = (UINT16) GetWorldDay();
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usSectorFound = SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );
		}
	}

    // Lesh: bugfix - replacing weapon in auto with another weapon w/o auto-mode
    if (bPos == HANDPOS && Item[ pObj->usItem ].usItemClass == IC_GUN)
    {
		//Madd: added code for nosemiauto tag
		if (!Weapon[ Item[pObj->usItem].ubClassIndex ].NoSemiAuto)
		{
			pSoldier->bWeaponMode = WM_NORMAL;
			pSoldier->bDoBurst = FALSE;
			pSoldier->bDoAutofire = FALSE;
		}
		else
		{
			pSoldier->bWeaponMode = WM_AUTOFIRE;
			pSoldier->bDoBurst = TRUE;
			pSoldier->bDoAutofire = TRUE;
		}
    }
    // Lesh: end

#ifdef JA2UB
	//handle the placing up of a new ja25 gun
	HandleNewGunComment( pSoldier, pObj->usItem, FALSE );
#endif	
	pInSlot = &(pSoldier->inv[bPos]);

	//we are placing an object, how we handle this depends on what is in the slot already
	if (pInSlot->exists() == false)
	{
		//if the object in the slot does not exist it is easy
		pObj->MoveThisObjectTo(*pInSlot, ALL_OBJECTS, pSoldier, bPos);
		if (pObj->exists() == false)
		{
			// dropped everything
			if (bPos == HANDPOS && Item[pInSlot->usItem].twohanded )
			{
				// We just performed a successful drop of a two-handed object into the
				// main hand
				if (pSoldier->inv[SECONDHANDPOS].exists() == true)
				{
					// swap what WAS in the second hand into the cursor
					pSoldier->inv[SECONDHANDPOS].MoveThisObjectTo(*pObj);
				}
			}
		 }
	}
	else
	{
		// replacement/reloading/merging/stacking
		//try to reload first
		switch (Item[pInSlot->usItem].usItemClass)
		{
			case IC_GUN:
				if (Item[pObj->usItem].usItemClass == IC_AMMO)
				{
					if (Weapon[pInSlot->usItem].ubCalibre == Magazine[Item[pObj->usItem].ubClassIndex].ubCalibre)
					{
						//CHRISL: Work differently with ammo crates but only when not in combat
						if(Item[pObj->usItem].ammocrate == TRUE)
						{
							if(!(gTacticalStatus.uiFlags & INCOMBAT))
							{
								UINT16		magSize, ubShotsLeft;
								OBJECTTYPE	tempClip;
								OBJECTTYPE	tempStack;
								bool		clipCreated;
								UINT32		newItem = 0;
								INT32		pocket=-1;
								UINT8		capacity=0;
								UINT8		bLoop;
								//find the ammo item we want to try and create
								for(int loop = 0; loop < MAXITEMS; loop++)
								{
									if(Item[loop].usItemClass == IC_AMMO)
									{
										if(Magazine[Item[loop].ubClassIndex].ubCalibre == Weapon[pInSlot->usItem].ubCalibre && Magazine[Item[loop].ubClassIndex].ubAmmoType == Magazine[Item[pObj->usItem].ubClassIndex].ubAmmoType && Magazine[Item[loop].ubClassIndex].ubMagSize == GetMagSize(pInSlot))
											newItem = loop;
									}
								}
								//Create a stack of up to 5 "newItem" clips 
								tempStack.initialize();
								clipCreated = false;
								ubShotsLeft = (*pObj)[0]->data.ubShotsLeft;
								for(UINT8 clip = 0; clip < 5; clip++)
								{
									magSize = GetMagSize(pInSlot);
									if(ubShotsLeft < magSize)
										magSize = ubShotsLeft;
									if(CreateAmmo(newItem, &tempClip, magSize))
									{
										tempStack.AddObjectsToStack(tempClip, -1, pSoldier, NUM_INV_SLOTS, MAX_OBJECTS_PER_SLOT);
										ubShotsLeft -= magSize;
										clipCreated = true;
										if(ubShotsLeft < 1)
											break;
									}
								}
								//Try to place the stack somewhere on the active merc
								if(clipCreated == true)
								{
									clipCreated = false;
									bLoop = tempStack.ubNumberOfObjects;
									while(tempStack.ubNumberOfObjects > 0)
									{
										pocket = PickPocket(pSoldier, BIGPOCKSTART, SMALLPOCKFINAL, tempStack.usItem, bLoop, &capacity, -1);
										if(pocket != -1)
										{
											pSoldier->inv[pocket].AddObjectsToStack(tempStack, bLoop, pSoldier, pocket);
										}
										else
										{
											bLoop--;
										}
										if(bLoop < 1)
											break;
									}
									if(tempStack.ubNumberOfObjects < 1)
										clipCreated = true;
									else
									{
										//Try to place stack on ground
										if( AutoPlaceObjectToWorld(pSoldier, &tempStack) )
										{
											clipCreated = true;
											if(guiCurrentScreen == GAME_SCREEN)
												NotifySoldiersToLookforItems( );
										}
										/*if(guiCurrentScreen == MAP_SCREEN && fShowMapInventoryPool == TRUE)
										{
											if(AutoPlaceObjectInInventoryStash(&tempStack, pSoldier->sGridNo))
												clipCreated = true;
										}
										else
										{
											if(AddItemToPool(pSoldier->sGridNo, &tempStack, 1, pSoldier->pathing.bLevel, WORLD_ITEM_REACHABLE, -1))
											{
												NotifySoldiersToLookforItems( );
												clipCreated = true;
											}
										}*/
									}
								}
								if(clipCreated == true)
								{
									(*pObj)[0]->data.ubShotsLeft = ubShotsLeft;
								}
								if((*pObj)[0]->data.ubShotsLeft < 1)
									pObj->RemoveObjectsFromStack(1);
								return( TRUE );
							}
							else
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMapInventoryErrorString[6] );
								return( FALSE );
							}
						}
						else
							return( ReloadGun( pSoldier, pInSlot, pObj ) );
					}
				}
				break;
			case IC_LAUNCHER:
				if ( ValidLaunchable( pObj->usItem, pInSlot->usItem ) ) {
					return( ReloadGun( pSoldier, pInSlot, pObj ) );
				}
			break;
			}
		//if we didn't reload, then we know we are stacking or swapping!

		if (IsSlotASmallPocket(bPos) == true && FitsInSmallPocket(pObj) == false) {
			//there is nothing we can do, just return
			return FALSE;
		}

		// CHRISL: When holding ammo and clicking on an appropriate ammo crate, add ammo to crate
		if(Item[pInSlot->usItem].ammocrate == TRUE && Item[pObj->usItem].usItemClass == IC_AMMO)
		{
			if(Magazine[Item[pInSlot->usItem].ubClassIndex].ubCalibre == Magazine[Item[pObj->usItem].ubClassIndex].ubCalibre &&
				Magazine[Item[pInSlot->usItem].ubClassIndex].ubAmmoType == Magazine[Item[pObj->usItem].ubClassIndex].ubAmmoType)
			{
				UINT16	magSpace = Magazine[Item[pInSlot->usItem].ubClassIndex].ubMagSize-(*pInSlot)[0]->data.ubShotsLeft;
				while(pObj->ubNumberOfObjects > 0)
				{
					if(magSpace >= (*pObj)[0]->data.ubShotsLeft)
					{
						magSpace -= (*pObj)[0]->data.ubShotsLeft;
						(*pInSlot)[0]->data.ubShotsLeft += (*pObj)[0]->data.ubShotsLeft;
						pObj->RemoveObjectsFromStack(1);
					}
					else
					{
						(*pObj)[0]->data.ubShotsLeft -= magSpace;
						(*pInSlot)[0]->data.ubShotsLeft += magSpace;
						break;
					}
				}
				if(pObj->ubNumberOfObjects > 0)
					return( FALSE );
				else
					return( TRUE );
			}
		}

		// CHRISL:
		ubSlotLimit = ItemSlotLimit( pObj, bPos, pSoldier );
		if (ubSlotLimit == 0)
		{
			//we have tried to stack but the stack is full, or we have tried to swap but the slot is wrong
			return( FreeUpSlotIfPossibleThenPlaceObject( pSoldier, bPos, pObj ) );
			}


		if ( pObj->usItem == pInSlot->usItem && ubSlotLimit > 1 && IsSlotAnLBESlot(bPos) == false )
			{
			//we have tried to stack, but remember we can't stack 2 LBEs into 1 LBE slot, they get swapped instead
			pInSlot->AddObjectsToStack( *pObj, ALL_OBJECTS, pSoldier, bPos );
		}

		else if ( (Item[pObj->usItem].twohanded ) && (bPos == HANDPOS) )
		{
			if (pSoldier->inv[SECONDHANDPOS].exists() == true) {
				// both pockets have something in them, so we can't swap
				return( FALSE );
			}
			else {
				//we swapped a 2 handed object into the main hand
				SwapObjs( pObj, pInSlot );
			}
		}

		else if (IsSlotAnLBESlot(bPos) == true && Item[pObj->usItem].usItemClass == IC_LBEGEAR)
		{
			/*CHRISL: If we're trying to swap LBE items between IC pockets, we have to be careful that items are moved
			into active pockets or that an LBENODE is properly created.*/
			if(pObj->HasAnyActiveLBEs(pSoldier) == false && !(_KeyDown(SHIFT))){
				std::vector<INT8> LBESlots;
				GetLBESlots(bPos, LBESlots);
				MoveItemsToActivePockets(pSoldier, LBESlots, bPos, pObj);
			}
			pInSlot->MoveThisObjectTo(gTempObject, -1, pSoldier, bPos);
			pObj->MoveThisObjectTo(*pInSlot, -1, pSoldier, bPos);
			//CHRISL: We need to make sure there are no items left in pObj or we'll lose them
			if(pObj->ubNumberOfObjects == 0)
				gTempObject.MoveThisObjectTo(*pObj, -1);
			else
				AutoPlaceObjectAnywhere(pSoldier, &gTempObject, FALSE);
		}

		else if (ubSlotLimit < pObj->ubNumberOfObjects)
		{
			//not enough room, so we free up some space
			return( FreeUpSlotIfPossibleThenPlaceObject( pSoldier, bPos, pObj ) );
		}

		else
		{
			//item fits here, swapping
			SwapObjs( pObj, pInSlot );
		}
	}

	// ATE: Put this in to see if we should update the robot, if we were given a controller...
	if ( pSoldier->bTeam == gbPlayerNum && fObjectWasRobotRemote )
	{
		pSoldier->UpdateRobotControllerGivenController( );
	}

	ApplyEquipmentBonuses(pSoldier);
	//Pulmu bugfix
	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//pInSlot->ubWeight = CalculateObjectWeight(pInSlot);
	//Pulmu end
	return( TRUE );
}

bool TryToStackInSlot(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, int bSlot)
{
	// CHRISL: Use new ItemSlotLimit function if we're using the new inventory system
	if (pSoldier->inv[bSlot].usItem == pObj->usItem && pSoldier->inv[bSlot].exists() == true)
	{
		if (pSoldier->inv[bSlot].ubNumberOfObjects < ItemSlotLimit( pObj, bSlot, pSoldier ) )
		{
			// NEW: If in SKI, don't auto-PLACE anything into a stackable slot that's currently hatched out!  Such slots
			// will disappear in their entirety if sold/moved, causing anything added through here to vanish also!
			if( !( ( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) && ShouldSoldierDisplayHatchOnItem( pSoldier->ubProfile, bSlot ) ) )
			{
				pSoldier->inv[bSlot].AddObjectsToStack(*pObj, -1, pSoldier, bSlot);
				if (pObj->exists() == false) {
					return true;
				}
			}
		}
	}
	return false;
}

bool TryToPlaceInSlot(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int& bSlot, int endSlot)
{
	//bSlot = FindEmptySlotWithin( pSoldier, bSlot, endSlot );
	//CHRISL: If something already exists, we want to fail since we can't place an object in this slot
	if (CanItemFitInPosition(pSoldier, pObj, bSlot, false) == false || pSoldier->inv[bSlot].exists() == true) {
		//bSlot = endSlot;
		return false;
	}

	if (bSlot == ITEM_NOT_FOUND) {
		bSlot = endSlot;
		return false;
	}

	if (bSlot == SECONDHANDPOS) {
		if (pSoldier->inv[HANDPOS].exists() == true) {
			return false;
		}
	}

	PlaceObject( pSoldier, bSlot, pObj, fNewItem );
	if (pObj->exists() == false) {
		return( true );
	}
	return false;
}

bool PlaceInAnySlot(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot)
{
	//first, try to STACK the item
	//try to STACK in any slot
	for(int bSlot = BODYPOSSTART; bSlot < BIGPOCKSTART; bSlot++) {
		if (bSlot != bExcludeSlot && TryToStackInSlot(pSoldier, pObj, bSlot) == true) {
			return true;
		}
	}

	if (FitsInSmallPocket(pObj) == true) {
		//try to STACK in small pockets
		for(int bSlot = SMALLPOCKSTART; bSlot < SMALLPOCKFINAL; bSlot++) {
			if (bSlot != bExcludeSlot && TryToStackInSlot(pSoldier, pObj, bSlot) == true) {
				return true;
			}
		}
	}

	//try to STACK in big pockets, and possibly medium pockets
	int bigPocketEnd = (UsingNewInventorySystem() == true) ? MEDPOCKFINAL : BIGPOCKFINAL;
	for(int bSlot = BIGPOCKSTART; bSlot < bigPocketEnd; bSlot++) {
		if (bSlot != bExcludeSlot && TryToStackInSlot(pSoldier, pObj, bSlot) == true) {
			return true;
		}
	}

	//now try to PLACE
	//try to PLACE in any body slot
	for(int bSlot = BODYPOSSTART; bSlot < BIGPOCKSTART; bSlot++) {
		if (bSlot != bExcludeSlot && TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, BODYPOSFINAL) == true) {
			return true;
		}
	}

	if(UsingNewInventorySystem() == true)
	{
		//CHRISL: Rather then a simple slot search, use PickPocket to find the most appropriate pocket to use
		INT32	sPocket=-1, mPocket=-1, lPocket=-1;
		UINT8	sCapacity=0;
		UINT8	mCapacity=0;
		UINT8	lCapacity=0;
		UINT8	capacity=0;
		int		bSlot;
		//Start with active big pockets
		sPocket=PickPocket(pSoldier, SMALLPOCKSTART, SMALLPOCKFINAL, pObj->usItem, pObj->ubNumberOfObjects, &sCapacity, bExcludeSlot);
		//Next search active medium pockets
		mPocket=PickPocket(pSoldier, MEDPOCKSTART, MEDPOCKFINAL, pObj->usItem, pObj->ubNumberOfObjects, &mCapacity, bExcludeSlot);
		//Lastly search active small pockets
		lPocket=PickPocket(pSoldier, BIGPOCKSTART, BIGPOCKFINAL, pObj->usItem, pObj->ubNumberOfObjects, &lCapacity, bExcludeSlot);
		//Finally, compare the three pockets we've found and return the pocket that is most logical to use
		capacity = min(sCapacity, mCapacity);
		capacity = min(lCapacity, capacity);
		if(capacity == 254) {	//no pocket found
			return false;
		}
		else if(capacity == sCapacity) {
			bSlot = sPocket;
		}
		else if(capacity == mCapacity) {
			bSlot = mPocket;
		}
		else if(capacity == lCapacity) {
			bSlot = lPocket;
		}
		if(TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, NUM_INV_SLOTS) == true)
			return true;
	}
	else
	{
		if (FitsInSmallPocket(pObj) == true) {
			//try to PLACE in small pockets
			for(int bSlot = SMALLPOCKSTART; bSlot < SMALLPOCKFINAL; bSlot++) {
				if (bSlot != bExcludeSlot && TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, NUM_INV_SLOTS) == true) {
					return true;
				}
			}
		}

		//try to PLACE in big pockets, and possibly medium pockets
		for(int bSlot = BIGPOCKSTART; bSlot < bigPocketEnd; bSlot++) {
			if (bSlot != bExcludeSlot && TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, bigPocketEnd) == true) {
				return true;
			}
		}
	}

	return false;
}

bool PlaceInAnyPocket(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot)
{
	//first, try to STACK the item
	if (FitsInSmallPocket(pObj) == true) {
		//try to STACK in small pockets
		for(int bSlot = SMALLPOCKSTART; bSlot < SMALLPOCKFINAL; bSlot++) {
			if (bSlot != bExcludeSlot && TryToStackInSlot(pSoldier, pObj, bSlot) == true) {
				return true;
			}
		}
	}

	//try to STACK in big pockets, and possibly medium pockets
	int bigPocketEnd = (UsingNewInventorySystem() == true) ? MEDPOCKFINAL : BIGPOCKFINAL;
	for(int bSlot = BIGPOCKSTART; bSlot < bigPocketEnd; bSlot++) {
		if (bSlot != bExcludeSlot && TryToStackInSlot(pSoldier, pObj, bSlot) == true) {
			return true;
		}
	}


	if (FitsInSmallPocket(pObj) == true) {
		//try to PLACE in small pockets
		for(int bSlot = SMALLPOCKSTART; bSlot < SMALLPOCKFINAL; bSlot++) {
			if (bSlot != bExcludeSlot && TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, NUM_INV_SLOTS) == true) {
				return true;
			}
		}
	}
	//try to PLACE in big pockets, and possibly medium pockets
	for(int bSlot = BIGPOCKSTART; bSlot < bigPocketEnd; bSlot++) {
		if (bSlot != bExcludeSlot && TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, bigPocketEnd) == true) {
			return true;
		}
	}
	return false;
}

bool PlaceInAnyBigOrMediumPocket(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot)
{
	//a special note, although some items do not fit in small pockets, and under the old system are restricted to big pockets,
	//under the new system they are intended to fit in medium pockets, if the item size and the pocket agree
	//An example would be a SMG fitting in a gun holster, which is medium.
	int bigPocketEnd = (UsingNewInventorySystem() == true) ? MEDPOCKFINAL : BIGPOCKFINAL;
	//first, try to STACK the item
	for(int bSlot = BIGPOCKSTART; bSlot < bigPocketEnd; bSlot++) {
		if (bSlot != bExcludeSlot && TryToStackInSlot(pSoldier, pObj, bSlot) == true) {
			return true;
		}
	}

	//now try to PLACE
	for(int bSlot = BIGPOCKSTART; bSlot < bigPocketEnd; bSlot++) {
		if (bSlot != bExcludeSlot && TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, bigPocketEnd) == true) {
			return true;
		}
	}
	return false;
}

bool PlaceInAnySmallPocket(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot)
{
	if (FitsInSmallPocket(pObj) == false) {
		return false;
	}

	//first, try to STACK the item
	for(int bSlot = SMALLPOCKSTART; bSlot < SMALLPOCKFINAL; bSlot++) {
		if (bSlot != bExcludeSlot && TryToStackInSlot(pSoldier, pObj, bSlot) == true) {
			return true;
		}
	}

	//try to PLACE in small pockets
	for(int bSlot = SMALLPOCKSTART; bSlot < SMALLPOCKFINAL; bSlot++) {
		if (bSlot != bExcludeSlot && TryToPlaceInSlot(pSoldier, pObj, fNewItem, bSlot, NUM_INV_SLOTS) == true) {
			return true;
		}
	}
	return false;
}

BOOLEAN AutoPlaceObjectAnywhere(SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem, INT8 bExcludeSlot)
{
	// This function will attempt to place an object into the soldier's inventory first.  If that doesn't work, it'll add the object to sector inventory
	if(pObj->exists() == false)
		return FALSE;

	if( AutoPlaceObject(pSoldier, pObj, fNewItem, bExcludeSlot) )
		return TRUE;
	else
		return (AutoPlaceObjectToWorld(pSoldier, pObj, TRUE) );

	return FALSE;
}

extern BOOLEAN IsMercInActiveSector(SOLDIERTYPE * pSoldier);
extern void CreateDestroyMapInventoryPoolButtons( BOOLEAN fExitFromMapScreen );
BOOLEAN AutoPlaceObjectToWorld(SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, INT8 bVisible)
{
	if(pObj->exists() == false)
		return FALSE;

	INT32 sGridNo = pSoldier?pSoldier->sGridNo:0;
	INT8 bLevel = pSoldier?pSoldier->pathing.bLevel:0;

	if( guiCurrentScreen == MAP_SCREEN )
	{
		// the_bob : added the check for whether pSoldier actually points to something to handle calling this function with pSoldier = NULL
		if (pSoldier){
			if(fShowMapInventoryPool && !IsMercInActiveSector(pSoldier) )
			{
				fShowMapInventoryPool = FALSE;
				CreateDestroyMapInventoryPoolButtons(FALSE);
			}

			 ChangeSelectedMapSector(pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ);
		}

		if(!fShowMapInventoryPool)
		{
			fShowMapInventoryPool = TRUE;
			CreateDestroyMapInventoryPoolButtons(FALSE);
		}
		fMapPanelDirty = TRUE;
		return( AutoPlaceObjectInInventoryStash(pObj, sGridNo) );
	}
	else
	{
		AddItemToPool(sGridNo, pObj, bVisible, bLevel, WORLD_ITEM_REACHABLE, 0);
		return TRUE;
	}

	return FALSE;
}

// CHRISL: Function needed for LBENODE
BOOLEAN AutoPlaceObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem, INT8 bExcludeSlot )
{
	INVTYPE	* pItem;
	UINT32			packCombo, backCombo;

	// statuses of extra objects would be 0 if the # exceeds the maximum
	//Assert( pObj->ubNumberOfObjects <= MAX_OBJECTS_PER_SLOT);

	//Pulmu bugfix		
	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//pObj->ubWeight = CalculateObjectWeight( pObj);
	pItem = &(Item[pObj->usItem]);
	int lbeClass;

	// Overrides to the standard system: put guns in hand, armour on body (if slot empty)
	switch (pItem->usItemClass)
	{
		case IC_GUN:
		case IC_THROWING_KNIFE:
		case IC_BLADE:
		case IC_LAUNCHER:
		case IC_BOMB:
		case IC_GRENADE:
			if (pSoldier->inv[HANDPOS].exists() == false)
			{
				// put the one-handed weapon in the guy's hand...
				PlaceObject( pSoldier, HANDPOS, pObj, fNewItem );
				if ( pObj->exists() == false )
				{
					return( TRUE );
				}
			}
			else if ( !(Item[pSoldier->inv[HANDPOS].usItem].twohanded ) && pSoldier->inv[SECONDHANDPOS].exists() == false)
			{
				// put the one-handed weapon in the guy's 2nd hand...
				PlaceObject( pSoldier, SECONDHANDPOS, pObj, fNewItem );
				if ( pObj->exists() == false )
				{
					return( TRUE );
				}
			}
			// two-handed objects are best handled in the main loop for large objects,
			// which checks the hands first anyhow
			//CHRISL: New switch.  Place items into Gunsling or Knife pocket is using NewInv, item is appropriate, and we
			//	didn't manage to place in hands.
			if(UsingNewInventorySystem() == true)
			{
				switch (pItem->usItemClass)
				{
					case IC_GUN:
						if(pSoldier->inv[GUNSLINGPOCKPOS].exists() == false)	// Long Gun use Gun Sling
						{
							PlaceObject( pSoldier, GUNSLINGPOCKPOS, pObj, fNewItem );
							if (pObj->exists() == false)
								return( TRUE );
						}
						break;
					case IC_THROWING_KNIFE:
					case IC_BLADE:
						if(pSoldier->inv[KNIFEPOCKPOS].exists() == false)	// Knife
						{
							PlaceObject( pSoldier, KNIFEPOCKPOS, pObj, fNewItem );
							if (pObj->exists() == false)
								return( TRUE );
						}
						break;
				}
			}
			break;
		case IC_ARMOUR:
			switch (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass)
			{
				case ARMOURCLASS_VEST:
					if (pSoldier->inv[VESTPOS].exists() == false)
					{
						// put on the armour!
						PlaceObject( pSoldier, VESTPOS, pObj, fNewItem );
						if ( pObj->exists() == false )
						{
							return( TRUE );
						}
					}
					break;
				case ARMOURCLASS_LEGGINGS:
					/* CHRISL:  If we're wearing leg protectors and pick up leggings, we want to leggings to override.
					This is only an issue during merc hiring when leggings will often be placed after leg protectors.
					However, this isn't as big an issue at this point because of the redesigns in the profile item sorting
					functions.*/
					if(Item[pSoldier->inv[LEGPOS].usItem].attachment)
					{
						SwapObjs(pSoldier, LEGPOS, pObj, TRUE);
						pSoldier->inv[LEGPOS].AttachObject(pSoldier, pObj, FALSE);
					}
					if (pSoldier->inv[LEGPOS].exists() == false)
					{
						// put on the armour!
						PlaceObject( pSoldier, LEGPOS, pObj, fNewItem );
					}
					if ( pObj->exists() == false )
					{
						return( TRUE );
					}
					break;
				case ARMOURCLASS_HELMET:
					if (pSoldier->inv[HELMETPOS].exists() == false)
					{
						// put on the armour!
						PlaceObject( pSoldier, HELMETPOS, pObj, fNewItem );
						if ( pObj->exists() == false )
						{
							return( TRUE );
						}
					}
					break;
				default:
					break;
			}
			// otherwise stuff it in a slot somewhere
			break;
		case IC_FACE:
			if ( (pSoldier->inv[HEAD1POS].exists() == false) && CompatibleFaceItem( pObj->usItem, pSoldier->inv[HEAD2POS].usItem ) )
			{
				PlaceObject( pSoldier, HEAD1POS, pObj, fNewItem );
				if ( pObj->exists() == false )
				{
					return( TRUE );
				}
			}
			else if ( (pSoldier->inv[HEAD2POS].exists() == false) && CompatibleFaceItem( pObj->usItem, pSoldier->inv[HEAD1POS].usItem ) )
			{
				PlaceObject( pSoldier, HEAD2POS, pObj, fNewItem );
				if ( pObj->exists() == false )
				{
					return( TRUE );
				}
			}
			break;
		// CHRISL:
		case IC_LBEGEAR:
			if(UsingNewInventorySystem() == false) {
				break;
			}
			lbeClass = LoadBearingEquipment[pItem->ubClassIndex].lbeClass;
			if(lbeClass == THIGH_PACK)	// Thigh pack
			{
				if (pSoldier->inv[LTHIGHPOCKPOS].exists() == false) {
					PlaceObject( pSoldier, LTHIGHPOCKPOS, pObj, fNewItem );
					if(pObj->exists() == false)
						return( TRUE );
				}
				if (pSoldier->inv[RTHIGHPOCKPOS].exists() == false) {
					PlaceObject( pSoldier, RTHIGHPOCKPOS, pObj, fNewItem );
					if(pObj->exists() == false)
						return( TRUE );
				}
			}
			else if(pSoldier->inv[VESTPOCKPOS].exists() == false && lbeClass == VEST_PACK)	// Vest pack
			{
				PlaceObject( pSoldier, VESTPOCKPOS, pObj, fNewItem );
				if(pObj->exists() == false)
					return( TRUE );
			}
			else if(pSoldier->inv[CPACKPOCKPOS].exists() == false && lbeClass == COMBAT_PACK)	// Combat pack
			{
				packCombo = LoadBearingEquipment[pItem->ubClassIndex].lbeCombo;
				backCombo = LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo;
				//DBrot: changed to bitwise comparison
				if((pSoldier->inv[BPACKPOCKPOS].exists() == true && packCombo != 0 && (backCombo & packCombo)) || pSoldier->inv[BPACKPOCKPOS].exists() == false)
				{
					PlaceObject( pSoldier, CPACKPOCKPOS, pObj, fNewItem );
					if(pObj->exists() == false)
						return( TRUE );
				}
			}
			else if(pSoldier->inv[BPACKPOCKPOS].exists() == false && lbeClass == BACKPACK)	// Backpack
			{
				//CHRISL: We're no longer restricting backpacks and gunslings from being used together
				//if(pSoldier->inv[GUNSLINGPOCKPOS].exists() == false)
				//{
					packCombo = LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo;
					backCombo = LoadBearingEquipment[pItem->ubClassIndex].lbeCombo;
					//DBrot: changed to bitwise comparison
					if((pSoldier->inv[CPACKPOCKPOS].exists() == true && backCombo != 0 && (backCombo & packCombo)) || pSoldier->inv[CPACKPOCKPOS].exists() == false)
					{
						PlaceObject( pSoldier, BPACKPOCKPOS, pObj, fNewItem );
						pSoldier->flags.DropPackFlag = FALSE;
						pSoldier->flags.ZipperFlag = FALSE;
						RenderBackpackButtons(ACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
						if(pObj->exists() == false)
							return( TRUE );
					}
				//}
			}
			break;
		default:
			break;
	}

	if (PlaceInAnySlot(pSoldier, pObj, (fNewItem == TRUE), bExcludeSlot) == true) {
		return TRUE;
	}
	return( FALSE );
}

BOOLEAN RemoveKeyFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj )
{
	UINT8 ubItem = 0;

	CHECKF( pObj );

	if( ( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 ) || ( pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID == INVALID_KEY_NUMBER ) )
	{
		return( FALSE );
	}
	else
	{
		// create an object
		ubItem = pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID;

		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber > 1 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber--;
		}
		else
		{

			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = 0;
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = INVALID_KEY_NUMBER;
		}

		return( CreateKeyObject( pObj, 1, ubItem ) );
	}

	return( FALSE );
}


BOOLEAN RemoveKeysFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, UINT8 ubNumberOfKeys ,OBJECTTYPE * pObj )
{
	UINT8 ubItems = 0;

	CHECKF( pObj );


	if( ( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 ) || ( pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID == INVALID_KEY_NUMBER ) )
	{
		return( FALSE );
	}
	else
	{
		//*pObj = pSoldier->inv[bPos];

		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber < ubNumberOfKeys )
		{
			ubNumberOfKeys = pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber;
		}


		ubItems = pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID;
		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber - ubNumberOfKeys > 0 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber--;
		}
		else
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = 0;
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = INVALID_KEY_NUMBER;
		}

		// create an object
		return( CreateKeyObject( pObj, ubNumberOfKeys, ubItems ) );
	}
}

// return number added
UINT8 AddKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj )
{
	UINT8 ubNumberNotAdded = 0;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC ) // redundant but what the hey
	{
		if ( KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound == 0 )
		{
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound = (UINT16) GetWorldDay();
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usSectorFound = SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );
		}
	}

	// check if we are going to far
	if ( ( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber + pObj->ubNumberOfObjects ) > ItemSlotLimit(pObj, STACK_SIZE_LIMIT) )
	{
		// only take what we can
		ubNumberNotAdded = pObj->ubNumberOfObjects - ( ItemSlotLimit(pObj, STACK_SIZE_LIMIT) - pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber );

		// set to max
		pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = ItemSlotLimit(pObj, STACK_SIZE_LIMIT);

		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = (*pObj)[0]->data.key.ubKeyID;
		}

		// return number used
		return( pObj->ubNumberOfObjects - ubNumberNotAdded );
	}
	else
	{
		// check
		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = (*pObj)[0]->data.key.ubKeyID;
		}

		pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber += pObj->ubNumberOfObjects;
	}

	return( pObj->ubNumberOfObjects );
}

UINT8 SwapKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj )
{
	// swap keys in keyring slot and keys in pocket
	// create temp object to hold keys currently in key ring slot
	CreateKeyObject( &gTempObject, pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber, pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID );

	pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = pObj->ubNumberOfObjects;
	pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = (*pObj)[0]->data.key.ubKeyID;

	// swap params?
	*pObj = gTempObject;

	return( 1 );
}


BOOLEAN CreateKeyObject( OBJECTTYPE * pObj , UINT8 ubNumberOfKeys, UINT8 ubKeyID )
{
	BOOLEAN fRet;

	pObj->initialize();
	fRet = CreateItems( (UINT16) (FIRST_KEY + LockTable[ ubKeyID ].usKeyItem), 100, ubNumberOfKeys, pObj );
	if (fRet)
	{
		(*pObj)[0]->data.key.ubKeyID = ubKeyID;
	}
	//fRet = CreateItems( (UINT16)(ubKeyIdValue + FIRST_KEY) , 100, ubNumberOfKeys, pObj )
	//return(  );
	return( fRet );
}


BOOLEAN AllocateObject( OBJECTTYPE **pObj )
{
	// create a key object
	*pObj = new OBJECTTYPE;
	Assert( pObj );

	return( TRUE );
}

BOOLEAN DeleteKeyObject( OBJECTTYPE * pObj )
{
	if( pObj == FALSE )
	{
		return( FALSE );
	}

	// free up space
	delete( pObj );

	return( TRUE );
}

UINT16 TotalPoints( OBJECTTYPE * pObj )
{
	UINT16	usPoints = 0;
	UINT8		ubLoop;

	for (ubLoop = 0; ubLoop < pObj->ubNumberOfObjects; ubLoop++)
	{
		usPoints += (*pObj)[ubLoop]->data.objectStatus;
	}
	return( usPoints );
}

UINT16 UseKitPoints( OBJECTTYPE * pObj, UINT16 usPoints, SOLDIERTYPE *pSoldier )
{
	// start consuming from the last kit in, so we end up with fewer fuller kits rather than
	// lots of half-empty ones.
	INT8		bLoop;
	UINT16 usOriginalPoints = usPoints;

	for (bLoop = pObj->ubNumberOfObjects - 1; bLoop >= 0; bLoop--)
	{
		// SANDRO - revisited this code, make the percentstatusdrainreduction count always
		if( (usPoints * (max( 0, (100 - Item[pObj->usItem].percentstatusdrainreduction)))/100) < (*pObj)[bLoop]->data.objectStatus )
		{
			(*pObj)[bLoop]->data.objectStatus -= (INT8)(usPoints * (max( 0, (100 - Item[pObj->usItem].percentstatusdrainreduction) ) )/100);
			return( usOriginalPoints );
		}
		else
		{
			// consume this kit totally
			usPoints -= (((*pObj)[bLoop]->data.objectStatus) / (max( 0, (100 - Item[pObj->usItem].percentstatusdrainreduction))) /100);
			(*pObj)[bLoop]->data.objectStatus = 0;

			pObj->ubNumberOfObjects--;
		}
		/*
		// SANDRO - heh, this is not very right solution.. in second case, the percentstatusdrainreduction should be taken into account too
		if (Item[pObj->usItem].percentstatusdrainreduction  > 0 && ((usPoints * (100 - Item[pObj->usItem].percentstatusdrainreduction))/100) < (*pObj)[bLoop]->data.objectStatus )
		{
			(*pObj)[bLoop]->data.objectStatus -= (INT8) ((usPoints * (100 - Item[pObj->usItem].percentstatusdrainreduction ) )/100);
			return( usOriginalPoints );
		}
		else if (usPoints < (UINT16) (*pObj)[bLoop]->data.objectStatus)
		{
			(*pObj)[bLoop]->data.objectStatus -= (INT8) usPoints;
			return( usOriginalPoints );
		}
		else
		{
			// consume this kit totally
			usPoints -= (*pObj)[bLoop]->data.objectStatus;
			(*pObj)[bLoop]->data.objectStatus = 0;

			pObj->ubNumberOfObjects--;
		}*/
	}

	// check if pocket/hand emptied..update inventory, then update panel
	if( pObj->exists() == false )
	{
		// Delete object
		DeleteObj( pObj );

		// dirty interface panel
		DirtyMercPanelInterface(  pSoldier, DIRTYLEVEL2 );
	}

	return( usOriginalPoints -  usPoints );
}

#ifdef PATHAI_VISIBLE_DEBUG

	extern BOOLEAN gfDrawPathPoints;

#else

	#ifdef AI_TIMING_TESTS
	extern UINT32 guiGreenTimeTotal, guiYellowTimeTotal, guiRedTimeTotal, guiBlackTimeTotal;
	extern UINT32 guiGreenCounter, guiYellowCounter, guiRedCounter, guiBlackCounter;
	extern UINT32 guiRedSeekTimeTotal, guiRedHelpTimeTotal, guiRedHideTimeTotal;
	extern UINT32 guiRedSeekCounter, guiRedHelpCounter; guiRedHideCounter;
	#endif

#endif


UINT16 MagazineClassIndexToItemType(UINT16 usMagIndex)
{
	UINT16				usLoop;

	// Note: if any ammo items in the item table are separated from the main group,
	// this function will have to be rewritten to scan the item table for an item
	// with item class ammo, which has class index usMagIndex
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType" ) );
	
	// WANNE: Now ammo can be inserted anywhere in Items.xml (before only on index > 70 [FIRST_AMMO] (fix by Realist)
	//for (usLoop = FIRST_AMMO; usLoop < MAXITEMS; usLoop++)
	for (usLoop = 0; usLoop < MAXITEMS; usLoop++)
	{
		if ( Item[usLoop].usItemClass  == 0 )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType: breaking at index %d", usLoop ) );
			break;
		}
		if (Item[usLoop].ubClassIndex == usMagIndex && Item[usLoop].usItemClass == IC_AMMO )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType: return %d", usLoop ) );
			return( usLoop );
		}
	}

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType: return none, got to %d", usLoop ) );
	return(NONE);
}


UINT16 DefaultMagazine( UINT16 usItem )
{
	WEAPONTYPE *	pWeapon;
	UINT16				usLoop;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DefaultMagazine: item = %d",usItem));
	if (!(Item[usItem].usItemClass & IC_GUN))
	{
		return( 0 );
	}

	pWeapon = &(Weapon[usItem]);
	usLoop = 0;
	while ( Magazine[usLoop].ubCalibre != NOAMMO )
	{
		if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre &&
				Magazine[usLoop].ubMagSize == pWeapon->ubMagSize &&
				AmmoTypes[ Magazine[usLoop].ubAmmoType ].standardIssue )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DefaultMagazine: found at index %d",usLoop));
			return(MagazineClassIndexToItemType(usLoop));
		}

		usLoop++;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DefaultMagazine: can't find any"));
	return( 0 );
}

UINT16 FindReplacementMagazine( UINT8 ubCalibre, UINT16 ubMagSize, UINT8 ubAmmoType )
{
	UINT16 usLoop;
	UINT16 usDefault;

	usLoop = 0;
	usDefault = NOTHING;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FindReplacementMagazine: calibre = %d, Mag size = %d, ammo type = %d",ubCalibre,ubMagSize,ubAmmoType));

	while ( Magazine[usLoop].ubCalibre != NOAMMO )
	{
		if (Magazine[usLoop].ubCalibre == ubCalibre &&
				Magazine[usLoop].ubMagSize == ubMagSize )
		{
			if ( Magazine[usLoop].ubAmmoType == ubAmmoType )
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FindReplacementMagazine: returning item = %d",MagazineClassIndexToItemType( usLoop )));
				return( MagazineClassIndexToItemType( usLoop ) );
			}
			else if ( usDefault == NOTHING )
			{
				// store this one to use if all else fails
				usDefault = MagazineClassIndexToItemType( usLoop );
			}

		}

		usLoop++;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FindReplacementMagazine: returning default item = %d",usDefault));
	return( usDefault );

}

UINT16 FindReplacementMagazineIfNecessary( UINT16 usOldGun, UINT16 usOldAmmo, UINT16 usNewGun )
{
	UINT16 usNewAmmo = NOTHING;

	if ( (Magazine[ Item[ usOldAmmo ].ubClassIndex ].ubCalibre == Weapon[ usOldGun ].ubCalibre) &&
			 (Magazine[ Item[ usOldAmmo ].ubClassIndex ].ubMagSize == Weapon[ usOldGun ].ubMagSize) )
	{
		// must replace this!
		usNewAmmo = FindReplacementMagazine( Weapon[ usNewGun ].ubCalibre, Weapon[ usNewGun ].ubMagSize, Magazine[ Item[ usOldAmmo ].ubClassIndex ].ubAmmoType );
	}

	return( usNewAmmo );
}

// increase this if any gun can have more types that this
#define MAX_AMMO_TYPES_PER_GUN		24  // MADD MARKER

UINT16 RandomMagazine( UINT16 usItem, UINT8 ubPercentStandard, UINT8 maxCoolness )
{
	// Note: if any ammo items in the item table are separated from the main group,
	// this function will have to be rewritten to scan the item table for an item
	// with item class ammo, which has class index ubLoop
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine (by index)"));

	WEAPONTYPE *	pWeapon;
	UINT16			usLoop;
	UINT16			loopItem;
	UINT16			usPossibleMagIndex[ MAX_AMMO_TYPES_PER_GUN ];
	UINT16			usPossibleMagCnt = 0;
	UINT8			ubMagChosen;

	if (!(Item[usItem].usItemClass & IC_GUN))
	{
		return( 0 );
	}

	pWeapon = &(Weapon[usItem]);

	// find & store all possible mag types that fit this gun
	usLoop = 0;
	while ( Magazine[ usLoop ].ubCalibre != NOAMMO )
	{
		loopItem = MagazineClassIndexToItemType(usLoop);

		if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre &&
				Magazine[usLoop].ubMagSize == pWeapon->ubMagSize && ItemIsLegal(loopItem)
				&& maxCoolness >= Item[loopItem].ubCoolness )
		{
			// store it! (make sure array is big enough)
			Assert(usPossibleMagCnt < MAX_AMMO_TYPES_PER_GUN);
			// Madd: check to see if allowed by army
			if ( gArmyItemChoices[ENEMYAMMOTYPES].ubChoices > 0 )
			{
				for ( int i=0;i<gArmyItemChoices[ENEMYAMMOTYPES].ubChoices;i++ )
				{
					if ( gArmyItemChoices[ENEMYAMMOTYPES].bItemNo[i] == Magazine[usLoop].ubAmmoType )
					{
						usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
						break;
					}
				}
			}
			else
			{
				usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
			}
		}

		usLoop++;
	}

	// no matches?
	if (usPossibleMagCnt == 0)
	{
		return( 0 );
	}
	else
	// only one match?
	if (usPossibleMagCnt == 1)
	{
		// use that, no choice
		return(MagazineClassIndexToItemType( usPossibleMagIndex[ 0 ] ));
	}
	else	// multiple choices
	{
		// Pick one at random, using supplied probability to pick the default
		if (Random(100) < ubPercentStandard)
		{
			ubMagChosen = 0;
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
			return( DefaultMagazine(usItem) );
		}
		else
		{
			// pick a non-standard type instead
			ubMagChosen = ( UINT8 ) (1 + Random(( UINT32 ) ( usPossibleMagCnt - 1 )));
		}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
		return( MagazineClassIndexToItemType(usPossibleMagIndex[ ubMagChosen ] ) );
	}
}

UINT16 RandomMagazine( OBJECTTYPE * pGun, UINT8 ubPercentStandard, UINT8 maxCoolness )
{
	// Note: if any ammo items in the item table are separated from the main group,
	// this function will have to be rewritten to scan the item table for an item
	// with item class ammo, which has class index ubLoop
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine"));

	WEAPONTYPE *	pWeapon;
	UINT16			usLoop;
	UINT16			loopItem;
	UINT16			usPossibleMagIndex[ MAX_AMMO_TYPES_PER_GUN ];
	UINT16			usPossibleMagCnt = 0;
	UINT8			ubMagChosen;

	if (!(Item[pGun->usItem].usItemClass & IC_GUN))
	{
		return( 0 );
	}

	pWeapon = &(Weapon[pGun->usItem]);

	// find & store all possible mag types that fit this gun
	usLoop = 0;
	while ( Magazine[ usLoop ].ubCalibre != NOAMMO )
	{
		loopItem = MagazineClassIndexToItemType(usLoop);

		if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre &&
				Magazine[usLoop].ubMagSize == GetMagSize(pGun) && ItemIsLegal(loopItem)
				&& maxCoolness >= Item[loopItem].ubCoolness )
		{
			// store it! (make sure array is big enough)
			Assert(usPossibleMagCnt < MAX_AMMO_TYPES_PER_GUN);
			// Madd: check to see if allowed by army
			if ( gArmyItemChoices[ENEMYAMMOTYPES].ubChoices > 0 )
			{
				for ( int i=0;i<gArmyItemChoices[ENEMYAMMOTYPES].ubChoices;i++ )
				{
					if ( gArmyItemChoices[ENEMYAMMOTYPES].bItemNo[i] == Magazine[usLoop].ubAmmoType )
					{
						usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
						break;
					}
				}
			}
			else
			{
				usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
			}
		}

		usLoop++;
	}

	// no matches?
	if (usPossibleMagCnt == 0)
	{
		return( 0 );
	}
	else
	// only one match?
	if (usPossibleMagCnt == 1)
	{
		// use that, no choice
		return(MagazineClassIndexToItemType(usPossibleMagIndex[ 0 ] ));
	}
	else	// multiple choices
	{
		// Pick one at random, using supplied probability to pick the default
		if (Random(100) < ubPercentStandard)
		{
			ubMagChosen = 0;
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
			return( DefaultMagazine(pGun->usItem) );
		}
		else
		{
			// pick a non-standard type instead
			ubMagChosen = ( UINT8 ) (1 + Random(( UINT32 ) ( usPossibleMagCnt - 1 )));
		}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
		return( MagazineClassIndexToItemType(usPossibleMagIndex[ ubMagChosen ] ) );
	}
}

BOOLEAN CreateGun( UINT16 usItem, INT16 bStatus, OBJECTTYPE * pObj )
{
	UINT16 usAmmo;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateGun: usItem = %d",usItem));


	Assert( pObj != NULL);
	if ( pObj == NULL )
	{
		return( FALSE );
	}

	pObj->initialize();
	pObj->usItem = usItem;
	pObj->ubNumberOfObjects = 1;
	//pObj->objectStack.resize(1);//not necessary due to init, here for code commenting
	StackedObjectData* pStackedObject = (*pObj)[0];
	pStackedObject->data.gun.bGunStatus = bStatus;
	pStackedObject->data.ubImprintID = NO_PROFILE;

	if (Weapon[ usItem ].ubWeaponClass == MONSTERCLASS)
	{
		pStackedObject->data.gun.ubGunShotsLeft = GetMagSize(pObj);
		pStackedObject->data.gun.ubGunAmmoType = AMMO_MONSTER;
		pStackedObject->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER; // 0verhaul:  Monsters don't have to reload!
	}
	else if ( EXPLOSIVE_GUN( usItem ) )
	{
		if ( Item[usItem].singleshotrocketlauncher )
		{
			pStackedObject->data.gun.ubGunShotsLeft = 1;
		}
		else
		{
			// cannon
			pStackedObject->data.gun.ubGunShotsLeft = 0;
		}
		pStackedObject->data.gun.bGunAmmoStatus = 100;
		pStackedObject->data.gun.ubGunAmmoType = 0;
	}
	else
	{
		usAmmo = DefaultMagazine( usItem );
		//CHRISL: Why do we have an assert here when the very next condition says to return a FALSE if usAmmo is 0?
		//Assert( usAmmo != 0 );
		if (usAmmo == 0)
		{
			// item's calibre & mag size not found in magazine list!
			return( FALSE );
		}
		else
		{
			pStackedObject->data.gun.usGunAmmoItem = usAmmo;
			pStackedObject->data.gun.ubGunAmmoType = Magazine[ Item[ usAmmo ].ubClassIndex].ubAmmoType;
			pStackedObject->data.gun.bGunAmmoStatus = 100;
			pStackedObject->data.gun.ubGunShotsLeft = Magazine[ Item[ usAmmo ].ubClassIndex ].ubMagSize;
			pStackedObject->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER; // Madd: new guns should have cartridge in chamber
		}
	}

	//WarmSteel - Init item slots.
	if(UsingNewAttachmentSystem()==true)
		InitItemAttachments(pObj);

	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//pObj->ubWeight = CalculateObjectWeight( pObj );
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateGun: Done"));
	// succesful
	return( TRUE );
}

BOOLEAN CreateAmmo( UINT16 usItem, OBJECTTYPE * pObj, UINT16 ubShotsLeft )
{
	if (pObj == NULL)
	{
		return( FALSE );
	}
	pObj->initialize();
	pObj->usItem = usItem;
	pObj->ubNumberOfObjects = 1;
	//pObj->objectStack.resize(1);//not necessary due to init, here for code commenting
	//if (ubShotsLeft == 0) {
	//	(*pObj)[0]->data.ubShotsLeft = Magazine[ Item[usItem].ubClassIndex ].ubMagSize;
	//}
	//else {
		(*pObj)[0]->data.ubShotsLeft = ubShotsLeft;
	//}
	//WarmSteel - Init attachment slots.
	if(UsingNewAttachmentSystem()==true)
		InitItemAttachments(pObj);

	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//pObj->ubWeight = CalculateObjectWeight( pObj );
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateAmmo: done"));

	return( TRUE );
}

BOOLEAN CreateItem( UINT16 usItem, INT16 bStatus, OBJECTTYPE * pObj )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateItem: usItem = %d",usItem));
	BOOLEAN fRet;

	if (usItem >= MAXITEMS)
	{
		DebugBreakpoint();
		return( FALSE );
	}
	if (Item[ usItem ].usItemClass == IC_GUN)
	{
		fRet = CreateGun( usItem, bStatus, pObj );
	}
	else if (Item[ usItem ].usItemClass == IC_AMMO)
	{
		fRet = CreateAmmo( usItem, pObj, Magazine[Item[usItem].ubClassIndex].ubMagSize );
	}
	else
	{
		pObj->initialize();
		pObj->usItem = usItem;
		pObj->ubNumberOfObjects = 1;
		//pObj->objectStack.resize(1);//not necessary due to init, here for code commenting
		if (usItem == MONEY || Item[usItem].usItemClass == IC_MONEY )
		{
			// special case... always set status to 100 when creating
			// and use status value to determine amount!
			(*pObj)[0]->data.objectStatus = 100;
			(*pObj)[0]->data.money.uiMoneyAmount = bStatus * 50;
		}
		else
		{
			(*pObj)[0]->data.objectStatus = bStatus;
		}
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//pObj->ubWeight = CalculateObjectWeight( pObj );
		fRet = TRUE;
		//WarmSteel - Init attachment slots.
		if(UsingNewAttachmentSystem()==true)
			InitItemAttachments(pObj);

	}
	if (fRet)
	{
//		if (Item[ usItem ].fFlags & ITEM_DEFAULT_UNDROPPABLE)
		if (Item[ usItem ].defaultundroppable )
		{
			(*pObj).fFlags |= OBJECT_UNDROPPABLE;
		}
		for(UINT8 cnt = 0; cnt < MAX_DEFAULT_ATTACHMENTS; cnt++){
			if(Item [ usItem ].defaultattachments[cnt] == 0)
				break;

			//cannot use gTempObject
			static OBJECTTYPE defaultAttachment;
			CreateItem(Item [ usItem ].defaultattachments[cnt],100,&defaultAttachment);
			pObj->AttachObject(NULL,&defaultAttachment, FALSE);
		}
	}


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateItem: return %d",fRet));
	return( fRet );
}

BOOLEAN CreateItems( UINT16 usItem, INT8 bStatus, UINT8 ubNumber, OBJECTTYPE * pObj )
{
	BOOLEAN fOk;
	fOk = CreateItem( usItem, bStatus, pObj );
	if (fOk)
	{
		if (ubNumber > 1) {
			//no need to recalc weight if just 1, CreateItem did that.
			for (int x = 1; x < ubNumber; ++x) {
				pObj->objectStack.push_back(pObj->objectStack.front());
			}
			pObj->ubNumberOfObjects = ubNumber;
			//ADB ubWeight has been removed, see comments in OBJECTTYPE
			//pObj->ubWeight = CalculateObjectWeight(pObj);
		}
		return( TRUE );
	}
	return( FALSE );
}

BOOLEAN CreateMoney( UINT32 uiMoney, OBJECTTYPE * pObj )
{
	BOOLEAN fOk;

	fOk = CreateItem( MONEY, 100, pObj );
	if (fOk)
	{
		(*pObj)[0]->data.money.uiMoneyAmount = uiMoney;
	}
	return( fOk );
}

BOOLEAN ArmBomb( OBJECTTYPE * pObj, INT8 bSetting )
{
	BOOLEAN fRemote = FALSE;
	BOOLEAN fPressure = FALSE;
	BOOLEAN fTimed = FALSE;
	BOOLEAN	fSwitch = FALSE;

	if (pObj->usItem == ACTION_ITEM)
	{
		switch( (*pObj)[0]->data.misc.bActionValue )
		{
			case ACTION_ITEM_SMALL_PIT:
			case ACTION_ITEM_LARGE_PIT:
				fPressure = TRUE;
				break;
			default:
				fRemote = TRUE;
				break;

		}
	}
	else if ( IsDetonatorAttached( pObj ) )
	{
		fTimed = TRUE;
	}
	else if ( (IsRemoteDetonatorAttached( pObj ) ) || (pObj->usItem == ACTION_ITEM) )
	{
		fRemote = TRUE;
	}
	else if ( Item[pObj->usItem].mine || pObj->usItem == TRIP_FLARE || pObj->usItem == TRIP_KLAXON || pObj->usItem == ACTION_ITEM )
	{
		fPressure = TRUE;
	}
	else if ( pObj->usItem == SWITCH )
	{
		// this makes a remote detonator into a pressure-sensitive trigger
		if ( bSetting == PANIC_FREQUENCY )
		{
			// panic trigger is only activated by expending APs, not by
			// stepping on it... so don't define a detonator type
			fSwitch = TRUE;
		}
		else
		{
			fPressure = TRUE;
		}
	}
	else
	{
		// no sorta bomb at all!
		return( FALSE );
	}

	if (fRemote)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_REMOTE;
		(*pObj)[0]->data.misc.bFrequency = bSetting;
	}
	else if (fPressure)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_PRESSURE;
		(*pObj)[0]->data.misc.bFrequency = 0;
	}
	else if (fTimed)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_TIMED;
		// In realtime the player could choose to put down a bomb right before a turn expires, SO
		// add 1 to the setting in RT
		(*pObj)[0]->data.misc.bDelay = bSetting;
		if ( !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
		{
			(*pObj)[0]->data.misc.bDelay++;
		}

	}
	else if (fSwitch)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_SWITCH;
		(*pObj)[0]->data.misc.bFrequency = bSetting;
	}
	else
	{
		return( FALSE );
	}

	(*pObj).fFlags |= OBJECT_ARMED_BOMB;
	(*pObj)[0]->data.misc.usBombItem = pObj->usItem;
	return( TRUE );
}

BOOLEAN OBJECTTYPE::RemoveAttachment( OBJECTTYPE* pAttachment, OBJECTTYPE * pNewObj, UINT8 subObject, SOLDIERTYPE * pSoldier, BOOLEAN fForceInseperable, BOOLEAN fRemoveProhibited )
{
	BOOLEAN		objDeleted = FALSE;
	std::vector<UINT16> usAttachmentSlotIndexVector;
	std::vector<UINT16> usRemAttachmentSlotIndexVector;
	UINT16		oldMagSize = 0;

	if ( pAttachment->exists() == false || this->exists() == false)
	{
		return( FALSE );
	}

	//Sometimes we have to force inseparable items off, sadly.
	if ( Item[ pAttachment->usItem ].inseparable && !fForceInseperable )
	{
		return( FALSE );
	}

	//CHRISL: This is so we can try to determine if the removed attachment altered our mag size.
	if(Item[this->usItem].usItemClass == IC_GUN)
		oldMagSize = GetMagSize(this);

	//CHRISL: I know this FOR loop is basically redundant to what the remove() function already does, but
	//	this setup includes a failsafe.  Now we'll only copy the attachment to our cursor (pNewObj) if
	//	iter and pAttachment are the same.  This should stop attachment duplication though it doesn't resolve
	//	the initial cause of the attachment duplication issue.

	//First look for attachment with the exact same memory adress.
	//This is so that you can click to remove an attachment and it will take off THAT attachment and not the first one that just "looks" like it.
	for (std::list<OBJECTTYPE>::iterator iter = (*this)[subObject]->attachments.begin();
		iter != (*this)[subObject]->attachments.end(); ++iter){
			//Compare the adress
			if(&(*iter) == pAttachment)
			{
				if(pNewObj != NULL)
				{
					*pNewObj = *pAttachment;
				}

				iter = (*this)[subObject]->RemoveAttachmentAtIter(iter);

				objDeleted = TRUE;
				break;
			}
	}
	//It is possible that the previous loop did not find the EXACT attachment we wanted to delete, look if there is one that is at least equal in data.
	if(!objDeleted){
		for (std::list<OBJECTTYPE>::iterator iter = (*this)[subObject]->attachments.begin();
			iter != (*this)[subObject]->attachments.end(); ++iter){
				//This compares the internal data of the objects.
				if(*iter == *pAttachment)
				{
					if(pNewObj != NULL)
					{
						*pNewObj = *pAttachment;
					}

					iter = (*this)[subObject]->RemoveAttachmentAtIter(iter);

					objDeleted = TRUE;
					break;
				}
		}
	}
	if(!objDeleted)
		return( FALSE );

	//After removing an attachment, the ammo capacity might have changed.
	if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[subObject]->data.gun.usGunAmmoItem != NONE && (*this)[subObject]->data.gun.ubGunShotsLeft > 0 && oldMagSize != GetMagSize(this, subObject) )
	{
		if ( (*this)[subObject]->data.gun.ubGunShotsLeft > GetMagSize(this, subObject) )
		{ //Too many rounds, eject ammo
			EjectAmmoAndPlace(pSoldier, this, subObject);
		}
		//CHRISL: We should update the usGunAmmoItem if we've changed the ammo capacity
		if((*this)[subObject]->data.gun.usGunAmmoItem != NONE){
			UINT16 usNewAmmoItem;
			usNewAmmoItem = FindReplacementMagazine(Weapon[this->usItem].ubCalibre ,GetMagSize(this, subObject),Magazine[Item[(*this)[subObject]->data.gun.usGunAmmoItem].ubClassIndex].ubAmmoType);
			(*this)[subObject]->data.gun.usGunAmmoItem = usNewAmmoItem;
		}
	}

	if(Item[this->usItem].usItemClass == IC_GUN && oldMagSize != GetMagSize(this, subObject)){
		fInterfacePanelDirty = DIRTYLEVEL2;
		RenderBulletIcon(this, subObject);
	}

	//CHRISL: We need to know if the removed attachment could have altered the base items potential attachments
	BOOLEAN	removeAttachments = TRUE, cleanAttachments = FALSE;
	INT8	loopCount = 0;
	while(removeAttachments){
		usRemAttachmentSlotIndexVector = GetItemSlots(pNewObj);
		if(usRemAttachmentSlotIndexVector.empty()){
			removeAttachments = FALSE;
		} else {
			cleanAttachments = TRUE;
			usAttachmentSlotIndexVector = GetItemSlots(this, subObject);
			for(attachmentList::iterator iter = (*this)[subObject]->attachments.begin(); iter != (*this)[subObject]->attachments.end(); ++iter){
				removeAttachments = FALSE;
				if(iter->exists()){
					if(!ValidItemAttachment(this, iter->usItem, FALSE, FALSE, subObject, usAttachmentSlotIndexVector)){	//attachment is no longer valid
						removeAttachments = TRUE;
						OBJECTTYPE	remObj;
						remObj = *iter;
						if(ValidItemAttachment(pNewObj, iter->usItem, FALSE, FALSE, 0, usRemAttachmentSlotIndexVector)){
							(*pNewObj)[0]->attachments.push_back((*iter));
							iter = (*this)[subObject]->RemoveAttachmentAtIter(iter);
							continue;
						}
						if(loopCount > 5){	//try moving attachments to the removed attachment 5 times before we drop anything
							if ( pSoldier && AutoPlaceObject( pSoldier, &remObj, FALSE ) )
							{
								iter = (*this)[subObject]->RemoveAttachmentAtIter(iter);
							} else {	// put it on the ground
								INT8 pathing = (pSoldier?pSoldier->pathing.bLevel:0);
								INT32 sGridNo = (pSoldier?pSoldier->sGridNo:0);
								if( AutoPlaceObjectToWorld(pSoldier, &remObj) )
									iter = (*this)[subObject]->RemoveAttachmentAtIter(iter);
								/*if(guiCurrentItemDescriptionScreen == MAP_SCREEN && fShowMapInventoryPool){
									if(AutoPlaceObjectInInventoryStash(&remObj, sGridNo)){
										iter = (*this)[subObject]->RemoveAttachmentAtIter(iter);
									}
								} else {
									AddItemToPool( sGridNo, &remObj, 1, pathing, WORLD_ITEM_REACHABLE, 0 );
									iter = (*this)[subObject]->RemoveAttachmentAtIter(iter);
								}*/
							}
						}
					}
				}
			}
		}
		loopCount++;
	}
	if(cleanAttachments){
		RemoveProhibitedAttachments(pSoldier, pNewObj, pNewObj->usItem);
		RemoveProhibitedAttachments(pSoldier, this, this->usItem);
	}
	if (pNewObj->exists() && Item[pNewObj->usItem].grenadelauncher )//UNDER_GLAUNCHER)
	{
		// look for any grenade; if it exists, we must make it an
		// attachment of the grenade launcher
		OBJECTTYPE* pGrenade = FindAttachmentByClass( this, IC_GRENADE );
		if (pGrenade->exists())
		{
			pNewObj->AttachObject(NULL, pGrenade, FALSE, 0, -1, 0);
			//ADB ubWeight has been removed, see comments in OBJECTTYPE
			//pNewObj->ubWeight = CalculateObjectWeight( pNewObj );
			this->RemoveAttachment(pGrenade, NULL, 0, NULL, 1, 0);
		}
	}
	//Removing an attachment can alter slots, check them.
	if(UsingNewAttachmentSystem()==true && fRemoveProhibited){
		RemoveProhibitedAttachments(pSoldier, this, this->usItem);
	}

	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//this->ubWeight = CalculateObjectWeight( this );
	return( TRUE );
}

void SetNewItem( SOLDIERTYPE *pSoldier, UINT8 ubInvPos, BOOLEAN fNewItem )
{
	if( fNewItem )
	{
		pSoldier->inv.bNewItemCount[ ubInvPos ]						 = -1;
		pSoldier->inv.bNewItemCycleCount[ ubInvPos ]			 = NEW_ITEM_CYCLE_COUNT;
		pSoldier->flags.fCheckForNewlyAddedItems             = TRUE;
	}
}


BOOLEAN PlaceObjectInSoldierProfile( UINT8 ubProfile, OBJECTTYPE *pObject )
{
	INT8				bLoop;
	SOLDIERTYPE *pSoldier;
	UINT16			usItem;
	INT16				bStatus;
	BOOLEAN			fReturnVal = FALSE;

	usItem	= pObject->usItem;
	Assert(pObject->ubNumberOfObjects == 1);
	bStatus = (*pObject)[0]->data.objectStatus;
	pSoldier = FindSoldierByProfileID( ubProfile, FALSE );

	if ( Item[ usItem ].usItemClass == IC_MONEY && gMercProfiles[ ubProfile ].uiMoney > 0 )
	{
		gMercProfiles[ ubProfile ].uiMoney += (*pObject)[0]->data.money.uiMoneyAmount;
		SetMoneyInSoldierProfile( ubProfile, gMercProfiles[ ubProfile ].uiMoney );
		return( TRUE );
	}

	// CHRISL:
	for (bLoop = BIGPOCKSTART; bLoop < NUM_INV_SLOTS; bLoop++)
	{
		if ( gMercProfiles[ ubProfile ].bInvNumber[ bLoop ] == 0 && (pSoldier == NULL || pSoldier->inv[ bLoop ].exists() == false ) )
		{

			// CJC: Deal with money by putting money into # stored in profile
			if ( Item[ usItem ].usItemClass == IC_MONEY )
			{
				gMercProfiles[ ubProfile ].uiMoney += (*pObject)[0]->data.money.uiMoneyAmount;
				// change any gold/silver to money
				usItem = MONEY;
			}
			else
			{
				gMercProfiles[ ubProfile ].inv[ bLoop ] = usItem;
				gMercProfiles[ ubProfile ].bInvStatus[ bLoop ] = bStatus;
				gMercProfiles[ ubProfile ].bInvNumber[ bLoop ] = pObject->ubNumberOfObjects;
			}

			fReturnVal = TRUE;
			break;
		}
	}

	//uiMoneyAmount
	if ( fReturnVal )
	{
		// ATE: Manage soldier pointer as well....
		//pSoldier = FindSoldierByProfileID( ubProfile, FALSE );

		// Do we have a valid profile?
		if ( pSoldier != NULL )
		{
			// OK, place in soldier...
			if ( usItem == MONEY )
			{
				CreateMoney( gMercProfiles[ ubProfile ].uiMoney, &(pSoldier->inv[ bLoop ] ) );
			}
			else
			{
				if ( pSoldier->ubProfile == MADLAB )
				{
					// remove ammo and drop
					pSoldier->pTempObject = new OBJECTTYPE;
					EmptyWeaponMagazine( pObject, pSoldier->pTempObject );
					AddItemToPool( pSoldier->sGridNo, pSoldier->pTempObject, 1, 0, 0, 0 );
					pSoldier->pTempObject = NULL;
					// remove attachments and drop them
					for (attachmentList::iterator iter = (*pObject)[0]->attachments.begin(); iter != (*pObject)[0]->attachments.end();) {
						//CHRISL: Because MADLAB needs to remove all attachments, even inseparable ones, we need to temporarily
						//	make all attachments removable.
						if(!iter->exists()){
							++iter;
							continue;
						}
						BOOLEAN	old_inseparable = FALSE;
						UINT32	old_item = iter->usItem;
						// drop it in Madlab's tile
						AddItemToPool( pSoldier->sGridNo, &(*iter), 1, 0, 0, 0 );
						old_inseparable = Item[old_item].inseparable;
						Item[old_item].inseparable = FALSE;
						pObject->RemoveAttachment(&(*iter));
						Item[old_item].inseparable = old_inseparable;
						if ((*pObject)[0]->AttachmentListSize() == 0) {
							break;
						}
						else{
							iter = (*pObject)[0]->attachments.begin();
						}
					}
				}

				CreateItem( usItem, bStatus, &(pSoldier->inv[ bLoop ] ) );
			}
		}
	}

	return( fReturnVal );
}

BOOLEAN RemoveObjectFromSoldierProfile( UINT8 ubProfile, UINT16 usItem )
{
	SOLDIERTYPE *pSoldier;
	BOOLEAN	fReturnVal = FALSE;

	if ( usItem == NOTHING )
	{
		return( TRUE );
	}
	MERCPROFILESTRUCT* pProfile = &gMercProfiles[ ubProfile ];
	for (unsigned int bLoop = 0; bLoop < pProfile->inv.size(); bLoop++)
	{
		if ( pProfile->inv[ bLoop ] == usItem )
		{
			pProfile->inv[ bLoop ] = NOTHING;
			pProfile->bInvStatus[ bLoop ] = 0;
			pProfile->bInvNumber[ bLoop ] = 0;

			fReturnVal = TRUE;
			break;
		}
	}

	// ATE: Manage soldier pointer as well....
	pSoldier = FindSoldierByProfileID( ubProfile, FALSE );

	// Do we have a valid profile?
	if ( pSoldier != NULL )
	{
		// Remove item...
		RemoveInvObject( pSoldier, usItem );
	}

	return( fReturnVal );
}


void SetMoneyInSoldierProfile( UINT8 ubProfile, UINT32 uiMoney )
{
	BOOLEAN					fRet;

	// remove all money from soldier
	do
	{
		fRet = RemoveObjectFromSoldierProfile( ubProfile, MONEY );
	}
	while ( fRet == TRUE );

	gMercProfiles[ ubProfile ].uiMoney = 0;

	if (uiMoney > 0)
	{
		// now add the amount specified
		CreateMoney( uiMoney, &gTempObject );
		PlaceObjectInSoldierProfile( ubProfile, &gTempObject );
	}
}

INT8 FindObjectInSoldierProfile( UINT8 ubProfile, UINT16 usItem )
{

	MERCPROFILESTRUCT* pProfile = &gMercProfiles[ ubProfile ];
	for (unsigned int bLoop = 0; bLoop < pProfile->inv.size(); bLoop++)
	{
		if ( pProfile->bInvNumber[ bLoop ] > 0 )
		{
			if ( pProfile->inv[ bLoop ] == usItem )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

BOOLEAN ObjectExistsInSoldierProfile( UINT8 ubProfile, UINT16 usItem )
{
	INT8	bSlot;

	bSlot = FindObjectInSoldierProfile( ubProfile, usItem );
	return( bSlot != NO_SLOT );
}

void RemoveInvObject( SOLDIERTYPE *pSoldier, UINT16 usItem )
{
	INT8 bInvPos;

	// find object
	bInvPos = FindObj( pSoldier, usItem );
	if (bInvPos != NO_SLOT)
	{
		// Erase!
		DeleteObj(&pSoldier->inv[ bInvPos ]);

		//Dirty!
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}

}

INT8 CheckItemForDamage( UINT16 usItem, INT32 iMaxDamage )
{
	INT8	bDamage = 0;

	// if the item is protective armour, reduce the amount of damage
	// by its armour value
	if (Item[usItem].usItemClass == IC_ARMOUR)
	{
		iMaxDamage -= (iMaxDamage * Armour[Item[usItem].ubClassIndex].ubProtection) / 100;
	}
	// metal items are tough and will be damaged less
//	if (Item[usItem].fFlags & ITEM_METAL)
	if (Item[usItem].metal )
	{
		iMaxDamage /= 2;
	}
	else if ( usItem == BLOODCAT_PELT )
	{
		iMaxDamage *= 2;
	}
	if (iMaxDamage > 0)
	{
		bDamage = (INT8) PreRandom( iMaxDamage );
	}
	return( bDamage );
}

BOOLEAN CheckForChainReaction( UINT16 usItem, INT16 bStatus, INT16 bDamage, BOOLEAN fOnGround )
{
	INT32 iChance;

	iChance = Explosive[Item[usItem].ubClassIndex].ubVolatility;
	if (iChance > 0)
	{

		// Scale the base chance by the damage caused to the item
		// (bigger the shock, bigger chance) and the condition of
		// the item after being hit!
		if (fOnGround)
		{
			// improve chance to make it practical to blow up explosives on the ground
			iChance = 50 + (iChance - 1) * 10;
		}
		// SANDRO - experimental - increased chance to chain reactions of carried explosives a bit
		else
		{
			iChance = 3 + (iChance - 1) * 3;
		}

		iChance = iChance * ( 100 + ( (100 - bStatus) + bDamage ) / 2 ) / 100;
		if ((INT32) PreRandom( 100 ) < iChance)
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN DamageItem( OBJECTTYPE * pObject, INT32 iDamage, BOOLEAN fOnGround )
{
	INT8		bLoop;
	INT16		bDamage;

	if ( (Item[pObject->usItem].damageable  || Item[ pObject->usItem ].usItemClass == IC_AMMO) && pObject->exists() == true)
	{

		for (bLoop = 0; bLoop < pObject->ubNumberOfObjects; bLoop++)
		{
			bool removed = false;
			// if the status of the item is negative then it's trapped/jammed;
			// leave it alone
			if (pObject->exists() == true && (*pObject)[bLoop]->data.objectStatus > 0)
			{
				bDamage = CheckItemForDamage( pObject->usItem, iDamage );
				switch( pObject->usItem )
				{
					case JAR_CREATURE_BLOOD:
					case JAR:
					case JAR_HUMAN_BLOOD:
					case JAR_ELIXIR:
						if ( PreRandom( bDamage ) > 5 )
						{
							// smash!
							bDamage = (*pObject)[bLoop]->data.objectStatus;
						}
						break;
					default:
						break;
				}
				if ( Item[ pObject->usItem ].usItemClass == IC_AMMO  )
				{
					if ( PreRandom( 100 ) < (UINT32) bDamage )
					{
						// destroy clip completely
						(*pObject)[bLoop]->data.objectStatus = 1;
					}
				}
				else
				{
					(*pObject)[bLoop]->data.objectStatus -= bDamage;
					if ((*pObject)[bLoop]->data.objectStatus < 1)
					{
						(*pObject)[bLoop]->data.objectStatus = 1;
					}
				}
				// I don't think we increase viewrange based on items any more
				// FUN STUFF!  Check for explosives going off as a result!
				if (Item[pObject->usItem].usItemClass & IC_EXPLOSV)
				{
					if (CheckForChainReaction( pObject->usItem, (*pObject)[bLoop]->data.objectStatus, bDamage, fOnGround ))
					{
						return( TRUE );
					}
				}

				// remove item from index AFTER checking explosions because need item data for explosion!
				if ( (*pObject)[bLoop]->data.objectStatus == 1 )
				{
					if ( pObject->ubNumberOfObjects > 1 )
					{
						removed = true;
						pObject->RemoveObjectAtIndex( bLoop );
						// since an item was just removed, the items above the current were all shifted down one;
						// to process them properly, we have to back up 1 in the counter
						--bLoop;
					}
				}
			}

			if (removed == false) {
				for (attachmentList::iterator iter = (*pObject)[bLoop]->attachments.begin(); iter != (*pObject)[bLoop]->attachments.end();) {
					DamageItem(&(*iter), iDamage, fOnGround);
					//could have removed the object at iter
					if (iter->exists() == true) {
						++iter;
					}
					else {				
						iter = (*pObject)[bLoop]->RemoveAttachmentAtIter(iter);

						if(UsingNewAttachmentSystem()==true)
							++iter;
					}
				}
			}
		}
	}

	return( FALSE );
}

void CheckEquipmentForDamage( SOLDIERTYPE *pSoldier, INT32 iDamage )
{
	INT8				bSlot;
	BOOLEAN			fBlowsUp;
	UINT8				ubNumberOfObjects;

	if ( TANK( pSoldier ) )
	{
		return;
	}

	for (bSlot = 0; bSlot < (INT8) pSoldier->inv.size(); bSlot++)
	{
		if (pSoldier->inv[bSlot].exists() == false) {
			continue;
		}
		ubNumberOfObjects = pSoldier->inv[bSlot].ubNumberOfObjects;
		fBlowsUp = DamageItem( &(pSoldier->inv[bSlot]), iDamage, FALSE );
		if (fBlowsUp)
		{
			// blow it up!
			if ( gTacticalStatus.ubAttackBusyCount )
			{
				IgniteExplosion( pSoldier->ubAttackerID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, pSoldier->inv[ bSlot ].usItem, pSoldier->pathing.bLevel );
			}
			else
			{
				IgniteExplosion( pSoldier->ubID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, pSoldier->inv[ bSlot ].usItem, pSoldier->pathing.bLevel );
			}

			//ADB when something in a stack blows up the whole stack goes, so no need to worry about number of items
			// Remove item!
			DeleteObj( &(pSoldier->inv[ bSlot ]) );

			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
		}
		else if ( ubNumberOfObjects != pSoldier->inv[bSlot].ubNumberOfObjects )
		{
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
		}
	}
}

void CheckEquipmentForFragileItemDamage( SOLDIERTYPE *pSoldier, INT32 iDamage )
{
	// glass jars etc can be damaged by falling over
	INT8				bSlot;
	UINT8				ubNumberOfObjects;
	BOOLEAN			fPlayedGlassBreak = FALSE;

	for (bSlot = 0; bSlot < (INT8) pSoldier->inv.size(); bSlot++)
	{
		if (pSoldier->inv[bSlot].exists() == false) {
			continue;
		}
		switch( pSoldier->inv[bSlot].usItem )
		{
			case JAR_CREATURE_BLOOD:
			case JAR:
			case JAR_HUMAN_BLOOD:
			case JAR_ELIXIR:
				ubNumberOfObjects = pSoldier->inv[bSlot].ubNumberOfObjects;
				DamageItem( &(pSoldier->inv[bSlot]), iDamage, FALSE );
				if ( !fPlayedGlassBreak && (ubNumberOfObjects != pSoldier->inv[bSlot].ubNumberOfObjects) )
				{
					PlayJA2Sample( GLASS_CRACK, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
					fPlayedGlassBreak = TRUE;
					// only dirty once
					DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				}
				break;
			default:
				break;
		}
	}
}


BOOLEAN DamageItemOnGround( OBJECTTYPE * pObject, INT32 sGridNo, INT8 bLevel, INT32 iDamage, UINT8 ubOwner )
{
#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"DamageItemOnGround ( usItem : %i , sGridNo : %i , bLevel : %i , iDamage : %i , ubOwner : %i )\n",pObject->usItem, sGridNo , bLevel , iDamage , ubOwner );
	MPDebugMsg(tmpMPDbgString);
#endif

	BOOLEAN			fBlowsUp;

	fBlowsUp = DamageItem( pObject, iDamage, TRUE );
	if ( fBlowsUp )
	{
		// OK, Ignite this explosion!
		IgniteExplosion( ubOwner, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, pObject->usItem, bLevel );

		// SANDRO - merc records
		if ( (pObject->fFlags & OBJECT_ARMED_BOMB) && ((*pObject)[0]->data.misc.ubBombOwner > 1) )
		{
			if ( MercPtrs[ ((*pObject)[0]->data.misc.ubBombOwner - 2) ]->ubProfile != NO_PROFILE && MercPtrs[ ((*pObject)[0]->data.misc.ubBombOwner - 2) ]->bTeam == gbPlayerNum ) 
				gMercProfiles[ MercPtrs[ ((*pObject)[0]->data.misc.ubBombOwner - 2) ]->ubProfile ].records.usExpDetonated++;
		}

		// Remove item!
		return( TRUE );
	}
	else if ( pObject->exists() == true && (pObject->ubNumberOfObjects < 2) && ((*pObject)[0]->data.objectStatus < USABLE) )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

// is the item a medical kit/first aid kit item?
INT8 IsMedicalKitItem( OBJECTTYPE *pObject )
{
	// check item id against current medical kits
	if ( Item[pObject->usItem].medicalkit && pObject->exists() == true)
		return 1;

	//switch( pObject->usItem )
	//{
	//	case( MEDICKIT ):
	//		// medical bag, return 1
	//		return ( 1 );
	//	break;
	//}

	return( 0 );
}

void SwapHandItems( SOLDIERTYPE * pSoldier )
{
	BOOLEAN		fOk;

	CHECKV( pSoldier );
	if (pSoldier->inv[HANDPOS].exists() == false || pSoldier->inv[SECONDHANDPOS].exists() == false)
	{
		// whatever is in the second hand can be swapped to the main hand!
		SwapObjs( pSoldier, HANDPOS, SECONDHANDPOS, TRUE );
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
	else
	{
		if (TwoHandedItem( pSoldier->inv[SECONDHANDPOS].usItem ) )
		{
			// must move the item in the main hand elsewhere in the inventory
			fOk = AutoPlaceObject( pSoldier, &(pSoldier->inv[HANDPOS]), FALSE, HANDPOS );
			if (!fOk)
			{
				return;
			}
			// the main hand is now empty so a swap is going to work...
		}
		SwapObjs( pSoldier, HANDPOS, SECONDHANDPOS, TRUE );
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
}

void SwapOutHandItem( SOLDIERTYPE * pSoldier )
{
	BOOLEAN			fOk;

	CHECKV( pSoldier );

	// puts away the item in the main hand
	if (pSoldier->inv[HANDPOS].exists() == true )
	{
		if (pSoldier->inv[SECONDHANDPOS].exists() == false)
		{
			// just swap the hand item to the second hand
			SwapObjs( pSoldier, HANDPOS, SECONDHANDPOS, TRUE );
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			return;
		}
		else
		{
			// try placing it somewhere else in our inventory
			fOk = AutoPlaceObject( pSoldier, &(pSoldier->inv[HANDPOS]), FALSE );
			if (fOk)
			{
				DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			}
			// otherwise there's no room for the item anywhere!
		}
	}
}

void WaterDamage( SOLDIERTYPE *pSoldier )
{
	// damage guy's equipment and camouflage due to water
	INT8		bLoop, bDamage, bDieSize;
	UINT32	uiRoll;

	if ( pSoldier->MercInDeepWater( ) )
	{
		for ( bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++ )
		{
			if (pSoldier->inv[bLoop].exists() == false) {
				continue;
			}
			// if there's an item here that can get water damaged...
//			if (pSoldier->inv[ bLoop ].usItem && Item[pSoldier->inv[ bLoop ].usItem].fFlags & ITEM_WATER_DAMAGES)
			if (pSoldier->inv[ bLoop ].usItem && Item[pSoldier->inv[ bLoop ].usItem].waterdamages )
			{
				// roll the 'ol 100-sided dice
				uiRoll = PreRandom(100);

				// 10% chance of getting damage!
				if (uiRoll < 10)
				{
					// lose between 1 and 10 status points each time
					bDamage = (INT8) (10 - uiRoll);

					// but don't let anything drop lower than 1%
					pSoldier->inv[bLoop][0]->data.objectStatus -= bDamage;
					if (pSoldier->inv[bLoop][0]->data.objectStatus < 1)
					{
						pSoldier->inv[bLoop][0]->data.objectStatus = 1;
					}
				}
			}
		}
	}
	BOOLEAN camoWoreOff = FALSE;

	/////////////////////////////////////////////////////////////////////////////////////
	// ADDED BY SANDRO - Ranger trait makes camouflage reduction lesser
	// I've messed this part a little to be more clean, the different camouflaged traits have been merged into one
	if ( gGameOptions.fNewTraitSystem)
	{
		// reduce camouflage by 2% per tile of deep water
		// and 1% for medium water
		if ( pSoldier->bCamo > 0 )
		{
			if ( HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) )
			{
				if ( pSoldier->MercInDeepWater( ) )
				{
					pSoldier->bCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
					pSoldier->bCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
				else
				{
					pSoldier->bCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
			}
			else
			{
				if ( pSoldier->MercInDeepWater( ) )
					pSoldier->bCamo = __max( 0, pSoldier->bCamo - 2 );
				else
					pSoldier->bCamo = __max( 0, pSoldier->bCamo - 1 );
			}

			if ( pSoldier->bCamo <= 0 )
			{
				pSoldier->bCamo = 0;
				camoWoreOff = TRUE;
			}
		}
		if ( pSoldier->urbanCamo > 0 )
		{
			if ( HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) )
			{
				if ( pSoldier->MercInDeepWater( ) )
				{
					pSoldier->urbanCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
					pSoldier->urbanCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
				else
				{
					pSoldier->urbanCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
			}
			else
			{
				if ( pSoldier->MercInDeepWater( ) )
					pSoldier->urbanCamo = __max( 0, pSoldier->urbanCamo - 2);
				else
					pSoldier->urbanCamo = __max( 0, pSoldier->urbanCamo - 1);
			}

			if ( pSoldier->urbanCamo <= 0 )
			{
				pSoldier->urbanCamo = 0;
				camoWoreOff = TRUE;
			}
		}
		if ( pSoldier->desertCamo > 0 )
		{
			if ( HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) )
			{
				if ( pSoldier->MercInDeepWater( ) )
				{
					pSoldier->desertCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
					pSoldier->desertCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
				else
				{
					pSoldier->desertCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
			}
			else
			{
				if ( pSoldier->MercInDeepWater( ) )
					pSoldier->desertCamo = __max( 0, pSoldier->desertCamo - 2);
				else
					pSoldier->desertCamo = __max( 0, pSoldier->desertCamo - 1);
			}

			if ( pSoldier->desertCamo <= 0 )
			{
				pSoldier->desertCamo = 0;
				camoWoreOff = TRUE;
			}
		}
		if ( pSoldier->snowCamo > 0 )
		{
			if ( HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) )
			{
				if ( pSoldier->MercInDeepWater( ) )
				{
					pSoldier->snowCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
					pSoldier->snowCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
				else
				{
					pSoldier->snowCamo -= (Chance(__max(0, 100 - gSkillTraitValues.ubRACamoWornountSpeedReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) ? 1 : 0 );
				}
			}
			else
			{
				if ( pSoldier->MercInDeepWater( ) )
					pSoldier->snowCamo = __max( 0, pSoldier->snowCamo - 2);
				else
					pSoldier->snowCamo = __max( 0, pSoldier->snowCamo - 1);
			}

			if ( pSoldier->snowCamo <= 0 )
			{
				pSoldier->snowCamo = 0;
				camoWoreOff = TRUE;
			}
		}
	}
	else if ( !HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_OT ) ) // Old Camouflaged trait
	{
		// reduce camouflage by 2% per tile of deep water
		// and 1% for medium water
		if ( pSoldier->bCamo > 0 )
		{
			if ( pSoldier->MercInDeepWater( ) )
				pSoldier->bCamo = __max( 0, pSoldier->bCamo - 2 );
			else
				pSoldier->bCamo = __max( 0, pSoldier->bCamo - 1 );

			if ( (pSoldier->bCamo)== 0)
				camoWoreOff = TRUE;
		}
		if ( pSoldier->urbanCamo > 0 )
		{
			if ( pSoldier->MercInDeepWater( ) )
				pSoldier->urbanCamo = __max( 0, pSoldier->urbanCamo - 2);
			else
				pSoldier->urbanCamo = __max( 0, pSoldier->urbanCamo - 1);
			
			if ( (pSoldier->urbanCamo)== 0)
				camoWoreOff = TRUE;
		}
		if ( pSoldier->desertCamo > 0 )
		{
			if ( pSoldier->MercInDeepWater( ) )
				pSoldier->desertCamo = __max( 0, pSoldier->desertCamo - 2);
			else
				pSoldier->desertCamo = __max( 0, pSoldier->desertCamo - 1);
			
			if ( (pSoldier->desertCamo)== 0)
				camoWoreOff = TRUE;
		}
		if ( pSoldier->snowCamo > 0 )
		{
			if ( pSoldier->MercInDeepWater( ) )
				pSoldier->snowCamo = __max( 0, pSoldier->snowCamo - 2);
			else
				pSoldier->snowCamo = __max( 0, pSoldier->snowCamo - 1);
			
			if ( (pSoldier->snowCamo)== 0)
				camoWoreOff = TRUE;
		}	
	}
	/////////////////////////////////////////////////////////////////////////////////////

	if ( camoWoreOff )
	{
		// Reload palettes....
		if ( pSoldier->bInSector )
		{
			pSoldier->CreateSoldierPalettes( );
		}
	//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_CAMMO_WASHED_OFF], pSoldier->name );
	
			if ( pSoldier->bCamo <= 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_JUNGLE_WASHED_OFF], pSoldier->name );
			}
			else if ( pSoldier->urbanCamo <= 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_URBAN_WASHED_OFF], pSoldier->name );
			}
			else if ( pSoldier->snowCamo <= 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_SNOW_WASHED_OFF], pSoldier->name );
			}
			else if ( pSoldier->desertCamo <= 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_DESERT_WASHED_OFF], pSoldier->name );
			}
	}






	if ( pSoldier->bTeam == gbPlayerNum && pSoldier->aiData.bMonsterSmell > 0 )
	{
		if ( pSoldier->MercInDeepWater( ) )
		{
			bDieSize = 10;
		}
		else
		{
			bDieSize = 20;
		}
		if ( Random( bDieSize ) == 0 )
		{
			pSoldier->aiData.bMonsterSmell--;
		}
	}

	DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
}

BOOLEAN ApplyCammo( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs )
{
	// Added - SANDRO
	INT8		bPointsToUse;
	UINT16	usTotalKitPoints;
	UINT16 iRemainingCamoAfterRemoving; 

	(*pfGoodAPs) = TRUE;

	if (pObj->exists() == false)
		return( FALSE );

	//////////////////////////////////////////////////////////////////////////////
	// added possibility to remove all camo by using a rag on self - SANDRO
	if (pObj->usItem == 1022 && gGameExternalOptions.fCamoRemoving)
	{
		if (!EnoughPoints( pSoldier, (APBPConstants[AP_CAMOFLAGE]/2), 0, TRUE ) )
		{
			(*pfGoodAPs) = FALSE;
			return( TRUE );
		}
		// 100 should be enough. The third value "0" means we will remove all types of camo
		ReduceCamoFromSoldier( pSoldier, 100, 0 );

		// damage the rag :) - actually you would need to flag it damagable in the items.XML
		DamageItem( pObj, 22, FALSE );

		DeductPoints( pSoldier, (APBPConstants[AP_CAMOFLAGE] / 2), 0 );

		// Reload palettes....
		if ( pSoldier->bInSector )
		{
			pSoldier->CreateSoldierPalettes( );
		}
		return( TRUE );
	}
	else if ( !Item[pObj->usItem].camouflagekit )
	{
		return( FALSE );
	}
	//////////////////////////////////////////////////////////////////////////////

	if (!EnoughPoints( pSoldier, APBPConstants[AP_CAMOFLAGE], 0, TRUE ) )
	{
    (*pfGoodAPs) = FALSE;
		return( TRUE );
	}

	usTotalKitPoints = TotalPoints( pObj );
	if (usTotalKitPoints == 0)
	{
		// HUH???
		return( FALSE );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Added a feature to determine which type of camo we use								//
	// and procedure able to reduce different type of camo											//
	// ******************************************************************************************** //

	if (gGameExternalOptions.fCamoRemoving)
	{
		int iJungleCamoTotal = pSoldier->bCamo + pSoldier->wornCamo;
		int iUrbanCamoTotal = pSoldier->urbanCamo + pSoldier->wornUrbanCamo;
		int iDesertCamoTotal = pSoldier->desertCamo + pSoldier->wornDesertCamo;
		int iSnowCamoTotal = pSoldier->snowCamo + pSoldier->wornSnowCamo;

		int totalCamo = iJungleCamoTotal + iUrbanCamoTotal + iDesertCamoTotal + iSnowCamoTotal;

		// First, check if we have an item with major JUNGLE camobonus
		if ( (Item[pObj->usItem].camobonus > Item[pObj->usItem].urbanCamobonus) && 
			 (Item[pObj->usItem].camobonus > Item[pObj->usItem].desertCamobonus) &&
			 (Item[pObj->usItem].camobonus > Item[pObj->usItem].snowCamobonus) )
		{
			if (iJungleCamoTotal >= 100)
				return( FALSE );

			// determine how much we can add
			bPointsToUse = (100 - (iJungleCamoTotal) + 1 ) / 2;
			bPointsToUse = __min( bPointsToUse, usTotalKitPoints );

			// determine how much we will add
			int iJungleCamoAdded = (int)(Item[pObj->usItem].camobonus * bPointsToUse * 2 / 100 );

			// if we have already too much different camo on ourselves, reduce some or all
			if ( (totalCamo + iJungleCamoAdded) > 100 )
			{
				// first, determine how much we actually want to remove
				int iCamoToRemove = (totalCamo + iJungleCamoAdded) - 100;

				// reduce camo.. IMPORTANT - third value is type of camo to skip (0=skip-none, 1=jungle, 2=urban, 3=desert,4=snow)
				iRemainingCamoAfterRemoving = ReduceCamoFromSoldier( pSoldier, iCamoToRemove, 1 ); // "1" - we want to keep the JUNGLE camo

				// if some camo haven't been reduced from us, the camo to add will be lessened by that value
				if (iRemainingCamoAfterRemoving > 0)
					iJungleCamoAdded = max(0, (iJungleCamoAdded - iRemainingCamoAfterRemoving));

				if (iJungleCamoAdded <= 0) // if we have nothing to add now, return
					return( FALSE );
				else // otherwise apply the camo
				{
					pSoldier->bCamo = __min( 100, pSoldier->bCamo + iJungleCamoAdded );
				}
			}
			else // everything's fine, apply!!
			{
				pSoldier->bCamo = __min( 100, pSoldier->bCamo + iJungleCamoAdded );
			}
		}
		// Second, check if we have an item with major URBAN camobonus
		else if ( (Item[pObj->usItem].urbanCamobonus > Item[pObj->usItem].camobonus) && 
			 (Item[pObj->usItem].urbanCamobonus > Item[pObj->usItem].desertCamobonus) &&
			 (Item[pObj->usItem].urbanCamobonus > Item[pObj->usItem].snowCamobonus) )
		{
			if (iUrbanCamoTotal >= 100)
				return( FALSE );

			// determine how much we can add
			bPointsToUse = (100 - (iUrbanCamoTotal) + 1 ) / 2;
			bPointsToUse = __min( bPointsToUse, usTotalKitPoints );

			// determine how much we will add
			int iUrbanCamoAdded = (int)(Item[pObj->usItem].urbanCamobonus * bPointsToUse * 2 / 100 );

			// if we have already too much different camo on ourselves, reduce some or all
			if ( (totalCamo + iUrbanCamoAdded) > 100 )
			{
				// first, determine how much we actually want to remove
				int iCamoToRemove = (totalCamo + iUrbanCamoAdded) - 100;

				// reduce camo.. IMPORTANT - third value is type of camo to skip (0=skip-none, 1=jungle, 2=urban, 3=desert,4=snow)
				iRemainingCamoAfterRemoving = ReduceCamoFromSoldier( pSoldier, iCamoToRemove, 2 ); // "2" - we want to keep the URBAN camo

				// if some camo haven't been reduced from us, the camo to add will be lessened by that value
				if (iRemainingCamoAfterRemoving > 0)
					iUrbanCamoAdded = max(0, (iUrbanCamoAdded - iRemainingCamoAfterRemoving));

				if (iUrbanCamoAdded <= 0) // if we have nothing to add now, return
					return( FALSE );
				else // otherwise apply the camo
				{
					pSoldier->urbanCamo = __min( 100, pSoldier->urbanCamo + iUrbanCamoAdded );
				}
			}
			else // everything's fine, apply!!
			{
				pSoldier->urbanCamo = __min( 100, pSoldier->urbanCamo + iUrbanCamoAdded );
			}
		}
		// Third, check if we have an item with major DESERT camobonus
		else if ( (Item[pObj->usItem].desertCamobonus > Item[pObj->usItem].camobonus) && 
			 (Item[pObj->usItem].desertCamobonus > Item[pObj->usItem].urbanCamobonus) &&
			 (Item[pObj->usItem].desertCamobonus > Item[pObj->usItem].snowCamobonus) )
		{
			if (iDesertCamoTotal >= 100)
				return( FALSE );

			// determine how much we can add
			bPointsToUse = (100 - (iDesertCamoTotal) + 1 ) / 2;
			bPointsToUse = __min( bPointsToUse, usTotalKitPoints );

			// determine how much we will add
			int iDesertCamoAdded = (int)(Item[pObj->usItem].desertCamobonus * bPointsToUse * 2 / 100 );

			// if we have already too much different camo on ourselves, reduce some or all
			if ( (totalCamo + iDesertCamoAdded) > 100 )
			{
				// first, determine how much we actually want to remove
				int iCamoToRemove = (totalCamo + iDesertCamoAdded) - 100;

				// reduce camo.. IMPORTANT - third value is type of camo to skip (0=skip-none, 1=jungle, 2=urban, 3=desert,4=snow)
				iRemainingCamoAfterRemoving = ReduceCamoFromSoldier( pSoldier, iCamoToRemove, 3 ); // "3" - we want to keep the DESERT camo

				// if some camo haven't been reduced from us, the camo to add will be lessened by that value
				if (iRemainingCamoAfterRemoving > 0)
					iDesertCamoAdded = max(0, (iDesertCamoAdded - iRemainingCamoAfterRemoving));

				if (iDesertCamoAdded <= 0) // if we have nothing to add now, return
					return( FALSE );
				else // otherwise apply the camo
				{
					pSoldier->desertCamo = __min( 100, pSoldier->desertCamo + iDesertCamoAdded );
				}
			}
			else // everything's fine, apply!!
			{
				pSoldier->desertCamo = __min( 100, pSoldier->desertCamo + iDesertCamoAdded );
			}
		}
		// Fourth, check if we have an item with major SNOW camobonus
		else if ( (Item[pObj->usItem].snowCamobonus > Item[pObj->usItem].camobonus) && 
			 (Item[pObj->usItem].snowCamobonus > Item[pObj->usItem].urbanCamobonus) &&
			 (Item[pObj->usItem].snowCamobonus > Item[pObj->usItem].desertCamobonus) )
		{
			if (iSnowCamoTotal >= 100)
				return( FALSE );

			// determine how much we can add
			bPointsToUse = (100 - (iSnowCamoTotal) + 1 ) / 2;
			bPointsToUse = __min( bPointsToUse, usTotalKitPoints );

			// determine how much we will add
			int iSnowCamoAdded = (int)(Item[pObj->usItem].snowCamobonus * bPointsToUse * 2 / 100 );

			// if we have already too much different camo on ourselves, reduce some or all
			if ( (totalCamo + iSnowCamoAdded) > 100 )
			{
				// first, determine how much we actually want to remove
				int iCamoToRemove = (totalCamo + iSnowCamoAdded) - 100;

				// reduce camo.. IMPORTANT - third value is type of camo to skip (0=skip-none, 1=jungle, 2=urban, 3=desert,4=snow)
				iRemainingCamoAfterRemoving = ReduceCamoFromSoldier( pSoldier, iCamoToRemove, 4 ); // "4" - we want to keep the SNOW camo

				// if some camo haven't been reduced from us, the camo to add will be lessened by that value
				if (iRemainingCamoAfterRemoving > 0)
					iSnowCamoAdded = max(0, (iSnowCamoAdded - iRemainingCamoAfterRemoving));

				if (iSnowCamoAdded <= 0) // if we have nothing to add now, return
					return( FALSE );
				else // otherwise apply the camo
				{
					pSoldier->snowCamo = __min( 100, pSoldier->snowCamo + iSnowCamoAdded );
				}
			}
			else // everything's fine, apply!!
			{
				pSoldier->snowCamo = __min( 100, pSoldier->snowCamo + iSnowCamoAdded );
			}
		}
		else // the item has no major camo, return
			return( FALSE );
	// ****************************************************************************************** //
	////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		//get total camo bonus for kit -- note that camo kits now require the camobonus tag to be set
		//int itemCamo = Item[pObj->usItem].camobonus + Item[pObj->usItem].urbanCamobonus + Item[pObj->usItem].desertCamobonus + Item[pObj->usItem].snowCamobonus;

		int totalCamo = pSoldier->bCamo + pSoldier->wornCamo + pSoldier->urbanCamo+pSoldier->wornUrbanCamo+pSoldier->desertCamo+pSoldier->wornDesertCamo+pSoldier->snowCamo+pSoldier->wornSnowCamo;
		if ((totalCamo) >= 100)
		{
			// nothing more to add
			return( FALSE );
		}

		// points are used up at a rate of 50% kit = 100% cammo on guy
		// add 1 to round off
		bPointsToUse = (100 - (totalCamo) + 1 ) / 2;
		bPointsToUse = __min( bPointsToUse, usTotalKitPoints );

		//figure out proportions of each to be applied, one item can theoretically have more than one camouflage type this way
		int urban = (int)(Item[pObj->usItem].urbanCamobonus * bPointsToUse * 2 / 100 );
		int jungle = (int)(Item[pObj->usItem].camobonus * bPointsToUse * 2 / 100 );
		int desert = (int)(Item[pObj->usItem].desertCamobonus * bPointsToUse * 2 / 100 );
		int snow = (int)(Item[pObj->usItem].snowCamobonus * bPointsToUse * 2 / 100 );

		pSoldier->bCamo = __min( 100, pSoldier->bCamo + jungle );
		pSoldier->urbanCamo = __min( 100, pSoldier->urbanCamo + urban );
		pSoldier->desertCamo = __min( 100, pSoldier->desertCamo + desert );
		pSoldier->snowCamo = __min( 100, pSoldier->snowCamo + snow );
	}

	UseKitPoints( pObj, bPointsToUse, pSoldier );

	DeductPoints( pSoldier, APBPConstants[AP_CAMOFLAGE], 0 );

	// Reload palettes....
	if ( pSoldier->bInSector )
	{
		pSoldier->CreateSoldierPalettes( );
	}

	return( TRUE );
}

BOOLEAN ApplyCanteen( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs )
{
	INT16		sPointsToUse;
	UINT16	usTotalKitPoints;

	(*pfGoodAPs) = TRUE;

	if (!Item[pObj->usItem].canteen || pObj->exists() == false)
	{
		return( FALSE );
	}

	usTotalKitPoints = TotalPoints( pObj );
	if (usTotalKitPoints == 0)
	{
		// HUH???
		return( FALSE );
	}

	if (!EnoughPoints( pSoldier, APBPConstants[AP_DRINK], 0, TRUE ) )
	{
    (*pfGoodAPs) = FALSE;
		return( TRUE );
	}

  if ( pSoldier->bTeam == gbPlayerNum )
  {
    if ( gMercProfiles[ pSoldier->ubProfile ].bSex == MALE )
    {
		  PlayJA2Sample( DRINK_CANTEEN_MALE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
    }
    else
    {
		  PlayJA2Sample( DRINK_CANTEEN_FEMALE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
    }
  }

	sPointsToUse = __min( 20, usTotalKitPoints );

	// CJC Feb 9.  Canteens don't seem effective enough, so doubled return from them
	DeductPoints( pSoldier, APBPConstants[AP_DRINK], (INT16) (2 * sPointsToUse * -(100 - pSoldier->bBreath) ) );

	UseKitPoints( pObj, sPointsToUse, pSoldier );

	return( TRUE );
}

#define MAX_HUMAN_CREATURE_SMELL (NORMAL_HUMAN_SMELL_STRENGTH - 1)

BOOLEAN ApplyElixir( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs )
{
	INT16		sPointsToUse;
	UINT16	usTotalKitPoints;

	(*pfGoodAPs) = TRUE;

	if (pObj->usItem != JAR_ELIXIR || pObj->exists() == false)
	{
		return( FALSE );
	}

	usTotalKitPoints = TotalPoints( pObj );
	if (usTotalKitPoints == 0)
	{
		// HUH???
		return( FALSE );
	}

	if (!EnoughPoints( pSoldier, APBPConstants[AP_CAMOFLAGE], 0, TRUE ) )
	{
    (*pfGoodAPs) = FALSE;
		return( TRUE );
	}

	DeductPoints( pSoldier, APBPConstants[AP_CAMOFLAGE], 0 );

	sPointsToUse = ( MAX_HUMAN_CREATURE_SMELL - pSoldier->aiData.bMonsterSmell ) * 2;
	sPointsToUse = __min( sPointsToUse, usTotalKitPoints );

	UseKitPoints( pObj, sPointsToUse, pSoldier );

	pSoldier->aiData.bMonsterSmell += sPointsToUse / 2;

	return( TRUE );
}

UINT32 ConvertProfileMoneyValueToObjectTypeMoneyValue( UINT8 ubStatus )
{
	return( ubStatus * 50 );
}

UINT8 ConvertObjectTypeMoneyValueToProfileMoneyValue( UINT32 uiMoneyAmount )
{
	return( (UINT8)( uiMoneyAmount / 50 ) );
}

BOOLEAN ItemIsCool( OBJECTTYPE * pObj )
{
	if (pObj->exists() == false) {
		return FALSE;
	}
	if ((*pObj)[0]->data.objectStatus < 60)
	{
		return( FALSE );
	}
	if ( Item[ pObj->usItem ].usItemClass & IC_WEAPON )
	{
		if ( Weapon[ pObj->usItem ].ubDeadliness >= 30 )
		{
			return( TRUE );
		}
	}
	else if ( Item[ pObj->usItem ].usItemClass & IC_ARMOUR )
	{
		if ( Armour[ Item[ pObj->usItem ].ubClassIndex ].ubProtection >= 20 )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

void ActivateXRayDevice( SOLDIERTYPE * pSoldier )
{
	SOLDIERTYPE *	pSoldier2;
	UINT32				uiSlot;

	if ( Item[pSoldier->inv[ HANDPOS ].usItem].needsbatteries && pSoldier->inv[ HANDPOS ].exists() == true)
	{
		// check for batteries
		OBJECTTYPE* pBatteries = FindAttachedBatteries( &(pSoldier->inv[HANDPOS]) );
		if ( pBatteries == 0 )
		{
			// doesn't work without batteries!
			return;
		}

		// use up 8-12 percent of batteries
		if ( Item[pBatteries->usItem].percentstatusdrainreduction > 0 )
			(*pBatteries)[0]->data.objectStatus -= (INT8)( (8 + Random( 5 )) * (100 - Item[(*pBatteries)[0]->data.objectStatus].percentstatusdrainreduction)/100 );
		else
			(*pBatteries)[0]->data.objectStatus -= (INT8)( (8 + Random( 5 )) );
		if ( (*pBatteries)[0]->data.objectStatus <= 0 )
		{
			// destroy batteries
			pBatteries->RemoveObjectsFromStack(1);
			if (pBatteries->exists() == false) {
				pSoldier->inv[HANDPOS].RemoveAttachment(pBatteries);
			}
		}
	}
	// first, scan through all mercs and turn off xrayed flag for anyone
	// previously xrayed by this guy
	for ( uiSlot = 0; uiSlot < guiNumMercSlots; uiSlot++ )
	{
		pSoldier2 = MercSlots[ uiSlot ];
		if ( pSoldier2 )
		{
			if ( (pSoldier2->ubMiscSoldierFlags & SOLDIER_MISC_XRAYED) && (pSoldier2->aiData.ubXRayedBy == pSoldier->ubID) )
			{
				pSoldier2->ubMiscSoldierFlags &= (~SOLDIER_MISC_XRAYED);
				pSoldier2->aiData.ubXRayedBy = NOBODY;
			}
		}
	}
	// now turn on xray for anyone within range
	for ( uiSlot = 0; uiSlot < guiNumMercSlots; uiSlot++ )
	{
		pSoldier2 = MercSlots[ uiSlot ];
		if ( pSoldier2 )
		{
			if ( pSoldier2->bTeam != pSoldier->bTeam && PythSpacesAway( pSoldier->sGridNo, pSoldier2->sGridNo ) < XRAY_RANGE )
			{
				pSoldier2->ubMiscSoldierFlags |= SOLDIER_MISC_XRAYED;
				pSoldier2->aiData.ubXRayedBy = pSoldier->ubID;
			}
		}
	}
	pSoldier->uiXRayActivatedTime = GetWorldTotalSeconds();
}

void TurnOffXRayEffects( SOLDIERTYPE * pSoldier )
{
	SOLDIERTYPE *	pSoldier2;
	UINT32				uiSlot;

	if ( !pSoldier->uiXRayActivatedTime )
	{
		return;
	}

	// scan through all mercs and turn off xrayed flag for anyone
	// xrayed by this guy
	for ( uiSlot = 0; uiSlot < guiNumMercSlots; uiSlot++ )
	{
		pSoldier2 = MercSlots[ uiSlot ];
		if ( pSoldier2 )
		{
			if ( (pSoldier2->ubMiscSoldierFlags & SOLDIER_MISC_XRAYED) && (pSoldier2->aiData.ubXRayedBy == pSoldier->ubID) )
			{
				pSoldier2->ubMiscSoldierFlags &= (~SOLDIER_MISC_XRAYED);
				pSoldier2->aiData.ubXRayedBy = NOBODY;
			}
		}
	}
	pSoldier->uiXRayActivatedTime = 0;
}



#ifdef JA2TESTVERSION
void DumpItemsList( void )
{
  CHAR8 zPrintFileName[60];
  FILE *FDump;
	UINT16 usItem;
	INVTYPE *pItem;

  // open output file
 	strcpy(zPrintFileName, "ItemDump.txt");
  FDump = fopen(zPrintFileName, "wt");

  if (FDump == NULL)
    return;

	// print headings
	fprintf(FDump, "            ITEM              COOLNESS  VALUE\n");
	fprintf(FDump, "============================  ========  =====\n");

	for( usItem = 0; usItem < MAXITEMS; usItem++ )
	{
		pItem= &( Item[ usItem ] );

		if (pItem->ubCoolness > 0 )
		{
			fprintf(FDump, "%28ls     %2d     $%4d\n", ItemNames[ usItem ], pItem->ubCoolness, pItem->usPrice );
		}
	}

  fclose(FDump);
}
#endif // JA2TESTVERSION


// Snap: status modifiers for various item bonuses:

// In JA Gold status above this limit does not affect item performance (for some items)
const INT8 STANDARD_STATUS_CUTOFF = 85;

// Scale bonus with item status
INT16 BonusReduce( INT16 bonus, INT16 status, INT16 statusCutoff = STANDARD_STATUS_CUTOFF )
{
	if ( bonus > 0 && status < statusCutoff && statusCutoff > 0 && statusCutoff <= 100 )
		return ( ( status * 100 ) / statusCutoff * bonus ) / 100;
	else // A penalty can't be reduced by status!
		return bonus;
}

// Scale bonus with item status. Status < 50% creates a penalty!
INT16 BonusReduceMore( INT16 bonus, INT16 status, INT16 statusCutoff = 100 )
{
	if ( bonus > 0 && status < statusCutoff && statusCutoff > 0 && statusCutoff <= 100 )
		return ( ( ( status * 100 ) / statusCutoff - 50 ) * bonus ) / 50;
	else // A penalty can't be reduced by status!
		return bonus;
}

// Some items either work or they don't...
INT16 BonusOnOff( INT16 bonus, INT16 status )
{
	if ( bonus > 0 )
		return (status >= 50) ? bonus : 0;
	else // A penalty can't be reduced by status!
		return bonus;
}

// HEADROCK HAM 4: Scopes now determined not by an aim bonus but by a magnification factor.
BOOLEAN NCTHIsScoped( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true && UsingNewCTHSystem() == true) {
		if ( Item[pObj->usItem].scopemagfactor > 1.0 || Item[(*pObj)[0]->data.gun.usGunAmmoItem].scopemagfactor > 1.0 )
			return TRUE;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if ( Item[iter->usItem].scopemagfactor > 1.0 && iter->exists() )
				return TRUE;
		}
	}

	return FALSE;
}

// Snap: a fast aimbonus check for AI
BOOLEAN IsScoped( OBJECTTYPE * pObj )
{
	if(UsingNewCTHSystem() == true)
		return NCTHIsScoped(pObj);

	if (pObj->exists() == true) {
		if ( Item[pObj->usItem].aimbonus > 0 || Item[(*pObj)[0]->data.gun.usGunAmmoItem].aimbonus > 0 )
			return TRUE;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if ( Item[iter->usItem].aimbonus > 0  && iter->exists())
				return TRUE;
		}
	}

	return FALSE;
}

// Snap: get aim bonus for a single item
INT16 GetItemAimBonus( const INVTYPE* pItem, INT32 iRange, INT16 ubAimTime )
{
	if ( iRange <= pItem->minrangeforaimbonus )	return 0;

	// reduce effective sight range by aimbonus% per extra aiming time AP
	// of the distance beyond minrangeforaimbonus.
	if ( pItem->aimbonus < 0)
		ubAimTime = 1;

	//CHRISL: The old system basically allowed scopes to reduce sight range by AimBonus%/click.  So a scope with an
	//	AimBonus 20, and 8 clicks, would reduce sight range by 160%.  In other words, it would effectively reduce sight
	//	range to 0 just for using 8 APs to aim.
	//return ( pItem->aimbonus * ubAimTime	* ( iRange - pItem->minrangeforaimbonus ) ) / 100;
	//	Instead, let's have the bonus reduce each click seperately and base the percentage on the new range.
	INT16 bonus = 0, stepBonus = 0;
	for( UINT8 step = 0; step < ubAimTime; step++)
	{
		stepBonus = (pItem->aimbonus * iRange)/100;
		iRange -= stepBonus;
		bonus += stepBonus;
		if(iRange < pItem->minrangeforaimbonus)
		{
			stepBonus = pItem->minrangeforaimbonus - iRange;
			bonus -= stepBonus;
			break;
		}
	}
	return(bonus);
}

INT16 GetAimBonus( OBJECTTYPE * pObj, INT32 iRange, INT16 ubAimTime )
{
	INT16 bonus = 0;

	if (pObj->exists() == true) {
		bonus = BonusReduceMore( GetItemAimBonus( &Item[pObj->usItem], iRange, ubAimTime ), (*pObj)[0]->data.objectStatus );
		bonus += GetItemAimBonus( &Item[(*pObj)[0]->data.gun.usGunAmmoItem], iRange, ubAimTime );

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduceMore( GetItemAimBonus( &Item[iter->usItem], iRange, ubAimTime ), (*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}

//WarmSteel - This is used to determine the base aim bonus of the scope on this gun.
//It searched for the most powerful scope that is not targetting under it's minimum range.
INT16 GetBaseScopeAimBonus( OBJECTTYPE * pObj, INT32 iRange )
{
	INT16 bonus = 0;

	//Valid integrated scope?
	if(Item[pObj->usItem].aimbonus > bonus && iRange >= Item[pObj->usItem].minrangeforaimbonus ){
		bonus = Item[pObj->usItem].aimbonus;
	}

	//Search for the most powerful scope we can use.
	for(attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); iter++)
	{
		if(Item[iter->usItem].aimbonus > bonus && iRange >= Item[iter->usItem].minrangeforaimbonus && iter->exists())
		{
			bonus = Item[iter->usItem].aimbonus;
		}
	}
	return( bonus );
}

// Madd: check equipment for aim bonus (penalties)
INT16 GetGearAimBonus( SOLDIERTYPE * pSoldier, INT32 iRange, INT16 ubAimTime  )
{
	INT16 bonus=0;

	for (int j = HELMETPOS; j <= HEAD2POS; j++)
	{
		OBJECTTYPE* pObj = &pSoldier->inv[j];
		if (pObj->exists() == true) {
			bonus += GetItemAimBonus( &Item[pSoldier->inv[j].usItem], iRange, ubAimTime );
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					bonus += GetItemAimBonus(&Item[iter->usItem],iRange,ubAimTime);
				}
			}
		}
	}

	return( bonus );
}

// Madd: check equipment for to hit bonus (penalties)
INT16 GetGearToHitBonus( SOLDIERTYPE * pSoldier )
{
	INT16 bonus=0;

	for (int j = HELMETPOS; j <= HEAD2POS; j++)
	{
		OBJECTTYPE* pObj = &pSoldier->inv[j];
		if (pObj->exists() == true) {
			bonus += Item[pSoldier->inv[j].usItem].tohitbonus;
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					bonus += Item[iter->usItem].tohitbonus;
				}
			}
		}
	}

	return( bonus );
}

// Madd: check equipment for AP bonus (penalties)
INT16 GetGearAPBonus( SOLDIERTYPE * pSoldier )
{
	INT16 bonus=0;

	for (int j = HELMETPOS; j <= HEAD2POS; j++)
	{
		if (pSoldier->inv[j].exists() == true) {
			bonus += Item[pSoldier->inv[j].usItem].APBonus;
			OBJECTTYPE* pObj = &pSoldier->inv[j];
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					bonus += Item[iter->usItem].APBonus;
				}
			}
		}
	}

	return( bonus );
}

UINT32 FindRangeBonusAttachment( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].rangebonus > 0  && iter->exists())
			{
				return( Item[iter->usItem].uiIndex );
			}
		}
	}
	return( NONE );
}

INT16 GetRangeBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		if(Item[pObj->usItem].usItemClass == IC_AMMO)
			return( Item[pObj->usItem].rangebonus );
		bonus = BonusReduce( Item[pObj->usItem].rangebonus, (*pObj)[0]->data.objectStatus );

		if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
			bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].rangebonus;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if ( !Item[iter->usItem].duckbill || ( Item[iter->usItem].duckbill && (*pObj)[0]->data.gun.ubGunAmmoType == AMMO_BUCKSHOT ) && iter->exists())
				bonus += BonusReduce( Item[iter->usItem].rangebonus, (*iter)[0]->data.objectStatus );
		}
	}
	return( bonus );
}


INT16 LaserBonus( const INVTYPE * pItem, INT32 iRange, UINT8 bLightLevel )
{
	// Snap: Reduce laser scope bonus at long ranges and high light levels

	if ( pItem->bestlaserrange == 0 || iRange <= pItem->bestlaserrange ) {
		// No penalty within this range
		return pItem->tohitbonus;
	}
	else {
		// Figure out max. visible distance for the laser dot:
		// day: 1.5*bestlaserrange, night: 2.5*bestlaserrange
		// iMaxLaserRange = bestlaserrange * ( 1.5 + ( bLightLevel - NORMAL_LIGHTLEVEL_DAY )
		//                                 / ( NORMAL_LIGHTLEVEL_NIGHT - NORMAL_LIGHTLEVEL_DAY ) )
		INT32 iMaxLaserRange = ( pItem->bestlaserrange*( 2*bLightLevel + 3*NORMAL_LIGHTLEVEL_NIGHT - 5*NORMAL_LIGHTLEVEL_DAY ) )
		                     / ( 2 * ( NORMAL_LIGHTLEVEL_NIGHT - NORMAL_LIGHTLEVEL_DAY ) );

		// Beyond bestlaserrange laser bonus drops linearly to 0
		INT16 bonus = ( pItem->tohitbonus * (iMaxLaserRange - iRange) )
		            / ( iMaxLaserRange - pItem->bestlaserrange );

		return (bonus > 0 ? bonus : 0);
	}
}

INT16 GetToHitBonus( OBJECTTYPE * pObj, INT32 iRange, UINT8 bLightLevel, BOOLEAN fProneStance )
{
	INT16 bonus=0;

	// Snap: bipod is effective only in the prone stance

	if (pObj->exists() == true) {
		if ( fProneStance )
			bonus += Item[pObj->usItem].bipod;

		bonus += BonusReduceMore( LaserBonus( &Item[pObj->usItem], iRange, bLightLevel), (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].tohitbonus;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				if ( fProneStance )
					bonus += Item[iter->usItem].bipod;

				bonus += BonusReduceMore( LaserBonus( &Item[iter->usItem], iRange, bLightLevel), (*iter)[0]->data.objectStatus );
			}
		}
	}

	// Snap (TODO): add special treatment of laser scopes
	return( bonus );
}

// HEADROCK HAM 4: The following functions return the value of new NCTH-related modifiers from an item and all its
// attachments. They are stance-based, meaning that the soldier's stance determines which modifier is referenced.
// For a "default" value, feed the function a value of ubStance=ANIM_STAND.

INT32 GetFlatBaseModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].flatbasemodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].flatbasemodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	return (iModifier);
}

INT32 GetPercentBaseModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].percentbasemodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].percentbasemodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return (iModifier);
}

INT32 GetFlatAimModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].flataimmodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].flataimmodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	return (iModifier);
}

INT32 GetPercentAimModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].percentaimmodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].percentaimmodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return (iModifier);
}

INT32 GetPercentCapModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].percentcapmodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].percentcapmodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return (iModifier);
}

INT32 GetPercentHandlingModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].percenthandlingmodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].percenthandlingmodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return ((UINT32)iModifier);
}

INT32 GetDropCompensationModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].percentdropcompensationmodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].percentdropcompensationmodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return (iModifier);
}

// HEADROCK HAM 4: This function returns the Max Counter Force modifier given by the weapon
INT32 GetCounterForceMaxModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].maxcounterforcemodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].maxcounterforcemodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}

	if(ubRef == 1)
		iModifier += (INT32)gGameCTHConstants.RECOIL_MAX_COUNTER_CROUCH;
	else if (ubRef == 2)
		iModifier += (INT32)gGameCTHConstants.RECOIL_MAX_COUNTER_PRONE;

	iModifier = __max(-100,iModifier);
	return (iModifier);
}

// HEADROCK HAM 4: This function returns the Counter Force Accuracy modifier given by the weapon
INT32 GetCounterForceAccuracyModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].counterforceaccuracymodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].counterforceaccuracymodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return (iModifier);
}

INT32 GetCounterForceFrequencyModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].counterforcefrequencymodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].counterforcefrequencymodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	return (iModifier);
}

INT32 GetTargetTrackingModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );
		
	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].targettrackingmodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) 
		{
			if (iter->exists())
			{
				iModifier += BonusReduceMore( Item[iter->usItem].targettrackingmodifier[ubRef], (*iter)[0]->data.objectStatus );
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return (iModifier);
}

INT32 GetAimLevelsModifier( OBJECTTYPE *pObj, UINT8 ubStance )
{
	INT32 iModifier=0;

	UINT8 ubRef = GetStanceModifierRef( ubStance );
		
	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		iModifier += Item[pObj->usItem].aimlevelsmodifier[ubRef];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				iModifier += Item[iter->usItem].aimlevelsmodifier[ubRef];
			}
		}
	}
	iModifier = __max(-100,iModifier);
	return (iModifier);
}

INT32 GetAimLevelsTraitModifier( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj )
{
	INT8 ubSkillModifier = 0;

	if( gGameOptions.fNewTraitSystem )
	{
		if ( Weapon[Item[pObj->usItem].ubClassIndex].ubWeaponType == GUN_PISTOL || Weapon[Item[pObj->usItem].ubClassIndex].ubWeaponType == GUN_M_PISTOL )
			ubSkillModifier -= NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT );
		else
			ubSkillModifier -= NUM_SKILL_TRAITS( pSoldier, SNIPER_NT );
	}
	else
	{
		ubSkillModifier -= NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT );
	}

	return (INT32)ubSkillModifier;

}

INT16 GetBurstToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance )
{
	INT16 bonus=0;

	// Snap: bipod is effective only in the prone stance
	// CHRISL: We don't want to count both bipod AND bursttohitbonus as some items get both bonuses

	if (pObj->exists() == true) {
		if ( fProneStance )
			bonus += BonusReduceMore( Item[pObj->usItem].bipod, (*pObj)[0]->data.objectStatus );
		else
			bonus += BonusReduceMore( Item[pObj->usItem].bursttohitbonus, (*pObj)[0]->data.objectStatus );
		// HEADROCK HAM B2.5: A certain setting in the New Tracer System can turn auto/burst penalties off
		// entirely, to make up for "Tracer Bump".
		if ( gGameExternalOptions.ubRealisticTracers != 1 )
			bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].bursttohitbonus ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				if ( fProneStance )
					bonus += BonusReduceMore( Item[iter->usItem].bipod, (*iter)[0]->data.objectStatus );
				else
					bonus += BonusReduceMore( Item[iter->usItem].bursttohitbonus, (*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}

void GetRecoil( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 *bRecoilX, INT8 *bRecoilY, UINT8 ubNumBullet )
{
	*bRecoilX = 0;
	*bRecoilY = 0;
 
	//if (ubNumBullet < 2)
	if (ubNumBullet < Weapon[pObj->usItem].ubRecoilDelay)
	{
		// The first bullet in a volley never has recoil - it hasn't "set in" yet. Only the second+ bullets
		// will have any recoil.
		*bRecoilX = 0;
		*bRecoilY = 0;
		return;
	}

	*bRecoilX = Weapon[pObj->usItem].bRecoilX;
	*bRecoilY = Weapon[pObj->usItem].bRecoilY;

	// Apply a percentage-based modifier. This can increase or decrease BOTH axes. At most, it can eliminate
	// recoil on the gun.

	INT16 sPercentRecoilModifier = GetPercentRecoilModifier( pObj );

	*bRecoilX += (*bRecoilX * sPercentRecoilModifier ) / 100;
	*bRecoilY += (*bRecoilY * sPercentRecoilModifier ) / 100;

	// Apply a flat modifier. This acts on either axis, and if powerful enough can "reverse polarity" of either
	// axis recoil. For instance, it can make a gun that normally pulls LEFT start pulling RIGHT instead.

	INT8 bRecoilAdjustX = 0;
	INT8 bRecoilAdjustY = 0;

	GetFlatRecoilModifier( pObj, &bRecoilAdjustX, &bRecoilAdjustY );

	*bRecoilX = __max(0, *bRecoilX + bRecoilAdjustX);
	*bRecoilY = __max(0, *bRecoilY + bRecoilAdjustY);

	return;
}

///////////////////////////////////////////////////////
// HEADROCK HAM 4: This function calculates the flat recoil adjustment for a gun. Flat adjustment increases
// or decreases recoil by a specific number of points in either the vertical or horizontal axes (or both).
// It can potentially cause a weapon it reverse its recoil direction.
void GetFlatRecoilModifier( OBJECTTYPE *pObj, INT8 *bRecoilModifierX, INT8 *bRecoilModifierY )
{

	INT8 bRecoilAdjustX = 0;
	INT8 bRecoilAdjustY = 0;

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		// Inherent item modifiers
		bRecoilAdjustX += BonusReduceMore( Item[pObj->usItem].RecoilModifierX, (*pObj)[0]->data.objectStatus );
		bRecoilAdjustY += BonusReduceMore( Item[pObj->usItem].RecoilModifierY, (*pObj)[0]->data.objectStatus );

		// Ammo item modifiers
		bRecoilAdjustX += Item[(*pObj)[0]->data.gun.usGunAmmoItem].RecoilModifierX;
		bRecoilAdjustY += Item[(*pObj)[0]->data.gun.usGunAmmoItem].RecoilModifierY;

		// Attachment item modifiers
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				bRecoilAdjustX += BonusReduceMore( Item[iter->usItem].RecoilModifierX, (*iter)[0]->data.objectStatus );
				bRecoilAdjustY += BonusReduceMore( Item[iter->usItem].RecoilModifierY, (*iter)[0]->data.objectStatus );
			}
		}
	}

	*bRecoilModifierX = bRecoilAdjustX;
	*bRecoilModifierY = bRecoilAdjustY;
}

///////////////////////////////////////////////////////////////////
// HEADROCK HAM 4: This calculates the percentile recoil adjustment of a gun.
// This adjustment either increases or decreases the gun's vertical and horizontal recoil at the same time. Due to
// the percentage-based nature of this modifier, it cannot cause a gun to reverse its recoil - only diminish it to
// zero.
INT16 GetPercentRecoilModifier( OBJECTTYPE *pObj )
{
	INT16 sRecoilAdjust = 0;

	if (pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		// Inherent item modifiers
		sRecoilAdjust += BonusReduceMore( Item[pObj->usItem].PercentRecoilModifier, (*pObj)[0]->data.objectStatus );

		// Ammo item modifiers
		sRecoilAdjust += Item[(*pObj)[0]->data.gun.usGunAmmoItem].PercentRecoilModifier;

		// Attachment item modifiers
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				sRecoilAdjust += BonusReduceMore( Item[iter->usItem].PercentRecoilModifier, (*iter)[0]->data.objectStatus );
			}
		}
	}

	sRecoilAdjust = __max(-100, sRecoilAdjust);
	return (sRecoilAdjust);
}

// HEADROCK HAM 4: This is used by functions that get stance-based modifiers from weapons. It turns a ubEndHeight variable
// as either 0, 1 or 2.
INT8 GetStanceModifierRef( INT8 ubStance )
{
	switch (ubStance)
	{
		case ANIM_PRONE:
			return(2);
		case ANIM_CROUCH:
			return(1);
		default:
			return(0);
	}
}

INT16 GetDamageBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = BonusReduce( Item[pObj->usItem].damagebonus, (*pObj)[0]->data.objectStatus );

		if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
			bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].damagebonus ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduce( Item[iter->usItem].damagebonus, (*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}


INT16 GetMeleeDamageBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = BonusReduce( Item[pObj->usItem].meleedamagebonus, (*pObj)[0]->data.objectStatus);
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].meleedamagebonus ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduce( Item[iter->usItem].meleedamagebonus, (*iter)[0]->data.objectStatus );
			}
		}
	}
	return( bonus );
}


INT16 GetPercentAPReduction( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = BonusReduceMore( Item[pObj->usItem].percentapreduction, (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentapreduction;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduceMore( Item[iter->usItem].percentapreduction,
					(*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}

INT16 GetMagSizeBonus( OBJECTTYPE * pObj, UINT8 subObject )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = BonusOnOff( Item[pObj->usItem].magsizebonus, (*pObj)[subObject]->data.objectStatus );

		for (attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusOnOff( Item[iter->usItem].magsizebonus, (*iter)[0]->data.objectStatus );
			}
		}
	}
	return( bonus );
}

INT16 GetBurstSizeBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = BonusOnOff( Item[pObj->usItem].burstsizebonus, (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].burstsizebonus ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusOnOff( Item[iter->usItem].burstsizebonus, (*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}

INT16 GetRateOfFireBonus( OBJECTTYPE * pObj )
{
	INT16 bonus=0;

	if (pObj->exists() == true) {
		if( (MAXITEMS <= pObj->usItem) || (MAXITEMS <= (*pObj)[0]->data.gun.usGunAmmoItem) )
		{
			DebugMsg(TOPIC_JA2, DBG_LEVEL_1, String("GetRateOfFireBonus would crash: pObj->usItem=%d or (*pObj)[0]->data.gun.usGunAmmoItem=%d ist higher than max %d", pObj->usItem, (*pObj)[0]->data.gun.usGunAmmoItem, MAXITEMS ));
			ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"GetRateOfFireBonus would crash: pObj->usItem=%d or (*pObj)[0]->data.gun.usGunAmmoItem=%d ist higher than max %d", pObj->usItem, (*pObj)[0]->data.gun.usGunAmmoItem, MAXITEMS );
			AssertMsg( 0, "GetRateOfFireBonus would crash" );
			return 0; /* cannot calculate Bonus, this only happens sometimes in FULLSCREEN */
		}

		bonus = BonusReduceMore( Item[pObj->usItem].rateoffirebonus, (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].rateoffirebonus ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduceMore( Item[iter->usItem].rateoffirebonus, (*iter)[0]->data.objectStatus );
			}
		}
	}
	return( bonus );
}

INT16 GetAutoToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance )
{
	INT16 bonus=0;

	// Snap: bipod is effective only in the prone stance
	// CHRISL: We don't want to count both bipod AND bursttohitbonus as some items get both bonuses

	if (pObj->exists() == true) {
		if ( fProneStance )
			bonus += BonusReduceMore( Item[pObj->usItem].bipod, (*pObj)[0]->data.objectStatus );
		else
			bonus += BonusReduceMore( Item[pObj->usItem].autofiretohitbonus, (*pObj)[0]->data.objectStatus );

		// HEADROCK HAM B2.5: This external setting determines whether autofire penalty is affected by
		// tracer ammo. At setting "1", it is disabled. This goes hand in hand with a new tracer effect that
		// "bumps" CTH up after firing a tracer bullet.
		if ( gGameExternalOptions.ubRealisticTracers != 1 ) 
			bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].autofiretohitbonus ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				if ( fProneStance )
					bonus += BonusReduceMore( Item[iter->usItem].bipod, (*iter)[0]->data.objectStatus );
				else
					bonus += BonusReduceMore( Item[iter->usItem].autofiretohitbonus, (*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}

INT16 GetPercentReadyTimeAPReduction( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = BonusReduceMore( Item[pObj->usItem].percentreadytimeapreduction, (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentreadytimeapreduction;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduceMore( Item[iter->usItem].percentreadytimeapreduction, (*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}

INT16 GetPercentAutofireAPReduction( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = BonusReduceMore( Item[pObj->usItem].percentautofireapreduction, (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentautofireapreduction;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduceMore( Item[iter->usItem].percentautofireapreduction, (*iter)[0]->data.objectStatus );
			}
		}
	}

	return( bonus );
}

INT16 GetPercentReloadTimeAPReduction( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		BonusReduceMore( Item[pObj->usItem].percentreloadtimeapreduction, (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentreloadtimeapreduction;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduceMore( Item[iter->usItem].percentreloadtimeapreduction, (*iter)[0]->data.objectStatus );
			}
		}
	}
	return( bonus );
}

INT16 GetPercentBurstFireAPReduction( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		BonusReduceMore( Item[pObj->usItem].percentburstfireapreduction, (*pObj)[0]->data.objectStatus );
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentburstfireapreduction;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduceMore( Item[iter->usItem].percentburstfireapreduction, (*iter)[0]->data.objectStatus );
			}
		}
	}
	return( bonus );
}


INT16 GetVisionRangeBonus( SOLDIERTYPE * pSoldier )
{
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		// Okay, it's time for some optimization here too
		pObj = &( pSoldier->inv[i]);
		if (pObj->exists() == true) {
			usItem = pObj->usItem;
			pItem = &(Item[usItem]);

			// Snap (TODO): binoculars and such should not be active by default
			if ( (i == HANDPOS || i == SECONDHANDPOS) && (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
			{
				continue;
			}

			//CHRISL: Binoculars can only be used in the primary hand
			if(i == SECONDHANDPOS && pItem->usItemClass & IC_MISC && pItem->visionrangebonus > 0)
			{
				continue;
			}

			if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true) )
			{
				bonus += BonusReduceMore( pItem->visionrangebonus,	(*pObj)[0]->data.objectStatus );
			}
		}
	}

	// Snap: check only attachments on a readied weapon!
	// 0verhaul:  Moved this bug fix into WeaponReady so that all CTH modifier functions may benefit from this fix
	//AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	if ( usingGunScope == true )
	{
		// SANDRO - added scouting check
		INT16 sScopebonus = 0;
		pObj = &( pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) {
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					sScopebonus += BonusReduceMore( Item[iter->usItem].visionrangebonus, (*iter)[0]->data.objectStatus );
				}
			}
		}
		if( sScopebonus > 0 && HAS_SKILL_TRAIT( pSoldier, SCOUTING_NT ) && gGameOptions.fNewTraitSystem)
		{
			sScopebonus += gSkillTraitValues.ubSCSightRangebonusWithScopes;
		}
		bonus += sScopebonus;
	}

	return( bonus );
}

// Snap: Scale night vision bonus with light level
INT16 NightBonusScale( INT16 bonus, UINT8 bLightLevel )
{
	if ( bLightLevel > NORMAL_LIGHTLEVEL_NIGHT ) {
		return idiv( bonus * ( SHADE_MIN - bLightLevel ),
			SHADE_MIN - NORMAL_LIGHTLEVEL_NIGHT );
	}
	else if ( bLightLevel > NORMAL_LIGHTLEVEL_DAY ) {
		return idiv( bonus * (bLightLevel - NORMAL_LIGHTLEVEL_DAY),
			NORMAL_LIGHTLEVEL_NIGHT - NORMAL_LIGHTLEVEL_DAY );
	}
	else return 0;
}

INT16 GetNightVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		if (pObj->exists() == true) {
			usItem = pObj->usItem;
			pItem = &(Item[usItem]);

			// Snap (TODO): binoculars and such should not be active by default
			if ( (i == HANDPOS || i == SECONDHANDPOS) && (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
			{
				continue;
			}

			//CHRISL: Binoculars can only be used in the primary hand
			if(i == SECONDHANDPOS && pItem->usItemClass & IC_MISC && pItem->nightvisionrangebonus > 0)
			{
				continue;
			}

			if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
			{
				bonus += BonusReduceMore(
					NightBonusScale( pItem->nightvisionrangebonus, bLightLevel ),
					(*pObj)[0]->data.objectStatus );
			}
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		// SANDRO - added scouting check
		INT16 sScopebonus = 0;
		pObj = &( pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) {
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					sScopebonus += BonusReduceMore(
						NightBonusScale( Item[iter->usItem].nightvisionrangebonus, bLightLevel ),
						(*iter)[0]->data.objectStatus );
				}
			}
		}
		if( sScopebonus > 0 && HAS_SKILL_TRAIT( pSoldier, SCOUTING_NT ) && gGameOptions.fNewTraitSystem)
		{
			sScopebonus += gSkillTraitValues.ubSCSightRangebonusWithScopes;
		}
		bonus += sScopebonus;
	}

	return( bonus );
}

INT16 GetCaveVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		if (pObj->exists() == true) {
			usItem = pObj->usItem;
			pItem = &(Item[usItem]);

			// Snap (TODO): binoculars and such should not be active by default
			if ( (i == HANDPOS || i == SECONDHANDPOS) &&
				   (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
			{
				continue;
			}

			//CHRISL: Binoculars can only be used in the primary hand
			if(i == SECONDHANDPOS && pItem->usItemClass & IC_MISC && pItem->cavevisionrangebonus > 0)
			{
				continue;
			}

			if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
			{
				bonus += BonusReduceMore(
					NightBonusScale( pItem->cavevisionrangebonus, bLightLevel ),
					(*pObj)[0]->data.objectStatus );
			}
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		// SANDRO - added scouting check
		INT16 sScopebonus = 0;
		pObj = &( pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) {
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					sScopebonus += BonusReduceMore(
						NightBonusScale( Item[iter->usItem].cavevisionrangebonus, bLightLevel ),
						(*iter)[0]->data.objectStatus );
				}
			}
		}
		if( sScopebonus > 0 && HAS_SKILL_TRAIT( pSoldier, SCOUTING_NT ) && gGameOptions.fNewTraitSystem)
		{
			sScopebonus += gSkillTraitValues.ubSCSightRangebonusWithScopes;
		}
		bonus += sScopebonus;
	}

	return( bonus );
}

INT16 GetDayVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	// Snap: Scale the bonus with the light level

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		if (pObj->exists() == true) {
			usItem = pObj->usItem;
			pItem = &(Item[usItem]);

			// Snap (TODO): binoculars and such should not be active by default
			if ( (i == HANDPOS || i == SECONDHANDPOS) &&
				   (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
			{
				continue;
			}

			//CHRISL: Binoculars can only be used in the primary hand
			if(i == SECONDHANDPOS && pItem->usItemClass & IC_MISC && pItem->dayvisionrangebonus > 0)
			{
				continue;
			}

			if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
			{
				bonus += BonusReduceMore( idiv( pItem->dayvisionrangebonus
					* (NORMAL_LIGHTLEVEL_NIGHT - bLightLevel), NORMAL_LIGHTLEVEL_NIGHT ),
					(*pObj)[0]->data.objectStatus );
			}
		}
	}

	// Snap: check only attachments on a raised weapon!
	//CHRISL: Since this is a daytime calculation, I think we want the difference between NORMAL_LIGHTLEVEL_NIGHT and
	//	NORMAL_LIGHTLEVEL_DAY.  To just use NORMAL_LIGHTLEVEL_NIGHT is the same as basing the calculation off of the
	//	difference between NORMAL_LIGHTLEVEL_NIGHT and 0, which represent bright light.
	if ( usingGunScope == true )
	{
		// SANDRO - added scouting check
		INT16 sScopebonus = 0;
		pObj = &( pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) {
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					sScopebonus += BonusReduceMore( idiv( Item[iter->usItem].dayvisionrangebonus
						* (NORMAL_LIGHTLEVEL_NIGHT - __max(bLightLevel,NORMAL_LIGHTLEVEL_DAY)), (NORMAL_LIGHTLEVEL_NIGHT-NORMAL_LIGHTLEVEL_DAY) ),
						(*iter)[0]->data.objectStatus );
				}
			}
		}
		if( sScopebonus > 0 && HAS_SKILL_TRAIT( pSoldier, SCOUTING_NT ) && gGameOptions.fNewTraitSystem)
		{
			sScopebonus += gSkillTraitValues.ubSCSightRangebonusWithScopes;
		}
		bonus += sScopebonus;
	}

	return( bonus );
}

INT16 GetBrightLightVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	// Snap: Scale the bonus with the light level

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		if (pObj->exists() == true) {
			usItem = pObj->usItem;
			pItem = &(Item[usItem]);

			// Snap (TODO): binoculars and such should not be active by default
			if ( (i == HANDPOS || i == SECONDHANDPOS) &&
				   (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
			{
				continue;
			}

			//CHRISL: Binoculars can only be used in the primary hand
			if(i == SECONDHANDPOS && pItem->usItemClass & IC_MISC && pItem->brightlightvisionrangebonus > 0)
			{
				continue;
			}

			if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
			{
				bonus += BonusReduceMore( idiv( pItem->brightlightvisionrangebonus
					* (NORMAL_LIGHTLEVEL_DAY - bLightLevel), NORMAL_LIGHTLEVEL_DAY ),
					(*pObj)[0]->data.objectStatus );
			}
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		// SANDRO - added scouting check
		INT16 sScopebonus = 0;
		pObj = &( pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) {
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists()){
					sScopebonus += BonusReduceMore( idiv( Item[iter->usItem].brightlightvisionrangebonus
						* (NORMAL_LIGHTLEVEL_DAY - bLightLevel), NORMAL_LIGHTLEVEL_DAY ),
						(*iter)[0]->data.objectStatus );
				}
			}
		}
		if( sScopebonus > 0 && HAS_SKILL_TRAIT( pSoldier, SCOUTING_NT ) && gGameOptions.fNewTraitSystem)
		{
			sScopebonus += gSkillTraitValues.ubSCSightRangebonusWithScopes;
		}
		bonus += sScopebonus;
	}

	return( bonus );
}

INT16 GetTotalVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	INT16 bonus = GetVisionRangeBonus(pSoldier);

	if ( bLightLevel > NORMAL_LIGHTLEVEL_DAY )
	{
		if ( pSoldier->pathing.bLevel == 0 )
		{
			bonus += GetNightVisionRangeBonus(pSoldier, bLightLevel);
		}
		else
		{
			bonus += GetCaveVisionRangeBonus(pSoldier, bLightLevel);
		}
	}

	if ( bLightLevel < NORMAL_LIGHTLEVEL_NIGHT )
	{
		bonus += GetDayVisionRangeBonus(pSoldier, bLightLevel);
	}

	if ( bLightLevel < NORMAL_LIGHTLEVEL_DAY )
	{
		bonus += GetBrightLightVisionRangeBonus(pSoldier, bLightLevel);
	}

	// SANDRO - STOMP traits - Scouting bonus for sight range with binoculars and similar
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SCOUTING_NT ) && pSoldier->pathing.bLevel == 0 )
	{
		OBJECTTYPE *pObj = &( pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) 
		{
			// we have no pointer to binoculars, so just check any misc items in hands which have some vision bonus
			if ( Item[pObj->usItem].usItemClass & IC_MISC && (Item[pObj->usItem].brightlightvisionrangebonus > 0 ||
				Item[pObj->usItem].dayvisionrangebonus > 0 || Item[pObj->usItem].nightvisionrangebonus > 0 || Item[pObj->usItem].visionrangebonus > 0 )) 
			{
				bonus += gSkillTraitValues.usSCSightRangebonusWithBinoculars;
			}
		}
	}

	return bonus;
}

UINT8 GetPercentTunnelVision( SOLDIERTYPE * pSoldier )
{
	UINT8 bonus = 0;
	UINT16 usItem;
	INVTYPE *pItem;

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		// Okay, it's time for some optimization here
		if (pSoldier->inv[i].exists() == true) {
			usItem = pSoldier->inv[i].usItem;
			pItem = &(Item[usItem]);

			if ( (i == HANDPOS || i == SECONDHANDPOS) && (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
			{
				continue;
			}

			//CHRISL: Binoculars can only be used in the primary hand
			if(i == SECONDHANDPOS && pItem->usItemClass & IC_MISC && pItem->percenttunnelvision > 0)
			{
				continue;
			}

			if ( !IsWeapon(usItem) )
			{
				bonus = __max( bonus, pItem->percenttunnelvision );
			}
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		OBJECTTYPE *pObj = &(pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) {
			usItem = pObj->usItem;
			pItem = &(Item[usItem]);

			if ( IsWeapon(usItem) ) //if not a weapon, then it was added already above
				bonus += Item[usItem].percenttunnelvision;

			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if(iter->exists())
					bonus += Item[iter->usItem].percenttunnelvision;
			}
		}
	}

	// SANDRO - STOMP traits - Scouting tunnel vision reduction with binoculars and similar
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SCOUTING_NT ) && pSoldier->pathing.bLevel == 0 )
	{
		OBJECTTYPE *pObj = &( pSoldier->inv[HANDPOS]);
		if (pObj->exists() == true) 
		{
			// we have no pointer to binoculars, so just check any misc items in hands which have some vision bonus
			if ( Item[pObj->usItem].usItemClass & IC_MISC && (Item[pObj->usItem].brightlightvisionrangebonus > 0 ||
				Item[pObj->usItem].dayvisionrangebonus > 0 || Item[pObj->usItem].nightvisionrangebonus > 0 || Item[pObj->usItem].visionrangebonus > 0 )) 
			{
				bonus = max( 0, (bonus - gSkillTraitValues.ubSCTunnelVisionReducedWithBinoculars));
			}
		}
	}
	// HEADROCK HAM 3.2: Further increase tunnel-vision for cowering characters.
	// SANDRO - this calls many sub-functions over and over, we should at least skip this for civilians and such  
	if ((gGameExternalOptions.ubCoweringReducesSightRange == 1 || gGameExternalOptions.ubCoweringReducesSightRange == 3) &&
		IS_MERC_BODY_TYPE(pSoldier) && (pSoldier->bTeam == ENEMY_TEAM || pSoldier->bTeam == MILITIA_TEAM || pSoldier->bTeam == gbPlayerNum) )
	{
		INT8 bTolerance = CalcSuppressionTolerance( pSoldier );

		// Make sure character is cowering.
		if ( pSoldier->aiData.bShock >= bTolerance && gGameExternalOptions.ubMaxSuppressionShock > 0 && 
			bonus < 100 )
		{
			// Calculates a "Flat" tunnel vision percentage
			UINT8 ubNormalCoweringTunnelVision = (100 * pSoldier->aiData.bShock) / gGameExternalOptions.ubMaxSuppressionShock;

			// Apply that percentage to the current tunnel vision
			UINT16 usActualCoweringTunnelVision = bonus + (((100-bonus) * ubNormalCoweringTunnelVision) / 100);

			// At shock 0, tunnel vision remains unchanged. At full shock, tunnel vision is full (100%)
			bonus = __min(100,usActualCoweringTunnelVision);
		}
	}

	if ( PTR_OURTEAM ) // Madd: adjust tunnel vision by difficulty level
		return( bonus );
	else
		return bonus / gGameOptions.ubDifficultyLevel;
}


BOOLEAN HasThermalOptics( SOLDIERTYPE * pSoldier )
{

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		if (pSoldier->inv[i].exists() == true) {
			if ( (i == HANDPOS || i == SECONDHANDPOS) &&
				   (Item[pSoldier->inv[i].usItem].usItemClass & IC_ARMOUR || Item[pSoldier->inv[i].usItem].usItemClass & IC_FACE ))
			{
				continue;
			}

			if (!IsWeapon(pSoldier->inv[i].usItem) || (IsWeapon(pSoldier->inv[i].usItem) && usingGunScope == true) )
			{
				if (Item[pSoldier->inv[i].usItem].thermaloptics)
					return TRUE;
			}
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		OBJECTTYPE* pObj = &pSoldier->inv[HANDPOS];
		if (pObj->exists() == true) {
			for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
				if ( Item[iter->usItem].thermaloptics && iter->exists() )
					return TRUE;
			}
		}
	}

	return( FALSE );
}


INT8 FindHearingAid( SOLDIERTYPE * pSoldier )
{
	for (INT8 i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		if (pSoldier->inv[i].exists() == true) {
			if ( (i == HANDPOS || i == SECONDHANDPOS) &&
				   (Item[pSoldier->inv[i].usItem].usItemClass & IC_ARMOUR || Item[pSoldier->inv[i].usItem].usItemClass & IC_FACE ))
			{
				continue;
			}

			if ( Item[pSoldier->inv[i].usItem].hearingrangebonus > 0 )
			{
				return( i );
			}
		}
	}
	return( NO_SLOT );
}

INT16 GetHearingRangeBonus( SOLDIERTYPE * pSoldier )
{
	INT16 bonus = 0;

	for (int i = BODYPOSSTART; i < BODYPOSFINAL; i++)
	{
		if (pSoldier->inv[i].exists() == true) {
			if ( (i == HANDPOS || i == SECONDHANDPOS) &&
				   (Item[pSoldier->inv[i].usItem].usItemClass & IC_ARMOUR || Item[pSoldier->inv[i].usItem].usItemClass & IC_FACE ))
			{
				continue;
			}

			bonus += BonusReduceMore( Item[pSoldier->inv[i].usItem].hearingrangebonus, pSoldier->inv[i][0]->data.objectStatus );
		}
	}
	return( bonus );
}


BOOLEAN IsDuckbill( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		if (Item[pObj->usItem].duckbill || Item[(*pObj)[0]->data.gun.usGunAmmoItem].duckbill )
			return TRUE;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].duckbill && iter->exists() )
			{
				return( TRUE );
			}
		}
	}
	return( FALSE );
}


UINT16 GetPercentNoiseVolume( OBJECTTYPE * pObj )
{
	UINT16 mod = 0;
	if (pObj->exists() == true) {
		mod = 100 - BonusReduce( Item[pObj->usItem].percentnoisereduction, (*pObj)[0]->data.objectStatus );
		mod = mod * ( 100 - Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentnoisereduction ) / 100;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
			mod = mod * ( 100 - BonusReduce( Item[iter->usItem].percentnoisereduction, (*iter)[0]->data.objectStatus ) ) / 100;
		}
	}

	return (mod > 0) ? mod : 0;
}


INT8 FindGasMask( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < NUM_INV_SLOTS; bLoop++)//dnl ch40 041009
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( Item[pSoldier->inv[bLoop].usItem].gasmask )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}


BOOLEAN IsDetonatorAttached( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		//if (Item[pObj->usItem].detonator)
		//	return TRUE;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].detonator && iter->exists() )
			{
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOLEAN IsRemoteDetonatorAttached( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		//if (Item[pObj->usItem].remotedetonator)
		//	return TRUE;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].remotedetonator && iter->exists() )
			{
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOLEAN IsFlashSuppressor( OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier )
{
	if (pObj->exists() == true) {
		//Madd: tracers automatically negate any muzzle flash suppression due to inherent lighting effects
		if (Item[pObj->usItem].usItemClass == IC_GUN && AmmoTypes[(*pObj)[0]->data.gun.ubGunAmmoType].tracerEffect && pSoldier->bDoBurst )
			return FALSE;

		if (Item[pObj->usItem].hidemuzzleflash )
			return TRUE;

		if ( Item[(*pObj)[0]->data.gun.usGunAmmoItem].hidemuzzleflash )
			return TRUE;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].hidemuzzleflash && iter->exists() )
			{
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

INT16 GetFlashSuppressorStatus( OBJECTTYPE * pObj )
{
	INT16 p=100;
	if (pObj->exists() == true) {
		if (Item[pObj->usItem].hidemuzzleflash )
			p=(*pObj)[0]->data.objectStatus;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].hidemuzzleflash && iter->exists() )
			{
				p =p+ (*iter)[0]->data.objectStatus;
			}
		}
	}
	p = min(p,100);
	return p;
}

BOOLEAN IsGrenadeLauncherAttached( OBJECTTYPE * pObj, UINT8 subObject )
{
	if (pObj->exists() == true) {
		if (Item[pObj->usItem].grenadelauncher )
			return TRUE;

		for (attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end(); ++iter) {
			if (Item[iter->usItem].grenadelauncher && iter->exists() )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

OBJECTTYPE* FindAttachment_GrenadeLauncher( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		if (Item[pObj->usItem].grenadelauncher )
			return pObj;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].grenadelauncher && iter->exists() )
			{
				return( &(*iter) );
			}
		}
	}
	return( 0 );
}
INT16 GetGrenadeLauncherStatus( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		if (Item[pObj->usItem].grenadelauncher  )
			return (*pObj)[0]->data.objectStatus;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].grenadelauncher && iter->exists())
			{
				return( (*iter)[0]->data.objectStatus );
			}
		}
	}
	return( ITEM_NOT_FOUND );
}
UINT16 GetAttachedGrenadeLauncher( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		if (Item[pObj->usItem].grenadelauncher  )
			return pObj->usItem;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].grenadelauncher && iter->exists())
			{
				return( (UINT16) Item[iter->usItem].uiIndex );
			}
		}
	}
	return( NONE );
}


INT16 GetAttachedArmourBonus( OBJECTTYPE * pObj )
{
	INT16 bonus=0;

	if (pObj->exists() == true) {
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists()){
				bonus += BonusReduce( Armour[Item[iter->usItem].ubClassIndex].ubProtection,
					(*iter)[0]->data.objectStatus );
			}
		}
	}
	return( bonus );
}


INT16 GetBulletSpeedBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if (pObj->exists() == true) {
		bonus = Item[pObj->usItem].bulletspeedbonus ;
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].bulletspeedbonus ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
			bonus = bonus +Item[iter->usItem].bulletspeedbonus  ;
		}
	}
	return( bonus );
}

BOOLEAN EXPLOSIVE_GUN ( UINT16 x)
{
	 //DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("EXPLOSIVE_GUN x = %d",x));

	if ( Item[x].rocketlauncher || Item[x].cannon )
		return TRUE;
	else
		return FALSE;
}

INT8 FindRocketLauncherOrCannon( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].rocketlauncher || Item[pSoldier->inv[bLoop].usItem].cannon )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

INT8 FindRocketLauncher( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].rocketlauncher )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

INT8 FindCannon( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( Item[pSoldier->inv[bLoop].usItem].cannon )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

INT8 FindUsableCrowbar( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if ( Item[pSoldier->inv[bLoop].usItem].crowbar && pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

OBJECTTYPE* FindAttachedBatteries( OBJECTTYPE * pObj )
{
	if (pObj->exists() == true) {
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (Item[iter->usItem].batteries && iter->exists())
			{
				return( &(*iter) );
			}
		}
	}
	return( 0 );
}
INT8 FindToolkit( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].toolkit )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}
INT8 FindMedKit( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].medicalkit  )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}
INT8 FindFirstAidKit( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].firstaidkit  )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}
INT8 FindCamoKit( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].camouflagekit   )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}
INT8 FindLocksmithKit( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].locksmithkit   )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}
INT8 FindWalkman( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = BODYPOSSTART; bLoop < BODYPOSFINAL; bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].walkman  )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}
INT8 FindTrigger( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].remotetrigger   )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}
INT8 FindRemoteControl( SOLDIERTYPE * pSoldier )
{
	INT8 bLoop;

	for (bLoop = BODYPOSSTART; bLoop < BODYPOSFINAL; bLoop++)
	{
		if (pSoldier->inv[bLoop].exists() == true) {
			if (Item[pSoldier->inv[bLoop].usItem].robotremotecontrol    )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

UINT16 LowestLaunchableCoolness(UINT16 launcherIndex)
{
	UINT16 i = 0;
	UINT16 lowestCoolness = 999;

	for( i = 0; i < MAXITEMS; i++ )
	{
		if ( Item[i].usItemClass  == 0 )
			break;

		if( ValidLaunchable( i, launcherIndex ) && ItemIsLegal(i) && Item[i].ubCoolness <= lowestCoolness )
		{
			lowestCoolness = Item[i].ubCoolness;
		}
	}
	return lowestCoolness;
}

UINT16 PickARandomLaunchable(UINT16 itemIndex)
{
	UINT16 usNumMatches = 0;
	UINT16 usRandom = 0;
	UINT16 i = 0;
	UINT16 lowestCoolness = LowestLaunchableCoolness(itemIndex);
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("PickARandomLaunchable: itemIndex = %d", itemIndex));

	// WANNE: This should fix the hang on the merc positioning screen (fix by Razer)
	//while( !usNumMatches )
	{ //Count the number of valid launchables
		for( i = 0; i < MAXITEMS; i++ )
		{
			if ( Item[i].usItemClass  == 0 )
				break;
			//Madd: quickfix: make it not choose best grenades right away.
			if( ValidLaunchable( i, itemIndex ) && ItemIsLegal(i) && Item[i].ubCoolness <= max(HighestPlayerProgressPercentage()/10,lowestCoolness) )
				usNumMatches++;
		}
	}

	if( usNumMatches )
	{
		usRandom = (UINT16)Random( usNumMatches );
		for( i = 0; i < MAXITEMS; i++ )
		{
			if ( Item[i].usItemClass  == 0 )
				break;

			if( ValidLaunchable( i, itemIndex ) && ItemIsLegal(i) && Item[i].ubCoolness <= max(HighestPlayerProgressPercentage()/10,lowestCoolness) )
			{
				if( usRandom )
					usRandom--;
				else
				{
					return i;
				}
			}
		}
	}

	return 0;
}
INT16 GetCamoBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if ( pObj->exists() == true ) {
		bonus = (Item[pObj->usItem].camobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (!Item[iter->usItem].camouflagekit && iter->exists())
				bonus += (INT16) (Item[iter->usItem].camobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
		}
	}
	return( bonus );
}
INT16 GetUrbanCamoBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if ( pObj->exists() == true ) {
		bonus = (Item[pObj->usItem].urbanCamobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (!Item[iter->usItem].camouflagekit && iter->exists())
				bonus += (INT16) (Item[iter->usItem].urbanCamobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
		}
	}
	return( bonus );
}
INT16 GetDesertCamoBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if ( pObj->exists() == true ) {
		bonus = (Item[pObj->usItem].desertCamobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (!Item[iter->usItem].camouflagekit && iter->exists())
				bonus += (INT16) (Item[iter->usItem].desertCamobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
		}
	}
	return( bonus );
}
INT16 GetSnowCamoBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if ( pObj->exists() == true ) {
		bonus = (Item[pObj->usItem].snowCamobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if (!Item[iter->usItem].camouflagekit && iter->exists())
				bonus += (INT16) (Item[iter->usItem].snowCamobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
		}
	}
	return( bonus );
}
INT16 GetWornCamo( SOLDIERTYPE * pSoldier )
{
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].exists() == true )
		{
			ttl += GetCamoBonus(&pSoldier->inv[bLoop]);
		}
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingNewInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].exists() == true )
				ttl += GetCamoBonus(&pSoldier->inv[bLoop]);
			//CHRISL: to prevent camo overlap, don't count armor vest camo if we're weaing an LBE vest that grants a bonus
			if(bLoop == VESTPOCKPOS && pSoldier->inv[VESTPOS].exists() == true && Item[pSoldier->inv[bLoop].usItem].camobonus > 0)
			{
				ttl -= Item[pSoldier->inv[VESTPOS].usItem].camobonus;
			}
		}
	}

	return __min( ttl, 100 );
}
INT16 GetWornUrbanCamo( SOLDIERTYPE * pSoldier )
{
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].exists() == true )
			ttl += GetUrbanCamoBonus(&pSoldier->inv[bLoop]);
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingNewInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].exists() == true )
				ttl += GetUrbanCamoBonus(&pSoldier->inv[bLoop]);
		}
	}

	return __min( ttl, 100 );
}
INT16 GetWornDesertCamo( SOLDIERTYPE * pSoldier )
{
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].exists() == true )
			ttl += GetDesertCamoBonus(&pSoldier->inv[bLoop]);
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingNewInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].exists() == true )
				ttl += GetDesertCamoBonus(&pSoldier->inv[bLoop]);
		}
	}
	return __min( ttl, 100 );
}
INT16 GetWornSnowCamo( SOLDIERTYPE * pSoldier )
{
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].exists() == true )
			ttl += GetSnowCamoBonus(&pSoldier->inv[bLoop]);
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingNewInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].exists() == true )
				ttl += GetSnowCamoBonus(&pSoldier->inv[bLoop]);
		}
	}
	return __min( ttl, 100 );
}

void ApplyEquipmentBonuses(SOLDIERTYPE * pSoldier)
{
	if ( pSoldier == NULL) return;

	INT16 newCamo = GetWornCamo ( pSoldier );
	INT16 oldCamo = pSoldier->wornCamo;
	if ( oldCamo != newCamo )
		pSoldier->wornCamo = (INT8)newCamo;

	INT16 newUrbanCamo = GetWornUrbanCamo ( pSoldier );
	INT16 oldUrbanCamo = pSoldier->wornUrbanCamo;
	if ( oldUrbanCamo != newUrbanCamo )
		pSoldier->wornUrbanCamo = (INT8)newUrbanCamo;

	INT16 newDesertCamo = GetWornDesertCamo ( pSoldier );
	INT16 oldDesertCamo = pSoldier->wornDesertCamo;
	if ( oldDesertCamo != newDesertCamo )
		pSoldier->wornDesertCamo = (INT8)newDesertCamo;

	INT16 newSnowCamo = GetWornSnowCamo ( pSoldier );
	INT16 oldSnowCamo = pSoldier->wornSnowCamo;
	if ( oldSnowCamo != newSnowCamo )
		pSoldier->wornSnowCamo = (INT8)newSnowCamo;

	if ( (newCamo > oldCamo || newUrbanCamo > oldUrbanCamo || newDesertCamo > oldDesertCamo || newSnowCamo > oldSnowCamo )&& pSoldier->bTeam == OUR_TEAM )
	{
		//CHRISL: This sound interferes with some RPC hiring in NewInv because of the camo bonus some LBE Vests give
		if(UsingNewInventorySystem() == false)
			pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );

		// WANNE: Only call the method if oldCame != newCamo
		if ( pSoldier->bInSector)
			pSoldier->CreateSoldierPalettes( );
	}
	else if ( (newCamo < oldCamo || newUrbanCamo < oldUrbanCamo || newDesertCamo < oldDesertCamo || newSnowCamo < oldSnowCamo )&& pSoldier->bTeam == OUR_TEAM )
	{
		// WANNE: Only call the method if oldCame != newCamo
		if ( pSoldier->bInSector)
			pSoldier->CreateSoldierPalettes( );
	}
	// WANNE: Madd, I commented this, because this leads to IRAs INVISIBLE BUG!
	// We should only call the CreateSoldierPalettes if oldCamo != newCamo. See above!
	//Madd: do this regardless of camo.  This will need to be called to do custom part colours and new overlays anyway.
	//if ( pSoldier->bInSector)
	//	pSoldier->CreateSoldierPalettes( );

	
	// WANNE: I disabled the call, because it leeds to endless loop when examining doors with explosives!
	/*
	// This should fix the bug and crashes with missing faces
	if (SetCamoFace( pSoldier ))
	{
		DeleteSoldierFace( pSoldier );// remove face
		pSoldier->iFaceIndex = InitSoldierFace( pSoldier );// create new face
	}
	*/

	fInterfacePanelDirty = DIRTYLEVEL2;
}

UINT16 GetFirstExplosiveOfType(UINT16 expType)
{
	for (int i=0;i<MAXITEMS;i++)
	{
		if ( (Item[i].usItemClass == IC_EXPLOSV || Item[i].usItemClass == IC_GRENADE) && Explosive[Item[i].ubClassIndex].ubType == expType )
			return i;
	}

	return 0;
}

// WDS - Smart goggle switching
OBJECTTYPE* FindSunGogglesInInv( SOLDIERTYPE * pSoldier, INT8 * bSlot, BOOLEAN * isAttach, BOOLEAN searchAllInventory )
{
	INT8	bLoop;
	INT16	bonusToBeat = 0;
	OBJECTTYPE*	pGoggles = 0;
	// CHRISL:
	// silversurfer: use the sum of day vision bonus and bright light bonus because both help at daytime
	// but make sure that day vision bonus is > 0 because we are not always looking at bright lights
	for (bLoop = (searchAllInventory ? HELMETPOS : HANDPOS); bLoop < NUM_INV_SLOTS; bLoop++) {
		if ( pSoldier->inv[bLoop].exists() == true ) {
			if ( Item[pSoldier->inv[bLoop].usItem].dayvisionrangebonus > 0 && 
					( Item[pSoldier->inv[bLoop].usItem].dayvisionrangebonus + Item[pSoldier->inv[bLoop].usItem].brightlightvisionrangebonus ) > bonusToBeat && 
					  Item[pSoldier->inv[bLoop].usItem].usItemClass == IC_FACE ) {
				pGoggles = &(pSoldier->inv[bLoop]);
				*bSlot = bLoop;
				*isAttach = FALSE;
				bonusToBeat = Item[pSoldier->inv[bLoop].usItem].dayvisionrangebonus + Item[pSoldier->inv[bLoop].usItem].brightlightvisionrangebonus;
			}
			if (searchAllInventory) {
				for (UINT8 loop = 0; loop < pSoldier->inv[bLoop].ubNumberOfObjects; loop ++){
					for (attachmentList::iterator iter = pSoldier->inv[bLoop][loop]->attachments.begin(); iter != pSoldier->inv[bLoop][loop]->attachments.end(); ++iter) {
						if ( iter->exists() && Item[ iter->usItem ].dayvisionrangebonus > 0 &&
							( Item[ iter->usItem ].dayvisionrangebonus + Item[ iter->usItem ].brightlightvisionrangebonus ) > bonusToBeat &&
							   Item[ iter->usItem ].usItemClass == IC_FACE ) {
							pGoggles = &(*iter);
							*bSlot = bLoop;
							*isAttach = TRUE;
							bonusToBeat = Item[ iter->usItem ].dayvisionrangebonus + Item[ iter->usItem ].brightlightvisionrangebonus;
						}
					}
				}
			}
		}
	}
	return( pGoggles );
}

OBJECTTYPE* FindNightGogglesInInv( SOLDIERTYPE * pSoldier, INT8 * bSlot, BOOLEAN * isAttach, BOOLEAN searchAllInventory )
{
	INT8	bLoop;
	INT16	bonusToBeat = 0;
	OBJECTTYPE*	pGoggles = 0;
	// CHRISL:
	// silversurfer: check if we are above ground, night vision is only useful there
	if ( pSoldier->bSectorZ == 0 )
	{
		for (bLoop = (searchAllInventory ? HELMETPOS : HANDPOS); bLoop < NUM_INV_SLOTS; bLoop++) {
			if ( pSoldier->inv[bLoop].exists() == true ) {
				if (Item[pSoldier->inv[bLoop].usItem].nightvisionrangebonus > bonusToBeat && Item[pSoldier->inv[bLoop].usItem].usItemClass == IC_FACE ) {
					pGoggles = &(pSoldier->inv[bLoop]);
					*bSlot = bLoop;
					*isAttach = FALSE;
					bonusToBeat = Item[pSoldier->inv[bLoop].usItem].nightvisionrangebonus;
				}
				if (searchAllInventory) {
					for (UINT8 loop = 0; loop < pSoldier->inv[bLoop].ubNumberOfObjects; loop ++){
						for (attachmentList::iterator iter = pSoldier->inv[bLoop][loop]->attachments.begin(); iter != pSoldier->inv[bLoop][loop]->attachments.end(); ++iter) {
							if ( iter->exists() && Item[ iter->usItem ].nightvisionrangebonus > bonusToBeat && Item[ iter->usItem ].usItemClass == IC_FACE ) {
								pGoggles = &(*iter);
								*bSlot = bLoop;
								*isAttach = TRUE;
								bonusToBeat = Item[ iter->usItem ].nightvisionrangebonus;
							}
						}
					}
				}
			}
		}
	}
	// below ground we need cave vision
	else
	{
		for (bLoop = (searchAllInventory ? HELMETPOS : HANDPOS); bLoop < NUM_INV_SLOTS; bLoop++) {
			if ( pSoldier->inv[bLoop].exists() == true ) {
				if (Item[pSoldier->inv[bLoop].usItem].cavevisionrangebonus > bonusToBeat && Item[pSoldier->inv[bLoop].usItem].usItemClass == IC_FACE ) {
					pGoggles = &(pSoldier->inv[bLoop]);
					*bSlot = bLoop;
					*isAttach = FALSE;
					bonusToBeat = Item[pSoldier->inv[bLoop].usItem].cavevisionrangebonus;
				}
				if (searchAllInventory) {
					for (UINT8 loop = 0; loop < pSoldier->inv[bLoop].ubNumberOfObjects; loop ++){
						for (attachmentList::iterator iter = pSoldier->inv[bLoop][loop]->attachments.begin(); iter != pSoldier->inv[bLoop][loop]->attachments.end(); ++iter) {
							if ( Item[ iter->usItem ].cavevisionrangebonus > bonusToBeat && Item[ iter->usItem ].usItemClass == IC_FACE && iter->exists() ) {
								pGoggles = &(*iter);
								*bSlot = bLoop;
								*isAttach = TRUE;
								bonusToBeat = Item[ iter->usItem ].cavevisionrangebonus;
							}
						}
					}
				}
			}
		}
	}

	return( pGoggles );
}

FLOAT GetHighestScopeMagnificationFactor( OBJECTTYPE * pObj )
{
	FLOAT BestFactor = 1.0;

	if ( pObj->exists() == true && UsingNewCTHSystem() == true ) {
		BestFactor = Item[pObj->usItem].scopemagfactor;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) 
		{
			if (iter->exists())
			{
				BestFactor = __max(BestFactor, Item[iter->usItem].scopemagfactor);
			}
		}
	}

	return( BestFactor );
}

INT16 GetMinRangeForAimBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;

	if ( pObj->exists() == true ) {
		bonus = Item[pObj->usItem].minrangeforaimbonus;
		//bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].minrangeforaimbonus;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += Item[iter->usItem].minrangeforaimbonus;
		}
	}

	return( bonus );
}

FLOAT GetScopeMagnificationFactor( OBJECTTYPE * pObj, FLOAT uiRange )
{
	FLOAT BestFactor = 1.0;
	FLOAT CurrentFactor = 0.0;
	FLOAT TargetMagFactor = __max(1.0f,(FLOAT)uiRange / (FLOAT)gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);
	FLOAT rangeModifier = gGameCTHConstants.SCOPE_RANGE_MULTIPLIER;

	TargetMagFactor = TargetMagFactor / rangeModifier;

	if(pObj->exists() == true && UsingNewCTHSystem() == true)
	{
		BestFactor = __max(1.0f, Item[pObj->usItem].scopemagfactor);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) 
		{
			if(iter->exists() == true)
			{
				if(BestFactor == 1.0f)
					BestFactor = __max(BestFactor, Item[iter->usItem].scopemagfactor);
				else if(Item[iter->usItem].scopemagfactor >= TargetMagFactor)
					BestFactor = Item[iter->usItem].scopemagfactor;
			}
		}
	}

	return __max(1.0f, BestFactor);
}

FLOAT GetBestScopeMagnificationFactor( SOLDIERTYPE *pSoldier, OBJECTTYPE * pObj, FLOAT uiRange )
{
	FLOAT BestFactor = 1.0;
	FLOAT TargetMagFactor = __max(1.0f,uiRange / (FLOAT)gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);
	FLOAT CurrentFactor = 0.0;
	FLOAT ActualCurrentFactor = 0.0;
	INT32 iCurrentTotalPenalty = 0;
	INT32 iBestTotalPenalty = 0;
	FLOAT rangeModifier = GetScopeRangeMultiplier(pSoldier, pObj, uiRange);
	FLOAT iProjectionFactor = CalcProjectionFactor(pSoldier, pObj, uiRange, 1);

	if (TargetMagFactor <= 1.0f)
	{
		// Target is at Iron Sights range. No scope is required.
		return 1.0f;
	}

	if ( pObj->exists() == true && UsingNewCTHSystem() == true )
	{
		// Real Scope Magnification Factor from the item
		CurrentFactor = __max(1.0f, Item[pObj->usItem].scopemagfactor);

		if (CurrentFactor > 1.0f)
		{
			// Actual Scope Mag Factor is what we get at the distance the target's at.
			ActualCurrentFactor = __min(CurrentFactor, (TargetMagFactor/rangeModifier));

			if (ActualCurrentFactor >= CurrentFactor)
			{
				// This scope gives no penalty. Record this as the best factor found so far.
				BestFactor = CurrentFactor;
				iBestTotalPenalty = 0;
			}
			else
			{
				// This scopes gives a penalty for shooting under its range.
				FLOAT dScopePenaltyRatio = (CurrentFactor * rangeModifier / TargetMagFactor);
				INT32 iScopePenalty = (INT32)((dScopePenaltyRatio * gGameCTHConstants.AIM_TOO_CLOSE_SCOPE) * (CurrentFactor / 2));

				// There's no previous scope to compare with so record this as the best factor for now.
				BestFactor = CurrentFactor;
				iBestTotalPenalty = iScopePenalty;
			}

		}
		
		// Now perform the same process for each scope installed on the item. The difference is, we also compare to 
		// BestTotalPenalty to find the scope that gives the least penalty compared to its bonus.
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) 
		{
			if (iter->exists() && Item[iter->usItem].scopemagfactor > 1.0f)
			{
				// Real Scope Magnification Factor from the item
				CurrentFactor = __max(1.0f, Item[iter->usItem].scopemagfactor);

				// Actual Scope Mag Factor is what we get at the distance the target's at.
				ActualCurrentFactor = __min(CurrentFactor, (TargetMagFactor/rangeModifier));

				if (ActualCurrentFactor >= CurrentFactor)
				{
					// This scope gives no penalty. Is it any better than the ones we've already processed?
					if (iBestTotalPenalty >= 0 && CurrentFactor > BestFactor)
					{
						// This is the best scope we've found so far. Record it.
						BestFactor = CurrentFactor;
						iBestTotalPenalty = 0;
					}
				}
				else
				{
					// This scope will give a penalty if used. Is it worth using compared to other scopes found?
					FLOAT dScopePenaltyRatio = (CurrentFactor * rangeModifier / TargetMagFactor);
					INT32 iScopePenalty = (INT32)((dScopePenaltyRatio * gGameCTHConstants.AIM_TOO_CLOSE_SCOPE) * (CurrentFactor / 2));

					// Is this scope any better than the ones we've already processed?
					if (iScopePenalty < iBestTotalPenalty)
					{
						// This is the best scope we've found so far. Record it.
						BestFactor = CurrentFactor;
						iBestTotalPenalty = iScopePenalty;
					}
				}
			}
		}

	}

	// Now that we have selected the best available scope, don't use it if we get a penalty and have a functional laser
	if(iBestTotalPenalty < 0 && iProjectionFactor > 1.0f)
		BestFactor = 1.0f;

	return( __max(1.0f, BestFactor) );
}

// HEADROCK HAM 4: This function finds the best projection equipment (laser/reflex) on the weapon for a given range.
FLOAT GetProjectionFactor( OBJECTTYPE * pObj )
{
	FLOAT BestFactor = 1.0;

	if ( pObj->exists() == true && UsingNewCTHSystem() == true ) {
		BestFactor = __max((FLOAT)Item[pObj->usItem].projectionfactor, 1.0f);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) 
		{
			if (iter->exists())
			{
				BestFactor = __max(BestFactor, Item[iter->usItem].projectionfactor);
			}
		}
	}

	return( BestFactor );
}

FLOAT GetScopeRangeMultiplier( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, FLOAT d2DDistance )
{
	FLOAT	iScopeFactor = 0;
	FLOAT	rangeModifier = gGameCTHConstants.SCOPE_RANGE_MULTIPLIER;

	iScopeFactor = GetScopeMagnificationFactor( pObj, d2DDistance );
	if( gGameOptions.fNewTraitSystem )
	{
		if(iScopeFactor > 5.0f)
			rangeModifier -= (NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ) * 0.05f);
		else
			rangeModifier -= (NUM_SKILL_TRAITS( pSoldier, RANGER_NT ) * 0.05f);
	}
	else
		rangeModifier -= (NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT ) * 0.05f);

	return rangeModifier;
}

UINT8 AllowedAimingLevelsNCTH( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	UINT8 aimLevels = 4;
	FLOAT iScopeMagFactor = 0.0, rangeMultiplier = 0.0;
	BOOLEAN allowed = TRUE;
	UINT16 weaponRange;
	UINT8 weaponType;
	BOOLEAN fTwoHanded, fUsingBipod;

	UINT32 uiRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );
	rangeMultiplier = GetScopeRangeMultiplier(pSoldier, &pSoldier->inv[pSoldier->ubAttackingHand], (FLOAT)uiRange);

	// HEADROCK HAM 4: This function has been radically altered AGAIN for the NCTH project.
	// Weapons can now have a tag that defines how many aim clicks they should have. Under the NCTH
	// system, weapons with FEWER aiming clicks are faster to aim without being any less accurate.
	//
	// If the weapon lacks a ubAimLevels tag, the program uses the old HAM 3 algorithm to figure
	// out how much it should have using its class and range.

	// Read from item
	aimLevels = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubAimLevels;
	fTwoHanded = Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].twohanded;
	weaponRange = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usRange + GetRangeBonus(&pSoldier->inv[pSoldier->ubAttackingHand]);
	weaponType = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubWeaponType;
	fUsingBipod = FALSE;

	// If outside limits...
	if (aimLevels < 1 ||
		aimLevels > 8 )
	{
		// Probably uninitialized. Run an algorithm instead.
		
		// Read weapon data
		
		// Define basic (no attachments), and absolute maximums
		if (weaponType == GUN_PISTOL || weaponType == GUN_M_PISTOL || fTwoHanded == 0)
		{
			aimLevels = 2;
 		}
		else if (weaponType == GUN_SHOTGUN || weaponType == GUN_LMG || weaponType == GUN_SMG)
 		{
			aimLevels = 3;
 		}
		else if ((weaponType == GUN_AS_RIFLE || weaponType == GUN_RIFLE ) && weaponRange <= 500)
		{
			aimLevels = 4;
		}
		else if (((weaponType == GUN_AS_RIFLE || weaponType == GUN_RIFLE) && weaponRange > 500) || 
						(weaponType == GUN_SN_RIFLE && weaponRange <= 500))
		{
			aimLevels = 6;
		}
		else if (weaponType == GUN_SN_RIFLE && weaponRange > 500)
		{
			aimLevels = 8;
		}
		else
		{
			aimLevels = 4;
		}
 	}
 
	// HEADROCK HAM 4: This modifier from the weapon and its attachments replaces the generic bipod bonus.
	aimLevels += GetAimLevelsModifier( &pSoldier->inv[pSoldier->ubAttackingHand], gAnimControl[ pSoldier->usAnimState ].ubHeight );

	aimLevels += GetAimLevelsTraitModifier( pSoldier, &pSoldier->inv[pSoldier->ubAttackingHand]);

	aimLevels = __max(1, aimLevels);
	aimLevels = __min(8, aimLevels);

 	return aimLevels;
 }

UINT8 AllowedAimingLevels(SOLDIERTYPE * pSoldier, INT32 sGridNo)
{
	if(UsingNewCTHSystem() == true)
		return AllowedAimingLevelsNCTH(pSoldier, sGridNo);

	// SANDRO was here - changed a few things around

	UINT8 aimLevels = 4;
	UINT16 sScopeBonus = 0;
	BOOLEAN allowed = TRUE;
	UINT8 weaponType;

	INT32 uiRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

	weaponType = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubWeaponType;

	if ( gGameExternalOptions.fAimLevelRestriction ) // Extra aiming on/off 
	{
		// HEADROCK HAM B2.6: Dynamic aiming level restrictions based on gun type and attachments.
		// HEADROCK HAM 3.5: Revamped this - it was illogically constructed.
		if ( gGameExternalOptions.fDynamicAimingTime )
		{
			// SANDRO - throwing knives are a special case, allow two aiming clicks for them
			if( weaponType == NOT_GUN ) 
			{
				if ( Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usItemClass == IC_THROWING_KNIFE )
				{
					aimLevels = 2;
					if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, THROWING_NT ) )
					{
						aimLevels += gSkillTraitValues.ubTHBladesAimClicksAdded;
					}
					return ( min(6, aimLevels) );
				}
				else
				{
					return ( 2 );
				}
			}

			UINT16 weaponRange;
			UINT8 maxAimForType, maxAimWithoutBipod;
			BOOLEAN fTwoHanded, fUsingBipod;
			
			// Read weapon data
			fTwoHanded = Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].twohanded;

			UINT16 usRange = GetModifiedGunRange(pSoldier->inv[pSoldier->ubAttackingHand].usItem);

			weaponRange = usRange + GetRangeBonus(&pSoldier->inv[pSoldier->ubAttackingHand]);
			fUsingBipod = FALSE;

			maxAimWithoutBipod = 4;
			
			// Define basic (no attachments), and absolute maximums
			if (weaponType == GUN_PISTOL || weaponType == GUN_M_PISTOL )
			{
				maxAimForType = 2;
				aimLevels = 1;
				maxAimWithoutBipod = 2;

				// SANDRO - STOMP traits - Gunslinger bonus aim clicks
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) )
				{
					maxAimForType += (gSkillTraitValues.ubGSAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ));
					aimLevels += (gSkillTraitValues.ubGSAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ));
					maxAimWithoutBipod += (gSkillTraitValues.ubGSAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ));
				}
			}
			else if ((weaponType == GUN_SMG && fTwoHanded == 0) || fTwoHanded == 0)
			{
				maxAimForType = 2;
				aimLevels = 1;
				maxAimWithoutBipod = 2;
			}
			else if (weaponType == GUN_SHOTGUN || weaponType == GUN_LMG || (weaponType == GUN_SMG && fTwoHanded == 1))
			{
				maxAimForType = 3;
				aimLevels = 2;
				maxAimWithoutBipod = 3;
			}
			else if ((weaponType == GUN_AS_RIFLE || weaponType == GUN_RIFLE ) && weaponRange <= 500)
			{
				maxAimForType = 4;
				aimLevels = 2;
				maxAimWithoutBipod = 3;

				// SANDRO - STOMP traits - Sniper bonus aim clicks
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) )
				{
					maxAimForType += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
					aimLevels += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
					maxAimWithoutBipod += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
				}
			}
			else if (((weaponType == GUN_AS_RIFLE || weaponType == GUN_RIFLE) && weaponRange > 500) || 
							(weaponType == GUN_SN_RIFLE && weaponRange <= 500))
			{
				maxAimForType = 6;
				aimLevels = 3;
				maxAimWithoutBipod = 4;
				// SANDRO - STOMP traits - Sniper bonus aim clicks
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) )
				{
					maxAimForType += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
					aimLevels += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
					maxAimWithoutBipod += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
				}
			}
			else if (weaponType == GUN_SN_RIFLE && weaponRange > 500)
			{
				maxAimForType = 8;
				aimLevels = 4;
				maxAimWithoutBipod = 3;

				// SANDRO - STOMP traits - Sniper bonus aim clicks
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) )
				{
					maxAimForType += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
					aimLevels += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
					maxAimWithoutBipod += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
				}
			}
			else
			{
				return 4;
			}

			// Determine whether a bipod is being used (prone)
			if (GetBipodBonus(&pSoldier->inv[pSoldier->ubAttackingHand])>0 && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE )
			{
				fUsingBipod = TRUE;
			}


			// don't break compatibility, let the users choose
			if (gGameExternalOptions.iAimLevelsCompatibilityOption != 0)
				sScopeBonus = OldWayOfCalculatingScopeBonus(pSoldier);
			else
				sScopeBonus = gGameExternalOptions.fAimLevelsDependOnDistance ?
					//WarmSteel - Using scope aimbonus instead, as it is used elsewhere like this too.
					//Also, you won't get extra aimclicks anymore if you're too close to use your scope.
					//I've externalized the scope types.
					GetBaseScopeAimBonus( &pSoldier->inv[pSoldier->ubAttackingHand], uiRange )
					: GetMinRangeForAimBonus( &pSoldier->inv[pSoldier->ubAttackingHand]);

				if ( sScopeBonus >= gGameExternalOptions.sVeryHighPowerScope ) 
				{
					aimLevels *= 2;
				}

				else if ( sScopeBonus >= gGameExternalOptions.sHighPowerScope ) 
				{
					aimLevels = (UINT8)((float)(aimLevels+1) * (float)1.5);
				}

				else if ( sScopeBonus >= gGameExternalOptions.sMediumPowerScope ) 
				{
					aimLevels = (UINT8)((float)(aimLevels+1) * (float)1.3);
				}

				// Smaller scopes increase by one.
				else if ( sScopeBonus > 0 )
				{
					aimLevels++;
				}

			// Make sure not over maximum allowed for weapon type.
			if (aimLevels > maxAimForType)
			{
				aimLevels = maxAimForType;
			}
			// Make sure not over maximum allowed without a bipod.
			if (!fUsingBipod)
			{
				aimLevels = __min(aimLevels, maxAimWithoutBipod);
			}
		}
		else // JA2 1.13 Basic aiming restrictions (8 levels for 10x scope, 6 levels for 7x scope)
		{
			OBJECTTYPE* pAttackingWeapon = &pSoldier->inv[pSoldier->ubAttackingHand];
			if ( !IsScoped( pAttackingWeapon ) )
			{
				// No scope. 4 Allowed.
				return (4);
			}
			
			//CHRRISL: yeah, this doesn't work.  GetMinRangeForAimBonus returns a range value in units while GetBaseScopeAimBonus returns a small number.
			//	The result is that if fAimLevelsDependOnDistance is false, all scopes are going to grant +4 aim clicks which is definitely not what
			//	we want to happen.  What we do want is simply to know whether we should send the range or use an extreme range value to guarantee that
			//	the scope is factored.
//			sScopeBonus = gGameExternalOptions.fAimLevelsDependOnDistance ?
//				GetBaseScopeAimBonus( pAttackingWeapon, iRange )
//				: GetMinRangeForAimBonus( pAttackingWeapon );
			if (gGameExternalOptions.iAimLevelsCompatibilityOption != 0)
				sScopeBonus = OldWayOfCalculatingScopeBonus(pSoldier);
			else
				sScopeBonus = gGameExternalOptions.fAimLevelsDependOnDistance ? GetBaseScopeAimBonus( pAttackingWeapon, uiRange ) : GetBaseScopeAimBonus( pAttackingWeapon, 25000 );

			if ( sScopeBonus >= gGameExternalOptions.sVeryHighPowerScope )
			{
				aimLevels += 2;
			}
			if ( sScopeBonus >= gGameExternalOptions.sHighPowerScope )
			{
				aimLevels += 2;
			}
			// SANDRO - STOMP traits - Sniper bonus aim clicks
			if ((weaponType == GUN_AS_RIFLE || weaponType == GUN_RIFLE || weaponType == GUN_SN_RIFLE) &&
				gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) )
			{
				aimLevels += (gSkillTraitValues.ubSNAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ));
			}
			// SANDRO - STOMP traits - Gunslinger bonus aim clicks
			if ((weaponType == GUN_PISTOL || weaponType == GUN_M_PISTOL) &&
				gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) )
			{
				aimLevels += (gSkillTraitValues.ubGSAimClicksAdded * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ));
			}
			//CHRISL: The system can't currently support more then 8 aim levels so make sure we can never have more then 8
			aimLevels = min(8, aimLevels);
		}
	}

	//CHRISL: Make sure we always limit to the proper number of aim clicks
	aimLevels = __max(1, aimLevels);
	aimLevels = __min(8, aimLevels);

	return aimLevels;
}

UINT8 GetAllowedAimingLevelsForItem( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, UINT8 ubStance )
{
	if ( !(Item[pObj->usItem].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE)) )
	{
		return 1;
	}

	INT8 aimLevels = 4;

	// HEADROCK HAM B2.6: Dynamic aiming level restrictions based on gun type and attachments.
	// HEADROCK HAM 3.5: Revamped this - it was illogically constructed.
	if ( gGameExternalOptions.fDynamicAimingTime )
	{
		UINT16 weaponRange;
		UINT8 weaponType;
		BOOLEAN fTwoHanded, fUsingBipod;

		aimLevels = 0;
		
		// Read weapon data
		fTwoHanded = Item[pObj->usItem].twohanded;
		weaponRange = Weapon[Item[pObj->usItem].ubClassIndex].usRange + GetRangeBonus(pObj);
		weaponType = Weapon[Item[pObj->usItem].ubClassIndex].ubWeaponType;
		fUsingBipod = FALSE;
		if(UsingNewCTHSystem() == true)
			aimLevels = Weapon[Item[pObj->usItem].ubClassIndex].ubAimLevels;
		
		// Only use default values if we don't find a weapon specific value.
		if(aimLevels == 0)
		{
			// Define basic (no attachments), and absolute maximums
			if (weaponType == GUN_PISTOL || weaponType == GUN_M_PISTOL || fTwoHanded == 0)
			{
				aimLevels = 2;
			}
			else if (weaponType == GUN_SHOTGUN || weaponType == GUN_LMG || weaponType == GUN_SMG)
			{
				aimLevels = 3;
			}
			else if ((weaponType == GUN_AS_RIFLE || weaponType == GUN_RIFLE ) && weaponRange <= 500)
			{
				aimLevels = 4;
			}
			else if (((weaponType == GUN_AS_RIFLE || weaponType == GUN_RIFLE) && weaponRange > 500) || 
							(weaponType == GUN_SN_RIFLE && weaponRange <= 500))
			{
				aimLevels = 6;
			}
			else if (weaponType == GUN_SN_RIFLE && weaponRange > 500)
			{
				aimLevels = 8;
			}
			else
			{
				return 4;
			}
		}

		// HEADROCK HAM 4: This modifier from the weapon and its attachments replaces the generic bipod bonus.
		aimLevels += GetAimLevelsModifier( pObj, ubStance );

		aimLevels += GetAimLevelsTraitModifier( pSoldier, pObj );

		aimLevels = __max(1, aimLevels);
		aimLevels = __min(8, aimLevels);

	}
	return aimLevels;
}

//Madd: added
INT16 GetStealthBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if ( pObj->exists() == true ) {
		bonus = BonusReduce(Item[pObj->usItem].stealthbonus,(*pObj)[0]->data.objectStatus);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
				bonus += (INT16) BonusReduce(Item[iter->usItem].stealthbonus,(*iter)[0]->data.objectStatus);
		}
	}
	return( bonus );
}
INT16 GetWornStealth( SOLDIERTYPE * pSoldier )
{
	//note: Stealth bonus is capped at 100
	//note: Stealth is not a perk! Stealth bonus only applies to equipment, and stacks with camouflage
	//note: stealth bonus is not affected by terrain like the camo bonus, otherwise they're very similar
	//note: stealth bonus also affects noise made by characters walking
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].exists() == true )
			ttl += GetStealthBonus(&pSoldier->inv[bLoop]);
	}

	// Add some default stealth ability to mercs with STEALTHY trait - SANDRO 
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT ))
		ttl += gSkillTraitValues.ubSTStealthBonus; 

	return __min( ttl, 100 );
}


/////////////////////////////
// HEADROCK: Several functions created for the Enhanced Description Box project, but may generally be useful some
// day. They calculate item bonuses, ammo bonuses and attachment bonuses, without requiring a SOLDIERTYPE or any
// other variable.
////////////////////////////

//WarmSteel - Function to get the total reliability from the gun and its attachments
INT16 GetReliability( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;

	bonus += Item[pObj->usItem].bReliability;

	//Ammo modifications need to be added aswell
	if ( Item[ pObj->usItem ].usItemClass == IC_GUN && (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
	{
		bonus += Item[( *pObj )[0]->data.gun.usGunAmmoItem].bReliability;
	}
	
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
	{
		if(iter->exists())
			bonus += Item[iter->usItem].bReliability;
	}

	return( bonus );
}

// HEADROCK: Function to get the total aim-bonus from the gun and its attachments
INT16 GetFlatAimBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;

	bonus += Item[pObj->usItem].aimbonus;
	
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
	{
		if(iter->exists())
			bonus += Item[iter->usItem].aimbonus;
	}

	return( bonus );
}

// HEADROCK: Function to get the final Loudness value of a gun, after ammo and attachment reductions
INT16 GetFinalLoudness( OBJECTTYPE * pObj )
{
	INT16 loudness = 0;
	INT16 loudnessModifier = 0;

	loudness = Weapon[Item[pObj->usItem].ubClassIndex].ubAttackVolume;

	
	// WANNE: Fix by Headrock
	// It seems that the game calculates noise reduction not by adding together the reduction from ammo and all attachments, 
	// but by applying them to the gun's loudness as percentages one by one. 
	// So instead of (60+20)% it does 60% and then 20%, giving completely different results.
	loudness = ( loudness * GetPercentNoiseVolume( pObj ) ) / 100;
	
	/*
	loudnessModifier += Item[pObj->usItem].percentnoisereduction;
	
	if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
		loudnessModifier += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentnoisereduction ;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) 
		{
			loudnessModifier += BonusReduce( Item[iter->usItem].percentnoisereduction, (*iter)[0]->data.objectStatus ) ;
		}

	loudness = loudness * ( 100 - loudnessModifier ) / 100;
	*/

	loudness = __max(loudness, 1);

	return ( loudness );
}

// HEADROCK: Function to get AP bonus from an item rather than a soldier

INT16 GetAPBonus( OBJECTTYPE * pObj )
{
	INT16 bonus=0;

	bonus += Item[ pObj->usItem ].APBonus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if(iter->exists())
			bonus += Item[iter->usItem].APBonus;
	}

	return( bonus );
}

// HEADROCK: Alternative function to determine flat to-hit bonus of weapon
INT16 GetFlatToHitBonus( OBJECTTYPE * pObj )
{
	INT16 bonus=0;

	bonus = Item[pObj->usItem].tohitbonus;
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].tohitbonus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter){
		if(iter->exists())
			bonus += Item[iter->usItem].tohitbonus;
	}

	return( bonus );
}

// HEADROCK: Function to get average of all "best laser range" attributes from weapon and attachments
INT16 GetAverageBestLaserRange( OBJECTTYPE * pObj )
{
	INT16 bonus=0;
	INT16 numModifiers=0;

	if (Item[pObj->usItem].bestlaserrange > 0)
	{
		numModifiers++;
		bonus += Item[pObj->usItem].bestlaserrange;
	}
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
	{
		if (Item[iter->usItem].bestlaserrange > 0 && iter->exists())
		{
			numModifiers++;
			bonus += Item[iter->usItem].bestlaserrange;
		}
	}

	if (numModifiers>0)
	{
		bonus = bonus / numModifiers;
	}

	return( bonus );
}

// HEADROCK: This function determines the bipod bonii of the gun or its attachments
INT16 GetBipodBonus( OBJECTTYPE * pObj )
{
	INT16 bonus=0;

	bonus = Item[pObj->usItem].bipod;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter){
		if(iter->exists())
			bonus += Item[iter->usItem].bipod;
	}

	return( bonus );
}

// HEADROCK: Added a function to calculate the vision range bonus of an object and its attachments. Takes an argument which determines what type
// of vision range bonus we want to get:
// 0 - Regular
// 1 - Day
// 2 - Night
// 3 - Bright Light
// 4 - Cave
INT16 GetItemVisionRangeBonus( OBJECTTYPE * pObj, INT16 VisionType )
{
	INT16 bonus = 0;

	if (VisionType == 0)
	{
		bonus += Item[ pObj->usItem ].visionrangebonus;
		
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
				bonus += Item[iter->usItem].visionrangebonus;
		}		
	}
	else if (VisionType == 1)
	{
		bonus += Item[ pObj->usItem ].dayvisionrangebonus;
		
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
				bonus += Item[iter->usItem].dayvisionrangebonus;
		}		
	}
	else if (VisionType == 2)
	{
		bonus += Item[ pObj->usItem ].nightvisionrangebonus;
		
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
				bonus += Item[iter->usItem].nightvisionrangebonus;
		}		
	}
	else if (VisionType == 3)
	{
		bonus += Item[ pObj->usItem ].brightlightvisionrangebonus;
		
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
				bonus += Item[iter->usItem].brightlightvisionrangebonus;
		}		
	}
	else if (VisionType == 4)
	{
		bonus += Item[ pObj->usItem ].cavevisionrangebonus;
		
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
				bonus += Item[iter->usItem].cavevisionrangebonus;
		}		
	}

	return( bonus );

}

// HEADROCK: function to get Tunnel Vision percent from an item and its attachments

UINT8 GetItemPercentTunnelVision( OBJECTTYPE * pObj )
{
	UINT8 bonus = 0;

	bonus += Item[ pObj->usItem ].percenttunnelvision;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if(iter->exists())
			bonus += Item[iter->usItem].percenttunnelvision;
	}

	bonus = __min(bonus, 100);

	return( bonus );
}

// HEADROCK: Function to calculate hearing range bonus without SOLDIERTYPE, from an item and its attachments
INT16 GetItemHearingRangeBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;

	bonus += Item[ pObj->usItem ].hearingrangebonus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if(iter->exists())
			bonus += Item[ iter->usItem ].hearingrangebonus;
	}

	return( bonus );

}

// HEADROCK: Flash Suppression Detector function that does not use SOLDIERTYPE. 
BOOLEAN IsFlashSuppressorAlt( OBJECTTYPE * pObj )
{
	if ( AmmoTypes[(*pObj)[0]->data.gun.ubGunAmmoType].tracerEffect )
		return FALSE;

	if ( Item[pObj->usItem].hidemuzzleflash )
		return TRUE;

	if ( Item[(*pObj)[0]->data.gun.usGunAmmoItem].hidemuzzleflash )
		return TRUE;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].hidemuzzleflash && iter->exists() )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

// HEADROCK: Calculate stealth not based on item status
INT16 GetBasicStealthBonus( OBJECTTYPE * pObj )
{
	INT16 bonus = 0;
	if ( pObj->exists() == true ) {
		bonus = Item[pObj->usItem].stealthbonus;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if(iter->exists())
				bonus += (INT16) Item[iter->usItem].stealthbonus;
		}
	}
	return( bonus );
}

// HEADROCK HAM 4: Calculate a gun's accuracy, including bonuses from ammo and attachments.
INT32 GetGunAccuracy( OBJECTTYPE *pObj )
{
	if(UsingNewCTHSystem() == false)
		return(Weapon[pObj->usItem].bAccuracy);

	INT32 bonus = 0;
	if ( pObj->exists() == true )
	{
		bonus = Weapon[Item[pObj->usItem].uiIndex].nAccuracy;
		bonus = (bonus * (*pObj)[0]->data.gun.bGunStatus) / 100;

		INT32 iModifier = GetAccuracyModifier( pObj );
		// Accuracy works in a very different way from most modifiers. At low levels, a small change is almost completely
		// irrelevant. At high levels, every point of accuracy can potentially increase the gun's effective range by
		// a large amount. Therefore, we apply this percentage in REVERSE - the higher our accuracy, the less change
		// we receive.
		// You can look at it a different way: We're actually adding/subtracting a percentage of the distance between
		// the gun's accuracy and Max Accuracy (100).
		// Examples:
		// Modifier = +20%
		// Initial Gun Accuracy = 90, Final Gun Accuracy = 90 + (20% of the gap = 20% of 100-90 = 20% of 10 = 2) = 92.
		// Initial Gun Accuracy = 10, Final Gun Accuracy = 10 + (20% of the gap = 20% of 100-10 = 20% of 90 = 18) = 28.
		bonus += ((100-bonus) * iModifier) / 100;
	}

	bonus = __max(0,bonus);
	bonus = __min(100,bonus);

	return( bonus );
}

// Get Accuracy Modifier from an item and its attachments
INT32 GetAccuracyModifier( OBJECTTYPE *pObj )
{
	INT32 bonus = 0;
	if ( pObj->exists() == true && UsingNewCTHSystem() == true )
	{
		bonus += Item[ pObj->usItem ].percentaccuracymodifier;

		if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
			bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentaccuracymodifier;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
		{
			if (iter->exists())
			{
				bonus += BonusReduceMore( (INT32) Item[iter->usItem].percentaccuracymodifier, (*iter)[0]->data.objectStatus );
			}
		}
	}

	return bonus;
}

// HEADROCK HAM 3.6: This is meant to squash an exploit where a backpack can be moved to your hand to avoid AP penalties.
// CHRISL: Carrying an empty backpack in a none BACKPACKPOS location shoudln't be an issue.
INT8 FindBackpackOnSoldier( SOLDIERTYPE * pSoldier )
{
	INT8	bLoop;

	for (bLoop = 0; bLoop < NUM_INV_SLOTS ; bLoop++)
	{
		if (pSoldier->inv[bLoop].exists())
		{
			if( bLoop == BPACKPOCKPOS )
				return( bLoop );
			if (Item[pSoldier->inv[bLoop].usItem].usItemClass == IC_LBEGEAR &&
				LoadBearingEquipment[Item[pSoldier->inv[bLoop].usItem].ubClassIndex].lbeClass == BACKPACK)
			{
				for (INT8 bLoop2 = 0; bLoop2 < pSoldier->inv[bLoop].ubNumberOfObjects; bLoop2++)
				{
					if(pSoldier->inv[bLoop].IsActiveLBE(bLoop2) == true)
						return( bLoop );
				}
			}
		}
	}
	return( ITEM_NOT_FOUND );
}

// HEADROCK HAM 3.6: This applies the INI modifier to explosives
UINT8 GetModifiedExplosiveDamage( UINT16 sDamage )
{
	if (sDamage == 0)
	{
	   return(0);
	}

	sDamage = (INT16)(( sDamage * gGameExternalOptions.iExplosivesDamageModifier ) / 100);
	sDamage = __max(1, sDamage);
	sDamage = __min(255, sDamage);

	return (UINT8)sDamage;
}

UINT8 GetModifiedMeleeDamage( UINT16 sDamage )
{
	if (sDamage == 0)
	{
	   return(0);
	}

	sDamage = (INT16)(( sDamage * gGameExternalOptions.iMeleeDamageModifier ) / 100);
	sDamage = __max(1, sDamage);
	sDamage = __min(255, sDamage);

	return (UINT8)sDamage;
}

UINT8 GetModifiedGunDamage( UINT16 sDamage )
{
	if (sDamage == 0)
	{
	   return(0);
	}

	sDamage = (INT16)(( sDamage * gGameExternalOptions.iGunDamageModifier ) / 100);
	sDamage = __max(1, sDamage);
	sDamage = __min(255, sDamage);

	return (UINT8)sDamage;
}

UINT16 GetModifiedGunRange(UINT16 usWeaponIndex)
{
	UINT16 ubRange = Weapon[usWeaponIndex].usRange;

	if (ubRange == 0)
	{
	   return(0);
	}

	// Only apply range modifier on "real" guns!
	if (Item[Weapon[usWeaponIndex].uiIndex].usItemClass == IC_GUN ||
		Item[Weapon[usWeaponIndex].uiIndex].usItemClass == IC_LAUNCHER)
	{
		ubRange = (INT16)(( ubRange * gGameExternalOptions.iGunRangeModifier ) / 100);
	}

	return (UINT16)ubRange;
}



/////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - Added a procedure to reduce camo on soldier, used when applying camo kit
INT16 ReduceCamoFromSoldier( SOLDIERTYPE * pSoldier, INT16 iCamoToRemove, INT16 iCamoToSkip )
{
	INT32 i;
	UINT16 iCamoToRemovePart = 3;

	if( iCamoToSkip > 0 ) // iCamoToSkip determines what camo type should not be reduced
		iCamoToRemovePart = 2;

	if ( (pSoldier->bCamo == 0) && (iCamoToSkip != 1) )
		iCamoToRemovePart -= 1;
	if ( (pSoldier->urbanCamo == 0) && (iCamoToSkip != 2) )
		iCamoToRemovePart -= 1;
	if ( (pSoldier->desertCamo == 0) && (iCamoToSkip != 3) )
		iCamoToRemovePart -= 1;
	if ( (pSoldier->snowCamo == 0) && (iCamoToSkip != 4) )
		iCamoToRemovePart -= 1;

	// this should never happen, but if, we still might try to go through the procedure below
	if ( iCamoToRemovePart < 0 )
		iCamoToRemovePart = 0;

	for (i = 0; i < 4; i++ ) // 4 times should be enough, a little paranoya here
	{
		// first, try to reduce jungle camo
		if ( ((iCamoToRemove / (1 + iCamoToRemovePart)) <= pSoldier->bCamo) && (pSoldier->bCamo > 0) && (iCamoToSkip != 1) )
		{
			// jungle camo enough to reduce
			pSoldier->bCamo -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemove -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemovePart = max( 0,(iCamoToRemovePart - 1));
			if( iCamoToRemove <= 0 )
				break;
		}
		else if ((pSoldier->bCamo > 0) && (iCamoToSkip != 1))
		{
			// jungle camo not enough to reduce by intended value, reduce only by what we have
			iCamoToRemove -= pSoldier->bCamo;
			pSoldier->bCamo = 0;
			iCamoToRemovePart = max( 0,(iCamoToRemovePart - 1));
			if( iCamoToRemove <= 0 )
				break;
		}
		// second, try to reduce urban camo
		if ( ((iCamoToRemove / (1 + iCamoToRemovePart)) <= pSoldier->urbanCamo) && (pSoldier->urbanCamo > 0) && (iCamoToSkip != 2) )
		{
			// urban camo enough to reduce
			pSoldier->urbanCamo -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemove -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemovePart = max( 0,(iCamoToRemovePart - 1));
			if( iCamoToRemove <= 0 )
				break;
		}
		else if ((pSoldier->urbanCamo > 0) && (iCamoToSkip != 2))
		{
			// urban camo not enough to reduce by intended value, reduce only by what we have
			iCamoToRemove -= pSoldier->urbanCamo;
			pSoldier->urbanCamo = 0;
			iCamoToRemovePart = max( 0,(iCamoToRemovePart - 1));
			if( iCamoToRemove <= 0 )
				break;
		}
		// third, try to reduce desert camo
		if ( ((iCamoToRemove / (1 + iCamoToRemovePart)) <= pSoldier->desertCamo) && (pSoldier->desertCamo > 0) && (iCamoToSkip != 3) )
		{
			// desert camo enough to reduce
			pSoldier->desertCamo -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemove -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemovePart = max( 0,(iCamoToRemovePart - 1));
			if( iCamoToRemove <= 0 )
				break;
		}
		else if ((pSoldier->desertCamo > 0) && (iCamoToSkip != 3))
		{
			// desert camo not enough to reduce by intended value, reduce only by what we have
			iCamoToRemove -= pSoldier->desertCamo;
			pSoldier->desertCamo = 0;
			iCamoToRemovePart = max( 0,(iCamoToRemovePart - 1));
			if( iCamoToRemove <= 0 )
				break;
		}
		// fourth, try to reduce snow camo
		if ( ((iCamoToRemove / (1 + iCamoToRemovePart)) <= pSoldier->snowCamo) && (pSoldier->snowCamo > 0) && (iCamoToSkip != 4) )
		{
			// snow camo enough to reduce
			pSoldier->snowCamo -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemove -= max(1,(iCamoToRemove / (1 + iCamoToRemovePart)));
			iCamoToRemovePart = max( 0,(iCamoToRemovePart - 1));
			if( iCamoToRemove <= 0 )
				break;
		}
		else if ((pSoldier->snowCamo > 0) && (iCamoToSkip != 4))
		{
			// snow camo not enough to reduce by intended value, reduce only by what we have
			iCamoToRemove -= pSoldier->snowCamo;
			pSoldier->snowCamo = 0;
			if( iCamoToRemove <= 0 )
				break;
		}
	}

	// return remaining value or zero
	return( max( 0, iCamoToRemove));
}

// SANDRO - added function to determine if we have Extended Ear on
BOOLEAN HasExtendedEarOn( SOLDIERTYPE * pSoldier )
{
	// optimistically assume, that anything electronic with hearing range bonus serves as extended ear as well
	if ( pSoldier->inv[HEAD1POS].exists() && (pSoldier->inv[HEAD1POS].usItem == EXTENDEDEAR ||
		(Item[pSoldier->inv[HEAD1POS].usItem].hearingrangebonus > 0 && Item[pSoldier->inv[HEAD1POS].usItem].electronic)) )
	{
		return( TRUE );
	}
	else if ( pSoldier->inv[HEAD2POS].exists() && (pSoldier->inv[HEAD2POS].usItem == EXTENDEDEAR ||
		(Item[pSoldier->inv[HEAD2POS].usItem].hearingrangebonus > 0 && Item[pSoldier->inv[HEAD2POS].usItem].electronic)) )
	{
		return( TRUE );
	}
	return( FALSE );
}


BOOLEAN UseTotalMedicalKitPoints( SOLDIERTYPE * pSoldier, UINT16 usPointsToConsume )
{
	OBJECTTYPE * pObj;
	UINT8 ubPocket;
	INT8 bLoop;

	// add up kit points of all medkits
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (ubPocket = HANDPOS; ubPocket < NUM_INV_SLOTS; ubPocket++)
	{
		if ( IsMedicalKitItem( &( pSoldier->inv[ ubPocket ] ) ) )
		{
			pObj = &(pSoldier->inv[ ubPocket ]);
			// start consuming from the last kit in, so we end up with fewer fuller kits rather than
			// lots of half-empty ones.
			for (bLoop = pObj->ubNumberOfObjects - 1; bLoop >= 0; bLoop--)
			{
				if( (usPointsToConsume * (max( 0, (100 - Item[pObj->usItem].percentstatusdrainreduction)))/100) < (*pObj)[bLoop]->data.objectStatus )
				{
					(*pObj)[bLoop]->data.objectStatus -= (INT8)(usPointsToConsume * (max( 0, (100 - Item[pObj->usItem].percentstatusdrainreduction) ) )/100);
					usPointsToConsume = 0;
					break;
				}
				else
				{
					// consume this kit totally
					usPointsToConsume -= (((*pObj)[bLoop]->data.objectStatus) / (max( 0, (100 - Item[pObj->usItem].percentstatusdrainreduction))) /100);
					(*pObj)[bLoop]->data.objectStatus = 0;

					pObj->ubNumberOfObjects--;
				}
			}
			// check if pocket/hand emptied..update inventory, then update panel
			if( pObj->exists() == false )
			{
				// Delete object
				DeleteObj( pObj );

				// dirty interface panel
				DirtyMercPanelInterface(  pSoldier, DIRTYLEVEL2 );
			}

			if( usPointsToConsume <= 0 )
				break;
		}
	}

	if (usPointsToConsume > 0)
		return( FALSE );
	else
		return( TRUE );
}

static UINT16 OldWayOfCalculatingScopeBonus(SOLDIERTYPE *pSoldier)
{
	// Yes, this may look stupid, maybe it IS stupid, but this is purely an option
	// to use code that was checked in before.
	// Please, do not trash it again.
	return max(0, GetMinRangeForAimBonus(& pSoldier->inv[pSoldier->ubAttackingHand])
		* gGameExternalOptions.iAimLevelsCompatibilityOption / gGameExternalOptions.ubStraightSightRange);
}
