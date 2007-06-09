#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Loading Screen.h"
	#include "INIReader.h"
#else
	#include "vsurface.h"
	#include "mapscreen.h"
	#include "Loading Screen.h"
	#include "Campaign Types.h"
	#include "Game Clock.h"
	#include "GameSettings.h"
	#include "Random.h"
	#include "Debug.h"
	#include "local.h"
	#include "Font Control.h"
	#include "font.h"
	#include "render dirty.h"
#endif

extern HVSURFACE ghFrameBuffer;

STR8 szSectorMap[MAP_WORLD_Y][MAP_WORLD_X] =   {
	{"N",  "N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N"  ,"N"  ,"N"  ,"N"  ,"N"  ,"N"  ,"N"     ,"N"},
	
	{"N",  "A1","A2","A3","A4","A5","A6","A7","A8","A9","A10","A11","A12","A13","A14","A15","A16"   ,"N"},
	{"N",  "B1","B2","B3","B4","B5","B6","B7","B8","B9","B10","B11","B12","B13","B14","B15","B16"   ,"N"},
	{"N",  "C1","C2","C3","C4","C5","C6","C7","C8","C9","C10","C11","C12","C13","C14","C15","C16"   ,"N"},
	{"N",  "D1","D2","D3","D4","D5","D6","D7","D8","D9","D10","D11","D12","D13","D14","D15","D16"   ,"N"},
	{"N",  "E1","E2","E3","E4","E5","E6","E7","E8","E9","E10","E11","E12","E13","E14","E15","E16"   ,"N"},
	{"N",  "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16"   ,"N"},
	{"N",  "G1","G2","G3","G4","G5","G6","G7","G8","G9","G10","G11","G12","G13","G14","G15","G16"   ,"N"},
	{"N",  "H1","H2","H3","H4","H5","H6","H7","H8","H9","H10","H11","H12","H13","H14","H15","H16"   ,"N"},
	{"N",  "I1","I2","I3","I4","I5","I6","I7","I8","I9","I10","I11","I12","I13","I14","I15","I16"   ,"N"},
	{"N",  "J1","J2","J3","J4","J5","J6","J7","J8","J9","J10","J11","J12","J13","J14","J15","J16"   ,"N"},
	{"N",  "K1","K2","K3","K4","K5","K6","K7","K8","K9","K10","K11","K12","K13","K14","K15","K16"   ,"N"},
	{"N",  "L1","L2","L3","L4","L5","L6","L7","L8","L9","L10","L11","L12","L13","L14","L15","L16"   ,"N"},
	{"N",  "M1","M2","M3","M4","M5","M6","M7","M8","M9","M10","M11","M12","M13","M14","M15","M16"   ,"N"},
	{"N",  "N1","N2","N3","N4","N5","N6","N7","N8","N9","N10","N11","N12","N13","N14","N15","N16"   ,"N"},
	{"N",  "O1","O2","O3","O4","O5","O6","O7","O8","O9","O10","O11","O12","O13","O14","O15","O16"   ,"N"},
	{"N",  "P1","P2","P3","P4","P5","P6","P7","P8","P9","P10","P11","P12","P13","P14","P15","P16"   ,"N"},
	
	{"N","N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N" ,"N"  ,"N"  ,"N"  ,"N"  ,"N"  ,"N"  ,"N"  ,"N"}
	};
	
UINT8 gubLastLoadingScreenID = LOADINGSCREEN_NOTHING;
STR8 szSector;
BOOLEAN bShowSmallImage = FALSE;

SECTOR_LOADSCREENS gSectorLoadscreens[MAX_SECTOR_LOADSCREENS];

