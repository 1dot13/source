#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Encrypted File.h"
	#include "FileMan.h"
	#include "Debug.h"
#endif

#include "Language Defines.h"
// anv: for selecting random line
#include "Random.h"

// anv: loading random line from the file
BOOLEAN LoadEncryptedDataFromFileRandomLine(STR pFileName, STR16 pDestString, UINT32 uiSeekAmount)
{
	HWFILE		hFile;	
	UINT32		uiBytesRead;
	UINT32		uiSeekFrom;
 
	hFile = FileOpen(pFileName, FILE_ACCESS_READ, FALSE);
	if ( !hFile )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed to FileOpen");
		return( FALSE );
	}

	uiSeekFrom = Random( ( FileGetSize(hFile) / uiSeekAmount ) - 1 );

	if ( FileSeek( hFile, uiSeekFrom * uiSeekAmount, FILE_SEEK_FROM_START ) == FALSE )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileSeek");
		return( FALSE );
	}

	if( !FileRead( hFile, pDestString, uiSeekAmount, &uiBytesRead) )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileRead");
		return( FALSE );
	}

	DecodeString(pDestString, uiSeekAmount);

	FileClose(hFile);
	return(TRUE);
}

BOOLEAN LoadEncryptedDataFromFile(STR pFileName, STR16 pDestString, UINT32 uiSeekFrom, UINT32 uiSeekAmount)
{
	HWFILE		hFile;	
	UINT32		uiBytesRead;


	hFile = FileOpen(pFileName, FILE_ACCESS_READ, FALSE);
	if ( !hFile )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed to FileOpen");
		return( FALSE );
	}

	if ( FileSeek( hFile, uiSeekFrom, FILE_SEEK_FROM_START ) == FALSE )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileSeek");
		return( FALSE );
	}

	if( !FileRead( hFile, pDestString, uiSeekAmount, &uiBytesRead) )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileRead");
		return( FALSE );
	}



	//// Decrement, by 1, any value > 32
	//for(i=0; (i<uiSeekAmount) && (pDestString[i] != 0); i++ )
	//{
	//	if( pDestString[i] > 33 )
	//		pDestString[i] -= 1;		
	//	

	//}

	DecodeString(pDestString, uiSeekAmount);

	FileClose(hFile);
	return(TRUE);
}


