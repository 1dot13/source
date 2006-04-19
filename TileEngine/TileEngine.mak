# Microsoft Developer Studio Generated NMAKE File, Based on TileEngine.dsp
!IF "$(CFG)" == ""
CFG=TileEngine - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to TileEngine - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "TileEngine - Win32 Release" && "$(CFG)" != "TileEngine - Win32 Debug" && "$(CFG)" != "TileEngine - Win32 Release with Debug Info" && "$(CFG)" != "TileEngine - Win32 Bounds Checker" && "$(CFG)" != "TileEngine - Win32 Debug Demo" && "$(CFG)" != "TileEngine - Win32 Release Demo" && "$(CFG)" != "TileEngine - Win32 Demo Release with Debug Info" && "$(CFG)" != "TileEngine - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TileEngine.mak" CFG="TileEngine - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TileEngine - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TileEngine - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "TileEngine - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "TileEngine - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "TileEngine - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "TileEngine - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "TileEngine - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "TileEngine - Win32 Demo Bounds Checker" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "TileEngine - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TileEngine - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TileEngine - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TileEngine - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TileEngine - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TileEngine - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\build\tacticalai" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TileEngine - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TileEngine - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\TileEngine.lib" "$(OUTDIR)\TileEngine.bsc"


CLEAN :
	-@erase "$(INTDIR)\Ambient Control.obj"
	-@erase "$(INTDIR)\Ambient Control.sbr"
	-@erase "$(INTDIR)\Buildings.obj"
	-@erase "$(INTDIR)\Buildings.sbr"
	-@erase "$(INTDIR)\environment.obj"
	-@erase "$(INTDIR)\environment.sbr"
	-@erase "$(INTDIR)\Exit Grids.obj"
	-@erase "$(INTDIR)\Exit Grids.sbr"
	-@erase "$(INTDIR)\Explosion Control.obj"
	-@erase "$(INTDIR)\Explosion Control.sbr"
	-@erase "$(INTDIR)\Fog Of War.obj"
	-@erase "$(INTDIR)\Fog Of War.sbr"
	-@erase "$(INTDIR)\Interactive Tiles.obj"
	-@erase "$(INTDIR)\Interactive Tiles.sbr"
	-@erase "$(INTDIR)\Isometric Utils.obj"
	-@erase "$(INTDIR)\Isometric Utils.sbr"
	-@erase "$(INTDIR)\LightEffects.obj"
	-@erase "$(INTDIR)\LightEffects.sbr"
	-@erase "$(INTDIR)\lighting.obj"
	-@erase "$(INTDIR)\lighting.sbr"
	-@erase "$(INTDIR)\Map Edgepoints.obj"
	-@erase "$(INTDIR)\Map Edgepoints.sbr"
	-@erase "$(INTDIR)\overhead map.obj"
	-@erase "$(INTDIR)\overhead map.sbr"
	-@erase "$(INTDIR)\phys math.obj"
	-@erase "$(INTDIR)\phys math.sbr"
	-@erase "$(INTDIR)\physics.obj"
	-@erase "$(INTDIR)\physics.sbr"
	-@erase "$(INTDIR)\pits.obj"
	-@erase "$(INTDIR)\pits.sbr"
	-@erase "$(INTDIR)\Radar Screen.obj"
	-@erase "$(INTDIR)\Radar Screen.sbr"
	-@erase "$(INTDIR)\Render Dirty.obj"
	-@erase "$(INTDIR)\Render Dirty.sbr"
	-@erase "$(INTDIR)\Render Fun.obj"
	-@erase "$(INTDIR)\Render Fun.sbr"
	-@erase "$(INTDIR)\renderworld.obj"
	-@erase "$(INTDIR)\renderworld.sbr"
	-@erase "$(INTDIR)\SaveLoadMap.obj"
	-@erase "$(INTDIR)\SaveLoadMap.sbr"
	-@erase "$(INTDIR)\Shade Table Util.obj"
	-@erase "$(INTDIR)\Shade Table Util.sbr"
	-@erase "$(INTDIR)\Simple Render Utils.obj"
	-@erase "$(INTDIR)\Simple Render Utils.sbr"
	-@erase "$(INTDIR)\Smell.obj"
	-@erase "$(INTDIR)\Smell.sbr"
	-@erase "$(INTDIR)\SmokeEffects.obj"
	-@erase "$(INTDIR)\SmokeEffects.sbr"
	-@erase "$(INTDIR)\structure.obj"
	-@erase "$(INTDIR)\structure.sbr"
	-@erase "$(INTDIR)\sysutil.obj"
	-@erase "$(INTDIR)\sysutil.sbr"
	-@erase "$(INTDIR)\Tactical Placement GUI.obj"
	-@erase "$(INTDIR)\Tactical Placement GUI.sbr"
	-@erase "$(INTDIR)\Tile Animation.obj"
	-@erase "$(INTDIR)\Tile Animation.sbr"
	-@erase "$(INTDIR)\Tile Cache.obj"
	-@erase "$(INTDIR)\Tile Cache.sbr"
	-@erase "$(INTDIR)\Tile Surface.obj"
	-@erase "$(INTDIR)\Tile Surface.sbr"
	-@erase "$(INTDIR)\TileDat.obj"
	-@erase "$(INTDIR)\TileDat.sbr"
	-@erase "$(INTDIR)\tiledef.obj"
	-@erase "$(INTDIR)\tiledef.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WorldDat.obj"
	-@erase "$(INTDIR)\WorldDat.sbr"
	-@erase "$(INTDIR)\worlddef.obj"
	-@erase "$(INTDIR)\worlddef.sbr"
	-@erase "$(INTDIR)\worldman.obj"
	-@erase "$(INTDIR)\worldman.sbr"
	-@erase "$(OUTDIR)\TileEngine.bsc"
	-@erase "$(OUTDIR)\TileEngine.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TileEngine.pch" /YX"TileEngine All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TileEngine.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambient Control.sbr" \
	"$(INTDIR)\Buildings.sbr" \
	"$(INTDIR)\environment.sbr" \
	"$(INTDIR)\Exit Grids.sbr" \
	"$(INTDIR)\Explosion Control.sbr" \
	"$(INTDIR)\Fog Of War.sbr" \
	"$(INTDIR)\Interactive Tiles.sbr" \
	"$(INTDIR)\Isometric Utils.sbr" \
	"$(INTDIR)\LightEffects.sbr" \
	"$(INTDIR)\lighting.sbr" \
	"$(INTDIR)\Map Edgepoints.sbr" \
	"$(INTDIR)\overhead map.sbr" \
	"$(INTDIR)\phys math.sbr" \
	"$(INTDIR)\physics.sbr" \
	"$(INTDIR)\pits.sbr" \
	"$(INTDIR)\Radar Screen.sbr" \
	"$(INTDIR)\Render Dirty.sbr" \
	"$(INTDIR)\Render Fun.sbr" \
	"$(INTDIR)\renderworld.sbr" \
	"$(INTDIR)\SaveLoadMap.sbr" \
	"$(INTDIR)\Shade Table Util.sbr" \
	"$(INTDIR)\Simple Render Utils.sbr" \
	"$(INTDIR)\Smell.sbr" \
	"$(INTDIR)\SmokeEffects.sbr" \
	"$(INTDIR)\structure.sbr" \
	"$(INTDIR)\sysutil.sbr" \
	"$(INTDIR)\Tactical Placement GUI.sbr" \
	"$(INTDIR)\Tile Animation.sbr" \
	"$(INTDIR)\Tile Cache.sbr" \
	"$(INTDIR)\Tile Surface.sbr" \
	"$(INTDIR)\TileDat.sbr" \
	"$(INTDIR)\tiledef.sbr" \
	"$(INTDIR)\WorldDat.sbr" \
	"$(INTDIR)\worlddef.sbr" \
	"$(INTDIR)\worldman.sbr"

