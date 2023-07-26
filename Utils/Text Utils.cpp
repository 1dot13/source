	#include "text.h"
	#include "Fileman.h"
	#include "GameSettings.h"
	// sevenfm
	#include <codecvt>
	#include <string>

BOOLEAN LoadItemInfo(UINT16 ubIndex, STR16 pNameString, STR16 pInfoString )
{
	int j = 0;

	if (pNameString != NULL)
	{
		wcsncpy( pNameString, Item[ubIndex].szLongItemName, 80);
		pNameString[79] ='\0';
	}

	if(pInfoString != NULL)
	{
		wcsncpy( pInfoString, Item[ubIndex].szItemDesc, 400);
		pInfoString[399] ='\0';
	}

	return(TRUE);
}

BOOLEAN LoadBRName(UINT16 ubIndex, STR16 pNameString )
{
	if (pNameString != NULL)
	{
		wcsncpy( pNameString, Item[ubIndex].szBRName, 80);
		pNameString[79] ='\0';
	}
	return TRUE;
}

BOOLEAN LoadBRDesc(UINT16 ubIndex, STR16 pDescString )
{
	if (pDescString != NULL)
	{
		wcsncpy( pDescString, Item[ubIndex].szBRDesc, 400);
		pDescString[399] ='\0';
	}

	return TRUE;
}

BOOLEAN LoadShortNameItemInfo(UINT16 ubIndex, STR16 pNameString )
{
	if(pNameString != NULL)
	{
		wcsncpy( pNameString, Item[ubIndex].szItemName, 80 );
		pNameString[79] ='\0';
	}

	return(TRUE);
}


void LoadAllItemNames( void )
{
	for ( UINT16 usLoop = 0; usLoop < gMAXITEMS_READ; ++usLoop )
	{
		LoadItemInfo( usLoop, ItemNames[usLoop], NULL );

		// Load short item info
		LoadShortNameItemInfo( usLoop, ShortItemNames[usLoop] );
	}
}

void LoadAllExternalText( void )
{
	LoadAllItemNames();
}

STR16 GetWeightUnitString( void )
{
	if ( gGameSettings.fOptions[ TOPTION_USE_METRIC_SYSTEM ] ) // metric
	{
		return( pMessageStrings[ MSG_KILOGRAM_ABBREVIATION ] );
	}
	else
	{
		return( pMessageStrings[ MSG_POUND_ABBREVIATION ] );
	}
}

FLOAT GetWeightBasedOnMetricOption( UINT32 uiObjectWeight )
{
	FLOAT fWeight = 0.0f;

	//if the user is smart and wants things displayed in 'metric'
	if ( gGameSettings.fOptions[ TOPTION_USE_METRIC_SYSTEM ] ) // metric
	{
		fWeight = (FLOAT)uiObjectWeight;
	}

	//else the user is a caveman and display it in pounds
	else
	{
		fWeight = uiObjectWeight * 2.2f;
	}

	return( fWeight );
}

static inline STR8 Trim(STR8 &p) { 
	while(isspace(*p)) *p++ = 0; 
	STR8 e = p + strlen(p) - 1;
	while (e > p && isspace(*e)) *e-- = 0;
	return p;
}

static inline STR16 Trim(STR16 &p) { 
	while(iswspace(*p)) *p++ = 0; 
	STR16 e = p + wcslen(p) - 1;
	while (e > p && iswspace(*e)) *e-- = 0;
	return p;
}


int StringToEnum(const STR value, const Str8EnumLookupType *table) 
{
	if (NULL == value || 0 == *value) 
		return 0;

	for (const Str8EnumLookupType *itr = table; itr->name != NULL; ++itr) {
		if (0 == _stricmp(value, itr->name)) 
			return itr->value;
	}
	STR end = NULL;
	return (int)strtol(value, &end, 0);
}

