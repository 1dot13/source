#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include <stdio.h>
	#include "Font Control.h" 
	#include "renderworld.h"
	#include "render dirty.h"
	#include "loadscreen.h"
	#include "selectwin.h"
	#include "EditorDefines.h"
	#include "messagebox.h"
	#include "Text Input.h"
	#include "Soldier Create.h"
	#include "Soldier Init List.h"
	#include "EditorBuildings.h"
	#include "Editor Taskbar Utils.h"
	#include "Editor Undo.h"
	#include "editscreen.h"
	#include "strategicmap.h"
	#include "Editor Modes.h"
	#include "Map Information.h"
	#include "Sys Globals.h"
	#include "Sector Summary.h"
	#include "newsmooth.h"
	#include "Simple Render Utils.h"
	#include "Animated ProgressBar.h"
	#include "EditorMercs.h"
	#include "lighting.h"
	#include "EditorMapInfo.h"
	#include "environment.h"
	#include "edit_sys.h"
	#include "EditorItems.h"
	#include "english.h"
	#include "gameloop.h"
	#include "message.h"
	#include "pits.h"
	#include "Item Statistics.h"
	#include "Scheduling.h"
	#include "MessageBoxScreen.h"
	#include <vfs/Core/vfs.h>//dnl ch37 110909
#endif

INT16 gsMouseWheelDeltaValue;

//===========================================================================

BOOLEAN gfErrorCatch = FALSE;
CHAR16 gzErrorCatchString[ 256 ] = L"";
INT32	giErrorCatchMessageBox = 0;

extern void RemoveMercsInSector();

enum{
	DIALOG_NONE,
	DIALOG_SAVE,
	DIALOG_LOAD,
	DIALOG_CANCEL,
	DIALOG_DELETE
};

extern UINT16 Counter;

//Hook into the text input code.  These callbacks help give back control, so we
//can use the dialog interface in conjunction with the 
void FDlgOkCallback( GUI_BUTTON *butn, INT32 reason );
void FDlgCancelCallback( GUI_BUTTON *butn, INT32 reason );
void FDlgUpCallback( GUI_BUTTON *butn, INT32 reason );
void FDlgDwnCallback( GUI_BUTTON *butn, INT32 reason );
void FDlgNamesCallback( GUI_BUTTON *butn, INT32 reason );
void UpdateWorldInfoCallback( GUI_BUTTON *b, INT32 reason );
void FileDialogModeCallback( UINT8 ubID, BOOLEAN fEntering );

UINT32 ProcessLoadSaveScreenMessageBoxResult();
BOOLEAN RemoveFromFDlgList( FDLG_LIST **head, FDLG_LIST *node );

void DrawFileDialog();
void HandleMainKeyEvents( InputAtom *pEvent );
void HandleMouseWheelEvents(void);//dnl ch36 150909
void SetTopFileToLetter( UINT16 usLetter );

INT32 iTotalFiles;
INT32 iTopFileShown;
INT32 iCurrFileShown;
INT32	iLastFileClicked;
INT32 iLastClickTime;

CHAR16 gzFilename[FILENAME_BUFLEN];//dnl ch39 190909
extern INT16 gsSelSectorX;
extern INT16 gsSelSectorY;

FDLG_LIST *FileList = NULL;

INT32 iFDlgState = DIALOG_NONE;
BOOLEAN gfDestroyFDlg = FALSE;
INT32 iFileDlgButtons[7];

BOOLEAN gfLoadError;
BOOLEAN gfReadOnly;
BOOLEAN gfFileExists;
BOOLEAN gfIllegalName;
BOOLEAN gfDeleteFile;
BOOLEAN gfNoFiles;
BOOLEAN gfSaveError;//dnl ch37 200909

CHAR16 zOrigName[60];
GETFILESTRUCT FileInfo;

BOOLEAN fEnteringLoadSaveScreen = TRUE;
BOOLEAN gfPassedSaveCheck = FALSE;

MOUSE_REGION BlanketRegion;

CHAR8	 gszCurrFilename[80];

enum{
	IOSTATUS_NONE,
	INITIATE_MAP_SAVE,
	SAVING_MAP,
	INITIATE_MAP_LOAD,
	LOADING_MAP
};
INT8 gbCurrentFileIOStatus;  //1 init saving message, 2 save, 3 init loading message, 4 load, 0 none
UINT32 ProcessFileIO();

//BOOLEAN fSavingFile;
extern UINT16 gusLightLevel, gusSavedLightLevel;
UINT32 LoadSaveScreenInit(void)
{
	gfUpdateSummaryInfo = TRUE;
	fEnteringLoadSaveScreen = TRUE;
	return TRUE;
}

UINT32 LoadSaveScreenShutdown(void)
{
 return TRUE;
}

