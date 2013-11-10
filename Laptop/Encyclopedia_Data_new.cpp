#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "Types.h"
	#include "WCheck.h"
	#include <string.h>
	#include "GameSettings.h"
	#include "Encyclopedia_Data_new.h"
	#include "Encyclopedia_new.h"
	#include "Item Types.h"			//item
	#include "Interface Items.h"	//item: load item graphic
	#include "Weapons.h"			//item
	#include "Queen Command.h"		//location: Sectorinfo
	#include "strategic.h"			//location: townIds
	#include "PostalService.h"		//location: airport
	#include "Campaign Types.h"		//location: facility types, descriptions
	#include "Soldier Profile.h"	//char: names, flags and IDs
	#include "Vehicles.h"			//char: vehicle image
	#include "LocalizedStrings.h"
	#include "Encrypted File.h"
	#include "mercs.h"
	#include "Overhead.h"			//char: menpointer for ubWhatKindofMercAmI
	#include "aim.h"				//char: aim availability
	#include "QuestText.h"			//quest: name
	#include "laptop.h"				//ui positions
	#include "Utilities.h"
	#include "Utils/Cursors.h"
	#include "sysutil.h"			//extra Buffer for scaling image
	#include "vsurface.h"			//fill extra buffer with black color
	#include "Text.h"
	#include "WordWrap.h"
    #include "Quests.h"
#endif

/** @ingroup ENCYCLOPEDIA
* @file
* Encyclopedia data page in laptop.
* @author Moa
*/

/** @addtogroup ENC_DATA
* Depending on the selection made in \ref ENC_MAIN of encyclopedia the data page shows \li Locations (\ref ENC_DATA_FILTER_LOCATION) \li Characters (\ref ENC_DATA_FILTER_CHARACTER) \li Items (\ref ENC_DATA_FILTER_ITEM) \li Quests (\ref ENC_DATA_FILTER_QUEST)
*
* On the left side of www-laptop screen are small buttons to filter the data. These buttons are refered to as filter buttons.
* \n Left click on a button will set the filter. Clicking on an active filterbutton will change the subfilter.
* \n Rightclick will reset the subfilter and activate the filter.
* \n \n Below the image there are two buttons for showing next and previous text page.
* \n On the bottom player can view the next or previous data or go back to main menu of encyclopedia.
*/

/// For switching between data sub pages.
enum ENC_DATA_SUBPAGE_T {
	ENC_DATA_NO_DATA = -1,	///< No data. If there is no data only the buttons and background is shown.
	ENC_DATA_PREVIOUS = 0,	///< Goto previous data. If there is no previous the 'previous' button gets deactivated.
	ENC_DATA_NEXT = 1,		///< Goto next data. If there is no next the 'next' button gets deactivated.
};

/// Encyclopedia data struct.
struct ENC_DATA_ENTRY_T {
	CHAR16 sDescr[560];		///< Detailed description (below image). Up to 7 lines of text each with 80 characters. Only printable characters allowed.
	CHAR16 sName[80];		///< zerro terminated string for name of data (Headline above image). Only printable characters allowed.
	CHAR16 sShortDesc[80];	///< Short description (below image). Up to 80 characters. Only printable characters allowed.
	UINT32 uiImageID;		///< Video object ID.
	INT16 iID;				///< Corresponds to the index of items, (modified) index of sector, quest index or profile ID. If no data is available this is -1.
	UINT8 uiTextPage;		///< Current text page. Number of pages is currently 2 as there is no use for more at the moment. To define different pages modify \ref CreateData() and \ref BtnEncyclopedia_Data_NextPreviousTextBtnCallBack() accordingly.
} gstEncyclopediaDataEntry; ///< Current data.

UINT8 gbEncyclopediaData_ItemVisible[MAXITEMS];///< Visibility of items. If an item should be visible the value is not 0. Gets loaded from savegame, updated by \ref EncyclopediaSetItemAsVisible() and reset by \ref EncyclopediaInitItemsVisibility().

/// Various filters for locations. Only filtered locations are shown. Subfilters, if any, are handled by callback function.
enum ENC_DATA_FILTER_LOCATION {
	ENC_DATA_FILTER_L_ALL = 0,		///< All locations visible
	ENC_DATA_FILTER_L_CITY,			///< Only Cities
	ENC_DATA_FILTER_L_SAM,			///< Only SAM sites
	ENC_DATA_FILTER_L_MINE,			///< Only mines
	ENC_DATA_FILTER_L_AIRPORT,		///< Only airports
	ENC_DATA_FILTER_L_WILDERNESS,	///< No city, no SAM, no mine, no airport
	ENC_DATA_FILTER_L_UNDERGROUND,	///< All underground sectors
	ENC_DATA_FILTER_L_FACILITY,		///< Only facilities, has multiple subfilters
	ENC_DATA_FILTER_L_OTHER			///< Important other sectors, TBD (posibly creatures lare, gas station, ec.)
} geEncyclopediaDataFilterLocation;	///< Current location filter

/// Various filters for characters. Only filtered characters are shown. Subfilters, if any, are handled by callback function.
enum ENC_DATA_FILTER_CHARACTER {
	ENC_DATA_FILTER_C_ALL = 0,		///< All characters visible
	ENC_DATA_FILTER_C_AIM,			///< hireable characters from A.I.M.
	ENC_DATA_FILTER_C_MERC,			///< hireable characters from M.E.R.C.
	ENC_DATA_FILTER_C_RPC,			///< Rebels
	ENC_DATA_FILTER_C_NPC,			///< non hireable characters
	ENC_DATA_FILTER_C_IMP,			///< player created characters
	ENC_DATA_FILTER_C_OTHER			///< vehicles and electronic 'chars'
} geEncyclopediaDataFilterCharacter;///< Current character filter

///  Various filters for items. Only filtered items are shown. Uses Bobby Ray's categories. Subfilters, if any, are handled by callback function.
enum ENC_DATA_FILTER_ITEM {
	ENC_DATA_FILTER_I_ALL = 0,		///< All items visible
	ENC_DATA_FILTER_I_GUN,			///< Only Guns, has multiple subfilters
	ENC_DATA_FILTER_I_AMMO,			///< Only Ammunition, has multiple subfilters
	ENC_DATA_FILTER_I_ARMOUR,		///< Only Armour pieces, has multiple subfilters
	ENC_DATA_FILTER_I_LBE,			///< Only LBE gear, has multiple subfilters
	ENC_DATA_FILTER_I_ATTACHMENT,	///< Only attachable items (TBD), has multiple subfilters
	ENC_DATA_FILTER_I_MISC			///< Everything else, has multiple subfilters
} geEncyclopediaDataFilterItem;		///< Current item filter

/// @brief Various filters for quests.@details Only filtered quests are shown. Subfilters, if any, are handled by callback function. 
enum ENC_DATA_FILTER_QUEST {
	ENC_DATA_FILTER_Q_ALL = 0,		///< All active and completed quests are visible.
	ENC_DATA_FILTER_Q_ACTIVE,		///< Only started, but not completed quests are shown.
	ENC_DATA_FILTER_Q_COMPLETE		///< All finished quests are shown.
} geEncyclopediaDataFilterQuest;	///< Current quest filter

/// @brief SubClass filter for all major filters.@details 0 means no subfilter. Actual meaning depends on major filter.
/// @see ENC_DATA_FILTER_CHARACTER, @see ENC_DATA_FILTER_CHARACTER,@see ENC_DATA_FILTER_ITEM,@see ENC_DATA_FILTER_QUEST
UINT16 guiEncyclopediaDataSubFilter;

/// Gap for name (top), image (top and left), filter buttons (top and left), short description (top and left), detailed description (top and left)
#define ENC_DATA_GAP 5
/// position of first filterbutton
#define ENC_DATA_FILTER_GAP_TOP 40
/// shadow height of image
#define ENC_DATA_IMAGE_HEIGHT 172
/// shadow height of short description
#define ENC_DATA_SHORT_DESC_HEIGHT 18
/// shadow height of detailed description
#define ENC_DATA_DESC_HEIGHT 100
/// Maximum allowed (major) filters. Change only if one of the four data page categories exceed the limit of (major) filter buttons.
#define ENC_DATA_MAX_FILTERS 9
/// Macro to get text for filter buttons. Works only for continuing subfilters (1,2,3..,255) where 0 is 'show all'. Uses the globals geENC_SubPage and guiEncyclopediaDataSubFilter @param defaultTStrP reseted filter texts CHAR16** @param subFilterTStrP Subfilter CHAR16** @param mf Current major filter.
#define GetDataFilterText(defaultTStrP,subFilterTStrP,mf) (guiEncyclopediaDataSubFilter?subFilterTStrP[(UINT16)guiEncyclopediaDataSubFilter+guiEncyclopediaSubFilterTextIndexStart[geENC_SubPage-1][mf-1]-1]:defaultTStrP[mf])

UINT8 guiEncyclopediaSubFilterTextIndexStart[ENC_NUM_SUBPAGES][ENC_DATA_MAX_FILTERS]; ///< Holds the indizes of first filter button texts. Used to update text on click. Initialized on gamestart.
INT32 giEncyclopedia_DataFilterBtnImage;					///< Image of filter button.
INT32 giEncyclopedia_DataFilterBtn[ ENC_DATA_MAX_FILTERS ];	///< Filter buttons. \see ENC_DATA_MAX_FILTERS.
INT32 giEncyclopedia_DataBtnImage;							///< Image of next/previous/main page buttons.
INT32 giEncyclopedia_DataBtn[ 5 ];							///< previousData, mainpage, nextData, previousText, nextText.
UINT8 guiEncyclopedia_TextPage;								///< Current text page.

//////////////
// external data to save includes
extern UINT8 gubMercArray[ NUM_PROFILES ];
extern UINT8 gubQuest[MAX_QUESTS];
//extern 	STR16 QuestDescText[];
//////////////
// function prototypes
extern BOOLEAN IsAmmoMatchinWeaponType(UINT16 usItemIndex, UINT8 ubWeaponType);
extern BOOLEAN GetSectorFlagStatus( INT16 sMapX, INT16 sMapY, UINT8 bMapZ, UINT32 uiFlagToSet );
extern BOOLEAN IsThereAMineInThisSector( INT16 sX, INT16 sY );
extern BOOLEAN IsMercMercAvailable( UINT8 ubMercID );
extern BOOLEAN LoadMercBioInfo(UINT8 ubIndex, STR16 pInfoString, STR16 pAddInfo);
extern void GetQuestStartedString( UINT8 ubQuestValue, STR16 sQuestString );
extern void GetQuestEndedString( UINT8 ubQuestValue, STR16 sQuestString );
//////////////
// helper functions

/**
* Calculates x,y and z sectorinfo coords from encyclopedia location ID.
* Location ID is encoded as: ([SectorInfo X coordinate - 1 (0..15)] + [SectorInfo Y coordinate - 1 (0..15)] * 16) * 4 + [SectorInfo Z coordinate(0..3)].
*
* @param encLocationID						[IN]range: 0 to 1023
* @param sSectorX, sSectorY, sSectorZ		[OUT] SectorInfo coords compatible to macro SECTOR(x,y)
* @return									TRUE if valid coords are calculated, FALSE if encLocationID is out of bounds.
*/
BOOLEAN GetEncyclopediaSectorCoordsFromLocationID( INT16 encLocationID, UINT16 &sSectorX, UINT16 &sSectorY, UINT8 &sSectorZ )
{
	UINT16 x,y;
	UINT8 z;
	CHECKF( encLocationID >= 0 );
	z = encLocationID % (MAXIMUM_VALID_Z_COORDINATE - MINIMUM_VALID_Z_COORDINATE + 1);
	CHECKF( z >= MINIMUM_VALID_Z_COORDINATE && z <= MAXIMUM_VALID_Z_COORDINATE );
	encLocationID /= (MAXIMUM_VALID_Z_COORDINATE - MINIMUM_VALID_Z_COORDINATE + 1);

	x = (encLocationID % (MAXIMUM_VALID_X_COORDINATE - MINIMUM_VALID_X_COORDINATE + 1) + 1);
	CHECKF( x >= MINIMUM_VALID_X_COORDINATE && x <= MAXIMUM_VALID_X_COORDINATE );
	encLocationID /= MAXIMUM_VALID_Y_COORDINATE - MINIMUM_VALID_Y_COORDINATE + 1;

	y = (encLocationID % MAXIMUM_VALID_Y_COORDINATE) + 1;
	CHECKF( y >= MINIMUM_VALID_Y_COORDINATE && y <= MAXIMUM_VALID_Y_COORDINATE );

	sSectorX = x; sSectorY = y; sSectorZ = z;
	return (TRUE);
}

