# Microsoft Developer Studio Project File - Name="Standard Gaming Platform" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Standard Gaming Platform - Win32 Debug Demo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Standard Gaming Platform.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Debug Demo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Standard Gaming Platform - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Standard Gaming Platform - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Standard Gaming Platform - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Standard Gaming Platform - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "Standard Gaming Platform - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Standard Gaming Platform - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Standard Gaming Platform - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Jagged Alliance 2/Development/Programming/Jagged Alliance 2/Build", AVAAAAAA"
# PROP Scc_LocalPath "..\ja2\build"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NO_ZLIB" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"JA2 SGP ALL.H" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\Build" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I "..\\" /I ".\\" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR /YX"JA2 SGP ALL.H" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release with Debug Info"
# PROP BASE Intermediate_Dir "Release with Debug Info"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release with Debug Info"
# PROP Intermediate_Dir "Release with Debug Info"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "NDEBUG" /D "JA2" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /YX"JA2 SGP ALL.H" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Standar0"
# PROP BASE Intermediate_Dir "Standar0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Standar0"
# PROP Intermediate_Dir "Standar0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /FR /YX"JA2 SGP ALL.H" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Standard"
# PROP BASE Intermediate_Dir "Standard"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Standard"
# PROP Intermediate_Dir "Standard"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /FR /YX"JA2 SGP ALL.H" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Standar1"
# PROP BASE Intermediate_Dir "Standar1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Standar1"
# PROP Intermediate_Dir "Standar1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /O2 /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "NDEBUG" /D "JA2" /D "WIN32" /D "_WINDOWS" /D "RELEASE_WITH_DEBUG_INFO" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /YX"JA2 SGP ALL.H" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Standar2"
# PROP BASE Intermediate_Dir "Standar2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Standar2"
# PROP Intermediate_Dir "Standar2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /O2 /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "NDEBUG" /D "JA2" /D "WIN32" /D "_WINDOWS" /D "RELEASE_WITH_DEBUG_INFO" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "XML_STATIC" /D "CINTERFACE" /YX"JA2 SGP ALL.H" /FD /c
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

# Name "Standard Gaming Platform - Win32 Release"
# Name "Standard Gaming Platform - Win32 Debug"
# Name "Standard Gaming Platform - Win32 Release with Debug Info"
# Name "Standard Gaming Platform - Win32 Bounds Checker"
# Name "Standard Gaming Platform - Win32 Debug Demo"
# Name "Standard Gaming Platform - Win32 Release Demo"
# Name "Standard Gaming Platform - Win32 Demo Release with Debug Info"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=".\Button Sound Control.cpp"
# End Source File
# Begin Source File

SOURCE=".\Button System.cpp"
# End Source File
# Begin Source File

SOURCE=".\Container.cpp"
# End Source File
# Begin Source File

SOURCE=".\Cursor Control.cpp"
# End Source File
# Begin Source File

SOURCE=".\DbMan.cpp"
# End Source File
# Begin Source File

SOURCE=".\DEBUG.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"

# ADD CPP /D "_DEBUG"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"

# ADD BASE CPP /D "_DEBUG"
# ADD CPP /D "_DEBUG"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"

# ADD BASE CPP /D "_DEBUG"
# ADD CPP /D "_DEBUG"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"

# ADD BASE CPP /D "_DEBUG"
# ADD CPP /D "_DEBUG"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\DirectDraw Calls.cpp"
# End Source File
# Begin Source File

SOURCE=".\DirectX Common.cpp"
# End Source File
# Begin Source File

SOURCE=".\English.cpp"
# End Source File
# Begin Source File

SOURCE=.\ExceptionHandling.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCat.cpp
# End Source File
# Begin Source File

SOURCE=".\FileMan.cpp"
# End Source File
# Begin Source File

SOURCE=".\Font.cpp"
# End Source File
# Begin Source File

SOURCE=".\himage.cpp"
# End Source File
# Begin Source File

SOURCE=".\impTGA.cpp"
# End Source File
# Begin Source File

SOURCE=".\input.cpp"
# End Source File
# Begin Source File

SOURCE=".\Install.cpp"
# End Source File
# Begin Source File

SOURCE=".\LibraryDataBase.cpp"
# End Source File
# Begin Source File

SOURCE=".\line.cpp"
# End Source File
# Begin Source File

SOURCE=".\MemMan.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"

# ADD CPP /D "_MEMMAN_DEBUG"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"

# ADD BASE CPP /D "_MEMMAN_DEBUG"
# ADD CPP /D "_MEMMAN_DEBUG"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"

# ADD BASE CPP /D "_MEMMAN_DEBUG"
# ADD CPP /D "_MEMMAN_DEBUG"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\mousesystem.cpp"
# End Source File
# Begin Source File

SOURCE=".\Mutex Manager.cpp"
# End Source File
# Begin Source File

SOURCE=".\PCX.cpp"
# End Source File
# Begin Source File

SOURCE=".\Random.cpp"
# End Source File
# Begin Source File

SOURCE=.\readdir.cpp
# End Source File
# Begin Source File

SOURCE=".\RegInst.cpp"
# End Source File
# Begin Source File