void LoadSaveScreenEntry()
{
	fEnteringLoadSaveScreen = FALSE;
	gbCurrentFileIOStatus	= IOSTATUS_NONE;

	gfReadOnly = FALSE;
	gfFileExists = FALSE;
	gfLoadError = FALSE;
	gfIllegalName = FALSE;
	gfDeleteFile = FALSE;
	gfNoFiles = FALSE;
	gfSaveError = FALSE;//dnl ch37 200909

	// setup filename dialog box
	// (*.dat and *.map) as file filter

	// If user clicks on a filename in the window, then turn off string focus and re-init the string with the new name.
	// If user hits ENTER or presses OK, then continue with the file loading/saving

	if(FileList)
	{
		TrashFDlgList(FileList);
		FileList = NULL;
	}

	iTopFileShown = iTotalFiles = 0;
#ifdef USE_VFS//dnl ch37 300909
	FDLG_LIST* TempFileList = NULL;
	vfs::CProfileStack* st = getVFS()->getProfileStack();
	vfs::CProfileStack::Iterator it = st->begin();
	while(!it.end())
	{
		TempFileList = NULL;
		vfs::CVirtualProfile* prof = it.value();
		memset(&FileInfo, 0, sizeof(GETFILESTRUCT));
		strcpy(FileInfo.zFileName, "< ");
		strcat(FileInfo.zFileName, prof->cName.utf8().c_str());
		strcat(FileInfo.zFileName, " >");
		FileInfo.zFileName[FILENAME_BUFLEN] = 0;
		FileInfo.uiFileAttribs = FILE_IS_DIRECTORY;
		if(iCurrentAction == ACTION_SAVE_MAP && prof->cWritable == false)
		{
			it.next();
			continue;
		}
		TempFileList = AddToFDlgList(TempFileList, &FileInfo);
		iTotalFiles++;
		vfs::CVirtualProfile::FileIterator fit = prof->files(L"MAPS/*");
		while(!fit.end())
		{
			vfs::String fname = fit.value()->getName().c_str();
			memset(&FileInfo, 0, sizeof(GETFILESTRUCT));
			strcpy(FileInfo.zFileName, fname.utf8().c_str());
			FileInfo.uiFileSize = fname.length();
			FileInfo.uiFileAttribs = (fit.value()->implementsWritable() ? FILE_IS_NORMAL : FILE_IS_READONLY);
			if(strlen(FileInfo.zFileName) < FILENAME_BUFLEN)
			{
				TempFileList = AddToFDlgList(TempFileList, &FileInfo);
				iTotalFiles++;
			}
			fit.next();
		}
		if(TempFileList)
		{
			while(TempFileList->pPrev)
				TempFileList = TempFileList->pPrev;
			if(FileList)
			{
				while(FileList->pNext)
					FileList = FileList->pNext;
				FileList->pNext = TempFileList;
				TempFileList->pPrev = FileList;
			}
			else
				FileList = TempFileList;
		}
		it.next();
	}
	while(FileList->pPrev)
		FileList = FileList->pPrev;
#else
	if(GetFileFirst("MAPS\\*.dat", &FileInfo))
	{
		if(strlen(FileInfo.zFileName) < FILENAME_BUFLEN)
		{
			FileList = AddToFDlgList(FileList, &FileInfo);
			iTotalFiles++;
		}
		while(GetFileNext(&FileInfo))
		{
			if(strlen(FileInfo.zFileName) < FILENAME_BUFLEN)
			{
				FileList = AddToFDlgList(FileList, &FileInfo);
				iTotalFiles++;
			}
		}
		GetFileClose(&FileInfo);
	}
#endif
	swprintf( zOrigName, L"%s Map (*.dat)", iCurrentAction == ACTION_SAVE_MAP ? L"Save" : L"Load" );

	swprintf( gzFilename, L"%S", gubFilename );

	CreateFileDialog( zOrigName );

	if( !iTotalFiles )
	{
		gfNoFiles = TRUE;
		if( iCurrentAction == ACTION_LOAD_MAP )
			DisableButton( iFileDlgButtons[0] );
	}

	iLastFileClicked = -1;
	iLastClickTime = 0;

}

UINT32 ProcessLoadSaveScreenMessageBoxResult()
{
	FDLG_LIST *curr, *temp;
	gfRenderWorld = TRUE;
	RemoveMessageBox();
	if( gfIllegalName )
	{
		fEnteringLoadSaveScreen = TRUE;
		RemoveFileDialog();
		MarkWorldDirty();
		return gfMessageBoxResult ? LOADSAVE_SCREEN : EDIT_SCREEN;
	}
	if( gfDeleteFile )
	{
		if( gfMessageBoxResult )
		{ //delete file
			INT32 x;
			curr = FileList;
			for( x = 0; x < iCurrFileShown && x < iTotalFiles && curr; x++ )
			{
				curr = curr->pNext;
			}
			if( curr )
			{
				if( gfReadOnly )
				{
					FileClearAttributes( gszCurrFilename );
					gfReadOnly = FALSE;
				}
				FileDelete( gszCurrFilename );

				//File is deleted so redo the text fields so they show the 
				//next file in the list.
				temp = curr->pNext;
				if( !temp )
					temp = curr->pPrev;
				if( !temp )
					wcscpy( gzFilename, L"" );
				else
					swprintf( gzFilename, L"%S", temp->FileInfo.zFileName );
				if( ValidFilename() )
				{
					SetInputFieldStringWith16BitString( 0, gzFilename );
				}
				else
				{
					SetInputFieldStringWith16BitString( 0, L"" );
					wcscpy( gzFilename, L"" );
				}
				RemoveFromFDlgList( &FileList, curr );
				iTotalFiles--;
				if( !iTotalFiles )
				{
					gfNoFiles = TRUE;
					if( iCurrentAction == ACTION_LOAD_MAP )
						DisableButton( iFileDlgButtons[0] );
				}
				if( iCurrFileShown >= iTotalFiles )
					iCurrFileShown--;
				if( iCurrFileShown < iTopFileShown )
					iTopFileShown -= 8;
				if( iTopFileShown < 0 )
					iTopFileShown = 0;
			}
		}
		MarkWorldDirty();
		RenderWorld();
		gfDeleteFile = FALSE;
		iFDlgState = DIALOG_NONE;
		return LOADSAVE_SCREEN;
	}
	if(gfLoadError || gfSaveError)//dnl ch37 200909
	{
		MarkWorldDirty();
		RenderWorld();
		fEnteringLoadSaveScreen = TRUE;
		return(gfMessageBoxResult ? LOADSAVE_SCREEN : EDIT_SCREEN);
	}
	if( gfReadOnly )
	{ //file is readonly.  Result will determine if the file dialog stays up.
		fEnteringLoadSaveScreen = TRUE;
		RemoveFileDialog();
		return gfMessageBoxResult ? LOADSAVE_SCREEN : EDIT_SCREEN;
	}
	if( gfFileExists )
	{
		if( gfMessageBoxResult )
		{ //okay to overwrite file
			RemoveFileDialog();
			gbCurrentFileIOStatus = INITIATE_MAP_SAVE;
			return LOADSAVE_SCREEN;
		}
		fEnteringLoadSaveScreen = TRUE;
		RemoveFileDialog();
		return(LOADSAVE_SCREEN);//dnl ch36 210909
	}
//	Assert( 0 );
	return LOADSAVE_SCREEN;
}

