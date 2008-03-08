#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#include "text.h"
	#include "Fileman.h"
	#include "GameSettings.h"
#endif

BOOLEAN LoadItemInfo(UINT16 ubIndex, STR16 pNameString, STR16 pInfoString )
{
	int j = 0;

	if (pNameString != NULL)
	{
#if 0
		j = -1;
		for (int i=0;i<80;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szLongItemName ))
			{
				pNameString[i] =	Item[ubIndex].szLongItemName	[j];

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

				#ifdef RUSSIAN
				if ((unsigned char)Item[ubIndex].szLongItemName [j] == 208) //d0
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szLongItemName [j + 1] )
					{
						//capital letters
						case 129:		pNameString[ i ] = 197; j++; break; //U+0401	d0 81	CYRILLIC CAPITAL LETTER IO

						case 144:		pNameString[ i ] = 192; j++; break; //U+0410	A	d0 90	CYRILLIC CAPITAL LETTER A
						case 145:		pNameString[ i ] = 193; j++; break;
						case 146:		pNameString[ i ] = 194; j++; break;
						case 147:		pNameString[ i ] = 195; j++; break;
						case 148:		pNameString[ i ] = 196; j++; break;
						case 149:		pNameString[ i ] = 197; j++; break;
						case 150:		pNameString[ i ] = 198; j++; break;
						case 151:		pNameString[ i ] = 199; j++; break;
						case 152:		pNameString[ i ] = 200; j++; break;
						case 153:		pNameString[ i ] = 201; j++; break;
						case 154:		pNameString[ i ] = 202; j++; break;
						case 155:		pNameString[ i ] = 203; j++; break;
						case 156:		pNameString[ i ] = 204; j++; break;
						case 157:		pNameString[ i ] = 205; j++; break;
						case 158:		pNameString[ i ] = 206; j++; break;
						case 159:		pNameString[ i ] = 207; j++; break;
						case 160:		pNameString[ i ] = 208; j++; break;
						case 161:		pNameString[ i ] = 209; j++; break;
						case 162:		pNameString[ i ] = 210; j++; break;
						case 163:		pNameString[ i ] = 211; j++; break;
						case 164:		pNameString[ i ] = 212; j++; break;
						case 165:		pNameString[ i ] = 213; j++; break;
						case 166:		pNameString[ i ] = 214; j++; break;
						case 167:		pNameString[ i ] = 215; j++; break;
						case 168:		pNameString[ i ] = 216; j++; break;
						case 169:		pNameString[ i ] = 217; j++; break;
						case 170:		pNameString[ i ] = 218; j++; break;
						case 171:		pNameString[ i ] = 219; j++; break;
						case 172:		pNameString[ i ] = 220; j++; break;
						case 173:		pNameString[ i ] = 221; j++; break;
						case 174:		pNameString[ i ] = 222; j++; break;
						case 175:		pNameString[ i ] = 223; j++; break; //U+042F		d0 af	CYRILLIC CAPITAL LETTER YA

						//small letters
						case 176:		pNameString[ i ] = 224; j++; break; //U+0430	a	d0 b0	CYRILLIC SMALL LETTER A
						case 177:		pNameString[ i ] = 225; j++; break;
						case 178:		pNameString[ i ] = 226; j++; break;
						case 179:		pNameString[ i ] = 227; j++; break;
						case 180:		pNameString[ i ] = 228; j++; break;
						case 181:		pNameString[ i ] = 229; j++; break;
						case 182:		pNameString[ i ] = 230; j++; break;
						case 183:		pNameString[ i ] = 231; j++; break;
						case 184:		pNameString[ i ] = 232; j++; break;
						case 185:		pNameString[ i ] = 233; j++; break;
						case 186:		pNameString[ i ] = 234; j++; break;
						case 187:		pNameString[ i ] = 235; j++; break;
						case 188:		pNameString[ i ] = 236; j++; break;
						case 189:		pNameString[ i ] = 237; j++; break;
						case 190:		pNameString[ i ] = 238; j++; break;
						case 191:		pNameString[ i ] = 239; j++; break; //U+043F		d0 bf	CYRILLIC SMALL LETTER PE
					}
				}

				if ( ((unsigned char)Item[ubIndex].szLongItemName [j] == 209) ) //d1
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szLongItemName [j + 1] )
					{
						case 128:		pNameString[ i ] = 240; j++; break; //U+0440	p	d1 80	CYRILLIC SMALL LETTER ER
						case 129:		pNameString[ i ] = 241; j++; break;
						case 130:		pNameString[ i ] = 242; j++; break;
						case 131:		pNameString[ i ] = 243; j++; break;
						case 132:		pNameString[ i ] = 244; j++; break;
						case 133:		pNameString[ i ] = 245; j++; break;
						case 134:		pNameString[ i ] = 246; j++; break;
						case 135:		pNameString[ i ] = 247; j++; break;
						case 136:		pNameString[ i ] = 248; j++; break;
						case 137:		pNameString[ i ] = 249; j++; break;
						case 138:		pNameString[ i ] = 250; j++; break;
						case 139:		pNameString[ i ] = 251; j++; break;
						case 140:		pNameString[ i ] = 252; j++; break;
						case 141:		pNameString[ i ] = 253; j++; break;
						case 142:		pNameString[ i ] = 254; j++; break;
						case 143:		pNameString[ i ] = 255; j++; break;	//U+044F		d1 8f	CYRILLIC SMALL LETTER YA

						case 145:		pNameString[ i ] = 229; j++; break;	//U+0451		d1 91	CYRILLIC SMALL LETTER IO
					}
				}
			#endif

			}
			else
			{
				pNameString[i] ='\0';
			}
		}