SOURCE=".\sgp.cpp"
# End Source File
# Begin Source File

SOURCE=".\shading.cpp"
# End Source File
# Begin Source File

SOURCE=".\soundman.cpp"
# End Source File
# Begin Source File

SOURCE=".\STCI.cpp"
# End Source File
# Begin Source File

SOURCE=.\stringicmp.cpp
# End Source File
# Begin Source File

SOURCE=".\timer.cpp"
# End Source File
# Begin Source File

SOURCE=".\video.cpp"
# End Source File
# Begin Source File

SOURCE=".\vobject.cpp"
# End Source File
# Begin Source File

SOURCE=".\vobject_blitters.cpp"
# End Source File
# Begin Source File

SOURCE=".\vsurface.cpp"
# End Source File
# Begin Source File

SOURCE=.\WinFont.cpp
# End Source File
# Begin Source File

SOURCE=".\ddraw.lib"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=".\Button System.h"
# End Source File
# Begin Source File

SOURCE=".\container.h"
# End Source File
# Begin Source File

SOURCE=".\Cursor Control.h"
# End Source File
# Begin Source File

SOURCE=".\DbMan.h"
# End Source File
# Begin Source File

SOURCE=".\Debug.h"
# End Source File
# Begin Source File

SOURCE=".\DirectDraw Calls.h"
# End Source File
# Begin Source File

SOURCE=".\DirectX Common.h"
# End Source File
# Begin Source File

SOURCE=".\english.h"
# End Source File
# Begin Source File

SOURCE=.\ExceptionHandling.h
# End Source File
# Begin Source File

SOURCE=.\FileCat.h
# End Source File
# Begin Source File

SOURCE=".\FileMan.h"
# End Source File
# Begin Source File

SOURCE="..\Utils\Font Control.h"
# End Source File
# Begin Source File

SOURCE=".\font.h"
# End Source File
# Begin Source File

SOURCE=".\gameloop.h"
# End Source File
# Begin Source File

SOURCE=".\himage.h"
# End Source File
# Begin Source File

SOURCE=".\imgfmt.h"
# End Source File
# Begin Source File

SOURCE=".\impTGA.h"
# End Source File
# Begin Source File

SOURCE=".\Input.h"
# End Source File
# Begin Source File

SOURCE=".\Install.h"
# End Source File
# Begin Source File

SOURCE=".\JA2 SGP ALL.H"
# End Source File
# Begin Source File

SOURCE=..\jascreens.h
# End Source File
# Begin Source File

SOURCE=".\LibraryDataBase.h"
# End Source File
# Begin Source File

SOURCE=".\line.h"
# End Source File
# Begin Source File

SOURCE=..\local.h
# End Source File
# Begin Source File

SOURCE=".\MemMan.h"
# End Source File
# Begin Source File

SOURCE=".\mousesystem.h"
# End Source File
# Begin Source File

SOURCE=".\mousesystem_macros.h"
# End Source File
# Begin Source File

SOURCE=".\Mutex Manager.h"
# End Source File
# Begin Source File

SOURCE=".\pcx.h"
# End Source File
# Begin Source File

SOURCE=".\random.h"
# End Source File
# Begin Source File

SOURCE=.\readdir.h
# End Source File
# Begin Source File

SOURCE=".\RegInst.h"
# End Source File
# Begin Source File

SOURCE="..\TileEngine\render dirty.h"
# End Source File
# Begin Source File

SOURCE=..\screenids.h
# End Source File
# Begin Source File

SOURCE=..\SCREENS.H
# End Source File
# Begin Source File

SOURCE=".\sgp.h"
# End Source File
# Begin Source File

SOURCE=".\shading.h"
# End Source File
# Begin Source File

SOURCE=".\soundman.h"
# End Source File
# Begin Source File

SOURCE=".\STCI.h"
# End Source File
# Begin Source File

SOURCE=.\stringicmp.h
# End Source File
# Begin Source File

SOURCE=".\timer.h"
# End Source File
# Begin Source File

SOURCE=".\TopicIDs.h"
# End Source File
# Begin Source File

SOURCE=".\TopicOps.h"
# End Source File
# Begin Source File

SOURCE=".\trle.h"
# End Source File
# Begin Source File

SOURCE=.\Types.h
# End Source File
# Begin Source File

SOURCE=".\Video.h"
# End Source File
# Begin Source File

SOURCE=".\video_private.h"
# End Source File
# Begin Source File

SOURCE=".\vobject.h"
# End Source File
# Begin Source File

SOURCE=".\vobject_blitters.h"
# End Source File
# Begin Source File

SOURCE=".\vobject_private.h"
# End Source File
# Begin Source File

SOURCE=".\vsurface.h"
# End Source File
# Begin Source File

SOURCE=".\vsurface_private.h"
# End Source File
# Begin Source File

SOURCE=".\WCheck.h"
# End Source File
# Begin Source File

SOURCE=".\Winbart97.h"
# End Source File
# Begin Source File

SOURCE=.\WinFont.h
# End Source File
# Begin Source File

SOURCE=".\WizShare.h"
# End Source File
# Begin Source File

SOURCE=".\ZCONF.H"
# End Source File
# End Group
# End Target
# End Project