//returns the UINT8 ID for the specified sector.
UINT8 GetLoadScreenID( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	SECTORINFO *pSector;
	UINT8 ubSectorID;
	BOOLEAN fNight = FALSE;

	ubSectorID = SECTOR( sSectorX, sSectorY );
	if( NightTime() ) //before 5AM or after 9PM
	{
		fNight = TRUE;
	}

	// Which map layer? (ground, basement 1-3)?
	switch( bSectorZ )
	{
		// Ground
		case 0:

			/* User made system - BEGIN */
			if (gGameExternalOptions.gfUseExternalLoadscreens)
			{
				switch( ubSectorID )
				{
					case SEC_A1:
					case SEC_A2:
					case SEC_A3:
					case SEC_A4:
					case SEC_A5:
					case SEC_A6:
					case SEC_A7:
					case SEC_A8:
				//	case SEC_A9:	// Start sector
					case SEC_A10:
					case SEC_A11:
					case SEC_A12:
					case SEC_A13:
					case SEC_A14:
					case SEC_A15:
					case SEC_A16:
					case SEC_B1:
					case SEC_B2:
					case SEC_B3:
					case SEC_B4:
					case SEC_B5:
					case SEC_B6:
					case SEC_B7:
					case SEC_B8:
					case SEC_B9:
					case SEC_B10:
					case SEC_B11:
					case SEC_B12:
					case SEC_B13:
					case SEC_B14:
					case SEC_B15:
					case SEC_B16:
					case SEC_C1:
					case SEC_C2:
					case SEC_C3:
					case SEC_C4:
					case SEC_C5:
					case SEC_C6:
					case SEC_C7:
					case SEC_C8:
					case SEC_C9:
					case SEC_C10:
					case SEC_C11:
					case SEC_C12:
					case SEC_C13:
					case SEC_C14:
					case SEC_C15:
					case SEC_C16:
					case SEC_D1:
					case SEC_D2:
					case SEC_D3:
					case SEC_D4:
					case SEC_D5:
					case SEC_D6:
					case SEC_D7:
					case SEC_D8:
					case SEC_D9:
					case SEC_D10:
					case SEC_D11:
					case SEC_D12:
					case SEC_D13:
					case SEC_D14:
					case SEC_D15:
					case SEC_D16:
					case SEC_E1:
					case SEC_E2:
					case SEC_E3:
					case SEC_E4:
					case SEC_E5:
					case SEC_E6:
					case SEC_E7:
					case SEC_E8:
					case SEC_E9:
					case SEC_E10:
					case SEC_E11:
					case SEC_E12:
					case SEC_E13:
					case SEC_E14:
					case SEC_E15:
					case SEC_E16:
					case SEC_F1:
					case SEC_F2:
					case SEC_F3:
					case SEC_F4:
					case SEC_F5:
					case SEC_F6:
					case SEC_F7:
					case SEC_F8:
					case SEC_F9:
					case SEC_F10:
					case SEC_F11:
					case SEC_F12:
					case SEC_F13:
					case SEC_F14:
					case SEC_F15:
					case SEC_F16:
					case SEC_G1:
					case SEC_G2:
					case SEC_G3:
					case SEC_G4:
					case SEC_G5:
					case SEC_G6:
					case SEC_G7:
					case SEC_G8:
					case SEC_G9:
					case SEC_G10:
					case SEC_G11:
					case SEC_G12:
					case SEC_G13:
					case SEC_G14:
					case SEC_G15:
					case SEC_G16:
					case SEC_H1:
					case SEC_H2:
					case SEC_H3:
					case SEC_H4:
					case SEC_H5:
					case SEC_H6:
					case SEC_H7:
					case SEC_H8:
					case SEC_H9:
					case SEC_H10:
					case SEC_H11:
					case SEC_H12:
					case SEC_H13:
					case SEC_H14:
					case SEC_H15:
					case SEC_H16:
					case SEC_I1:
					case SEC_I2:
					case SEC_I3:
					case SEC_I4:
					case SEC_I5:
					case SEC_I6:
					case SEC_I7:
					case SEC_I8:
					case SEC_I9:
					case SEC_I10:
					case SEC_I11:
					case SEC_I12:
					case SEC_I13:
					case SEC_I14:
					case SEC_I15:
					case SEC_I16:
					case SEC_J1:
					case SEC_J2:
					case SEC_J3:
					case SEC_J4:
					case SEC_J5:
					case SEC_J6:
					case SEC_J7:
					case SEC_J8:
					case SEC_J9:
					case SEC_J10:
					case SEC_J11:
					case SEC_J12:
					case SEC_J13:
					case SEC_J14:
					case SEC_J15:
					case SEC_J16:
					case SEC_K1:
					case SEC_K2:
					case SEC_K3:
					case SEC_K4:
					case SEC_K5:
					case SEC_K6:
					case SEC_K7:
					case SEC_K8:
					case SEC_K9:
					case SEC_K10:
					case SEC_K11:
					case SEC_K12:
					case SEC_K13:
					case SEC_K14:
					case SEC_K15:
					case SEC_K16:
					case SEC_L1:
					case SEC_L2:
					case SEC_L3:
					case SEC_L4:
					case SEC_L5:
					case SEC_L6:
					case SEC_L7:
					case SEC_L8:
					case SEC_L9:
					case SEC_L10:
					case SEC_L11:
					case SEC_L12:
					case SEC_L13:
					case SEC_L14:
					case SEC_L15:
					case SEC_L16:
					case SEC_M1:
					case SEC_M2:
					case SEC_M3:
					case SEC_M4:
					case SEC_M5:
					case SEC_M6:
					case SEC_M7:
					case SEC_M8:
					case SEC_M9:
					case SEC_M10:
					case SEC_M11:
					case SEC_M12:
					case SEC_M13:
					case SEC_M14:
					case SEC_M15:
					case SEC_M16:
					case SEC_N1:
					case SEC_N2:
					case SEC_N3:
					case SEC_N4:
					case SEC_N5:
					case SEC_N6:
					case SEC_N7:
					case SEC_N8:
					case SEC_N9:
					case SEC_N10:
					case SEC_N11:
					case SEC_N12:
					case SEC_N13:
					case SEC_N14:
					case SEC_N15:
					case SEC_N16:
					case SEC_O1:
					case SEC_O2:
					case SEC_O3:
					case SEC_O4:
					case SEC_O5:
					case SEC_O6:
					case SEC_O7:
					case SEC_O8:
					case SEC_O9:
					case SEC_O10:
					case SEC_O11:
					case SEC_O12:
					case SEC_O13:
					case SEC_O14:
					case SEC_O15:
					case SEC_O16:
					case SEC_P1:
					case SEC_P2:
					case SEC_P3:
					case SEC_P4:
					case SEC_P5:
					case SEC_P6:
					case SEC_P7:
					case SEC_P8:
					case SEC_P9:
					case SEC_P10:
					case SEC_P11:
					case SEC_P12:
					case SEC_P13:
					case SEC_P14:
					case SEC_P15:
					case SEC_P16:
						szSector = szSectorMap [sSectorY][sSectorX];
						
						if( fNight )
						{
							return NIGHT;
						}
						else
						{
							return DAY;
						}
						
					case SEC_A9:
						szSector = "A9";
								
						if( !DidGameJustStart() )
						{		
							if( fNight )
							{
								return NIGHT;
							}
							else
							{
								return DAY;
							}												
						}
						else
						{
							return HELI;
						}
				}	
			} /* WANNE: User made System - END */
			
			/* WANNE: Sir-Tech System - BEGIN */
			else
			{
				switch( ubSectorID )
				{
					case SEC_A2:
					case SEC_B2:
						if( fNight )
							return LOADINGSCREEN_NIGHTCHITZENA;
						return LOADINGSCREEN_DAYCHITZENA;
					case SEC_A9:
						if( !DidGameJustStart() )
						{
							if( fNight )
								return LOADINGSCREEN_NIGHTOMERTA;
							return LOADINGSCREEN_DAYOMERTA;
						}
						return LOADINGSCREEN_HELI;
					case SEC_A10:
						if( fNight )
							return LOADINGSCREEN_NIGHTOMERTA;
						return LOADINGSCREEN_DAYOMERTA;
					case SEC_P3:
						if( fNight )
							return LOADINGSCREEN_NIGHTPALACE;
						return LOADINGSCREEN_DAYPALACE;
					case SEC_H13:
					case SEC_H14: //military installations
					case SEC_I13:
					case SEC_N7:
						if( fNight )
							return LOADINGSCREEN_NIGHTMILITARY;
						return LOADINGSCREEN_DAYMILITARY;
					case SEC_K4: 
						if( fNight )
							return LOADINGSCREEN_NIGHTLAB;
						return LOADINGSCREEN_DAYLAB;
					case SEC_J9:
						if( fNight )
							return LOADINGSCREEN_NIGHTPRISON;
						return LOADINGSCREEN_DAYPRISON;
					case SEC_D2:
					case SEC_D15:
					case SEC_I8:
					case SEC_N4:
						if( fNight )
							return LOADINGSCREEN_NIGHTSAM;
						return LOADINGSCREEN_DAYSAM;
					case SEC_F8:
						if( fNight )
							return LOADINGSCREEN_NIGHTHOSPITAL;
						return LOADINGSCREEN_DAYHOSPITAL;
					case SEC_B13:
					case SEC_N3:
						if( fNight )
							return LOADINGSCREEN_NIGHTAIRPORT;
						return LOADINGSCREEN_DAYAIRPORT;
					case SEC_L11:
					case SEC_L12:
						if( fNight )
							return LOADINGSCREEN_NIGHTBALIME;
						return LOADINGSCREEN_DAYBALIME;
					case SEC_H3:
					case SEC_H8:
					case SEC_D4:
						if( fNight )
							return LOADINGSCREEN_NIGHTMINE;
						return LOADINGSCREEN_DAYMINE;
				}

				pSector = &SectorInfo[ ubSectorID ];
				switch( pSector->ubTraversability[ 4 ] )
				{
					case TOWN:
						if( fNight )
						{
							if( Random( 2 ) )
							{
								return LOADINGSCREEN_NIGHTTOWN2;
							}
							else
							{
								return LOADINGSCREEN_NIGHTTOWN1;
							}
						}
						else
						{
							if( Random( 2 ) )
							{
								return LOADINGSCREEN_DAYTOWN2;
							}
							else
							{
								return LOADINGSCREEN_DAYTOWN1;
							}
						}
					case SAND:	
					case SAND_ROAD:
						if( fNight )
						{
							return LOADINGSCREEN_NIGHTDESERT;
						}
						else
						{
							return LOADINGSCREEN_DAYDESERT;
						}
					case FARMLAND:
					case FARMLAND_ROAD:
					case ROAD:					
						if( fNight )
						{
							return LOADINGSCREEN_NIGHTGENERIC;
						}
						else
						{
							return LOADINGSCREEN_DAYGENERIC;
						}
					case PLAINS:
					case SPARSE:
					case HILLS:
					case PLAINS_ROAD:
					case SPARSE_ROAD:
					case HILLS_ROAD:
						if( fNight )
						{
							return LOADINGSCREEN_NIGHTWILD;
						}
						else
						{
							return LOADINGSCREEN_DAYWILD;
						}
					case DENSE:
					case SWAMP:
					case SWAMP_ROAD:
					case DENSE_ROAD:
						if( fNight )
						{
							return LOADINGSCREEN_NIGHTFOREST;
						}
						else
						{
							return LOADINGSCREEN_DAYFOREST;
						}
					case TROPICS:
					case TROPICS_ROAD:
					case WATER:
					case NS_RIVER:
					case EW_RIVER:
					case COASTAL:
					case COASTAL_ROAD:
						if( fNight )
						{
							return LOADINGSCREEN_NIGHTTROPICAL;
						}
						else
						{
							return LOADINGSCREEN_DAYTROPICAL;
						}
					default:
						Assert( 0 );
						if( fNight )
						{
							return LOADINGSCREEN_NIGHTGENERIC;
						}
						else
						{
							return LOADINGSCREEN_DAYGENERIC;
						}
				}
				break;
			/* WANNE: Sir-Tech System */
		}

		// Basement Level 1
		case 1:
			switch( ubSectorID )
			{
				case SEC_A10: //Miguel's basement
				case SEC_I13:	//Alma prison dungeon
				case SEC_J9:	//Tixa prison dungeon
				case SEC_K4:	//Orta weapons plant
				case SEC_O3:  //Meduna 
				case SEC_P3:  //Meduna 
					return LOADINGSCREEN_BASEMENT;
				default:			//rest are mines
					return LOADINGSCREEN_MINE;
			}
			break;
		// Basement Level 2 and 3
		case 2:	
		case 3:
			//all level 2 and 3 maps are caves!
			return LOADINGSCREEN_CAVE;
		default:
			// shouldn't ever happen
			Assert( FALSE );

			if( fNight )
			{
				return LOADINGSCREEN_NIGHTGENERIC;
			}
			else
			{
				return LOADINGSCREEN_DAYGENERIC;
			}
	}
}