UINT32 LoadSaveScreenHandle(void)
{
	FDLG_LIST *FListNode;
	INT32 x;
	InputAtom DialogEvent;

	if( fEnteringLoadSaveScreen )
	{
		LoadSaveScreenEntry();
	}

	if( gbCurrentFileIOStatus ) //loading or saving map
	{
		UINT32 uiScreen;
		uiScreen = ProcessFileIO();
		if( uiScreen == EDIT_SCREEN && gbCurrentFileIOStatus == LOADING_MAP )
			RemoveProgressBar( 0 );
		return uiScreen;
	}

	if( gubMessageBoxStatus )
	{
		if( MessageBoxHandled() )
			return ProcessLoadSaveScreenMessageBoxResult();
		return LOADSAVE_SCREEN;
	}

	//handle all key input.
	while( DequeueEvent(&DialogEvent) )
	{
		if( !HandleTextInput(&DialogEvent) && (DialogEvent.usEvent == KEY_DOWN || DialogEvent.usEvent == KEY_REPEAT) )
		{
			HandleMainKeyEvents( &DialogEvent );
		}
	}

	HandleMouseWheelEvents();//dnl ch36 150909

	DrawFileDialog();

	// Skip to first filename to show
	FListNode = FileList;
	for(x=0;x<iTopFileShown && x<iTotalFiles && FListNode != NULL;x++)
	{
		FListNode = FListNode->pNext;
	}

	// Show up to 8 filenames in the window
	SetFont( FONT12POINT1 );
	if( gfNoFiles )
	{
		SetFontForeground( FONT_LTRED );
		SetFontBackground( 142 );
	  mprintf( iScreenWidthOffset + 226, iScreenHeightOffset + 126, L"NO FILES IN \\MAPS DIRECTORY" );
	}
	else for(x=iTopFileShown;x<(iTopFileShown+8) && x<iTotalFiles && FListNode != NULL; x++)
	{
		if( !EditingText() && x == iCurrFileShown  )
		{
			SetFontForeground( FONT_GRAY2 );
			SetFontBackground( FONT_METALGRAY );
		}
		else
		{
			SetFontForeground( FONT_BLACK );
			SetFontBackground( 142 );
		}
		mprintf( iScreenWidthOffset + 186,(iScreenHeightOffset + 73+ (x-iTopFileShown)*15 ), L"%S", FListNode->FileInfo.zFileName);
		FListNode = FListNode->pNext;
	}

	RenderAllTextFields();

	InvalidateScreen();

	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	switch( iFDlgState )
	{
		case DIALOG_CANCEL:
			RemoveFileDialog();
			fEnteringLoadSaveScreen = TRUE;
			return EDIT_SCREEN;
		case DIALOG_DELETE:
			sprintf( gszCurrFilename, "MAPS\\%S", gzFilename );
			if( GetFileFirst(gszCurrFilename, &FileInfo) )
			{
				CHAR16 str[40];
				if( FileInfo.uiFileAttribs & (FILE_IS_READONLY|FILE_IS_HIDDEN|FILE_IS_SYSTEM) )
				{
					swprintf( str, L" Delete READ-ONLY file %s? ", gzFilename );
					gfReadOnly = TRUE;
				}
				else
					swprintf( str, L" Delete file %s? ", gzFilename );
				gfDeleteFile = TRUE;
				CreateMessageBox( str );
			}
			return LOADSAVE_SCREEN;
		case DIALOG_SAVE://dnl ch37 230909
		{
			if(!ExtractFilenameFromFields())
			{
				CreateMessageBox(L" Illegal filename.  Try another filename? ");
				gfIllegalName = TRUE;
				iFDlgState = DIALOG_NONE;
				return(LOADSAVE_SCREEN);
			}
			sprintf(gszCurrFilename, "MAPS\\%S", gzFilename);
			gfFileExists = FALSE;
#ifndef USE_VFS
			gfReadOnly = FALSE;
			if(FileExists(gszCurrFilename))
			{
				gfFileExists = TRUE;
				if(GetFileFirst(gszCurrFilename, &FileInfo))
				{
					if(FileInfo.uiFileAttribs & (FILE_IS_READONLY|FILE_IS_DIRECTORY|FILE_IS_HIDDEN|FILE_IS_SYSTEM|FILE_IS_OFFLINE|FILE_IS_TEMPORARY))
						gfReadOnly = TRUE;
					GetFileClose(&FileInfo);
				}
			}
#else
			gfReadOnly = TRUE;
			vfs::CProfileStack* st = getVFS()->getProfileStack();
			vfs::CProfileStack::Iterator it = st->begin();
			while(!it.end())
			{
				vfs::CVirtualProfile* prof = it.value();
				if(prof->cWritable == true)
				{
					gfReadOnly = FALSE;
					vfs::Path const& path = gszCurrFilename;
					vfs::IBaseFile *file = prof->getFile(path);
					if(file)
					{
						gfFileExists = TRUE;
						if(file->implementsWritable() == false)
							gfReadOnly = TRUE;
					}
					break;
				}
				it.next();
			}
#endif
			if(gfReadOnly)
			{
				CreateMessageBox(L" File is read only!  Choose a different name? ");
				return( LOADSAVE_SCREEN);
			}
			else if(gfFileExists)
			{
				CreateMessageBox(L" File exists, Overwrite? ");
				return(LOADSAVE_SCREEN);
			}
			RemoveFileDialog();
			gbCurrentFileIOStatus = INITIATE_MAP_SAVE;
			return(LOADSAVE_SCREEN);
		}
		case DIALOG_LOAD:
			if( !ExtractFilenameFromFields() )
			{
				CreateMessageBox( L" Illegal filename.  Try another filename? " );
				gfIllegalName = TRUE;
				iFDlgState = DIALOG_NONE;
				return LOADSAVE_SCREEN;
			}
			RemoveFileDialog();
			CreateProgressBar( 0, iScreenWidthOffset + 118, iScreenHeightOffset + 183, iScreenWidthOffset + 522, iScreenHeightOffset + 202 );
			DefineProgressBarPanel( 0, 65, 79, 94, iScreenWidthOffset + 100, iScreenHeightOffset + 155, iScreenWidthOffset + 540, iScreenHeightOffset + 235 );
			swprintf( zOrigName, L"Loading map:  %s", gzFilename );
			SetProgressBarTitle( 0, zOrigName, BLOCKFONT2, FONT_RED, FONT_NEARBLACK );
			gbCurrentFileIOStatus = INITIATE_MAP_LOAD;
			return LOADSAVE_SCREEN ;
		default:
			iFDlgState = DIALOG_NONE;
	}
	iFDlgState = DIALOG_NONE;
	return LOADSAVE_SCREEN ;
}


