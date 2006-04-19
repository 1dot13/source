# Microsoft Developer Studio Generated NMAKE File, Based on Standard Gaming Platform.dsp
!IF "$(CFG)" == ""
CFG=Standard Gaming Platform - Win32 Debug Demo
!MESSAGE No configuration specified. Defaulting to Standard Gaming Platform - Win32 Debug Demo.
!ENDIF 

!IF "$(CFG)" != "Standard Gaming Platform - Win32 Release" && "$(CFG)" != "Standard Gaming Platform - Win32 Debug" && "$(CFG)" != "Standard Gaming Platform - Win32 Release with Debug Info" && "$(CFG)" != "Standard Gaming Platform - Win32 Bounds Checker" && "$(CFG)" != "Standard Gaming Platform - Win32 Debug Demo" && "$(CFG)" != "Standard Gaming Platform - Win32 Release Demo" && "$(CFG)" != "Standard Gaming Platform - Win32 Demo Release with Debug Info"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Standard Gaming Platform.lib" "$(OUTDIR)\Standard Gaming Platform.bsc"


CLEAN :
	-@erase "$(INTDIR)\Button Sound Control.obj"
	-@erase "$(INTDIR)\Button Sound Control.sbr"
	-@erase "$(INTDIR)\Button System.obj"
	-@erase "$(INTDIR)\Button System.sbr"
	-@erase "$(INTDIR)\Container.obj"
	-@erase "$(INTDIR)\Container.sbr"
	-@erase "$(INTDIR)\Cursor Control.obj"
	-@erase "$(INTDIR)\Cursor Control.sbr"
	-@erase "$(INTDIR)\DbMan.obj"
	-@erase "$(INTDIR)\DbMan.sbr"
	-@erase "$(INTDIR)\DEBUG.obj"
	-@erase "$(INTDIR)\DEBUG.sbr"
	-@erase "$(INTDIR)\DirectDraw Calls.obj"
	-@erase "$(INTDIR)\DirectDraw Calls.sbr"
	-@erase "$(INTDIR)\DirectX Common.obj"
	-@erase "$(INTDIR)\DirectX Common.sbr"
	-@erase "$(INTDIR)\English.obj"
	-@erase "$(INTDIR)\English.sbr"
	-@erase "$(INTDIR)\ExceptionHandling.obj"
	-@erase "$(INTDIR)\ExceptionHandling.sbr"
	-@erase "$(INTDIR)\FileMan.obj"
	-@erase "$(INTDIR)\FileMan.sbr"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\Font.sbr"
	-@erase "$(INTDIR)\himage.obj"
	-@erase "$(INTDIR)\himage.sbr"
	-@erase "$(INTDIR)\impTGA.obj"
	-@erase "$(INTDIR)\impTGA.sbr"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\input.sbr"
	-@erase "$(INTDIR)\Install.obj"
	-@erase "$(INTDIR)\Install.sbr"
	-@erase "$(INTDIR)\LibraryDataBase.obj"
	-@erase "$(INTDIR)\LibraryDataBase.sbr"
	-@erase "$(INTDIR)\line.obj"
	-@erase "$(INTDIR)\line.sbr"
	-@erase "$(INTDIR)\MemMan.obj"
	-@erase "$(INTDIR)\MemMan.sbr"
	-@erase "$(INTDIR)\mousesystem.obj"
	-@erase "$(INTDIR)\mousesystem.sbr"
	-@erase "$(INTDIR)\Mutex Manager.obj"
	-@erase "$(INTDIR)\Mutex Manager.sbr"
	-@erase "$(INTDIR)\PCX.obj"
	-@erase "$(INTDIR)\PCX.sbr"
	-@erase "$(INTDIR)\Random.obj"
	-@erase "$(INTDIR)\Random.sbr"
	-@erase "$(INTDIR)\RegInst.obj"
	-@erase "$(INTDIR)\RegInst.sbr"
	-@erase "$(INTDIR)\sgp.obj"
	-@erase "$(INTDIR)\sgp.sbr"
	-@erase "$(INTDIR)\shading.obj"
	-@erase "$(INTDIR)\shading.sbr"
	-@erase "$(INTDIR)\soundman.obj"
	-@erase "$(INTDIR)\soundman.sbr"
	-@erase "$(INTDIR)\STCI.obj"
	-@erase "$(INTDIR)\STCI.sbr"
	-@erase "$(INTDIR)\timer.obj"
	-@erase "$(INTDIR)\timer.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\video.obj"
	-@erase "$(INTDIR)\video.sbr"
	-@erase "$(INTDIR)\vobject.obj"
	-@erase "$(INTDIR)\vobject.sbr"
	-@erase "$(INTDIR)\vobject_blitters.obj"
	-@erase "$(INTDIR)\vobject_blitters.sbr"
	-@erase "$(INTDIR)\vsurface.obj"
	-@erase "$(INTDIR)\vsurface.sbr"
	-@erase "$(INTDIR)\WinFont.obj"
	-@erase "$(INTDIR)\WinFont.sbr"
	-@erase "$(OUTDIR)\Standard Gaming Platform.bsc"
	-@erase "$(OUTDIR)\Standard Gaming Platform.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NO_ZLIB" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Standard Gaming Platform.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Button Sound Control.sbr" \
	"$(INTDIR)\Button System.sbr" \
	"$(INTDIR)\Container.sbr" \
	"$(INTDIR)\Cursor Control.sbr" \
	"$(INTDIR)\DbMan.sbr" \
	"$(INTDIR)\DEBUG.sbr" \
	"$(INTDIR)\DirectDraw Calls.sbr" \
	"$(INTDIR)\DirectX Common.sbr" \
	"$(INTDIR)\English.sbr" \
	"$(INTDIR)\ExceptionHandling.sbr" \
	"$(INTDIR)\FileMan.sbr" \
	"$(INTDIR)\Font.sbr" \
	"$(INTDIR)\himage.sbr" \
	"$(INTDIR)\impTGA.sbr" \
	"$(INTDIR)\input.sbr" \
	"$(INTDIR)\Install.sbr" \
	"$(INTDIR)\LibraryDataBase.sbr" \
	"$(INTDIR)\line.sbr" \
	"$(INTDIR)\MemMan.sbr" \
	"$(INTDIR)\mousesystem.sbr" \
	"$(INTDIR)\Mutex Manager.sbr" \
	"$(INTDIR)\PCX.sbr" \
	"$(INTDIR)\Random.sbr" \
	"$(INTDIR)\RegInst.sbr" \
	"$(INTDIR)\sgp.sbr" \
	"$(INTDIR)\shading.sbr" \
	"$(INTDIR)\soundman.sbr" \
	"$(INTDIR)\STCI.sbr" \
	"$(INTDIR)\timer.sbr" \
	"$(INTDIR)\video.sbr" \
	"$(INTDIR)\vobject.sbr" \
	"$(INTDIR)\vobject_blitters.sbr" \
	"$(INTDIR)\vsurface.sbr" \
	"$(INTDIR)\WinFont.sbr"