extern BOOLEAN gfSchedulesHosed;

//sets up the loadscreen with specified ID, and draws it to the FRAME_BUFFER, 
//and refreshing the screen with it.
void DisplayLoadScreenWithID( UINT8 ubLoadScreenID )
{
	VSURFACE_DESC		vs_desc;
	HVSURFACE			hVSurface;
	UINT32				uiLoadScreen;
	STRING512			smallImage;
	BOOLEAN				fExternalLS = FALSE;

	bShowSmallImage = FALSE;

	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;

	if (gGameExternalOptions.gfUseExternalLoadscreens)
	{
		if ((ubLoadScreenID == HELI || ubLoadScreenID == DAY || ubLoadScreenID == DAY_ALT ||
			ubLoadScreenID == NIGHT || ubLoadScreenID == NIGHT_ALT) && szSector != NULL)
		{
			fExternalLS = TRUE;
		}
		else
		{
			fExternalLS = FALSE;
		}
	}
	else
	{
		if ((ubLoadScreenID == HELI || ubLoadScreenID == DAY || ubLoadScreenID == DAY_ALT ||
			ubLoadScreenID == NIGHT || ubLoadScreenID == NIGHT_ALT) && szSector != NULL)
		{
			fExternalLS = TRUE;
		}
		else
		{
			fExternalLS = FALSE;
		}
	}

	if (fExternalLS)
	{
		// Get the image path
		char szImagePath[80];
		char szImageFormat[5];
		char szFullImagePath[80];

		// Start screen
		if (ubLoadScreenID == HELI)
		{
			strncpy(szImagePath, gSectorLoadscreens[0].szDay, 80);

			strncpy(szImageFormat, ".", MAX_IMAGE_FORMAT_CHARS);
			strcat(szImageFormat, gSectorLoadscreens[0].szImageFormat);
		}
		else
		{
			for (int i = 1; i < MAX_SECTOR_LOADSCREENS; i++)
			{
				// We found the sector
				if (strcmp(gSectorLoadscreens[i].szLocation, szSector) == 0)
				{
					strncpy(szImageFormat, ".", MAX_IMAGE_FORMAT_CHARS);

					strcat(szImageFormat, gSectorLoadscreens[i].szImageFormat);

					BOOLEAN fAlternate = gSectorLoadscreens[i].RandomAltSector;

					if (fAlternate)
					{
						// Day
						if (ubLoadScreenID == DAY)
						{
							if( Random( 2 ) )
							{
								ubLoadScreenID = DAY;
							}
							else
							{
								ubLoadScreenID = DAY_ALT;
							}
						}
						// Night
						else
						{
							if( Random( 2 ) )
							{
								ubLoadScreenID = NIGHT;
							}
							else
							{
								ubLoadScreenID = NIGHT_ALT;
							}
						}
					}

					switch (ubLoadScreenID)
					{
						case DAY:
							strncpy(szImagePath, gSectorLoadscreens[i].szDay, MAX_IMAGE_PATH_CHARS);
							break;
						case DAY_ALT:
							strncpy(szImagePath, gSectorLoadscreens[i].szDayAlt, MAX_IMAGE_PATH_CHARS);
							break;
						case NIGHT:
							strncpy(szImagePath, gSectorLoadscreens[i].szNight, MAX_IMAGE_PATH_CHARS);
							break;
						case NIGHT_ALT:
							strncpy(szImagePath, gSectorLoadscreens[i].szNightAlt, MAX_IMAGE_PATH_CHARS);
							break;
					}
					break;
				}
			}
		}

		// Small image: 640x480
		strncpy(szFullImagePath, szImagePath, 80);
		strcat(szFullImagePath, szImageFormat);
		strcpy(smallImage,szFullImagePath);

		// Actual image, depending on the resolution
		if (iResolution == 1)
		{
			strncpy(szFullImagePath, szImagePath, 80);
			strcat(szFullImagePath, "_800x600");
			strcat(szFullImagePath, szImageFormat);
		}
		else if (iResolution == 2)
		{
			strncpy(szFullImagePath, szImagePath, 80);
			strcat(szFullImagePath, "_1024x768");
			strcat(szFullImagePath, szImageFormat);
		}

		strcpy(vs_desc.ImageFile,szFullImagePath);
	}
	else
	{
		switch( ubLoadScreenID )
					{
			case LOADINGSCREEN_NOTHING:
				strcpy(smallImage, "LOADSCREENS\\LS_Heli.sti");
				if (iResolution == 0)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli_1024x768.sti");
				}
				break;
			case LOADINGSCREEN_DAYGENERIC:
				strcpy(smallImage, "LOADSCREENS\\LS_DayGeneric.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayGeneric.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayGeneric_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayGeneric_1024x768.sti");
				}
				break;
			case LOADINGSCREEN_DAYTOWN1:
				strcpy(smallImage, "LOADSCREENS\\LS_DayTown1.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTown1.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTown1_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTown1_1024x768.sti");
				}
				break;
			case LOADINGSCREEN_DAYTOWN2:
				strcpy(smallImage, "LOADSCREENS\\LS_DayTown2.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTown2.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTown2_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTown2_1024x768.sti");
				}

				break;
			case LOADINGSCREEN_DAYWILD:
				strcpy(smallImage, "LOADSCREENS\\LS_DayWild.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayWild.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayWild_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayWild_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYTROPICAL:
				strcpy(smallImage, "LOADSCREENS\\LS_DayTropical.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTropical.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTropical_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayTropical_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYFOREST:
				strcpy(smallImage, "LOADSCREENS\\LS_DayForest.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayForest.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayForest_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayForest_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYDESERT:
				strcpy(smallImage, "LOADSCREENS\\LS_DayDesert.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayDesert.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayDesert_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayDesert_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYPALACE:
				strcpy(smallImage, "LOADSCREENS\\LS_DayPalace.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPalace.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPalace_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPalace_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTGENERIC:
				strcpy(smallImage, "LOADSCREENS\\LS_NightGeneric.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightGeneric.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightGeneric_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightGeneric_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTWILD:
				strcpy(smallImage, "LOADSCREENS\\LS_NightWild.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightWild.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightWild_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightWild_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTTOWN1:
				strcpy(smallImage, "LOADSCREENS\\LS_NightTown1.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTown1.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTown1_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTown1_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTTOWN2:
				strcpy(smallImage, "LOADSCREENS\\LS_NightTown2.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTown2.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTown2_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTown2_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTFOREST:
				strcpy(smallImage, "LOADSCREENS\\LS_NightForest.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightForest.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightForest_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightForest_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTTROPICAL:
				strcpy(smallImage, "LOADSCREENS\\LS_NightTropical.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTropical.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTropical_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightTropical_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTDESERT:
				strcpy(smallImage, "LOADSCREENS\\LS_NightDesert.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightDesert.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightDesert_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightDesert_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTPALACE:
				strcpy(smallImage, "LOADSCREENS\\LS_NightPalace.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPalace.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPalace_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPalace_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_HELI:
				strcpy(smallImage, "LOADSCREENS\\LS_Heli.sti");
				if (iResolution == 0)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_BASEMENT:
				strcpy(smallImage, "LOADSCREENS\\LS_Basement.sti");
				if (iResolution == 0)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Basement.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Basement_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Basement_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_MINE:
				strcpy(smallImage, "LOADSCREENS\\LS_Mine.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Mine.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Mine_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Mine_1024x768.sti");
				}

				break;
			case LOADINGSCREEN_CAVE:
				strcpy(smallImage, "LOADSCREENS\\LS_Cave.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Cave.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Cave_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Cave_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYPINE:
				strcpy(smallImage, "LOADSCREENS\\LS_DayPine.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPine.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPine_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPine_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTPINE:
				strcpy(smallImage, "LOADSCREENS\\LS_NightPine.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPine.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPine_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPine_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYMILITARY:
				strcpy(smallImage, "LOADSCREENS\\LS_DayMilitary.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayMilitary.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayMilitary_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayMilitary_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTMILITARY:
				strcpy(smallImage, "LOADSCREENS\\LS_NightMilitary.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightMilitary.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightMilitary_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightMilitary_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYSAM:
				strcpy(smallImage, "LOADSCREENS\\LS_DaySAM.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DaySAM.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DaySAM_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DaySAM_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTSAM:
				strcpy(smallImage, "LOADSCREENS\\LS_NightSAM.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightSAM.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightSAM_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightSAM_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYPRISON:
				strcpy(smallImage, "LOADSCREENS\\LS_DayPrison.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPrison.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPrison_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayPrison_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTPRISON:
				strcpy(smallImage, "LOADSCREENS\\LS_NightPrison.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPrison.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPrison_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightPrison_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYHOSPITAL:
				strcpy(smallImage, "LOADSCREENS\\LS_DayHospital.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayHospital.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayHospital_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayHospital_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTHOSPITAL:
				strcpy(smallImage, "LOADSCREENS\\LS_NightHospital.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightHospital.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightHospital_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightHospital_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYAIRPORT:
				strcpy(smallImage, "LOADSCREENS\\LS_DayAirport.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayAirport.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayAirport_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayAirport_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTAIRPORT:
				strcpy(smallImage, "LOADSCREENS\\LS_NightAirport.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightAirport.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightAirport_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightAirport_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYLAB:
				strcpy(smallImage, "LOADSCREENS\\LS_DayLab.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayLab.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayLab_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayLab_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTLAB:
				strcpy(smallImage, "LOADSCREENS\\LS_NightLab.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightLab.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightLab_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightLab_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYOMERTA:
				strcpy(smallImage, "LOADSCREENS\\LS_DayOmerta.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayOmerta.sti");
				}
				else if (iResolution == 1)
				{	
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayOmerta_800x600.sti");
				}
				else if (iResolution == 2)
				{	
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayOmerta_1024x768.sti");
				}
				break;
			case LOADINGSCREEN_NIGHTOMERTA:
				strcpy(smallImage, "LOADSCREENS\\LS_NightOmerta.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightOmerta.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightOmerta_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightOmerta_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYCHITZENA:
				strcpy(smallImage, "LOADSCREENS\\LS_DayChitzena.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayChitzena.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayChitzena_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayChitzena_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_NIGHTCHITZENA:
				strcpy(smallImage, "LOADSCREENS\\LS_NightChitzena.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightChitzena.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightChitzena_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightChitzena_1024x768.sti");
				}
				
				break;
			case LOADINGSCREEN_DAYMINE:
				strcpy(smallImage, "LOADSCREENS\\LS_DayMine.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayMine.sti" );
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayMine_800x600.sti" );
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayMine_1024x768.sti" );
				}
				
				break;
			case LOADINGSCREEN_NIGHTMINE:
				strcpy(smallImage, "LOADSCREENS\\LS_NightMine.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightMine.sti" );
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightMine_800x600.sti" );
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightMine_1024x768.sti" );
				}
				
				break;
			case LOADINGSCREEN_DAYBALIME:
				strcpy(smallImage, "LOADSCREENS\\LS_DayBalime.sti");
				if (iResolution == 0)
				{
				strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayBalime.sti" );
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayBalime_800x600.sti" );
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_DayBalime_1024x768.sti" );
				}
				
				break;
			case LOADINGSCREEN_NIGHTBALIME:
				strcpy(smallImage, "LOADSCREENS\\LS_NightBalime.sti");
				if (iResolution == 0)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightBalime.sti" );
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightBalime_800x600.sti" );
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_NightBalime_1024x768.sti" );
				}
				
				break;
			default:
				strcpy(smallImage, "LOADSCREENS\\LS_Heli.sti");
				if (iResolution == 0)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli.sti");
				}
				else if (iResolution == 1)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli_800x600.sti");
				}
				else if (iResolution == 2)
				{
					strcpy(vs_desc.ImageFile, "LOADSCREENS\\LS_Heli_1024x768.sti");
				}
				break;
		}	// Switch - END
	}	

	// Sti loadscreen (big image) is not available
	if ( AddVideoSurface( &vs_desc, &uiLoadScreen ) == FALSE && iResolution > 0)
	{
		bShowSmallImage = TRUE;
		strcpy(vs_desc.ImageFile, smallImage);
	}

	if( gfSchedulesHosed )
	{
		SetFont( FONT10ARIAL );
		SetFontForeground( FONT_YELLOW );
		SetFontShadow( FONT_NEARBLACK );
		ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
		mprintf( 5, 5, L"Error loading save, attempting to patch save to version 1.02...", vs_desc.ImageFile );
	}
	else if( AddVideoSurface( &vs_desc, &uiLoadScreen ) )
	{ //Blit the background image
		GetVideoSurface( &hVSurface, uiLoadScreen );

		// Special case -> show the small image centered
		if (iResolution > 0 && bShowSmallImage == TRUE)
		{
			BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, iScreenWidthOffset, iScreenHeightOffset, 0, NULL );
		}
		else
		{
			BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, 0, 0, 0, NULL );
		}

		DeleteVideoSurfaceFromIndex( uiLoadScreen );
	}
	else
	{ //Failed to load the file, so use a black screen and print out message.
		SetFont( FONT10ARIAL );
		SetFontForeground( FONT_YELLOW );
		SetFontShadow( FONT_NEARBLACK );
		ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
		mprintf( 5, 5, L"%S loadscreen data file not found...", vs_desc.ImageFile );
	}

	gubLastLoadingScreenID = ubLoadScreenID;
	InvalidateScreen( );
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();
	RefreshScreen( NULL );
}