#else
		wcsncpy( pNameString, Item[ubIndex].szLongItemName, 80);
		pNameString[79] ='\0';
#endif
	}

	if(pInfoString != NULL)
	{
#if 0
		j = -1;
		for (int i=0;i<400;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szItemDesc ))
			{
				pInfoString[i] =	Item[ubIndex].szItemDesc	[j];

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


				#ifdef RUSSIAN
				if ((unsigned char)Item[ubIndex].szItemDesc [j] == 208) //d0
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szItemDesc [j + 1] )
					{
						//capital letters
						case 129:		pInfoString[ i ] = 197; j++; break; //U+0401	d0 81	CYRILLIC CAPITAL LETTER IO

						case 144:		pInfoString[ i ] = 192; j++; break; //U+0410	A	d0 90	CYRILLIC CAPITAL LETTER A
						case 145:		pInfoString[ i ] = 193; j++; break;
						case 146:		pInfoString[ i ] = 194; j++; break;
						case 147:		pInfoString[ i ] = 195; j++; break;
						case 148:		pInfoString[ i ] = 196; j++; break;
						case 149:		pInfoString[ i ] = 197; j++; break;
						case 150:		pInfoString[ i ] = 198; j++; break;
						case 151:		pInfoString[ i ] = 199; j++; break;
						case 152:		pInfoString[ i ] = 200; j++; break;
						case 153:		pInfoString[ i ] = 201; j++; break;
						case 154:		pInfoString[ i ] = 202; j++; break;
						case 155:		pInfoString[ i ] = 203; j++; break;
						case 156:		pInfoString[ i ] = 204; j++; break;
						case 157:		pInfoString[ i ] = 205; j++; break;
						case 158:		pInfoString[ i ] = 206; j++; break;
						case 159:		pInfoString[ i ] = 207; j++; break;
						case 160:		pInfoString[ i ] = 208; j++; break;
						case 161:		pInfoString[ i ] = 209; j++; break;
						case 162:		pInfoString[ i ] = 210; j++; break;
						case 163:		pInfoString[ i ] = 211; j++; break;
						case 164:		pInfoString[ i ] = 212; j++; break;
						case 165:		pInfoString[ i ] = 213; j++; break;
						case 166:		pInfoString[ i ] = 214; j++; break;
						case 167:		pInfoString[ i ] = 215; j++; break;
						case 168:		pInfoString[ i ] = 216; j++; break;
						case 169:		pInfoString[ i ] = 217; j++; break;
						case 170:		pInfoString[ i ] = 218; j++; break;
						case 171:		pInfoString[ i ] = 219; j++; break;
						case 172:		pInfoString[ i ] = 220; j++; break;
						case 173:		pInfoString[ i ] = 221; j++; break;
						case 174:		pInfoString[ i ] = 222; j++; break;
						case 175:		pInfoString[ i ] = 223; j++; break; //U+042F		d0 af	CYRILLIC CAPITAL LETTER YA

						//small letters
						case 176:		pInfoString[ i ] = 224; j++; break; //U+0430	a	d0 b0	CYRILLIC SMALL LETTER A
						case 177:		pInfoString[ i ] = 225; j++; break;
						case 178:		pInfoString[ i ] = 226; j++; break;
						case 179:		pInfoString[ i ] = 227; j++; break;
						case 180:		pInfoString[ i ] = 228; j++; break;
						case 181:		pInfoString[ i ] = 229; j++; break;
						case 182:		pInfoString[ i ] = 230; j++; break;
						case 183:		pInfoString[ i ] = 231; j++; break;
						case 184:		pInfoString[ i ] = 232; j++; break;
						case 185:		pInfoString[ i ] = 233; j++; break;
						case 186:		pInfoString[ i ] = 234; j++; break;
						case 187:		pInfoString[ i ] = 235; j++; break;
						case 188:		pInfoString[ i ] = 236; j++; break;
						case 189:		pInfoString[ i ] = 237; j++; break;
						case 190:		pInfoString[ i ] = 238; j++; break;
						case 191:		pInfoString[ i ] = 239; j++; break; //U+043F		d0 bf	CYRILLIC SMALL LETTER PE
					}
				}

				if ( ((unsigned char)Item[ubIndex].szItemDesc [j] == 209) ) //d1
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szItemDesc [j + 1] )
					{
						case 128:		pInfoString[ i ] = 240; j++; break; //U+0440	p	d1 80	CYRILLIC SMALL LETTER ER
						case 129:		pInfoString[ i ] = 241; j++; break;
						case 130:		pInfoString[ i ] = 242; j++; break;
						case 131:		pInfoString[ i ] = 243; j++; break;
						case 132:		pInfoString[ i ] = 244; j++; break;
						case 133:		pInfoString[ i ] = 245; j++; break;
						case 134:		pInfoString[ i ] = 246; j++; break;
						case 135:		pInfoString[ i ] = 247; j++; break;
						case 136:		pInfoString[ i ] = 248; j++; break;
						case 137:		pInfoString[ i ] = 249; j++; break;
						case 138:		pInfoString[ i ] = 250; j++; break;
						case 139:		pInfoString[ i ] = 251; j++; break;
						case 140:		pInfoString[ i ] = 252; j++; break;
						case 141:		pInfoString[ i ] = 253; j++; break;
						case 142:		pInfoString[ i ] = 254; j++; break;
						case 143:		pInfoString[ i ] = 255; j++; break;	//U+044F		d1 8f	CYRILLIC SMALL LETTER YA

						case 145:		pInfoString[ i ] = 229; j++; break;	//U+0451		d1 91	CYRILLIC SMALL LETTER IO
					}
				}

				//if ( ((unsigned char)Item[ubIndex].szItemDesc [j] == 211) ) //d3
				//{
				//	// This character determines the special character
				//	switch ( (unsigned char)Item[ubIndex].szItemDesc [j + 1] )
				//	{
				//		case 162:		pInfoString[ i ] = 20; j++; break;//U+04E2	d3a2	CYRILLIC CAPITAL LETTER I WITH MACRON
				//		case 163:		pInfoString[ i ] = 20; j++; break;//U+04E3	d3a3	CYRILLIC SMALL LETTER I WITH MACRON
				//	}
				//}
			#endif
			}
			else
			{
				pInfoString[i] ='\0';
			}
		}
