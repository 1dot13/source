#ifndef ITEMS_H
#define ITEMS_H
#include "Soldier Control.h"
#include "Overhead Types.h"
#include "Weapons.h"

#include <map>		// added by Flugente

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

// Snap: Moved here from opplist.cpp
// These defined are no longer used to calculate item bonuses,
// but for lack of anything better, they are still used in other places
#define NIGHTSIGHTGOGGLES_BONUS 2 * STRAIGHT_RATIO
#define UVGOGGLES_BONUS 4 * STRAIGHT_RATIO

enum
{
	EXPLOSV_NORMAL,
	EXPLOSV_STUN,
	EXPLOSV_TEARGAS,
	EXPLOSV_MUSTGAS,
	EXPLOSV_FLARE,
	EXPLOSV_NOISE,
	EXPLOSV_SMOKE,
	EXPLOSV_CREATUREGAS,
	EXPLOSV_BURNABLEGAS,
	EXPLOSV_FLASHBANG,
	EXPLOSV_SIGNAL_SMOKE,
	EXPLOSV_SMOKE_DEBRIS,
	EXPLOSV_SMOKE_FIRERETARDANT,
	EXPLOSV_ANY_TYPE,
};

extern UINT8 SlotToPocket[7];
extern BOOLEAN WeaponInHand( SOLDIERTYPE * pSoldier );

INT8 FindAmmo( SOLDIERTYPE * pSoldier, UINT8 ubCalibre, UINT16 ubMagSize, UINT8 ubAmmoType, INT8 bExcludeSlot );
INT8 FindBestWeaponIfCurrentIsOutOfRange(SOLDIERTYPE * pSoldier, INT8 bCurrentWeaponIndex, UINT16 bWantedRange);

INT16 FindAttachmentSlot(OBJECTTYPE* pObj, UINT16 usItem, UINT8 subObject = 0);
OBJECTTYPE* FindAttachment(OBJECTTYPE * pObj, UINT16 usItem, UINT8 subObject = 0);
extern INT8 FindObjClass(SOLDIERTYPE * pSoldier, UINT32 usItemClass);
extern INT8 FindAIUsableObjClass(SOLDIERTYPE * pSoldier, UINT32 usItemClass, BOOLEAN fSidearm = FALSE);
extern INT8 FindAIUsableObjClassWithin(SOLDIERTYPE * pSoldier, UINT32 usItemClass, INT8 bLower, INT8 bUpper);
extern INT8 FindEmptySlotWithin(SOLDIERTYPE * pSoldier, INT8 bLower, INT8 bUpper);
extern INT8 FindObjInObjRange(SOLDIERTYPE * pSoldier, UINT16 usItem1, UINT16 usItem2);
extern INT8 FindLaunchable(SOLDIERTYPE * pSoldier, UINT16 usWeapon);
extern INT8 FindGLGrenade(SOLDIERTYPE * pSoldier);
extern INT8 FindThrowableGrenade(SOLDIERTYPE * pSoldier, UINT8 ubGrenadeType = EXPLOSV_ANY_TYPE, UINT8 ubMinDamaga = 0);
extern UINT8 CountThrowableGrenades(SOLDIERTYPE * pSoldier, UINT8 ubGrenadeType = EXPLOSV_ANY_TYPE, UINT8 ubMinDamaga = 0);
extern INT8 FindUsableObj(SOLDIERTYPE * pSoldier, UINT16 usItem);

void DeleteObj(OBJECTTYPE * pObj );
void SwapObjs( OBJECTTYPE * pObj1, OBJECTTYPE * pObj2 );
void SwapObjs(SOLDIERTYPE* pSoldier, int leftSlot, int rightSlot, BOOLEAN fPermanent);
void SwapObjs(SOLDIERTYPE* pSoldier, int slot, OBJECTTYPE* pObject, BOOLEAN fPermanent);
void CleanUpItemStats( OBJECTTYPE * pObj );

//Returns true if swapped, false if added to end of stack
extern BOOLEAN PlaceObjectAtObjectIndex( OBJECTTYPE * pSourceObj, OBJECTTYPE * pTargetObj, UINT8 ubIndex, UINT32 ubCap );

UINT16 CalculateAmmoWeight( UINT16 usGunAmmoItem, UINT16 ubShotsLeft );
UINT16	CalculateObjectWeight( OBJECTTYPE *pObject );
UINT32 GetTotalWeight( SOLDIERTYPE* pSoldier );
UINT32 CalculateCarriedWeight( SOLDIERTYPE * pSoldier, BOOLEAN fConsiderDragging = FALSE );
// CHRISL:
UINT16 CalculateItemSize( OBJECTTYPE *pObject );


extern UINT16 TotalPoints( OBJECTTYPE * pObj );
extern UINT16 UseKitPoints( OBJECTTYPE * pObj, UINT16 usPoints, SOLDIERTYPE *pSoldier );

extern BOOLEAN EmptyWeaponMagazine( OBJECTTYPE * pWeapon, OBJECTTYPE *pAmmo, UINT32 subObject = 0 );
BOOLEAN CreateItem( UINT16 usItem, INT16 bStatus, OBJECTTYPE * pObj );
BOOLEAN CreateAmmo( UINT16 usItem, OBJECTTYPE * pObj, UINT16 ubShotsLeft = 0);
BOOLEAN CreateItems( UINT16 usItem, INT16 bStatus, UINT8 ubNumber, OBJECTTYPE * pObj );
BOOLEAN CreateMoney( UINT32 uiMoney, OBJECTTYPE * pObj );
extern UINT16 DefaultMagazine( UINT16 usItem );
UINT16 RandomMagazine( UINT16 usItem, UINT8 ubPercentStandard, UINT8 maxCoolness, INT8 bSoldierClass );
UINT16 RandomMagazine( OBJECTTYPE * pGun, UINT8 ubPercentStandard, UINT8 maxCoolness, INT8 bSoldierClass );
extern BOOLEAN ReloadGun( SOLDIERTYPE * pSoldier, OBJECTTYPE * pGun, OBJECTTYPE * pAmmo, UINT32 subObject = 0 );

