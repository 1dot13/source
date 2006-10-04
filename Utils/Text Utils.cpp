#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#include "text.h"
	#include "Fileman.h"
#endif

BOOLEAN LoadItemInfo(UINT16 ubIndex, STR16 pNameString, STR16 pInfoString )
{
	if (pNameString != NULL)
	{
		for (int i=0;i<80;i++)
		{
			if ( i<(int)strlen(Item[ubIndex].szLongItemName ))
			{
				pNameString[i] =  Item[ubIndex].szLongItemName  [i];

				// WANNE: German specific characters
				#ifdef GERMAN
				switch( pNameString[ i ] )
				{
					// ü
					case 65532:	pNameString[i] = 252; break;
					// Ü
					case 65500:	pNameString[i] = 220; break;
					// ä
					case 65508:	pNameString[i] = 228; break;
					// Ä
					case 65476:	pNameString[i] = 196; break;
					// ö
					case 65526:	pNameString[i] = 246; break;
					// Ö
					case 65494:	pNameString[i] = 214; break;
					// ß
					case 65503:	pNameString[i] = 223; break;
				}
				#endif

				#ifdef POLISH
				switch( pNameString[ i ] )
				{
					case 260:		pNameString[ i ] = 165;		break;
					case 262:		pNameString[ i ] = 198;		break;
					case 280:		pNameString[ i ] = 202;		break;
					case 321:		pNameString[ i ] = 163;		break;
					case 323:		pNameString[ i ] = 209;		break;
					case 211:		pNameString[ i ] = 211;		break;

					case 346:		pNameString[ i ] = 338;		break;
					case 379:		pNameString[ i ] = 175;		break;
					case 377:		pNameString[ i ] = 143;		break;
					case 261:		pNameString[ i ] = 185;		break;
					case 263:		pNameString[ i ] = 230;		break;
					case 281:		pNameString[ i ] = 234;		break;

					case 322:		pNameString[ i ] = 179;		break;
					case 324:		pNameString[ i ] = 241;		break;
					case 243:		pNameString[ i ] = 243;		break;
					case 347:		pNameString[ i ] = 339;		break;
					case 380:		pNameString[ i ] = 191;		break;
					case 378:		pNameString[ i ] = 376;		break;
				}
				#endif
			}
			else
			{
				pNameString[i] ='\0';
			}
		}
	}

	if(pInfoString != NULL)
	{
		// WANNE: CHANGE THIS
		for (int i=0;i<400;i++)
		{
			if ( i<(int)strlen(Item[ubIndex].szItemDesc ))
			{
				pInfoString[i] =  Item[ubIndex].szItemDesc  [i];

				// WANNE: German specific characters
				#ifdef GERMAN
				switch( pInfoString[ i ] )
				{
					// ü
					case 65532:	pInfoString[i] = 252; break;
					// Ü
					case 65500:	pInfoString[i] = 220; break;
					// ä
					case 65508:	pInfoString[i] = 228; break;
					// Ä
					case 65476:	pInfoString[i] = 196; break;
					// ö
					case 65526:	pInfoString[i] = 246; break;
					// Ö
					case 65494:	pInfoString[i] = 214; break;
					// ß
					case 65503:	pInfoString[i] = 223; break;
				}
				#endif

				#ifdef POLISH
				switch( pNameString[ i ] )
				{
					case 260:		pInfoString[ i ] = 165;		break;
					case 262:		pInfoString[ i ] = 198;		break;
					case 280:		pInfoString[ i ] = 202;		break;
					case 321:		pInfoString[ i ] = 163;		break;
					case 323:		pInfoString[ i ] = 209;		break;
					case 211:		pInfoString[ i ] = 211;		break;

					case 346:		pInfoString[ i ] = 338;		break;
					case 379:		pInfoString[ i ] = 175;		break;
					case 377:		pInfoString[ i ] = 143;		break;
					case 261:		pInfoString[ i ] = 185;		break;
					case 263:		pInfoString[ i ] = 230;		break;
					case 281:		pInfoString[ i ] = 234;		break;

					case 322:		pInfoString[ i ] = 179;		break;
					case 324:		pInfoString[ i ] = 241;		break;
					case 243:		pInfoString[ i ] = 243;		break;
					case 347:		pInfoString[ i ] = 339;		break;
					case 380:		pInfoString[ i ] = 191;		break;
					case 378:		pInfoString[ i ] = 376;		break;
				}
				#endif
			}
			else
			{
				pInfoString[i] ='\0';
			}
		}
	}

	return(TRUE);
}