int StringToEnum(const STR8 value, const Str16EnumLookupType *table) 
{
	if (NULL == value || 0 == *value) 
		return 0;

	int result = 0;
	int len = strlen(value)+1;
	STR16 wval = (STR16)malloc( len*sizeof(CHAR16) );
	mbstowcs(wval, value, len);
	for (const Str16EnumLookupType *itr = table; itr->name != NULL; ++itr) {
		if (0 == _wcsicmp(wval, itr->name)) {
			result = itr->value;
		}
	}
	free(wval);
	return (result) ? result : (int)strtol(value, NULL, 0);
}

int StringToEnum(const STR16 value, const Str8EnumLookupType *table) 
{
	if (NULL == value || 0 == *value) 
		return 0;

	int result = 0;
	int len = wcslen(value)+1;
	STR8 mval = (STR8)malloc( len*sizeof(CHAR8) );
	wcstombs(mval, value, len);
	for (const Str8EnumLookupType *itr = table; itr->name != NULL; ++itr) {
		if (0 == _stricmp(mval, itr->name)) {
			result = itr->value;
		}
	}
	free(mval);
	return (result) ? result : (int)wcstol(value, NULL, 0);
}

int StringToEnum(const STR16 value, const Str16EnumLookupType *table) {
	if (NULL == value || 0 == *value) 
		return 0;

	for (const Str16EnumLookupType *itr = table; itr->name != NULL; ++itr) {
		if (0 == _wcsicmp(value, itr->name)) 
			return itr->value;
	}
	return (int)wcstol(value, NULL, 0);
}



// routine for parsing white space separated lines.  Handled like command line parameters w.r.t quotes.
void ParseCommandLine (
                        const char *start,
                        char **argv,
                        char *args,
                        int *numargs,
                        int *numchars
                        )
{
   const char NULCHAR    = '\0';
   const char SPACECHAR  = ' ';
   const char TABCHAR    = '\t';
   const char RETURNCHAR = '\r';
   const char LINEFEEDCHAR = '\n';
   const char DQUOTECHAR = '\"';
   const char SLASHCHAR  = '\\';
   const char *p;
   int inquote;                    /* 1 = inside quotes */
   int copychar;                   /* 1 = copy char to *args */
   unsigned numslash;              /* num of backslashes seen */

   *numchars = 0;
   *numargs = 0;                   /* the program name at least */

   p = start;

   inquote = 0;

   /* loop on each argument */
   for(;;) 
   {
      if ( *p ) { while (*p == SPACECHAR || *p == TABCHAR || *p == RETURNCHAR || *p == LINEFEEDCHAR) ++p; }

      if (*p == NULCHAR) break; /* end of args */

      /* scan an argument */
      if (argv)
         *argv++ = args;     /* store ptr to arg */
      ++*numargs;

      /* loop through scanning one argument */
      for (;;) 
      {
         copychar = 1;
         /* Rules: 2N backslashes + " ==> N backslashes and begin/end quote
         2N+1 backslashes + " ==> N backslashes + literal "
         N backslashes ==> N backslashes */
         numslash = 0;
         while (*p == SLASHCHAR) 
         {
            /* count number of backslashes for use below */
            ++p;
            ++numslash;
         }
         if (*p == DQUOTECHAR) 
         {
            /* if 2N backslashes before, start/end quote, otherwise copy literally */
            if (numslash % 2 == 0) {
               if (inquote) {
                  if (p[1] == DQUOTECHAR)
                     p++;    /* Double quote inside quoted string */
                  else        /* skip first quote char and copy second */
                     copychar = 0;
               } else
                  copychar = 0;       /* don't copy quote */

               inquote = !inquote;
            }
            numslash /= 2;          /* divide numslash by two */
         }

         /* copy slashes */
         while (numslash--) {
            if (args)
               *args++ = SLASHCHAR;
            ++*numchars;
         }

         /* if at end of arg, break loop */
         if (*p == NULCHAR || (!inquote && (*p == SPACECHAR || *p == TABCHAR || *p == RETURNCHAR || *p == LINEFEEDCHAR)))
            break;

         /* copy character into argument */
         if (copychar) 
         {
            if (args)
               *args++ = *p;
            ++*numchars;
         }
         ++p;
      }

      /* null-terminate the argument */
      if (args)
         *args++ = NULCHAR;          /* terminate string */
      ++*numchars;
   }
   /* We put one last argument in -- a null ptr */
   if (argv)
      *argv++ = NULL;
   ++*numargs;
}


