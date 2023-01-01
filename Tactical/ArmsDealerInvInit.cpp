	#include "Types.h"
	#include "ArmsDealerInvInit.h"
	//#include "Item Types.h"
	#include "Arms Dealer Init.h"
	#include "DEbug.h"
	#include "weapons.h"
	#include "Campaign.h"
	#include "GameSettings.h"
	#include "AIInternals.h"
	#include "LaptopSave.h"
	#include "bobbyr.h"
	#include "Random.h"
	#include "Shopkeeper Interface.h"
	#include "connect.h"
	#include "Rebel Command.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

extern INT8	gbSelectedArmsDealerID;

// CHRISL: Updated this to include IC_LBEGEAR class
// This table controls the order items appear in inventory at BR's and dealers, and which kinds of items are sold used
ITEM_SORT_ENTRY DealerItemSortInfo[ ] =
{
//  item class					weapon class	sold used?
	{ IC_GUN,							HANDGUNCLASS, TRUE	},
	{ IC_GUN,							SHOTGUNCLASS, TRUE	},
	{ IC_GUN,							SMGCLASS,			TRUE	},
	{ IC_GUN,							RIFLECLASS,		TRUE	},
	{ IC_GUN,							MGCLASS,			FALSE	},
	{ IC_LAUNCHER,				NOGUNCLASS,		FALSE	},
	{ IC_AMMO,						NOGUNCLASS,		FALSE	},
	{ IC_GRENADE,					NOGUNCLASS,		FALSE	},
	{ IC_BOMB,						NOGUNCLASS,		FALSE	},
	{ IC_BLADE,						NOGUNCLASS,		FALSE	},
	{ IC_THROWING_KNIFE,	NOGUNCLASS,		FALSE	},
	{ IC_PUNCH,						NOGUNCLASS,		FALSE	},
	{ IC_LBEGEAR,					NOGUNCLASS,		TRUE	},
	{ IC_ARMOUR,					NOGUNCLASS,		TRUE	},
	{ IC_FACE,						NOGUNCLASS,		TRUE	},
	{ IC_MEDKIT,					NOGUNCLASS,		FALSE	},
	{ IC_KIT,							NOGUNCLASS,		FALSE	},
	{ IC_MISC,						NOGUNCLASS,		TRUE	},
	{ IC_THROWN,					NOGUNCLASS,		FALSE	},
	{ IC_KEY,							NOGUNCLASS,		FALSE	},

	// marks end of list
	{ IC_NONE,						NOGUNCLASS,		},
};

//
// Setup the inventory arrays for each of the arms dealers
//
//	The arrays are composed of pairs of numbers
//		The first is the item index
//		The second is the amount of the items the dealer will try to keep in his inventory


//
// Tony ( Weapons only )
//