"$(OUTDIR)\Standard Gaming Platform.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Standard Gaming Platform.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Button Sound Control.obj" \
	"$(INTDIR)\Button System.obj" \
	"$(INTDIR)\Container.obj" \
	"$(INTDIR)\Cursor Control.obj" \
	"$(INTDIR)\DbMan.obj" \
	"$(INTDIR)\DEBUG.obj" \
	"$(INTDIR)\DirectDraw Calls.obj" \
	"$(INTDIR)\DirectX Common.obj" \
	"$(INTDIR)\English.obj" \
	"$(INTDIR)\ExceptionHandling.obj" \
	"$(INTDIR)\FileMan.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\himage.obj" \
	"$(INTDIR)\impTGA.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\Install.obj" \
	"$(INTDIR)\LibraryDataBase.obj" \
	"$(INTDIR)\line.obj" \
	"$(INTDIR)\MemMan.obj" \
	"$(INTDIR)\mousesystem.obj" \
	"$(INTDIR)\Mutex Manager.obj" \
	"$(INTDIR)\PCX.obj" \
	"$(INTDIR)\Random.obj" \
	"$(INTDIR)\RegInst.obj" \
	"$(INTDIR)\sgp.obj" \
	"$(INTDIR)\shading.obj" \
	"$(INTDIR)\soundman.obj" \
	"$(INTDIR)\STCI.obj" \
	"$(INTDIR)\timer.obj" \
	"$(INTDIR)\video.obj" \
	"$(INTDIR)\vobject.obj" \
	"$(INTDIR)\vobject_blitters.obj" \
	"$(INTDIR)\vsurface.obj" \
	"$(INTDIR)\WinFont.obj" \
	".\ddraw.lib"

"$(OUTDIR)\Standard Gaming Platform.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Standard Gaming Platform.lib" "$(OUTDIR)\Standard Gaming Platform.bsc"


CLEAN :
	-@erase "$(INTDIR)\Button Sound Control.obj"
	-@erase "$(INTDIR)\Button Sound Control.sbr"
	-@erase "$(INTDIR)\Button System.obj"
	-@erase "$(INTDIR)\Button System.sbr"
	-@erase "$(INTDIR)\Container.obj"
	-@erase "$(INTDIR)\Container.sbr"
	-@erase "$(INTDIR)\Cursor Control.obj"
	-@erase "$(INTDIR)\Cursor Control.sbr"
	-@erase "$(INTDIR)\DbMan.obj"
	-@erase "$(INTDIR)\DbMan.sbr"
	-@erase "$(INTDIR)\DEBUG.obj"
	-@erase "$(INTDIR)\DEBUG.sbr"
	-@erase "$(INTDIR)\DirectDraw Calls.obj"
	-@erase "$(INTDIR)\DirectDraw Calls.sbr"
	-@erase "$(INTDIR)\DirectX Common.obj"
	-@erase "$(INTDIR)\DirectX Common.sbr"
	-@erase "$(INTDIR)\English.obj"
	-@erase "$(INTDIR)\English.sbr"
	-@erase "$(INTDIR)\ExceptionHandling.obj"
	-@erase "$(INTDIR)\ExceptionHandling.sbr"
	-@erase "$(INTDIR)\FileMan.obj"
	-@erase "$(INTDIR)\FileMan.sbr"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\Font.sbr"
	-@erase "$(INTDIR)\himage.obj"
	-@erase "$(INTDIR)\himage.sbr"
	-@erase "$(INTDIR)\impTGA.obj"
	-@erase "$(INTDIR)\impTGA.sbr"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\input.sbr"
	-@erase "$(INTDIR)\Install.obj"
	-@erase "$(INTDIR)\Install.sbr"
	-@erase "$(INTDIR)\LibraryDataBase.obj"
	-@erase "$(INTDIR)\LibraryDataBase.sbr"
	-@erase "$(INTDIR)\line.obj"
	-@erase "$(INTDIR)\line.sbr"
	-@erase "$(INTDIR)\MemMan.obj"
	-@erase "$(INTDIR)\MemMan.sbr"
	-@erase "$(INTDIR)\mousesystem.obj"
	-@erase "$(INTDIR)\mousesystem.sbr"
	-@erase "$(INTDIR)\Mutex Manager.obj"
	-@erase "$(INTDIR)\Mutex Manager.sbr"
	-@erase "$(INTDIR)\PCX.obj"
	-@erase "$(INTDIR)\PCX.sbr"
	-@erase "$(INTDIR)\Random.obj"
	-@erase "$(INTDIR)\Random.sbr"
	-@erase "$(INTDIR)\RegInst.obj"
	-@erase "$(INTDIR)\RegInst.sbr"
	-@erase "$(INTDIR)\sgp.obj"
	-@erase "$(INTDIR)\sgp.sbr"
	-@erase "$(INTDIR)\shading.obj"
	-@erase "$(INTDIR)\shading.sbr"
	-@erase "$(INTDIR)\soundman.obj"
	-@erase "$(INTDIR)\soundman.sbr"
	-@erase "$(INTDIR)\STCI.obj"
	-@erase "$(INTDIR)\STCI.sbr"
	-@erase "$(INTDIR)\timer.obj"
	-@erase "$(INTDIR)\timer.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\video.obj"
	-@erase "$(INTDIR)\video.sbr"
	-@erase "$(INTDIR)\vobject.obj"
	-@erase "$(INTDIR)\vobject.sbr"
	-@erase "$(INTDIR)\vobject_blitters.obj"
	-@erase "$(INTDIR)\vobject_blitters.sbr"
	-@erase "$(INTDIR)\vsurface.obj"
	-@erase "$(INTDIR)\vsurface.sbr"
	-@erase "$(INTDIR)\WinFont.obj"
	-@erase "$(INTDIR)\WinFont.sbr"
	-@erase "$(OUTDIR)\Standard Gaming Platform.bsc"
	-@erase "$(OUTDIR)\Standard Gaming Platform.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Build" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I "..\\" /I ".\\" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Standard Gaming Platform.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Button Sound Control.sbr" \
	"$(INTDIR)\Button System.sbr" \
	"$(INTDIR)\Container.sbr" \
	"$(INTDIR)\Cursor Control.sbr" \
	"$(INTDIR)\DbMan.sbr" \
	"$(INTDIR)\DEBUG.sbr" \
	"$(INTDIR)\DirectDraw Calls.sbr" \
	"$(INTDIR)\DirectX Common.sbr" \
	"$(INTDIR)\English.sbr" \
	"$(INTDIR)\ExceptionHandling.sbr" \
	"$(INTDIR)\FileMan.sbr" \
	"$(INTDIR)\Font.sbr" \
	"$(INTDIR)\himage.sbr" \
	"$(INTDIR)\impTGA.sbr" \
	"$(INTDIR)\input.sbr" \
	"$(INTDIR)\Install.sbr" \
	"$(INTDIR)\LibraryDataBase.sbr" \
	"$(INTDIR)\line.sbr" \
	"$(INTDIR)\MemMan.sbr" \
	"$(INTDIR)\mousesystem.sbr" \
	"$(INTDIR)\Mutex Manager.sbr" \
	"$(INTDIR)\PCX.sbr" \
	"$(INTDIR)\Random.sbr" \
	"$(INTDIR)\RegInst.sbr" \
	"$(INTDIR)\sgp.sbr" \
	"$(INTDIR)\shading.sbr" \
	"$(INTDIR)\soundman.sbr" \
	"$(INTDIR)\STCI.sbr" \
	"$(INTDIR)\timer.sbr" \
	"$(INTDIR)\video.sbr" \
	"$(INTDIR)\vobject.sbr" \
	"$(INTDIR)\vobject_blitters.sbr" \
	"$(INTDIR)\vsurface.sbr" \
	"$(INTDIR)\WinFont.sbr"

