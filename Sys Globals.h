#ifndef __SYS_GLOBALS_H
#define __SYS_GLOBALS_H

#define			SHOW_MIN_FPS			0
#define			SHOW_FULL_FPS			1

extern UINT8	gubErrorText[ 200 ];
extern BOOLEAN	gfAniEditMode;
extern BOOLEAN	gfEditMode;
extern BOOLEAN	fFirstTimeInGameScreen;
extern BOOLEAN	fDirtyRectangleMode;
extern INT8		gDebugStr[128];
extern INT8		gSystemDebugStr[128];

extern BOOLEAN	gfMode;
extern INT16	gsCurrentActionPoints;
extern INT8		gbFPSDisplay;
extern BOOLEAN	gfResetInputCheck;
extern BOOLEAN	gfGlobalError;

extern UINT32	guiGameCycleCounter;


// VIDEO OVERLAYS 
extern INT32	giFPSOverlay;
extern INT32	giCounterPeriodOverlay;

extern	BOOLEAN  SET_ERROR( const char *String, ...);


//inline size_t strlen(INT8 *str) { return strlen((const char *)str); }
template <typename type1>
inline size_t strlen(type1 str) { return strlen((const char *)str); }

template <typename type1, typename type2>
inline char *strncat(type1 str1, type2 str2, size_t n)
{ return strncat((char *)str1, (const char *)str2, n); }

/*
inline char *strncat(INT8 *str1, INT8 *str2, size_t n) { return strncat((char *)str1, (const char *)str2, n); }
inline char *strncat(char *str1, INT8 *str2, size_t n) { return strncat(str1, (const char *)str2, n); }
inline char *strncat(INT8 *str1, const char *str2, size_t n) { return strncat((char *)str1, str2, n); }
*/
//inline long atol(INT8 *str) { return atol((const char *)str); }
template <typename type1>
inline long atol(type1 str) { return atol((const char *)str); }

template <typename type1, typename type2>
inline char *strncpy(type1 str1, type2 str2, size_t n) { return strncpy((char *)str1, (const char *)str2, n); }

template <typename type1, typename type2>
inline char *strcpy(type1 str1, type2 str2) { return strcpy((char *)str1, (const char *)str2); }

template <typename string1, typename string2>
inline size_t strcspn(string1 str1, string2 str2) { return strcspn((const char *)str1, (const char *)str2); }

template <typename type1, typename type2>
inline wchar_t *wcscpy(type1 str1, type2 str2) { return wcscpy((wchar_t *)str1, (const wchar_t *) str2); }

template <typename string1, typename string2>
inline wchar_t *wcscat(string1 str1, string2 str2) { return wcscat((wchar_t *)str1, (const wchar_t *)str2); }

template <typename string1, typename string2>
inline char *strcat(string1 str1, string2 str2) { return strcat((char *)str1, (const char *)str2); }

template <typename string1, typename string2>
inline int _stricmp(string1 str1, string2 str2) { return _stricmp((const char *)str1, (const char *) str2); }

template <typename string1>
inline char *strrchr(string1 str1, int n) { return strrchr((const char *)str1, n); }

template <typename string1, typename string2>
inline int stricmp(string1 str1, string2 str2) { return stricmp((const char *)str1, (const char *)str2); }

template <typename string1, typename string2>
inline int strcmp(string1 str1, string2 str2) { return strcmp((const char *)str1, (const char *) str2); }

template <typename string1, typename string2>
inline char *strstr(string1 str1, string2 str2) { return strstr((const char *)str1, (const char *)str2); }

#endif
