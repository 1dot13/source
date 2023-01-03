#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "Types.h"
	#include <stdlib.h>
	#include <malloc.h>
	#include <stdio.h>
	#include "FileMan.h"
#endif



#ifdef JA2
#endif

//If we are to use exception handling
#ifdef ENABLE_EXCEPTION_HANDLING


const int NumCodeBytes = 16;	// Number of code bytes to record.
const int MaxStackDump = 2048;	// Maximum number of DWORDS in stack dumps.
const int StackColumns = 8;		// Number of columns in stack dump.

#define	ONEK			1024
#define	SIXTYFOURK		(64*ONEK)
#define	ONEM			(ONEK*ONEK)
#define	ONEG			(ONEK*ONEK*ONEK)

//ppp
void		ErrorLog(HWFILE LogFile, STR8	Format, ...);
STR			GetExceptionString( DWORD uiExceptionCode );
void		DisplayRegisters( HWFILE hFile, CONTEXT	*pContext );
BOOLEAN GetAndDisplayModuleAndSystemInfo( HWFILE hFile, CONTEXT *pContext );
BOOLEAN DisplayStack( HWFILE hFile, CONTEXT *pContext );
void		RecordModuleList(HWFILE hFile );
void		PrintTime(STR8 output, FILETIME TimeToPrint);
static	void ShowModuleInfo(HWFILE hFile, HINSTANCE ModuleHandle);

static LPTOP_LEVEL_EXCEPTION_FILTER g_pfnOrigFilt = NULL ;

LONG __stdcall ERCrashDumpExceptionFilter(EXCEPTION_POINTERS* pExPtrs);
LONG __stdcall ERCrashDumpExceptionFilterEx(const char* pAppName, const char* pPath, EXCEPTION_POINTERS* pExPtrs);
static void ERLogModules(HWFILE fdump);

#if defined(_IMAGEHLP_) && defined(_X86_)
#define MAX_SYMNAME_SIZE  1024
static CHAR symBuffer[sizeof(IMAGEHLP_SYMBOL)+MAX_SYMNAME_SIZE];
static PIMAGEHLP_SYMBOL g_sym = (PIMAGEHLP_SYMBOL) symBuffer;
static void ERLogStackWalk(HWFILE fdump, EXCEPTION_POINTERS* pExPtrs);
#endif //defined(_IMAGEHLP_) && defined(_X86_)