"$(OUTDIR)\Standard Gaming Platform.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Standard Gaming Platform.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Button Sound Control.obj" \
	"$(INTDIR)\Button System.obj" \
	"$(INTDIR)\Container.obj" \
	"$(INTDIR)\Cursor Control.obj" \
	"$(INTDIR)\DbMan.obj" \
	"$(INTDIR)\DEBUG.obj" \
	"$(INTDIR)\DirectDraw Calls.obj" \
	"$(INTDIR)\DirectX Common.obj" \
	"$(INTDIR)\English.obj" \
	"$(INTDIR)\ExceptionHandling.obj" \
	"$(INTDIR)\FileMan.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\himage.obj" \
	"$(INTDIR)\impTGA.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\Install.obj" \
	"$(INTDIR)\LibraryDataBase.obj" \
	"$(INTDIR)\line.obj" \
	"$(INTDIR)\MemMan.obj" \
	"$(INTDIR)\mousesystem.obj" \
	"$(INTDIR)\Mutex Manager.obj" \
	"$(INTDIR)\PCX.obj" \
	"$(INTDIR)\Random.obj" \
	"$(INTDIR)\RegInst.obj" \
	"$(INTDIR)\sgp.obj" \
	"$(INTDIR)\shading.obj" \
	"$(INTDIR)\soundman.obj" \
	"$(INTDIR)\STCI.obj" \
	"$(INTDIR)\timer.obj" \
	"$(INTDIR)\video.obj" \
	"$(INTDIR)\vobject.obj" \
	"$(INTDIR)\vobject_blitters.obj" \
	"$(INTDIR)\vsurface.obj" \
	"$(INTDIR)\WinFont.obj" \
	".\ddraw.lib"

"$(OUTDIR)\Standard Gaming Platform.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug Info
INTDIR=.\Release with Debug Info
# Begin Custom Macros
OutDir=.\Release with Debug Info
# End Custom Macros

ALL : "$(OUTDIR)\Standard Gaming Platform.lib"


CLEAN :
	-@erase "$(INTDIR)\Button Sound Control.obj"
	-@erase "$(INTDIR)\Button System.obj"
	-@erase "$(INTDIR)\Container.obj"
	-@erase "$(INTDIR)\Cursor Control.obj"
	-@erase "$(INTDIR)\DbMan.obj"
	-@erase "$(INTDIR)\DEBUG.obj"
	-@erase "$(INTDIR)\DirectDraw Calls.obj"
	-@erase "$(INTDIR)\DirectX Common.obj"
	-@erase "$(INTDIR)\English.obj"
	-@erase "$(INTDIR)\ExceptionHandling.obj"
	-@erase "$(INTDIR)\FileMan.obj"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\himage.obj"
	-@erase "$(INTDIR)\impTGA.obj"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\Install.obj"
	-@erase "$(INTDIR)\LibraryDataBase.obj"
	-@erase "$(INTDIR)\line.obj"
	-@erase "$(INTDIR)\MemMan.obj"
	-@erase "$(INTDIR)\mousesystem.obj"
	-@erase "$(INTDIR)\Mutex Manager.obj"
	-@erase "$(INTDIR)\PCX.obj"
	-@erase "$(INTDIR)\Random.obj"
	-@erase "$(INTDIR)\RegInst.obj"
	-@erase "$(INTDIR)\sgp.obj"
	-@erase "$(INTDIR)\shading.obj"
	-@erase "$(INTDIR)\soundman.obj"
	-@erase "$(INTDIR)\STCI.obj"
	-@erase "$(INTDIR)\timer.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\video.obj"
	-@erase "$(INTDIR)\vobject.obj"
	-@erase "$(INTDIR)\vobject_blitters.obj"
	-@erase "$(INTDIR)\vsurface.obj"
	-@erase "$(INTDIR)\WinFont.obj"
	-@erase "$(OUTDIR)\Standard Gaming Platform.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Standard Gaming Platform.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Standard Gaming Platform.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Button Sound Control.obj" \
	"$(INTDIR)\Button System.obj" \
	"$(INTDIR)\Container.obj" \
	"$(INTDIR)\Cursor Control.obj" \
	"$(INTDIR)\DbMan.obj" \
	"$(INTDIR)\DEBUG.obj" \
	"$(INTDIR)\DirectDraw Calls.obj" \
	"$(INTDIR)\DirectX Common.obj" \
	"$(INTDIR)\English.obj" \
	"$(INTDIR)\ExceptionHandling.obj" \
	"$(INTDIR)\FileMan.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\himage.obj" \
	"$(INTDIR)\impTGA.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\Install.obj" \
	"$(INTDIR)\LibraryDataBase.obj" \
	"$(INTDIR)\line.obj" \
	"$(INTDIR)\MemMan.obj" \
	"$(INTDIR)\mousesystem.obj" \
	"$(INTDIR)\Mutex Manager.obj" \
	"$(INTDIR)\PCX.obj" \
	"$(INTDIR)\Random.obj" \
	"$(INTDIR)\RegInst.obj" \
	"$(INTDIR)\sgp.obj" \
	"$(INTDIR)\shading.obj" \
	"$(INTDIR)\soundman.obj" \
	"$(INTDIR)\STCI.obj" \
	"$(INTDIR)\timer.obj" \
	"$(INTDIR)\video.obj" \
	"$(INTDIR)\vobject.obj" \
	"$(INTDIR)\vobject_blitters.obj" \
	"$(INTDIR)\vsurface.obj" \
	"$(INTDIR)\WinFont.obj" \
	".\ddraw.lib"

"$(OUTDIR)\Standard Gaming Platform.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"

OUTDIR=.\Standar0
INTDIR=.\Standar0
# Begin Custom Macros
OutDir=.\Standar0
# End Custom Macros

ALL : "$(OUTDIR)\Standard Gaming Platform.lib" "$(OUTDIR)\Standard Gaming Platform.bsc"