void CreateFileDialog( STR16 zTitle )
{

	iFDlgState = DIALOG_NONE;

	DisableEditorTaskbar();

	MSYS_DefineRegion( &BlanketRegion, 0, 0, gsVIEWPORT_END_X, gsVIEWPORT_END_Y, MSYS_PRIORITY_HIGH - 5, 0, 0, 0 );

	//Okay and cancel buttons
	iFileDlgButtons[0] = CreateTextButton( L"Okay", FONT12POINT1, FONT_BLACK, FONT_BLACK,
		BUTTON_USE_DEFAULT, iScreenWidthOffset + 354, iScreenHeightOffset + 225, 50, 30, BUTTON_NO_TOGGLE,	MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, FDlgOkCallback );
	iFileDlgButtons[1] = CreateTextButton( L"Cancel", FONT12POINT1, FONT_BLACK, FONT_BLACK,
		BUTTON_USE_DEFAULT, iScreenWidthOffset + 406, iScreenHeightOffset + 225, 50, 30, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, FDlgCancelCallback );

	//dnl ch36 150909 Scroll buttons
	iFileDlgButtons[2] = CreateSimpleButton(iScreenWidthOffset+426, iScreenHeightOffset+92-19, "EDITOR//uparrow.sti", BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, FDlgUpCallback);
	iFileDlgButtons[3] = CreateSimpleButton(iScreenWidthOffset+426, iScreenHeightOffset+182-19, "EDITOR//downarrow.sti", BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, FDlgDwnCallback);

	//File list window
	iFileDlgButtons[4] = CreateHotSpot( (iScreenWidthOffset + 179+4), (iScreenHeightOffset + 69+3), (179+4+240), (69+120+3), MSYS_PRIORITY_HIGH-1, BUTTON_NO_CALLBACK, FDlgNamesCallback);	
	//Title button
	iFileDlgButtons[5] = CreateTextButton(zTitle, HUGEFONT, FONT_LTKHAKI, FONT_DKKHAKI,
		BUTTON_USE_DEFAULT,iScreenWidthOffset + 179, iScreenHeightOffset + 39,281,30,BUTTON_NO_TOGGLE,
		MSYS_PRIORITY_HIGH-2,BUTTON_NO_CALLBACK,BUTTON_NO_CALLBACK);
	DisableButton(iFileDlgButtons[5]);
	SpecifyDisabledButtonStyle( iFileDlgButtons[5], DISABLED_STYLE_NONE );

	iFileDlgButtons[6] = -1;
	if( iCurrentAction == ACTION_SAVE_MAP )
	{	//checkboxes
		//The update world info checkbox
		iFileDlgButtons[6] = CreateCheckBoxButton( iScreenWidthOffset + 183, iScreenHeightOffset + 229, "EDITOR//smcheckbox.sti", MSYS_PRIORITY_HIGH, UpdateWorldInfoCallback );
		if( gfUpdateSummaryInfo )
			ButtonList[ iFileDlgButtons[6] ]->uiFlags |= BUTTON_CLICKED_ON;
	}

	//Add the text input fields
	InitTextInputModeWithScheme( DEFAULT_SCHEME );
	//dnl ch36 150909 field 1 (filename)
	AddTextInputField(iScreenWidthOffset+183, iScreenHeightOffset+195, 240, 20, MSYS_PRIORITY_HIGH, gzFilename, (FILENAME_BUFLEN-1), INPUTTYPE_EXCLUSIVE_DOSFILENAME);
	//field 2 -- user field that allows mouse/key interaction with the filename list
	AddUserInputField( FileDialogModeCallback );

}

void UpdateWorldInfoCallback( GUI_BUTTON *b, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
		gfUpdateSummaryInfo = b->uiFlags & BUTTON_CLICKED_ON ? TRUE : FALSE;
}

//This is a hook into the text input code.  This callback is called whenever the user is currently
//editing text, and presses Tab to transfer to the file dialog mode.  When this happens, we set the text
//field to the currently selected file in the list which is already know.
void FileDialogModeCallback( UINT8 ubID, BOOLEAN fEntering )
{
	INT32 x;
	FDLG_LIST *FListNode;
	if( fEntering )
	{
		// Skip to first filename
		FListNode = FileList;
		for(x=0;x<iTopFileShown && x<iTotalFiles && FListNode != NULL;x++)
		{
			FListNode = FListNode->pNext;
		}
		// Find the already selected filename
		for(x = iTopFileShown; x < iTopFileShown + 8 && x < iTotalFiles && FListNode != NULL; x++ )
		{
			if( iCurrFileShown == (x-iTopFileShown) )
			{
				FListNode->FileInfo.zFileName[30] = 0;
				SetInputFieldStringWith8BitString( 0, FListNode->FileInfo.zFileName );
				return;
			}
			FListNode = FListNode->pNext;
		}
	}
}

