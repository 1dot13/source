#ifndef __MINIGAME_H
#define __MINIGAME_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

// Flugente: lets create a few minigames
enum
{
	MINIGAME_FIRST = 0,

	TETRIS = MINIGAME_FIRST,
	PONG,
	PICTURE,

	MINIGAME_MAX,
};

// enums of gamestates
enum
{
	MINIGAME_STARTSCREEN = 0,
	MINIGAME_GAME,
	MINIGAME_GAMEOVER,
	MINIGAME_PAUSE,
};

void SetNextGame( UINT32 ausGame, UINT32 ausGameState = MINIGAME_STARTSCREEN );

UINT32	MiniGameScreenInit( void );
UINT32	MiniGameScreenHandle( void );
UINT32	MiniGameScreenShutdown( void );

void MiniGame_Init_Tetris();
UINT32 MiniGame_Handle_Tetris();

void MiniGame_Init_Pong( );
UINT32 MiniGame_Handle_Pong( );

void SetInteractivePicture( std::string aStr, bool aVal );
void MiniGame_Init_Picture();
UINT32 MiniGame_Handle_Picture();

#endif //__MINIGAME_H