CLEAN :
	-@erase "$(INTDIR)\Button Sound Control.obj"
	-@erase "$(INTDIR)\Button Sound Control.sbr"
	-@erase "$(INTDIR)\Button System.obj"
	-@erase "$(INTDIR)\Button System.sbr"
	-@erase "$(INTDIR)\Container.obj"
	-@erase "$(INTDIR)\Container.sbr"
	-@erase "$(INTDIR)\Cursor Control.obj"
	-@erase "$(INTDIR)\Cursor Control.sbr"
	-@erase "$(INTDIR)\DbMan.obj"
	-@erase "$(INTDIR)\DbMan.sbr"
	-@erase "$(INTDIR)\DEBUG.obj"
	-@erase "$(INTDIR)\DEBUG.sbr"
	-@erase "$(INTDIR)\DirectDraw Calls.obj"
	-@erase "$(INTDIR)\DirectDraw Calls.sbr"
	-@erase "$(INTDIR)\DirectX Common.obj"
	-@erase "$(INTDIR)\DirectX Common.sbr"
	-@erase "$(INTDIR)\English.obj"
	-@erase "$(INTDIR)\English.sbr"
	-@erase "$(INTDIR)\ExceptionHandling.obj"
	-@erase "$(INTDIR)\ExceptionHandling.sbr"
	-@erase "$(INTDIR)\FileMan.obj"
	-@erase "$(INTDIR)\FileMan.sbr"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\Font.sbr"
	-@erase "$(INTDIR)\himage.obj"
	-@erase "$(INTDIR)\himage.sbr"
	-@erase "$(INTDIR)\impTGA.obj"
	-@erase "$(INTDIR)\impTGA.sbr"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\input.sbr"
	-@erase "$(INTDIR)\Install.obj"
	-@erase "$(INTDIR)\Install.sbr"
	-@erase "$(INTDIR)\LibraryDataBase.obj"
	-@erase "$(INTDIR)\LibraryDataBase.sbr"
	-@erase "$(INTDIR)\line.obj"
	-@erase "$(INTDIR)\line.sbr"
	-@erase "$(INTDIR)\MemMan.obj"
	-@erase "$(INTDIR)\MemMan.sbr"
	-@erase "$(INTDIR)\mousesystem.obj"
	-@erase "$(INTDIR)\mousesystem.sbr"
	-@erase "$(INTDIR)\Mutex Manager.obj"
	-@erase "$(INTDIR)\Mutex Manager.sbr"
	-@erase "$(INTDIR)\PCX.obj"
	-@erase "$(INTDIR)\PCX.sbr"
	-@erase "$(INTDIR)\Random.obj"
	-@erase "$(INTDIR)\Random.sbr"
	-@erase "$(INTDIR)\RegInst.obj"
	-@erase "$(INTDIR)\RegInst.sbr"
	-@erase "$(INTDIR)\sgp.obj"
	-@erase "$(INTDIR)\sgp.sbr"
	-@erase "$(INTDIR)\shading.obj"
	-@erase "$(INTDIR)\shading.sbr"
	-@erase "$(INTDIR)\soundman.obj"
	-@erase "$(INTDIR)\soundman.sbr"
	-@erase "$(INTDIR)\STCI.obj"
	-@erase "$(INTDIR)\STCI.sbr"
	-@erase "$(INTDIR)\timer.obj"
	-@erase "$(INTDIR)\timer.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\video.obj"
	-@erase "$(INTDIR)\video.sbr"
	-@erase "$(INTDIR)\vobject.obj"
	-@erase "$(INTDIR)\vobject.sbr"
	-@erase "$(INTDIR)\vobject_blitters.obj"
	-@erase "$(INTDIR)\vobject_blitters.sbr"
	-@erase "$(INTDIR)\vsurface.obj"
	-@erase "$(INTDIR)\vsurface.sbr"
	-@erase "$(INTDIR)\WinFont.obj"
	-@erase "$(INTDIR)\WinFont.sbr"
	-@erase "$(OUTDIR)\Standard Gaming Platform.bsc"
	-@erase "$(OUTDIR)\Standard Gaming Platform.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Standard Gaming Platform.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Button Sound Control.sbr" \
	"$(INTDIR)\Button System.sbr" \
	"$(INTDIR)\Container.sbr" \
	"$(INTDIR)\Cursor Control.sbr" \
	"$(INTDIR)\DbMan.sbr" \
	"$(INTDIR)\DEBUG.sbr" \
	"$(INTDIR)\DirectDraw Calls.sbr" \
	"$(INTDIR)\DirectX Common.sbr" \
	"$(INTDIR)\English.sbr" \
	"$(INTDIR)\ExceptionHandling.sbr" \
	"$(INTDIR)\FileMan.sbr" \
	"$(INTDIR)\Font.sbr" \
	"$(INTDIR)\himage.sbr" \
	"$(INTDIR)\impTGA.sbr" \
	"$(INTDIR)\input.sbr" \
	"$(INTDIR)\Install.sbr" \
	"$(INTDIR)\LibraryDataBase.sbr" \
	"$(INTDIR)\line.sbr" \
	"$(INTDIR)\MemMan.sbr" \
	"$(INTDIR)\mousesystem.sbr" \
	"$(INTDIR)\Mutex Manager.sbr" \
	"$(INTDIR)\PCX.sbr" \
	"$(INTDIR)\Random.sbr" \
	"$(INTDIR)\RegInst.sbr" \
	"$(INTDIR)\sgp.sbr" \
	"$(INTDIR)\shading.sbr" \
	"$(INTDIR)\soundman.sbr" \
	"$(INTDIR)\STCI.sbr" \
	"$(INTDIR)\timer.sbr" \
	"$(INTDIR)\video.sbr" \
	"$(INTDIR)\vobject.sbr" \
	"$(INTDIR)\vobject_blitters.sbr" \
	"$(INTDIR)\vsurface.sbr" \
	"$(INTDIR)\WinFont.sbr"

"$(OUTDIR)\Standard Gaming Platform.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Standard Gaming Platform.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Button Sound Control.obj" \
	"$(INTDIR)\Button System.obj" \
	"$(INTDIR)\Container.obj" \
	"$(INTDIR)\Cursor Control.obj" \
	"$(INTDIR)\DbMan.obj" \
	"$(INTDIR)\DEBUG.obj" \
	"$(INTDIR)\DirectDraw Calls.obj" \
	"$(INTDIR)\DirectX Common.obj" \
	"$(INTDIR)\English.obj" \
	"$(INTDIR)\ExceptionHandling.obj" \
	"$(INTDIR)\FileMan.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\himage.obj" \
	"$(INTDIR)\impTGA.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\Install.obj" \
	"$(INTDIR)\LibraryDataBase.obj" \
	"$(INTDIR)\line.obj" \
	"$(INTDIR)\MemMan.obj" \
	"$(INTDIR)\mousesystem.obj" \
	"$(INTDIR)\Mutex Manager.obj" \
	"$(INTDIR)\PCX.obj" \
	"$(INTDIR)\Random.obj" \
	"$(INTDIR)\RegInst.obj" \
	"$(INTDIR)\sgp.obj" \
	"$(INTDIR)\shading.obj" \
	"$(INTDIR)\soundman.obj" \
	"$(INTDIR)\STCI.obj" \
	"$(INTDIR)\timer.obj" \
	"$(INTDIR)\video.obj" \
	"$(INTDIR)\vobject.obj" \
	"$(INTDIR)\vobject_blitters.obj" \
	"$(INTDIR)\vsurface.obj" \
	"$(INTDIR)\WinFont.obj" \
	".\ddraw.lib"