/**
* Calculates encyclopedia location ID from x,y and z sectorinfo coords.
* Location ID is encoded as: ([SectorInfo X coordinate - 1 (0..15)] + [SectorInfo Y coordinate - 1 (0..15)] * 16) * 4 + [SectorInfo Z coordinate(0..4)].
*
* @param sSectorX, sSectorY, sSectorZ		[IN] SectorInfo coords compatible to macro SECTOR(x,y)
* @param encLocationID						[OUT]  range: 0..MAXIMUM_VALID_X_COORDINATE*MAXIMUM_VALID_Y_COORDINATE (0..1023)
* @return									TRUE if valid coords are calculated, FALSE if encLocationID is out of bounds.
*/
BOOLEAN GetEncyclopediaLocationIDFromSectorCoords( UINT16 sSectorX, UINT16 sSectorY, UINT8 sSectorZ, INT16 &encLocationID )
{
	CHECKF( sSectorX >= MINIMUM_VALID_X_COORDINATE && sSectorX <= MAXIMUM_VALID_X_COORDINATE );
	CHECKF( sSectorY >= MINIMUM_VALID_Y_COORDINATE && sSectorY <= MAXIMUM_VALID_Y_COORDINATE );
	CHECKF( sSectorZ >= MINIMUM_VALID_Z_COORDINATE && sSectorZ <= MINIMUM_VALID_Z_COORDINATE );
	
	encLocationID = ((sSectorX-1) + (sSectorY-1) * 16) * 4 + sSectorZ;
	return (TRUE);
}

/**
* Searches for the next or previous location.
*
* Takes filter settings into account.
* @param curLoc			Current location.
* @param next			If set to TRUE the next otherwise the previous location is selected.
* @param filterClass	Major filter @see ENC_DATA_FILTER_LOCATION
* @param filterSubClass	Sub filter. Ignored if the major filter does not support subfilters.
* @return				Returns a matching Location ID or -1 if none was found.
* @see ENC_DATA_FILTER_LOACTION
*/
INT16 GetNextPreviousLocation( INT16 curLoc, BOOLEAN next, ENC_DATA_FILTER_LOCATION filterClass, UINT16 filterSubClass )
{
	extern UINT8 gubBloodcatLairSectorId;
	extern std::vector<PDestinationStruct> gDestinationTable;
	extern CPostalService gPostalService;
	UINT16 x, y, MAXID = (MAXIMUM_VALID_X_COORDINATE * MAXIMUM_VALID_Y_COORDINATE * 4)-1;
	UINT8 z;
	BOOLEAN found = FALSE;
	CHECKBI( gGameExternalOptions.gEncyclopedia );
	CHECKBI( geENC_SubPage == ENC_LOCATIONS );
	//reset to 0 if out of bounds
	if ( curLoc < 0 || curLoc > MAXID )
		curLoc = 0;

	//loop all sectors
	for ( INT16 newID = curLoc + (next? 1: -1); 0 <= newID && newID <= MAXID; (next? newID++: newID--) )
	{
//#ifdef _DEBUG
//		Assert(GetEncyclopediaSectorCoordsFromLocationID( newID, x, y, z ));
//#else
		CHECKF(GetEncyclopediaSectorCoordsFromLocationID( newID, x, y, z ));
//#endif
		//ignore unvisited, !!do not change!!
		if( !GetSectorFlagStatus( x, y, z, SF_ALREADY_VISITED ) )
			continue;

		switch (filterClass)
		{
		case ENC_DATA_FILTER_L_ALL :		// All locations visible
			found = TRUE;
			break; //found
		case ENC_DATA_FILTER_L_CITY :			// Only Cities
			if( StrategicMap[CALCULATE_STRATEGIC_INDEX(x,y)].bNameId )
			{
				found = TRUE; // found location. Note that we allready checked for visited, so underground sectors are included as long as their [x,y] position matches.
				break;
			}
			continue;//no town
		case ENC_DATA_FILTER_L_SAM :			// Only SAM sites
			if( IsThisSectorASAMSector( x, y, z ) )
			{
				break;
				found = TRUE;
			}
			continue; //no SAM
		case ENC_DATA_FILTER_L_MINE :			// Only mines
			if( IsThereAMineInThisSector( x, y ) )
			{
				found = TRUE;
				break;
			}
			continue; // no mine
		case ENC_DATA_FILTER_L_AIRPORT :		// Only airports
			if ( gPostalService.IsSectorAShipmentSector((UINT8)x,(UINT8)y,z) )
			{
				found = TRUE;
				break;// break loop
			}
			continue; // no airport
		case ENC_DATA_FILTER_L_WILDERNESS :		// No underground, no city, no SAM (, no mine, no airport)
			if( z == 0 && !StrategicMap[CALCULATE_STRATEGIC_INDEX(x,y)].bNameId && !IsThisSectorASAMSector( x, y, z ) )
			{
				found = TRUE;
				break; //found
			}
			continue; // no wilderness
		case ENC_DATA_FILTER_L_UNDERGROUND:		// All underground sectors
			if( z > 0 )
			{
				found = TRUE;
				break; // found
			}
			continue; //no underground
		case ENC_DATA_FILTER_L_FACILITY :		// Only facilities, has multiple subfilters
			CHECKBI( filterSubClass < NUM_FACILITY_TYPES );
			if( z == 0 && filterSubClass == 0 )
			{// no subfilter, check all possible facilities
				for( UINT8 i = 0; i < NUM_FACILITIES; i++ )
				{
					if( gFacilityLocations[SECTOR(x,y)][i].fFacilityHere )
					{
						found = TRUE;
						break;
					}
				}
				if( found )
					break; // found
			}
			else if( z == 0 && gFacilityLocations[SECTOR(x,y)][filterSubClass].fFacilityHere )
			{
				found = TRUE;
				break; // found
			}
			continue; // no facility
		case ENC_DATA_FILTER_L_OTHER :			// Important other sectors, TBD (posibly creatures lare, gas station, ec.)
			if( z == 0 && SECTOR(x,y) == gubBloodcatLairSectorId )
			{
				found = TRUE;
				break; // found
			}
			continue;// no other
		}//end switch

		if(found)
		{
			return newID;
		}
	}//end loop
	return (ENC_DATA_NO_DATA);
}

/**
* @brief Searches for the next or previous character.
* @param curChar current index of gMercProfile
* @return index of next or previous gMercProfile
* @see GetNextPreviousLocation()
* @see ENC_DATA_FILTER_CHARACTER
*/
INT16 GetNextPreviousCharacter( INT16 curChar, BOOLEAN next, ENC_DATA_FILTER_CHARACTER filterClass, UINT16 filterSubClass )
{
	extern UINT8 gubMercArray[ NUM_PROFILES ];
	BOOLEAN notFound = TRUE;

	//reset to -1 if out of bounds
	if( curChar < 0 || curChar > NUM_PROFILES )
		curChar = -1;

	//loop through all profiles, start with current, direction determined by next. This wraps automatically because NUM_PROFILES+1 == sizeof(UINT8) == 256
	for( UINT8 i = curChar + (next?1:-1); 0 <= i && i < NUM_PROFILES; (next? i++: i--) )
	{
		//skip index 255 (used for not valid)
		if( i == (UINT8)-1 )
			continue;
		//todo: find a way to determine if we met that person allready and exclude cutscene
		// skip profiles we do not have interacted with, ignoring GOODGUY and TOWN_DOESNT_CARE_ABOUT_DEATH flags
		// NOTE: recruited flag does not get updated correctly at the moment -> needs fix (on dismiss, death, imp creation, merc/aim/npc hire ec.)
		if( !gMercProfiles[ i ].ubMiscFlags && !gMercProfiles[ i ].ubMiscFlags2 && !(gMercProfiles[ i ].ubMiscFlags3 & ~(PROFILE_MISC_FLAG3_GOODGUY | PROFILE_MISC_FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH)))
			continue;

		
/** @details
* Resources to gather data from:
*| variable name										| Type									|Index = person?					|Usage
*|:---------------------------------------------------- | :------------------------------------ | :-------------------------------: | :------------------------------------------------------------
*|1a. gMercProfiles[ NUM_PROFILES ]						|MERCPROFILESTRUCT, huge object			| yes for most, but not for all		| Used to create soldiers, can be used to get Name, Descriptions, Pictures. Each index is potentially a group of people (e.g. 200, vehicles and epc). On the other hand most profiles are unique to a soldier.
*|1b. Menptr[ NUM_PROFILES ]							|SOLDIERTYPE, huge struct				| YES								| On tactical those are the people / vehicles / monsters / EPCs. Can provide a reference to a profile. Most likely they are destroyed upon unloading the map, to be recreated with potentially different profiles. Therfore not usable here.
*|2a. AimMercArray[ NUM_PROFILES ]						|UINT8, IDs for gMercProfiles			| YES								| Used for AIM website. Aim members are on beginning of array, empty spots have (UNIT8)-1.
*|2b. gAimAvailability[ NUM_PROFILES ]					|AIM_AVAILABLE, stuct					| YES								| Used for AIM website to get bio text and profile ID.
*|2c. gProfilesAIM[ NUM_PROFILES ]						|AIM_PROFIL, IDs for gMercProfiles		| YES								| In contrast to 2b Index of gMercProfiles == Index of gProfilesAIM and gProfilesAIM[i].profileId == i when it is an AIM member, otherwise gProfilesAIM[i].profileId == (UINT8)-1.
*|3a. gubMercArray[ NUM_PROFILES ]						|UINT8, IDs for gMercProfiles			| YES								| Used for MERC website. MERC mebers are on beginning of array, empty spots have (UNIT8)-1.
*|3b. gConditionsForMercAvailability[ NUM_PROFILES ]	|CONTITION_FOR_MERC_AVAILABLE, struct	| YES								| Used for MERC website to get bio text, profile ID and other data. MERC mebers are on beginning of array, empty spots have ProfilId = 0.
*|3c. gProfilesMERC[ NUM_PROFILES ]						|MERC_PROFIL see 2c						| YES								| see 2c.
*|4.  gProfilesNPC[ NUM_PROFILES ]						|NPC_PROFIL see 2c						| YES								| see 2c.
*|5.  gProfilesRPC[ NUM_PROFILES ]						|RPC_PROFIL see 2c						| YES								| see 2c.
*|6a. pVehicleList(variable Size)						|VEHICLETYPE, medium struct				| YES								| Same principle as 1b, but data valid regardless of game state.
*|6b. gNewVehicle[ NUM_PROFILES ]						|NEW_CAR, medium struct					| NO								| Used to get vehicle 'face', Snds, name ec. Index is identical to gMercProfiles.
*|6c. gProfilesVehicle[ NUM_PROFILES ]					|VEHICLE_PROFIL see 2c					| NO								| see 2c. Potentially more then one vehicle of a type can be created, but this is irrelevant as we are showing only the different types anyway.
*|7.  gProfilesIMP[ NUM_PROFILES ]						|IMP_PROFILE see 2c						| YES								| see 2c.
*/
		notFound = FALSE;
		switch(filterClass)
		{
		case ENC_DATA_FILTER_C_ALL :		// All characters visible
			// no filter, found char
			return i;
		case ENC_DATA_FILTER_C_AIM :		// hireable characters from A.I.M.
			if( gProfilesAIM[i].ProfilId != (UINT8)-1 )
				return gProfilesAIM[i].ProfilId;
			//if( gAimAvailability[ i ].ProfilId != (UINT8)-1 )
			//	return gAimAvailability[ i ].ProfilId;
			notFound = TRUE;//no AIM
			break;
		case ENC_DATA_FILTER_C_MERC :		// hireable characters from M.E.R.C.
			//if( gubMercArray[ i ] != 0 )
			//	return gubMercArray[ i ];
			if( gProfilesMERC[i].ProfilId != (UINT8)-1 )
				return gProfilesMERC[i].ProfilId;
			notFound = TRUE;//no AIM, no MERC
			break;
		case ENC_DATA_FILTER_C_OTHER :		// vehicles and electronic 'chars'
			//if( notFound && IsMercHireable( i ) && i != 254 && Menptr[i].iVehicleId )
			//	return i;
			if( (filterSubClass == 0 || filterSubClass == 1) && gProfilesVehicle[i].ProfilId != (UINT8)-1 )
				return gProfilesVehicle[i].ProfilId;
			if( (filterSubClass == 0 || filterSubClass == 2) && (gMercProfiles[i].ubMiscFlags & PROFILE_MISC_FLAG_EPCACTIVE) )
				return i;
			notFound = TRUE;
			break;
		case ENC_DATA_FILTER_C_RPC :		// Rebels
			//if( gMercProfiles[ i ].ubCivilianGroup == REBEL_CIV_GROUP && IsMercHireable( i ) )
			//	return i;//no
			if( gProfilesRPC[i].ProfilId != (UINT8)-1 )
				return gProfilesRPC[i].ProfilId;
			notFound = TRUE;
			break;
		case ENC_DATA_FILTER_C_NPC :		// non hireable characters
			//if( gMercProfiles[ i ].ubCivilianGroup > NON_CIV_GROUP && gMercProfiles[ i ].ubCivilianGroup != REBEL_CIV_GROUP && !IsMercHireable( i ) )
			//	return i;
			if( gProfilesNPC[i].ProfilId != (UINT8)-1 )
				return gProfilesNPC[i].ProfilId;
			notFound = TRUE;
			break;
		case ENC_DATA_FILTER_C_IMP :		// player created characters
			//for(UINT impSlot = 0; impSlot < NUM_PROFILES; impSlot++)
			//	if( gGameExternalOptions.iaIMPSlots[ impSlot ] == i )
			//		return i;
			if( gProfilesIMP[i].ProfilId != (UINT8)-1 )
				return gProfilesIMP[i].ProfilId;
			notFound = TRUE;
			break;
		}
	}




	return ENC_DATA_NO_DATA;
}