bool IsBackpackSlot(INT8 bSlot);
// CHRISL: Add new inventory version of ItemSlotLimit function
UINT8 ItemSlotLimit( OBJECTTYPE * pObject, INT16 bSlot, SOLDIERTYPE *pSoldier = NULL, BOOLEAN cntAttach = TRUE);

// Function to put an item in a soldier profile
// It's very primitive, just finds an empty place!
BOOLEAN PlaceObjectInSoldierProfile( UINT8 ubProfile, OBJECTTYPE *pObject );
BOOLEAN RemoveObjectFromSoldierProfile( UINT8 ubProfile, UINT16 usItem );
INT8 FindObjectInSoldierProfile( UINT8 ubProfile, UINT16 usItem );

BOOLEAN ObjectExistsInSoldierProfile( UINT8 ubProfile, UINT16 usItem );
void SetMoneyInSoldierProfile( UINT8 ubProfile, UINT32 uiMoney );

void CheckEquipmentForDamage( SOLDIERTYPE *pSoldier, INT32 iDamage );
BOOLEAN ArmBomb( OBJECTTYPE * pObj, INT8 bSetting );

// NOTE TO ANDREW: 
// 
// The following functions expect that pObj points to the object
// "in the cursor", which should have memory allocated for it already
BOOLEAN PlaceObject( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj );

BOOLEAN AutoPlaceObjectAnywhere(SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem, INT8 bExcludeSlot = NO_SLOT);
BOOLEAN AutoPlaceObjectToWorld(SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, INT8 bVisible = TRUE);

// Send fNewItem to true to set off new item glow in inv panel
BOOLEAN AutoPlaceObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem, INT8 bExcludeSlot = NO_SLOT, BOOLEAN fStackOrSingleSlot = FALSE );

//ADB
bool FitsInSmallPocket(OBJECTTYPE* pObj);

bool PlaceInAnySlot(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot = NO_SLOT, bool fStackOrSingleSlot = FALSE);
bool PlaceInAnyPocket(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot = NO_SLOT);
bool PlaceInAnySmallPocket(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot = NO_SLOT);
bool PlaceInAnyBigOrMediumPocket(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int bExcludeSlot = NO_SLOT);
bool TryToStackInSlot(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, int bSlot);
bool TryToPlaceInSlot(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, bool fNewItem, int& startSlot, int endSlot);


void RemoveInvObject( SOLDIERTYPE *pSoldier, UINT16 usItem );
void InitItemAttachments(OBJECTTYPE* pObj);
std::vector<UINT16> GetItemSlots(OBJECTTYPE* pObj, UINT8 subObject = 0, BOOLEAN fAttachment = FALSE);
void RemoveProhibitedAttachments(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, UINT16 usItem, BOOLEAN fOnlyRemoveWhenSlotsChange = 1);
// HEADROCK HAM 5: Added argument for statusindex.
attachmentList ReInitMergedItem(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, UINT16 usOldItem, UINT8 ubStatusIndex); //Madd: return a list of attachments we couldn't reattach or drop, so the calling function can decide what to do with them
void EjectAmmoAndPlace(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, UINT8 subObject = 0);

BOOLEAN CanItemFitInVehicle( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement );
BOOLEAN CanItemFitInRobot( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement );
BOOLEAN CanItemFitInPosition( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement );

void DistributeStatus(OBJECTTYPE* pSourceObject, OBJECTTYPE* pTargetObject, INT16 bMaxPoints);

UINT8 SwapKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj );

// create a keyobject
BOOLEAN CreateKeyObject( OBJECTTYPE * pObj , UINT8 ubNumberOfKeys, UINT8 ubKeyIdValue );
BOOLEAN DeleteKeyObject( OBJECTTYPE * pObj );
BOOLEAN AllocateObject( OBJECTTYPE **pObj );

// removes a key from a *KEYRING* slot
BOOLEAN RemoveKeyFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj );

// take several
BOOLEAN RemoveKeysFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, UINT8 ubNumberOfKeys ,OBJECTTYPE * pObj );

// add the keys to an inventory slot
UINT8 AddKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj );



//Kris:	December 9, 1997
//I need a bunch of validation functions for ammo, attachments, etc., so I'll be adding them here.
//Chris, maybe you might find these useful, or add your own.	I don't really know what I'm doing yet,
//so feel free to correct me...

//Simple check to see if the item has any attachments
BOOLEAN ItemHasAttachments( OBJECTTYPE *pItem, SOLDIERTYPE * pSoldier = NULL, UINT8 iter = 0 );

