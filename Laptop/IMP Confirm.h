#ifndef __IMP_CONFIRM_H
#define __IMP_CONFIRM_H

#include "Soldier Profile.h"

void EnterIMPConfirm( void );
void RenderIMPConfirm( void );
void ExitIMPConfirm( void );
void HandleIMPConfirm( void );

#define MAX_NEW_IMP_PORTRAITS NUM_PROFILES

typedef struct
{
	UINT16 uiEyeXPositions;
	UINT16 uiEyeYPositions;
	UINT16 uiMouthXPositions;
	UINT16 uiMouthYPositions;
	UINT16 uiIndex;
	UINT16 FaceIndex; 
	UINT16 PortraitId;
	BOOLEAN bSex;
	INT32 iCurrentSkin;
	INT32 iCurrentHair;
	INT32 iCurrentShirt;
	INT32 iCurrentPants;
	BOOLEAN bBigBody;
	BOOLEAN bBadAss;
} IMP_FACE_VALUES;

typedef struct
{
	BOOLEAN Enabled;
	
	UINT16 uiEyeXPositions;
	UINT16 uiEyeYPositions;
	UINT16 uiMouthXPositions;
	UINT16 uiMouthYPositions;
	UINT16 uiIndex;
	UINT16 FaceIndex; 
	UINT16 PortraitId;
	BOOLEAN bSex;
	INT32 iCurrentSkin;
	INT32 iCurrentHair;
	INT32 iCurrentShirt;
	INT32 iCurrentPants;
	BOOLEAN bBigBody;
	BOOLEAN bBadAss;
	
} IMP_VALUES;

extern IMP_VALUES gIMPValues[NUM_PROFILES];

// Flugente: voicesets are no longer tied to a slot
typedef struct
{
	CHAR16	szVoiceSetName[20];
	UINT16 voiceset;
	BOOLEAN bSex;
	BOOLEAN exists;		// set upon reading the xml: this voice exists
} IMP_VOICESET;

extern std::vector<IMP_VOICESET> gIMPVoice;

BOOLEAN AddCharacterToPlayersTeam( void );
BOOLEAN LoadImpCharacter( STR fileName );
void WriteOutCurrentImpCharacter( INT32 iProfileId );
void WriteOutCurrentImpCharacter( INT32 iProfileId, STR fileName );

void ResetIMPCharactersEyesAndMouthOffsets( UINT8 ubMercProfileID );

BOOLEAN ImpExists ( STR fileName );

INT32 iGetProfileCost( void ); // added by SANDRO

void MakeProfileInvItemAnySlot( MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubStatus, UINT8 ubHowMany );

#endif