void RemoveFileDialog(void)
{
	INT32 x;

	MSYS_RemoveRegion( &BlanketRegion );

	for(x=0; x<6; x++)
	{
		RemoveButton(iFileDlgButtons[x]);
	}

	if( iFileDlgButtons[6] != -1 )
	{
		RemoveButton( iFileDlgButtons[6] );
	}

	TrashFDlgList( FileList );

	FileList = NULL;

	InvalidateScreen( );

	EnableEditorTaskbar();
	KillTextInputMode();
	MarkWorldDirty();
	RenderWorld();
	EndFrameBufferRender();
}

void DrawFileDialog(void)
{
	//dnl ch36 150909
	ColorFillVideoSurfaceArea(FRAME_BUFFER,	iScreenWidthOffset+179, iScreenHeightOffset+69, iScreenWidthOffset+179+281, iScreenHeightOffset+261, Get16BPPColor(FROMRGB(136, 138, 135)));
	ColorFillVideoSurfaceArea(FRAME_BUFFER,	iScreenWidthOffset+180, iScreenHeightOffset+70, iScreenWidthOffset+179+281, iScreenHeightOffset+261, Get16BPPColor(FROMRGB(24, 61, 81)));
	ColorFillVideoSurfaceArea(FRAME_BUFFER,	iScreenWidthOffset+180, iScreenHeightOffset+70, iScreenWidthOffset+179+280, iScreenHeightOffset+260, Get16BPPColor(FROMRGB(65, 79, 94)));
	ColorFillVideoSurfaceArea(FRAME_BUFFER, iScreenWidthOffset+179+4, iScreenHeightOffset+69+3, iScreenWidthOffset+179+4+240, iScreenHeightOffset+69+123, Get16BPPColor(FROMRGB(24, 61, 81)));
	ColorFillVideoSurfaceArea(FRAME_BUFFER, iScreenWidthOffset+179+5, iScreenHeightOffset+69+4, iScreenWidthOffset+179+4+240, iScreenHeightOffset+69+123, Get16BPPColor(FROMRGB(136, 138, 135)));
	ColorFillVideoSurfaceArea(FRAME_BUFFER, iScreenWidthOffset+179+5, iScreenHeightOffset+69+4, iScreenWidthOffset+179+3+240, iScreenHeightOffset+69+122, Get16BPPColor(FROMRGB(250, 240, 188)));

	MarkButtonsDirty();
	RenderButtons();
	RenderButtonsFastHelp();

	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_LTKHAKI );
	SetFontShadow( FONT_DKKHAKI );
	SetFontBackground( FONT_BLACK );
	mprintf( iScreenWidthOffset + 183, iScreenHeightOffset + 217, L"Filename" );
	
	if( iFileDlgButtons[6] != -1 ) 
	{
		mprintf( iScreenWidthOffset + 200, iScreenHeightOffset + 231, L"Update world info" );
	}
}

//The callback calls this function passing the relative y position of where
//the user clicked on the hot spot.
void SelectFileDialogYPos( UINT16 usRelativeYPos )
{
	INT16 sSelName;
	INT32 x;
	FDLG_LIST *FListNode;

	sSelName = usRelativeYPos / 15;
	
	//This is a field in the text editmode, but clicked via mouse.
	SetActiveField( 1 );

	// Skip to first filename
	FListNode = FileList;
	for(x=0;x<iTopFileShown && x<iTotalFiles && FListNode != NULL;x++)
	{
		FListNode = FListNode->pNext;
	}
	
	for(x=iTopFileShown;x<(iTopFileShown+8) && x<iTotalFiles && FListNode != NULL; x++)
	{
		if( (INT32)sSelName == (x-iTopFileShown) )
		{
			INT32 iCurrClickTime;
			iCurrFileShown = x;
			FListNode->FileInfo.zFileName[30] = 0;
			swprintf( gzFilename, L"%S", FListNode->FileInfo.zFileName );
			if( ValidFilename() )
			{
				SetInputFieldStringWith16BitString( 0, gzFilename );
			}
			else
			{
				SetInputFieldStringWith16BitString( 0, L"" );
				wcscpy( gzFilename, L"" );
			}

			RenderInactiveTextField( 0 );

			//Calculate and process any double clicking...
			iCurrClickTime = GetJA2Clock();
			if( iCurrClickTime - iLastClickTime < 400 && x == iLastFileClicked )
			{ //Considered a double click, so activate load/save this filename.
				gfDestroyFDlg = TRUE;
				iFDlgState = iCurrentAction == ACTION_SAVE_MAP ? DIALOG_SAVE : DIALOG_LOAD;
			}
			iLastClickTime = iCurrClickTime;
			iLastFileClicked = x;
		}
		FListNode = FListNode->pNext;
	}
}

FDLG_LIST *AddToFDlgList(FDLG_LIST *pList, GETFILESTRUCT *pInfo)
{
	FDLG_LIST *pNode;

	// Add to start of list
	if ( pList == NULL )
	{
		pNode = (FDLG_LIST *)MemAlloc( sizeof(FDLG_LIST) );
		pNode->FileInfo = *pInfo;
		pNode->pPrev = pNode->pNext = NULL;
		return(pNode);
	}

	// Add and sort alphabetically without regard to case -- function limited to 10 chars comparison
	if ( _stricmp(pList->FileInfo.zFileName, pInfo->zFileName ) > 0 )
	{
		// pInfo is smaller than pList (i.e. Insert before)
		pNode = (FDLG_LIST *)MemAlloc( sizeof(FDLG_LIST) );
		pNode->FileInfo = *pInfo;
		pNode->pNext = pList;
		pNode->pPrev = pList->pPrev;
		pList->pPrev = pNode;
		return(pNode);
	}
	else
	{
		pList->pNext = AddToFDlgList( pList->pNext, pInfo );
		pList->pNext->pPrev = pList;
	}
	return(pList);
}

