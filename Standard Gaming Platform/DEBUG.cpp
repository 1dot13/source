
// JA2

//**************************************************************************
//
// Filename :	debug.c
//
//	Purpose :	debug manager implementation
//
// Modification history :
//
//		xxxxx96:LH				- Creation
//		xxnov96:HJH				- made it work
//
//**************************************************************************

// Because we're in a library, define SGP_DEBUG here - the client may not always
// use the code to write text, because the header switches on the define
//#define SGP_DEBUG
//#undef _DEBUG

// WDS:  Note:  To compile with VS2005 in release mode you might have to not define SGP_DEBUG and
// undefine _DEBUG or you get linker errors.


	#include "types.h"
	#include <windows.h>
	#include <ddeml.h>
	#include <stdio.h>
	#include <string>
	#include <sstream>
	#include "debug.h"
	#include "WCheck.h"
	#include "TopicIDs.h"
	#include "TopicOps.h"
	#include "WizShare.h"

	//Kris addition
	#ifdef JA2
		#include "screenids.h"
		#include "Sys Globals.h"
		#include "jascreens.h"
		#include "gameloop.h"
		#include "input.h"
	#endif

	// CJC added
	#ifndef _NO_DEBUG_TXT
		#include "fileman.h"
	#endif
#include	"GameSettings.h"
#include "SaveLoadGame.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOLEAN gfRecordToFile     = FALSE;
BOOLEAN gfRecordToDebugger = TRUE;

// moved from header file: 24mar98:HJH
UINT32	guiProfileStart, guiExecutions, guiProfileTime;
INT32		giProfileCount;

// Had to move these outside the ifdef SGP_DEBUG below, because
// they are required for the String() function, which is NOT a 
// debug-mode only function, it's used in release-mode as well! -- DB
 
CHAR8 gubAssertString[512];

#define MAX_MSG_LENGTH2 512
CHAR8		gbTmpDebugString[8][MAX_MSG_LENGTH2];
UINT8		gubStringIndex = 0;

#ifdef __cplusplus
}
#endif

#ifdef SGP_DEBUG

#ifdef __cplusplus
extern "C" {
#endif

//**************************************************************************
//
//				Defines
//
//**************************************************************************

#define BUFSIZE        100
#define TIMER_TIMEOUT  1000

//**************************************************************************
//
//				Variables
//
//**************************************************************************


UINT32	guiTimerID = 0;

bool           gfDebugTopics[MAX_TOPICS_ALLOTED];

// remove debug .txt file
void RemoveDebugText( void );

STRING512 debugLogFileName;
FILE * debugLogFile = 0;

#ifdef __cplusplus
}
#endif

//**************************************************************************
//
//				Functions
//
//**************************************************************************


//**************************************************************************
//
// DbgGetLogFileName
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		xxjun98:CJC		->creation
//
//**************************************************************************
bool DbgGetLogFileName( STRING512 pcName )
{
	// use the provided buffer to get the directory name, then tack on
	// "\debug.txt"
#ifndef _NO_DEBUG_TXT
	if ( ! GetExecutableDirectory( pcName ) )
	{
		return false;
	}

	if ( strlen( pcName ) > (512 - strlen( "\\debug.txt" ) - 1 ) )
	{
		// no room!
		return false;
	}

	strcat( pcName, "\\debug.txt" );
#endif

	return true;
}


//**************************************************************************
//
// DbgInitialize
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		xxnov96:HJH		->creation
//
//**************************************************************************

bool DbgInitialize()
{
	DbgClearAllTopics();

	gfRecordToFile = TRUE;
	gfRecordToDebugger = TRUE;
	gubAssertString[0] = '\0';

#ifndef _NO_DEBUG_TXT
	if (! DbgGetLogFileName( debugLogFileName ) ) {
		return false;
	}
	// clear debug text file out
	RemoveDebugText( );
#endif

	return true;
}