/**
* @brief Searches for the next or previous item.
* @see GetNextPreviousLocation()
* @see ENC_DATA_FILTER_ITEM
*/
INT16 GetNextPreviousItem( INT16 curItem, BOOLEAN next, ENC_DATA_FILTER_ITEM filterClass, UINT16 filterSubClass )
{
	//reset to 1 if out of bounds
	if ( curItem < 1 || curItem >= MAXITEMS )
		curItem = 1;

	// loop thru items, start with current, direction determined by next
	for ( INT16 i = curItem + (next? 1 : -1); (next? i < MAXITEMS : i > 0); (next? i++: i--) )
	{
		// last item, break!
		if ( Item[ i ].uiIndex == 0 )
			break;
		// not discovered yet, go ahead!
		if ( gbEncyclopediaData_ItemVisible[i] < gGameExternalOptions.ubEncyclopediaItemMask )
			continue;

		switch ( filterClass )
		{
			case ENC_DATA_FILTER_I_ALL :
				// no filter, found item.
				return i;
			case ENC_DATA_FILTER_I_GUN :
				if ( Item[ i ].usItemClass & IC_BOBBY_GUN )
				{
					// no subClass filter, found item.
					if ( filterSubClass == 0 )
						return i;
					// subClass filter match, found item
					else if ( Weapon[ i ].ubWeaponType == (UINT8)filterSubClass || ( filterSubClass == 9 && (Item[ i ].usItemClass & IC_LAUNCHER) ) )
						return i;
				}
				// not found, go ahead!
				continue;
			case ENC_DATA_FILTER_I_AMMO :
				if ( Item[ i ].usItemClass & IC_AMMO )
				{
					// no subfilters, found item
					if( filterSubClass == 0 )
						return i;
					// subClass filter match, found item
					else if( IsAmmoMatchinWeaponType( i, (UINT8)filterSubClass ) )
						return i;
				}
				else if ( /*(Item[ i ].usItemClass & IC_BOMB) && */filterSubClass == 9 && (Item[ i ].attachmentclass & (AC_ROCKET | AC_GRENADE)) )
					return i;
				// not found, go ahead!
				continue;
			case ENC_DATA_FILTER_I_ARMOUR :
				if( Item[ i ].usItemClass & IC_ARMOUR )
				{
					// no subClass filter, found item.
					if ( filterSubClass == 0 )
						return i;
					else if( Armour[ Item[ i ].ubClassIndex ].ubArmourClass == (UINT8)filterSubClass-1 )
						return i;
				}
				// not found, go ahead!
				continue;
			case ENC_DATA_FILTER_I_LBE :
				if( Item[ i ].usItemClass & IC_LBEGEAR )
				{
					if ( filterSubClass == 0)
						return i;
					if( LoadBearingEquipment[ Item[ i ].ubClassIndex ].lbeClass == filterSubClass )
						return i;
				}
				//not found, go ahead!
				continue;
			case ENC_DATA_FILTER_I_ATTACHMENT :
				if ( !Item[ i ].attachment || !(Item[ i ].usItemClass & (IC_MISC)) )
					continue;// no attachment, skip
				switch( filterSubClass )
				{
				case 0 :
					if( !Item[ i ].attachmentclass || !(Item[ i ].attachmentclass & (AC_ROCKET | AC_GRENADE)) )
						return i;
					break;
				case BOBBYR_FILTER_MISC_OPTICS_ATTACHMENTS :
					if( Item[ i ].attachmentclass & (AC_SCOPE | AC_SIGHT | AC_IRONSIGHT | AC_LASER) )
						return i;
					break;
				case BOBBYR_FILTER_MISC_SIDE_AND_BOTTOM_ATTACHMENTS :
					if ( Item[ i ].attachmentclass & (AC_FEEDER | AC_FOREGRIP | AC_BIPOD | AC_MAGWELL) )
						return i;
					break;
				case BOBBYR_FILTER_MISC_MUZZLE_ATTACHMENTS :
					if ( Item[ i ].attachmentclass & (AC_EXTENDER | AC_BAYONET | AC_RIFLEGRENADE | AC_MUZZLE) )
						return i;
					break;
				case BOBBYR_FILTER_MISC_STOCK_ATTACHMENTS :
					if ( Item[ i ].attachmentclass & (AC_STOCK | AC_SLING | AC_EXTERNAL) )
						return i;
					break;
				case BOBBYR_FILTER_MISC_INTERNAL_ATTACHMENTS :
					if ( Item[ i ].attachmentclass & (AC_BATTERY | AC_INTERNAL | AC_UNDERBARREL) )
						return i;
					break;
				case BOBBYR_FILTER_MISC_OTHER_ATTACHMENTS :
					if ( !Item[ i ].attachmentclass || (Item[ i ].attachmentclass & (AC_DEFUSE | AC_DETONATOR | AC_HELMET | AC_VEST | AC_PANTS | AC_MODPOUCH | AC_MISC16 | AC_MISC17 | AC_MISC18)) )
						return i;
					break;
				}
				// not found, go ahead!
				continue;
			case ENC_DATA_FILTER_I_MISC :
				if( (Item[ i ].usItemClass & (IC_BOBBY_GUN | IC_AMMO | IC_LBEGEAR | IC_ARMOUR )) /*|| Item[ i ].attachment*/ || (Item[ i ].attachmentclass /*& (AC_ROCKET | AC_GRENADE)*/) )
					continue;// allready covered above, skip
				switch ( filterSubClass )
				{
				case 0 :
					if( Item[ i ].usItemClass & (IC_BLADE | IC_THROWING_KNIFE | IC_PUNCH | IC_GRENADE | IC_BOMB | IC_MEDKIT | IC_KIT | IC_FACE | IC_MISC | IC_MONEY ) )
						return i;
					break;
				case BOBBYR_FILTER_MISC_BLADE :
					if( Item[ i ].usItemClass & IC_BLADE )
						return i;
					break;
				case BOBBYR_FILTER_MISC_THROWING_KNIFE :
					if( Item[ i ].usItemClass & IC_THROWING_KNIFE )
						return i;
					break;
				case BOBBYR_FILTER_MISC_PUNCH :
					if( Item[ i ].usItemClass & IC_PUNCH )
						return i;
					break;
				case BOBBYR_FILTER_MISC_GRENADE :
					if( Item[ i ].usItemClass & IC_GRENADE )
						return i;
					break;
				case BOBBYR_FILTER_MISC_BOMB :
					if( Item[ i ].usItemClass & IC_BOMB )
						return i;
					break;
				case BOBBYR_FILTER_MISC_MEDKIT :
					if( Item[ i ].usItemClass & IC_MEDKIT )
						return i;
					break;
				case BOBBYR_FILTER_MISC_KIT :
					if( Item[ i ].usItemClass & IC_KIT )
						return i;
					break;
				case BOBBYR_FILTER_MISC_FACE :
					if( Item[ i ].usItemClass & IC_FACE )
						return i;
					break;
				case BOBBYR_FILTER_MISC_MISC :
					if( Item[ i ].usItemClass & (IC_MISC | IC_MONEY) )
						return i;
					break;
				}
				// not found, go ahead!
				continue;
			default :
				Assert(0);
		}
	}//end loop

	//nothing found
	return ENC_DATA_NO_DATA;
}

/**
* @brief Searches for the next or previous quest.
* @see GetNextPreviousLocation()
* @see ENC_DATA_FILTER_QUEST
* @param curQuest Quest ID matching index of gubQuest in quests.cpp
*/
INT16 GetNextPreviousQuest( INT16 curQuest, BOOLEAN next, ENC_DATA_FILTER_QUEST filterClass, UINT16 filterSubClass )
{
	//reset to -1 if out of bounds
	if ( curQuest < 0 || curQuest >= MAX_QUESTS )
		curQuest = -1;

	// loop thru quests, start with current, direction determined by next
	for( INT16 i = curQuest + (next? 1: -1); 0 <= i && i < (UINT8)MAX_QUESTS; next? i++: i-- )
	{
		switch( filterClass)
		{
		case ENC_DATA_FILTER_Q_ALL :
			if( gubQuest[ i ] != QUESTNOTSTARTED )
				return i;
			continue;// not found
		case ENC_DATA_FILTER_Q_ACTIVE :
			if( gubQuest[ i ] == QUESTINPROGRESS )
				return i;
			continue;//not found
		case ENC_DATA_FILTER_Q_COMPLETE :
			if( gubQuest[ i ] == QUESTDONE )
				return i;
			continue;//not found
		}
	}
	return ENC_DATA_NO_DATA;
}