BOOLEAN RemoveFromFDlgList( FDLG_LIST **head, FDLG_LIST *node )
{
	FDLG_LIST *curr;
	curr = *head;
	while( curr )
	{
		if( curr == node )
		{
			if( *head == node )
				*head = (*head)->pNext;
			if( curr->pPrev )
				curr->pPrev->pNext = curr->pNext;
			if( curr->pNext )
				curr->pNext->pPrev = curr->pPrev;
			MemFree( node );
			node = NULL;
			return TRUE;
		}
		curr = curr->pNext;
	}
	return FALSE; //wasn't deleted 
}

void TrashFDlgList(FDLG_LIST *pList)
{
	FDLG_LIST *pNode;

	while(pList)//dnl ch49 061009 Rewind, just to be sure
	{
		if(pList->pPrev)
			pList = pList->pPrev;
		else
			break;
	}
	while(pList != NULL)
	{
		pNode = pList;
		pList = pList->pNext;
		MemFree(pNode);
	}
}

void SetTopFileToLetter( UINT16 usLetter )
{
	UINT32 x;
	FDLG_LIST *curr;
	FDLG_LIST *prev;
	UINT16 usNodeLetter;

	// Skip to first filename
	x = 0;
	curr = prev = FileList;
	while( curr )
	{
		usNodeLetter = curr->FileInfo.zFileName[0]; //first letter of filename.
		if( usNodeLetter < 'a' )
			usNodeLetter += 32; //convert uppercase to lower case A=65, a=97
		if( usLetter <= usNodeLetter )
			break;
		prev = curr;
		curr = curr->pNext;
		x++;
	}
	if( FileList )	
	{
		iCurrFileShown = x;
		iTopFileShown = x;
		if( iTopFileShown > iTotalFiles - 7 )
			iTopFileShown = iTotalFiles - 7;
		SetInputFieldStringWith8BitString( 0, prev->FileInfo.zFileName );
	}
}

void HandleMainKeyEvents( InputAtom *pEvent )
{
	INT32 iPrevFileShown = iCurrFileShown;
	//Replace Alt-x press with ESC.
	if( pEvent->usKeyState & ALT_DOWN && pEvent->usParam == 'x' )
		pEvent->usParam = ESC;
	switch( pEvent->usParam )
	{
		case ENTER:
			if( gfNoFiles && iCurrentAction == ACTION_LOAD_MAP )
				break;
			gfDestroyFDlg = TRUE;
			iFDlgState = iCurrentAction == ACTION_SAVE_MAP ? DIALOG_SAVE : DIALOG_LOAD;
			break;
		case ESC:
			gfDestroyFDlg = TRUE;
			iFDlgState = DIALOG_CANCEL;
			break;
		case PGUP:
			if( iTopFileShown > 7 )
			{
				iTopFileShown -= 7;
				iCurrFileShown -= 7;
			}
			else
			{
				iTopFileShown = 0;
				iCurrFileShown = 0;
			}
			break;
		case PGDN:
			iTopFileShown += 7;
			iCurrFileShown += 7;
			if( iTopFileShown > iTotalFiles-7 )
				iTopFileShown = iTotalFiles - 7;
			if( iCurrFileShown >= iTotalFiles )
				iCurrFileShown = iTotalFiles - 1;
			break;
		case UPARROW:
			if( iCurrFileShown > 0 )
				iCurrFileShown--;
			if( iTopFileShown > iCurrFileShown )
				iTopFileShown = iCurrFileShown;
			break;
		case DNARROW:
			iCurrFileShown++;
			if( iCurrFileShown >= iTotalFiles )
				iCurrFileShown = iTotalFiles - 1;
			else if( iTopFileShown < iCurrFileShown-7 )
				iTopFileShown++;
			break;
		case HOME:
		case CTRL_HOME:
			iTopFileShown = 0;
			iCurrFileShown = 0;
			break;
		case END:
		case CTRL_END:
			iTopFileShown = iTotalFiles-7;
			iCurrFileShown = iTotalFiles-1;
			break;
		case DEL:
			iFDlgState = DIALOG_DELETE;
			break;
		default:
			//This case handles jumping the file list to display the file with the letter pressed.
			if( pEvent->usParam >= 'a' && pEvent->usParam <= 'z' ||
					pEvent->usParam >= 'A' && pEvent->usParam <= 'Z' )
			{
				if( pEvent->usParam >= 'A' && pEvent->usParam <= 'Z' ) //convert upper case to lower case
					pEvent->usParam += 32;	// A = 65, a = 97 (difference of 32)
				SetTopFileToLetter( (UINT16)pEvent->usParam );
			}
			break;
	}
	//Update the text field if the file value has changed.
	if( iCurrFileShown != iPrevFileShown )
	{
		INT32 x;
		FDLG_LIST *curr;
		x = 0;
		curr = FileList;
		while( curr && x != iCurrFileShown )
		{
			curr = curr->pNext;
			x++;
		}
		if( curr )	
		{
			SetInputFieldStringWith8BitString( 0, curr->FileInfo.zFileName );
			swprintf( gzFilename, L"%S", curr->FileInfo.zFileName );
		}
	}
}

void HandleMouseWheelEvents(void)//dnl ch36 150909
{
	if(_WheelValue > 0)
	{
		while(_WheelValue--)
		{
			if(iTopFileShown > 0)
				iTopFileShown--;
		}
	}
	else if(_WheelValue < 0)
	{
		while(_WheelValue++)
		{
			if((iTopFileShown+7) < iTotalFiles)
				iTopFileShown++;
		}
	}
	_WheelValue = 0;
}

void InitErrorCatchDialog()
{
	SGPRect CenteringRect= {0 + xResOffset, 0, SCREEN_WIDTH - xResOffset, SCREEN_HEIGHT };

	// do message box and return
	giErrorCatchMessageBox = DoMessageBox( MSG_BOX_BASIC_STYLE, gzErrorCatchString, 
											EDIT_SCREEN, MSG_BOX_FLAG_OK, NULL, &CenteringRect );
	gfErrorCatch = FALSE;
}