#else
		wcsncpy( pInfoString, Item[ubIndex].szItemDesc, 400);
		pInfoString[399] ='\0';
#endif
	}

	return(TRUE);
}

BOOLEAN LoadBRName(UINT16 ubIndex, STR16 pNameString )
{
	if (pNameString != NULL)
	{
#if 0
		int j = -1;

		for (int i=0;i<80;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szBRName))
			{
				pNameString[i] =	Item[ubIndex].szBRName [j];

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

				#ifdef RUSSIAN
				if ((unsigned char)Item[ubIndex].szBRName [j] == 208) //d0
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szBRName [j + 1] )
					{
						//capital letters
						case 129:		pNameString[ i ] = 197; j++; break; //U+0401	d0 81	CYRILLIC CAPITAL LETTER IO

						case 144:		pNameString[ i ] = 192; j++; break; //U+0410	A	d0 90	CYRILLIC CAPITAL LETTER A
						case 145:		pNameString[ i ] = 193; j++; break;
						case 146:		pNameString[ i ] = 194; j++; break;
						case 147:		pNameString[ i ] = 195; j++; break;
						case 148:		pNameString[ i ] = 196; j++; break;
						case 149:		pNameString[ i ] = 197; j++; break;
						case 150:		pNameString[ i ] = 198; j++; break;
						case 151:		pNameString[ i ] = 199; j++; break;
						case 152:		pNameString[ i ] = 200; j++; break;
						case 153:		pNameString[ i ] = 201; j++; break;
						case 154:		pNameString[ i ] = 202; j++; break;
						case 155:		pNameString[ i ] = 203; j++; break;
						case 156:		pNameString[ i ] = 204; j++; break;
						case 157:		pNameString[ i ] = 205; j++; break;
						case 158:		pNameString[ i ] = 206; j++; break;
						case 159:		pNameString[ i ] = 207; j++; break;
						case 160:		pNameString[ i ] = 208; j++; break;
						case 161:		pNameString[ i ] = 209; j++; break;
						case 162:		pNameString[ i ] = 210; j++; break;
						case 163:		pNameString[ i ] = 211; j++; break;
						case 164:		pNameString[ i ] = 212; j++; break;
						case 165:		pNameString[ i ] = 213; j++; break;
						case 166:		pNameString[ i ] = 214; j++; break;
						case 167:		pNameString[ i ] = 215; j++; break;
						case 168:		pNameString[ i ] = 216; j++; break;
						case 169:		pNameString[ i ] = 217; j++; break;
						case 170:		pNameString[ i ] = 218; j++; break;
						case 171:		pNameString[ i ] = 219; j++; break;
						case 172:		pNameString[ i ] = 220; j++; break;
						case 173:		pNameString[ i ] = 221; j++; break;
						case 174:		pNameString[ i ] = 222; j++; break;
						case 175:		pNameString[ i ] = 223; j++; break; //U+042F		d0 af	CYRILLIC CAPITAL LETTER YA

						//small letters
						case 176:		pNameString[ i ] = 224; j++; break; //U+0430	a	d0 b0	CYRILLIC SMALL LETTER A
						case 177:		pNameString[ i ] = 225; j++; break;
						case 178:		pNameString[ i ] = 226; j++; break;
						case 179:		pNameString[ i ] = 227; j++; break;
						case 180:		pNameString[ i ] = 228; j++; break;
						case 181:		pNameString[ i ] = 229; j++; break;
						case 182:		pNameString[ i ] = 230; j++; break;
						case 183:		pNameString[ i ] = 231; j++; break;
						case 184:		pNameString[ i ] = 232; j++; break;
						case 185:		pNameString[ i ] = 233; j++; break;
						case 186:		pNameString[ i ] = 234; j++; break;
						case 187:		pNameString[ i ] = 235; j++; break;
						case 188:		pNameString[ i ] = 236; j++; break;
						case 189:		pNameString[ i ] = 237; j++; break;
						case 190:		pNameString[ i ] = 238; j++; break;
						case 191:		pNameString[ i ] = 239; j++; break; //U+043F		d0 bf	CYRILLIC SMALL LETTER PE
					}
				}

				if ( ((unsigned char)Item[ubIndex].szBRName [j] == 209) ) //d1
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szBRName [j + 1] )
					{
						case 128:		pNameString[ i ] = 240; j++; break; //U+0440	p	d1 80	CYRILLIC SMALL LETTER ER
						case 129:		pNameString[ i ] = 241; j++; break;
						case 130:		pNameString[ i ] = 242; j++; break;
						case 131:		pNameString[ i ] = 243; j++; break;
						case 132:		pNameString[ i ] = 244; j++; break;
						case 133:		pNameString[ i ] = 245; j++; break;
						case 134:		pNameString[ i ] = 246; j++; break;
						case 135:		pNameString[ i ] = 247; j++; break;
						case 136:		pNameString[ i ] = 248; j++; break;
						case 137:		pNameString[ i ] = 249; j++; break;
						case 138:		pNameString[ i ] = 250; j++; break;
						case 139:		pNameString[ i ] = 251; j++; break;
						case 140:		pNameString[ i ] = 252; j++; break;
						case 141:		pNameString[ i ] = 253; j++; break;
						case 142:		pNameString[ i ] = 254; j++; break;
						case 143:		pNameString[ i ] = 255; j++; break;	//U+044F		d1 8f	CYRILLIC SMALL LETTER YA

						case 145:		pNameString[ i ] = 229; j++; break;	//U+0451		d1 91	CYRILLIC SMALL LETTER IO
					}
				}
				#endif
			}
			else
			{
				pNameString[i] ='\0';
			}
		}