"$(OUTDIR)\Standard Gaming Platform.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"

OUTDIR=.\Standard
INTDIR=.\Standard
# Begin Custom Macros
OutDir=.\Standard
# End Custom Macros

ALL : "$(OUTDIR)\Standard Gaming Platform.lib" "$(OUTDIR)\Standard Gaming Platform.bsc"


CLEAN :
	-@erase "$(INTDIR)\Button Sound Control.obj"
	-@erase "$(INTDIR)\Button Sound Control.sbr"
	-@erase "$(INTDIR)\Button System.obj"
	-@erase "$(INTDIR)\Button System.sbr"
	-@erase "$(INTDIR)\Container.obj"
	-@erase "$(INTDIR)\Container.sbr"
	-@erase "$(INTDIR)\Cursor Control.obj"
	-@erase "$(INTDIR)\Cursor Control.sbr"
	-@erase "$(INTDIR)\DbMan.obj"
	-@erase "$(INTDIR)\DbMan.sbr"
	-@erase "$(INTDIR)\DEBUG.obj"
	-@erase "$(INTDIR)\DEBUG.sbr"
	-@erase "$(INTDIR)\DirectDraw Calls.obj"
	-@erase "$(INTDIR)\DirectDraw Calls.sbr"
	-@erase "$(INTDIR)\DirectX Common.obj"
	-@erase "$(INTDIR)\DirectX Common.sbr"
	-@erase "$(INTDIR)\English.obj"
	-@erase "$(INTDIR)\English.sbr"
	-@erase "$(INTDIR)\ExceptionHandling.obj"
	-@erase "$(INTDIR)\ExceptionHandling.sbr"
	-@erase "$(INTDIR)\FileMan.obj"
	-@erase "$(INTDIR)\FileMan.sbr"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\Font.sbr"
	-@erase "$(INTDIR)\himage.obj"
	-@erase "$(INTDIR)\himage.sbr"
	-@erase "$(INTDIR)\impTGA.obj"
	-@erase "$(INTDIR)\impTGA.sbr"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\input.sbr"
	-@erase "$(INTDIR)\Install.obj"
	-@erase "$(INTDIR)\Install.sbr"
	-@erase "$(INTDIR)\LibraryDataBase.obj"
	-@erase "$(INTDIR)\LibraryDataBase.sbr"
	-@erase "$(INTDIR)\line.obj"
	-@erase "$(INTDIR)\line.sbr"
	-@erase "$(INTDIR)\MemMan.obj"
	-@erase "$(INTDIR)\MemMan.sbr"
	-@erase "$(INTDIR)\mousesystem.obj"
	-@erase "$(INTDIR)\mousesystem.sbr"
	-@erase "$(INTDIR)\Mutex Manager.obj"
	-@erase "$(INTDIR)\Mutex Manager.sbr"
	-@erase "$(INTDIR)\PCX.obj"
	-@erase "$(INTDIR)\PCX.sbr"
	-@erase "$(INTDIR)\Random.obj"
	-@erase "$(INTDIR)\Random.sbr"
	-@erase "$(INTDIR)\RegInst.obj"
	-@erase "$(INTDIR)\RegInst.sbr"
	-@erase "$(INTDIR)\sgp.obj"
	-@erase "$(INTDIR)\sgp.sbr"
	-@erase "$(INTDIR)\shading.obj"
	-@erase "$(INTDIR)\shading.sbr"
	-@erase "$(INTDIR)\soundman.obj"
	-@erase "$(INTDIR)\soundman.sbr"
	-@erase "$(INTDIR)\STCI.obj"
	-@erase "$(INTDIR)\STCI.sbr"
	-@erase "$(INTDIR)\timer.obj"
	-@erase "$(INTDIR)\timer.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\video.obj"
	-@erase "$(INTDIR)\video.sbr"
	-@erase "$(INTDIR)\vobject.obj"
	-@erase "$(INTDIR)\vobject.sbr"
	-@erase "$(INTDIR)\vobject_blitters.obj"
	-@erase "$(INTDIR)\vobject_blitters.sbr"
	-@erase "$(INTDIR)\vsurface.obj"
	-@erase "$(INTDIR)\vsurface.sbr"
	-@erase "$(INTDIR)\WinFont.obj"
	-@erase "$(INTDIR)\WinFont.sbr"
	-@erase "$(OUTDIR)\Standard Gaming Platform.bsc"
	-@erase "$(OUTDIR)\Standard Gaming Platform.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Standard Gaming Platform.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Button Sound Control.sbr" \
	"$(INTDIR)\Button System.sbr" \
	"$(INTDIR)\Container.sbr" \
	"$(INTDIR)\Cursor Control.sbr" \
	"$(INTDIR)\DbMan.sbr" \
	"$(INTDIR)\DEBUG.sbr" \
	"$(INTDIR)\DirectDraw Calls.sbr" \
	"$(INTDIR)\DirectX Common.sbr" \
	"$(INTDIR)\English.sbr" \
	"$(INTDIR)\ExceptionHandling.sbr" \
	"$(INTDIR)\FileMan.sbr" \
	"$(INTDIR)\Font.sbr" \
	"$(INTDIR)\himage.sbr" \
	"$(INTDIR)\impTGA.sbr" \
	"$(INTDIR)\input.sbr" \
	"$(INTDIR)\Install.sbr" \
	"$(INTDIR)\LibraryDataBase.sbr" \
	"$(INTDIR)\line.sbr" \
	"$(INTDIR)\MemMan.sbr" \
	"$(INTDIR)\mousesystem.sbr" \
	"$(INTDIR)\Mutex Manager.sbr" \
	"$(INTDIR)\PCX.sbr" \
	"$(INTDIR)\Random.sbr" \
	"$(INTDIR)\RegInst.sbr" \
	"$(INTDIR)\sgp.sbr" \
	"$(INTDIR)\shading.sbr" \
	"$(INTDIR)\soundman.sbr" \
	"$(INTDIR)\STCI.sbr" \
	"$(INTDIR)\timer.sbr" \
	"$(INTDIR)\video.sbr" \
	"$(INTDIR)\vobject.sbr" \
	"$(INTDIR)\vobject_blitters.sbr" \
	"$(INTDIR)\vsurface.sbr" \
	"$(INTDIR)\WinFont.sbr"