extern BOOLEAN ReEvaluateWorld( const STR8	puiFilename );

//Because loading and saving the map takes a few seconds, we want to post a message
//on the screen and then update it which requires passing the screen back to the main loop.
//When we come back for the next frame, we then actually save or load the map.  So this
//process takes two full screen cycles.
UINT32 ProcessFileIO()
{
	INT16 usStartX, usStartY;
	CHAR8 ubNewFilename[50];
	BOOLEAN fAltMap;//dnl ch31 150909
	switch( gbCurrentFileIOStatus )
	{
		case INITIATE_MAP_SAVE:	//draw save message 
			StartFrameBufferRender( );
			SaveFontSettings();
			SetFont( HUGEFONT );
			SetFontForeground( FONT_LTKHAKI );
			SetFontShadow( FONT_DKKHAKI );
			SetFontBackground( 0 );
			swprintf( zOrigName, L"Saving map:  %s", gzFilename );
			usStartX = iScreenWidthOffset + 320 - StringPixLength( zOrigName, LARGEFONT1 ) / 2;
			usStartY = iScreenHeightOffset + 180 - GetFontHeight( LARGEFONT1 ) / 2;
			mprintf( usStartX, usStartY, zOrigName );

			InvalidateScreen( );
			EndFrameBufferRender( );
			gbCurrentFileIOStatus = SAVING_MAP;
			return LOADSAVE_SCREEN;
		case SAVING_MAP: //save map
			sprintf( ubNewFilename, "%S", gzFilename );
			RaiseWorldLand();
			if( gfShowPits )
				RemoveAllPits();
			OptimizeSchedules();
			ShowHighGround(4);//dnl ch41 210909
			//dnl ch33 091009
			BOOLEAN fRet;
			if(gfVanillaMode && iNewMapWorldRows == OLD_WORLD_ROWS && iNewMapWorldCols == OLD_WORLD_COLS)
				fRet = SaveWorld(ubNewFilename, VANILLA_MAJOR_MAP_VERSION, VANILLA_MINOR_MAP_VERSION);
			else
				fRet = SaveWorld(ubNewFilename);
			if(!fRet)
			{
				//dnl ch37 150909
				gfSaveError = TRUE;
				if(gfErrorCatch)
				{
					InitErrorCatchDialog();
					return(EDIT_SCREEN);
				}
				gbCurrentFileIOStatus = IOSTATUS_NONE;
				CreateMessageBox((STR16)(_BS(L" Error saving ") << (const char*)ubNewFilename << L" file. Try another filename? " << _BS::wget).c_str() );
				return(guiCurrentScreen);
			}
			if( gfShowPits )
				AddAllPits();
			GetSectorFromFileName(gzFilename, gWorldSectorX, gWorldSectorY, gbWorldSectorZ, fAltMap);//dnl ch31 140909
			if( gfGlobalSummaryExists )
				UpdateSectorSummary( gzFilename, gfUpdateSummaryInfo );
			else//dnl ch30 150909
				ReEvaluateWorld(ubNewFilename);
			iCurrentAction = ACTION_NULL;
			gbCurrentFileIOStatus = IOSTATUS_NONE;
			gfRenderWorld = TRUE;
			gfRenderTaskbar = TRUE;
			fEnteringLoadSaveScreen = TRUE;
			RestoreFontSettings();
			if( gfErrorCatch )
			{
				InitErrorCatchDialog();
				return EDIT_SCREEN;
			}

			fNewMapSaved = TRUE;

			return EDIT_SCREEN;
		case INITIATE_MAP_LOAD: //draw load message
			SaveFontSettings();
			gbCurrentFileIOStatus = LOADING_MAP;
			if( gfEditMode && iCurrentTaskbar == TASK_MERCS )
				IndicateSelectedMerc( SELECT_NO_MERC );
			SpecifyItemToEdit( NULL, -1 );
			return LOADSAVE_SCREEN;
		case LOADING_MAP: //load map
			DisableUndo();
			sprintf( ubNewFilename, "%S", gzFilename );
			
			RemoveMercsInSector( );

			 // Want to override crash, so user can do something else.
			if(!ReEvaluateWorld(ubNewFilename) || !LoadWorld(ubNewFilename))//dnl ch36 140909
			{
				EnableUndo();
				gbCurrentFileIOStatus = IOSTATUS_NONE;
				gfGlobalError = FALSE;
				gfLoadError = TRUE;
				CreateMessageBox((STR16)(_BS(L" Error loading ") << (const char*)ubNewFilename << L" file. Try another filename? " << _BS::wget).c_str());
				return(guiCurrentScreen);
			}
			//ADB these are NOT set yet! but they need to be, duh
			CompileWorldMovementCosts();
			GetSectorFromFileName(gzFilename, gWorldSectorX, gWorldSectorY, gbWorldSectorZ, fAltMap);//dnl ch31 140909
			RestoreFontSettings();

			//Load successful, update necessary information.

			//ATE: Any current mercs are transfered here...
			//UpdateMercsInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

			AddSoldierInitListTeamToWorld( ENEMY_TEAM,		255 );
			AddSoldierInitListTeamToWorld( CREATURE_TEAM,	255 );
			AddSoldierInitListTeamToWorld( MILITIA_TEAM,	255 );
			AddSoldierInitListTeamToWorld( CIV_TEAM,		255 );
			iCurrentAction = ACTION_NULL;
			gbCurrentFileIOStatus = IOSTATUS_NONE;
			if( !gfCaves && !gfBasement )
			{
				gusLightLevel = 12;
				if( ubAmbientLightLevel != 4 )
				{
					ubAmbientLightLevel = 4;
					LightSetBaseLevel( ubAmbientLightLevel );
				}
			}
			else
				gusLightLevel = (UINT16)(EDITOR_LIGHT_MAX - ubAmbientLightLevel );
			gEditorLightColor = gpLightColors[ 0 ];
			gfRenderWorld = TRUE;
			gfRenderTaskbar = TRUE;
			fEnteringLoadSaveScreen = TRUE;
			InitJA2SelectionWindow();
			ShowEntryPoints();
			EnableUndo();
			RemoveAllFromUndoList();
			SetEditorSmoothingMode( gMapInformation.ubEditorSmoothingType );
			if( gMapInformation.ubEditorSmoothingType == SMOOTHING_CAVES )
				AnalyseCaveMapForStructureInfo();

			AddLockedDoorCursors();
			gubCurrRoomNumber = gubMaxRoomNumber;
			UpdateRoofsView();
			UpdateWallsView();
			ShowLightPositionHandles();
			SetMercTeamVisibility( ENEMY_TEAM, gfShowEnemies );
			SetMercTeamVisibility( CREATURE_TEAM, gfShowCreatures );
			SetMercTeamVisibility( MILITIA_TEAM, gfShowRebels );
			SetMercTeamVisibility( CIV_TEAM, gfShowCivilians );
			BuildItemPoolList();
			gpItemPool = NULL;//dnl ch26 210909
			fShowHighGround = FALSE;//dnl ch2 210909
			fRaiseWorld = FALSE;//dnl ch3 210909
			ShowHighGround(4);//dnl ch41 210909
			SetRenderCenter(WORLD_COLS/2, WORLD_ROWS/2);//dnl ch43 280909
			if( gfShowPits )
				AddAllPits();

			if( iCurrentTaskbar == TASK_MAPINFO )
			{ //We have to temporarily remove the current textinput mode,
				//update the disabled text field values, then restore the current
				//text input fields.
				SaveAndRemoveCurrentTextInputMode();
				UpdateMapInfoFields();
				RestoreSavedTextInputMode();
			}
			return EDIT_SCREEN;
	}
	gbCurrentFileIOStatus = IOSTATUS_NONE;
	return LOADSAVE_SCREEN;
}