INT32 RecordExceptionInfo( EXCEPTION_POINTERS *pExceptInfo )
{
	EXCEPTION_RECORD Record;
	CONTEXT	Context;
	CHAR8		zFileName[512];
	CHAR8		zDate[512];
	CHAR8		zTime[512];
	HWFILE	hFile;
	CHAR8		zString[2048];
	SYSTEMTIME SysTime;
	CHAR8		zNewLine[] = "\r\n";


	//create local copies of the exception info
	memcpy( &Record, pExceptInfo->ExceptionRecord , sizeof( EXCEPTION_RECORD ) );
	memcpy( &Context, pExceptInfo->ContextRecord, sizeof( CONTEXT ) );


	//
	//	Open a file to output the current state of the game
	//

	// Get the current time
	GetLocalTime( &SysTime );

	//create a date string
	sprintf( zDate, "%02d_%02d_%d", SysTime.wDay, SysTime.wMonth, SysTime.wYear );

	//create a time string
	sprintf( zTime, "%02d_%02d", SysTime.wHour, SysTime.wMinute );

	//create the crash file
	sprintf( zFileName, "Crash Report_%s___%s.txt", zDate, zTime );

	// create the save game file
	hFile = FileOpen( zFileName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( !hFile )
	{
		FileClose( hFile );
		return( 0 );
	}

	//
	// Display the version number
	//
#ifdef JA2

	//Dispay Ja's version number
	ErrorLog( hFile, "%S %S: %s.	%S",zVersionLabel, zRevisionNumber, czVersionNumber, zTrackingNumber );

	//Insert a new line
	ErrorLog( hFile, zNewLine );

	//Insert a new line
	ErrorLog( hFile, zNewLine );
#endif



	//
	// Write out the current state of the system
	//

	GetAndDisplayModuleAndSystemInfo( hFile, &Context );


	//Insert a new line
	ErrorLog( hFile, zNewLine );

	//Display the address of where the exception occurred
	sprintf( zString, "Exception occurred at address: 0x%08x\r\n", Record.ExceptionAddress );
	ErrorLog( hFile, zString );


	//if the exception was an access violation, display the offending address
	if( Record.ExceptionCode == EXCEPTION_ACCESS_VIOLATION && Record.NumberParameters != 0 )
	{
		if( Record.ExceptionInformation[0] != 0 )
		{
			//Display the address of where the access violation occurred
			sprintf( zString, "\tWrite Access Violation at: 0x%08x\r\n", Record.ExceptionInformation[1] );
		}
		else
		{
			//Display the address of where the access violation occurred
			sprintf( zString, "\tWrite Access Violation at: 0x%08x\r\n", Record.ExceptionInformation[1] );
		}

		ErrorLog( hFile, zString );
	}


	//Insert a new line
	ErrorLog( hFile, zNewLine );

	//Display the exception that caused this
	sprintf( zString, "Exact Error Message \r\n \"%s\"\r\n", GetExceptionString( Record.ExceptionCode ) );
	ErrorLog( hFile, zString );

	//Insert a new line
	ErrorLog( hFile, zNewLine );

	//Dispay if the code 'could' continue
	if( Record.ExceptionFlags != 0 )
		sprintf( zString, "%s\r\n", "The game 'can NOT' continue" );
	else
		sprintf( zString, "%s\r\n", "The game 'CAN' continue" );
	ErrorLog( hFile, zString );


	//Insert a new line
	ErrorLog( hFile, zNewLine );
	ErrorLog( hFile, zNewLine );

	//
	// Display the current context information
	//
	DisplayRegisters( hFile, &Context );

	ErrorLog( hFile, zNewLine );
	ErrorLog( hFile, zNewLine );

	// Stack walk if symbles are available
	ERLogStackWalk( hFile, pExceptInfo );
	ErrorLog( hFile, zNewLine );
	ErrorLog( hFile, zNewLine );

	//display the stack ( call stack + local variables )
	DisplayStack( hFile, &Context);

	//Display some spaces
	ErrorLog( hFile, zNewLine );
	ErrorLog( hFile, zNewLine );

	//Display all modules currently loaded
	//RecordModuleList(hFile );

	ERLogModules(hFile); // records version

	//eee

	FileClose( hFile );

	return( EXCEPTION_EXECUTE_HANDLER );
}



void ErrorLog( HWFILE hFile, STR8	Format, ...)
{
	char buffer[2000];	// wvsprintf never prints more than one K.
	UINT32	uiNumBytesWritten=0;
	UINT32	uiStringWidth;
	va_list arglist;
	va_start( arglist, Format);
	wvsprintf(buffer, Format, arglist);
	va_end( arglist);

	//WriteFile(LogFile, buffer, lstrlen(buffer), &NumBytes, 0);


	uiStringWidth = lstrlen(buffer);

	//write out the string
	FileWrite( hFile, buffer, uiStringWidth, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiStringWidth )
	{
		FileClose( hFile );
		return;
	}

}

STR	GetExceptionString( DWORD uiExceptionCode )
{
	switch( uiExceptionCode )
	{
		case EXCEPTION_ACCESS_VIOLATION:
			return( "The thread tried to read from or write to a virtual address for which it does not have the appropriate access.");
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
 			return( "The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.");
		case EXCEPTION_BREAKPOINT:
 			return( "A breakpoint was encountered.");
		case EXCEPTION_DATATYPE_MISALIGNMENT:
 			return( "The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.");
		case EXCEPTION_FLT_DENORMAL_OPERAND:
 			return( "One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.");
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
 			return( "The thread tried to divide a floating-point value by a floating-point divisor of zero.");
		case EXCEPTION_FLT_INEXACT_RESULT:
 			return( "The result of a floating-point operation cannot be represented exactly as a decimal fraction.");
		case EXCEPTION_FLT_INVALID_OPERATION:
 			return( "This exception represents any floating-point exception not included in this list.");
		case EXCEPTION_FLT_OVERFLOW:
 			return( "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.");
		case EXCEPTION_FLT_STACK_CHECK:
 			return( "The stack overflowed or underflowed as the result of a floating-point operation.");
		case EXCEPTION_FLT_UNDERFLOW:
 			return( "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.");
		case EXCEPTION_ILLEGAL_INSTRUCTION:
 			return( "The thread tried to execute an invalid instruction.");
		case EXCEPTION_IN_PAGE_ERROR:
 			return( "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.");
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
 			return( "The thread tried to divide an integer value by an integer divisor of zero.");
		case EXCEPTION_INT_OVERFLOW:
 			return( "The result of an integer operation caused a carry out of the most significant bit of the result.");
		case EXCEPTION_INVALID_DISPOSITION:
 			return( "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.");
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
 			return( "The thread tried to continue execution after a noncontinuable exception occurred.");
		case EXCEPTION_PRIV_INSTRUCTION:
 			return( "The thread tried to execute an instruction whose operation is not allowed in the current machine mode."); 
		case EXCEPTION_SINGLE_STEP:
 			return( "A trace trap or other single-instruction mechanism signaled that one instruction has been executed.");
		case EXCEPTION_STACK_OVERFLOW:
 			return( "The thread used up its stack.");

		default:
			return("Exception not in case ");
			break;
	}
}


void DisplayRegisters( HWFILE hFile, CONTEXT	*pContext )
{
	ErrorLog( hFile, "Registers:\r\n");
	ErrorLog( hFile, "\tEAX=%08x CS=%04x EIP=%08x EFLGS=%08x\r\n",
				pContext->Eax, pContext->SegCs, pContext->Eip, pContext->EFlags);
	ErrorLog( hFile, "\tEBX=%08x SS=%04x ESP=%08x EBP=%08x\r\n",
				pContext->Ebx, pContext->SegSs, pContext->Esp, pContext->Ebp);
	ErrorLog( hFile, "\tECX=%08x DS=%04x ESI=%08x FS=%04x\r\n",
				pContext->Ecx, pContext->SegDs, pContext->Esi, pContext->SegFs);
	ErrorLog( hFile, "\tEDX=%08x ES=%04x EDI=%08x GS=%04x\r\n",
				pContext->Edx, pContext->SegEs, pContext->Edi, pContext->SegGs);
//	ErrorLog( hFile, "Bytes at CS:EIP:\r\n");
}

BOOLEAN GetAndDisplayModuleAndSystemInfo( HWFILE hFile, CONTEXT *pContext )
{
	char zFileName[2048];
	char zString[2048];
	SYSTEM_INFO	SystemInfo;
	MEMORYSTATUS	MemInfo;
//	MEMORY_BASIC_INFORMATION	MemBasicInfo;
	size_t PageSize;
	size_t pageNum = 0;
	SGP_FILETIME LastWriteTime;

	if( GetModuleFileName(0, zFileName, sizeof(zFileName) ) == 0)
	{
		return( FALSE );
	}

	MemInfo.dwLength = sizeof(MemInfo);
	GlobalMemoryStatus(&MemInfo);

	//Display the filename
	ErrorLog( hFile, "File:\r\n\t%s\r\n", zFileName);


	//Get the time the file was created
	if (GetFileManFileTime(hFile, 0, 0, &LastWriteTime))
	{
		PrintTime( zString, LastWriteTime);
	
		ErrorLog( hFile, "\tFile created on: %s\r\n", zString );
	}


	//Get cpu type and number
	GetSystemInfo(&SystemInfo);
	ErrorLog( hFile, "\t%d type %d processor.\r\n", SystemInfo.dwNumberOfProcessors, SystemInfo.dwProcessorType );

	//Get free ram
	ErrorLog( hFile, "\tTotal Physical Memory: %d Megs.\r\n", MemInfo.dwTotalPhys/(1024*1024) );


	PageSize = SystemInfo.dwPageSize;

	pageNum = 0;

/*
	//Get current instruction pointer
	if( VirtualQuery((void *)(pageNum * PageSize), &MemBasicInfo, sizeof(MemBasicInfo) ) )
	{
		if (MemBasicInfo.RegionSize > 0)
		{
			ErrorLog( hFile, "\r\n\r\nJagged is loaded into memory at: 0x%08d.\r\n", MemBasicInfo.AllocationBase );
		}
	}
*/

	ErrorLog( hFile, "Segment( CS:EIP ):\t%04x:%08x.\r\n", pContext->SegCs, pContext->Eip );

	return( TRUE );
}



//
// This code for this function is based ( stolen ) from Bruce Dawson's article in Game Developer Magazine Jan 99
//
BOOLEAN DisplayStack( HWFILE hFile, CONTEXT	*pContext	)
{
	int Count = 0;
	char	buffer[1000] = "";
	const int safetyzone = 50;
	STR8 	nearend = buffer + sizeof(buffer) - safetyzone;
	STR8 	output = buffer;

	// Time to print part or all of the stack to the error log. This allows
	// us to figure out the call stack, parameters, local variables, etc.
		ErrorLog( hFile, "Stack dump:\r\n" );

	__try
	{
		// Esp contains the bottom of the stack, or at least the bottom of
		// the currently used area.
		DWORD* pStack = (DWORD *)pContext->Esp;
		DWORD* pStackTop;
		__asm
		{
			// Load the top (highest address) of the stack from the
			// thread information block. It will be found there in
			// Win9x and Windows NT.
			mov	eax, fs:[4]
			mov pStackTop, eax
		}
		if (pStackTop > pStack + MaxStackDump)
			pStackTop = pStack + MaxStackDump;


		// Too many calls to WriteFile can take a long time, causing
		// confusing delays when programs crash. Therefore I implemented
		// simple buffering for the stack dumping code instead of calling
		// hprintf directly.

		while (pStack + 1 <= pStackTop)
		{
			STR8 Suffix = " ";

			if ((Count % StackColumns) == 0)
				output += wsprintf(output, "%08x: ", pStack);

			if ((++Count % StackColumns) == 0 || pStack + 2 > pStackTop)
				Suffix = "\r\n";
			output += wsprintf(output, "%08x%s", *pStack, Suffix);
			pStack++;
	
			// Check for when the buffer is almost full, and flush it to disk.
			if (output > nearend)
			{
				ErrorLog( hFile, "%s", buffer);
				buffer[0] = 0;
				output = buffer;
			}
		}
		// Print out any final characters from the cache.
		ErrorLog( hFile, "%s", buffer);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		ErrorLog( hFile, "Exception encountered during stack dump.\r\n");
	}

	return( TRUE );
}


//
// This code for this function is ( stolen :) from Bruce Dawson's article in Game Developer Magazine Jan 99
//

// Print the specified FILETIME to output in a human readable format,
// without using the C run time.

void PrintTime(STR8 output, FILETIME TimeToPrint)
{
	WORD Date, Time;
	if (FileTimeToLocalFileTime(&TimeToPrint, &TimeToPrint) &&
				FileTimeToDosDateTime(&TimeToPrint, &Date, &Time))
	{
		// What a silly way to print out the file date/time. Oh well,
		// it works, and I'm not aware of a cleaner way to do it.
		wsprintf(output, "%02d/%02d/%d %02d:%02d:%02d",
					(Date / 32) & 15, Date & 31, (Date / 512) + 1980,
					(Time / 2048), (Time / 32) & 63, (Time & 31) * 2);
	}
	else
		output[0] = 0;
}







//
// This code for this function is ( stolen :) from Bruce Dawson's article in Game Developer Magazine Jan 99
//

// Scan memory looking for code modules (DLLs or EXEs). VirtualQuery is used
// to find all the blocks of address space that were reserved or committed,
// and ShowModuleInfo will display module information if they are code
// modules.


void RecordModuleList(HWFILE hFile )
{
	ErrorLog( hFile, "\r\n"
					"Module list: names, addresses, sizes, time stamps "
			"and file times:\r\n");
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	const size_t PageSize = SystemInfo.dwPageSize;
	// Set NumPages to the number of pages in the 4GByte address space,
	// while being careful to avoid overflowing ints.
	const size_t NumPages = 4 * size_t(ONEG / PageSize);
	size_t pageNum = 0;
	void *LastAllocationBase = 0;
	while (pageNum < NumPages)
	{
		MEMORY_BASIC_INFORMATION	MemInfo;
		if (VirtualQuery((void *)(pageNum * PageSize), &MemInfo,
					sizeof(MemInfo)))
		{
			if (MemInfo.RegionSize > 0)
			{
				// Adjust the page number to skip over this block of memory.
				pageNum += MemInfo.RegionSize / PageSize;
				if (MemInfo.State == MEM_COMMIT && MemInfo.AllocationBase >
							LastAllocationBase)
				{
					// Look for new blocks of committed memory, and try
					// recording their module names - this will fail
					// gracefully if they aren't code modules.
					LastAllocationBase = MemInfo.AllocationBase;
					ShowModuleInfo(hFile, (HINSTANCE)LastAllocationBase);
				}
			}
			else
				pageNum += SIXTYFOURK / PageSize;
		}
		else
			pageNum += SIXTYFOURK / PageSize;
		// If VirtualQuery fails we advance by 64K because that is the
		// granularity of address space doled out by VirtualAlloc().
	}
}



//
// This code for this function is ( stolen :) from Bruce Dawson's article in Game Developer Magazine Jan 99
//
static void ShowModuleInfo(HWFILE hFile, HINSTANCE ModuleHandle)
{
	char ModName[MAX_PATH];
	__try
	{
		if (GetModuleFileName(ModuleHandle, ModName, sizeof(ModName)) > 0)
		{
			// If GetModuleFileName returns greater than zero then this must
			// be a valid code module address. Therefore we can try to walk
			// our way through its structures to find the link time stamp.
			IMAGE_DOS_HEADER *DosHeader = (IMAGE_DOS_HEADER*)ModuleHandle;
		 if (IMAGE_DOS_SIGNATURE != DosHeader->e_magic)
	 	 return;
			IMAGE_NT_HEADERS *NTHeader = (IMAGE_NT_HEADERS*)((STR8 )DosHeader
						+ DosHeader->e_lfanew);
		 if (IMAGE_NT_SIGNATURE != NTHeader->Signature)
	 	 return;
			// Open the code module file so that we can get its file date
			// and size.
			HANDLE ModuleFile = CreateFile(ModName, GENERIC_READ,
						FILE_SHARE_READ, 0, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, 0);
			char TimeBuffer[100] = "";
			DWORD FileSize = 0;
			if (ModuleFile != INVALID_HANDLE_VALUE)
			{
				FileSize = GetFileSize(ModuleFile, 0);
				FILETIME	LastWriteTime;
				if (GetFileTime(ModuleFile, 0, 0, &LastWriteTime))
				{
					wsprintf(TimeBuffer, " - file date is ");
					PrintTime(TimeBuffer + lstrlen(TimeBuffer), LastWriteTime);
				}
				CloseHandle(ModuleFile);
			}
			ErrorLog( hFile, "%-35s, loaded at 0x%08x - %7d bytes - %08x%s\r\n",
						ModName, ModuleHandle, FileSize,
						NTHeader->FileHeader.TimeDateStamp, TimeBuffer);
		}
	}
	// Handle any exceptions by continuing from this point.
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

//////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/


#define _USE_VERSIONING_
#define _USE_PSAPI_

#ifdef _USE_VERSIONING_
#include <winver.h>
#endif
#define _USE_PSAPI_
#ifdef _USE_PSAPI_
// Copied from psapi.h
typedef struct _MODULEINFO {
	LPVOID lpBaseOfDll;
	DWORD SizeOfImage;
	LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;
#endif

#ifndef _IMAGEHLP64
// copied from imagehlp.h
typedef struct _ER_IMAGEHLP_LINE64 {
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE64)
	PVOID    Key;                    // internal
	DWORD    LineNumber;             // line number in file
	PCHAR    FileName;               // full filename
	DWORD64  Address;                // first instruction of line
} ER_IMAGEHLP_LINE64, *PER_IMAGEHLP_LINE64;
#endif

/*----------------------------------------------------------------------
   IsNT - Detect if this is an NT installation
----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

BOOL __stdcall IsNT ( void )
{
	static BOOL s_bHasVersion = FALSE ; // Indicates that the version information is valid.
	static BOOL s_bIsNT = TRUE ; // Indicates NT or 95/98.
	BOOL bRet;
    OSVERSIONINFO stOSVI ;

    if ( TRUE == s_bHasVersion )
        return ( TRUE == s_bIsNT ) ;

    memset ( &stOSVI , 0, sizeof ( OSVERSIONINFO ) ) ;
    stOSVI.dwOSVersionInfoSize = sizeof ( OSVERSIONINFO ) ;

    bRet = GetVersionEx ( &stOSVI ) ;
    if ( FALSE == bRet )
        return ( FALSE ) ;

    // Check the version and call the appropriate thing.
    if ( VER_PLATFORM_WIN32_NT == stOSVI.dwPlatformId )
        s_bIsNT = TRUE ;
    else
        s_bIsNT = FALSE ;
    s_bHasVersion = TRUE ;
    return ( TRUE == s_bIsNT ) ;
}

/*----------------------------------------------------------------------
   Version.dll Wrappers
----------------------------------------------------------------------*/
#ifdef _USE_VERSIONING_

typedef DWORD (__stdcall *FVN_GetFileVersionInfoSize)(LPCTSTR, LPDWORD );
static FVN_GetFileVersionInfoSize g_GetFileVersionInfoSize = NULL;

static DWORD __stdcall 
ERGetFileVersionInfoSize(LPCTSTR lptstrFilename, LPDWORD lpdwHandle)
{
	if (g_GetFileVersionInfoSize)
	{
		return (*g_GetFileVersionInfoSize)(lptstrFilename, lpdwHandle);
	}
	return 0;
}


typedef BOOL (__stdcall *FVN_GetFileVersionInfo)(LPCTSTR, DWORD, DWORD, LPVOID);
static FVN_GetFileVersionInfo g_GetFileVersionInfo = NULL;

static BOOL __stdcall
ERGetFileVersionInfo(LPCTSTR lptstrFilename, DWORD dwHandle, 
						  DWORD dwLen, LPVOID lpData)
{
	if (g_GetFileVersionInfo)
	{
		return (*g_GetFileVersionInfo)(lptstrFilename, dwHandle, dwLen, lpData);
	}
	return 0;
}

typedef BOOL (__stdcall *FVN_VerQueryValue)(const LPVOID, LPTSTR, LPVOID *, PUINT);
static FVN_VerQueryValue g_VerQueryValue = NULL;

static BOOL __stdcall
ERVerQueryValue(const LPVOID pBlock, LPTSTR lpSubBlock, 
			  	            LPVOID *lplpBuffer, PUINT puLen)
{
	if (g_VerQueryValue)
	{
		return (*g_VerQueryValue)(pBlock, lpSubBlock, lplpBuffer, puLen);
	}
	return 0;
}

static HMODULE g_VerMod = NULL;
static BOOL ERLoadVersionDLL()
{
	if (!g_VerMod)
	{
		g_VerMod = LoadLibrary("Version.dll");
		if (g_VerMod)
		{
			g_GetFileVersionInfoSize = (FVN_GetFileVersionInfoSize)GetProcAddress(g_VerMod, "GetFileVersionInfoSizeA");
			g_GetFileVersionInfo     = (FVN_GetFileVersionInfo)    GetProcAddress(g_VerMod, "GetFileVersionInfoA");
			g_VerQueryValue          = (FVN_VerQueryValue)         GetProcAddress(g_VerMod, "VerQueryValueA");
		}
	}
	if (g_VerMod && g_GetFileVersionInfoSize && g_GetFileVersionInfo && g_VerQueryValue)
		return TRUE;
	return FALSE;
}

#endif //_USE_VERSIONING_

/*----------------------------------------------------------------------
   imagehlp.dll Wrappers
----------------------------------------------------------------------*/
#if defined(_IMAGEHLP_) && defined(_X86_)

typedef BOOL (__stdcall *FVN_SymInitialize)(HANDLE, PSTR, BOOL);
static FVN_SymInitialize g_SymInitialize = NULL;

static BOOL __stdcall
ERSymInitialize(HANDLE hProcess, PSTR UserSearchPath, BOOL fInvadeProcess)
{
	if (g_SymInitialize)
	{
		return (*g_SymInitialize)(hProcess, UserSearchPath, fInvadeProcess);
	}
	return 0;
}


typedef BOOL (__stdcall *FVN_SymCleanup)(HANDLE);
static FVN_SymCleanup g_SymCleanup = NULL;

static BOOL __stdcall ERSymCleanup(HANDLE hProcess)
{
	if (g_SymCleanup)
	{
		return (*g_SymCleanup)(hProcess);
	}
	return 0;
}

typedef BOOL (__stdcall *FVN_StackWalk)(DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, 
			  PREAD_PROCESS_MEMORY_ROUTINE,  PFUNCTION_TABLE_ACCESS_ROUTINE ,
			  PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE);
static FVN_StackWalk g_StackWalk = NULL;

static BOOL __stdcall ERStackWalk(
  DWORD MachineType, 
  HANDLE hProcess, 
  HANDLE hThread, 
  LPSTACKFRAME StackFrame, 
  PVOID ContextRecord, 
  PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine,  
  PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine,
  PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine, 
  PTRANSLATE_ADDRESS_ROUTINE TranslateAddress 
)
{
	if (g_StackWalk)
	{
		return (*g_StackWalk)(MachineType, hProcess, hThread, StackFrame, 
				ContextRecord, ReadMemoryRoutine,  FunctionTableAccessRoutine, 
				GetModuleBaseRoutine, TranslateAddress 
			);
	}
	return 0;
}

typedef LPVOID (__stdcall *FVN_SymFunctionTableAccess)(HANDLE,  DWORD);
static FVN_SymFunctionTableAccess g_SymFunctionTableAccess = NULL;

static LPVOID __stdcall 
ERSymFunctionTableAccess(HANDLE hProcess,  DWORD AddrBase)
{
	if (g_SymFunctionTableAccess)
	{
		return (*g_SymFunctionTableAccess)(hProcess,  AddrBase);
	}
	return 0;
}

typedef BOOL (__stdcall *FVN_SymGetModuleBase)(HANDLE, DWORD);
static FVN_SymGetModuleBase g_SymGetModuleBase = NULL;

static DWORD __stdcall 
ERSymGetModuleBase(HANDLE hProcess, DWORD dwAddr)
{
	if (g_SymGetModuleBase)
	{
		return (*g_SymGetModuleBase)(hProcess, dwAddr);
	}
	return 0;
}

typedef BOOL (__stdcall *FVN_SymGetModuleInfo)(HANDLE, DWORD, PIMAGEHLP_MODULE);
static FVN_SymGetModuleInfo g_SymGetModuleInfo = NULL;

static BOOL __stdcall 
ERSymGetModuleInfo(HANDLE hProcess, DWORD dwAddr, PIMAGEHLP_MODULE ModuleInfo)
{
	if (g_SymGetModuleInfo)
	{
		return (*g_SymGetModuleInfo)(hProcess, dwAddr, ModuleInfo);
	}
	return 0;
}

typedef BOOL (__stdcall *FVN_SymGetSymFromAddr)(HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL);
static FVN_SymGetSymFromAddr g_SymGetSymFromAddr = NULL;

static BOOL __stdcall 
ERSymGetSymFromAddr(HANDLE hProcess, DWORD Address, PDWORD Displacement, PIMAGEHLP_SYMBOL Symbol)
{
	if (g_SymGetSymFromAddr)
	{
		return (*g_SymGetSymFromAddr)(hProcess, Address, Displacement, Symbol);
	}
	return 0;
}

typedef BOOL (__stdcall *FVN_SymGetLineFromAddr)(HANDLE, DWORD64, PDWORD, PER_IMAGEHLP_LINE64);
static FVN_SymGetLineFromAddr g_SymGetLineFromAddr = NULL;
static BOOL __stdcall 
	ERSymGetLineFromAddr(HANDLE hProcess, DWORD64 Address, PDWORD Displacement, PER_IMAGEHLP_LINE64 Line)
{
	if (g_SymGetLineFromAddr)
	{
		return (*g_SymGetLineFromAddr)(hProcess, Address, Displacement, Line);
	}
	return 0;
}

typedef BOOL (WINAPI *FVN_MiniDumpWriteDump)(	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);
static FVN_MiniDumpWriteDump g_MiniDumpWriteDump = NULL;

static CRITICAL_SECTION g_miniCritSec;

static BOOL __stdcall 
	ERMiniDumpWriteDump(HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam)
{
	if (g_MiniDumpWriteDump)
	{
		// DBGHELP.DLL is not thread safe
		BOOL result;
		EnterCriticalSection(&g_miniCritSec);
		result = (*g_MiniDumpWriteDump)(hProcess, dwPid, hFile, DumpType, ExceptionParam, UserStreamParam, CallbackParam);
		LeaveCriticalSection(&g_miniCritSec);
		return result;
	}
	return 0;
}


static HMODULE g_ImgHlpMod = NULL;
static BOOL ERLoadImageHlpDLL()
{
	if (!g_ImgHlpMod)
	{
		g_ImgHlpMod = LoadLibrary("dbghelp.dll");
		if (!g_ImgHlpMod)
			g_ImgHlpMod = LoadLibrary("Imagehlp.dll");

		if (g_ImgHlpMod)
		{
			g_SymInitialize = (FVN_SymInitialize)GetProcAddress(g_ImgHlpMod, "SymInitialize");
			g_SymCleanup = (FVN_SymCleanup)GetProcAddress(g_ImgHlpMod, "SymCleanup");
			g_StackWalk = (FVN_StackWalk)GetProcAddress(g_ImgHlpMod, "StackWalk");
			g_SymFunctionTableAccess = (FVN_SymFunctionTableAccess)GetProcAddress(g_ImgHlpMod, "SymFunctionTableAccess");
			g_SymGetModuleBase = (FVN_SymGetModuleBase)GetProcAddress(g_ImgHlpMod, "SymGetModuleBase");
			g_SymGetModuleInfo = (FVN_SymGetModuleInfo)GetProcAddress(g_ImgHlpMod, "SymGetModuleInfo");
			g_SymGetSymFromAddr = (FVN_SymGetSymFromAddr)GetProcAddress(g_ImgHlpMod, "SymGetSymFromAddr");
			g_SymGetLineFromAddr = (FVN_SymGetLineFromAddr)GetProcAddress(g_ImgHlpMod, "SymGetLineFromAddr64");

			InitializeCriticalSection(&g_miniCritSec);
			g_MiniDumpWriteDump = (FVN_MiniDumpWriteDump)GetProcAddress(g_ImgHlpMod, "MiniDumpWriteDump");
		}
	}
	if (g_ImgHlpMod && 
		g_SymInitialize    && g_SymCleanup && 
		g_StackWalk        && g_SymFunctionTableAccess && 
		g_SymGetModuleBase && g_SymGetModuleInfo &&
		g_SymGetSymFromAddr)
		return TRUE;
	return FALSE;
}
#endif // defined(_IMAGEHLP_) && defined(_X86_)


/*----------------------------------------------------------------------
   Version.dll Wrappers
----------------------------------------------------------------------*/
#ifdef _USE_PSAPI_

typedef BOOL (__stdcall *FVN_EnumProcessModules)(HANDLE, HMODULE *, DWORD , LPDWORD );
static FVN_EnumProcessModules g_EnumProcessModules = NULL;

static BOOL __stdcall 
EREnumProcessModules(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded)
{
	if (g_EnumProcessModules)
	{
		return (*g_EnumProcessModules)(hProcess, lphModule, cb, lpcbNeeded);
	}
	return 0;
}

typedef BOOL (__stdcall *FVN_GetModuleInformation)(HANDLE, HMODULE, LPMODULEINFO, DWORD);
static FVN_GetModuleInformation g_GetModuleInformation = NULL;

static BOOL __stdcall 
ERGetModuleInformation(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb)
{
	if (g_GetModuleInformation)
	{
		return (*g_GetModuleInformation)(hProcess, hModule, lpmodinfo, cb);
	}
	return 0;
}

static HMODULE g_PSAPIMod = NULL;
static BOOL ERLoadPSAPIDLL()
{
	if (!g_PSAPIMod)
	{
		g_PSAPIMod = LoadLibrary("psapi.dll");
		if (g_PSAPIMod)
		{
			g_EnumProcessModules = (FVN_EnumProcessModules)GetProcAddress(g_PSAPIMod, "EnumProcessModules");
			g_GetModuleInformation = (FVN_GetModuleInformation)GetProcAddress(g_PSAPIMod, "GetModuleInformation");
		}
	}
	if (g_PSAPIMod && g_EnumProcessModules && g_GetModuleInformation)
		return TRUE;
	return FALSE;
}


#endif //_USE_PSAPI_


#pragma region Crash MiniDump Handler

static BOOL ERGetImpersonationToken(HANDLE* phToken)
{
	*phToken = NULL;
	if(!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, TRUE, phToken))
	{
		if(GetLastError() == ERROR_NO_TOKEN)
		{
			// No impersonation token for the current thread available - go for the process token
			if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, phToken))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

static BOOL EREnablePriv(LPCTSTR pszPriv, HANDLE hToken, TOKEN_PRIVILEGES* ptpOld)
{
	BOOL bOk = FALSE;

	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	bOk = LookupPrivilegeValue( 0, pszPriv, &tp.Privileges[0].Luid);
	if(bOk)
	{
		DWORD cbOld = sizeof(*ptpOld);
		bOk = AdjustTokenPrivileges(hToken, FALSE, &tp, cbOld, ptpOld, &cbOld);
	}

	return (bOk && (ERROR_NOT_ALL_ASSIGNED != GetLastError()));
}

static BOOL ERRestorePriv(HANDLE hToken, TOKEN_PRIVILEGES* ptpOld)
{
	BOOL bOk = AdjustTokenPrivileges(hToken, FALSE, ptpOld, 0, 0, 0);	
	return (bOk && (ERROR_NOT_ALL_ASSIGNED != GetLastError()));
}

static BOOL ERGenerateMiniDump(CHAR *szFileName, PEXCEPTION_POINTERS pExceptionInfo)
{
	BOOL bRet = FALSE;
	DWORD dwLastError = 0;
	HANDLE hDumpFile = 0;
	MINIDUMP_EXCEPTION_INFORMATION stInfo = {0};
	TOKEN_PRIVILEGES tp;
	HANDLE hImpersonationToken = NULL;
	BOOL bPrivilegeEnabled;

	if(!ERGetImpersonationToken(&hImpersonationToken))
	{
		return FALSE;
	}

	// Create the dump file
	hDumpFile = CreateFileA(szFileName, 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_WRITE | FILE_SHARE_READ, 
		0, CREATE_ALWAYS, 0, 0);
	if(hDumpFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hImpersonationToken);
		return FALSE;
	}

	// Write the dump
	stInfo.ThreadId = GetCurrentThreadId();
	stInfo.ExceptionPointers = pExceptionInfo;
	stInfo.ClientPointers = TRUE;

	// We need the SeDebugPrivilege to be able to run MiniDumpWriteDump
	bPrivilegeEnabled = EREnablePriv(SE_DEBUG_NAME, hImpersonationToken, &tp);

// VS 2008 and VS 2010
#if _MSC_VER >= 1500
	bRet = ERMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile
		, (MINIDUMP_TYPE)(MiniDumpWithHandleData|MiniDumpWithThreadInfo|MiniDumpWithDataSegs), &stInfo, NULL, NULL);
// VS 2005
#else
	bRet = ERMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile
		, (MINIDUMP_TYPE)(MiniDumpWithHandleData|MiniDumpWithDataSegs), &stInfo, NULL, NULL);