//**************************************************************************
//
// DbgShutdown
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		xxnov96:HJH		->creation
//
//**************************************************************************

void DbgShutdown()
{
	DbgMessageReal( -1, CLIENT_SHUTDOWN, 0, "SGP Going Down" );
}


//**************************************************************************
//
// DbgTopicRegistration
//
//
// Parameter List :
// Return Value :
// Modification history :
//
//		June 97: BR		->creation
//
//**************************************************************************

void DbgTopicRegistration( unsigned ubCmd, const unsigned usTopicID, const char *zMessage )
{
	if ( usTopicID == NULL )
		return;

	AssertGE(ubCmd, TOPIC_REGISTER);
	AssertLE(ubCmd, TOPIC_UNREGISTER);

	if( ubCmd == TOPIC_REGISTER )
	{
		gfDebugTopics[ usTopicID ] = true;
		DbgMessageReal(usTopicID, TOPIC_MESSAGE, DBG_LEVEL_0, zMessage );
	}
	else if( ubCmd == TOPIC_UNREGISTER )
	{
		if ( usTopicID >= MAX_TOPICS_ALLOTED )
			return;

		DbgMessageReal( usTopicID, TOPIC_MESSAGE, DBG_LEVEL_0, zMessage );
		gfDebugTopics[ usTopicID ] = false;
	}
}


// *************************************************************************
// Clear the debug txt file out to prevent it from getting huge
//
//
// *************************************************************************

void RemoveDebugText()
{
	DeleteFile( debugLogFileName );
	debugLogFile = 0;
}


//**************************************************************************
//
// DbgClearAllTopics
//
//
// Parameter List :
// Return Value :
// Modification history :
//
//		June 97: BR		->creation
//
//**************************************************************************

void DbgClearAllTopics( void )
{
	for( unsigned usIndex = 0; usIndex < MAX_TOPICS_ALLOTED; usIndex++)
	{
		gfDebugTopics[ usIndex ] = false;
	}
}

//**************************************************************************
//
// DbgMessageReal
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		xxnov96:HJH		->creation
//
//**************************************************************************
void DbgMessageReal(unsigned uiTopicId, unsigned uiCommand, unsigned uiDebugLevel, const char* strMessage)
{
#ifndef _NO_DEBUG_TXT
  static FILE      *OutFile = 0;
#endif
	// Check for a registered topic ID
	if ( (uiTopicId < MAX_TOPICS_ALLOTED) && (gfDebugTopics[uiTopicId]) )
	{
		OutputDebugString ( strMessage );
		OutputDebugString ( "\n" );

//add _NO_DEBUG_TXT to your SGP preprocessor definitions to avoid this f**king huge file from 
//slowly growing behind the scenes!!!!
#ifndef _NO_DEBUG_TXT
		if (!debugLogFile)
			debugLogFile = fopen(debugLogFileName, "a+t");
	    if (debugLogFile) { 
			fprintf(debugLogFile, "%s\n", strMessage);
			fflush( debugLogFile );
		}
#endif
	}
}

//**************************************************************************
//
// DbgSetDebugLevel
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		11nov96:HJH		->creation
//
//**************************************************************************

