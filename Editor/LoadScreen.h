#include "BuildDefines.h"
#include "Fileman.h"

#define FILENAME_BUFLEN (30 + 4 + 1)//dnl ch39 190909 +4 is for ".dat", +1 is for '\0' //dnl ch81 021213

#ifdef JA2EDITOR

#ifndef _JA2_LOAD_SCREEN_
#define _JA2_LOAD_SCREEN_

typedef struct _FDLG_LIST_TAG
{
	GETFILESTRUCT FileInfo;
	struct _FDLG_LIST_TAG	*pNext;
	struct _FDLG_LIST_TAG *pPrev;
} FDLG_LIST;

extern FDLG_LIST *AddToFDlgList(FDLG_LIST *pList, GETFILESTRUCT *pInfo);

extern void TrashFDlgList(FDLG_LIST *pList);

extern INT32 iFDlgState;
extern INT32 iFDlgSelectedSlot;

extern BOOLEAN gfAskForName;
extern BOOLEAN gfCreatedFDlg;
extern BOOLEAN gfDestroyFDlg;

UINT32 WaitForFileName( void );
void RemoveFileDialog(void);
void CreateFileDialog( STR16 zTitle );

void SelectFileDialogYPos( UINT16 usRelativeYPos );


void BuildFilenameWithCoordinate();
void BuildCoordinateWithFilename();
//dnl ch36 200909
BOOLEAN ExtractFilenameFromFields(void);
//dnl ch31 150909
#define ValidFilename() ValidMapFileName(gzFilename)
BOOLEAN ValidMapFileName(STR16 szFilename);
//dnl ch37 200909
BOOLEAN ExternalLoadMap(STR16 szFilename);
BOOLEAN ExternalSaveMap(STR16 szFilename);

void InitErrorCatchDialog();

extern BOOLEAN gfErrorCatch;
extern CHAR16 gzErrorCatchString[ 256 ];

//dnl ch81 031213
extern CHAR16 gzFilename[FILENAME_BUFLEN];
extern CHAR8 gzProfileName[FILENAME_BUFLEN];

#endif
#endif