BOOLEAN LoadBRName(UINT16 ubIndex, STR16 pNameString )
{
	if (pNameString != NULL)
	{
		for (int i=0;i<80;i++)
		{
			if ( i<(int)strlen(Item[ubIndex].szBRName))
			{
				pNameString[i] =  Item[ubIndex].szBRName [i];

				// WANNE: German specific characters
				#ifdef GERMAN
				switch( pNameString[ i ] )
				{
					// ü
					case 65532:	pNameString[i] = 252; break;
					// Ü
					case 65500:	pNameString[i] = 220; break;
					// ä
					case 65508:	pNameString[i] = 228; break;
					// Ä
					case 65476:	pNameString[i] = 196; break;
					// ö
					case 65526:	pNameString[i] = 246; break;
					// Ö
					case 65494:	pNameString[i] = 214; break;
					// ß
					case 65503:	pNameString[i] = 223; break;
				}
				#endif

				#ifdef POLISH
				switch( pNameString[ i ] )
				{
					case 260:		pNameString[ i ] = 165;		break;
					case 262:		pNameString[ i ] = 198;		break;
					case 280:		pNameString[ i ] = 202;		break;
					case 321:		pNameString[ i ] = 163;		break;
					case 323:		pNameString[ i ] = 209;		break;
					case 211:		pNameString[ i ] = 211;		break;

					case 346:		pNameString[ i ] = 338;		break;
					case 379:		pNameString[ i ] = 175;		break;
					case 377:		pNameString[ i ] = 143;		break;
					case 261:		pNameString[ i ] = 185;		break;
					case 263:		pNameString[ i ] = 230;		break;
					case 281:		pNameString[ i ] = 234;		break;

					case 322:		pNameString[ i ] = 179;		break;
					case 324:		pNameString[ i ] = 241;		break;
					case 243:		pNameString[ i ] = 243;		break;
					case 347:		pNameString[ i ] = 339;		break;
					case 380:		pNameString[ i ] = 191;		break;
					case 378:		pNameString[ i ] = 376;		break;
				}
				#endif
			}
			else
			{
				pNameString[i] ='\0';
			}
		}
	}
	return TRUE;
}

