#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Loading Screen.h"
#else
	//???
#endif

extern HVSURFACE ghFrameBuffer;



UINT8 gubLastLoadingScreenID = LOADINGSCREEN_NOTHING;
BOOLEAN bShowSmallImage = FALSE;


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
	switch( bSectorZ )
	{
		case 0:
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
						return LOADINGSCREEN_NIGHTTOWN1;
					}
					if( Random( 2 ) )
					{
						return LOADINGSCREEN_DAYTOWN2;
					}
					return LOADINGSCREEN_DAYTOWN1;
				case SAND:	
				case SAND_ROAD:
					if( fNight )
					{
						return LOADINGSCREEN_NIGHTDESERT;
					}
					return LOADINGSCREEN_DAYDESERT;
				case FARMLAND:
				case FARMLAND_ROAD:
				case ROAD:					
					if( fNight )
					{
						return LOADINGSCREEN_NIGHTGENERIC;
					}
					return LOADINGSCREEN_DAYGENERIC;
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
					return LOADINGSCREEN_DAYWILD;
				case DENSE:
				case SWAMP:
				case SWAMP_ROAD:
				case DENSE_ROAD:
					if( fNight )
					{
						return LOADINGSCREEN_NIGHTFOREST;
					}
					return LOADINGSCREEN_DAYFOREST;
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
					return LOADINGSCREEN_DAYTROPICAL;
				default:
					Assert( 0 );
					if( fNight )
					{
						return LOADINGSCREEN_NIGHTGENERIC;
					}
					return LOADINGSCREEN_DAYGENERIC;
			}
			break;
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
			return LOADINGSCREEN_DAYGENERIC;
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

	bShowSmallImage = FALSE;

	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;

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