#else
		wcsncpy( pNameString, Item[ubIndex].szBRName, 80);
		pNameString[79] ='\0';
#endif
	}
	return TRUE;
}

BOOLEAN LoadBRDesc(UINT16 ubIndex, STR16 pDescString )
{
	if (pDescString != NULL)
	{
#if 0
		int j = -1;

		for (int i=0;i<400;i++)
		{
			j++;
			if ( j<(int)strlen(Item[ubIndex].szBRDesc))
			{
				pDescString[i] =	Item[ubIndex].szBRDesc [j];

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

				#ifdef RUSSIAN
				if ((unsigned char)Item[ubIndex].szBRDesc [j] == 208) //d0
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szBRDesc [j + 1] )
					{
						//capital letters
						case 129:		pDescString[ i ] = 197; j++; break; //U+0401	d0 81	CYRILLIC CAPITAL LETTER IO

						case 144:		pDescString[ i ] = 192; j++; break; //U+0410	A	d0 90	CYRILLIC CAPITAL LETTER A
						case 145:		pDescString[ i ] = 193; j++; break;
						case 146:		pDescString[ i ] = 194; j++; break;
						case 147:		pDescString[ i ] = 195; j++; break;
						case 148:		pDescString[ i ] = 196; j++; break;
						case 149:		pDescString[ i ] = 197; j++; break;
						case 150:		pDescString[ i ] = 198; j++; break;
						case 151:		pDescString[ i ] = 199; j++; break;
						case 152:		pDescString[ i ] = 200; j++; break;
						case 153:		pDescString[ i ] = 201; j++; break;
						case 154:		pDescString[ i ] = 202; j++; break;
						case 155:		pDescString[ i ] = 203; j++; break;
						case 156:		pDescString[ i ] = 204; j++; break;
						case 157:		pDescString[ i ] = 205; j++; break;
						case 158:		pDescString[ i ] = 206; j++; break;
						case 159:		pDescString[ i ] = 207; j++; break;
						case 160:		pDescString[ i ] = 208; j++; break;
						case 161:		pDescString[ i ] = 209; j++; break;
						case 162:		pDescString[ i ] = 210; j++; break;
						case 163:		pDescString[ i ] = 211; j++; break;
						case 164:		pDescString[ i ] = 212; j++; break;
						case 165:		pDescString[ i ] = 213; j++; break;
						case 166:		pDescString[ i ] = 214; j++; break;
						case 167:		pDescString[ i ] = 215; j++; break;
						case 168:		pDescString[ i ] = 216; j++; break;
						case 169:		pDescString[ i ] = 217; j++; break;
						case 170:		pDescString[ i ] = 218; j++; break;
						case 171:		pDescString[ i ] = 219; j++; break;
						case 172:		pDescString[ i ] = 220; j++; break;
						case 173:		pDescString[ i ] = 221; j++; break;
						case 174:		pDescString[ i ] = 222; j++; break;
						case 175:		pDescString[ i ] = 223; j++; break; //U+042F		d0 af	CYRILLIC CAPITAL LETTER YA

						//small letters
						case 176:		pDescString[ i ] = 224; j++; break; //U+0430	a	d0 b0	CYRILLIC SMALL LETTER A
						case 177:		pDescString[ i ] = 225; j++; break;
						case 178:		pDescString[ i ] = 226; j++; break;
						case 179:		pDescString[ i ] = 227; j++; break;
						case 180:		pDescString[ i ] = 228; j++; break;
						case 181:		pDescString[ i ] = 229; j++; break;
						case 182:		pDescString[ i ] = 230; j++; break;
						case 183:		pDescString[ i ] = 231; j++; break;
						case 184:		pDescString[ i ] = 232; j++; break;
						case 185:		pDescString[ i ] = 233; j++; break;
						case 186:		pDescString[ i ] = 234; j++; break;
						case 187:		pDescString[ i ] = 235; j++; break;
						case 188:		pDescString[ i ] = 236; j++; break;
						case 189:		pDescString[ i ] = 237; j++; break;
						case 190:		pDescString[ i ] = 238; j++; break;
						case 191:		pDescString[ i ] = 239; j++; break; //U+043F		d0 bf	CYRILLIC SMALL LETTER PE
					}
				}

				if ( ((unsigned char)Item[ubIndex].szBRDesc [j] == 209) ) //d1
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szBRDesc [j + 1] )
					{
						case 128:		pDescString[ i ] = 240; j++; break; //U+0440	p	d1 80	CYRILLIC SMALL LETTER ER
						case 129:		pDescString[ i ] = 241; j++; break;
						case 130:		pDescString[ i ] = 242; j++; break;
						case 131:		pDescString[ i ] = 243; j++; break;
						case 132:		pDescString[ i ] = 244; j++; break;
						case 133:		pDescString[ i ] = 245; j++; break;
						case 134:		pDescString[ i ] = 246; j++; break;
						case 135:		pDescString[ i ] = 247; j++; break;
						case 136:		pDescString[ i ] = 248; j++; break;
						case 137:		pDescString[ i ] = 249; j++; break;
						case 138:		pDescString[ i ] = 250; j++; break;
						case 139:		pDescString[ i ] = 251; j++; break;
						case 140:		pDescString[ i ] = 252; j++; break;
						case 141:		pDescString[ i ] = 253; j++; break;
						case 142:		pDescString[ i ] = 254; j++; break;
						case 143:		pDescString[ i ] = 255; j++; break;	//U+044F		d1 8f	CYRILLIC SMALL LETTER YA

						case 145:		pDescString[ i ] = 229; j++; break;	//U+0451		d1 91	CYRILLIC SMALL LETTER IO
					}
				}
				#endif
			}
			else
			{
				pDescString[i] ='\0';
			}
		}