DEALER_POSSIBLE_INV gTonyInventory[MAXITEMS+1];// =
//{
//	//Rare guns/ammo that Tony will buy although he won't ever sell them
//	{	ROCKET_RIFLE,					0 },
//	{	AUTO_ROCKET_RIFLE,		0 },
//	{ AUTOMAG_III,					0 },
////	{ FLAMETHROWER,					0 },
//
//
//	//Weapons
//	{ GLOCK_17,							1 },		/* Glock 17        */
//	{ GLOCK_18,							1 },		/* Glock 18        */
//	{ BERETTA_92F,					1 },		/* Beretta 92F     */
//	{ BERETTA_93R,					1 },		/* Beretta 93R     */
//	{ SW38,									1 },		/* .38 S&W Special */
//	{ BARRACUDA,						1 },		/* .357 Barracuda  */
//	{ DESERTEAGLE,					1 },		/* .357 DesertEagle*/
//	{ M1911,								1 },		/* .45 M1911			 */
//	{ MP5K,									1 },		/* H&K MP5K      	 */
//	{ MAC10,								1 },		/* .45 MAC-10	     */
//
//	{ THOMPSON,							1 },		/* Thompson M1A1   */
//	{ COMMANDO,							1 },		/* Colt Commando   */
//	{ MP53,									1 },		/* H&K MP53		 		 */
//	{ AKSU74,								1 },		/* AKSU-74         */
//	{ TYPE85,								1 },		/* Type-85         */
//	{ SKS,									1 },		/* SKS             */
//	{ DRAGUNOV,							1 },		/* Dragunov        */
//	{ M24,									1 },		/* M24             */
//	{ AUG,									1 },		/* Steyr AUG       */
//
//	{ G41,									1 },		/* H&K G41         */
//	{ MINI14,								1 },		/* Ruger Mini-14   */
//	{ C7,										1 },		/* C-7             */
//	{ FAMAS,								1 },		/* FA-MAS          */
//	{ AK74,									1 },		/* AK-74           */
//	{ AKM,									1 },		/* AKM             */
//	{ M14,									1 },		/* M-14            */
//	{ G3A3,									1 },		/* H&K G3A3        */
//	{ FNFAL,								1 },		/* FN-FAL          */
//
//	{ MINIMI,								1 },
//	{ RPK74,								1 },
//	{ HK21E,								1 },
//
//	{ M870,									1 },		/* Remington M870  */
//	{ SPAS15,								1 },		/* SPAS-15         */
//
//	{ GLAUNCHER,						1 },		/* grenade launcher*/
//	{ UNDER_GLAUNCHER,			1 },		/* underslung g.l. */
//	{ ROCKET_LAUNCHER,			1 },		/* rocket Launcher */
//	{ RPG7,			1 },		/* rpg 7*/
//	{ MORTAR,								1 },
//
//	// SAP guns
//	{ G11,									1 },
//	{ CAWS,									1 },
//	{ P90,									1 },
//
//	{ DART_GUN,							1 },
//
//	{ M900,							1 },
//	{ M950,							1 },
//	{ M960,							1 },
//	{ MICRO_UZI,							1 },
//	{ ENFIELD,							1 },
//	{ TAR21,							1 },
//	{ PSG,							1 },
//	{ VAL_SILENT,							1 },
//	{ BARRETT,							1 },
//	{ MP5,							1 },
//	{ UMP45,							1 },
//	{ MP5SD,							1 },
//	{ MP5N,							1 },
//	{ FIVE7,							1 },
//	{ P7M8,							1 },
//	{ G36C,							1 },
//	{ G36K,							1 },
//	{ MSG90A1,							1 },
//	{ BENNELLI,							1 },
//	{ AK103,							1 },
//
//	//Ammo
//	{ CLIP9_15,							8 },
//	{ CLIP9_30,							6 },
//	{ CLIP9_15_AP,					3 },		/* CLIP9_15_AP */
//	{ CLIP9_30_AP,				  3 },		/* CLIP9_30_AP */
//	{ CLIP9_15_HP,				  3 },		/* CLIP9_15_HP */
//	{ CLIP9_30_HP,				  3 },		/* CLIP9_30_HP */
//
//	{ CLIP38_6,							10},		/* CLIP38_6 */
//	{ CLIP38_6_AP,				  5 },		/* CLIP38_6_AP */
//	{ CLIP38_6_HP,				  5 },		/* CLIP38_6_HP */
//
//	{ CLIP45_7,							6 },		/* CLIP45_7 */				// 70
//
//	{ CLIP45_30,					  8 },		/* CLIP45_30 */
//	{ CLIP45_7_AP,					3 },		/* CLIP45_7_AP */
//	{ CLIP45_30_AP,					3 },		/* CLIP45_30_AP */
//	{ CLIP45_7_HP,					3 },		/* CLIP45_7_HP */
//	{ CLIP45_30_HP,					3 },		/* CLIP45_30_HP */
//
//	{ CLIP357_6,					  6 },		/* CLIP357_6 */
//	{ CLIP357_9,					  5 },		/* CLIP357_9 */
//	{ CLIP357_6_AP,				  3 },		/* CLIP357_6_AP */
//	{ CLIP357_9_AP,					3 },		/* CLIP357_9_AP */
//	{ CLIP357_6_HP,					3 },		/* CLIP357_6_HP */			//80
//	{ CLIP357_9_HP,					3 },		/* CLIP357_9_HP */
//
//	{ CLIP545_30_AP,				6 },		/* CLIP545_30_AP */
//	{ CLIP545_30_HP,				3 },		/* CLIP545_30_HP */
//
//	{ CLIP556_30_AP,				6 },		/* CLIP556_30_AP */
//	{ CLIP556_30_HP,				3 },		/* CLIP556_30_HP */
//
//	{ CLIP762W_10_AP,				6 },		/* CLIP762W_10_AP */
//	{ CLIP762W_30_AP,				5 },		/* CLIP762W_30_AP */
//	{ CLIP762W_10_HP,				3 },		/* CLIP762W_10_HP */
//	{ CLIP762W_30_HP,				3 },		/* CLIP762W_30_HP */
//
//	{ CLIP762N_5_AP,				8 },		/* CLIP762N_5_AP */			//90
//	{ CLIP762N_20_AP,				5 },		/* CLIP762N_20_AP */
//	{ CLIP762N_5_HP,				3 },		/* CLIP762N_5_HP */
//	{ CLIP762N_20_HP,				3 },		/* CLIP762N_20_HP */
//
//	{ CLIP47_50_SAP,				5 },		/* CLIP47_50_SAP */
//
//	{ CLIP57_50_AP,					6 },		/* CLIP57_50_AP */
//	{ CLIP57_50_HP,					3 },		/* CLIP57_50_HP */
//
//	{ CLIP12G_7,						9 },		/* CLIP12G_7 */
//	{ CLIP12G_7_BUCKSHOT,   9 },		/* CLIP12G_7_BUCKSHOT */
//
//	{ CLIPCAWS_10_SAP,			5 },		/* CLIPCAWS_10_SAP */
//	{ CLIPCAWS_10_FLECH,		3 },		/* CLIPCAWS_10_FLECH */			//100
//
//	{ CLIPROCKET_AP,				3 },
//	{ CLIPROCKET_HE,				1 },
//	{ CLIPROCKET_HEAT,			1 },
//
//	{ CLIPDART_SLEEP,				5	},
//
//	{ CLIP50_11,				3	},
//	{ CLIP9H_20,				3	},
//	{ CLIP9_50,				4	},
//	{ CLIP9_50_AP,				3	},
//	{ CLIP9_50_HP,				2	},
//	{ DRUM545_75_AP,				3},
//	{ DRUM545_75_HP,				2	},
//	{ BELT556_200_AP,				3},
//	{ BELT556_200_HP,				2	},
//	{ BELT762N_100_AP,				3},
//	{ BELT762N_100_HP,				2	},
//	{ CLIP57_20_AP,				6	},
//	{ CLIP57_20_HP,				4	},
////	{ CLIPFLAME,						5	},
//
//	// "launchables" (New! From McCain!) - these are basically ammo
//	{ GL_HE_GRENADE,				2 },
//	{ GL_TEARGAS_GRENADE,		2 },
//	{ GL_STUN_GRENADE,			2 },
//	{ GL_SMOKE_GRENADE,			2 },
//	{	MORTAR_SHELL,					1 },
//
//	// knives
//	{	COMBAT_KNIFE,					3 },
//	{	THROWING_KNIFE,				6 },
//	{	BRASS_KNUCKLES,				1 },
//	{	MACHETE,							1 },
//
//	// attachments
//	{ SILENCER,							3 },
//	{ SNIPERSCOPE,					3 },
//	{ LASERSCOPE,						2 },
//	{ BIPOD,								3 },
//	{ DUCKBILL,							3 },
//	{ FLASH_SUPPRESSOR,					3 },
//	{ REFLEX_SCOPED,					3 },
//	{ REFLEX_UNSCOPED,					3 },
//
///*
//	// grenades
//	{ STUN_GRENADE,					5 },
//	{ TEARGAS_GRENADE,			5 },
//	{ MUSTARD_GRENADE,			5 },
//	{ MINI_GRENADE,					5 },
//	{ HAND_GRENADE,					5 },
//	{ SMOKE_GRENADE,				5 },
//*/
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Devin		( Explosives )
//
//Betty UB
#ifdef JA2UB
DEALER_POSSIBLE_INV gBettyInventory[MAXITEMS+1];// =
#else
DEALER_POSSIBLE_INV gDevinInventory[MAXITEMS+1];// =
#endif
//{
//	{	STUN_GRENADE,							3 },
//	{	TEARGAS_GRENADE,					3 },
//	{	MUSTARD_GRENADE,					2 },
//	{	MINI_GRENADE,							3 },
//	{	HAND_GRENADE,							2 },
//	{ SMOKE_GRENADE,						3 },
//
//	{	GL_HE_GRENADE,						2 },
//	{	GL_TEARGAS_GRENADE,				2 },
//	{	GL_STUN_GRENADE,					2 },
//	{	GL_SMOKE_GRENADE,					2 },
//	{	MORTAR_SHELL,							1 },
//	{	RPG_HE_ROCKET,							2 },
//	{	RPG_AP_ROCKET,							2 },
//	{	RPG_FRAG_ROCKET,						2 },
//
//	{	CLIPROCKET_AP,						1 },
//	{ CLIPROCKET_HE,						1 },
//	{ CLIPROCKET_HEAT,					1 },
//
//	{ DETONATOR,								10},
//	{ REMDETONATOR,							5 },
//	{ REMOTEBOMBTRIGGER,				5 },
//
//	{	MINE,											6 },
//	{	RDX,											5 },
//	{	TNT,											5 },
//	{	C1,												4 },
//	{	HMX,											3 },
//	{	C4,												2 },
//
//	{	SHAPED_CHARGE,						5 },
//
////	{	TRIP_FLARE,								2 },
////	{	TRIP_KLAXON,							2 },
//
//	{ GLAUNCHER,								1 },		/* grenade launcher*/
//	{ UNDER_GLAUNCHER,					1 },		/* underslung g.l. */
//	{ ROCKET_LAUNCHER,					1 },		/* rocket Launcher */
//	{ RPG7,					1 },		/* rpg 7 */
//	{ MORTAR,										1 },
//
//	{	METALDETECTOR,						2 },
//	{	WIRECUTTERS,							1 },
//	{	DUCT_TAPE,								1 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Franz	(Expensive pawn shop )
//
DEALER_POSSIBLE_INV gFranzInventory[MAXITEMS+1];// =
//{
//	{ NIGHTGOGGLES,							3 },
//
//	{ LASERSCOPE,								3 },
//	{ METALDETECTOR,						2 },
//	{ EXTENDEDEAR,							2 },
//
//	{	DART_GUN,									1 },
//
//	{ KEVLAR_VEST,							1	},
//	{ KEVLAR_LEGGINGS,					1 },
//	{ KEVLAR_HELMET,						1	},
//	{ KEVLAR2_VEST,							1 },
//	{	SPECTRA_VEST,							1 },
//	{	SPECTRA_LEGGINGS,					1 },
//	{	SPECTRA_HELMET,						1 },
//
//	{ CERAMIC_PLATES,						1 },
//
//	{	CAMOUFLAGEKIT,						1 },
//
//	{ VIDEO_CAMERA,							1 },		// for robot quest
//
//	{ LAME_BOY,									1 },
//	{ FUMBLE_PAK,								1 },
//
//	{ GOLDWATCH,								1 },
//	{ GOLFCLUBS,								1 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Keith		( Cheap Pawn Shop )
//
DEALER_POSSIBLE_INV gKeithInventory[MAXITEMS+1];// =
//{
//	{ FIRSTAIDKIT,			5 },
//
//	// WARNING: Keith must not carry any guns, it would conflict with his story/quest
//
//	{	COMBAT_KNIFE,			2 },
//	{ THROWING_KNIFE,		3 },
//	{	BRASS_KNUCKLES,		1 },
//	{	MACHETE,					1 },
//
//	{ SUNGOGGLES,				3 },
//	{ FLAK_JACKET,			2	},
//	{ STEEL_HELMET,			3 },
//	{ LEATHER_JACKET,		1 },
//
//	{ CANTEEN,					5 },
//	{ CROWBAR,					1 },
//	{ JAR,							6 },
//
//	{	TOOLKIT,					1 },
//	{	GASMASK,					1 },
//
//	{ SILVER_PLATTER,		1 },
//
//	{ WALKMAN,					1 },
//	{ PORTABLETV,				1 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Sam		( Hardware )
//
DEALER_POSSIBLE_INV gSamInventory[MAXITEMS+1];// =
//{
//	{ FIRSTAIDKIT,			3 },
//
//	{ LOCKSMITHKIT,			4 },
//	{ TOOLKIT,					3 },
//
//	{ CANTEEN,					5 },
//
//	{ CROWBAR,					3 },
//	{ WIRECUTTERS,			3 },
//
//	{ DUCKBILL,					3 },
//	{ JAR,							12},
//	{	BREAK_LIGHT,			12},		// flares
//
//	{	METALDETECTOR,		1 },
//
//	{ VIDEO_CAMERA,			1 },
//
//	{ QUICK_GLUE,				3 },
//	{ COPPER_WIRE,			5 },
//	{ BATTERIES,				10 },
//
//	{ CLIP9_15,					5 },
//	{ CLIP9_30,					5 },
//	{ CLIP38_6,					5 },
//	{ CLIP45_7,					5 },
//	{ CLIP45_30,				5 },
//	{ CLIP357_6,				5 },
//	{ CLIP357_9,				5 },
//	{ CLIP12G_7,					9 },
//	{ CLIP12G_7_BUCKSHOT,  9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Jake			( Junk )
//
DEALER_POSSIBLE_INV gJakeInventory[MAXITEMS+1];// =
//{
//	{ FIRSTAIDKIT,			4 },
//	{ MEDICKIT,					3 },
//
//	{ SW38,							1 },
//	{ CLIP38_6,					5 },
//
//	{ JAR,							3 },
//	{ CANTEEN,					2 },
//	{ BEER,							6 },
//
//	{ CROWBAR,					1 },
//	{ WIRECUTTERS,			1 },
//
//	{ COMBAT_KNIFE,			1 },
//	{ THROWING_KNIFE,		1 },
//	{	BRASS_KNUCKLES,		1 },
//	{ MACHETE,					1 },
//
//	{	BREAK_LIGHT,			5 },		// flares
//
//	{	BIPOD,						1 },
//
//	{ TSHIRT,						6 },
//	{ CIGARS,						3 },
//	{ PORNOS,						1 },
//
//	{ LOCKSMITHKIT,			1 },
//
//	// "new" items, presumed unsafe for demo
//	{ TSHIRT_DEIDRANNA,	2 },
//	{	XRAY_BULB,				1 },
//
//	// additional stuff possible in real game
//	{ GLOCK_17,					1 },		/* Glock 17        */
//	{ GLOCK_18,					1 },		/* Glock 18        */
//	{ BERETTA_92F,			1 },		/* Beretta 92F     */
//	{ BERETTA_93R,			1 },		/* Beretta 93R     */
//	{ BARRACUDA,				1 },		/* .357 Barracuda  */
//	{ DESERTEAGLE,			1 },		/* .357 DesertEagle*/
//	{ M1911,						1 },		/* .45 M1911			 */
//
//	{ DISCARDED_LAW,		1 },
//
//	{ STEEL_HELMET,			1 },
//
//	{ TOOLKIT,					1 },
//
//	{ WINE,							1 },
//	{ ALCOHOL,					1 },
//
//	{ GOLDWATCH,				1 },
//	{ GOLFCLUBS,				1 },
//	{ WALKMAN,					1 },
//	{ PORTABLETV,				1 },
//
//	// stuff a real pawn shop wouldn't have, but it does make him a bit more useful
//	{ COMPOUND18,				1 },
//	{ CERAMIC_PLATES,		1 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};
//