/**
* @brief Fills gstEncyclopediaDataEntry according to the subpage we are in.
* @return TRUE if there is data, FALSE if the data is empty
* @see ENC_DATA_ENTRY_T
*/
BOOLEAN CreateData( INT16 newID )
{
	CHAR8 fileName[80];
	VOBJECT_DESC vObjDesc;

	CHECKF( gGameExternalOptions.gEncyclopedia );
	CHECKF( geENC_SubPage);

	UINT8 textPage = gstEncyclopediaDataEntry.uiTextPage;//want to keep that, otherwise we are forced to set the text outside of this function on text page change.
	//////
	// clear data

	// unload graphic if still exists
	if ( gstEncyclopediaDataEntry.uiImageID )
		DeleteVideoObjectFromIndex( gstEncyclopediaDataEntry.uiImageID );

	// reset remaining data
	memset( &gstEncyclopediaDataEntry, 0x0, sizeof(ENC_DATA_ENTRY_T) );

	//////
	// set ID
	if ( newID < 0 )
	{
		gstEncyclopediaDataEntry.iID = ENC_DATA_NO_DATA;
		return FALSE;
	}

	//////
	// recover text page.
	gstEncyclopediaDataEntry.uiTextPage = textPage;

	//////
	// set descriptions and load image
	switch (geENC_SubPage)
	{
	case ENC_LOCATIONS :
		//check for valid ID
		// set name
		GetSectorIDString( SECTORX( newID/(MAXIMUM_VALID_Z_COORDINATE+1) ), SECTORY( newID/(MAXIMUM_VALID_Z_COORDINATE+1) ), newID%(MAXIMUM_VALID_Z_COORDINATE+1), gstEncyclopediaDataEntry.sName, TRUE );
		// no short description TODO
		// no detailed description TODO
		// so only image left, its most exciting anyway:)
		{
			UINT16 x,y;
			UINT8 z;
			GetEncyclopediaSectorCoordsFromLocationID( newID, x, y, z );
			GetMapFileName( SECTORX( newID/(MAXIMUM_VALID_Z_COORDINATE+1) ), SECTORY( newID/(MAXIMUM_VALID_Z_COORDINATE+1) ), newID%(MAXIMUM_VALID_Z_COORDINATE+1), fileName, FALSE, TRUE );
			//remove file extension
			for( INT32 cnt = strlen(fileName)-1; cnt>=0; cnt-- )
				if( fileName[cnt] == '.' )
				{
					fileName[cnt] = '\0';
					break;
				}
			sprintf(vObjDesc.ImageFile, "Encyclopedia\\Maps\\%s.STI", fileName);
			vObjDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			if( !FileExists(vObjDesc.ImageFile) || !AddVideoObject( &vObjDesc, &gstEncyclopediaDataEntry.uiImageID ) )//no file? try loading tiny little radar image...
			{
				vObjDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
				sprintf(vObjDesc.ImageFile, "RadarMaps\\%s.STI", fileName);
				if( !FileExists(vObjDesc.ImageFile) || !AddVideoObject( &vObjDesc, &gstEncyclopediaDataEntry.uiImageID ) )//no file? no map...
				{
					//if( !LoadMapGraphicForLocation( NULL, &gstEncyclopediaDataEntry.uiImageID ) )
					gstEncyclopediaDataEntry.uiImageID = 0;
				}
			}
		}
		break;
	case ENC_CHARACTERS :
		CHECKF( newID < NUM_PROFILES );
		// set name
		std::memcpy( gstEncyclopediaDataEntry.sName, gMercProfiles[ newID ].zNickname, sizeof(gMercProfiles[ newID ].zNickname) );
		// set short description
		std::memcpy( gstEncyclopediaDataEntry.sShortDesc, gMercProfiles[ newID ].zName, sizeof(gMercProfiles[ newID ].zName) );
		//set detailed description
		{
			CHAR16 szMercInfo[800],szMercInfo2[800];
			//UINT16 strLen;
			szMercInfo[0] = szMercInfo2[0] = 0;
			if( !g_bUseXML_Strings )
			{//read encoded .dat
				//find aimbio or mercbio index matching gMercProfiles index
				for( UINT8 i = 0; i < NUM_PROFILES - 1; i++ )
					if( gAimAvailability[i].ProfilId == newID )
					{
						LoadEncryptedDataFromFile("BINARYDATA\\AIMBIOS.EDT", szMercInfo, 7*80*2*gAimAvailability[i].AimBio, 5*80*2);
						LoadEncryptedDataFromFile("BINARYDATA\\AIMBIOS.EDT", szMercInfo2, 7*80*2*gAimAvailability[i].AimBio + 5*80*2, 2*80*2 );
						break;
					}
					else if ( gConditionsForMercAvailability[i].ProfilId == newID )
					{
						LoadEncryptedDataFromFile("BINARYDATA\\MERCBIOS.EDT", szMercInfo, 7*80*2*gConditionsForMercAvailability[i].MercBio, 5*80*2);
						LoadEncryptedDataFromFile("BINARYDATA\\MERCBIOS.EDT", szMercInfo2, 7*80*2*gConditionsForMercAvailability[i].MercBio + 5*80*2, 2*80*2 );
						break;
					}
				//if no description reset to 0
				if( szMercInfo[0] == (CHAR16)-1 )
					szMercInfo[0] = 0;
				if( szMercInfo2[0] == (CHAR16)-1 )
					szMercInfo2[0] = 0;
			}
			else
			{//read .xml
				if( !Loc::GetString(Loc::AIM_BIOGRAPHY, L"Bio", newID, szMercInfo, 400) )
					szMercInfo[0] = 0;
				if( !Loc::GetString(Loc::AIM_BIOGRAPHY, L"Add", newID, szMercInfo2, 160) )
					szMercInfo2[0] = 0;
			}
			//print
			//strLen = wcslen( szMercInfo ) + wcslen( szMercInfo2 ) + 160;
			//if( gstEncyclopediaDataEntry.uiTextPage == 0 && (strLen < sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16)) )
			//{//both fit on one page
			//	wcscpy_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), szMercInfo );
			//	wcscat_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), L"                                                                               " );
			//	wcscat_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), L"                                                                               " );
			//	wcscat_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), szMercInfo2 );
			//}
			//else
				if( gstEncyclopediaDataEntry.uiTextPage == 0 )
			{//aditional info dont fit, print only bio
				wcscpy_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), szMercInfo );
			}
			else if( gstEncyclopediaDataEntry.uiTextPage == 1 )
			{//dont fit on single page, print additional info on page 1
				wcscpy_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), szMercInfo2 );
			}
			else if( gstEncyclopediaDataEntry.uiTextPage == 2 )
			{//2: debug text misc flag
				STR16 dbgText = { L"PROFILE=%d, FLAG_RECRUITED=%d, FLAG_HAVESEENCREATURE=%d, FLAG_FORCENPCQUOTE=%d, FLAG_WOUNDEDBYPLAYER=%d, FLAG_TEMP_NPC_QUOTE_DATA_EXISTS=%d, FLAG_SAID_HOSTILE_QUOTE=%d, FLAG_EPCACTIVE=%d, FLAG_ALREADY_USED_ITEMS=%d" }; 
				swprintf( gstEncyclopediaDataEntry.sDescr, dbgText, newID,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_HAVESEENCREATURE,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_FORCENPCQUOTE,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_WOUNDEDBYPLAYER,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_EPCACTIVE,
					gMercProfiles[newID].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS );
			}
			else if( gstEncyclopediaDataEntry.uiTextPage == 3 )
			{//3: debug text misc flag2
				STR16 dbgText = { L"PROFILE=%d, FLAG2_DONT_ADD_TO_SECTOR=%d, FLAG2_LEFT_COUNTRY=%d, FLAG2_BANDAGED_TODAY=%d, FLAG2_SAID_FIRSTSEEN_QUOTE=%d, FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE=%d, FLAG2_MARRIED_TO_HICKS=%d, FLAG2_ASKED_BY_HICKS=%d, FLAG2_MERC_GEARKIT_UNPAID=%d" }; 
				swprintf( gstEncyclopediaDataEntry.sDescr, dbgText, newID,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_LEFT_COUNTRY,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_BANDAGED_TODAY,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_MARRIED_TO_HICKS,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_ASKED_BY_HICKS,
					gMercProfiles[newID].ubMiscFlags2 & PROFILE_MISC_FLAG2_MERC_GEARKIT_UNPAID );
			}
			else if( gstEncyclopediaDataEntry.uiTextPage == 4 )
			{//4: debug text misc flag3
				STR16 dbgText = { L"PROFILE=%d, FLAG3_PLAYER_LEFT_MSG_FOR_MERC_AT_AIM=%d, FLAG3_PERMANENT_INSERTION_CODE=%d, FLAG3_PLAYER_HAD_CHANCE_TO_HIRE=%d, FLAG3_HANDLE_DONE_TRAVERSAL=%d, FLAG3_NPC_PISSED_OFF=%d, FLAG3_MERC_MERC_IS_DEAD_AND_QUOTE_SAID=%d, FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH=%d, FLAG3_GOODGUY=%d" }; 
				swprintf( gstEncyclopediaDataEntry.sDescr, dbgText, newID,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_PLAYER_LEFT_MSG_FOR_MERC_AT_AIM,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_PLAYER_HAD_CHANCE_TO_HIRE,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_NPC_PISSED_OFF,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_MERC_MERC_IS_DEAD_AND_QUOTE_SAID,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH,
					gMercProfiles[newID].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY );
			}
		}

		// create image
		if( gProfilesVehicle[newID].ProfilId == newID)
		{//this is a vehicle
			std::strcpy( vObjDesc.ImageFile, gNewVehicle[newID].szIconFace );
			vObjDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			if( !AddVideoObject(&vObjDesc, &gstEncyclopediaDataEntry.uiImageID) )
				gstEncyclopediaDataEntry.uiImageID = 0;
		}
		else
		{//not vehicle, process normal
			if(gGameExternalOptions.fReadProfileDataFromXML)
			{
				// HEADROCK PROFEX: Do not read direct profile number, instead, look inside the profile for a ubFaceIndex value.
				sprintf( fileName, "%02d.sti", gMercProfiles[newID].ubFaceIndex);
			}
			else
			{
				sprintf( fileName, "%02d.sti", newID);
			}
			//FilenameForBPP( vObjDesc.ImageFile, fileName); removed: no faces with [ID]_8.sti
			sprintf( vObjDesc.ImageFile, "faces\\BIGFACES\\%s", fileName );
			vObjDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		
			if( !AddVideoObject(&vObjDesc, &gstEncyclopediaDataEntry.uiImageID) )
			{
				//try with prefix 'B'
				sprintf( vObjDesc.ImageFile, "faces\\B%s",fileName );
				vObjDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
				if( !AddVideoObject(&vObjDesc, &gstEncyclopediaDataEntry.uiImageID) )
					gstEncyclopediaDataEntry.uiImageID = 0;
			}
		}
		break;
	case ENC_ITEMS :
		// set name
		std::memcpy( gstEncyclopediaDataEntry.sName, Item[ newID ].szItemName, sizeof(Item[ newID ].szItemName) );
		// set short description
		std::memcpy( gstEncyclopediaDataEntry.sShortDesc, Item[ newID ].szLongItemName, sizeof(Item[ newID ].szLongItemName) );
		// set detailed description
		if( gstEncyclopediaDataEntry.uiTextPage == 0 )
			std::memcpy( gstEncyclopediaDataEntry.sDescr, Item[ newID ].szItemDesc, sizeof(Item[ newID ].szItemDesc) );
		else
			std::memcpy( gstEncyclopediaDataEntry.sDescr, Item[ newID ].szBRDesc, sizeof(Item[ newID ].szBRDesc) );
		// load image
		if ( !LoadTileGraphicForItem( &Item[ newID ], &gstEncyclopediaDataEntry.uiImageID ) )
			gstEncyclopediaDataEntry.uiImageID = 0;
		break;
	case ENC_QUESTS :
		// set name
		CHAR16 questStartedString[160],questCompletedString[160];
		wcscpy_s( gstEncyclopediaDataEntry.sName, sizeof(gstEncyclopediaDataEntry.sName)/sizeof(CHAR16), QuestDescText[ newID ] );
		// set description
		GetQuestStartedString( (UINT8)newID, questStartedString );
		wcscpy_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), questStartedString );
		if( gubQuest[(UINT8) newID] == QUESTDONE )
		{
			wcscat_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), L"                                                                               " );
			wcscat_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), L"                                                                               " );
			GetQuestEndedString( (UINT8)newID, questCompletedString );
			wcscat_s( gstEncyclopediaDataEntry.sDescr, sizeof(gstEncyclopediaDataEntry.sDescr)/sizeof(CHAR16), questCompletedString );
		}
		break;
	default :
		return FALSE;
	}

	//finally set ID, everything else covered above
	gstEncyclopediaDataEntry.iID = newID;

	return TRUE;
}

/**
* Prepares a change to a different subpage.
*
* Subpage content depends on selected data type (@ref ENC_SUBPAGE_T).
* As there are more then UINT8 possible pages this function behaves differently then standart wwwLaptop function:
* It uses the parameter to go to first, next or previous data entry.
* @param ubDirection previous (0), next (1) or first (255) data entry.
* @see ENC_DATA_SUBPAGE_T
* @deprecated use CreateData() instead
*/
void ChangingEncyclopediaDataSubPage( UINT8 ubDirection )
{
}

//////////////
// Call backs

