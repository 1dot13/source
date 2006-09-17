#ifndef __IMP_CONFIRM_H
#define __IMP_CONFIRM_H

void EnterIMPConfirm( void );
void RenderIMPConfirm( void );
void ExitIMPConfirm( void );
void HandleIMPConfirm( void );

BOOLEAN AddCharacterToPlayersTeam( void );
void LoadImpCharacter( STR fileName );
void WriteOutCurrentImpCharacter( INT32 iProfileId );
void WriteOutCurrentImpCharacter( INT32 iProfileId, STR fileName );

void ResetIMPCharactersEyesAndMouthOffsets( UINT8 ubMercProfileID );

BOOLEAN ImpExists ( STR fileName );

// WANNE: Determines the free and current slot of the imp in the gMercProfiles array
INT32 GetNextFreeIMPSlot( void );
INT32 GetCurrentIMPSlot( void );

#endif