"$(OUTDIR)\Standard Gaming Platform.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Standard Gaming Platform.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Button Sound Control.obj" \
	"$(INTDIR)\Button System.obj" \
	"$(INTDIR)\Container.obj" \
	"$(INTDIR)\Cursor Control.obj" \
	"$(INTDIR)\DbMan.obj" \
	"$(INTDIR)\DEBUG.obj" \
	"$(INTDIR)\DirectDraw Calls.obj" \
	"$(INTDIR)\DirectX Common.obj" \
	"$(INTDIR)\English.obj" \
	"$(INTDIR)\ExceptionHandling.obj" \
	"$(INTDIR)\FileMan.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\himage.obj" \
	"$(INTDIR)\impTGA.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\Install.obj" \
	"$(INTDIR)\LibraryDataBase.obj" \
	"$(INTDIR)\line.obj" \
	"$(INTDIR)\MemMan.obj" \
	"$(INTDIR)\mousesystem.obj" \
	"$(INTDIR)\Mutex Manager.obj" \
	"$(INTDIR)\PCX.obj" \
	"$(INTDIR)\Random.obj" \
	"$(INTDIR)\RegInst.obj" \
	"$(INTDIR)\sgp.obj" \
	"$(INTDIR)\shading.obj" \
	"$(INTDIR)\soundman.obj" \
	"$(INTDIR)\STCI.obj" \
	"$(INTDIR)\timer.obj" \
	"$(INTDIR)\video.obj" \
	"$(INTDIR)\vobject.obj" \
	"$(INTDIR)\vobject_blitters.obj" \
	"$(INTDIR)\vsurface.obj" \
	"$(INTDIR)\WinFont.obj" \
	".\ddraw.lib"

"$(OUTDIR)\Standard Gaming Platform.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"

OUTDIR=.\Standar1
INTDIR=.\Standar1
# Begin Custom Macros
OutDir=.\Standar1
# End Custom Macros

ALL : "$(OUTDIR)\Standard Gaming Platform.lib"


CLEAN :
	-@erase "$(INTDIR)\Button Sound Control.obj"
	-@erase "$(INTDIR)\Button System.obj"
	-@erase "$(INTDIR)\Container.obj"
	-@erase "$(INTDIR)\Cursor Control.obj"
	-@erase "$(INTDIR)\DbMan.obj"
	-@erase "$(INTDIR)\DEBUG.obj"
	-@erase "$(INTDIR)\DirectDraw Calls.obj"
	-@erase "$(INTDIR)\DirectX Common.obj"
	-@erase "$(INTDIR)\English.obj"
	-@erase "$(INTDIR)\ExceptionHandling.obj"
	-@erase "$(INTDIR)\FileMan.obj"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\himage.obj"
	-@erase "$(INTDIR)\impTGA.obj"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\Install.obj"
	-@erase "$(INTDIR)\LibraryDataBase.obj"
	-@erase "$(INTDIR)\line.obj"
	-@erase "$(INTDIR)\MemMan.obj"
	-@erase "$(INTDIR)\mousesystem.obj"
	-@erase "$(INTDIR)\Mutex Manager.obj"
	-@erase "$(INTDIR)\PCX.obj"
	-@erase "$(INTDIR)\Random.obj"
	-@erase "$(INTDIR)\RegInst.obj"
	-@erase "$(INTDIR)\sgp.obj"
	-@erase "$(INTDIR)\shading.obj"
	-@erase "$(INTDIR)\soundman.obj"
	-@erase "$(INTDIR)\STCI.obj"
	-@erase "$(INTDIR)\timer.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\video.obj"
	-@erase "$(INTDIR)\vobject.obj"
	-@erase "$(INTDIR)\vobject_blitters.obj"
	-@erase "$(INTDIR)\vsurface.obj"
	-@erase "$(INTDIR)\WinFont.obj"
	-@erase "$(OUTDIR)\Standard Gaming Platform.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Standard Gaming Platform.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Standard Gaming Platform.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Button Sound Control.obj" \
	"$(INTDIR)\Button System.obj" \
	"$(INTDIR)\Container.obj" \
	"$(INTDIR)\Cursor Control.obj" \
	"$(INTDIR)\DbMan.obj" \
	"$(INTDIR)\DEBUG.obj" \
	"$(INTDIR)\DirectDraw Calls.obj" \
	"$(INTDIR)\DirectX Common.obj" \
	"$(INTDIR)\English.obj" \
	"$(INTDIR)\ExceptionHandling.obj" \
	"$(INTDIR)\FileMan.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\himage.obj" \
	"$(INTDIR)\impTGA.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\Install.obj" \
	"$(INTDIR)\LibraryDataBase.obj" \
	"$(INTDIR)\line.obj" \
	"$(INTDIR)\MemMan.obj" \
	"$(INTDIR)\mousesystem.obj" \
	"$(INTDIR)\Mutex Manager.obj" \
	"$(INTDIR)\PCX.obj" \
	"$(INTDIR)\Random.obj" \
	"$(INTDIR)\RegInst.obj" \
	"$(INTDIR)\sgp.obj" \
	"$(INTDIR)\shading.obj" \
	"$(INTDIR)\soundman.obj" \
	"$(INTDIR)\STCI.obj" \
	"$(INTDIR)\timer.obj" \
	"$(INTDIR)\video.obj" \
	"$(INTDIR)\vobject.obj" \
	"$(INTDIR)\vobject_blitters.obj" \
	"$(INTDIR)\vsurface.obj" \
	"$(INTDIR)\WinFont.obj" \
	".\ddraw.lib"

"$(OUTDIR)\Standard Gaming Platform.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"

OUTDIR=.\Standar2
INTDIR=.\Standar2
# Begin Custom Macros
OutDir=.\Standar2
# End Custom Macros

ALL : "$(OUTDIR)\Standard Gaming Platform.lib"


CLEAN :
	-@erase "$(INTDIR)\Button Sound Control.obj"
	-@erase "$(INTDIR)\Button System.obj"
	-@erase "$(INTDIR)\Container.obj"
	-@erase "$(INTDIR)\Cursor Control.obj"
	-@erase "$(INTDIR)\DbMan.obj"
	-@erase "$(INTDIR)\DEBUG.obj"
	-@erase "$(INTDIR)\DirectDraw Calls.obj"
	-@erase "$(INTDIR)\DirectX Common.obj"
	-@erase "$(INTDIR)\English.obj"
	-@erase "$(INTDIR)\ExceptionHandling.obj"
	-@erase "$(INTDIR)\FileMan.obj"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\himage.obj"
	-@erase "$(INTDIR)\impTGA.obj"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\Install.obj"
	-@erase "$(INTDIR)\LibraryDataBase.obj"
	-@erase "$(INTDIR)\line.obj"
	-@erase "$(INTDIR)\MemMan.obj"
	-@erase "$(INTDIR)\mousesystem.obj"
	-@erase "$(INTDIR)\Mutex Manager.obj"
	-@erase "$(INTDIR)\PCX.obj"
	-@erase "$(INTDIR)\Random.obj"
	-@erase "$(INTDIR)\RegInst.obj"
	-@erase "$(INTDIR)\sgp.obj"
	-@erase "$(INTDIR)\shading.obj"
	-@erase "$(INTDIR)\soundman.obj"
	-@erase "$(INTDIR)\STCI.obj"
	-@erase "$(INTDIR)\timer.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\video.obj"
	-@erase "$(INTDIR)\vobject.obj"
	-@erase "$(INTDIR)\vobject_blitters.obj"
	-@erase "$(INTDIR)\vsurface.obj"
	-@erase "$(INTDIR)\WinFont.obj"
	-@erase "$(OUTDIR)\Standard Gaming Platform.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "XML_STATIC" /D "CINTERFACE" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Standard Gaming Platform.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Standard Gaming Platform.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Button Sound Control.obj" \
	"$(INTDIR)\Button System.obj" \
	"$(INTDIR)\Container.obj" \
	"$(INTDIR)\Cursor Control.obj" \
	"$(INTDIR)\DbMan.obj" \
	"$(INTDIR)\DEBUG.obj" \
	"$(INTDIR)\DirectDraw Calls.obj" \
	"$(INTDIR)\DirectX Common.obj" \
	"$(INTDIR)\English.obj" \
	"$(INTDIR)\ExceptionHandling.obj" \
	"$(INTDIR)\FileMan.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\himage.obj" \
	"$(INTDIR)\impTGA.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\Install.obj" \
	"$(INTDIR)\LibraryDataBase.obj" \
	"$(INTDIR)\line.obj" \
	"$(INTDIR)\MemMan.obj" \
	"$(INTDIR)\mousesystem.obj" \
	"$(INTDIR)\Mutex Manager.obj" \
	"$(INTDIR)\PCX.obj" \
	"$(INTDIR)\Random.obj" \
	"$(INTDIR)\RegInst.obj" \
	"$(INTDIR)\sgp.obj" \
	"$(INTDIR)\shading.obj" \
	"$(INTDIR)\soundman.obj" \
	"$(INTDIR)\STCI.obj" \
	"$(INTDIR)\timer.obj" \
	"$(INTDIR)\video.obj" \
	"$(INTDIR)\vobject.obj" \
	"$(INTDIR)\vobject_blitters.obj" \
	"$(INTDIR)\vsurface.obj" \
	"$(INTDIR)\WinFont.obj" \
	".\ddraw.lib"

