#define _WIN32_WINNT 0x0500 

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include "Console.h"
#include "Lua Interpreter.h"

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

extern HWND ghWindow;

// maximum mumber of lines the output console should have
static const WORD MAX_CONSOLE_LINES = 500;

BOOL WINAPI ControlHandlerRoutine(
  DWORD dwCtrlType
)
{
	switch (dwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
		// Tell the main window that the console wants out
		PostMessage( ghWindow, WM_USER, 0, 0 );
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		// Ignore all of these signals.  The game itself will handle the important stuff anyway.
		return TRUE;
	}

	return FALSE;
}


void CreateConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
//	HWND hConWindow;

	// allocate a console for this app
	AllocConsole();

	// Eliminate the game-killing control signals
	SetConsoleCtrlHandler( ControlHandlerRoutine, TRUE );

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), 
		&coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), 
		coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
	// point to console as well
	ios::sync_with_stdio();

	// Print the warning message
	printf( "Do not close this window!\n");
	printf( "> " );
}

void PollConsole ()
{
	static int buf_idx = 0;
	static char buf[16384];

	if (_kbhit() )
	{
		buf[ buf_idx++ ] = _getche();
		if (buf[ buf_idx-1] == 8) {
			_putch( ' ');
			buf_idx--;
			if (buf_idx > 0)
			{
				_putch( 8 );
				buf_idx--;
			}
		}
	}

	if (buf[ buf_idx-1] == '\r')
	{
		buf[ buf_idx-1] = 0;
		printf( "\n");
		EvalLua( buf);
		printf( "\n> ");
		buf_idx = 0;
	}
}

//End of File