#else
		wcsncpy( pDescString, Item[ubIndex].szBRDesc, 400);
		pDescString[399] ='\0';
#endif
	}

	return TRUE;
}

BOOLEAN LoadShortNameItemInfo(UINT16 ubIndex, STR16 pNameString )
{
	if(pNameString != NULL)
	{
#if 0
		int j = -1;

		for (int i=0;i<80;i++)
		{
			j++;

			if ( i<(int)wcslen(Item[ubIndex].szItemName))
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



				#ifdef RUSSIAN
				if ((unsigned char)Item[ubIndex].szItemName [j] == 208) //d0
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szItemName [j + 1] )
					{
						//capital letters
						case 129:		pNameString[ i ] = 197; j++; break; //U+0401	d0 81	CYRILLIC CAPITAL LETTER IO

						case 144:		pNameString[ i ] = 192; j++; break; //U+0410	A	d0 90	CYRILLIC CAPITAL LETTER A
						case 145:		pNameString[ i ] = 193; j++; break;
						case 146:		pNameString[ i ] = 194; j++; break;
						case 147:		pNameString[ i ] = 195; j++; break;
						case 148:		pNameString[ i ] = 196; j++; break;
						case 149:		pNameString[ i ] = 197; j++; break;
						case 150:		pNameString[ i ] = 198; j++; break;
						case 151:		pNameString[ i ] = 199; j++; break;
						case 152:		pNameString[ i ] = 200; j++; break;
						case 153:		pNameString[ i ] = 201; j++; break;
						case 154:		pNameString[ i ] = 202; j++; break;
						case 155:		pNameString[ i ] = 203; j++; break;
						case 156:		pNameString[ i ] = 204; j++; break;
						case 157:		pNameString[ i ] = 205; j++; break;
						case 158:		pNameString[ i ] = 206; j++; break;
						case 159:		pNameString[ i ] = 207; j++; break;
						case 160:		pNameString[ i ] = 208; j++; break;
						case 161:		pNameString[ i ] = 209; j++; break;
						case 162:		pNameString[ i ] = 210; j++; break;
						case 163:		pNameString[ i ] = 211; j++; break;
						case 164:		pNameString[ i ] = 212; j++; break;
						case 165:		pNameString[ i ] = 213; j++; break;
						case 166:		pNameString[ i ] = 214; j++; break;
						case 167:		pNameString[ i ] = 215; j++; break;
						case 168:		pNameString[ i ] = 216; j++; break;
						case 169:		pNameString[ i ] = 217; j++; break;
						case 170:		pNameString[ i ] = 218; j++; break;
						case 171:		pNameString[ i ] = 219; j++; break;
						case 172:		pNameString[ i ] = 220; j++; break;
						case 173:		pNameString[ i ] = 221; j++; break;
						case 174:		pNameString[ i ] = 222; j++; break;
						case 175:		pNameString[ i ] = 223; j++; break; //U+042F		d0 af	CYRILLIC CAPITAL LETTER YA

						//small letters
						case 176:		pNameString[ i ] = 224; j++; break; //U+0430	a	d0 b0	CYRILLIC SMALL LETTER A
						case 177:		pNameString[ i ] = 225; j++; break;
						case 178:		pNameString[ i ] = 226; j++; break;
						case 179:		pNameString[ i ] = 227; j++; break;
						case 180:		pNameString[ i ] = 228; j++; break;
						case 181:		pNameString[ i ] = 229; j++; break;
						case 182:		pNameString[ i ] = 230; j++; break;
						case 183:		pNameString[ i ] = 231; j++; break;
						case 184:		pNameString[ i ] = 232; j++; break;
						case 185:		pNameString[ i ] = 233; j++; break;
						case 186:		pNameString[ i ] = 234; j++; break;
						case 187:		pNameString[ i ] = 235; j++; break;
						case 188:		pNameString[ i ] = 236; j++; break;
						case 189:		pNameString[ i ] = 237; j++; break;
						case 190:		pNameString[ i ] = 238; j++; break;
						case 191:		pNameString[ i ] = 239; j++; break; //U+043F		d0 bf	CYRILLIC SMALL LETTER PE
					}
				}

				if ( ((unsigned char)Item[ubIndex].szItemName [j] == 209) ) //d1
				{
					// This character determines the special character
					switch ( (unsigned char)Item[ubIndex].szItemName [j + 1] )
					{
						case 128:		pNameString[ i ] = 240; j++; break; //U+0440	p	d1 80	CYRILLIC SMALL LETTER ER
						case 129:		pNameString[ i ] = 241; j++; break;
						case 130:		pNameString[ i ] = 242; j++; break;
						case 131:		pNameString[ i ] = 243; j++; break;
						case 132:		pNameString[ i ] = 244; j++; break;
						case 133:		pNameString[ i ] = 245; j++; break;
						case 134:		pNameString[ i ] = 246; j++; break;
						case 135:		pNameString[ i ] = 247; j++; break;
						case 136:		pNameString[ i ] = 248; j++; break;
						case 137:		pNameString[ i ] = 249; j++; break;
						case 138:		pNameString[ i ] = 250; j++; break;
						case 139:		pNameString[ i ] = 251; j++; break;
						case 140:		pNameString[ i ] = 252; j++; break;
						case 141:		pNameString[ i ] = 253; j++; break;
						case 142:		pNameString[ i ] = 254; j++; break;
						case 143:		pNameString[ i ] = 255; j++; break;	//U+044F		d1 8f	CYRILLIC SMALL LETTER YA

						case 145:		pNameString[ i ] = 229; j++; break;	//U+0451		d1 91	CYRILLIC SMALL LETTER IO
					}
				}
			#endif

			}
			else
			{
				pNameString[i] ='\0';
			}
		}
#else
		wcsncpy( pNameString, Item[ubIndex].szItemName, 80 );
		pNameString[79] ='\0';
#endif
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