bool DbgSetDebugLevel(unsigned uiTopicId, unsigned uiDebugLevel)
{
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

void			_DebugRecordToFile(BOOLEAN gfState)
{
	gfRecordToFile = gfState;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void			_DebugRecordToDebugger(BOOLEAN gfState)
{
	gfRecordToDebugger = gfState;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wiz8 compatible debug messaging

void			_DebugMessage(const char *pString, unsigned uiLineNum, const char *pSourceFile)
{
	CHAR8 ubOutputString[512];

	//
	// Build the output string
	//

	sprintf( ubOutputString, "{ %ld } %s [Line %d in %s]\n", GetTickCount(), pString, uiLineNum, pSourceFile );

	//
	// Output to debugger
	//

	if (gfRecordToDebugger)
	{
		OutputDebugString( (LPCSTR) ubOutputString );
	}

	//
	// Record to file if required
	//

#ifndef _NO_DEBUG_TXT
	if (gfRecordToFile)
	{
		if (!debugLogFile)
			debugLogFile = fopen( debugLogFileName, "a+t" );
		if (debugLogFile) {
			fputs( ubOutputString, debugLogFile );
			fflush( debugLogFile );
		}
	}
#endif
}
#else
bool	DbgInitialize() { return true; };
void	DbgShutdown() {};
#endif


#ifdef FORCE_ASSERTS_ON


//////////////////////////////////////////////////////////////////////

extern HVOBJECT FontObjs[25];


void _FailMessage(const char* message, unsigned lineNum, const char * functionName, const char* sourceFileName)
{
	// This function shouldn't recurse
	static bool alreadyInThisFunction = false;
	if (alreadyInThisFunction)
		return;
	alreadyInThisFunction = true;

	std::stringstream basicInformation;
	basicInformation << "Assertion Failure [Line " << lineNum;
	if (functionName) {
		basicInformation << " in function " << functionName;
	}
	basicInformation << " in file " << sourceFileName << "]";

	std::stringstream outputString;
	outputString << "{ " << GetTickCount() << " } " << basicInformation;

	//Build the output strings
	if( message )
		sprintf( gubAssertString, message );	
	else
		sprintf( gubAssertString, "" );

	//Output to debugger
	if (gfRecordToDebugger)
		OutputDebugString( outputString.str().c_str() );
	
	DbgMessage( TOPIC_GAME, DBG_LEVEL_1, outputString.str().c_str());

	//This will actually bring up a screen that prints out the assert message
	//until the user hits esc or alt-x.
	// WDS - Automatically try to save when an assertion failure occurs
	if (gGameExternalOptions.autoSaveOnAssertionFailure &&
		!alreadySaving) {
		sprintf( gubErrorText, "%s. Attempting to do a debug save as SaveGame%d.sav (this may fail)", basicInformation.str().c_str(), SAVE__ASSERTION_FAILURE );
	} else {
		sprintf( gubErrorText, "%s", basicInformation.str().c_str());
	}
	SetPendingNewScreen( ERROR_SCREEN );
	SetCurrentScreen( ERROR_SCREEN );

	// WDS - Automatically try to save when an assertion failure occurs
	if (gGameExternalOptions.autoSaveOnAssertionFailure &&
		!alreadySaving) {
		SaveGame( SAVE__ASSERTION_FAILURE, L"Assertion Failure Auto Save" );
	}

    MSG Message;
	while (gfProgramIsRunning)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE))
		{ // We have a message on the WIN95 queue, let's get it
			if (!GetMessage(&Message, NULL, 0, 0))
			{ // It's quitting time
				continue;
			}
			// Ok, now that we have the message, let's handle it
			TranslateMessage(&Message);
			DispatchMessage(&Message);      
		}
		else
		{ // Windows hasn't processed any messages, therefore we handle the rest
			GameLoop();        
			gfSGPInputReceived  =  FALSE;			
		}
	}

	alreadyInThisFunction = false;
	exit(0);
}

#endif

// This is NOT a _DEBUG only function! It is also needed in
// release mode builds. -- DB
STR8 String(const STR8 String, ...)
{

  va_list  ArgPtr;
  UINT8    usIndex;

  // Record string index. This index is used since we live in a multitasking environment. 
  // It is still not bulletproof, but it's better than a single string
  usIndex = gubStringIndex++;
  if (gubStringIndex == 8)
  { // reset string pointer
    gubStringIndex = 0;
  }

  va_start(ArgPtr, String);
  vsprintf( gbTmpDebugString[usIndex], String, ArgPtr);
  va_end(ArgPtr);

  return gbTmpDebugString[usIndex];

}
