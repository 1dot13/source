#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "Types.h"
	#include "Store Inventory.h"
	#include "Random.h"
	#include "Weapons.h"
	#include "Debug.h"
	#include "LaptopSave.h"
	#include "ShopKeeper Interface.h"
	#include "armsdealerinvinit.h"
	#include "GameSettings.h"
#endif


UINT8 StoreInventory[MAXITEMS][BOBBY_RAY_LISTS]; //=
//{
////
//// The first column is for Bobby Rays new inventory,					BOBBY_RAY_NEW,
//// The second column is for Bobby Rays used inventory,				BOBBY_RAY_USED,
////
//	{0,   0 },		/* nothing */
//// MADD MARKER
////---WEAPONS---
//	{8,		1 },		/* Glock 17        */	
//	{6,		1 },		/* Glock 18        */	
//	{8,		1 },		/* Beretta 92F     */	
//	{6,		1 },		/* Beretta 93R     */	
//	{8,		1 },		/* .38 S&W Special */	
//	{8,		1 },		/* .357 Barracuda  */	
//	{6,		1 },		/* .357 DesertEagle*/ 
//	{7,		1 },		/* .45 M1911			 */ 
//	{4,		1 },		/* H&K MP5K      	 */	
//	{4,		1 },		/* .45 MAC-10	     */			// 10
//
//	{4,		1 },		/* Thompson M1A1   */	
//	{4,		1 },		/* Colt Commando   */	
//	{4,		1 },		/* H&K MP53		 		 */	
//	{4,		1 },		/* AKSU-74         */ 
//	{2,		1 },		/* 5.7mm FN P90    */ 
//	{4,		1 },		/* Type-85         */ 
//	{3,		1 },		/* SKS             */ 
//	{2,		1 },		/* Dragunov        */ 
//	{2,		1 },		/* M24             */ 
//	{3,		1 },		/* Steyr AUG       */			//20
//
//	{3,		1 },		/* H&K G41         */ 
//	{3,		1 },		/* Ruger Mini-14   */ 
//	{3,		1 },		/* C-7             */ 
//	{3,		1 },		/* FA-MAS          */ 
//	{3,		1 },		/* AK-74           */ 
//	{3,		1 },		/* AKM             */ 
//	{3,		1 },		/* M-14            */ 
//	{3,		1 },		/* FN-FAL          */	
//	{3,		1 },		/* H&K G3A3        */ 
//	{3,		1 },		/* H&K G11         */			// 30
//
//	{5,		1 },		/* Remington M870  */	
//	{3,		1 },		/* SPAS-15         */ 
//	{2,		1 },		/* CAWS            */
//	{2,		1 },		/* FN Minimi       */
//	{2,		1 },		/* RPK-74          */
//	{2,		1 },		/* H&K 21E         */	
//	{5,		0 },		/* combat knife    */
//	{7,		0 },		/* throwing knife  */
//	{0,		0 },		/* rock            */
//	{2,		0 },		/* grenade launcher*/		//	40
//
//	{2,		0 },		/* mortar */
//	{0,		0 },		/* another rock    */
//	{0,		0 },		/* claws */			
//	{0,		0 },		/* claws */
//	{0,		0 },		/* claws */
//	{0,		0 },		/* claws */
//	{0,		0 },		/* tentacles */
//	{0,		0 },		/* spit  */
//	{2,		0 },		/* brass knuckles */
//	{6,		0 },		/* underslung g.l. */				// 50
//
//	{4,		0 },		/* rocket launcher */
//	{0,		0 },		/* bloodcat claws */
//	{0,		0 },		/* bloodcat bite */
//	{2,		0 },		/* machete */
//	{1,		0 },		/* rocket rifle */
//	{0,		0 },		/* Automag III */
//	{0,		0 },		/* spit */
//	{0,		0 },		/* spit */
//	{0,		0 },		/* spit */
//	{0,		0 },		/* tank cannon */			// 60
//
//	{2,		1 },		/* dart gun */
//	{0,		0 },		/* bloody throwing knife */
//	{0,		0 },		/* flamethrower */
//	{3,		0 },		/* Crowbar       */	
//	{1,		0 },		/* Auto Rocket Rifle */
//	{2,		1 },		/* barrett - nothing */
//	{2,		1 },		/* val silent - nothing */
//	{2,		1 },		/* psg - nothing */
//	{2,		1 },		/* tar 21 - nothing */
//	{0,		0 },		/* nothing */			// 70
//
////---AMMO---
//	{50,	0 },		/* CLIP9_15 */
//	{40,	0 },		/* CLIP9_30 */
//	{8,		0 },		/* CLIP9_15_AP */			
//	{4,		0 },		/* CLIP9_30_AP */	
//	{7,		0 },		/* CLIP9_15_HP */	
//	{4,		0 },		/* CLIP9_30_HP */	
//	{50,	0 },		/* CLIP38_6 */
//	{8,		0 },		/* CLIP38_6_AP */
//	{6,		0 },		/* CLIP38_6_HP */
//	{40,	0 },		/* CLIP45_7 */				// 80
//
//	{25,	0 },		/* CLIP45_30 */
//	{6,		0 },		/* CLIP45_7_AP */		
//	{8,		0 },		/* CLIP45_30_AP */	
//	{6,		0 },		/* CLIP45_7_HP */		
//	{5,		0 },		/* CLIP45_30_HP */	
//	{40,	0 },		/* CLIP357_6 */			
//	{20,	0 },		/* CLIP357_9 */			
//	{6,		0 },		/* CLIP357_6_AP */	
//	{5,		0 },		/* CLIP357_9_AP */	
//	{4,		0 },		/* CLIP357_6_HP */			//90
//
//	{4,		0 },		/* CLIP357_9_HP */	
//	{25,	0 },		/* CLIP545_30_AP */	
//	{5,		0 },		/* CLIP545_30_HP */	
//	{15,	0 },		/* CLIP556_30_AP */	
//	{5,		0 },		/* CLIP556_30_HP */	
//	{15,	0 },		/* CLIP762W_10_AP */
//	{12,	0 },		/* CLIP762W_30_AP */
//	{4,		0 },		/* CLIP762W_10_HP */
//	{5,		0 },		/* CLIP762W_30_HP */
//	{10,	0 },		/* CLIP762N_5_AP */			//100
//
//	{10,	0 },		/* CLIP762N_20_AP */
//	{5,		0 },		/* CLIP762N_5_HP */	
//	{5,		0 },		/* CLIP762N_20_HP */	
//	{10,	0 },		/* CLIP47_50_SAP */		
//	{10,	0 },		/* CLIP57_50_AP */		
//	{5,		0 },		/* CLIP57_50_HP */		
//	{20,	0 },		/* CLIP12G_7 */				
//	{40,	0 },		/* CLIP12G_7_BUCKSHOT */
//	{10,	0 },		/* CLIPCAWS_10_SAP */			
//	{10,	0 },		/* CLIPCAWS_10_FLECH */			//110
//
//	{5,		0 },		/* CLIPROCKET_AP */
//	{5,		0 },		/* CLIPROCKET_HE */	
//	{5,		0 },		/* CLIPROCKET_HEAT  */	
//	{20,	0 },		/* sleep dart */
//	{0,		0 },		/* Clip Flame */
//	{6,		0 },		/* CLIP50_11 */	
//	{5,		0 },		/* CLIP9H_20 */						//120
//	{8,		0 },		/* CLIP9_50 */	
//	{4,		0 },		/* CLIP9_50_AP */	
//	{4,		0 },		/* CLIP9_50_HP */	
//
//	{2,		0 },		/* DRUM545_75_AP */	
//	{1,		0 },		/* DRUM545_75_HP */	
//	{2,		0 },		/* BELT556_200_AP */	
//	{1,		0 },		/* BELT556_200_HP */	
//	{2,		0 },		/* BELT762N_100_AP */	
//	{1,		0 },		/* BELT762N_100_HP */	
//	{10,	0 },		/* CLIP57_20_AP */	
//	{5,		0 },		/* CLIP57_20_HP */	
//	{0,		0 },		/* nothing */	
//	{0,		0 },		/* nothing */						//130
//
////---EXPLOSIVES---
//	{7,		0 },		/* stun grenade				*/
//	{7,		0 },		/* tear gas grenade   */
//	{5,		0 },		/* mustard gas grenade*/
//	{7,		0 },		/* mini hand grenade  */
//	{6, 	0 },		/* reg hand grenade   */
//	{0,		0 },		/* RDX                */
//	{6,		0 },		/* TNT (="explosives")*/
//	{0,		0 },		/* HMX (=RDX+TNT)     */
//	{0,		0 },		/* C1  (=RDX+min oil) */
//	{6,		0 },		/* mortar shell       */		//140
//
//	{0,		0 },		/* mine               */
//	{2,		0 },		/* C4  ("plastique")  */
//	{0,		0 },		/* trip flare				  */
//	{0,		0 },		/* trip klaxon        */
//	{4,		0 },		/* shaped charge */
//	{4,		0 },		/* break light */
//	{5,		0 },		/* 40mm HE grenade */
//	{5, 	0 },		/* 40mm gas grenade */
//	{5,		0 },		/* 40mm stun grenade */
//	{5,		0 },		/* 40mm smoke grenade */						//150
//
//	{7,		0 },		/* smoke hand grenade */
//	{0,		0 },		/* tank shell */
//	{0,		0 },		/* structure ignite */
//	{0,		0 },		/* creature cocktail */
//	{0,		0 },		/* nothing */
//	{0,		0 },		/* nothing */
//	{0,		0 },		/* nothing */
//	{0,		0 },		/* nothing */
//	{0,		0 },		/* nothing */
//	{0,		0 },		/* nothing */					//160
//
////---ARMOUR---
//	{2,		1 },		/* Flak jacket     */
//	{0,		0 },		/* Flak jacket w X */
//	{0,		0 },		/* Flak jacket w Y */
//	{1,		1 },		/* Kevlar vest   */
//	{0,		0 },		/* Kevlar vest w X */
//	{0,		0 },		/* Kevlar vest w Y */
//	{1,		1 },		/* Spectra vest  */
//	{0,		0 },		/* Spectra vest w X*/
//	{0,		0 },		/* Spectra vest w Y*/
//	{1,		1 },		/* Kevlar leggings */			//170
//
//	{0,		0 },		/* Kevlar legs w X */
//	{0,		0 },		/* Kevlar legs w Y */
//	{1,		1 },		/* Spectra leggings*/
//	{0,		0 },		/* Spectra legs w X*/
//	{0,		0 },		/* Spectra legs w Y*/
//	{3,		1 },		/* Steel helmet    */
//	{1,		1 },		/* Kevlar helmet   */
//	{0,		0 },		/* Kevlar helm w X */
//	{0,		0 },		/* Kevlar helm w Y */
//	{1,		1 },		/* Spectra helmet  */				//180
//
//	{0,		0 },		/* Spectra helm w X*/
//	{0,		0 },		/* Spectra helm w Y*/
//	{0,		0 },		/* Ceramic plates  */ 
//	{0,		0 },		/* hide */
//	{0,		0 },		/* hide */
//	{0,		0 },		/* hide */
//	{0,		0 },		/* hide */
//	{1,		0 },		/* Leather jacket */
//	{1,		0 },		/* Leather jacket w kevlar */
//	{0,		0 },		/* Leather jacket w kevlar 18 */						//190
//
//	{1,		0 },		/* Leather jacket w kevlar Y */
//	{0,		0 },		/* hide */
//	{0,		0 },		/* hide */
//	{0,		0 },		/* T-shirt (Arulco) */
//	{0,		0 },		/* T-shirt (Deidranna) */
//	{1,		1 },		/* Kevlar 2 Vest */
//	{0,		0 },		/* Kevlar 2 Vest w 18 */
//	{0,		0 },		/* Kevlar 2 Vest w Y */
//	{0,		0 },		/* nothing */
//	{0,		0 },		/* nothing */				//200
//
////---MISC---
//	{8,		0 },		/* First aid kit */
//	{6,		0 },		/* Medical Kit   */
//	{4,		1 },		/* Tool Kit	     */
//	{3,		1 },	  /* Locksmith kit */
//	{4,		0 },	  /* Camouflage kit*/
//	{0,		0 },	  /* nothing */					// Boobytrap kit - unused
//	{6,		1 },		/* Silencer      */
//	{6,		1 },		/* Sniper scope  */
//	{6,		1 },		/* Bipod         */
//	{6,		1 },		/* Extended ear	 */		// 210
//
//	{6,		1 },		/* Night goggles */
//	{8,		0 },		/* Sun goggles	 */
//	{6,		1 },		/* Gas mask   	 */
//	{10,	0 },		/* Canteen       */
//	{2,		0 },		/* Metal detector*/	
//	{6,		0 },		/* Compound 18	 */	
//	{0,		0 },		/* Jar w/Queen Blood */	
//	{0,		0 },		/* Jar w/Elixir */
//	{0,		0 },		/* Money         */
//	{0,		0 },		/* Glass jar		 */		//220
//
//	{0,		0 },		/* Jar w/Creature Blood */
//	{0,		0 },		/* Adrenaline Booster */
//	{6,		0 },		/* Detonator     */	
//	{6,		0 },		/* Rem Detonator */	
//	{0,		0 },		/* VideoTape     */	
//	{0,		0 },		/* Deed					 */	
//	{0,		0 },		/* Letter				 */
//	{0,		0 },		/* Terrorist Info */	
//	{0,		0 },		/* Chalice       */	
//	{0,		0 },		/* Mission4      */		//230
//
//	{0,		0 },		/* Mission5      */
//	{0,		0 },		/* Mission6      */	
//	{0,		0 },		/* Switch        */
//	{0,		0 },		/* Action Item   */	
//	{0,		0 },		/* Syringe2      */	
//	{0,		0 },		/* Syringe3      */
//	{0,		0 },		/* Syringe4      */
//	{0,		0 },		/* Syringe5      */
//	{0,		0 },		/* Jar w/Human blood   */
//	{0,		0 },		/* Ownership item */		//240
//
//	{6,		1 },		/* Laser scope   */
//	{6,		0 },		/* Remote trigger*/
//	{3,		0 },		/* Wirecutters   */			//243
//	{3,		0 },		/* Duckbill      */		
//	{0,		0 },	/* Alcohol  */				
//	{1,		1 },	/* UV Goggles */
//	{0,		0 },	/* Discarded LAW */
//	{0,		0 },	/* head - generic */	
//	{0,		0 },	/* head - Imposter*/	
//	{0,		0 },	/* head - T-Rex */			// 250
//
//	{0,		0 },	/* head - Slay */			
//	{0,		0 },	/* head - Druggist */	
//	{0,		0 },	/* head - Matron */		
//	{0,		0 },	/* head - Tiffany */	
//	{0,		0 },	/* wine     */				
//	{0,		0 },	/* beer    */				
//	{0,		0 },	/* pornos  */				
//	{0,		0 },	/* video camera */				
//	{0,		0 },	/* robot remote control */				
//	{0,		0 },	/* creature claws */		// 260
//
//	{0,		0 },	/* creature flesh */				
//	{0,		0 },	/* creature organ */				
//	{0,		0 },	/* remote trigger */				
//	{0,		0 },	/* gold watch */				
//	{0,		0 },	/* golf clubs */				
//	{0,		0 },	/* walkman */				
//	{0,		0 },	/* portable TV */				
//	{0,		0 },	/* Money for player's account */				
//	{0,		0 },	/* cigars */				
//	{0,		0 },	/* nothing */				// 270
//
//	{0,		0 },	/* key */						// 271
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//
//	{6,		1 },	/* flash suppressor */						// 281
//	{2,		1 },	/* rpg 7 */				
//	{4,		0 },	/* RPG_HE_ROCKET */				
//	{4,		0 },	/* RPG_AP_ROCKET */				
//	{4,		0 },	/* RPG_FRAG_ROCKET */				
//	{6,		1 },	/* reflex_scoped */				
//	{6,		1 },	/* reflex_unscoped */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//
//	{0,		0 },	/* key */						// 291
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//	{0,		0 },	/* key */				
//
//	{0,		0 },	/* key */						// 301
//	{0,		0 },	/* key */				
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */		
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//
//	{0,		0 },	/* nothing */				// 311
//	{0,		0 },	/* nothing */		
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//
//	{0,		0 },	/* nothing */				// 321
//	{0,		0 },	/* nothing */		
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{3,		1 },		/* m900 */			
//	{6,		1 },		/* m950 */
//	{3,		1 },		/* m960a - 330 */
//
//	{4,		1 },		/* micro uzi - 331 */
//	{2,		1 },		/* enfield */
//	{5,		1 },	/* MP5 */
//	{3,		1 },	/* mp5sd */
//	{2,		1 },	/* mp5n */
//	{3,		1 },	/* ump45 */
//	{4,		1 },	/* five7 */
//	{6,		1 },	/* p7m8 */
//	{5,		1 },	/* g36k */
//	{4,		1 },	/* g36c */
//
//	{2,		1 },	/* MSG */				// 341
//	{3,		1 },	/* Bennelli */		
//	{3,		1 },	/* AK103 */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//	{0,		0 },	/* nothing */
//
//};


 //MADD MARKER
