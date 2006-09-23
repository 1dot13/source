# Microsoft Developer Studio Project File - Name="Tactical" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Tactical - Win32 Demo Bounds Checker
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Tactical.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tactical.mak" CFG="Tactical - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tactical - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Tactical - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Tactical - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Tactical - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "Tactical - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Tactical - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Tactical - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Tactical - Win32 Demo Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Jagged Alliance 2/Development/Programming/Jagged Alliance 2/Build", AVAAAAAA"
# PROP Scc_LocalPath "..\..\..\ja2\build"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tactical - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release with Debug Info"
# PROP BASE Intermediate_Dir "Release with Debug Info"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release with Debug"
# PROP Intermediate_Dir "Release with Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "NDEBUG" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tactica0"
# PROP BASE Intermediate_Dir "Tactica0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Bounds Checker"
# PROP Intermediate_Dir "Bounds Checker"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tactical"
# PROP BASE Intermediate_Dir "Tactical"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug Demo"
# PROP Intermediate_Dir "Debug Demo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tactica1"
# PROP BASE Intermediate_Dir "Tactica1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release Demo"
# PROP Intermediate_Dir "Release Demo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "NDEBUG" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "RELEASE_WITH_DEBUG_INFO" /FR /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tactica2"
# PROP BASE Intermediate_Dir "Tactica2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Demo Release with Debug"
# PROP Intermediate_Dir "Demo Release with Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "NDEBUG" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "RELEASE_WITH_DEBUG_INFO" /FR /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tactica3"
# PROP BASE Intermediate_Dir "Tactica3"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Demo Bounds Checker"
# PROP Intermediate_Dir "Demo Bounds Checker"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "CALLBACKTIMER" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "BOUNDS_CHECKER" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR /YX"Tactical All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Tactical - Win32 Release"
# Name "Tactical - Win32 Debug"
# Name "Tactical - Win32 Release with Debug Info"
# Name "Tactical - Win32 Bounds Checker"
# Name "Tactical - Win32 Debug Demo"
# Name "Tactical - Win32 Release Demo"
# Name "Tactical - Win32 Demo Release with Debug Info"
# Name "Tactical - Win32 Demo Bounds Checker"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=".\Air Raid.cpp"
# End Source File
# Begin Source File

SOURCE=".\Animation Cache.cpp"
# End Source File
# Begin Source File

SOURCE=".\Animation Control.cpp"
# End Source File
# Begin Source File

SOURCE=".\Animation Data.cpp"
# End Source File
# Begin Source File

SOURCE=".\Arms Dealer Init.cpp"
# End Source File
# Begin Source File

SOURCE=.\ArmsDealerInvInit.cpp
# End Source File
# Begin Source File

SOURCE=".\Auto Bandage.cpp"
# End Source File
# Begin Source File

SOURCE=.\Boxing.cpp
# End Source File
# Begin Source File

SOURCE=.\bullets.cpp
# End Source File
# Begin Source File

SOURCE=.\Campaign.cpp
# End Source File
# Begin Source File

SOURCE=".\Civ Quotes.cpp"
# End Source File
# Begin Source File

SOURCE=".\Dialogue Control.cpp"
# End Source File
# Begin Source File

SOURCE=.\DisplayCover.cpp
# End Source File
# Begin Source File

SOURCE=".\Drugs And Alcohol.cpp"
# End Source File
# Begin Source File

SOURCE=".\End Game.cpp"
# End Source File
# Begin Source File

SOURCE=".\Enemy Soldier Save.cpp"
# End Source File
# Begin Source File

SOURCE=.\EnemyItemDrops.cpp
# End Source File
# Begin Source File

SOURCE=.\Faces.cpp
# End Source File
# Begin Source File

SOURCE=.\fov.cpp
# End Source File
# Begin Source File

SOURCE=.\GAP.cpp
# End Source File
# Begin Source File

SOURCE=".\Handle Doors.cpp"

!IF  "$(CFG)" == "Tactical - Win32 Release"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

# ADD CPP /Z7 /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\Handle Items.cpp"
# End Source File
# Begin Source File

SOURCE=".\Handle UI Plan.cpp"
# End Source File
# Begin Source File

SOURCE=".\Handle UI.cpp"
# End Source File
# Begin Source File