//
// Howard		( Pharmaceuticals )
//
DEALER_POSSIBLE_INV gHowardInventory[MAXITEMS+1];// =
//{
//	{ FIRSTAIDKIT,				10},
//	{ MEDICKIT,						5 },
//	{ ADRENALINE_BOOSTER,	5 },
//	{ REGEN_BOOSTER,			5 },
//
//	{ ALCOHOL,						3 },
//	{	COMBAT_KNIFE,				2 },
//
//	{	CLIPDART_SLEEP,			5 },
//
//	{	CHEWING_GUM,				3 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Gabby			( Creature parts and Blood )
//
DEALER_POSSIBLE_INV gGabbyInventory[MAXITEMS+1];// =
//{
//	{ JAR,											12 },
//	{ JAR_ELIXIR,								3 },
//	// buys these, but can't supply them (player is the only source)
//	{	JAR_CREATURE_BLOOD,				0 },
//	{ JAR_QUEEN_CREATURE_BLOOD, 0 },
//	{ BLOODCAT_CLAWS,						0 },
//	{ BLOODCAT_TEETH,						0 },
//	{ BLOODCAT_PELT,						0 },
//	{ CREATURE_PART_CLAWS,			0 },
//	{ CREATURE_PART_FLESH,			0 },
//	{ CREATURE_PART_ORGAN,			0 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Frank  ( Alcohol )
//
DEALER_POSSIBLE_INV gFrankInventory[MAXITEMS+1];// =
//{
//	{ BEER,							12 },
//	{ WINE,							6 },
//	{ ALCOHOL,					9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Elgin  ( Alcohol )
//
DEALER_POSSIBLE_INV gElginInventory[MAXITEMS+1];// =
//{
//	{ BEER,							12 },
//	{ WINE,							6 },
//	{ ALCOHOL,					9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Manny  ( Alcohol )
//
DEALER_POSSIBLE_INV gMannyInventory[MAXITEMS+1];// =
//{
//	{ BEER,							12 },
//	{ WINE,							6 },
//	{ ALCOHOL,					9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Herve Santos		( Alcohol )
//
DEALER_POSSIBLE_INV gHerveInventory[MAXITEMS+1];// =
//{
//	{ BEER,							12 },
//	{ WINE,							6 },
//	{ ALCOHOL,					9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Peter Santos ( Alcohol )
//
DEALER_POSSIBLE_INV gPeterInventory[MAXITEMS+1];// =
//{
//	{ BEER,							12 },
//	{ WINE,							6 },
//	{ ALCOHOL,					9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Alberto Santos		( Alcohol )
//
DEALER_POSSIBLE_INV gAlbertoInventory[MAXITEMS+1];// =
//{
//	{ BEER,							12 },
//	{ WINE,							6 },
//	{ ALCOHOL,					9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Carlo Santos		( Alcohol )
//
DEALER_POSSIBLE_INV gCarloInventory[MAXITEMS+1];// =
//{
//	{ BEER,							12 },
//	{ WINE,							6 },
//	{ ALCOHOL,					9 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Micky	( BUYS Animal / Creature parts )
//

