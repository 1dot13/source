#ifndef _GAME_VERSION_H_
#define _GAME_VERSION_H_


#ifdef __cplusplus
extern "C" {
#endif


//
//	Keeps track of the game version
//

extern	CHAR16		zVersionLabel[256];
extern	CHAR8		czVersionNumber[16];
extern	CHAR16		zTrackingNumber[16];


//ADB:	I needed these here so I moved them, and why put them in *.cpp anyways?
//
//		Keeps track of the saved game version.	Increment the saved game version whenever 
//	you will invalidate the saved game file
#define			SAVE_GAME_VERSION					103	//102	//101	//100 // 99


//these I don't so much need here
#define			CURRENT_SAVEGAME_DATATYPE_VERSION					103
#define			FIRST_SAVEGAME_DATATYPE_CHANGE					102
//#define			SECOND_SAVEGAME_DATATYPE_CHANGE				999, #define	CURRENT_SAVEGAME_DATATYPE_VERSION	999


//#define RUSSIANGOLD
#ifdef __cplusplus
}
#endif


#endif