"$(OUTDIR)\Standard Gaming Platform.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Standard Gaming Platform.dep")
!INCLUDE "Standard Gaming Platform.dep"
!ELSE 
!MESSAGE Warning: cannot find "Standard Gaming Platform.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Standard Gaming Platform - Win32 Release" || "$(CFG)" == "Standard Gaming Platform - Win32 Debug" || "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info" || "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker" || "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo" || "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo" || "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"
SOURCE=".\Button Sound Control.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Button Sound Control.obj"	"$(INTDIR)\Button Sound Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Button Sound Control.obj"	"$(INTDIR)\Button Sound Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Button Sound Control.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Button Sound Control.obj"	"$(INTDIR)\Button Sound Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Button Sound Control.obj"	"$(INTDIR)\Button Sound Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Button Sound Control.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Button Sound Control.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\Button System.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Button System.obj"	"$(INTDIR)\Button System.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Button System.obj"	"$(INTDIR)\Button System.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Button System.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Button System.obj"	"$(INTDIR)\Button System.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Button System.obj"	"$(INTDIR)\Button System.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Button System.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Button System.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\Container.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Container.obj"	"$(INTDIR)\Container.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Container.obj"	"$(INTDIR)\Container.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Container.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Container.obj"	"$(INTDIR)\Container.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Container.obj"	"$(INTDIR)\Container.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Container.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Container.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\Cursor Control.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Cursor Control.obj"	"$(INTDIR)\Cursor Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Cursor Control.obj"	"$(INTDIR)\Cursor Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Cursor Control.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Cursor Control.obj"	"$(INTDIR)\Cursor Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Cursor Control.obj"	"$(INTDIR)\Cursor Control.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Cursor Control.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Cursor Control.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\DbMan.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\DbMan.obj"	"$(INTDIR)\DbMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\DbMan.obj"	"$(INTDIR)\DbMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\DbMan.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\DbMan.obj"	"$(INTDIR)\DbMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\DbMan.obj"	"$(INTDIR)\DbMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\DbMan.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\DbMan.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\DEBUG.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NO_ZLIB" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /D "_DEBUG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DEBUG.obj"	"$(INTDIR)\DEBUG.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Build" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I "..\\" /I ".\\" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DEBUG.obj"	"$(INTDIR)\DEBUG.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /D "_DEBUG" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DEBUG.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DEBUG.obj"	"$(INTDIR)\DEBUG.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DEBUG.obj"	"$(INTDIR)\DEBUG.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_DEBUG" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DEBUG.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "XML_STATIC" /D "CINTERFACE" /D "_DEBUG" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DEBUG.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=".\DirectDraw Calls.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\DirectDraw Calls.obj"	"$(INTDIR)\DirectDraw Calls.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\DirectDraw Calls.obj"	"$(INTDIR)\DirectDraw Calls.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\DirectDraw Calls.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\DirectDraw Calls.obj"	"$(INTDIR)\DirectDraw Calls.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\DirectDraw Calls.obj"	"$(INTDIR)\DirectDraw Calls.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\DirectDraw Calls.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\DirectDraw Calls.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\DirectX Common.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\DirectX Common.obj"	"$(INTDIR)\DirectX Common.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\DirectX Common.obj"	"$(INTDIR)\DirectX Common.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\DirectX Common.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\DirectX Common.obj"	"$(INTDIR)\DirectX Common.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\DirectX Common.obj"	"$(INTDIR)\DirectX Common.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\DirectX Common.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\DirectX Common.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\English.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\English.obj"	"$(INTDIR)\English.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\English.obj"	"$(INTDIR)\English.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\English.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\English.obj"	"$(INTDIR)\English.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\English.obj"	"$(INTDIR)\English.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\English.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\English.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ExceptionHandling.cpp

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\ExceptionHandling.obj"	"$(INTDIR)\ExceptionHandling.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\ExceptionHandling.obj"	"$(INTDIR)\ExceptionHandling.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\ExceptionHandling.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\ExceptionHandling.obj"	"$(INTDIR)\ExceptionHandling.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\ExceptionHandling.obj"	"$(INTDIR)\ExceptionHandling.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\ExceptionHandling.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\ExceptionHandling.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\FileMan.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\FileMan.obj"	"$(INTDIR)\FileMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\FileMan.obj"	"$(INTDIR)\FileMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\FileMan.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\FileMan.obj"	"$(INTDIR)\FileMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\FileMan.obj"	"$(INTDIR)\FileMan.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\FileMan.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\FileMan.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\Font.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Font.obj"	"$(INTDIR)\Font.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Font.obj"	"$(INTDIR)\Font.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Font.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Font.obj"	"$(INTDIR)\Font.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Font.obj"	"$(INTDIR)\Font.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Font.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Font.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\himage.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\himage.obj"	"$(INTDIR)\himage.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\himage.obj"	"$(INTDIR)\himage.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\himage.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\himage.obj"	"$(INTDIR)\himage.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\himage.obj"	"$(INTDIR)\himage.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\himage.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\himage.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\impTGA.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\impTGA.obj"	"$(INTDIR)\impTGA.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\impTGA.obj"	"$(INTDIR)\impTGA.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\impTGA.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\impTGA.obj"	"$(INTDIR)\impTGA.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\impTGA.obj"	"$(INTDIR)\impTGA.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\impTGA.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\impTGA.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\input.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\input.obj"	"$(INTDIR)\input.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\input.obj"	"$(INTDIR)\input.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\input.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\input.obj"	"$(INTDIR)\input.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\input.obj"	"$(INTDIR)\input.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\input.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\input.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\Install.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Install.obj"	"$(INTDIR)\Install.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Install.obj"	"$(INTDIR)\Install.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Install.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Install.obj"	"$(INTDIR)\Install.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Install.obj"	"$(INTDIR)\Install.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Install.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Install.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\LibraryDataBase.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\LibraryDataBase.obj"	"$(INTDIR)\LibraryDataBase.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\LibraryDataBase.obj"	"$(INTDIR)\LibraryDataBase.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\LibraryDataBase.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\LibraryDataBase.obj"	"$(INTDIR)\LibraryDataBase.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\LibraryDataBase.obj"	"$(INTDIR)\LibraryDataBase.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\LibraryDataBase.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\LibraryDataBase.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\line.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\line.obj"	"$(INTDIR)\line.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\line.obj"	"$(INTDIR)\line.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\line.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\line.obj"	"$(INTDIR)\line.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\line.obj"	"$(INTDIR)\line.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\line.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\line.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\MemMan.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NO_ZLIB" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MemMan.obj"	"$(INTDIR)\MemMan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Build" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I "..\\" /I ".\\" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /D "_MEMMAN_DEBUG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MemMan.obj"	"$(INTDIR)\MemMan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MemMan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_VTUNE_PROFILING" /D "_MEMMAN_DEBUG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MemMan.obj"	"$(INTDIR)\MemMan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "_MEMMAN_DEBUG" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MemMan.obj"	"$(INTDIR)\MemMan.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\build" /I "\ja2\build\TileEngine" /I "\ja2\build\Tactical" /I "\ja2\build\Utils" /I "\ja2\build\strategic" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MemMan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\\" /I "..\TileEngine" /I "..\Tactical" /I "..\Utils" /I "..\strategic" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "NO_ZLIB" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "JA2_PRECOMPILED_HEADERS" /D "NO_ZLIB_COMPRESSION" /D "XML_STATIC" /D "CINTERFACE" /Fp"$(INTDIR)\Standard Gaming Platform.pch" /YX"JA2 SGP ALL.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MemMan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=".\mousesystem.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\mousesystem.obj"	"$(INTDIR)\mousesystem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\mousesystem.obj"	"$(INTDIR)\mousesystem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\mousesystem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\mousesystem.obj"	"$(INTDIR)\mousesystem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\mousesystem.obj"	"$(INTDIR)\mousesystem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\mousesystem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\mousesystem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\Mutex Manager.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Mutex Manager.obj"	"$(INTDIR)\Mutex Manager.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Mutex Manager.obj"	"$(INTDIR)\Mutex Manager.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Mutex Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Mutex Manager.obj"	"$(INTDIR)\Mutex Manager.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Mutex Manager.obj"	"$(INTDIR)\Mutex Manager.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Mutex Manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Mutex Manager.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\PCX.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\PCX.obj"	"$(INTDIR)\PCX.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\PCX.obj"	"$(INTDIR)\PCX.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\PCX.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\PCX.obj"	"$(INTDIR)\PCX.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\PCX.obj"	"$(INTDIR)\PCX.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\PCX.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\PCX.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\Random.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\Random.obj"	"$(INTDIR)\Random.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\Random.obj"	"$(INTDIR)\Random.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\Random.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\Random.obj"	"$(INTDIR)\Random.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\Random.obj"	"$(INTDIR)\Random.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\Random.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\Random.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\RegInst.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\RegInst.obj"	"$(INTDIR)\RegInst.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\RegInst.obj"	"$(INTDIR)\RegInst.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\RegInst.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\RegInst.obj"	"$(INTDIR)\RegInst.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\RegInst.obj"	"$(INTDIR)\RegInst.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\RegInst.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\RegInst.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\sgp.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\sgp.obj"	"$(INTDIR)\sgp.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\sgp.obj"	"$(INTDIR)\sgp.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\sgp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\sgp.obj"	"$(INTDIR)\sgp.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\sgp.obj"	"$(INTDIR)\sgp.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\sgp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\sgp.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\shading.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\shading.obj"	"$(INTDIR)\shading.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\shading.obj"	"$(INTDIR)\shading.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\shading.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\shading.obj"	"$(INTDIR)\shading.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\shading.obj"	"$(INTDIR)\shading.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\shading.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\shading.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\soundman.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\soundman.obj"	"$(INTDIR)\soundman.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\soundman.obj"	"$(INTDIR)\soundman.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\soundman.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\soundman.obj"	"$(INTDIR)\soundman.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\soundman.obj"	"$(INTDIR)\soundman.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\soundman.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\soundman.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\STCI.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\STCI.obj"	"$(INTDIR)\STCI.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\STCI.obj"	"$(INTDIR)\STCI.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\STCI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\STCI.obj"	"$(INTDIR)\STCI.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\STCI.obj"	"$(INTDIR)\STCI.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\STCI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\STCI.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\timer.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\timer.obj"	"$(INTDIR)\timer.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\timer.obj"	"$(INTDIR)\timer.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\timer.obj"	"$(INTDIR)\timer.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\timer.obj"	"$(INTDIR)\timer.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\timer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\timer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\video.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\video.obj"	"$(INTDIR)\video.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\video.obj"	"$(INTDIR)\video.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\video.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\video.obj"	"$(INTDIR)\video.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\video.obj"	"$(INTDIR)\video.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\video.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\video.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\vobject.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\vobject.obj"	"$(INTDIR)\vobject.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\vobject.obj"	"$(INTDIR)\vobject.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\vobject.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\vobject.obj"	"$(INTDIR)\vobject.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\vobject.obj"	"$(INTDIR)\vobject.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\vobject.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\vobject.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\vobject_blitters.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\vobject_blitters.obj"	"$(INTDIR)\vobject_blitters.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\vobject_blitters.obj"	"$(INTDIR)\vobject_blitters.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\vobject_blitters.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\vobject_blitters.obj"	"$(INTDIR)\vobject_blitters.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\vobject_blitters.obj"	"$(INTDIR)\vobject_blitters.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\vobject_blitters.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\vobject_blitters.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\vsurface.cpp"

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\vsurface.obj"	"$(INTDIR)\vsurface.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\vsurface.obj"	"$(INTDIR)\vsurface.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\vsurface.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\vsurface.obj"	"$(INTDIR)\vsurface.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\vsurface.obj"	"$(INTDIR)\vsurface.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\vsurface.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\vsurface.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\WinFont.cpp

!IF  "$(CFG)" == "Standard Gaming Platform - Win32 Release"


"$(INTDIR)\WinFont.obj"	"$(INTDIR)\WinFont.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug"


"$(INTDIR)\WinFont.obj"	"$(INTDIR)\WinFont.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release with Debug Info"


"$(INTDIR)\WinFont.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Bounds Checker"


"$(INTDIR)\WinFont.obj"	"$(INTDIR)\WinFont.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Debug Demo"


"$(INTDIR)\WinFont.obj"	"$(INTDIR)\WinFont.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Release Demo"


"$(INTDIR)\WinFont.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Standard Gaming Platform - Win32 Demo Release with Debug Info"


"$(INTDIR)\WinFont.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

