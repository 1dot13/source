#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#include "text.h"
	#include "Fileman.h"
#endif

BOOLEAN LoadItemInfo(UINT16 ubIndex, STR16 pNameString, STR16 pInfoString )
{
	int j = 0;

	if (pNameString != NULL)
	{
		j = -1;
		for (int i=0;i<80;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szLongItemName ))
			{
				pNameString[i] =  Item[ubIndex].szLongItemName  [j];

				#ifdef GERMAN
				// We have a german special character
				if (Item[ubIndex].szLongItemName [j] == -61)
				{
					// This character determines the special character
					switch (Item[ubIndex].szLongItemName [j + 1])
					{
						// ü
						case -68:	
							pNameString[i] = 252;
							// Skip next character, because "umlaute" have 2 chars
							j++;
							break;
						// Ü
						case -100:	
							pNameString[i] = 220;
							j++;
							break;
						// ä
						case -92:	
							pNameString[i] = 228; 
							j++;
							break;
						// Ä
						case -124:	
							pNameString[i] = 196; 
							j++;
							break;
						// ö
						case -74:	
							pNameString[i] = 246; 
							j++;
							break;
						// Ö
						case -106:	
							pNameString[i] = 214; 
							j++;
							break;
						// ß
						case -97:	
							pNameString[i] = 223; 
							j++;
							break;
					}
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
		j = -1;
		for (int i=0;i<400;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szItemDesc ))
			{
				pInfoString[i] =  Item[ubIndex].szItemDesc  [j];

				#ifdef GERMAN
				// We have a german special character
				if (Item[ubIndex].szItemDesc [j] == -61)
				{
					// This character determines the special character
					switch (Item[ubIndex].szItemDesc [j + 1])
					{
						// ü
						case -68:	
							pInfoString[i] = 252;
							// Skip next character, because "umlaute" have 2 chars
							j++;
							break;
						// Ü
						case -100:	
							pInfoString[i] = 220;
							j++;
							break;
						// ä
						case -92:	
							pInfoString[i] = 228; 
							j++;
							break;
						// Ä
						case -124:	
							pInfoString[i] = 196; 
							j++;
							break;
						// ö
						case -74:	
							pInfoString[i] = 246; 
							j++;
							break;
						// Ö
						case -106:	
							pInfoString[i] = 214; 
							j++;
							break;
						// ß
						case -97:	
							pInfoString[i] = 223; 
							j++;
							break;
					}
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
		int j = -1;

		for (int i=0;i<80;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szBRName))
			{
				pNameString[i] =  Item[ubIndex].szBRName [j];

				#ifdef GERMAN
				// We have a german special character
				if (Item[ubIndex].szBRName [j] == -61)
				{
					// This character determines the special character
					switch (Item[ubIndex].szBRName [j + 1])
					{
						// ü
						case -68:	
							pNameString[i] = 252;
							// Skip next character, because "umlaute" have 2 chars
							j++;
							break;
						// Ü
						case -100:	
							pNameString[i] = 220;
							j++;
							break;
						// ä
						case -92:	
							pNameString[i] = 228; 
							j++;
							break;
						// Ä
						case -124:	
							pNameString[i] = 196; 
							j++;
							break;
						// ö
						case -74:	
							pNameString[i] = 246; 
							j++;
							break;
						// Ö
						case -106:	
							pNameString[i] = 214; 
							j++;
							break;
						// ß
						case -97:	
							pNameString[i] = 223; 
							j++;
							break;
					}
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
		int j = -1;

		for (int i=0;i<400;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szBRDesc))
			{
				pDescString[i] =  Item[ubIndex].szBRDesc [j];

				// WANNE: German specific characters
				#ifdef GERMAN
				// We have a german special character
				if (Item[ubIndex].szBRDesc [j] == -61)
				{
					// This character determines the special character
					switch (Item[ubIndex].szBRDesc [j + 1])
					{
						// ü
						case -68:	
							pDescString[i] = 252;
							// Skip next character, because "umlaute" have 2 chars
							j++;
							break;
						// Ü
						case -100:	
							pDescString[i] = 220;
							j++;
							break;
						// ä
						case -92:	
							pDescString[i] = 228; 
							j++;
							break;
						// Ä
						case -124:	
							pDescString[i] = 196; 
							j++;
							break;
						// ö
						case -74:	
							pDescString[i] = 246; 
							j++;
							break;
						// Ö
						case -106:	
							pDescString[i] = 214; 
							j++;
							break;
						// ß
						case -97:	
							pDescString[i] = 223; 
							j++;
							break;
					}
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
		int j = -1;

		// WANNE:
		for (int i=0;i<80;i++)
		{
			j++;
			
			if ( i<(int)strlen(Item[ubIndex].szItemName))
			{
				pNameString[i] = Item[ubIndex].szItemName [j];

				// WANNE: German specific characters
				#ifdef GERMAN
				// We have a german special character
				if (Item[ubIndex].szItemName [j] == -61)
				{
					// This character determines the special character
					switch (Item[ubIndex].szItemName [j + 1])
					{
						// ü
						case -68:	
							pNameString[i] = 252;
							// Skip next character, because "umlaute" have 2 chars
							j++;
							break;
						// Ü
						case -100:	
							pNameString[i] = 220;
							j++;
							break;
						// ä
						case -92:	
							pNameString[i] = 228; 
							j++;
							break;
						// Ä
						case -124:	
							pNameString[i] = 196; 
							j++;
							break;
						// ö
						case -74:	
							pNameString[i] = 246; 
							j++;
							break;
						// Ö
						case -106:	
							pNameString[i] = 214; 
							j++;
							break;
						// ß
						case -97:	
							pNameString[i] = 223; 
							j++;
							break;
					}
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