//Determine if this item can receive this attachment.	This is different, in that it may
//be possible to have this attachment on this item, but may already have an attachment on
//it which doesn't work simultaneously with the new attachment (like a silencer and duckbill).
BOOLEAN ValidItemAttachment( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment, BOOLEAN fDisplayMessage = TRUE, UINT8 subObject = NULL, std::vector<UINT16> usAttachmentSlotIndexVector = std::vector<UINT16>());
//Return true if usAttachment would fit on an item with usItem, without considering possible other attachments on this gun. This may be inaccurate for NAS, because slots can change.
BOOLEAN ValidAttachment( UINT16 usAttachment, UINT16 usItem, UINT8* ubAPCost = NULL );
//This function does the same as the above, but is more accurate.
BOOLEAN ValidAttachment( UINT16 usAttachment, OBJECTTYPE * pObj, UINT8* ubAPCost = NULL, UINT8 subObject = 0, std::vector<UINT16> usAttachmentSlotIndexVector = std::vector<UINT16>());
BOOLEAN ValidItemAttachmentSlot( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment, BOOLEAN fDisplayMessage = TRUE, UINT8 subObject = 0, INT16 slotCount = -1, BOOLEAN fIgnoreAttachmentInSlot = FALSE, OBJECTTYPE ** ppAttachInSlot = NULL, std::vector<UINT16> usAttachmentSlotIndexVector = std::vector<UINT16>());

// Determines if it is possible to merge an item with any item whose class 
// is the same as the indicated item
BOOLEAN ValidAttachmentClass( UINT16 usAttachment, UINT16 usItem );

//Determines if it is possible to equip this weapon with this ammo.
BOOLEAN ValidAmmoType( UINT16 usItem, UINT16 usAmmoType );

BOOLEAN ItemIsDamageable(UINT16 usItem);
BOOLEAN ItemIsRepairable(UINT16 usItem);
BOOLEAN ItemIsDamagedByWater(UINT16 usItem);
BOOLEAN ItemIsMetal(UINT16 usItem);
BOOLEAN ItemSinks(UINT16 usItem);
BOOLEAN ItemIsTwoHanded(UINT16 usItem);
BOOLEAN ItemIsHiddenAddon(UINT16 usItem);
BOOLEAN ItemIsNotBuyable(UINT16 usItem);
BOOLEAN ItemIsAttachment(UINT16 usItem);
BOOLEAN ItemIsHiddenAttachment(UINT16 usItem);
BOOLEAN ItemIsOnlyInTonsOfGuns(UINT16 usItem);
BOOLEAN ItemIsNotInEditor(UINT16 usItem);
BOOLEAN ItemIsUndroppableByDefault(UINT16 usItem);
BOOLEAN ItemIsUnaerodynamic(UINT16 usItem);
BOOLEAN ItemIsElectronic(UINT16 usItem);
BOOLEAN ItemIsCannon(UINT16 usItem);
BOOLEAN ItemIsRocketRifle(UINT16 usItem);
BOOLEAN ItemHasFingerPrintID(UINT16 usItem);
BOOLEAN ItemIsMetalDetector(UINT16 usItem);
BOOLEAN ItemIsGasmask(UINT16 usItem);
BOOLEAN ItemIsLockBomb(UINT16 usItem);
BOOLEAN ItemIsFlare(UINT16 usItem);
BOOLEAN ItemIsGrenadeLauncher(UINT16 usItem);
BOOLEAN ItemIsMortar(UINT16 usItem);
BOOLEAN ItemIsDuckbill(UINT16 usItem);
BOOLEAN ItemIsDetonator(UINT16 usItem);
BOOLEAN ItemIsRemoteDetonator(UINT16 usItem);
BOOLEAN ItemHasHiddenMuzzleFlash(UINT16 usItem);
BOOLEAN ItemIsRocketLauncher(UINT16 usItem);
BOOLEAN ItemIsSingleShotRocketLauncher(UINT16 usItem);
BOOLEAN ItemIsBrassKnuckles(UINT16 usItem);
BOOLEAN ItemIsCrowbar(UINT16 usItem);
BOOLEAN ItemIsGLgrenade(UINT16 usItem);
BOOLEAN ItemIsFlakJacket(UINT16 usItem);
BOOLEAN ItemIsLeatherJacket(UINT16 usItem);
BOOLEAN ItemIsBatteries(UINT16 usItem);
BOOLEAN ItemNeedsBatteries(UINT16 usItem);
BOOLEAN ItemHasXRay(UINT16 usItem);
BOOLEAN ItemIsWirecutters(UINT16 usItem);
BOOLEAN ItemIsToolkit(UINT16 usItem);
BOOLEAN ItemIsFirstAidKit(UINT16 usItem);
BOOLEAN ItemIsMedicalKit(UINT16 usItem);
BOOLEAN ItemIsCanteen(UINT16 usItem);
BOOLEAN ItemIsJar(UINT16 usItem);
BOOLEAN ItemIsCanAndString(UINT16 usItem);
BOOLEAN ItemIsMarbles(UINT16 usItem);
BOOLEAN ItemIsWalkman(UINT16 usItem);
BOOLEAN ItemIsRemoteTrigger(UINT16 usItem);
BOOLEAN ItemIsRobotRemote(UINT16 usItem);
BOOLEAN ItemIsCamoKit(UINT16 usItem);
BOOLEAN ItemIsLocksmithKit(UINT16 usItem);
BOOLEAN ItemIsMine(UINT16 usItem);
BOOLEAN ItemIsATMine(UINT16 usItem);
BOOLEAN ItemIsHardware(UINT16 usItem);
BOOLEAN ItemIsMedical(UINT16 usItem);
BOOLEAN ItemIsGascan(UINT16 usItem);
BOOLEAN ItemContainsLiquid(UINT16 usItem);
BOOLEAN ItemIsRock(UINT16 usItem);
BOOLEAN ItemIsThermalOptics(UINT16 usItem);
BOOLEAN ItemIsOnlyInScifi(UINT16 usItem);
BOOLEAN ItemIsOnlyInNIV(UINT16 usItem);
BOOLEAN ItemIsBarrel(UINT16 usItem);
BOOLEAN ItemHasTripwireActivation(UINT16 usItem);
BOOLEAN ItemIsTripwire(UINT16 usItem);
BOOLEAN ItemIsDirectional(UINT16 usItem);
BOOLEAN ItemBlocksIronsight(UINT16 usItem);
BOOLEAN ItemAllowsClimbing(UINT16 usItem);
BOOLEAN ItemIsCigarette(UINT16 usItem);
BOOLEAN ItemIsOnlyInDisease(UINT16 usItem);
BOOLEAN ItemProvidesRobotCamo(UINT16 usItem);
BOOLEAN ItemProvidesRobotNightvision(UINT16 usItem);
BOOLEAN ItemProvidesRobotLaserBonus(UINT16 usItem);