DEALER_POSSIBLE_INV gMickyInventory[MAXITEMS+1];// =
//{
//	// ONLY BUYS THIS STUFF, DOESN'T SELL IT
//	{ BLOODCAT_CLAWS,	0 },
//	{ BLOODCAT_TEETH,	0 },
//	{ BLOODCAT_PELT,		0 },
//	{ CREATURE_PART_CLAWS,	0 },
//	{ CREATURE_PART_FLESH,	0 },
//	{ CREATURE_PART_ORGAN,	0 },
//	{ JAR_QUEEN_CREATURE_BLOOD, 0 },
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};


//
// Arnie		( Weapons REPAIR )
//
DEALER_POSSIBLE_INV gArnieInventory[MAXITEMS+1];// =
//{
//	// NO INVENTORY
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};

//
// Perko			( REPAIR)
//
#ifdef JA2UB
DEALER_POSSIBLE_INV gRaulInventory[MAXITEMS+1];
#else
DEALER_POSSIBLE_INV gPerkoInventory[MAXITEMS+1];// =
#endif
//{
//	// NO INVENTORY
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};

//
// Fredo			( Electronics REPAIR)
//
DEALER_POSSIBLE_INV gFredoInventory[MAXITEMS+1];// =
//{
//	// NO INVENTORY
//
//	{ LAST_DEALER_ITEM,	NO_DEALER_ITEM },		//Last One
//};

DEALER_POSSIBLE_INV gTinaInventory[MAXITEMS+1];

DEALER_POSSIBLE_INV gArmsDealerAdditional[ADDITIONAL_ARMS_DEALERS][MAXITEMS+1];

// prototypes

INT8 GetMaxItemAmount( DEALER_POSSIBLE_INV *pInv, UINT16 usItemIndex )
{
	UINT16	usCnt = 0;

	//loop through the array until a the LAST_DEALER_ITEM is hit
	while ( pInv[usCnt].sItemIndex != LAST_DEALER_ITEM )
	{
		//if this item is the one we want
		if ( pInv[usCnt].sItemIndex == usItemIndex )
			return( pInv[usCnt].ubOptimalNumber );

		// move to the next item
		++usCnt;
	}

	return( NO_DEALER_ITEM );
}