//LOADSCREEN
void FDlgNamesCallback( GUI_BUTTON *butn, INT32 reason )
{
	if( reason & (MSYS_CALLBACK_REASON_LBUTTON_UP) )
	{
		SelectFileDialogYPos( butn->Area.RelativeYPos );
	}
}


void FDlgOkCallback( GUI_BUTTON *butn, INT32 reason )
{
	if( reason & (MSYS_CALLBACK_REASON_LBUTTON_UP) )
	{
		gfDestroyFDlg = TRUE;
		iFDlgState = iCurrentAction == ACTION_SAVE_MAP ? DIALOG_SAVE : DIALOG_LOAD;	
	}
}

void FDlgCancelCallback( GUI_BUTTON *butn, INT32 reason )
{
	if( reason & (MSYS_CALLBACK_REASON_LBUTTON_UP) )
	{
		gfDestroyFDlg = TRUE;
		iFDlgState = DIALOG_CANCEL;	
	}
}

void FDlgUpCallback( GUI_BUTTON *butn, INT32 reason )
{
	if( reason & (MSYS_CALLBACK_REASON_LBUTTON_UP) )
	{
		if(iTopFileShown > 0)
			iTopFileShown--;
	}
}

void FDlgDwnCallback( GUI_BUTTON *butn, INT32 reason )
{
	if( reason & (MSYS_CALLBACK_REASON_LBUTTON_UP) )
	{
		if( (iTopFileShown+7) < iTotalFiles )
			iTopFileShown++;
	}
}

//dnl ch36 200909
BOOLEAN ExtractFilenameFromFields(void)
{
	Get16BitStringFromField(0, gzFilename, FILENAME_BUFLEN);
	size_t len = wcslen(gzFilename);
	if(gzFilename[len-4] != L'.' && len < (FILENAME_BUFLEN-4))
		wcscat(gzFilename, L".dat");
	return(ValidFilename());
}

//dnl ch31 140909
BOOLEAN ValidMapFileName(STR16 szFileName)
{
	CHAR16 szFileExt[4+1];
	size_t len = wcslen(szFileName);
	if(len >= 5)
	{
		wcscpy(szFileExt, szFileName+len-4);
		for(int i=0; i<4; i++)
			szFileExt[i] = towupper(szFileExt[i]);
		if(wcscmp(szFileExt, L".DAT") == 0)
			return(TRUE);
	}
	return(FALSE);
}

//dnl ch37 200909
BOOLEAN ExternalLoadMap(STR16 szFilename)
{
	gfLoadError = FALSE;
	Assert(szFilename);
	if(!wcslen( szFilename))
		return(FALSE);
	wcscpy(gzFilename, szFilename);
	if(!ValidFilename())
		return(FALSE);
	gbCurrentFileIOStatus = INITIATE_MAP_LOAD;
	ProcessFileIO();
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();
	RefreshScreen(NULL);
	ProcessFileIO();
	if(gfLoadError)
		return(FALSE);
	return(TRUE);
}

BOOLEAN ExternalSaveMap(STR16 szFilename)
{
	gfSaveError = FALSE;
	Assert(szFilename);
	if(!wcslen(szFilename))
		return(FALSE);
	wcscpy(gzFilename, szFilename);
	if(!ValidFilename())
		return(FALSE);
	gbCurrentFileIOStatus = INITIATE_MAP_SAVE;
	ProcessFileIO();
	if(gfSaveError)
		return(FALSE);
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();
	RefreshScreen(NULL);
	ProcessFileIO();
	if(gfSaveError)
		return(FALSE);
	return(TRUE);
}

#else //non-editor version

#include "types.h"
#include "screenids.h"

UINT32 LoadSaveScreenInit()
{
	return TRUE ;
}

UINT32	LoadSaveScreenHandle( )
{
	//If this screen ever gets set, then this is a bad thing -- endless loop
	return ERROR_SCREEN ;
}

UINT32 LoadSaveScreenShutdown( )
{
	return TRUE ;
}

#endif




 