//Existing functions without header def's, added them here, just incase I'll need to call
//them from the editor.

UINT8 AttachmentAPCost( UINT16 usAttachment, UINT16 usItem, SOLDIERTYPE * pSoldier ); // SANDRO - added argument
UINT8 AttachmentAPCost( UINT16 usAttachment, OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier, UINT8 subObject = 0, std::vector<UINT16> usAttachmentSlotIndexVector = std::vector<UINT16>() );


BOOLEAN ValidLaunchable( UINT16 usLaunchable, UINT16 usItem );
UINT16 GetLauncherFromLaunchable( UINT16 usLaunchable );

BOOLEAN ValidMerge( UINT16 usMerge, UINT16 usItem );
BOOLEAN EvaluateValidMerge( UINT16 usMerge, UINT16 usItem, UINT16 * pusResult, UINT16 * pusResult2, UINT8 * pubType, UINT8 * pubAPCost );


// is the item passed a medical/ first aid kit? and what type?
INT8 IsMedicalKitItem( OBJECTTYPE *pObject );

BOOLEAN AutoReload( SOLDIERTYPE * pSoldier, bool aReloadEvenIfNotEmpty = true );
INT8 FindAmmoToReload( SOLDIERTYPE * pSoldier, INT8 bWeaponIn, INT8 bExcludeSlot );

void SwapOutHandItem( SOLDIERTYPE * pSoldier );
void SwapHandItems( SOLDIERTYPE * pSoldier );

OBJECTTYPE* FindAttachmentByClass( OBJECTTYPE * pObj, UINT32 uiItemClass, UINT8 subObject = 0 );
OBJECTTYPE* FindAttachmentByAttachmentClass( OBJECTTYPE * pObj, UINT32 uiAttachmentClass, UINT8 subObject = 0 );
void WaterDamage( SOLDIERTYPE *pSoldier );

INT8 FindObj( SOLDIERTYPE * pSoldier, UINT16 usItem, INT8 bLower = 0, INT8 bUpper = NUM_INV_SLOTS );

BOOLEAN ApplyCamo( SOLDIERTYPE * pSoldier, UINT16 usItem, UINT16& usrPointsToUse );
BOOLEAN ApplyCanteen( SOLDIERTYPE * pSoldier, UINT16 usItem, UINT16 usPointsToUse );
BOOLEAN ApplyElixir( SOLDIERTYPE * pSoldier, UINT16 usItem, UINT16& usrPointsToUse );
BOOLEAN ApplyClothes( SOLDIERTYPE * pSoldier, UINT16 usItem, UINT16 usPointsToUse );	// Flugente: apply clothes, and eventually disguise

UINT32 ConvertProfileMoneyValueToObjectTypeMoneyValue( UINT8 ubStatus );
UINT8 ConvertObjectTypeMoneyValueToProfileMoneyValue( UINT32 uiMoneyAmount );

BOOLEAN CheckForChainReaction( UINT16 usItem, INT16 bStatus, INT16 bDamage, BOOLEAN fOnGround );

BOOLEAN ItemIsLegal( UINT16 usItemIndex, BOOLEAN fIgnoreCoolness = FALSE );
UINT16 StandardGunListReplacement( UINT16 usGun );
UINT16 FindReplacementMagazine( UINT8 ubCalibre, UINT16 ubMagSize, UINT8 ubAmmoType);
UINT16 FindReplacementMagazineIfNecessary( UINT16 usOldGun, UINT16 usOldAmmo, UINT16 usNewGun );

BOOLEAN DamageItemOnGround( OBJECTTYPE * pObject, INT32 sGridNo, INT8 bLevel, INT32 iDamage, SoldierID ubOwner );

BOOLEAN CompatibleFaceItem( UINT16 usItem1, UINT16 usItem2 );

UINT32 MoneySlotLimit( INT8 bSlot );

void CheckEquipmentForFragileItemDamage( SOLDIERTYPE *pSoldier, INT32 iDamage );

// Range of Xray device
#define XRAY_RANGE 40
// Seconds that Xray lasts
#define XRAY_TIME 5

extern void ActivateXRayDevice( SOLDIERTYPE * pSoldier );
extern void TurnOnXRayEffects( SOLDIERTYPE * pSoldier );
extern void TurnOffXRayEffects( SOLDIERTYPE * pSoldier );
OBJECTTYPE* FindLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon );
UINT16 FindLegalGrenade(UINT16 usItem);

void DamageObj( OBJECTTYPE * pObj, INT8 bAmount, UINT8 subObject = 0 );

UINT16 MagazineClassIndexToItemType(UINT16 usMagIndex);

// Item property-related stuff added by Madd Mugsy