// routine for parsing white space separated lines.  Handled like command line parameters w.r.t quotes.
void ParseCommandLine (
                        const wchar_t *start,
                        wchar_t **argv,
                        wchar_t *args,
                        int *numargs,
                        int *numchars
                        )
{
   const wchar_t NULCHAR    = L'\0';
   const wchar_t SPACECHAR  = L' ';
   const wchar_t TABCHAR    = L'\t';
   const wchar_t RETURNCHAR = L'\r';
   const wchar_t LINEFEEDCHAR = L'\n';
   const wchar_t DQUOTECHAR = L'\"';
   const wchar_t SLASHCHAR  = L'\\';
   const wchar_t *p;
   int inquote;                    /* 1 = inside quotes */
   int copychar;                   /* 1 = copy char to *args */
   unsigned numslash;              /* num of backslashes seen */

   *numchars = 0;
   *numargs = 0;                   /* the program name at least */

   p = start;

   inquote = 0;

   /* loop on each argument */
   for(;;) 
   {
      if ( *p ) { while (*p == SPACECHAR || *p == TABCHAR || *p == RETURNCHAR || *p == LINEFEEDCHAR) ++p; }

      if (*p == NULCHAR) break; /* end of args */

      /* scan an argument */
      if (argv)
         *argv++ = args;     /* store ptr to arg */
      ++*numargs;

      /* loop through scanning one argument */
      for (;;) 
      {
         copychar = 1;
         /* Rules: 2N backslashes + " ==> N backslashes and begin/end quote
         2N+1 backslashes + " ==> N backslashes + literal "
         N backslashes ==> N backslashes */
         numslash = 0;
         while (*p == SLASHCHAR) 
         {
            /* count number of backslashes for use below */
            ++p;
            ++numslash;
         }
         if (*p == DQUOTECHAR) 
         {
            /* if 2N backslashes before, start/end quote, otherwise copy literally */
            if (numslash % 2 == 0) {
               if (inquote) {
                  if (p[1] == DQUOTECHAR)
                     p++;    /* Double quote inside quoted string */
                  else        /* skip first quote char and copy second */
                     copychar = 0;
               } else
                  copychar = 0;       /* don't copy quote */

               inquote = !inquote;
            }
            numslash /= 2;          /* divide numslash by two */
         }

         /* copy slashes */
         while (numslash--) {
            if (args)
               *args++ = SLASHCHAR;
            ++*numchars;
         }

         /* if at end of arg, break loop */
         if (*p == NULCHAR || (!inquote && (*p == SPACECHAR || *p == TABCHAR || *p == RETURNCHAR || *p == LINEFEEDCHAR)))
            break;

         /* copy character into argument */
         if (copychar) 
         {
            if (args)
               *args++ = *p;
            ++*numchars;
         }
         ++p;
      }

      /* null-terminate the argument */
      if (args)
         *args++ = NULCHAR;          /* terminate string */
      ++*numchars;
   }
   /* We put one last argument in -- a null ptr */
   if (argv)
      *argv++ = NULL;
   ++*numargs;
}

inline std::string narrow(std::wstring const& text)
{
	std::locale const loc("");
	wchar_t const* from = text.c_str();
	std::size_t const len = text.size();
	std::vector<char> buffer(len + 1);
	std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);

	return std::string(&buffer[0], &buffer[len]);
}

// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