INT16 WeaponROF[ MAXITEMS ];//=
//{
//	0,		/* Nothing */	
//
//	40,		/* Glock 17        */	
//	1300,		/* Glock 18        */	
//	45,		/* Beretta 92F     */	
//	1100,		/* Beretta 93R     */	
//	25,		/* .38 S&W Special */	
//	23,		/* .357 Barracuda  */	
//	27,		/* .357 DesertEagle*/ 
//	35,		/* .45 M1911			 */ 
//	900,		/* H&K MP5K      	 */	
//	1090,		/* .45 MAC-10	     */			// 10
//
//	700,		/* Thompson M1A1   */	
//	900,		/* Colt Commando   */	
//	800,		/* H&K MP53		 		 */	
//	900,		/* AKSU-74         */ 
//	900,		/* 5.7mm FN P90    */ 
//	780,		/* Type-85         */ 
//	40,		/* SKS             */ 
//	20,		/* Dragunov        */ 
//	20,		/* M24             */ 
//	650,		/* Steyr AUG       */			//20
//
//	850,		/* H&K G41         */ 
//	750,		/* Ruger Mini-14   */ 
//	600,		/* C-7             */ 
//	900,		/* FA-MAS          */ 
//	650,		/* AK-74           */ 
//	600,		/* AKM             */ 
//	750,		/* M-14            */ 
//	650,		/* FN-FAL          */	
//	500,		/* H&K G3A3        */ 
//	600,		/* H&K G11         */			// 30
//
//	21,		/* Remington M870  */	
//	30,		/* SPAS-15         */ 
//	-1,		/* CAWS            */
//	750,		/* FN Minimi       */
//	800,		/* RPK-74          */
//	800,		/* H&K 21E         */	
//	0,			/* combat knife */
//	0,			/* throwing knife */
//	0,			/* rock */
//	1,			/* grenade launcher */		// 40
//
//	1,		/* mortar */
//	0,		/* another rock    */
//	0,		/* claws */			
//	0,		/* claws */
//	0,		/* claws */
//	0,		/* claws */
//	0,		/* tentacles */
//	0,		/* spit  */
//	0,		/* brass knuckles */
//	1,		/* underslung g.l. */				// 50
//
//	1,		/* rocket launcher */
//	0,		/* nothing */
//	0,		/* nothing */
//	0,		/* machete */
//	1,		/* rocket rifle */
//	666,	/* Automag III */
//	0,		/* spit */
//	0,		/* spit */
//	0,		/* spit */
//	1,		/* tank cannon */
//	1,		/* dart gun */
//
//0, //	BLOODY_THROWING_KNIFE,
//1, //	FLAMETHROWER,
//0, //	CROWBAR,
//1, //	AUTO_ROCKET_RIFLE,	//65
//
//20, //	BARRETT, //previously unused
//25, //	VAL_SILENT, //previously unused
//22,//	PSG, //previously unused
//900, //	TAR21, //previously unused
//
//	0,		// Max_weapons 70
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 80
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 90
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 100
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 110
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 120
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 130
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 140
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 150
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 160
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 170
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 180
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 190
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 200
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 210
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 220
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 230
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 240
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 250
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 260
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 270
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 280
//	0,		// nothing
//	1,		// rpg 7
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 290
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 300
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 310
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 320
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	1300,		/* m900 */			
//	600,		/* m950  */
//	600,		/* m960a - 330 */
//	800,		/* micro uzi */
//	600,		/* enfield */
//	800,		// mp5
//	800,		// mp5sd
//	900,		// mp5n
//	800,		// ump45
//	45,		// five7
//	50,		// p7m8
//	750,		// G36k
//	750,		// G36c 340
//	2,		// MSG90A1
//	35,		// bennelli
//	600,		// ak103
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing
//	0,		// nothing 350
//};