/** @addtogroup CALLBACK Callbacks
* @ingroup ENC_DATA
* @{
*/
extern void BtnEncyclopedia_newSelectDataPageBtnCallBack( GUI_BUTTON *btn, INT32 reason );

/**
* @brief Encyclopedia filter button call back.
* @details Handles all filter buttons. Uses userdata at index 1 for major filter and index 2 for subfilters.
* Subfilter gets reset on rightclick, and updated on leftclick.
* If you want to add new subfilters change the getPreviousNextXXXX function accordingly.
* To create new (major) filters update the matching filter enum, EnterEncyclopediaData(), ExitEncyclopediaData(). CreateData() and RenderEncyclopediaData() should adapt.
* Assumes that there is at least one filter button giEncyclopedia_DataFilterBtn[ 0 ]
*/
void BtnEncyclopedia_Data_FilterBtnCallBack( GUI_BUTTON *btn, INT32 reason )
{
	INT16 nextData = ENC_DATA_NO_DATA;

	CHECKV( gGameExternalOptions.gEncyclopedia );
	CHECKV( btn );

	UINT8 userDataFilterMode = (UINT8)MSYS_GetBtnUserData( btn, 1 );
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( userDataFilterMode == 0 )
		{
			GUI_BUTTON *tmpButton;
			// first button clicked (show all), reset filters
			geEncyclopediaDataFilterLocation = ENC_DATA_FILTER_L_ALL;
			geEncyclopediaDataFilterCharacter = ENC_DATA_FILTER_C_ALL;
			geEncyclopediaDataFilterItem = ENC_DATA_FILTER_I_ALL;
			geEncyclopediaDataFilterQuest = ENC_DATA_FILTER_Q_ALL;
			guiEncyclopediaDataSubFilter = 0;
			// data only needs to be updated if a subfilter was selected with no data otherwise it should show up in 'all'
			if( gstEncyclopediaDataEntry.iID == ENC_DATA_NO_DATA )
			{
				if( geENC_SubPage == ENC_LOCATIONS )
					CreateData( GetNextPreviousLocation( ENC_DATA_NO_DATA, ENC_DATA_NEXT, ENC_DATA_FILTER_L_ALL, 0 ) );
				else if( geENC_SubPage == ENC_CHARACTERS )
					CreateData( GetNextPreviousCharacter( ENC_DATA_NO_DATA, ENC_DATA_NEXT, ENC_DATA_FILTER_C_ALL, 0 ) );
				else if( geENC_SubPage == ENC_ITEMS )
					CreateData( GetNextPreviousItem( ENC_DATA_NO_DATA, ENC_DATA_NEXT, ENC_DATA_FILTER_I_ALL, 0 ) );
				else if( geENC_SubPage == ENC_QUESTS )
					CreateData( GetNextPreviousQuest( ENC_DATA_NO_DATA, ENC_DATA_NEXT, ENC_DATA_FILTER_Q_ALL, 0 ) );
				// since we changed data render again
				if( gstEncyclopediaDataEntry.iID != ENC_DATA_NO_DATA )
					fPausedReDrawScreenFlag = TRUE;
			}
			// deselect all other filter buttons
			for( UINT8 i = 1; i < sizeof(giEncyclopedia_DataFilterBtn)/sizeof(INT32); i++ )
			{
				if( giEncyclopedia_DataFilterBtn[ i ] != BUTTON_NO_SLOT )
				{
					tmpButton = GetButtonPtr( giEncyclopedia_DataFilterBtn[ i ] );
					tmpButton->uiFlags &= (~BUTTON_CLICKED_ON);
					InvalidateRegion(tmpButton->Area.RegionTopLeftX, tmpButton->Area.RegionTopLeftY, tmpButton->Area.RegionBottomRightX, tmpButton->Area.RegionBottomRightY);
				}
			}
			// Disable 'All' button to mark it as selected
			DisableButton( giEncyclopedia_DataFilterBtn[ 0 ] );
			// activate navigation buttons
			EnableButton( giEncyclopedia_DataBtn[ 2 ] );
			EnableButton( giEncyclopedia_DataBtn[ 4 ] );
		}
		else
		{
			// other button then first pressed, check if allready down

			guiEncyclopediaDataSubFilter = (UINT16)MSYS_GetBtnUserData( btn, 2 );

			if( btn->uiFlags & BUTTON_CLICKED_ON )
			{
				// button down

				// set new button text, userdata and global subfilter
				switch( geENC_SubPage )
				{
				case ENC_LOCATIONS :
					// set major filter
					//geEncyclopediaDataFilterLocation = static_cast<ENC_DATA_FILTER_LOCATION>(userDataFilterMode);
					// select next subfilter
					switch( geEncyclopediaDataFilterLocation )
					{
					case ENC_DATA_FILTER_L_FACILITY :
						// Only Facility has subfilters: gFacilityTypes (variable defined in xml) using 0 for all facilities
						if( guiEncyclopediaDataSubFilter >= NUM_FACILITY_TYPES - 1)
							guiEncyclopediaDataSubFilter = 0;
						else
							guiEncyclopediaDataSubFilter++;
						CHAR16 tempStr[8];
						if( guiEncyclopediaDataSubFilter )
							wcsncpy( tempStr, gFacilityTypes[ guiEncyclopediaDataSubFilter ].szFacilityShortName, 7 );// limit text to 7 chars
							//SpecifyButtonText( btn->IDNum, gFacilityTypes[ guiEncyclopediaDataSubFilter ].szFacilityShortName );
						else
							wcsncpy( tempStr, GetDataFilterText( pEncyclopediaFilterLocationText, pEncyclopediaSubFilterLocationText, geEncyclopediaDataFilterLocation ), 7 );
							//SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterLocationText, pEncyclopediaSubFilterLocationText, geEncyclopediaDataFilterLocation ) );
						tempStr[7] = 0;
						SpecifyButtonText( btn->IDNum, tempStr );
						break;
					default :
						guiEncyclopediaDataSubFilter = 0;
					}
					
					MSYS_SetBtnUserData( btn->IDNum, 2, guiEncyclopediaDataSubFilter );
					nextData = GetNextPreviousLocation( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterLocation, guiEncyclopediaDataSubFilter );
					break;
				case ENC_CHARACTERS :
					// set major filter
					//geEncyclopediaDataFilterCharacter = static_cast<ENC_DATA_FILTER_CHARACTER>(userDataFilterMode);
					switch( geEncyclopediaDataFilterCharacter )
					{
					case ENC_DATA_FILTER_C_OTHER :
						// Only OTHER has 3 subfilter: EPC, vehicle and other, switching these states
						if( guiEncyclopediaDataSubFilter > 1 )
							guiEncyclopediaDataSubFilter = 0;
						else
							guiEncyclopediaDataSubFilter++;
						MSYS_SetBtnUserData( btn->IDNum, 2, guiEncyclopediaDataSubFilter );
						SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterCharText, pEncyclopediaSubFilterCharText, ENC_DATA_FILTER_C_OTHER ) );
						break;
					default :
						// All remaining filters have no subfilters, userdata and button text wont change.
						guiEncyclopediaDataSubFilter = 0;
					}
					nextData = GetNextPreviousCharacter( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterCharacter, guiEncyclopediaDataSubFilter );
					break;
				case ENC_ITEMS :
					// set major filter
					//geEncyclopediaDataFilterItem = static_cast<ENC_DATA_FILTER_ITEM>(userDataFilterMode);
					// select next sub filter
					switch( geEncyclopediaDataFilterItem )
					{
					case ENC_DATA_FILTER_I_ALL :
						//All has no subfilters, userdata and button text wont change as well
						guiEncyclopediaDataSubFilter = 0;
						break;
					case ENC_DATA_FILTER_I_GUN :
						// guns have 9 subfilters plus one for all guns (using NOT_GUN). Subfilter is 'weapon exact type' see Weapons.h except for heavy weapon (using index 9)
						if( (guiEncyclopediaDataSubFilter - 1) == GUN_SHOTGUN )
							// last subfilter, reset
							guiEncyclopediaDataSubFilter = NOT_GUN;
						else
							// get next subfilter
							guiEncyclopediaDataSubFilter++;
						SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterItemText, pEncyclopediaSubFilterItemText, ENC_DATA_FILTER_I_GUN ) );
						break;
					case ENC_DATA_FILTER_I_AMMO :
						// ammunition has 9 subfilters plus one for all (using NOT_GUN). Subfilter is 'weapon exact type' see Weapons.h, except for grenades (using index 9)
						if( (guiEncyclopediaDataSubFilter - 1) == GUN_SHOTGUN )
							guiEncyclopediaDataSubFilter = NOT_GUN;
						else
							guiEncyclopediaDataSubFilter++;
						//SpecifyButtonText( btn->IDNum, (guiEncyclopediaDataSubFilter == NOT_GUN? BobbyRText[4] : (guiEncyclopediaDataSubFilter < 9? BobbyRFilter[guiEncyclopediaDataSubFilter+8] : BobbyRFilter[28])) );
						SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterItemText, pEncyclopediaSubFilterItemText, ENC_DATA_FILTER_I_AMMO ) );
						break;
					case ENC_DATA_FILTER_I_ARMOUR :
						// armor has 4 subfilters plus one for all (using 0 for all and shifting all others). Subfilter is 'ARMOUR CLASSES' + 1 see Weapons.h
						if( guiEncyclopediaDataSubFilter == ARMOURCLASS_PLATE + 1 )
							guiEncyclopediaDataSubFilter = 0;
						else
							guiEncyclopediaDataSubFilter++;
						SpecifyButtonText( btn->IDNum, (guiEncyclopediaDataSubFilter == 0? pEncyclopediaFilterItemText[3] : pEncyclopediaSubFilterItemText[guiEncyclopediaDataSubFilter + 19]) );
						break;
					case ENC_DATA_FILTER_I_LBE :
						// LBE has 6 subfilters plus one for all (using 0). Subfilter is enum eLBE_CLASS (1..6)
						if( guiEncyclopediaDataSubFilter == OTHER_POCKET )
							guiEncyclopediaDataSubFilter = 0;
						else
							guiEncyclopediaDataSubFilter++;
						SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterItemText, pEncyclopediaSubFilterItemText, ENC_DATA_FILTER_I_LBE ) );
						break;
					case ENC_DATA_FILTER_I_ATTACHMENT :
						// attachment has 5 subfilters plus one for all (using 0). Subfilter is BOBBYR_FILTER_MISC_OPTICS_ATTACHMENTS to BOBBYR_FILTER_MISC_OTHER_ATTACHMENTS and 0.
						if( guiEncyclopediaDataSubFilter == BOBBYR_FILTER_MISC_OTHER_ATTACHMENTS )
							guiEncyclopediaDataSubFilter = 0;
						else if( guiEncyclopediaDataSubFilter < BOBBYR_FILTER_MISC_OPTICS_ATTACHMENTS )
							guiEncyclopediaDataSubFilter = BOBBYR_FILTER_MISC_OPTICS_ATTACHMENTS;
						else
							guiEncyclopediaDataSubFilter++;
						SpecifyButtonText( btn->IDNum, (guiEncyclopediaDataSubFilter == 0? pEncyclopediaFilterItemText[5] : pEncyclopediaSubFilterItemText[guiEncyclopediaDataSubFilter - 2 ]) );
						break;
					case ENC_DATA_FILTER_I_MISC :
						// misc has 9 subfilters plus one for all. Subfilter is BOBBYR_FILTER_MISC_BLADE...:25-32 + 40 + 0, moved 40 to 33 for convinience.
						if( guiEncyclopediaDataSubFilter == BOBBYR_FILTER_MISC_MISC )
							// last subfilter, reset
							guiEncyclopediaDataSubFilter = 0;
						else if( guiEncyclopediaDataSubFilter < BOBBYR_FILTER_MISC_BLADE )
							guiEncyclopediaDataSubFilter = BOBBYR_FILTER_MISC_BLADE;
						else if( guiEncyclopediaDataSubFilter == BOBBYR_FILTER_MISC_FACE )
							guiEncyclopediaDataSubFilter = BOBBYR_FILTER_MISC_MISC;
						else
							guiEncyclopediaDataSubFilter++;
						SpecifyButtonText( btn->IDNum, (guiEncyclopediaDataSubFilter == 0? pEncyclopediaFilterItemText[6] : 
							(guiEncyclopediaDataSubFilter == BOBBYR_FILTER_MISC_MISC? pEncyclopediaSubFilterItemText[ 47 ] : pEncyclopediaSubFilterItemText[ guiEncyclopediaDataSubFilter + 14 ]) ) );
						break;
					}

					MSYS_SetBtnUserData( btn->IDNum, 2, guiEncyclopediaDataSubFilter );
					nextData = GetNextPreviousItem( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterItem, guiEncyclopediaDataSubFilter );
					break;
				case ENC_QUESTS :
					//set major filter
					//geEncyclopediaDataFilterQuest = static_cast<ENC_DATA_FILTER_QUEST>(userDataFilterMode);
					// no subfilters for quests
					guiEncyclopediaDataSubFilter = 0;
					MSYS_SetBtnUserData( btn->IDNum, 2, guiEncyclopediaDataSubFilter );
					nextData = GetNextPreviousQuest( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterQuest, guiEncyclopediaDataSubFilter );
					//TODO
					break;
				default :
					Assert(0);
				}
			} //end filter button down (not first)
			else
			{
				// filter button up (not first),...
				guiEncyclopediaDataSubFilter = (UINT16)MSYS_GetBtnUserData( btn, 2 );

				// ... change to down,...
				btn->uiFlags |= BUTTON_CLICKED_ON;
				InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
				// ... update data...
				switch( geENC_SubPage )
				{
				case ENC_LOCATIONS :
					geEncyclopediaDataFilterLocation = static_cast<ENC_DATA_FILTER_LOCATION>(userDataFilterMode);
					nextData = GetNextPreviousLocation( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterLocation, guiEncyclopediaDataSubFilter );
					break;
				case ENC_CHARACTERS :
					geEncyclopediaDataFilterCharacter = static_cast<ENC_DATA_FILTER_CHARACTER>(userDataFilterMode);
					nextData = GetNextPreviousCharacter( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterCharacter, guiEncyclopediaDataSubFilter );
					break;
				case ENC_ITEMS :
					geEncyclopediaDataFilterItem = static_cast<ENC_DATA_FILTER_ITEM>(userDataFilterMode);
					nextData = GetNextPreviousItem( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterItem, guiEncyclopediaDataSubFilter );
					break;
				case ENC_QUESTS :
					geEncyclopediaDataFilterQuest = static_cast<ENC_DATA_FILTER_QUEST>(userDataFilterMode);
					nextData = GetNextPreviousQuest( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterQuest, guiEncyclopediaDataSubFilter );
					break;
				default :
					Assert(0);
				}

			} // end filter button up (not first)
			CreateData( nextData );
			// ...and activate first button.
			EnableButton( giEncyclopedia_DataFilterBtn[ 0 ] );
			// activate navigation buttons
			EnableButton( giEncyclopedia_DataBtn[ 2 ] );
			EnableButton( giEncyclopedia_DataBtn[ 4 ] );
			// release all other buttons except this one
			for( UINT8 i = 0; i < sizeof(giEncyclopedia_DataFilterBtn)/sizeof(INT32); i++ )
			{
				if( giEncyclopedia_DataFilterBtn[i] != btn->IDNum && giEncyclopedia_DataFilterBtn[i] != BUTTON_NO_SLOT )
				{
					GetButtonPtr(giEncyclopedia_DataFilterBtn[i])->uiFlags &= (~BUTTON_CLICKED_ON);
				}
			}
			// render new data
			fPausedReDrawScreenFlag = TRUE;
		}//end not first filter button pressed
	}//end reason & MSYS_CALLBACK_REASON_LBUTTON_UP
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_UP && userDataFilterMode != 0 )
	{
		//filterbuttons reset subfilter on rightclick
		// mark button as pressed...
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		// ...reset subfilter...
		guiEncyclopediaDataSubFilter = 0;
		MSYS_SetBtnUserData( btn->IDNum, 2, guiEncyclopediaDataSubFilter );
		// ...get new data and update button text and filters...
		switch( geENC_SubPage )
		{
		case ENC_LOCATIONS :
			geEncyclopediaDataFilterLocation = static_cast<ENC_DATA_FILTER_LOCATION>(userDataFilterMode);
			SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterLocationText, pEncyclopediaSubFilterLocationText, geEncyclopediaDataFilterLocation ) );
			nextData = GetNextPreviousLocation( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterLocation, guiEncyclopediaDataSubFilter );
			break;
		case ENC_CHARACTERS :
			geEncyclopediaDataFilterCharacter = static_cast<ENC_DATA_FILTER_CHARACTER>(userDataFilterMode);
			SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterCharText, pEncyclopediaSubFilterCharText, geEncyclopediaDataFilterCharacter ) );
			nextData = GetNextPreviousCharacter( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterCharacter, guiEncyclopediaDataSubFilter );
			break;
		case ENC_ITEMS :
			geEncyclopediaDataFilterItem = static_cast<ENC_DATA_FILTER_ITEM>(userDataFilterMode);;
			SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterItemText, pEncyclopediaSubFilterItemText, geEncyclopediaDataFilterItem ) );
			nextData = GetNextPreviousItem( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterItem, guiEncyclopediaDataSubFilter );
			break;
		case ENC_QUESTS :
			geEncyclopediaDataFilterQuest = static_cast<ENC_DATA_FILTER_QUEST>(userDataFilterMode);
			SpecifyButtonText( btn->IDNum, GetDataFilterText( pEncyclopediaFilterQuestText, pEncyclopediaSubFilterQuestText, geEncyclopediaDataFilterQuest ) );
			nextData = GetNextPreviousQuest( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterQuest, guiEncyclopediaDataSubFilter );
			break;
		default :
			Assert(0);
		}
		CreateData( nextData );
		// ...and activate first button.
		EnableButton( giEncyclopedia_DataFilterBtn[ 0 ] );
		// activate navigation buttons
		EnableButton( giEncyclopedia_DataBtn[ 2 ] );
		EnableButton( giEncyclopedia_DataBtn[ 4 ] );
		// release all other buttons except this one
		for( UINT8 i = 0; i < sizeof(giEncyclopedia_DataFilterBtn)/sizeof(INT32); i++ )
		{
			if( giEncyclopedia_DataFilterBtn[i] != btn->IDNum && giEncyclopedia_DataFilterBtn[i] != BUTTON_NO_SLOT )
			{
				GetButtonPtr(giEncyclopedia_DataFilterBtn[i])->uiFlags &= (~BUTTON_CLICKED_ON);
			}
		}
		// render new data
		fPausedReDrawScreenFlag = TRUE;
		// play button down sound (only required for right mouse click)
		PlayButtonSound( btn->IDNum, BUTTON_SOUND_CLICKED_OFF );
	}

}