#endif

	if(bPrivilegeEnabled)
	{
		// Restore the privilege
		ERRestorePriv(hImpersonationToken, &tp);
	}
	
	CloseHandle(hDumpFile);
	CloseHandle(hImpersonationToken);

	return bRet;
}


/*----------------------------------------------------------------------
   Handler
----------------------------------------------------------------------*/

LONG __stdcall ERCrashDumpExceptionFilterEx(const CHAR *pAppName, const CHAR* pPath, EXCEPTION_POINTERS* pExPtrs)
{
	LONG lRet = EXCEPTION_CONTINUE_SEARCH ;
	FILE *fdump = NULL;
	HANDLE hProc = GetCurrentProcess();
	SYSTEMTIME stTime;
	CHAR pszFilename[MAX_PATH], szPathName[_MAX_PATH], szBuffer[_MAX_PATH];
	size_t cbFilename = sizeof(pszFilename) / sizeof(pszFilename[0]) - 1;
	if (!ERLoadImageHlpDLL())
		return lRet;

	__try
	{
		szPathName[0] = 0;
		if (pPath && *pPath)
			GetFullPathNameA(pPath, MAX_PATH, szPathName, NULL);
		if (szPathName[strlen(szPathName)-1] != '\\')
			lstrcat(szPathName, "\\");

		// Create filename
		GetLocalTime(&stTime); 

		if (pAppName == NULL)
		{
			GetModuleFileName(NULL, szBuffer, MAX_PATH);
			CHAR *pszFilePart = strrchr(szBuffer, '\\');
			pszFilePart = (pszFilePart == NULL) ? szBuffer : pszFilePart+1;
			if (CHAR *pExt = strrchr(pszFilePart, '.') )
				*pExt = 0;
			pAppName = pszFilePart;
		}

		// Filename is composed like this, to avoid collisions;
		// <DumpPath>\<APP>-Crash-<PID>-<TID>-YYYYMMDD-HHMMSS.dmp
		_snprintf_s(pszFilename, cbFilename, cbFilename, "%s-Crash-%ld-%ld-%04d%02d%02d-%02d%02d%02d", pAppName, GetCurrentProcessId(), GetCurrentThreadId(), stTime.wYear,stTime.wMonth,stTime.wDay,stTime.wHour, stTime.wMinute, stTime.wSecond);
		lstrcat(szPathName, pszFilename);
		lstrcpy(szPathName, ".dmp");

		// Generate proper mini dump
		ERGenerateMiniDump(szPathName, pExPtrs);
	}	
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		lRet = EXCEPTION_CONTINUE_SEARCH ;
	}
	if (hProc != (HANDLE)0xFFFFFFFF) CloseHandle(hProc);
	if (fdump) fclose(fdump);
	return ( lRet ) ;
}

