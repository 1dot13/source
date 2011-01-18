#ifndef __IMP_CONFIRM_H
#define __IMP_CONFIRM_H

void EnterIMPConfirm( void );
void RenderIMPConfirm( void );
void ExitIMPConfirm( void );
void HandleIMPConfirm( void );

#define MAX_NEW_IMP_PORTRAITS 50

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

extern IMP_FACE_VALUES gIMPFaceValues[200];


typedef struct
{
	UINT16 uiIndexIMP;
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
	
} IMP_MALE_VALUES;

typedef struct
{
	UINT16 uiIndexIMP;
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
} IMP_FEMALE_VALUES;

extern IMP_FEMALE_VALUES gIMPFemaleValues[200];
extern IMP_MALE_VALUES gIMPMaleValues[200];

BOOLEAN AddCharacterToPlayersTeam( void );
BOOLEAN LoadImpCharacter( STR fileName );
void WriteOutCurrentImpCharacter( INT32 iProfileId );
void WriteOutCurrentImpCharacter( INT32 iProfileId, STR fileName );

void ResetIMPCharactersEyesAndMouthOffsets( UINT8 ubMercProfileID );

BOOLEAN ImpExists ( STR fileName );

INT32 iGetProfileCost( void ); // added by SANDRO

#endif

