# Microsoft Developer Studio Project File - Name="Laptop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Laptop - Win32 Demo Bounds Checker
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Laptop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Laptop.mak" CFG="Laptop - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Laptop - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Laptop - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Laptop - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Laptop - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "Laptop - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Laptop - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Laptop - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Laptop - Win32 Demo Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Jagged Alliance 2/Development/Programming/Jagged Alliance 2/Build", AVAAAAAA"
# PROP Scc_LocalPath "..\..\..\ja2\build"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Laptop - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I ".\\" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"Laptop All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Laptop__"
# PROP BASE Intermediate_Dir "Laptop__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I ".\\" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"Laptop All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Laptop - Win32 Release with Debug Info"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Laptop_0"
# PROP BASE Intermediate_Dir "Laptop_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release with Debug"
# PROP Intermediate_Dir "Release with Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "JA2" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Editor" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"Laptop All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Laptop - Win32 Bounds Checker"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Laptop_2"
# PROP BASE Intermediate_Dir "Laptop_2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Bounds Checker"
# PROP Intermediate_Dir "Bounds Checker"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR /YX"Laptop All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug Demo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Laptop_1"
# PROP BASE Intermediate_Dir "Laptop_1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug Demo"
# PROP Intermediate_Dir "Debug Demo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR /YX"Laptop All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Laptop - Win32 Release Demo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Laptop_3"
# PROP BASE Intermediate_Dir "Laptop_3"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release Demo"
# PROP Intermediate_Dir "Release Demo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "JA2" /D "RELEASE_WITH_DEBUG_INFO" /FR /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR /YX"Laptop All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Release with Debug Info"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Laptop_4"
# PROP BASE Intermediate_Dir "Laptop_4"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Demo Release with Debug"
# PROP Intermediate_Dir "Demo Release with Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "JA2" /D "RELEASE_WITH_DEBUG_INFO" /FR /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Editor" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"Laptop All.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Bounds Checker"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Laptop_5"
# PROP BASE Intermediate_Dir "Laptop_5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Demo Bounds Checker"
# PROP Intermediate_Dir "Demo Bounds Checker"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "BOUNDS_CHECKER" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR /YX"Laptop All.h" /FD /c
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

# Name "Laptop - Win32 Release"
# Name "Laptop - Win32 Debug"
# Name "Laptop - Win32 Release with Debug Info"
# Name "Laptop - Win32 Bounds Checker"
# Name "Laptop - Win32 Debug Demo"
# Name "Laptop - Win32 Release Demo"
# Name "Laptop - Win32 Demo Release with Debug Info"
# Name "Laptop - Win32 Demo Bounds Checker"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\aim.cpp
# End Source File
# Begin Source File

SOURCE=.\AimArchives.cpp
# End Source File
# Begin Source File

SOURCE=.\AimFacialIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AimHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\AimLinks.cpp
# End Source File
# Begin Source File

SOURCE=.\AimMembers.cpp
# End Source File
# Begin Source File

SOURCE=.\AimPolicies.cpp
# End Source File
# Begin Source File

SOURCE=.\AimSort.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyR.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyRAmmo.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyRArmour.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyRGuns.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyRMailOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyRMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyRShipments.cpp
# End Source File
# Begin Source File

SOURCE=.\BobbyRUsed.cpp
# End Source File
# Begin Source File

SOURCE=.\BrokenLink.cpp
# End Source File
# Begin Source File

SOURCE=.\CharProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\email.cpp
# End Source File
# Begin Source File

SOURCE=.\files.cpp
# End Source File
# Begin Source File

SOURCE=.\finances.cpp
# End Source File
# Begin Source File

SOURCE=".\florist Cards.cpp"
# End Source File
# Begin Source File

SOURCE=".\florist Gallery.cpp"
# End Source File
# Begin Source File

SOURCE=".\florist Order Form.cpp"
# End Source File
# Begin Source File

SOURCE=.\florist.cpp
# End Source File
# Begin Source File

SOURCE=.\funeral.cpp
# End Source File
# Begin Source File

SOURCE=.\history.cpp
# End Source File
# Begin Source File

SOURCE=".\IMP AboutUs.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Attribute Entrance.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Attribute Finish.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Attribute Selection.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Begin Screen.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Compile Character.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Confirm.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Finish.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP HomePage.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP MainPage.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Personality Entrance.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Personality Finish.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Personality Quiz.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Portraits.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Skill Trait.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Text System.cpp"
# End Source File
# Begin Source File

SOURCE=".\IMP Voices.cpp"
# End Source File
# Begin Source File