BOOLEAN IsScoped( OBJECTTYPE * pObj );
BOOLEAN NCTHIsScoped( OBJECTTYPE * pObj );
INT16 GetReliability( OBJECTTYPE * pObj );
INT16 GetAimBonus( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, INT32 iRange, INT16 ubAimTime );
//WarmSteel - Used to determine the base aim bonus of the most powerful scope.
INT16 GetBaseScopeAimBonus( OBJECTTYPE * pObj, INT32 iRange );
// HEADROCK: Function to get the natural aimbonus of the weapon and its attachments
INT16 GetFlatAimBonus( OBJECTTYPE * pObj );
// HEADROCK: Function to get the final loudness value of a weapon, after reduction from its own characteristics, ammo and attachments
INT16 GetFinalLoudness( OBJECTTYPE * pObj );
//INT16 GetMinAimBonusRange( OBJECTTYPE * pObj );
INT16 GetToHitBonus( OBJECTTYPE * pObj, INT32 iRange, UINT8 bLightLevel, BOOLEAN fProneStance = FALSE );
// HEADROCK: Added alternate function that only returns the natural to-hit-bonii of a weapon, ammo and attachments
INT16 GetFlatToHitBonus( OBJECTTYPE * pObj );
// HEADROCK: Function to get average of best laser ranges from weapon and attachments
INT16 GetAverageBestLaserRange( OBJECTTYPE * pObj );
// get the best laser range from the weapon and attachments
INT16 GetBestLaserRange( OBJECTTYPE * pObj );
// HEADROCK: Function to get bipod bonus from weapon and its attachments
INT16 GetBipodBonus( OBJECTTYPE * pObj );
INT16 GetBurstToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance = FALSE );
INT16 GetAutoToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance = FALSE	);
INT16 GetGearAimBonus( SOLDIERTYPE * pSoldier, INT32 iRange, INT16 sAimTime);
INT16 GetGearToHitBonus( SOLDIERTYPE * pSoldier );
INT16 GetMinRangeForAimBonus( SOLDIERTYPE* pSoldier, OBJECTTYPE * pObj );

// HEADROCK: Function to get the final loudness value of a weapon, after reduction from its own characteristics, ammo and attachments
INT16 GetFinalLoudness( OBJECTTYPE * pObj );
UINT32 FindRangeBonusAttachment( OBJECTTYPE * pObj );
INT16 GetRangeBonus( OBJECTTYPE * pObj );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HEADROCK HAM 4: The following functions return the value of new NCTH-related modifiers from an item and all its
// attachments. They are stance-based, meaning that the soldier's stance determines which modifier is referenced.
// For a "default" value, feed the function a value of ubStance=ANIM_STAND.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Flugente: unified function (no need to have 12 functions that all do the same thing and clutter the code)
enum {
	ITEMMODIFIER_FLATBASE = 0,
	ITEMMODIFIER_PERCENTBASE,
	ITEMMODIFIER_FLATAIM,
	ITEMMODIFIER_PERCENTAIM,
	ITEMMODIFIER_PERCENTCAP,
	ITEMMODIFIER_PERCENTHANDLING,
	ITEMMODIFIER_DROPCOMPENSATION,
	ITEMMODIFIER_COUNTERFORCEMAX,
	ITEMMODIFIER_COUNTERFORCEACCURACY,
	ITEMMODIFIER_TRACKING,
	ITEMMODIFIER_AIMLEVELS,

	// not NCTH, but works the same way...
	ITEMMODIFIER_SPOTTER,

	ITEMMODIFIER_MAX,
};

INT32 GetItemModifier(OBJECTTYPE* pObj, UINT8 ubRef, UINT8 usType);
INT32 GetObjectModifier( SOLDIERTYPE* pSoldier, OBJECTTYPE *pObj, UINT8 ubStance, UINT8 type );

INT32 GetAimLevelsTraitModifier( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj );

// Returns the character's stance as 0/1/2.
UINT8 GetStanceModifierRef( INT8 ubStance );

INT16 GetDamageBonus( OBJECTTYPE * pObj );
INT16 GetMeleeDamageBonus( OBJECTTYPE * pObj );

INT16 GetPercentAPReduction( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj );
INT16 GetPercentAutofireAPReduction( OBJECTTYPE * pObj );
INT16 GetPercentBurstFireAPReduction( OBJECTTYPE * pObj );
INT16 GetPercentReadyTimeAPReduction( OBJECTTYPE * pObj );
INT16 GetPercentReloadTimeAPReduction( OBJECTTYPE * pObj );
INT16 GetRateOfFireBonus( OBJECTTYPE * pObj );
INT16 GetBurstSizeBonus( OBJECTTYPE * pObj );
BOOLEAN HasFastBurst( OBJECTTYPE * pObj );
INT16 GetMagSizeBonus( OBJECTTYPE * pObj, UINT8 subObject = 0 );

// HEADROCK HAM 4: This function now calculates and returns the weapon's recoil as X/Y offsets.
void GetRecoil( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, FLOAT *bRecoilX, FLOAT *bRecoilY, UINT8 ubNumBullet );
void GetBaseFlatRecoilModifier( OBJECTTYPE *pObj, FLOAT *bRecoilModifierX, FLOAT *bRecoilModifierY);
void GetAttachmentFlatRecoilModifier( OBJECTTYPE *pObj, FLOAT *bRecoilModifierX, FLOAT *bRecoilModifierY);
void GetFlatRecoilModifier( OBJECTTYPE *pObj, FLOAT *bRecoilModifierX, FLOAT *bRecoilModifierY );
INT16 GetBasePercentRecoilModifier( OBJECTTYPE *pObj );
INT16 GetAttachmentPercentRecoilModifier( OBJECTTYPE *pObj);
INT16 GetPercentRecoilModifier( OBJECTTYPE *pObj );
// HEADROCK HAM 4: This function returns whether the last bullet in a burst/autofire volley was a tracer.
BOOLEAN WasPrevBulletATracer( SOLDIERTYPE *pSoldier, OBJECTTYPE *pWeapon );