// SetupStoreInventory sets up the initial quantity on hand for all of Bobby Ray's inventory items
void SetupStoreInventory( STORE_INVENTORY *pInventoryArray, BOOLEAN fUsed )
{
	UINT16 i;
	UINT16 usItemIndex;
	UINT8 ubNumBought;

	//loop through all items BR can stock to init a starting quantity on hand 
	for(i = 0; i < LaptopSaveInfo.usInventoryListLength[fUsed]; i++)
	{
		usItemIndex = pInventoryArray[ i ].usItemIndex;
		Assert(usItemIndex < MAXITEMS);

		ubNumBought = DetermineInitialInvItems( -1, usItemIndex, StoreInventory[ usItemIndex ][ fUsed ], fUsed);
		if ( ubNumBought > 0)
		{
			// If doing used items
			if( fUsed )
			{
				// then there should only be 1 of them, and it's damaged
				pInventoryArray[i].ubQtyOnHand = 1;
				pInventoryArray[i].ubItemQuality = 20 + (UINT8) Random( 60 );
			}
			else	// new
			{
				// Madd
				pInventoryArray[i].ubQtyOnHand = ubNumBought * gGameOptions.ubBobbyRay;
				pInventoryArray[i].ubItemQuality = 100;
			}
		}
		else
		{
			// doesn't sell / not in stock
			pInventoryArray[i].ubQtyOnHand = 0;
			pInventoryArray[i].ubItemQuality = 0;
		}
	}
}