SOURCE=.\IMPVideoObjects.cpp
# End Source File
# Begin Source File

SOURCE=".\insurance Comments.cpp"
# End Source File
# Begin Source File

SOURCE=".\insurance Contract.cpp"
# End Source File
# Begin Source File

SOURCE=".\insurance Info.cpp"
# End Source File
# Begin Source File

SOURCE=.\insurance.cpp
# End Source File
# Begin Source File

SOURCE=.\laptop.cpp
# End Source File
# Begin Source File

SOURCE=".\mercs Account.cpp"
# End Source File
# Begin Source File

SOURCE=".\mercs Files.cpp"
# End Source File
# Begin Source File

SOURCE=".\mercs No Account.cpp"
# End Source File
# Begin Source File

SOURCE=.\mercs.cpp
# End Source File
# Begin Source File

SOURCE=.\personnel.cpp

!IF  "$(CFG)" == "Laptop - Win32 Release"

!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug"

!ELSEIF  "$(CFG)" == "Laptop - Win32 Release with Debug Info"

# ADD CPP /W4 /O2

!ELSEIF  "$(CFG)" == "Laptop - Win32 Bounds Checker"

!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Laptop - Win32 Release Demo"

# ADD BASE CPP /W4 /O2
# ADD CPP /W4 /O2

!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Release with Debug Info"

# ADD BASE CPP /W4 /O2
# ADD CPP /W4 /O2

!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Bounds Checker"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sirtech.cpp
# End Source File
# Begin Source File

SOURCE=".\Store Inventory.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\aim.h
# End Source File
# Begin Source File

SOURCE=.\AimMembers.h
# End Source File
# Begin Source File

SOURCE=.\BobbyRAmmo.h
# End Source File
# Begin Source File

SOURCE=.\BobbyRArmour.h
# End Source File
# Begin Source File

SOURCE=.\BobbyRGuns.h
# End Source File
# Begin Source File

SOURCE=.\BobbyRMailOrder.h
# End Source File
# Begin Source File

SOURCE=.\BobbyRMisc.h
# End Source File
# Begin Source File

SOURCE=.\BobbyRShipments.h
# End Source File
# Begin Source File

SOURCE=.\BobbyRUsed.h
# End Source File
# Begin Source File

SOURCE=.\BrokenLink.h
# End Source File
# Begin Source File

SOURCE=.\CharProfile.h
# End Source File
# Begin Source File

SOURCE=.\email.h
# End Source File
# Begin Source File

SOURCE=.\files.h
# End Source File
# Begin Source File

SOURCE=.\finances.h
# End Source File
# Begin Source File

SOURCE=".\florist Cards.h"
# End Source File
# Begin Source File

SOURCE=".\florist Gallery.h"
# End Source File
# Begin Source File

SOURCE=".\florist Order Form.h"
# End Source File
# Begin Source File

SOURCE=.\florist.h
# End Source File
# Begin Source File

SOURCE=.\funeral.h
# End Source File
# Begin Source File

SOURCE=.\history.h
# End Source File
# Begin Source File

SOURCE=".\IMP AboutUs.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Attribute Entrance.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Attribute Finish.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Attribute Selection.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Begin Screen.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Compile Character.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Confirm.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Finish.h"
# End Source File
# Begin Source File

SOURCE=".\IMP HomePage.h"
# End Source File
# Begin Source File

SOURCE=".\IMP MainPage.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Personality Entrance.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Personality Finish.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Personality Quiz.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Portraits.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Skill Trait.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Text System.h"
# End Source File
# Begin Source File

SOURCE=".\IMP Voices.h"
# End Source File
# Begin Source File

SOURCE=.\IMPVideoObjects.h
# End Source File
# Begin Source File

SOURCE=".\insurance Comments.h"
# End Source File
# Begin Source File

SOURCE=".\insurance Contract.h"
# End Source File
# Begin Source File

SOURCE=".\insurance Info.h"
# End Source File
# Begin Source File

SOURCE=".\Insurance Text.h"
# End Source File
# Begin Source File

SOURCE=.\insurance.h
# End Source File
# Begin Source File

SOURCE=".\Laptop All.h"
# End Source File
# Begin Source File

SOURCE=.\laptop.h
# End Source File
# Begin Source File

SOURCE=.\LaptopSave.h
# End Source File
# Begin Source File

SOURCE=.\mercs.h
# End Source File
# Begin Source File

SOURCE=.\personnel.h
# End Source File
# Begin Source File

SOURCE=.\sirtech.h
# End Source File
# Begin Source File

SOURCE=".\Store Inventory.h"
# End Source File
# End Group
# End Target
# End Project