INT8 GetDealersMaxItemAmount( UINT8 ubDealerID, UINT16 usItemIndex )
{
	switch( ubDealerID )
	{
	
#ifdef JA2UB
		case ARMS_DEALER_BETTY:
			return( GetMaxItemAmount( gBettyInventory, usItemIndex ) );
			break;
#endif			
  // Ja25: Not in exp.
		case ARMS_DEALER_TONY:
			return( GetMaxItemAmount( gTonyInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_FRANK:
			return( GetMaxItemAmount( gFrankInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_MICKY:
			return( GetMaxItemAmount( gMickyInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_ARNIE:
			return( GetMaxItemAmount( gArnieInventory, usItemIndex ) );
			break;
#ifdef JA2UB
		case ARMS_DEALER_RAUL:
			return( GetMaxItemAmount( gRaulInventory, usItemIndex ) ); //
			break;
#else			
		case ARMS_DEALER_PERKO:
			return( GetMaxItemAmount( gPerkoInventory, usItemIndex ) );
			break;
#endif
		case ARMS_DEALER_KEITH:
			return( GetMaxItemAmount( gKeithInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_BAR_BRO_1:
			return( GetMaxItemAmount( gHerveInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_BAR_BRO_2:
			return( GetMaxItemAmount( gPeterInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_BAR_BRO_3:
			return( GetMaxItemAmount( gAlbertoInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_BAR_BRO_4:
			return( GetMaxItemAmount( gCarloInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_JAKE:
			return( GetMaxItemAmount( gJakeInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_FRANZ:
			return( GetMaxItemAmount( gFranzInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_HOWARD:
			return( GetMaxItemAmount( gHowardInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_SAM:
			return( GetMaxItemAmount( gSamInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_FREDO:
			return( GetMaxItemAmount( gFredoInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_GABBY:
			return( GetMaxItemAmount( gGabbyInventory, usItemIndex ) );
			break;
#ifdef JA2UB
			//no ub
#else
		case ARMS_DEALER_DEVIN:
			return( GetMaxItemAmount( gDevinInventory, usItemIndex ) );
			break;
#endif
		case ARMS_DEALER_ELGIN:
			return( GetMaxItemAmount( gElginInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_MANNY:
			return( GetMaxItemAmount( gMannyInventory, usItemIndex ) );
			break;

		case ARMS_DEALER_TINA:
			return( GetMaxItemAmount( gTinaInventory, usItemIndex ) );
			break;

		default:
			{
				if ( ubDealerID < NUM_ARMS_DEALERS )
				{
					UINT8 additionaldealernumber = ubDealerID - ARMS_DEALER_ADDITIONAL_1;
					return( GetMaxItemAmount( gArmsDealerAdditional[additionaldealernumber], usItemIndex ) );
				}
			}
			Assert( FALSE );
			return( 0 );
			break;
	}
}

DEALER_POSSIBLE_INV *GetPointerToDealersPossibleInventory( UINT16 ubArmsDealerID )
{
	switch( ubArmsDealerID )
	{
	#ifdef JA2UB
		case ARMS_DEALER_BETTY:
			return( gBettyInventory );
			break;
	#endif
		case ARMS_DEALER_TONY:
			return( gTonyInventory );
			break;

		case ARMS_DEALER_FRANK:
			return( gFrankInventory );
			break;

		case ARMS_DEALER_MICKY:
			return( gMickyInventory );
			break;

		case ARMS_DEALER_ARNIE:
			return( gArnieInventory );
			break;

#ifdef JA2UB
		case ARMS_DEALER_RAUL:
			return( gRaulInventory );
			break;
#else
		case ARMS_DEALER_PERKO:
			return( gPerkoInventory );
			break;
#endif
		case ARMS_DEALER_KEITH:
			return( gKeithInventory );
			break;

		case ARMS_DEALER_BAR_BRO_1:
			return( gHerveInventory );
			break;

		case ARMS_DEALER_BAR_BRO_2:
			return( gPeterInventory );
			break;

		case ARMS_DEALER_BAR_BRO_3:
			return( gAlbertoInventory );
			break;

		case ARMS_DEALER_BAR_BRO_4:
			return( gCarloInventory );
			break;

		case ARMS_DEALER_JAKE:
			return( gJakeInventory );
			break;

		case ARMS_DEALER_FRANZ:
			return( gFranzInventory );
			break;

		case ARMS_DEALER_HOWARD:
			return( gHowardInventory );
			break;

		case ARMS_DEALER_SAM:
			return( gSamInventory );
			break;

		case ARMS_DEALER_FREDO:
			return( gFredoInventory );
			break;

		case ARMS_DEALER_GABBY:
			return( gGabbyInventory );
			break;
#ifdef JA2UB
			//no UB
#else
		case ARMS_DEALER_DEVIN:
			return( gDevinInventory );
			break;
#endif			
		case ARMS_DEALER_ELGIN:
			return( gElginInventory );
			break;

		case ARMS_DEALER_MANNY:
			return( gMannyInventory );
			break;

		case ARMS_DEALER_TINA:
			return( gTinaInventory );
			break;

		default:
			{
				if ( ubArmsDealerID < NUM_ARMS_DEALERS )
				{
					UINT16 additionaldealernumber = ubArmsDealerID - ARMS_DEALER_ADDITIONAL_1;
					return( gArmsDealerAdditional[additionaldealernumber] );
				}
			}
			return( NULL );
	}
}

//Madd: added boolean fUsed
UINT8 GetCurrentSuitabilityForItem( INT8 bArmsDealer, UINT16 usItemIndex, BOOLEAN fUsed )
{
	unsigned ubItemCoolness;
	unsigned ubMinCoolness, ubMaxCoolness;

	const int bobbyRaysID = -1;
	
	// item suitability varies with the player's maximum progress through the game.  The farther he gets, the better items
	// we make available.  Weak items become more and more infrequent later in the game, although they never quite vanish.

	// items illegal in this game are unsuitable [this checks guns vs. current GunSet!]
	if (!ItemIsLegal( usItemIndex ))
	{
		return(ITEM_SUITABILITY_NONE);
	}

	// items normally not sold at shops are unsuitable
//	if ( Item[ usItemIndex ].fFlags & ITEM_NOT_BUYABLE )
	if ( Item[ usItemIndex ].notbuyable  )
	{
		return(ITEM_SUITABILITY_NONE);
	}
	
	ubItemCoolness = Item[ usItemIndex ].ubCoolness;

	if (ubItemCoolness == 0)
	{
		// items without a coolness rating can't be sold to the player by shopkeepers
		return(ITEM_SUITABILITY_NONE);
	}

	// the following staple items are always deemed highly suitable regardless of player's progress:
	//switch (usItemIndex)
	//{
	//	case FIRSTAIDKIT:
	//	case MEDICKIT:
	//	case TOOLKIT:
	//	case LOCKSMITHKIT:

	//	case CANTEEN:
	//	case CROWBAR:
	//	case JAR:
	//	case JAR_ELIXIR:
	//	case JAR_CREATURE_BLOOD:
		if ( Item[usItemIndex].medical || Item[usItemIndex].canteen || Item[usItemIndex].medicalkit || Item[usItemIndex].locksmithkit || Item[usItemIndex].toolkit || Item[usItemIndex].crowbar || Item[usItemIndex].jar )
			return(ITEM_SUITABILITY_ALWAYS);
	//}
	
	// If it's not BobbyRay, Tony, or Devin
	if (bArmsDealer != bobbyRaysID && armsDealerInfo[bArmsDealer].allInventoryAlwaysAvailable)
	{
		// all the other dealers have very limited inventories, so their suitability remains constant at all times in game
		return(ITEM_SUITABILITY_HIGH);
	}
	
	// figure out the appropriate range of coolness based on player's maximum progress so far
	
	if ( bArmsDealer == bobbyRaysID ) {
		ubMinCoolness = 1;
	    ubMaxCoolness = HighestPlayerProgressPercentage() / 10 + 1;
	} else {
		ubMinCoolness = HighestPlayerProgressPercentage() * armsDealerInfo[bArmsDealer].coolnessProgressRate / 100;
	    ubMaxCoolness = HighestPlayerProgressPercentage() * armsDealerInfo[bArmsDealer].coolnessProgressRate / 100 + 1;
	}

	//Madd:  Bobby Ray's will sell higher coolness stuff if it's used, and may also have a better selection at the start of the game, depending on selection
	if ( (bArmsDealer == bobbyRaysID || armsDealerInfo[bArmsDealer].useBRSetting) && gGameOptions.ubBobbyRayQuality > BR_GOOD )
	{
		ubMaxCoolness += gGameOptions.ubBobbyRayQuality - 1;
		if (fUsed )
		{
			ubMaxCoolness += 1;
		}
	}

	// WDS - Improve Tony's and Devin's inventory like BR's
	// Tony has the better stuff sooner (than Bobby R's)
	if (bArmsDealer >= 0) {
		ubMinCoolness += armsDealerInfo[bArmsDealer].addToCoolness + RebelCommand::GetMerchantCoolnessBonus();
		ubMaxCoolness += armsDealerInfo[bArmsDealer].addToCoolness + RebelCommand::GetMerchantCoolnessBonus();
		ubMinCoolness = max( armsDealerInfo[bArmsDealer].minCoolness, min( 9, ubMinCoolness ) );
		// silversurfer: max coolness should never be lower than min coolness!
		//ubMaxCoolness = max( 2, min( armsDealerInfo[bArmsDealer].maxCoolness, ubMaxCoolness ) );
		ubMaxCoolness = max( (ubMinCoolness + 1), min( armsDealerInfo[bArmsDealer].maxCoolness, ubMaxCoolness ) );
	}
	/*
	if (bArmsDealer == ARMS_DEALER_TONY)
	{
		ubMinCoolness += 1;
		ubMaxCoolness += 1;

		if (gGameExternalOptions.tonyUsesBRSetting) {
			ubMinCoolness += gGameOptions.ubBobbyRayQuality / 3;
			ubMaxCoolness += gGameOptions.ubBobbyRayQuality;
		}
	}
	else if (bArmsDealer == ARMS_DEALER_DEVIN)
	{
		// almost everything Devin sells is pretty cool (4+), so gotta apply a minimum or he'd have nothing early on
		if ( ubMinCoolness < 3 )
		{
			ubMinCoolness = 3;
			ubMaxCoolness = 4;
		}

		if (gGameExternalOptions.devinUsesBRSetting) {
			ubMinCoolness += gGameOptions.ubBobbyRayQuality / 3;
			ubMaxCoolness += gGameOptions.ubBobbyRayQuality;
		}
	}
    */

	ubMinCoolness = max( 1, min( 9, ubMinCoolness ) );
	ubMaxCoolness = max( 2, min( 10, ubMaxCoolness ) );

	if ( bArmsDealer == bobbyRaysID )
	{
		//Madd: BR's always sells ammo for its guns
		if (Item[usItemIndex].usItemClass == IC_AMMO && ubItemCoolness <= ubMaxCoolness )
		{
			return(ITEM_SUITABILITY_ALWAYS);
		}
	}

	// if item is too cool for current level of progress
	if (ubItemCoolness > ubMaxCoolness)
	{
		return(ITEM_SUITABILITY_NONE);
	}

	// if item is exactly within the current coolness window
	if ((ubItemCoolness >= ubMinCoolness) && (ubItemCoolness <= ubMaxCoolness))
	{
		return(ITEM_SUITABILITY_HIGH);
	}

	// if item is still relatively close to low end of the window
	if ((ubItemCoolness + 2) >= ubMinCoolness)
	{
		return(ITEM_SUITABILITY_MEDIUM);
	}

	// item is way uncool for player's current progress, but it's still possible for it to make an appearance
	return(ITEM_SUITABILITY_LOW);
}

UINT8 ChanceOfItemTransaction( INT8 bArmsDealer, UINT16 usItemIndex, BOOLEAN fDealerIsSelling, BOOLEAN fUsed )
{
	UINT8 ubItemCoolness;
	UINT8 ubChance = 0;
	BOOLEAN fBobbyRay = FALSE;
	
	// make sure dealers don't carry used items that they shouldn't
	if ( fUsed && !fDealerIsSelling && !CanDealerItemBeSoldUsed( usItemIndex ) )
		return( 0 );
	
	if (bArmsDealer == -1)
	{
		// Bobby Ray has an easier time getting resupplied than the local dealers do
		fBobbyRay = TRUE;
	}

	ubItemCoolness = Item[ usItemIndex ].ubCoolness;

	switch (GetCurrentSuitabilityForItem( bArmsDealer, usItemIndex, fUsed ) )
	{
		case ITEM_SUITABILITY_NONE:
			if (fDealerIsSelling)
			{
				// dealer always gets rid of stuff that is too advanced or inappropriate ASAP
				ubChance = 100;
			}
			else // dealer is buying
			{
				// can't get these at all
				ubChance = 0;
			}
			break;

		case ITEM_SUITABILITY_LOW:
			ubChance = (fBobbyRay) ? 25 : 15;
			break;

		case ITEM_SUITABILITY_MEDIUM:
			ubChance = (fBobbyRay) ? 50 : 30;
			break;

		case ITEM_SUITABILITY_HIGH:
			ubChance = (fBobbyRay) ? 75 : 50;
			break;

		case ITEM_SUITABILITY_ALWAYS:
			if (fDealerIsSelling)
			{
				// sells just like suitability high
				ubChance = 75;
			}
			else // dealer is buying
			{
				// dealer can always get a (re)supply of these
				ubChance = 100;
			}
			break;

		default:
			Assert(0);
			break;
	}

	// Madd
	if ( !fDealerIsSelling && fBobbyRay )
	    ubChance = min(100,gGameOptions.ubBobbyRayQuality * ubChance);

	// if there's any uncertainty
	if ((ubChance > 0) && (ubChance < 100))
	{
		// cooler items sell faster
		if (fDealerIsSelling)
		{
			ubChance += (5 * ubItemCoolness);

			// ARM: New - keep stuff on the shelves longer
			ubChance /= 2;

			if ( fBobbyRay )
			    ubChance /= gGameOptions.ubBobbyRayQuality;
		}

		// used items are traded more rarely
		if (fUsed)
		{
			ubChance /= 2;
		}
	}
	
	return(ubChance);
}

BOOLEAN ItemTransactionOccurs( INT8 bArmsDealer, UINT16 usItemIndex, BOOLEAN fDealerIsSelling, BOOLEAN fUsed )
{
	INT16 sInventorySlot;	
	UINT8 ubChance = ChanceOfItemTransaction( bArmsDealer, usItemIndex, fDealerIsSelling, fUsed );

	// if the dealer is buying, and a chance exists (i.e. the item is "eligible")
	if (!fDealerIsSelling && (ubChance > 0))
	{
		// mark it as such
		if (bArmsDealer == -1)
		{
			if (fUsed)
			{
				sInventorySlot = GetInventorySlotForItem(LaptopSaveInfo.BobbyRayUsedInventory, usItemIndex, fUsed);
				LaptopSaveInfo.BobbyRayUsedInventory[ sInventorySlot ].fPreviouslyEligible = TRUE;
			}
			else
			{
				sInventorySlot = GetInventorySlotForItem(LaptopSaveInfo.BobbyRayInventory, usItemIndex, fUsed);
				LaptopSaveInfo.BobbyRayInventory    [ sInventorySlot ].fPreviouslyEligible = TRUE;
			}
		}
		else
		{
			gArmsDealerStatus[ bArmsDealer ].fPreviouslyEligible[ usItemIndex ] = TRUE;
		}
	}

	// roll to see if a transaction occurs
	return Chance( ubChance );
}

UINT8 DetermineInitialInvItems( INT8 bArmsDealerID, UINT16 usItemIndex, UINT8 ubChances, BOOLEAN fUsed)
{
	if ( is_networked )
		return ubChances;

	// initial inventory is now rolled for one item at a time, instead of one type at a time, to improve variety
	UINT8 ubNumBought = 0;

	for ( UINT8 ubCnt = 0; ubCnt < ubChances; ++ubCnt)
	{
		if ( ItemTransactionOccurs( bArmsDealerID, usItemIndex, DEALER_BUYING, fUsed ) )
		{
			++ubNumBought;
		}
	}

	return( ubNumBought );
}

UINT8 HowManyItemsAreSold( INT8 bArmsDealerID, UINT16 usItemIndex, UINT8 ubNumInStock, BOOLEAN fUsed)
{
	// items are now virtually "sold" one at a time
	UINT8 ubNumSold = 0;

	for ( UINT8 ubCnt = 0; ubCnt < ubNumInStock; ++ubCnt)
	{
		if (ItemTransactionOccurs( bArmsDealerID, usItemIndex, DEALER_SELLING, fUsed))
		{
			++ubNumSold;
		}
	}

	return( ubNumSold );
}

UINT8 HowManyItemsToReorder(UINT8 ubWanted, UINT8 ubStillHave)
{
	UINT8 ubNumReordered = 0;
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("HowManyItemsToReorder: wanted = %d, still have = %d",ubWanted, ubStillHave));

	// WANNE: Disabled assertion and fixed possibly problem
	//Assert(ubStillHave <= ubWanted);

	if (ubWanted > ubStillHave)
	{
		ubNumReordered = ubWanted - ubStillHave;

		//randomize the amount. 33% of the time we add to it, 33% we subtract from it, rest leave it alone
		switch (Random(3))
		{
			case 0:
				ubNumReordered += ubNumReordered / 2;
				break;
			case 1:
				ubNumReordered -= ubNumReordered / 2;
				break;
		}
	}

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("HowManyBRItemsToOrder: reordered = %d",ubNumReordered));
	return(ubNumReordered);
}

int BobbyRayItemQsortCompare(const void *pArg1, const void *pArg2)
{
	UINT16 usItem1Index = ( ( STORE_INVENTORY * ) pArg1 )->usItemIndex;
	UINT16 usItem2Index = ( ( STORE_INVENTORY * ) pArg2 )->usItemIndex;

	UINT8 ubItem1Quality = ( ( STORE_INVENTORY * ) pArg1 )->ubItemQuality;
	UINT8 ubItem2Quality = ( ( STORE_INVENTORY * ) pArg2 )->ubItemQuality;

	return( CompareItemsForSorting( usItem1Index, usItem2Index, ubItem1Quality, ubItem2Quality ) );
}

int CompareItemsForSorting( UINT16 usItem1Index, UINT16 usItem2Index, UINT16 ubItem1Quality, UINT16 ubItem2Quality )
{
	UINT8 ubItem1Category = GetDealerItemCategoryNumber( usItem1Index );
	UINT8 ubItem2Category = GetDealerItemCategoryNumber( usItem2Index );

	// lower category first
	if ( ubItem1Category < ubItem2Category )
	{
		return( -1 );
	}
	else if ( ubItem1Category > ubItem2Category )
	{
		return( 1 );
	}

	if ( Item[usItem1Index].usItemClass == IC_GUN && Item[usItem2Index].usItemClass == IC_GUN )
	{
		UINT8 ubItem1WeaponType = Weapon[Item[usItem1Index].ubClassIndex].ubWeaponType;
		UINT8 ubItem2WeaponType = Weapon[Item[usItem2Index].ubClassIndex].ubWeaponType;
		
		if ( ubItem1WeaponType < ubItem2WeaponType )
		{
			return (-1);
		}
		else if ( ubItem1WeaponType > ubItem2WeaponType )
		{
			return (1);
		}
	}

	// the same category
	//if ( Item[ usItem1Index ].usItemClass == IC_AMMO && Item[ usItem2Index ].usItemClass == IC_AMMO )
	//{
		//UINT8		ubItem1Calibre;
		//UINT8		ubItem2Calibre;
		//UINT8		ubItem1MagSize;
		//UINT8		ubItem2MagSize;

		// AMMO is sorted by caliber first
		//ubItem1Calibre = Magazine[ Item[ usItem1Index ].ubClassIndex ].ubCalibre;
		//ubItem2Calibre = Magazine[ Item[ usItem2Index ].ubClassIndex ].ubCalibre;
		//if ( ubItem1Calibre > ubItem2Calibre )
		//{
		//	return( -1 );
		//}
		//else
		//if ( ubItem1Calibre < ubItem2Calibre )
		//{
		//	return( 1 );
		//}
		//// the same caliber - compare size of magazine, then fall out of if statement
		//ubItem1MagSize = Magazine[ Item[ usItem1Index ].ubClassIndex ].ubMagSize;
		//ubItem2MagSize = Magazine[ Item[ usItem2Index ].ubClassIndex ].ubMagSize;
		//if ( ubItem1MagSize > ubItem2MagSize )
		//{
		//	return( -1 );
		//}
		//else
		//if ( ubItem1MagSize < ubItem2MagSize )
		//{
		//	return( 1 );
		//}
	//}
	//else
	//{
		// items other than ammo are compared on coolness first
		//ubItem1Coolness = Item[ usItem1Index ].ubCoolness;
		//ubItem2Coolness = Item[ usItem2Index ].ubCoolness;

		//// higher coolness first
		//if ( ubItem1Coolness > ubItem2Coolness )
		//{
		//	return( -1 );
		//}
		//else
		//if ( ubItem1Coolness < ubItem2Coolness )
		//{
		//	return( 1 );
		//}
	//}

	// the same coolness/caliber - compare base prices then
	//usItem1Price = Item[ usItem1Index ].usPrice;
	//usItem2Price = Item[ usItem2Index ].usPrice;

	//// higher price first
	//if ( usItem1Price > usItem2Price )
	//{
	//	return( -1 );
	//}
	//else
	//if ( usItem1Price < usItem2Price )
	//{
	//	return( 1 );
	//}
	//else
	//{
		// the same price - compare item #s, then

		//// lower index first
		//if ( usItem1Index < usItem2Index )
		//{
		//	return( -1 );
		//}
		//else
		//if ( usItem1Index > usItem2Index )
		//{
		//	return( 1 );
		//}

		//Madd: sort by name (for now at least):
		if (_wcsicmp(Item[usItem1Index].szBRName,Item[usItem2Index].szBRName) < 0 )
			return -1;
		else if (_wcsicmp(Item[usItem1Index].szBRName,Item[usItem2Index].szBRName) > 0 )
			return 1;


		// same item type = compare item quality, then
		// higher quality first
		if ( ubItem1Quality > ubItem2Quality )
		{
			return( -1 );
		}
		else if ( ubItem1Quality < ubItem2Quality )
		{
			return( 1 );
		}
	//}
	// identical items!
	return( 0 );
}

UINT8 GetDealerItemCategoryNumber( UINT16 usItemIndex )
{
	UINT8		ubWeaponClass;
	UINT8		ubCategory	= 0;
	UINT32		uiItemClass	= Item[ usItemIndex ].usItemClass;

	if ( usItemIndex < gMAXITEMS_READ && IsWeapon( usItemIndex ) )
	{
		ubWeaponClass = Weapon[usItemIndex].ubWeaponClass;
	}
	else
	{
		// not a weapon, so no weapon class, this won't be needed
		ubWeaponClass = 0;
	}

	ubCategory = 0;

	// search table until end-of-list marker is encountered
	while ( DealerItemSortInfo[ ubCategory ].uiItemClass != IC_NONE )
	{
		if ( DealerItemSortInfo[ ubCategory ].uiItemClass == uiItemClass )
		{
			// if not a type of gun
			if ( uiItemClass != IC_GUN )
			{
				// then we're found it
				return ( ubCategory );
			}
			else
			{
				// for guns, must also match on weapon class
				if ( DealerItemSortInfo[ ubCategory ].ubWeaponClass == ubWeaponClass )
				{
					// then we're found it
					return ( ubCategory );
				}
			}
		}

		// check vs. next category in the list
		++ubCategory;
	}

	// WANNE: commented the assert out, because we always get the assertion in debug mode!
	// should never be trying to locate an item that's not covered in the table!
	//Assert( FALSE );
	return( 0 );
}

BOOLEAN CanDealerItemBeSoldUsed( UINT16 usItemIndex )
{
//	if ( !( Item[ usItemIndex ].fFlags & ITEM_DAMAGEABLE ) )
	if ( !( Item[ usItemIndex ].damageable  ) )
		return(FALSE);

	// certain items, although they're damagable, shouldn't be sold in a used condition
	return( DealerItemSortInfo[ GetDealerItemCategoryNumber( usItemIndex ) ].fAllowUsed );
}