LONG __stdcall ERCrashDumpExceptionFilter (EXCEPTION_POINTERS* pExPtrs)
{
	return ERCrashDumpExceptionFilterEx(NULL, NULL, pExPtrs);
}


LONG __stdcall ERGetVersionStringA(LPCSTR szModName, LPSTR szVersion, int maxlen)
{
#ifdef _USE_VERSIONING_
	if (ERLoadVersionDLL())
	{
		UINT  dwBytes = 0;     
		LPVOID lpBuffer = 0; 
		LPVOID lpData;
		DWORD dwSize;

		szVersion[0] = 0;
		dwSize = ERGetFileVersionInfoSize(szModName, 0);
		lpData = alloca(dwSize);
		ERGetFileVersionInfo(szModName, 0, dwSize, lpData);
		if (ERVerQueryValue(lpData, TEXT("\\"), &lpBuffer, &dwBytes))
		{
			VS_FIXEDFILEINFO *lpvs = (VS_FIXEDFILEINFO *)lpBuffer;
			if (lpvs->dwFileVersionLS)
			{
				sprintf_s(szVersion, maxlen, "%d.%d.%d.%d",
					HIWORD(lpvs->dwFileVersionMS),    LOWORD(lpvs->dwFileVersionMS),
					HIWORD(lpvs->dwFileVersionLS),    LOWORD(lpvs->dwFileVersionLS)
					);
			}
			else if (lpvs->dwFileVersionMS)
			{
				sprintf_s(szVersion, maxlen, "%d.%d",
					HIWORD(lpvs->dwFileVersionMS),    LOWORD(lpvs->dwFileVersionMS)
					);
			}
			return strlen(szVersion);
		}
	}
#endif
	return 0;
}