SOURCE=".\Interface Control.cpp"
# End Source File
# Begin Source File

SOURCE=".\Interface Cursors.cpp"
# End Source File
# Begin Source File

SOURCE=".\Interface Dialogue.cpp"
# End Source File
# Begin Source File

SOURCE=".\Interface Items.cpp"
# End Source File
# Begin Source File

SOURCE=".\Interface Panels.cpp"
# End Source File
# Begin Source File

SOURCE=".\Interface Utils.cpp"
# End Source File
# Begin Source File

SOURCE=.\Interface.cpp
# End Source File
# Begin Source File

SOURCE=".\Inventory Choosing.cpp"
# End Source File
# Begin Source File

SOURCE=.\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\Keys.cpp
# End Source File
# Begin Source File

SOURCE=.\LOS.cpp

!IF  "$(CFG)" == "Tactical - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

# ADD BASE CPP /Od
# ADD CPP /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

# ADD BASE CPP /Od
# ADD CPP /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

# ADD BASE CPP /Od
# ADD CPP /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\Map Information.cpp"
# End Source File
# Begin Source File

SOURCE=".\Merc Entering.cpp"
# End Source File
# Begin Source File

SOURCE=".\Merc Hiring.cpp"
# End Source File
# Begin Source File

SOURCE=".\Militia Control.cpp"
# End Source File
# Begin Source File

SOURCE=.\Morale.cpp
# End Source File
# Begin Source File

SOURCE=.\opplist.cpp
# End Source File
# Begin Source File

SOURCE=.\Overhead.cpp

!IF  "$(CFG)" == "Tactical - Win32 Release"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

# ADD CPP /Z7 /O2

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PATHAI.cpp

!IF  "$(CFG)" == "Tactical - Win32 Release"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

# ADD CPP /Z7 /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Points.cpp
# End Source File
# Begin Source File

SOURCE=.\QARRAY.cpp
# End Source File
# Begin Source File

SOURCE=.\Rain.cpp
# End Source File
# Begin Source File

SOURCE=".\Real Time Input.cpp"
# End Source File
# Begin Source File

SOURCE=".\Rotting Corpses.cpp"
# End Source File
# Begin Source File

SOURCE=".\ShopKeeper Interface.cpp"
# End Source File
# Begin Source File

SOURCE=.\SkillCheck.cpp
# End Source File
# Begin Source File

SOURCE=".\Soldier Add.cpp"
# End Source File
# Begin Source File

SOURCE=".\Soldier Ani.cpp"
# End Source File
# Begin Source File

SOURCE=".\Soldier Control.cpp"
# End Source File
# Begin Source File

SOURCE=".\Soldier Create.cpp"
# End Source File
# Begin Source File

SOURCE=".\Soldier Find.cpp"
# End Source File
# Begin Source File

SOURCE=".\Soldier Init List.cpp"

!IF  "$(CFG)" == "Tactical - Win32 Release"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

# ADD CPP /Z7 /O2

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\Soldier Profile.cpp"
# End Source File
# Begin Source File

SOURCE=".\Soldier Tile.cpp"

!IF  "$(CFG)" == "Tactical - Win32 Release"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

# ADD CPP /Z7 /Od

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

# ADD BASE CPP /Od
# ADD CPP /Od

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SoldierTooltips.cpp
# End Source File
# Begin Source File

SOURCE=".\Spread Burst.cpp"
# End Source File
# Begin Source File

SOURCE=.\Squads.cpp
# End Source File
# Begin Source File

SOURCE=".\Strategic Exit GUI.cpp"
# End Source File
# Begin Source File

SOURCE=".\Structure Wrap.cpp"
# End Source File
# Begin Source File

SOURCE=".\Tactical Save.cpp"
# End Source File
# Begin Source File

SOURCE=".\Tactical Turns.cpp"
# End Source File
# Begin Source File

SOURCE=.\TeamTurns.cpp
# End Source File
# Begin Source File

SOURCE=".\Turn Based Input.cpp"
# End Source File
# Begin Source File

SOURCE=".\UI Cursors.cpp"
# End Source File
# Begin Source File

SOURCE=.\Vehicles.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons.cpp
# End Source File
# Begin Source File

SOURCE=".\World Items.cpp"
# End Source File
# Begin Source File