"$(OUTDIR)\TileEngine.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TileEngine.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Ambient Control.obj" \
	"$(INTDIR)\Buildings.obj" \
	"$(INTDIR)\environment.obj" \
	"$(INTDIR)\Exit Grids.obj" \
	"$(INTDIR)\Explosion Control.obj" \
	"$(INTDIR)\Fog Of War.obj" \
	"$(INTDIR)\Interactive Tiles.obj" \
	"$(INTDIR)\Isometric Utils.obj" \
	"$(INTDIR)\LightEffects.obj" \
	"$(INTDIR)\lighting.obj" \
	"$(INTDIR)\Map Edgepoints.obj" \
	"$(INTDIR)\overhead map.obj" \
	"$(INTDIR)\phys math.obj" \
	"$(INTDIR)\physics.obj" \
	"$(INTDIR)\pits.obj" \
	"$(INTDIR)\Radar Screen.obj" \
	"$(INTDIR)\Render Dirty.obj" \
	"$(INTDIR)\Render Fun.obj" \
	"$(INTDIR)\renderworld.obj" \
	"$(INTDIR)\SaveLoadMap.obj" \
	"$(INTDIR)\Shade Table Util.obj" \
	"$(INTDIR)\Simple Render Utils.obj" \
	"$(INTDIR)\Smell.obj" \
	"$(INTDIR)\SmokeEffects.obj" \
	"$(INTDIR)\structure.obj" \
	"$(INTDIR)\sysutil.obj" \
	"$(INTDIR)\Tactical Placement GUI.obj" \
	"$(INTDIR)\Tile Animation.obj" \
	"$(INTDIR)\Tile Cache.obj" \
	"$(INTDIR)\Tile Surface.obj" \
	"$(INTDIR)\TileDat.obj" \
	"$(INTDIR)\tiledef.obj" \
	"$(INTDIR)\WorldDat.obj" \
	"$(INTDIR)\worlddef.obj" \
	"$(INTDIR)\worldman.obj"