#if defined(_IMAGEHLP_) && defined(_X86_)

BOOL __stdcall ERReadProcessMemory ( HANDLE   hProc,
                                      LPCVOID lpBaseAddress,
                                      LPVOID  lpBuffer,
                                      DWORD   nSize,
                                      LPDWORD lpNumberOfBytesRead  )
{
    return ( ReadProcessMemory ( GetCurrentProcess ( ) ,
                                 lpBaseAddress         ,
                                 lpBuffer              ,
                                 nSize                 ,
                                 lpNumberOfBytesRead    ) ) ;
}

static void ERLogStackWalk(HWFILE fdump, EXCEPTION_POINTERS* pExPtrs)
{
    #define SAVE_EBP(f)        f.Reserved[0]
    #define TRAP_TSS(f)        f.Reserved[1]
    #define TRAP_EDITED(f)     f.Reserved[1]
    #define SAVE_TRAP(f)       f.Reserved[2]

    CONTEXT	Context;
    DWORD dwDisplacement = 0;
    char *szSymName;
    IMAGEHLP_MODULE mi;
    STACKFRAME stFrame;
	DWORD i;
	HANDLE hProc = (HANDLE)GetCurrentProcess();

	memcpy( &Context, pExPtrs->ContextRecord, sizeof( CONTEXT ) );

	if (!ERLoadImageHlpDLL())
		return;

	ErrorLog(fdump, "Stack Walk:\r\n");

	ERSymInitialize(hProc, NULL, TRUE);

	memset(g_sym, 0, MAX_SYMNAME_SIZE + sizeof(IMAGEHLP_SYMBOL) ) ;
    g_sym->SizeOfStruct  = sizeof(IMAGEHLP_SYMBOL);
    g_sym->MaxNameLength = MAX_SYMNAME_SIZE;


    ZeroMemory( &stFrame, sizeof(stFrame) );

    stFrame.AddrPC.Offset       = Context.Eip ;
    stFrame.AddrPC.Mode         = AddrModeFlat                ;
    stFrame.AddrStack.Offset    = Context.Esp ;
    stFrame.AddrStack.Mode      = AddrModeFlat                ;
    stFrame.AddrFrame.Offset    = Context.Ebp ;
    stFrame.AddrFrame.Mode      = AddrModeFlat                ;

    ErrorLog(fdump, "FramePtr ReturnAd Param#1  Param#2  Param#3  Param#4  Function Name\r\n");

    for (i=0; i<15; i++) 
	{
        if (!ERStackWalk( IMAGE_FILE_MACHINE_I386,
						hProc,
						GetCurrentThread(),
						&stFrame,
                        &Context,
                        NULL,
						ERSymFunctionTableAccess,
						ERSymGetModuleBase,
                        NULL)) 
		{
            break;
        }
		
        if (ERSymGetSymFromAddr(hProc, stFrame.AddrPC.Offset, &dwDisplacement, g_sym)) {
            szSymName = g_sym->Name;
        }
        else {
            szSymName = "<nosymbols>";
        }
        ErrorLog(fdump, "%08x %08x %08x %08x %08x %08x ",
					  stFrame.AddrFrame.Offset,
					  stFrame.AddrReturn.Offset,
					  stFrame.Params[0],
					  stFrame.Params[1],
					  stFrame.Params[2],
					  stFrame.Params[3]
                );

        memset(&mi, 0, sizeof(mi));
        mi.SizeOfStruct = sizeof(mi);
        if (ERSymGetModuleInfo(hProc, stFrame.AddrPC.Offset, &mi )) {
            ErrorLog(fdump, "%s!", mi.ModuleName );
        }

        ErrorLog(fdump, "%s ", szSymName );

        if (g_sym && (g_sym->Flags & SYMF_OMAP_GENERATED || g_sym->Flags & SYMF_OMAP_MODIFIED)) {
            ErrorLog(fdump, "[omap] " );
        }

        if (stFrame.FuncTableEntry) 
		{
            PFPO_DATA pFpoData = (PFPO_DATA)stFrame.FuncTableEntry;
            switch (pFpoData->cbFrame) 
			{
                case FRAME_FPO:
                    if (pFpoData->fHasSEH) 
					{
                        ErrorLog(fdump, "(FPO: [SEH])" );
                    } else 
					{
                        ErrorLog(fdump, " (FPO:" );
                        if (pFpoData->fUseBP) 
						{
                            ErrorLog(fdump, " [EBP 0x%08x]", SAVE_EBP(stFrame) );
                        }
                        ErrorLog(fdump, " [%d,%d,%d])",   pFpoData->cdwParams,
														 pFpoData->cdwLocals,
														 pFpoData->cbRegs);
                    }
                    break;
                case FRAME_NONFPO:
                    ErrorLog(fdump, "(FPO: Non-FPO [%d,%d,%d])",
                                 pFpoData->cdwParams,
                                 pFpoData->cdwLocals,
                                 pFpoData->cbRegs);
                    break;

                case FRAME_TRAP:
                case FRAME_TSS:
                default:
                    ErrorLog(fdump, "(UNKNOWN FPO TYPE)" );
                    break;
            }
        }
		ER_IMAGEHLP_LINE64 lineInfo;
		ZeroMemory( &lineInfo, sizeof(lineInfo) );
		lineInfo.SizeOfStruct = sizeof(lineInfo);
		dwDisplacement = 0;
		if ( ERSymGetLineFromAddr(hProc, stFrame.AddrPC.Offset, &dwDisplacement, &lineInfo) )
			ErrorLog(fdump, " \t%s:%d ", lineInfo.FileName, lineInfo.LineNumber);

        ErrorLog(fdump, "\r\n" );
    }
    ErrorLog(fdump, "\r\n" );

	ERSymCleanup(hProc);

    return;
}
#endif //_IMAGEHLP_