/**
* @brief Encyclopedia text page button callback.
* @details Handles switching between different text pages within the same data entry.
*/
void BtnEncyclopedia_Data_NextPreviousTextBtnCallBack( GUI_BUTTON *btn, INT32 reason )
{
	CHECKV( gGameExternalOptions.gEncyclopedia );
	CHECKV( btn );
	BOOLEAN next = (BOOLEAN)MSYS_GetBtnUserData( btn, 1 );

	if( (reason & MSYS_CALLBACK_REASON_LBUTTON_UP) || (reason & MSYS_CALLBACK_REASON_RBUTTON_UP) )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY );
#ifdef _DEBUG
		gstEncyclopediaDataEntry.uiTextPage += next? 1 : -1;
#else
		if( gstEncyclopediaDataEntry.uiTextPage == 0 && next )
			gstEncyclopediaDataEntry.uiTextPage = 1;
		else if( gstEncyclopediaDataEntry.uiTextPage == 1 && !next )
			gstEncyclopediaDataEntry.uiTextPage = 0;
		else return;
#endif
		CreateData( gstEncyclopediaDataEntry.iID );
		fPausedReDrawScreenFlag = TRUE;
	}
}

/**
* @brief Encyclopedia data page button callback.
* @details Handles traversing thru data either forward or backward. Uses userdata at index 1 to determine which direction to go.
*/
void BtnEncyclopedia_Data_NextPreviousDataBtnCallBack ( GUI_BUTTON *btn, INT32 reason )
{
	CHECKV( gGameExternalOptions.gEncyclopedia );
	CHECKV( btn );

	if ( (reason & MSYS_CALLBACK_REASON_LBUTTON_UP) || (reason & MSYS_CALLBACK_REASON_RBUTTON_UP) )
	{
		INT16 nextData = ENC_DATA_NO_DATA;
		BOOLEAN next = (BOOLEAN)MSYS_GetBtnUserData( btn, 1 );
		// get next or previous data depending on which button was pressed
		switch (geENC_SubPage)
		{
		case ENC_LOCATIONS :
			nextData = GetNextPreviousLocation( gstEncyclopediaDataEntry.iID, next, geEncyclopediaDataFilterLocation, guiEncyclopediaDataSubFilter );
			break;
		case ENC_CHARACTERS :
			nextData = GetNextPreviousCharacter( gstEncyclopediaDataEntry.iID, next, geEncyclopediaDataFilterCharacter, guiEncyclopediaDataSubFilter );
			break;
		case ENC_ITEMS :
			nextData = GetNextPreviousItem( gstEncyclopediaDataEntry.iID, next, geEncyclopediaDataFilterItem, guiEncyclopediaDataSubFilter );
			break;
		case ENC_QUESTS :
			nextData = GetNextPreviousQuest( gstEncyclopediaDataEntry.iID, next, geEncyclopediaDataFilterQuest, guiEncyclopediaDataSubFilter );
			break;
		default:
			Assert(0);
		}
		// release button
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		// check for changed data
		if( nextData == ENC_DATA_NO_DATA )
		{
			// no data?...
			if (next)
			{
				// ...disable next button.
				CHECKV( giEncyclopedia_DataBtn[4] );
				DisableButton( giEncyclopedia_DataBtn[4] );
			}
			else
			{
				// ...disable previous button.
				CHECKV( giEncyclopedia_DataBtn[2] );
				DisableButton( giEncyclopedia_DataBtn[2] );
			}
		}
		else if( nextData != gstEncyclopediaDataEntry.iID )
		{
			//new data?
			CreateData( nextData );
			// reenable button
			if (next)
			{
				// ...enable previous button.
				CHECKV( giEncyclopedia_DataBtn[2] );
				EnableButton( giEncyclopedia_DataBtn[2] );
			}
			else
			{
				// ...enable next button.
				CHECKV( giEncyclopedia_DataBtn[4] );
				EnableButton( giEncyclopedia_DataBtn[4] );
			}
			// rerender screen since we changed data.
			fPausedReDrawScreenFlag = TRUE;
		}
	}
}
///@}

//////////////
// laptop functions