// Snap: GetTotalVisionRangeBonus checks for light levels
// and returns cumulative bonus from the following functions:
//
// GetVisionRangeBonus applies to all light levels
// GetNightVisionRangeBonus: bLightLevel > NORMAL_LIGHTLEVEL_DAY, above ground only
// GetCaveVisionRangeBonus: bLightLevel > NORMAL_LIGHTLEVEL_DAY, underground only
// GetDayVisionRangeBonus: NORMAL_LIGHTLEVEL_DAY <= bLightLevel < NORMAL_LIGHTLEVEL_NIGHT
// GetBrightLightVisionRangeBonus: bLightLevel < NORMAL_LIGHTLEVEL_DAY
// (remember: darker is higher!)
INT16 GetTotalVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel );
// HEADROCK: Following function used to get the vision range bonus of an item and its attachments. Second argument controls specific
// vision type.
INT16 GetItemVisionRangeBonus( OBJECTTYPE * pObj, INT16 VisionType );
// HEADROCK: This function calculates percent tunnel vision from an object and its attachments.
UINT8 GetItemPercentTunnelVision( OBJECTTYPE * pObj );

// Snap: Scale night vision bonus with light level
INT16 NightBonusScale( INT16 bonus, UINT8 bLightLevel );

UINT8 GetPercentTunnelVision( SOLDIERTYPE * pSoldier );
BOOLEAN HasThermalOptics( SOLDIERTYPE * pSoldier );

INT8 FindHearingAid( SOLDIERTYPE * pSoldier );
INT16 GetHearingRangeBonus( SOLDIERTYPE * pSoldier );
// HEADROCK: This is the same function as above, but calculates hearing range bonus without SOLDIERTYPE, from an item and its attachments
INT16 GetItemHearingRangeBonus( OBJECTTYPE * pObj );

INT8 IsGrenadeLauncher( OBJECTTYPE * pObj );
INT16 GetGrenadeLauncherStatus( OBJECTTYPE * pObj );
BOOLEAN IsGrenadeLauncherAttached( OBJECTTYPE * pObj, UINT8 subObject = 0 );
OBJECTTYPE* FindAttachment_GrenadeLauncher( OBJECTTYPE * pObj );
UINT16 GetAttachedGrenadeLauncher( OBJECTTYPE * pObj );

// JMich & Flugente: functions for underbarrel weapons and bayonets
// As we can have both underbarrel gun and a bayonet, use usFlag to specify (IC_GUN or IC_BLADE are used here, or both)
BOOLEAN IsWeaponAttached( OBJECTTYPE * pObj, UINT32 usFlag, UINT8 subObject = 0 );
OBJECTTYPE* FindAttachedWeapon( OBJECTTYPE * pObj, UINT32 usFlag );
UINT16 GetAttachedWeapon( OBJECTTYPE * pObj, UINT32 usFlag );

INT8 FindRocketLauncherOrCannon( SOLDIERTYPE * pSoldier );
INT8 FindNonSmokeLaunchable( SOLDIERTYPE * pSoldier, UINT16 usWeapon );
OBJECTTYPE* FindNonSmokeLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon );
UINT16 PickARandomLaunchable(UINT16 itemIndex);
bool BadGoggles(SOLDIERTYPE *pTeamSoldier);

BOOLEAN IsFlashSuppressor( OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier );
// HEADROCK: Declaration of new function to detect flash suppression without SOLDIERTYPE argument
BOOLEAN IsFlashSuppressorAlt( OBJECTTYPE * pObj );
BOOLEAN IsRemoteDetonator( OBJECTTYPE * pObj );
BOOLEAN IsDetonator( OBJECTTYPE * pObj );
BOOLEAN IsDuckbill( OBJECTTYPE * pObj );

// Snap: Returns CUMULATIVE noise volume modifier, with component status factored in.
// Noise volume is then calculated as volume * GetPercentNoiseVolume / 100
UINT16 GetPercentNoiseVolume( OBJECTTYPE * pObj );

INT16 GetBulletSpeedBonus( OBJECTTYPE * pObj );
INT8 FindGasMask( SOLDIERTYPE * pSoldier );
INT8 FindLockBomb( SOLDIERTYPE * pSoldier );
INT8 FindMetalDetectorInHand( SOLDIERTYPE * pSoldier );		// Flugente: changed, now only works if detector is in hands
INT8 FindCannon( SOLDIERTYPE * pSoldier );
INT8 FindUsableCrowbar( SOLDIERTYPE * pSoldier );
INT8 FindToolkit( SOLDIERTYPE * pSoldier );
BOOLEAN IsDetonatorAttached( OBJECTTYPE * pObj );			// Flugente: no more needed, use HasAttachmentOfClass( pObj, AC_DETONATOR ) instead
BOOLEAN IsRemoteDetonatorAttached( OBJECTTYPE * pObj );		// Flugente: no more needed, use HasAttachmentOfClass( pObj, AC_REMOTEDET ) instead
OBJECTTYPE* FindAttachedBatteries( OBJECTTYPE * pObj );
INT8 FindMedKit( SOLDIERTYPE * pSoldier );
INT8 FindFirstAidKit( SOLDIERTYPE * pSoldier );
INT8 FindDisarmKit( SOLDIERTYPE * pSoldier );	//JMich_SkillsModifiers: Added function to check for disarm bonus
INT8 FindLocksmithKit( SOLDIERTYPE * pSoldier );
INT8 FindCamoKit( SOLDIERTYPE * pSoldier );
INT8 FindWalkman( SOLDIERTYPE * pSoldier );
INT8 FindTrigger( SOLDIERTYPE * pSoldier );
INT8 FindRemoteControl( SOLDIERTYPE * pSoldier );
INT16 GetWornCamo( SOLDIERTYPE * pSoldier );
INT16 GetCamoBonus( OBJECTTYPE * pObj );
INT16 GetWornStealth( SOLDIERTYPE * pSoldier );
INT16 GetStealthBonus( OBJECTTYPE * pObj );
// HEADROCK: Non-status-dependent version of the above
INT16 GetBasicStealthBonus( OBJECTTYPE * pObj );