//************************************
// Method:    ERLogStackWalk
// FullName:  ERLogStackWalk
// Access:    public static 
// Returns:   LPCSTR  (Free with LocalFree)
// Qualifier:
// Parameter: EXCEPTION_POINTERS * pExPtrs
//************************************
LPCSTR ERStackWalk(EXCEPTION_POINTERS* pExPtrs)
{
#if defined(_IMAGEHLP_) && defined(_X86_)
	CONTEXT	Context;
	DWORD dwDisplacement = 0;
	char *szSymName;
	IMAGEHLP_MODULE mi;
	STACKFRAME stFrame;
	DWORD i;
	HANDLE hProc = (HANDLE)GetCurrentProcess();

	if (!ERLoadImageHlpDLL())
		return NULL;

	std::stringstream ss;

	HMODULE hModule = GetModuleHandle(NULL);

	memcpy( &Context, pExPtrs->ContextRecord, sizeof( CONTEXT ) );

	ERSymInitialize(hProc, NULL, TRUE);

	memset(g_sym, 0, MAX_SYMNAME_SIZE + sizeof(IMAGEHLP_SYMBOL) ) ;
	g_sym->SizeOfStruct  = sizeof(IMAGEHLP_SYMBOL);
	g_sym->MaxNameLength = MAX_SYMNAME_SIZE;

	ZeroMemory( &stFrame, sizeof(stFrame) );

	stFrame.AddrPC.Offset       = Context.Eip ;
	stFrame.AddrPC.Mode         = AddrModeFlat                ;
	stFrame.AddrStack.Offset    = Context.Esp ;
	stFrame.AddrStack.Mode      = AddrModeFlat                ;
	stFrame.AddrFrame.Offset    = Context.Ebp ;
	stFrame.AddrFrame.Mode      = AddrModeFlat                ;

	for (i=0; i<100; i++) 
	{
		if (!ERStackWalk( IMAGE_FILE_MACHINE_I386,
			hProc,
			GetCurrentThread(),
			&stFrame,
			&Context,
			NULL,
			ERSymFunctionTableAccess,
			ERSymGetModuleBase,
			NULL)) 
		{
			break;
		}

		if (ERSymGetSymFromAddr(hProc, stFrame.AddrPC.Offset, &dwDisplacement, g_sym)) {
			szSymName = g_sym->Name;
		}
		else {
			szSymName = "<nosymbols>";
		}
		memset(&mi, 0, sizeof(mi));
		mi.SizeOfStruct = sizeof(mi);
		if (ERSymGetModuleInfo(hProc, stFrame.AddrPC.Offset, &mi )) {
			if (mi.BaseOfImage != (DWORD)hModule)
			{
				ss << mi.ModuleName << "!";
			}
		}
		ss << szSymName << " ";

		ER_IMAGEHLP_LINE64 lineInfo;
		ZeroMemory( &lineInfo, sizeof(lineInfo) );
		lineInfo.SizeOfStruct = sizeof(lineInfo);
		dwDisplacement = 0;
		if ( ERSymGetLineFromAddr(hProc, stFrame.AddrPC.Offset, &dwDisplacement, &lineInfo) )
		{
			LPSTR pFileName = strrchr(lineInfo.FileName, '\\');
			if (pFileName) ++pFileName;
			if (!pFileName) pFileName = lineInfo.FileName;

			ss << " \t" << pFileName << ":" << lineInfo.LineNumber << " ";
		}
		ss << std::endl;
	}
	ss << std::endl;

	ERSymCleanup(hProc);

	std::string str = ss.str();
	LPSTR lpResult = (LPSTR)LocalAlloc(LPTR, str.length()+1);
	strcpy(lpResult, str.c_str());
	return lpResult;
#else
	return NULL;
#endif //_IMAGEHLP_
}