void DecodeString(STR16 pDestString, UINT32 uiSeekAmount)
{
	UINT32		i;

	// Decrement, by 1, any value > 32
	for(i=0; (i<uiSeekAmount) && (pDestString[i] != 0); i++ )
	{
		if( pDestString[i] > 33 )
			pDestString[i] -= 1;	

		//#ifdef GERMAN
  //             //switch( pDestString[ i ] )
  //             //{
  //             //     // ü
  //             //     case 252:          pDestString[i] = 252;          break;
  //             //     // Ü
  //             //     case 220:          pDestString[i] = 220;          break;
  //             //     // ä
  //             //     case 228:          pDestString[i] = 228;          break;
  //             //     // Ä
  //             //     case 196:          pDestString[i] = 196;          break;
  //             //     // ö
  //             //     case 246:          pDestString[i] = 246;          break;
  //             //     // Ö
  //             //     case 214:          pDestString[i] = 214;          break;
  //             //     // ß
  //             //     case 223:          pDestString[i] = 223;          break;
  //             //}
		//#endif

		//#ifdef RUSSIAN
		//	switch( pDestString[ i ] )
		//	{
		//		//capital letters
		//		case 168:          pDestString[ i ] = 1025;  break;	//U+0401		   d0 81     CYRILLIC CAPITAL LETTER IO
  //              case 192:          pDestString[ i ] = 1040;  break; //U+0410     A     d0 90     CYRILLIC CAPITAL LETTER A
  //              case 193:          pDestString[ i ] = 1041;  break;
  //              case 194:          pDestString[ i ] = 1042;  break;
  //              case 195:          pDestString[ i ] = 1043;  break;
  //              case 196:          pDestString[ i ] = 1044;  break;
  //              case 197:          pDestString[ i ] = 1045;  break;
  //              case 198:          pDestString[ i ] = 1046;  break;
  //              case 199:          pDestString[ i ] = 1047;  break;
  //              case 200:          pDestString[ i ] = 1048;  break;
  //              case 201:          pDestString[ i ] = 1049;  break;
  //              case 202:          pDestString[ i ] = 1050;  break;
  //              case 203:          pDestString[ i ] = 1051;  break;
  //              case 204:          pDestString[ i ] = 1052;  break;
  //              case 205:          pDestString[ i ] = 1053;  break;
  //              case 206:          pDestString[ i ] = 1054;  break;
  //              case 207:          pDestString[ i ] = 1055;  break;
  //              case 208:          pDestString[ i ] = 1056;  break;
  //              case 209:          pDestString[ i ] = 1057;  break;
  //              case 210:          pDestString[ i ] = 1058;  break;
  //              case 211:          pDestString[ i ] = 1059;  break;
  //              case 212:          pDestString[ i ] = 1060;  break;
  //              case 213:          pDestString[ i ] = 1061;  break;
  //              case 214:          pDestString[ i ] = 1062;  break;
  //              case 215:          pDestString[ i ] = 1063;  break;
  //              case 216:          pDestString[ i ] = 1064;  break;
  //              case 217:          pDestString[ i ] = 1065;  break;
  //              case 218:          pDestString[ i ] = 1066;  break;
  //              case 219:          pDestString[ i ] = 1067;  break;
  //              case 220:          pDestString[ i ] = 1068;  break;
  //              case 221:          pDestString[ i ] = 1069;  break;
  //              case 222:          pDestString[ i ] = 1070;  break;
  //              case 223:          pDestString[ i ] = 1071;  break; //U+042F           d0 af     CYRILLIC CAPITAL LETTER YA

  //              //small letters
		//		case 184:          pDestString[ i ] = 1105;  break; //U+0451           d1 91     CYRILLIC SMALL LETTER IO
				//case 185:          pDestString[ i ] = 8470;  break;		// ?
				//case 178:          pDestString[ i ] = 1030;  break;		// ?
				//case 161:          pDestString[ i ] = 1038;  break;		// í
				//case 179:          pDestString[ i ] = 1110;  break;		// ?
				//case 162:          pDestString[ i ] = 1118;  break;		// ó
				//case 165:          pDestString[ i ] = 1168;  break;		// Ñ
				//case 170:          pDestString[ i ] = 1028;  break;		// ¬
				//case 175:          pDestString[ i ] = 1031;  break;		// »
				//case 180:          pDestString[ i ] = 1169;  break;		// ?
				//case 186:          pDestString[ i ] = 1108;  break;		// ?
				//case 191:          pDestString[ i ] = 1111;  break;		// ?
  //              case 224:          pDestString[ i ] = 1072;  break; //U+0430     a     d0 b0     CYRILLIC SMALL LETTER A
  //              case 225:          pDestString[ i ] = 1073;  break;
  //              case 226:          pDestString[ i ] = 1074;  break;
  //              case 227:          pDestString[ i ] = 1075;  break;
  //              case 228:          pDestString[ i ] = 1076;  break;
  //              case 229:          pDestString[ i ] = 1077;  break;
  //              case 230:          pDestString[ i ] = 1078;  break;
  //              case 231:          pDestString[ i ] = 1079;  break;
  //              case 232:          pDestString[ i ] = 1080;  break;
  //              case 233:          pDestString[ i ] = 1081;  break;
  //              case 234:          pDestString[ i ] = 1082;  break;
  //              case 235:          pDestString[ i ] = 1083;  break;
  //              case 236:          pDestString[ i ] = 1084;  break;
  //              case 237:          pDestString[ i ] = 1085;  break;
  //              case 238:          pDestString[ i ] = 1086;  break;
  //              case 239:          pDestString[ i ] = 1087;  break; //U+043F           d0 bf     CYRILLIC SMALL LETTER PE
  //              case 240:          pDestString[ i ] = 1088;  break; //U+0440     p     d1 80     CYRILLIC SMALL LETTER ER
  //              case 241:          pDestString[ i ] = 1089;  break;
  //              case 242:          pDestString[ i ] = 1090;  break;
  //              case 243:          pDestString[ i ] = 1091;  break;
  //              case 244:          pDestString[ i ] = 1092;  break;
  //              case 245:          pDestString[ i ] = 1093;  break;
  //              case 246:          pDestString[ i ] = 1094;  break;
  //              case 247:          pDestString[ i ] = 1095;  break;
  //              case 248:          pDestString[ i ] = 1096;  break;
  //              case 249:          pDestString[ i ] = 1097;  break;
  //              case 250:          pDestString[ i ] = 1098;  break;
  //              case 251:          pDestString[ i ] = 1099;  break;
  //              case 252:          pDestString[ i ] = 1100;  break;
  //              case 253:          pDestString[ i ] = 1101;  break;
  //              case 254:          pDestString[ i ] = 1102;  break;
  //              case 255:          pDestString[ i ] = 1103;  break; //U+044F           d1 8f     CYRILLIC SMALL LETTER YA    

		//	}
		//#endif

		//#ifdef POLISH
		//   switch( pDestString[ i ] )
		//   {
		//		case 165:          pDestString[ i ] = 260;          break;
		//		case 198:          pDestString[ i ] = 262;          break;
		//		case 202:          pDestString[ i ] = 280;          break;
		//		case 163:          pDestString[ i ] = 321;          break;
		//		case 209:          pDestString[ i ] = 323;          break;
		//		case 211:          pDestString[ i ] = 211;          break;

		//		case 338:          pDestString[ i ] = 346;          break;
		//		case 175:          pDestString[ i ] = 379;          break;
		//		case 143:          pDestString[ i ] = 377;          break;
		//		case 185:          pDestString[ i ] = 261;          break;
		//		case 230:          pDestString[ i ] = 263;          break;
		//		case 234:          pDestString[ i ] = 281;          break;

		//		case 179:          pDestString[ i ] = 322;          break;
		//		case 241:          pDestString[ i ] = 324;          break;
		//		case 243:          pDestString[ i ] = 243;          break;
		//		case 339:          pDestString[ i ] = 347;          break;
		//		case 191:          pDestString[ i ] = 380;          break;
		//		case 376:          pDestString[ i ] = 378;          break;
		//   }
		//#endif
	}
}