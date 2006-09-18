#ifndef __IMP_CONFIRM_H
#define __IMP_CONFIRM_H

void EnterIMPConfirm( void );
void RenderIMPConfirm( void );
void ExitIMPConfirm( void );
void HandleIMPConfirm( void );

BOOLEAN AddCharacterToPlayersTeam( void );
BOOLEAN LoadImpCharacter( STR fileName );
void WriteOutCurrentImpCharacter( INT32 iProfileId );
void WriteOutCurrentImpCharacter( INT32 iProfileId, STR fileName );

void ResetIMPCharactersEyesAndMouthOffsets( UINT8 ubMercProfileID );

BOOLEAN ImpExists ( STR fileName );

#endif