//************************************
// Method:    ERGetFirstModuleException
// FullName:  ERGetFirstModuleException
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: EXCEPTION_POINTERS * pExPtrs
// Parameter: HMODULE hModule
// Parameter: LPSTR funcName
// Parameter: INT funcNameLen
// Parameter: LPSTR sourceName
// Parameter: INT sourceNameLen
// Parameter: INT * lpLineNum
//************************************
BOOL ERGetFirstModuleException(
	  EXCEPTION_POINTERS* pExPtrs
	, HMODULE hModule
	, LPSTR funcName, INT funcNameLen
	, LPSTR sourceName, INT sourceNameLen
	, INT *lpLineNum
	)
{
#if defined(_IMAGEHLP_) && defined(_X86_)
	CONTEXT	Context;
	DWORD dwDisplacement = 0;
	char *szSymName;
	IMAGEHLP_MODULE mi;
	STACKFRAME stFrame;
	DWORD i;
	HANDLE hProc = (HANDLE)GetCurrentProcess();
	BOOL bFound = FALSE;

	if (!ERLoadImageHlpDLL())
		return FALSE;

	memcpy( &Context, pExPtrs->ContextRecord, sizeof( CONTEXT ) );

	if (hModule == NULL)
		hModule = GetModuleHandle(NULL);

	ERSymInitialize(hProc, NULL, TRUE);

	memset(g_sym, 0, MAX_SYMNAME_SIZE + sizeof(IMAGEHLP_SYMBOL) ) ;
	g_sym->SizeOfStruct  = sizeof(IMAGEHLP_SYMBOL);
	g_sym->MaxNameLength = MAX_SYMNAME_SIZE;

	ZeroMemory( &stFrame, sizeof(stFrame) );

	stFrame.AddrPC.Offset       = Context.Eip ;
	stFrame.AddrPC.Mode         = AddrModeFlat                ;
	stFrame.AddrStack.Offset    = Context.Esp ;
	stFrame.AddrStack.Mode      = AddrModeFlat                ;
	stFrame.AddrFrame.Offset    = Context.Ebp ;
	stFrame.AddrFrame.Mode      = AddrModeFlat                ;

	for (i=0; i<15; i++) 
	{
		if (!ERStackWalk( IMAGE_FILE_MACHINE_I386,
			hProc,
			GetCurrentThread(),
			&stFrame,
			&Context,
			NULL,
			ERSymFunctionTableAccess,
			ERSymGetModuleBase,
			NULL)) 
		{
			break;
		}

		if (ERSymGetSymFromAddr(hProc, stFrame.AddrPC.Offset, &dwDisplacement, g_sym)) {
			szSymName = g_sym->Name;
		}
		else {
			szSymName = "<nosymbols>";
		}
		memset(&mi, 0, sizeof(mi));
		mi.SizeOfStruct = sizeof(mi);
		if (!ERSymGetModuleInfo(hProc, stFrame.AddrPC.Offset, &mi )) 
			continue;

		if (mi.BaseOfImage != (DWORD)hModule)
			continue;

		ER_IMAGEHLP_LINE64 lineInfo;
		ZeroMemory( &lineInfo, sizeof(lineInfo) );
		lineInfo.SizeOfStruct = sizeof(lineInfo);
		dwDisplacement = 0;
		if ( ERSymGetLineFromAddr(hProc, stFrame.AddrPC.Offset, &dwDisplacement, &lineInfo) )
		{
			LPSTR pFileName = strrchr(lineInfo.FileName, '\\');
			if (pFileName) ++pFileName;
			if (!pFileName) pFileName = lineInfo.FileName;

			if (funcName) lstrcpyn(funcName,szSymName, funcNameLen);
			if (sourceName) lstrcpyn(sourceName,pFileName, sourceNameLen);
			if (lpLineNum) *lpLineNum = lineInfo.LineNumber;
			bFound = TRUE;
			break;
		}
	}

	ERSymCleanup(hProc);
	return bFound;
#else
	return FALSE;
#endif //_IMAGEHLP_
}