/** @defgroup ENC_DATA Data page
* Encyclopedia data page.
* @ingroup ENCYCLOPEDIA
* @{*/
/**
* Checks for all needed files (graphics, xmls, ini option).
*
* Called when a new game is started (guiCurrentScreen == 0), entering new game screen/loading game from main menu (guiCurrentScreen == MAINMENU_SCREEN) and quitting game (guiCurrentScreen == MSG_BOX_SCREEN).
* Sets gui element initial values.
* Shows error message and deactivates the Encyclopedia if a file is missing.
* @todo check for all files needed by data page here.
*/
void GameInitEncyclopediaData_NEW(  )
{
	//initialize UI elements
	memset( giEncyclopedia_DataBtn, BUTTON_NO_SLOT, sizeof(giEncyclopedia_DataBtn) );
	giEncyclopedia_DataBtnImage = BUTTON_NO_IMAGE;
	memset( giEncyclopedia_DataFilterBtn, BUTTON_NO_SLOT, sizeof(giEncyclopedia_DataFilterBtn) );
	giEncyclopedia_DataFilterBtnImage = BUTTON_NO_IMAGE;
#if 0//debug
	memset( gbEncyclopediaData_ItemVisible, ENC_ITEM_DISCOVERED_NOT_REACHABLE, sizeof(gbEncyclopediaData_ItemVisible) );
	gbEncyclopediaData_ItemVisible[1] = ENC_ITEM_NOT_DISCOVERED;
#else
	if( guiCurrentScreen == MAINMENU_SCREEN )
		EncyclopediaInitItemsVisibility();
#endif
	// do following only once at start of JA2
	CHECKV( guiCurrentScreen == 0 );
	//prepare indexes for subfilter texts defined in _LanguageText.cpp, assuming there are blank separators between filter button texts ("1", "2", "3", "", "1", "", "1", "2", "3", "4")
	STR16 *pText, testStr;
	UINT8 failsafe = 1, index;
	for( UINT8 site = 1; site <= ENC_NUM_SUBPAGES; site++ )
	{
		//get text pointer
		switch(site)
		{
		case ENC_LOCATIONS :
			pText = pEncyclopediaSubFilterLocationText;
			break;
		case ENC_CHARACTERS :
			pText = pEncyclopediaSubFilterCharText;
			break;
		case ENC_ITEMS :
			pText = pEncyclopediaSubFilterItemText;
			break;
		case ENC_QUESTS :
			pText = pEncyclopediaSubFilterQuestText;
			break;
		default : 
			AssertMsg(0, "Unrecognized DataPage in GameInitEncyclopediaData()");
			return;
		}
		index = 0;
		for( UINT8 filter = 0; filter < ENC_DATA_MAX_FILTERS-1; filter++ )
			if(filter == 0) guiEncyclopediaSubFilterTextIndexStart[site-1][filter] = 0;
			else
			{
				testStr = pText[index],failsafe = 1;
				while( failsafe && testStr[0] != L'\0' ) testStr = pText[++index], failsafe++;
				guiEncyclopediaSubFilterTextIndexStart[site-1][filter] = ++index;
			}
	}
}

/**
* Helper function gets called AFTER rendering by game loop.
*
* Can be used to poll keyboard or mouse events.
* Can also be used for animations (e.g. showing blinking position of person on a map, simulate loading of commercials, animated zoom ec.).
*/
void HandleEncyclopediaData_NEW(  )
{
}

/**
* Creates video objects, buttons, fasthelp texts, mousregions.
*
* Called when entering encyclopedia data web page in laptop. Prepares data page of Encyclopedia.
* @return FALSE on error. TRUE on success.
* @see ExitEncyclopediaData_NEW()
*/
BOOLEAN EnterEncyclopediaData_NEW(  )
{
	VOBJECT_DESC	VObjectDesc;
	INT16			sNewItem = -1;
	extern UINT32	guiEncyclopediaBG;//background image from Encyclopedia main page
	UINT16 filterButtonSizeX, filterButtonSizeY, buttonSizeX, buttonSizeY;
	UINT8 numFilterButtons = 0, filterButtonHelpTextFirstID;
	STR16 *filterButtonText, *filterButtonHelpText;

	//error if not enabled
	CHECKF( gGameExternalOptions.gEncyclopedia );
	//error if the laptop mode does not match encyclopedia
	CHECKF( LAPTOP_MODE_ENCYCLOPEDIA_DATA == guiCurrentLaptopMode );

	SetBookMark( ENCYCLOPEDIA_BOOKMARK );//bookmark should be allways set, just in case it is not.
	//////
	// load the background graphic
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "ENCYCLOPEDIA\\encyclopediabackground.sti", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiEncyclopediaBG ) );

	//////
	// get first data, get number of buttons and text
	guiEncyclopediaDataSubFilter = 0;
	switch( geENC_SubPage )
	{
		case ENC_LOCATIONS :
			//reset filter
			geEncyclopediaDataFilterLocation = ENC_DATA_FILTER_L_ALL;
			//find first item
			sNewItem = GetNextPreviousLocation( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterLocation, guiEncyclopediaDataSubFilter );
			//get number of buttons
			numFilterButtons = ENC_DATA_FILTER_L_OTHER + 1;
			//get button texts and helptexts
			filterButtonText = pEncyclopediaFilterLocationText;
			filterButtonHelpText = pEncyclopediaFilterLocationText;
			filterButtonHelpTextFirstID = numFilterButtons;
			break;
		case ENC_CHARACTERS :
			geEncyclopediaDataFilterCharacter = ENC_DATA_FILTER_C_ALL;
			sNewItem = GetNextPreviousCharacter( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterCharacter, guiEncyclopediaDataSubFilter );
			numFilterButtons = ENC_DATA_FILTER_C_OTHER + 1;
			filterButtonText = pEncyclopediaFilterCharText;
			filterButtonHelpText = pEncyclopediaFilterCharText;
			filterButtonHelpTextFirstID = numFilterButtons;
			break;
		case ENC_ITEMS :
			geEncyclopediaDataFilterItem = ENC_DATA_FILTER_I_ALL;
			sNewItem = GetNextPreviousItem( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterItem, guiEncyclopediaDataSubFilter );
			numFilterButtons = ENC_DATA_FILTER_I_MISC + 1;
			filterButtonText = pEncyclopediaFilterItemText;
			filterButtonHelpText = pEncyclopediaFilterItemText;
			filterButtonHelpTextFirstID = numFilterButtons;
			break;
		case ENC_QUESTS :
			geEncyclopediaDataFilterQuest = ENC_DATA_FILTER_Q_ALL;
			sNewItem = GetNextPreviousQuest( ENC_DATA_NO_DATA, ENC_DATA_NEXT, geEncyclopediaDataFilterQuest, guiEncyclopediaDataSubFilter );
			numFilterButtons = ENC_DATA_FILTER_Q_COMPLETE + 1;
			filterButtonText = pEncyclopediaFilterQuestText;
			filterButtonHelpText = pEncyclopediaFilterQuestText;
			filterButtonHelpTextFirstID = numFilterButtons;
			break;
		default :
			AssertMsg(0, "Unrecognized DataPage in EnterEncyclopediaData()");
			return (FALSE);
	}

	//////
	// load filter button graphic and get size, NOTE: Needs to be loaded to determine size for other UI elements, even when no filter buttons will be created.
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	giEncyclopedia_DataFilterBtnImage = LoadButtonImage( "ENCYCLOPEDIA\\BUTTONF.sti", BUTTON_NO_IMAGE, 0, BUTTON_NO_IMAGE , 1, BUTTON_NO_IMAGE );
	CHECKF( giEncyclopedia_DataFilterBtnImage != BUTTON_NO_IMAGE );
	filterButtonSizeX = GetWidthOfButtonPic( giEncyclopedia_DataFilterBtnImage, 0 );
	filterButtonSizeY = GetHeightOfButtonPic ( giEncyclopedia_DataFilterBtnImage, 0 );

	//////
	// create filter buttons
	for( UINT8 i = 0; i < numFilterButtons; i++ )
	{
		giEncyclopedia_DataFilterBtn[ i ] = CreateIconAndTextButton( giEncyclopedia_DataFilterBtnImage,//image
			filterButtonText[ i ], BLOCKFONTNARROW, FONT_MCOLOR_DKWHITE, NO_SHADOW, FONT_GRAY4, NO_SHADOW, TEXT_CJUSTIFIED,//text
			LAPTOP_SCREEN_UL_X + ENC_DATA_GAP, //x position: left side
			LAPTOP_SCREEN_WEB_UL_Y + ENC_DATA_FILTER_GAP_TOP + i * (filterButtonSizeY + ENC_DATA_GAP),//y position: top + gap + previous buttons with gaps
			BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, BUTTON_NO_CALLBACK, BtnEncyclopedia_Data_FilterBtnCallBack );
		//created?
		CHECKF( giEncyclopedia_DataFilterBtn[ i ] != BUTTON_NO_SLOT );
		//cursor
		SetButtonCursor( giEncyclopedia_DataFilterBtn[ i ], CURSOR_LAPTOP_SCREEN );
		//user data for filters
		MSYS_SetBtnUserData( giEncyclopedia_DataFilterBtn[ i ], 1, i );//Filter, fixed per button.
		MSYS_SetBtnUserData( giEncyclopedia_DataFilterBtn[ i ], 2, 0 );//SubFilter, gets updated on button click. This way we can 'load' the setting of a filter button.
		//fast help text
		SetButtonFastHelpText( giEncyclopedia_DataFilterBtn[ i ], filterButtonHelpText[ i + filterButtonHelpTextFirstID ] );
		///TODO: disabling shifted button icon/text does not work
		//GetButtonPtr( giEncyclopedia_DataFilterBtn[ i ] )->fShiftImage = FALSE;
	}
	// set first filter button as selected
	if( numFilterButtons )
	{
		DisableButton( giEncyclopedia_DataFilterBtn[ 0 ] );
	}

	//////
	// load button graphic for next/previous/main page/nextText/previousText and get size
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	giEncyclopedia_DataBtnImage = LoadButtonImage( "ENCYCLOPEDIA\\BOTTOMBUTTONS2.STI", BUTTON_NO_IMAGE, 0, BUTTON_NO_IMAGE , 1, BUTTON_NO_IMAGE );
	CHECKF( giEncyclopedia_DataBtnImage != BUTTON_NO_IMAGE );
	buttonSizeX = GetWidthOfButtonPic( giEncyclopedia_DataBtnImage, 0 );
	buttonSizeY = GetHeightOfButtonPic( giEncyclopedia_DataBtnImage, 0 );

	//////
	// create previousText button
	giEncyclopedia_DataBtn[ 0 ] = CreateIconAndTextButton( giEncyclopedia_DataBtnImage,//image
		pLocationPageText[ 0 ], FONT10ARIAL, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, TEXT_CJUSTIFIED,//text
		LAPTOP_SCREEN_UL_X + filterButtonSizeX + 2*ENC_DATA_GAP,//x position: left alligned to image box
		LAPTOP_SCREEN_WEB_UL_Y + GetFontHeight(FONT14ARIAL) + ENC_DATA_IMAGE_HEIGHT + ENC_DATA_SHORT_DESC_HEIGHT + 5*ENC_DATA_GAP,//y position: below short descr box
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, BtnEncyclopedia_Data_NextPreviousTextBtnCallBack );
	CHECKF( giEncyclopedia_DataBtn[ 0 ] != BUTTON_NO_SLOT );
	MSYS_SetBtnUserData( giEncyclopedia_DataBtn[ 0 ], 1 , FALSE );//previous -> next = FALSE

	//////
	// create nextText button
	giEncyclopedia_DataBtn[ 1 ] = CreateIconAndTextButton( giEncyclopedia_DataBtnImage,//image
		pLocationPageText[ 2 ], FONT10ARIAL, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, TEXT_CJUSTIFIED,//text
		LAPTOP_SCREEN_LR_X - filterButtonSizeX - 2*ENC_DATA_GAP - buttonSizeX,//x position: right alligned to image box
		LAPTOP_SCREEN_WEB_UL_Y + GetFontHeight(FONT14ARIAL) + ENC_DATA_IMAGE_HEIGHT + ENC_DATA_SHORT_DESC_HEIGHT + 5*ENC_DATA_GAP,//y position: below short descr box
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, BtnEncyclopedia_Data_NextPreviousTextBtnCallBack );
	CHECKF( giEncyclopedia_DataBtn[ 1 ] != BUTTON_NO_SLOT );
	MSYS_SetBtnUserData( giEncyclopedia_DataBtn[ 1 ], 1 , TRUE );//next -> next = TRUE

	//////
	// create previousData button
	giEncyclopedia_DataBtn[ 2 ] = CreateIconAndTextButton( giEncyclopedia_DataBtnImage,//image
		pSectorPageText[ 0 ], FONT10ARIAL, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, TEXT_CJUSTIFIED,//text
		LAPTOP_SCREEN_UL_X + LAPTOP_SCREEN_WIDTH/2 - 2*buttonSizeX,//x position: center of laptop - 2* button size
		LAPTOP_SCREEN_WEB_UL_Y + GetFontHeight(FONT14ARIAL) + ENC_DATA_IMAGE_HEIGHT + ENC_DATA_SHORT_DESC_HEIGHT + buttonSizeY + ENC_DATA_DESC_HEIGHT + 7*ENC_DATA_GAP,//y position: below all
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, BtnEncyclopedia_Data_NextPreviousDataBtnCallBack );
	CHECKF( giEncyclopedia_DataBtn[ 2 ] != BUTTON_NO_SLOT );
	MSYS_SetBtnUserData( giEncyclopedia_DataBtn[ 2 ], 1 , FALSE );//previous -> next = FALSE

	//////
	// create main page button
	giEncyclopedia_DataBtn[ 3 ] = CreateIconAndTextButton( giEncyclopedia_DataBtnImage,//image
		pSectorPageText[ 1 ], FONT10ARIAL, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, TEXT_CJUSTIFIED,//text
		LAPTOP_SCREEN_UL_X + LAPTOP_SCREEN_WIDTH/2 - buttonSizeX/2,//x position: center of laptop
		LAPTOP_SCREEN_WEB_UL_Y + GetFontHeight(FONT14ARIAL) + ENC_DATA_IMAGE_HEIGHT + ENC_DATA_SHORT_DESC_HEIGHT + buttonSizeY + ENC_DATA_DESC_HEIGHT + 7*ENC_DATA_GAP,//y position: below all
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, BtnEncyclopedia_newSelectDataPageBtnCallBack );
	CHECKF( giEncyclopedia_DataBtn[ 3 ] != BUTTON_NO_SLOT );
	MSYS_SetBtnUserData( giEncyclopedia_DataBtn[ 3 ], 1 , ENC_MAINPAGE );

	//////
	// create nextData button
	giEncyclopedia_DataBtn[ 4 ] = CreateIconAndTextButton( giEncyclopedia_DataBtnImage,//image
		pSectorPageText[ 2 ], FONT10ARIAL, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, TEXT_CJUSTIFIED,//text
		LAPTOP_SCREEN_UL_X + LAPTOP_SCREEN_WIDTH/2 + buttonSizeX,//x position: center of laptop + buttonsize
		LAPTOP_SCREEN_WEB_UL_Y + GetFontHeight(FONT14ARIAL) + ENC_DATA_IMAGE_HEIGHT + ENC_DATA_SHORT_DESC_HEIGHT + buttonSizeY + ENC_DATA_DESC_HEIGHT + 7*ENC_DATA_GAP,//y position: below all
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, BtnEncyclopedia_Data_NextPreviousDataBtnCallBack );
	CHECKF( giEncyclopedia_DataBtn[ 4 ] != BUTTON_NO_SLOT );
	MSYS_SetBtnUserData( giEncyclopedia_DataBtn[ 4 ], 1 , TRUE );//next -> next = TRUE

	//////
	// set cursors and make button down state slightly shifted
	for( UINT8 i = 0; i < 5; i++ )
	{
		SetButtonCursor( giEncyclopedia_DataBtn[ i ], CURSOR_LAPTOP_SCREEN );
		GetButtonPtr( giEncyclopedia_DataBtn[ i ] )->fShiftImage = TRUE;
	}

	//////
	// fill data, also creates video object for image and deletes old one.
	CreateData(sNewItem);

	return (TRUE);
}