BOOLEAN LoadBRDesc(UINT16 ubIndex, STR16 pDescString )
{
	if (pDescString != NULL)
	{
		for (int i=0;i<400;i++)
		{
			if ( i<(int)strlen(Item[ubIndex].szBRDesc))
			{
				pDescString[i] =  Item[ubIndex].szBRDesc [i];

				// WANNE: German specific characters
				#ifdef GERMAN
				switch( pDescString[ i ] )
				{
					// ü
					case 65532:	pDescString[i] = 252; break;
					// Ü
					case 65500:	pDescString[i] = 220; break;
					// ä
					case 65508:	pDescString[i] = 228; break;
					// Ä
					case 65476:	pDescString[i] = 196; break;
					// ö
					case 65526:	pDescString[i] = 246; break;
					// Ö
					case 65494:	pDescString[i] = 214; break;
					// ß
					case 65503:	pDescString[i] = 223; break;
				}
				#endif

				#ifdef POLISH
				switch( pDescString[ i ] )
				{
					case 260:		pDescString[ i ] = 165;		break;
					case 262:		pDescString[ i ] = 198;		break;
					case 280:		pDescString[ i ] = 202;		break;
					case 321:		pDescString[ i ] = 163;		break;
					case 323:		pDescString[ i ] = 209;		break;
					case 211:		pDescString[ i ] = 211;		break;

					case 346:		pDescString[ i ] = 338;		break;
					case 379:		pDescString[ i ] = 175;		break;
					case 377:		pDescString[ i ] = 143;		break;
					case 261:		pDescString[ i ] = 185;		break;
					case 263:		pDescString[ i ] = 230;		break;
					case 281:		pDescString[ i ] = 234;		break;

					case 322:		pDescString[ i ] = 179;		break;
					case 324:		pDescString[ i ] = 241;		break;
					case 243:		pDescString[ i ] = 243;		break;
					case 347:		pDescString[ i ] = 339;		break;
					case 380:		pDescString[ i ] = 191;		break;
					case 378:		pDescString[ i ] = 376;		break;
				}
				#endif
			}
			else
			{
				pDescString[i] ='\0';
			}
		}
	}

	return TRUE;
}

BOOLEAN LoadShortNameItemInfo(UINT16 ubIndex, STR16 pNameString )
{
	if(pNameString != NULL)
	{
		// WANNE:
		for (int i=0;i<80;i++)
		{
			if ( i<(int)strlen(Item[ubIndex].szItemName))
			{
				pNameString[i] =  Item[ubIndex].szItemName [i];

				// WANNE: German specific characters
				#ifdef GERMAN
				switch( pNameString[ i ] )
				{
					// ü
					case 65532:	pNameString[i] = 252; break;
					// Ü
					case 65500:	pNameString[i] = 220; break;
					// ä
					case 65508:	pNameString[i] = 228; break;
					// Ä
					case 65476:	pNameString[i] = 196; break;
					// ö
					case 65526:	pNameString[i] = 246; break;
					// Ö
					case 65494:	pNameString[i] = 214; break;
					// ß
					case 65503:	pNameString[i] = 223; break;
				}
				#endif

				#ifdef POLISH
				switch( pNameString[ i ] )
				{
					case 260:		pNameString[ i ] = 165;		break;
					case 262:		pNameString[ i ] = 198;		break;
					case 280:		pNameString[ i ] = 202;		break;
					case 321:		pNameString[ i ] = 163;		break;
					case 323:		pNameString[ i ] = 209;		break;
					case 211:		pNameString[ i ] = 211;		break;

					case 346:		pNameString[ i ] = 338;		break;
					case 379:		pNameString[ i ] = 175;		break;
					case 377:		pNameString[ i ] = 143;		break;
					case 261:		pNameString[ i ] = 185;		break;
					case 263:		pNameString[ i ] = 230;		break;
					case 281:		pNameString[ i ] = 234;		break;

					case 322:		pNameString[ i ] = 179;		break;
					case 324:		pNameString[ i ] = 241;		break;
					case 243:		pNameString[ i ] = 243;		break;
					case 347:		pNameString[ i ] = 339;		break;
					case 380:		pNameString[ i ] = 191;		break;
					case 378:		pNameString[ i ] = 376;		break;
				}
				#endif
			}
			else
			{
				pNameString[i] ='\0';
			}
		}
	}

	return(TRUE);
}


void LoadAllItemNames( void )
{
	UINT16 usLoop;

	for (usLoop = 0; usLoop < MAXITEMS; usLoop++)
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

INT16* GetWeightUnitString( void )
{
	if ( gGameSettings.fOptions[ TOPTION_USE_METRIC_SYSTEM ] ) // metric
	{
		return(INT16 *)( pMessageStrings[ MSG_KILOGRAM_ABBREVIATION ] );
	}
	else
	{
		return(INT16 *)( pMessageStrings[ MSG_POUND_ABBREVIATION ] );
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