void ERLogModules(HWFILE fdump)
{
#ifdef _USE_PSAPI_
	if (!ERLoadImageHlpDLL())
		return;

	HANDLE hProc = (HANDLE)GetCurrentProcess();
	ERSymInitialize(hProc, NULL, TRUE);

	if (IsNT())
	{
		if (ERLoadPSAPIDLL())
		{
			HMODULE hMods[1024];
			DWORD cbNeeded;
			unsigned int i;

#ifdef _USE_VERSIONING_
			BOOL bVerOK = ERLoadVersionDLL();
#endif
			if( EREnumProcessModules(hProc, hMods, sizeof(hMods), &cbNeeded))
			{
				for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
				{
					char szModName[MAX_PATH];
					MODULEINFO mi;
					memset(&mi, 0, sizeof(MODULEINFO));

					ERGetModuleInformation(hProc, hMods[i], &mi, sizeof(MODULEINFO));

					// Get the full path to the module's file.
					if ( GetModuleFileName( hMods[i], szModName, sizeof(szModName)))
					{
						BOOL bPrintSimple = TRUE;

						// Open the code module file so that we can get its file date
						// and size.
						HANDLE ModuleFile = CreateFile(szModName, GENERIC_READ,
							FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
						char TimeBuffer[100] = "";
						DWORD FileSize = 0;
						if (ModuleFile != INVALID_HANDLE_VALUE)
						{
							FileSize = GetFileSize(ModuleFile, 0);
							FILETIME	LastWriteTime;
							if (GetFileTime(ModuleFile, 0, 0, &LastWriteTime))
							{
								wsprintf(TimeBuffer, " - file date is ");
								PrintTime(TimeBuffer + lstrlen(TimeBuffer), LastWriteTime);
							}
							CloseHandle(ModuleFile);
						}

#ifdef _USE_VERSIONING_
						if (bVerOK)
						{
							UINT  dwBytes = 0;     
							LPVOID lpBuffer = 0; 
							LPVOID lpData;
							DWORD dwSize;

							dwSize = ERGetFileVersionInfoSize(szModName, 0);
							lpData = alloca(dwSize);
							ERGetFileVersionInfo(szModName, 0, dwSize, lpData);
							if (ERVerQueryValue(lpData, TEXT("\\"), &lpBuffer, &dwBytes))
							{
								VS_FIXEDFILEINFO *lpvs = (VS_FIXEDFILEINFO *)lpBuffer;

								ErrorLog(fdump, "(%.8X - %.8X) %s \t %d.%d.%d.%d \t %d.%d.%d.%d \t %s\r\n",
									mi.lpBaseOfDll, ((LPBYTE)(mi.lpBaseOfDll)) + mi.SizeOfImage,
									szModName,
									HIWORD(lpvs->dwFileVersionMS),    LOWORD(lpvs->dwFileVersionMS),
									HIWORD(lpvs->dwFileVersionLS),    LOWORD(lpvs->dwFileVersionLS),
									HIWORD(lpvs->dwProductVersionMS), LOWORD(lpvs->dwProductVersionMS),
									HIWORD(lpvs->dwProductVersionLS), LOWORD(lpvs->dwProductVersionLS),
									TimeBuffer
									);
								bPrintSimple = FALSE;
							}
						}
#endif
						if (bPrintSimple)
						{
							ErrorLog(fdump, "(%.8X - %.8X) %s \t %s\r\n",
								mi.lpBaseOfDll, ((LPBYTE)(mi.lpBaseOfDll)) + mi.SizeOfImage,
								szModName, TimeBuffer
								);
						}
					}
				}
			}
		}
	}
	ERSymCleanup(hProc);
#endif	
}

#endif