/**
* Destroys video objects, buttons, fasthelp texts, mousregions.
*
* Called on exit of encyclopedia data web page in laptop. Cleans up data page of Encyclopedia.
* @return FALSE on error. TRUE on success.
* @see EnterEncyclopediaData_NEW()
*/
BOOLEAN ExitEncyclopediaData_NEW(  )
{
	extern UINT32	guiEncyclopediaBG;//background image from Encyclopedia main page
	BOOLEAN success = TRUE;
	// error if not enabled
	CHECKF( gGameExternalOptions.gEncyclopedia );
	// error if the laptop mode does not match encyclopedia
	CHECKF( LAPTOP_MODE_ENCYCLOPEDIA_DATA == guiPreviousLaptopMode );

	// Delete background
	success &= DeleteVideoObjectFromIndex( guiEncyclopediaBG );
	guiEncyclopediaBG = 0;

	// Reset filters
	guiEncyclopediaDataSubFilter = 0;
	geEncyclopediaDataFilterLocation = ENC_DATA_FILTER_L_ALL;
	geEncyclopediaDataFilterCharacter = ENC_DATA_FILTER_C_ALL;
	geEncyclopediaDataFilterItem = ENC_DATA_FILTER_I_ALL;
	geEncyclopediaDataFilterQuest = ENC_DATA_FILTER_Q_ALL;
	
	// Reset data and remove data image
	success &= CreateData( -1 );

	// Destroy filter buttons
	for( UINT8 i = 0; i < (UINT8)(sizeof(giEncyclopedia_DataFilterBtn)/sizeof(INT32)); i++ )
	{
		if( giEncyclopedia_DataFilterBtn[ i ] != BUTTON_NO_SLOT )
		{
			RemoveButton( giEncyclopedia_DataFilterBtn[ i ] );
			giEncyclopedia_DataFilterBtn[ i ] = BUTTON_NO_SLOT;
		}
	}

	// Delete filter button image
	if ( giEncyclopedia_DataFilterBtnImage != BUTTON_NO_IMAGE )
	{
		UnloadButtonImage( giEncyclopedia_DataFilterBtnImage );
		giEncyclopedia_DataFilterBtnImage = BUTTON_NO_IMAGE;
	}

	// Delete navigation buttons
	for( UINT8 i = 0; i < (UINT8)(sizeof(giEncyclopedia_DataBtn)/sizeof(INT32)); i++ )
	{
		if ( giEncyclopedia_DataBtn[ i ] != BUTTON_NO_SLOT )
		{
			RemoveButton( giEncyclopedia_DataBtn[ i ] );
			giEncyclopedia_DataBtn[ i ] = BUTTON_NO_SLOT;
		}
	}

	// Delete navigation button image
	if ( giEncyclopedia_DataBtnImage != BUTTON_NO_IMAGE )
	{
		UnloadButtonImage( giEncyclopedia_DataBtnImage );
		giEncyclopedia_DataBtnImage = BUTTON_NO_IMAGE;
	}

	return success;
}

/**
* @brief Renders data page of encyclopedia.
*/
void RenderEncyclopediaData_NEW(  )
{
	UINT16 x, y, shadowWidth;
	extern UINT32	guiEncyclopediaBG;//background image from Encyclopedia main page

	CHECKV( gGameExternalOptions.gEncyclopedia );

	// render background
	x = LAPTOP_SCREEN_UL_X;
	y = LAPTOP_SCREEN_WEB_UL_Y;
	CHECKV( BltVideoObjectFromIndex( FRAME_BUFFER, guiEncyclopediaBG, 0, x, y, VO_BLT_SRCTRANSPARENCY, NULL ) );


	y += ENC_DATA_GAP;
	x += 2*ENC_DATA_GAP + GetWidthOfButtonPic( giEncyclopedia_DataFilterBtnImage, 0 );
	shadowWidth = LAPTOP_SCREEN_WIDTH - 4*ENC_DATA_GAP - 2*GetWidthOfButtonPic( giEncyclopedia_DataFilterBtnImage, 0 );

	// render name y-centered
	if( gstEncyclopediaDataEntry.sName[ 0 ] != '\0' )
	{
		CHECKV( DrawTextToScreen( gstEncyclopediaDataEntry.sName, x, y, shadowWidth, FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED ) );
	}

	y += 2*ENC_DATA_GAP + GetFontHeight( FONT14ARIAL );

	// Shadow image area
	CHECKV( ShadowVideoSurfaceRect( FRAME_BUFFER, x, y, x + shadowWidth, y + ENC_DATA_IMAGE_HEIGHT ) );

	// Render image centered into shadowed surface
	if( gstEncyclopediaDataEntry.uiImageID )
	{//TODO: scaling only works on 16Bit surface, BltStretchVideoSurface needs to support the other bit depths as well.
		FLOAT scaleRatio;
		HVOBJECT hVObject;
		SGPRect sourceRect, destRect;
		CHECKV( GetVideoObject( &hVObject, gstEncyclopediaDataEntry.uiImageID ) );
		CHECKV( hVObject ); CHECKV( hVObject->pETRLEObject);
		// 'min( xDest/xSrc, yDest/ySrc )'
		scaleRatio = hVObject->pETRLEObject->usWidth? (FLOAT)(shadowWidth - 2*ENC_DATA_GAP)/hVObject->pETRLEObject->usWidth: 0.0f;
		scaleRatio = min( scaleRatio, hVObject->pETRLEObject->usHeight? (FLOAT)(ENC_DATA_IMAGE_HEIGHT - 2*ENC_DATA_GAP)/hVObject->pETRLEObject->usHeight: 0.0f );
		// blt original image into extraBuffer, as there might be negative offsets it gets rendered 100 pix shifted
		ColorFillVideoSurfaceArea( guiEXTRABUFFER, 0, 0, 100 + shadowWidth, 100 + ENC_DATA_IMAGE_HEIGHT, 0 );
		BltVideoObjectFromIndex( guiEXTRABUFFER, gstEncyclopediaDataEntry.uiImageID, 0, 100, 100, VO_BLT_SRCTRANSPARENCY, NULL );
		// now blt extra buffer scalled into FRAME_BUFFER
		sourceRect.iLeft = 100 + hVObject->pETRLEObject->sOffsetX;
		sourceRect.iTop = 100 + hVObject->pETRLEObject->sOffsetY;
		sourceRect.iRight = 100 + hVObject->pETRLEObject->usWidth + hVObject->pETRLEObject->sOffsetX;
		sourceRect.iBottom = 100 + hVObject->pETRLEObject->usHeight + hVObject->pETRLEObject->sOffsetY;
		destRect.iLeft = x + ENC_DATA_GAP + shadowWidth/2 - (UINT16)(scaleRatio * hVObject->pETRLEObject->usWidth/2);
		destRect.iTop = y + ENC_DATA_GAP + ENC_DATA_IMAGE_HEIGHT/2 - (UINT16)(scaleRatio * hVObject->pETRLEObject->usHeight/2);
		destRect.iRight = x - ENC_DATA_GAP + shadowWidth/2 + (UINT16)(scaleRatio * hVObject->pETRLEObject->usWidth/2);
		destRect.iBottom = y - ENC_DATA_GAP + ENC_DATA_IMAGE_HEIGHT/2 + (UINT16)(scaleRatio * hVObject->pETRLEObject->usHeight/2);
		BltStretchVideoSurface( FRAME_BUFFER, guiEXTRABUFFER, 0, 0, VO_BLT_SRCTRANSPARENCY, &sourceRect, &destRect );
	}
	y += ENC_DATA_IMAGE_HEIGHT + ENC_DATA_GAP;

	// Shadow short desc. area
	CHECKV( ShadowVideoSurfaceRect( FRAME_BUFFER, x, y, x + shadowWidth, y + ENC_DATA_SHORT_DESC_HEIGHT ) );
	y += ENC_DATA_GAP;

	// Render short description
	if( gstEncyclopediaDataEntry.sShortDesc[ 0 ] != '\0' )
	{
		CHECKV( DrawTextToScreen( gstEncyclopediaDataEntry.sShortDesc, x + ENC_DATA_GAP, y, shadowWidth, FONT10ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED ) );
	}
	y += ENC_DATA_SHORT_DESC_HEIGHT + GetButtonHeight( giEncyclopedia_DataBtn[ 0 ] ) + ENC_DATA_GAP;

	// Shadow descritpion area
	CHECKV( ShadowVideoSurfaceRect( FRAME_BUFFER, x, y, x + shadowWidth, y + ENC_DATA_DESC_HEIGHT ) );

	// Render detailed description using word wrap
	if ( gstEncyclopediaDataEntry.sDescr[ 0 ] != '\0' )
	{
		CHECKV( DisplayWrappedString( x + ENC_DATA_GAP, y + ENC_DATA_GAP, shadowWidth - 2*ENC_DATA_GAP, ENC_DATA_GAP/2, FONT10ARIAL, FONT_MCOLOR_WHITE, gstEncyclopediaDataEntry.sDescr, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED ) );
	}

	// finish render
	MarkButtonsDirty();
	RenderWWWProgramTitleBar();
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
	//fReDrawScreenFlag = TRUE;
	//fPausedReDrawScreenFlag = TRUE;
	//////
	// Render buttons
	//RenderButtons();
}

/** @} */