SOURCE=.\XML_AmmoStrings.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_AmmoTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Armour.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_AttachmentInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Attachments.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_BurstSounds.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_ComboMergeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_CompatibleFaceItems.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_EnemyAmmoDrops.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_EnemyArmourDrops.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_EnemyExplosiveDrops.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_EnemyItemChoice.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_EnemyMiscDrops.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_EnemyWeaponChoice.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_EnemyWeaponDrops.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Explosive.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_IMPItemChoices.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_IncompatibleAttachments.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Launchable.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Magazine.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Merge.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Sounds.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_TonyInventory.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\aim.h
# End Source File
# Begin Source File

SOURCE=".\animation cache.h"
# End Source File
# Begin Source File

SOURCE=".\Animation Control.h"
# End Source File
# Begin Source File

SOURCE=".\Animation Data.h"
# End Source File
# Begin Source File

SOURCE=".\Arms Dealer Init.h"
# End Source File
# Begin Source File

SOURCE=.\Boxing.h
# End Source File
# Begin Source File

SOURCE=.\Campaign.h
# End Source File
# Begin Source File

SOURCE=".\Enemy Soldier Save.h"
# End Source File
# Begin Source File

SOURCE=".\Handle Items.h"
# End Source File
# Begin Source File

SOURCE=".\Handle UI.h"
# End Source File
# Begin Source File

SOURCE=".\Interface Cursors.h"
# End Source File
# Begin Source File

SOURCE=".\Interface Items.h"
# End Source File
# Begin Source File

SOURCE=".\interface panels.h"
# End Source File
# Begin Source File

SOURCE=.\Interface.h
# End Source File
# Begin Source File

SOURCE=".\Inventory Choosing.h"
# End Source File
# Begin Source File

SOURCE=".\Item Types.h"
# End Source File
# Begin Source File

SOURCE=.\Items.h
# End Source File
# Begin Source File

SOURCE=.\Keys.h
# End Source File
# Begin Source File

SOURCE=.\los.h
# End Source File
# Begin Source File

SOURCE=".\Map Information.h"
# End Source File
# Begin Source File

SOURCE=".\Merc Hiring.h"
# End Source File
# Begin Source File

SOURCE=".\Militia Control.h"
# End Source File
# Begin Source File

SOURCE=.\Morale.h
# End Source File
# Begin Source File

SOURCE=.\opplist.h
# End Source File
# Begin Source File

SOURCE=".\overhead types.h"
# End Source File
# Begin Source File

SOURCE=.\Overhead.h
# End Source File
# Begin Source File

SOURCE=.\PATHAI.H
# End Source File
# Begin Source File

SOURCE=.\points.h
# End Source File
# Begin Source File

SOURCE=".\ShopKeeper Interface.h"
# End Source File
# Begin Source File

SOURCE=".\ShopKeeper Quotes.h"
# End Source File
# Begin Source File

SOURCE=.\SkillCheck.h
# End Source File
# Begin Source File

SOURCE=".\Soldier Add.h"
# End Source File
# Begin Source File

SOURCE=".\soldier ani.h"
# End Source File
# Begin Source File

SOURCE=".\Soldier Control.h"
# End Source File
# Begin Source File

SOURCE=".\Soldier Create.h"
# End Source File
# Begin Source File

SOURCE=".\Soldier Init List.h"
# End Source File
# Begin Source File

SOURCE=".\Soldier macros.h"
# End Source File
# Begin Source File

SOURCE=".\soldier profile type.h"
# End Source File
# Begin Source File

SOURCE=".\Soldier Profile.h"
# End Source File
# Begin Source File

SOURCE=".\Spread burst.h"
# End Source File
# Begin Source File

SOURCE=.\Squads.h
# End Source File
# Begin Source File

SOURCE=".\Strategic Exit GUI.h"
# End Source File
# Begin Source File

SOURCE=".\Tactical All.h"
# End Source File
# Begin Source File

SOURCE=".\Tactical Save.h"
# End Source File
# Begin Source File

SOURCE=.\TeamTurns.h
# End Source File
# Begin Source File

SOURCE=.\Vehicles.h
# End Source File
# Begin Source File

SOURCE=.\weapons.h
# End Source File
# Begin Source File

SOURCE=".\World Items.h"
# End Source File
# Begin Source File

SOURCE=.\XML.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