void ApplyEquipmentBonuses(SOLDIERTYPE * pSoldier);

INT16 GetGearAimBonus( SOLDIERTYPE * pSoldier, INT32 iRange, INT16 ubAimTime);
INT16 GetGearToHitBonus( SOLDIERTYPE * pSoldier );
INT16 GetGearAPBonus( SOLDIERTYPE * pSoldier );
// HEADROCK: This function gets total AP bonus for an item and its attachments
INT16 GetAPBonus( OBJECTTYPE * pObj );

UINT16 GetFirstExplosiveOfType(UINT16 expType);
UINT16 GetFirstHandGrenadeOfType(UINT16 expType);
UINT16 GetHandGrenadeOfType(UINT16 usDefaultItem, UINT16 usType);

BOOLEAN ItemIsHandGrenade(UINT16 usItem);

OBJECTTYPE* FindSunGogglesInInv( SOLDIERTYPE * pSoldier, INT8 * bSlot, BOOLEAN * isAttach, BOOLEAN searchAllInventory = FALSE );
OBJECTTYPE* FindNightGogglesInInv( SOLDIERTYPE * pSoldier, INT8 * bSlot, BOOLEAN * isAttach, BOOLEAN searchAllInventory = FALSE  );

UINT8 AllowedAimingLevels(SOLDIERTYPE * pSoldier, INT32 sGridNo);
// HEADROCK HAM 4: New functions to determine Scope and Laser factors on a weapon.
FLOAT GetHighestScopeMagnificationFactor( OBJECTTYPE *pObj );
FLOAT GetScopeMagnificationFactor( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, FLOAT uiRange );
FLOAT GetBestScopeMagnificationFactor( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, FLOAT uiRange );
FLOAT GetProjectionFactor( OBJECTTYPE *pObj );
// Flugente: projection factor while using scope modes excludes those factors coming from not-used scopes and sights
FLOAT GetScopeModeProjectionFactor( SOLDIERTYPE *pSoldier, OBJECTTYPE * pObj );

FLOAT GetScopeRangeMultiplier( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, FLOAT d2DDistance );
INT32 GetGunAccuracy( OBJECTTYPE *pObj );
INT32 GetAccuracyModifier( OBJECTTYPE *pObj );
// HEADROCK HAM 4: Added range factor
UINT8 AllowedAimingLevelsNCTH( SOLDIERTYPE *pSoldier, INT32 sGridNo);
UINT8 GetAllowedAimingLevelsForItem( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, UINT8 ubStance );

INT16 GetWornUrbanCamo( SOLDIERTYPE * pSoldier );
INT16 GetUrbanCamoBonus( OBJECTTYPE * pObj );
INT16 GetWornDesertCamo( SOLDIERTYPE * pSoldier );
INT16 GetDesertCamoBonus( OBJECTTYPE * pObj );
INT16 GetWornSnowCamo( SOLDIERTYPE * pSoldier );
INT16 GetSnowCamoBonus( OBJECTTYPE * pObj );

// HEADROCK HAM 3.6: Looks for a backpack anywhere on this character.
INT8 FindBackpackOnSoldier( SOLDIERTYPE * pSoldier );

// HEADROCK HAM 3.6: Handler functions to get modified damage with INI parameter.
UINT8 GetModifiedExplosiveDamage( UINT16 ubDamage, UINT8 ubType );
UINT8 GetModifiedMeleeDamage( UINT16 ubDamage );
UINT8 GetModifiedGunDamage( UINT16 ubDamage );

UINT16 GetModifiedGunRange( UINT16 usWeaponIndex );


// SANDRO - added some procedures
INT16 ReduceCamoFromSoldier( SOLDIERTYPE * pSoldier, INT16 iCamoToRemove, INT16 iCamoToSkip );
BOOLEAN HasExtendedEarOn( SOLDIERTYPE * pSoldier );
BOOLEAN UseTotalMedicalKitPoints( SOLDIERTYPE * pSoldier, UINT16 usPointsToConsume );

// Flugente
FLOAT GetItemCooldownModificator( OBJECTTYPE * pObj );
FLOAT GetItemCooldownFactor( OBJECTTYPE * pObj );
void  GetScopeLists( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, std::map<INT8, OBJECTTYPE*>& arScopeMap );

// check if item belongs to a specific attachment class
BOOLEAN IsAttachmentClass( UINT16 usItem, UINT32 aFlag );
BOOLEAN HasAttachmentOfClass( OBJECTTYPE * pObj, UINT32 aFlag );