"$(OUTDIR)\TileEngine.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("TileEngine.dep")
!INCLUDE "TileEngine.dep"
!ELSE 
!MESSAGE Warning: cannot find "TileEngine.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TileEngine - Win32 Release" || "$(CFG)" == "TileEngine - Win32 Debug" || "$(CFG)" == "TileEngine - Win32 Release with Debug Info" || "$(CFG)" == "TileEngine - Win32 Bounds Checker" || "$(CFG)" == "TileEngine - Win32 Debug Demo" || "$(CFG)" == "TileEngine - Win32 Release Demo" || "$(CFG)" == "TileEngine - Win32 Demo Release with Debug Info" || "$(CFG)" == "TileEngine - Win32 Demo Bounds Checker"
SOURCE=".\Ambient Control.cpp"

"$(INTDIR)\Ambient Control.obj"	"$(INTDIR)\Ambient Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Buildings.cpp

"$(INTDIR)\Buildings.obj"	"$(INTDIR)\Buildings.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\environment.cpp

"$(INTDIR)\environment.obj"	"$(INTDIR)\environment.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Exit Grids.cpp"

"$(INTDIR)\Exit Grids.obj"	"$(INTDIR)\Exit Grids.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Explosion Control.cpp"

"$(INTDIR)\Explosion Control.obj"	"$(INTDIR)\Explosion Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Fog Of War.cpp"

"$(INTDIR)\Fog Of War.obj"	"$(INTDIR)\Fog Of War.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Interactive Tiles.cpp"

"$(INTDIR)\Interactive Tiles.obj"	"$(INTDIR)\Interactive Tiles.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Isometric Utils.cpp"

"$(INTDIR)\Isometric Utils.obj"	"$(INTDIR)\Isometric Utils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LightEffects.cpp

"$(INTDIR)\LightEffects.obj"	"$(INTDIR)\LightEffects.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lighting.cpp

"$(INTDIR)\lighting.obj"	"$(INTDIR)\lighting.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Edgepoints.cpp"

"$(INTDIR)\Map Edgepoints.obj"	"$(INTDIR)\Map Edgepoints.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\overhead map.cpp"

"$(INTDIR)\overhead map.obj"	"$(INTDIR)\overhead map.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\phys math.cpp"

"$(INTDIR)\phys math.obj"	"$(INTDIR)\phys math.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\physics.cpp

"$(INTDIR)\physics.obj"	"$(INTDIR)\physics.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pits.cpp

"$(INTDIR)\pits.obj"	"$(INTDIR)\pits.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Radar Screen.cpp"

"$(INTDIR)\Radar Screen.obj"	"$(INTDIR)\Radar Screen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Render Dirty.cpp"

"$(INTDIR)\Render Dirty.obj"	"$(INTDIR)\Render Dirty.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Render Fun.cpp"

"$(INTDIR)\Render Fun.obj"	"$(INTDIR)\Render Fun.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\renderworld.cpp

"$(INTDIR)\renderworld.obj"	"$(INTDIR)\renderworld.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SaveLoadMap.cpp

"$(INTDIR)\SaveLoadMap.obj"	"$(INTDIR)\SaveLoadMap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Shade Table Util.cpp"

"$(INTDIR)\Shade Table Util.obj"	"$(INTDIR)\Shade Table Util.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Simple Render Utils.cpp"

"$(INTDIR)\Simple Render Utils.obj"	"$(INTDIR)\Simple Render Utils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Smell.cpp

"$(INTDIR)\Smell.obj"	"$(INTDIR)\Smell.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SmokeEffects.cpp

"$(INTDIR)\SmokeEffects.obj"	"$(INTDIR)\SmokeEffects.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\structure.cpp

"$(INTDIR)\structure.obj"	"$(INTDIR)\structure.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sysutil.cpp

"$(INTDIR)\sysutil.obj"	"$(INTDIR)\sysutil.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Tactical Placement GUI.cpp"

"$(INTDIR)\Tactical Placement GUI.obj"	"$(INTDIR)\Tactical Placement GUI.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Tile Animation.cpp"

"$(INTDIR)\Tile Animation.obj"	"$(INTDIR)\Tile Animation.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Tile Cache.cpp"

"$(INTDIR)\Tile Cache.obj"	"$(INTDIR)\Tile Cache.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Tile Surface.cpp"

"$(INTDIR)\Tile Surface.obj"	"$(INTDIR)\Tile Surface.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TileDat.cpp

"$(INTDIR)\TileDat.obj"	"$(INTDIR)\TileDat.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tiledef.cpp

"$(INTDIR)\tiledef.obj"	"$(INTDIR)\tiledef.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WorldDat.cpp

"$(INTDIR)\WorldDat.obj"	"$(INTDIR)\WorldDat.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\worlddef.cpp

"$(INTDIR)\worlddef.obj"	"$(INTDIR)\worlddef.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\worldman.cpp

"$(INTDIR)\worldman.obj"	"$(INTDIR)\worldman.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