BOOLEAN DoesGunOfSameClassExistInInventory( UINT8 ubItemIndex, UINT8 ubDealerID )
{
	UINT16 i;

	STORE_INVENTORY *pInventoryArray;

	pInventoryArray = GetPtrToStoreInventory( ubDealerID );
	if( pInventoryArray == NULL )
		return( FALSE );


	//go through all of the guns
	for(i=0; i<MAXITEMS; i++) 
	{
		//if it's the class we are looking for 
		if( Weapon[ i ].ubWeaponClass == ubItemIndex )
		{
			// and it's a sufficiently cool gun to be counted as good
			if (Item[ i ].ubCoolness >= 4)
			{
				//if there is already a qty on hand, then we found a match
				if( pInventoryArray[ i ].ubQtyOnHand )
				{
					return(TRUE);
				}
			}
		}
	}
	return(FALSE);
}





////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////

STORE_INVENTORY *GetPtrToStoreInventory( UINT8 ubDealerID )
{
	if( ubDealerID >= BOBBY_RAY_LISTS )
		return( NULL );


	if( ubDealerID == BOBBY_RAY_NEW )
		return( LaptopSaveInfo.BobbyRayInventory );
	else if( ubDealerID == BOBBY_RAY_USED )
		return( LaptopSaveInfo.BobbyRayUsedInventory );
	else
		Assert( 0 );
//	else
//		return( gArmsDealersInventory[ ubDealerID - TONYS_ITEMS ] );


	return( NULL );
}



/*
INT16	CountNumberOfItemsInStoreInventory( UINT8 ubArmsDealerID )
{
	UINT16	cnt;
	INT16		ubNumItems=0;

	STORE_INVENTORY *pInventoryArray;

	pInventoryArray = GetPtrToStoreInventory( ubArmsDealerID );
	if( pInventoryArray == NULL )
		return( -1 );


	for( cnt=0; cnt<MAXITEMS; cnt++ )
	{
		if( pInventoryArray[cnt].ubQtyOnHand > 0 )
			ubNumItems++;
	}

	return( ubNumItems );
}
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////