//Madd: Common Attachment Framework
bool IsAttachmentPointAvailable( OBJECTTYPE * pObject, UINT8 subObject, UINT32 attachmentID );
bool IsAttachmentPointAvailable( UINT32 itemID, UINT32 attachmentID );
bool IsAttachmentPointAvailable( UINT64 point, UINT32 attachmentID, BOOLEAN onlyCheckAttachments );
UINT64 GetAvailableAttachmentPoint ( OBJECTTYPE * pObject, UINT8 subObject );

// Flugente: check if and how a bomb has been set up
void CheckBombSpecifics( OBJECTTYPE * pObj, INT8* detonatortype, INT8* setting, INT8* defusefrequency );

// Flugente: check for specific flags
BOOLEAN HasItemFlag(UINT16 usItem, UINT64 aFlag);
BOOLEAN HasItemFlag2(UINT16 usItem, UINT64 aFlag);

// Flugente: get first item number that has this flag. Use with caution, as we search in all items
BOOLEAN GetFirstItemWithFlag( UINT16* pusItem, UINT64 aFlag );

// Flugente: check if this object is currently fed from an external source (belts in inventory, other mercs)
BOOLEAN ObjectIsBeingFedExternal(SOLDIERTYPE* pSoldier, OBJECTTYPE * pObject);

// is this object currently used to feed an externally fed object? This can be in our or someone else's inventory
BOOLEAN ObjectIsExternalFeeder(SOLDIERTYPE* pSoldier, OBJECTTYPE * pObject);

OBJECTTYPE* GetExternalFeedingObject(SOLDIERTYPE* pSoldier, OBJECTTYPE * pObject);

BOOLEAN DeductBulletViaExternalFeeding(SOLDIERTYPE* pSoldier, OBJECTTYPE * pObject);

#define SECTORFLAG_RAIDPOSSIBLE_BLOODCAT		0x00000001				// bloodcats can attack this sector
#define SECTORFLAG_RAIDPOSSIBLE_ZOMBIE			0x00000002				// zombies can attack this sector
#define SECTORFLAG_RAIDPOSSIBLE_BANDIT			0x00000004				// bandits can attack this sector

// Flugente: additional xml data for sectors
typedef struct
{
	UINT8		usWaterType;			// type of water source in this sector
	UINT16		usNaturalDirt;			// extra dirt percentage when firing in this sector
	UINT8		usCurfewValue;			// determines wether mercs disguising as civilian are automatically discovered (certain sectors are 'restricted' to civilians)
	INT8		sRadioScanModifier;		// modifies the scan range of the radio scan assignment in this sector

	std::vector<UINT16>	prisonroomvector;	// room numbers of prisons

	UINT16		usCivilianPopulation;	// total amount of civilians that are supposed to be in this sector. This does not have to be the number of civilians in tactical

	UINT8		rainchance;				// chance that it rains in this sector 
	UINT8		sandstormchance;		// chance that a sandstorm happens in this sector 
	UINT8		snowchance;				// chance that it snows in this sector

	UINT8		snakechance;			// if positive, snakes can attack in water, the higher the number, the more often it happens
	UINT8		numsnakes;				// how many snakes can be active at maximum

	UINT8		maxworkers;				// maximum number of workers in this sector
	UINT16		wealth;					// rating of how rich the population in this sector is

	UINT32		usSectorFlagMask;		// various flags

} SECTOR_EXT_DATA;

// get dirt increase for object with attachments, fConsiderAmmo: with ammo
FLOAT GetItemDirtIncreaseFactor( OBJECTTYPE * pObj, BOOLEAN fConsiderAmmo = TRUE );

//DBrot: get the volume of all attached pouches
UINT8 GetVolumeAlreadyTaken(OBJECTTYPE * pObj, INT16 exceptSlot);
INT16 GetPocketFromAttachment(OBJECTTYPE * pObj, UINT8 pMap, int subObject = 0);
UINT8 GetFirstPocketOnItem(UINT16 usIndex);

INT8 GetNumberAltFireAimLevels( SOLDIERTYPE * pSoldier, INT32 iGridNo );

// Flugente: retrieve a specific clothes item, if such a thing exists
BOOLEAN	GetFirstClothesItemWithSpecificData( UINT16* pusItem, PaletteRepID aPalVest, PaletteRepID aPalPants );

// Flugente: function to determine what item a random item spawns
BOOLEAN GetItemFromRandomItem( UINT16 usRandomItem, UINT16* pusNewItem );

// Flugente: can item be applied to other people?
BOOLEAN ItemCanBeAppliedToOthers( UINT16 usItem );

//zwwooooo - IoV: change RangeBonus to ratable (Orange by kenkenkenken in IoV921)
INT32 GetPercentRangeBonus( OBJECTTYPE * pObj );

// silversurfer: returns the SleepModifier from the characters inventory
UINT8 GetInventorySleepModifier( SOLDIERTYPE *pSoldier );

void AttachDefaultAttachments(OBJECTTYPE *pObj, BOOLEAN fAllDefaultAttachments=TRUE);//dnl ch75 261013
BOOLEAN FindAttachmentRange(UINT16 usAttachment, UINT32* pStartIndex, UINT32* pEndIndex);

// Flugente: is this object useable by militia?
BOOLEAN ObjectIsMilitiaRelevant( OBJECTTYPE *pObj );

FLOAT GetAttackAPTraitMultiplier( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, UINT8 ubMode);

// sevenfm: check if this type of grenade can use delayed mode
BOOLEAN CanDelayGrenadeExplosion( UINT16 usItem );
UINT16 GetLaunchableOfExplosionType(UINT16 launcher, UINT8 explosionType);

#endif
