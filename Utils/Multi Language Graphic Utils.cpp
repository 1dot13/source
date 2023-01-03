#ifdef PRECOMPILEDHEADERS
#include "Utils All.h"
#else
#include "builddefines.h"
#include "stdio.h"
#include "Windows.h"
#include "Types.h"
#include "Multi Language Graphic Utils.h"
#endif

#include "Language Defines.h"
//SB
#include "FileMan.h"

BOOLEAN GetMLGFilename( SGPFILENAME filename, UINT16 usMLGGraphicID )
{
	#if defined( ENGLISH )
		switch( usMLGGraphicID )
		{
			case MLG_AIMSYMBOL:
				sprintf( filename, "LAPTOP\\AimSymbol.sti" );
				return TRUE;
			case MLG_AIMSYMBOL_SMALL:
				sprintf( filename, "LAPTOP\\AimSymbol_Small.sti" );
				return TRUE;
			case MLG_BOBBYNAME:
				sprintf( filename, "LAPTOP\\BobbyName.sti" );
				return TRUE;
			case MLG_BOBBYRAYAD21:
				sprintf( filename, "LAPTOP\\BobbyRayAd_21.sti" );
				return TRUE;
			case MLG_BOBBYRAYLINK:
				sprintf( filename, "LAPTOP\\BobbyRayLink.sti" );
				return TRUE;
			case MLG_CLOSED:
				sprintf( filename, "LAPTOP\\Closed.sti" );
				return TRUE;
			case MLG_CONFIRMORDER:
				sprintf( filename, "LAPTOP\\ConfirmOrder.sti" );
				return TRUE;
			case MLG_DESKTOP:
				sprintf( filename, "LAPTOP\\desktop.pcx" );
				return TRUE;
			case MLG_FUNERALAD9:
				sprintf( filename, "LAPTOP\\FuneralAd_9.sti" );
				return TRUE;
			case MLG_GOLDPIECEBUTTONS:
				sprintf( filename, "INTERFACE\\goldpiecebuttons.sti" );
				return TRUE;
			case MLG_HISTORY:
				sprintf( filename, "LAPTOP\\history.sti" );
				return TRUE;
			case MLG_INSURANCEAD10:
				sprintf( filename, "LAPTOP\\insurancead_10.sti" );
				return TRUE;
			case MLG_INSURANCELINK:
				sprintf( filename, "LAPTOP\\insurancelink.sti" );
				return TRUE;
			case MLG_INSURANCETITLE:
				sprintf( filename, "LAPTOP\\largetitle.sti" );
				return TRUE;
			case MLG_LARGEFLORISTSYMBOL:
				sprintf( filename, "LAPTOP\\LargeSymbol.sti" );
				return TRUE;
			case MLG_SMALLFLORISTSYMBOL:
				sprintf( filename, "LAPTOP\\SmallSymbol.sti" );
				return TRUE;
			case MLG_MCGILLICUTTYS:
				sprintf( filename, "LAPTOP\\McGillicuttys.sti" );
				return TRUE;
			case MLG_MORTUARY:
				sprintf( filename, "LAPTOP\\Mortuary.sti" );
				return TRUE;
			case MLG_MORTUARYLINK:
				sprintf( filename, "LAPTOP\\MortuaryLink.sti" );
				return TRUE;
			case MLG_ORDERGRID:
				sprintf( filename, "LAPTOP\\OrderGrid.sti" );
				return TRUE;
			case MLG_PREBATTLEPANEL:
				sprintf( filename, "INTERFACE\\PreBattlePanel.sti" );
				return TRUE;
			case MLG_SMALLTITLE:
				sprintf( filename, "LAPTOP\\SmallTitle.sti" );
				return TRUE;
			case MLG_STATSBOX:
				sprintf( filename, "LAPTOP\\StatsBox.sti" );
				return TRUE;
			case MLG_STOREPLAQUE:
				sprintf( filename, "LAPTOP\\BobbyStorePlaque.sti" );
				return TRUE;
			case MLG_TITLETEXT:
				sprintf( filename, "LOADSCREENS\\titletext.sti" );
				return TRUE;
			case MLG_TITLETEXT_MP:
				sprintf( filename, "LOADSCREENS\\titletext_mp.sti" );
				return TRUE;
			case MLG_TOALUMNI:
				sprintf( filename, "LAPTOP\\ToAlumni.sti" );
				return TRUE;
			case MLG_TOMUGSHOTS:
				sprintf( filename, "LAPTOP\\ToMugShots.sti" );
				return TRUE;
			case MLG_TOSTATS:
				sprintf( filename, "LAPTOP\\ToStats.sti" );
				return TRUE;
			case MLG_WARNING:
				sprintf( filename, "LAPTOP\\Warning.sti" );
				return TRUE;
			case MLG_YOURAD13:
				sprintf( filename, "LAPTOP\\YourAd_13.sti" );
				return TRUE;
			case MLG_OPTIONHEADER:
				sprintf( filename, "INTERFACE\\optionscreenaddons.sti" );
				return TRUE;
			case MLG_LOADSAVEHEADER:
				sprintf( filename, "INTERFACE\\loadscreenaddons.sti" );
				return TRUE;
			case MLG_SPLASH:
				sprintf( filename, "INTERFACE\\splash.sti" );
				return TRUE;
			case MLG_IMPSYMBOL:
				sprintf( filename, "LAPTOP\\IMPSymbol.sti" );
				return TRUE;
//inshy: translation needed for russian version
			case MLG_BOBBYRAYTITLE:
				sprintf( filename, "LAPTOP\\BOBBYRAYTITLE.STI" );
				return TRUE;
			case MLG_BR:
				sprintf( filename, "LAPTOP\\BR.STI" );
				return TRUE;
			case MLG_MP_GOLDPIECEBUTTONS:
				sprintf( filename, "INTERFACE\\MPGOLDPIECEBUTTONS.sti" );
				return TRUE;
			case MLG_ITEMINFOADVANCEDICONS:
				sprintf( filename, "INTERFACE\\ITEMINFOADVANCEDICONS.sti" );
				return TRUE;
		}

	#elif defined( GERMAN )
		switch( usMLGGraphicID )
		{
			case MLG_AIMSYMBOL:
				//Same graphic (no translation needed)
				sprintf( filename, "LAPTOP\\AimSymbol.sti" );
				return TRUE;
			case MLG_AIMSYMBOL_SMALL:
				//Same graphic (no translation needed)
				sprintf( filename, "LAPTOP\\AimSymbol_Small.sti" );
				return TRUE;
			case MLG_BOBBYNAME:
				//Same graphic (no translation needed)
				sprintf( filename, "LAPTOP\\BobbyName.sti" );
				return TRUE;
			case MLG_BOBBYRAYAD21:
				//Same graphic (no translation needed)
				sprintf( filename, "LAPTOP\\BobbyRayAd_21.sti" );
				return TRUE;
			case MLG_BOBBYRAYLINK:
				sprintf( filename, "GERMAN\\BobbyRayLink_german.sti" );
				return TRUE;
			case MLG_CLOSED:
				sprintf( filename, "GERMAN\\Closed_german.sti" );
				return TRUE;
			case MLG_CONFIRMORDER:
				sprintf( filename, "GERMAN\\ConfirmOrder_german.sti" );
				return TRUE;
			case MLG_DESKTOP:
				sprintf( filename, "GERMAN\\desktop_german.pcx" );
				return TRUE;
			case MLG_FUNERALAD9:
				sprintf( filename, "GERMAN\\FuneralAd_12_german.sti" );
				return TRUE;
			case MLG_GOLDPIECEBUTTONS:
				sprintf( filename, "GERMAN\\goldpiecebuttons_german.sti" );
				return TRUE;
			case MLG_HISTORY:
				sprintf( filename, "GERMAN\\history_german.sti" );
				return TRUE;
			case MLG_IMPSYMBOL:
				sprintf( filename, "German\\IMPSymbol_german.sti" );
				return TRUE;
			case MLG_INSURANCEAD10:
				sprintf( filename, "GERMAN\\insurancead_10_german.sti" );
				return TRUE;
			case MLG_INSURANCELINK:
				sprintf( filename, "GERMAN\\insurancelink_german.sti" );
				return TRUE;
			case MLG_INSURANCETITLE:
				sprintf( filename, "GERMAN\\largetitle_german.sti" );
				return TRUE;
			case MLG_LARGEFLORISTSYMBOL:
				sprintf( filename, "GERMAN\\LargeSymbol_german.sti" );
				return TRUE;
			case MLG_SMALLFLORISTSYMBOL:
				sprintf( filename, "GERMAN\\SmallSymbol_german.sti" );
				return TRUE;
			case MLG_MCGILLICUTTYS:
				sprintf( filename, "GERMAN\\McGillicuttys_german.sti" );
				return TRUE;
			case MLG_MORTUARY:
				sprintf( filename, "GERMAN\\Mortuary_german.sti" );
				return TRUE;
			case MLG_MORTUARYLINK:
				sprintf( filename, "GERMAN\\MortuaryLink_german.sti" );
				return TRUE;
			case MLG_PREBATTLEPANEL:
				sprintf( filename, "GERMAN\\PreBattlePanel_german.sti" );
				return TRUE;
			case MLG_SMALLTITLE:
				sprintf( filename, "GERMAN\\SmallTitle_german.sti" );
				return TRUE;
			case MLG_STATSBOX:
				//Same file
				sprintf( filename, "LAPTOP\\StatsBox.sti" );
				return TRUE;
			case MLG_STOREPLAQUE:
				sprintf( filename, "GERMAN\\StorePlaque_german.sti" );
				return TRUE;
			case MLG_TITLETEXT:
				sprintf( filename, "GERMAN\\titletext_german.sti" );
				return TRUE;
			case MLG_TITLETEXT_MP:
				sprintf( filename, "GERMAN\\titletext_mp_german.sti" );
				return TRUE;
			case MLG_TOALUMNI:
				sprintf( filename, "GERMAN\\ToAlumni_german.sti" );
				return TRUE;
			case MLG_TOMUGSHOTS:
				sprintf( filename, "GERMAN\\ToMugShots_german.sti" );
				return TRUE;
			case MLG_TOSTATS:
				sprintf( filename, "GERMAN\\ToStats_german.sti" );
				return TRUE;
			case MLG_WARNING:
				sprintf( filename, "GERMAN\\Warning_german.sti" );
				return TRUE;
			case MLG_YOURAD13:
				sprintf( filename, "GERMAN\\YourAd_13_german.sti" );
				return TRUE;
			case MLG_OPTIONHEADER:
				sprintf( filename, "GERMAN\\optionscreenaddons_german.sti" );
				return TRUE;
			case MLG_LOADSAVEHEADER:
				sprintf( filename, "GERMAN\\loadscreenaddons_german.sti" );
				return TRUE;
			case MLG_ORDERGRID:
				//Same file
				sprintf( filename, "LAPTOP\\OrderGrid.sti" );
				return TRUE;
			case MLG_SPLASH:
				sprintf( filename, "German\\splash_german.sti" );
				return TRUE;
//inshy: Same graphic (no translation needed)
			case MLG_BOBBYRAYTITLE:
				sprintf( filename, "LAPTOP\\BOBBYRAYTITLE.STI" );
				return TRUE;
			case MLG_BR:
				sprintf( filename, "LAPTOP\\BR.STI" );
				return TRUE;
			case MLG_MP_GOLDPIECEBUTTONS:
				sprintf( filename, "GERMAN\\MPGOLDPIECEBUTTONS_german.sti" );
				return TRUE;
			case MLG_ITEMINFOADVANCEDICONS:
				sprintf( filename, "GERMAN\\ITEMINFOADVANCEDICONS_german.sti" );
				return TRUE;
		}

	#else

		UINT8 zLanguage[64];

		//The foreign language defined determines the name of the directory and filename.
		//For example, the German version of:
		//
		//		"LAPTOP\\IMPSymbol.sti" 
		//
		//	would become:
		//
		//		"GERMAN\\IMPSymbol_German.sti"

		#if   defined( DUTCH )
		  sprintf( (char *)zLanguage, "DUTCH" );
		#elif defined( FRENCH )
		  sprintf( (char *)zLanguage, "FRENCH" );
		#elif defined( GERMAN )
		  sprintf( (char *)zLanguage, "GERMAN" );
		#elif defined( ITALIAN )
		  sprintf( (char *)zLanguage, "ITALIAN" );
		#elif defined( JAPANESE )
		  sprintf( (char *)zLanguage, "JAPANESE" );
		#elif defined( KOREAN )
		  sprintf( (char *)zLanguage, "KOREAN" );
		#elif defined( POLISH )
		  sprintf( (char *)zLanguage, "POLISH" );
		#elif defined( RUSSIAN )
		  sprintf( (char *)zLanguage, "RUSSIAN" );
		#elif defined( SPANISH )
		  sprintf( (char *)zLanguage, "SPANISH" );
		#elif defined( CHINESE )
		  sprintf( (char *)zLanguage, "CHINESE" );
		#else
		#	error "At least You have to specify a Language somewhere. See comments above."
		#endif

//SB: Also check for russian Gold version, like English
		switch( usMLGGraphicID )
		{
			case MLG_AIMSYMBOL:
				sprintf( filename, "%s\\AimSymbol_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_AIMSYMBOL_SMALL:
				sprintf( filename, "%s\\AimSymbol_Small_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_BOBBYNAME:
				sprintf( filename, "%s\\BobbyName_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_BOBBYRAYAD21:
				sprintf( filename, "%s\\BobbyRayAd_21_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_BOBBYRAYLINK:
				sprintf( filename, "%s\\BobbyRayLink_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_CLOSED:
				sprintf( filename, "%s\\Closed_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_CONFIRMORDER:
				sprintf( filename, "%s\\ConfirmOrder_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_DESKTOP:
				sprintf( filename, "%s\\desktop_%s.pcx", zLanguage, zLanguage );
				break;
			case MLG_FUNERALAD9:
				sprintf( filename, "%s\\FuneralAd_9_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_GOLDPIECEBUTTONS:
				sprintf( filename, "%s\\goldpiecebuttons_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_HISTORY:
				sprintf( filename, "%s\\history_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_INSURANCEAD10:
				sprintf( filename, "%s\\insurancead_10_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_INSURANCELINK:
				sprintf( filename, "%s\\insurancelink_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_INSURANCETITLE:
				sprintf( filename, "%s\\largetitle_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_LARGEFLORISTSYMBOL:
				sprintf( filename, "%s\\LargeSymbol_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_ORDERGRID:
				sprintf( filename, "%s\\OrderGrid_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_SMALLFLORISTSYMBOL:
				sprintf( filename, "%s\\SmallSymbol_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_STATSBOX:
				sprintf( filename, "%s\\StatsBox_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_MCGILLICUTTYS:
				sprintf( filename, "%s\\McGillicuttys_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_MORTUARY:
				sprintf( filename, "%s\\Mortuary_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_MORTUARYLINK:
				sprintf( filename, "%s\\MortuaryLink_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_PREBATTLEPANEL:
				sprintf( filename, "%s\\PreBattlePanel_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_SMALLTITLE:
				sprintf( filename, "%s\\SmallTitle_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_STOREPLAQUE:
				sprintf( filename, "%s\\StorePlaque_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_TITLETEXT:
				sprintf( filename, "%s\\titletext_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_TITLETEXT_MP:
				sprintf( filename, "%s\\titletext_mp_%s.sti", zLanguage, zLanguage );
				return TRUE;
			case MLG_TOALUMNI:
				sprintf( filename, "%s\\ToAlumni_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_TOMUGSHOTS:
				sprintf( filename, "%s\\ToMugShots_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_TOSTATS:
				sprintf( filename, "%s\\ToStats_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_WARNING:
				sprintf( filename, "%s\\Warning_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_YOURAD13:
				sprintf( filename, "%s\\YourAd_13_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_OPTIONHEADER:
				sprintf( filename, "%s\\optionscreenaddons_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_LOADSAVEHEADER:
				sprintf( filename, "%s\\loadscreenaddons_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_SPLASH:
				sprintf( filename, "%s\\splash_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_IMPSYMBOL:
				sprintf( filename, "%s\\IMPSymbol_%s.sti", zLanguage, zLanguage );
				break;
//inshy: translation needed for russian version
			case MLG_BOBBYRAYTITLE:
				sprintf( filename, "%s\\BOBBYRAYTITLE_%s.STI", zLanguage, zLanguage );
				break;
			case MLG_BR:
				sprintf( filename, "%s\\BR_%s.STI", zLanguage, zLanguage );
				break;
			case MLG_MP_GOLDPIECEBUTTONS:
				sprintf( filename, "%s\\MPGOLDPIECEBUTTONS_%s.sti", zLanguage, zLanguage );
				break;
			case MLG_ITEMINFOADVANCEDICONS:
				sprintf( filename, "%s\\ITEMINFOADVANCEDICONS_%s.sti", zLanguage, zLanguage );
				break;
			default:
				return FALSE;
		}

		if(FileExists( filename ))
			return TRUE;

		switch( usMLGGraphicID )
		{
		case MLG_AIMSYMBOL:
			sprintf( filename, "LAPTOP\\AimSymbol.sti" );
			return TRUE;
		case MLG_AIMSYMBOL_SMALL:
			sprintf( filename, "LAPTOP\\AimSymbol_Small.sti" );
			return TRUE;
		case MLG_BOBBYNAME:
			sprintf( filename, "LAPTOP\\BobbyName.sti" );
			return TRUE;
		case MLG_BOBBYRAYAD21:
			sprintf( filename, "LAPTOP\\BobbyRayAd_21.sti" );
			return TRUE;
		case MLG_BOBBYRAYLINK:
			sprintf( filename, "LAPTOP\\BobbyRayLink.sti" );
			return TRUE;
		case MLG_CLOSED:
			sprintf( filename, "LAPTOP\\Closed.sti" );
			return TRUE;
		case MLG_CONFIRMORDER:
			sprintf( filename, "LAPTOP\\ConfirmOrder.sti" );
			return TRUE;
		case MLG_DESKTOP:
			sprintf( filename, "LAPTOP\\desktop.pcx" );
			return TRUE;
		case MLG_FUNERALAD9:
			sprintf( filename, "LAPTOP\\FuneralAd_9.sti" );
			return TRUE;
		case MLG_GOLDPIECEBUTTONS:
			sprintf( filename, "INTERFACE\\goldpiecebuttons.sti" );
			return TRUE;
		case MLG_HISTORY:
			sprintf( filename, "LAPTOP\\history.sti" );
			return TRUE;
		case MLG_INSURANCEAD10:
			sprintf( filename, "LAPTOP\\insurancead_10.sti" );
			return TRUE;
		case MLG_INSURANCELINK:
			sprintf( filename, "LAPTOP\\insurancelink.sti" );
			return TRUE;
		case MLG_INSURANCETITLE:
			sprintf( filename, "LAPTOP\\largetitle.sti" );
			return TRUE;
		case MLG_LARGEFLORISTSYMBOL:
			sprintf( filename, "LAPTOP\\LargeSymbol.sti" );
			return TRUE;
		case MLG_SMALLFLORISTSYMBOL:
			sprintf( filename, "LAPTOP\\SmallSymbol.sti" );
			return TRUE;
		case MLG_MCGILLICUTTYS:
			sprintf( filename, "LAPTOP\\McGillicuttys.sti" );
			return TRUE;
		case MLG_MORTUARY:
			sprintf( filename, "LAPTOP\\Mortuary.sti" );
			return TRUE;
		case MLG_MORTUARYLINK:
			sprintf( filename, "LAPTOP\\MortuaryLink.sti" );
			return TRUE;
		case MLG_ORDERGRID:
			sprintf( filename, "LAPTOP\\OrderGrid.sti" );
			return TRUE;
		case MLG_PREBATTLEPANEL:
			sprintf( filename, "INTERFACE\\PreBattlePanel.sti" );
			return TRUE;
		case MLG_SMALLTITLE:
			sprintf( filename, "LAPTOP\\SmallTitle.sti" );
			return TRUE;
		case MLG_STATSBOX:
			sprintf( filename, "LAPTOP\\StatsBox.sti" );
			return TRUE;
		case MLG_STOREPLAQUE:
			sprintf( filename, "LAPTOP\\BobbyStorePlaque.sti" );
			return TRUE;
		case MLG_TITLETEXT:
			sprintf( filename, "LOADSCREENS\\titletext.sti" );
			return TRUE;
		case MLG_TITLETEXT_MP:
			sprintf( filename, "LOADSCREENS\\titletext_mp.sti" );
			return TRUE;
		case MLG_TOALUMNI:
			sprintf( filename, "LAPTOP\\ToAlumni.sti" );
			return TRUE;
		case MLG_TOMUGSHOTS:
			sprintf( filename, "LAPTOP\\ToMugShots.sti" );
			return TRUE;
		case MLG_TOSTATS:
			sprintf( filename, "LAPTOP\\ToStats.sti" );
			return TRUE;
		case MLG_WARNING:
			sprintf( filename, "LAPTOP\\Warning.sti" );
			return TRUE;
		case MLG_YOURAD13:
			sprintf( filename, "LAPTOP\\YourAd_13.sti" );
			return TRUE;
		case MLG_OPTIONHEADER:
			sprintf( filename, "INTERFACE\\optionscreenaddons.sti" );
			return TRUE;
		case MLG_LOADSAVEHEADER:
			sprintf( filename, "INTERFACE\\loadscreenaddons.sti" );
			return TRUE;
		case MLG_SPLASH:
			sprintf( filename, "INTERFACE\\splash.sti" );
			return TRUE;
		case MLG_IMPSYMBOL:
			sprintf( filename, "LAPTOP\\IMPSymbol.sti" );
			return TRUE;
//inshy: translation needed for russian version
		case MLG_BOBBYRAYTITLE:
			sprintf( filename, "LAPTOP\\BOBBYRAYTITLE.STI" );
			return TRUE;
		case MLG_BR:
			sprintf( filename, "LAPTOP\\BR.STI" );
			return TRUE;
		case MLG_MP_GOLDPIECEBUTTONS:
			sprintf( filename, "INTERFACE\\MPGOLDPIECEBUTTONS.sti" );
			return TRUE;
		case MLG_ITEMINFOADVANCEDICONS:
			sprintf( filename, "INTERFACE\\ITEMINFOADVANCEDICONS.sti" );
			return TRUE;
		}

	#endif

	return FALSE;
}
