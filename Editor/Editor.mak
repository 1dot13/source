# Microsoft Developer Studio Generated NMAKE File, Based on Editor.dsp
!IF "$(CFG)" == ""
CFG=Editor - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to Editor - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "Editor - Win32 Release" && "$(CFG)" != "Editor - Win32 Debug" && "$(CFG)" != "Editor - Win32 Release with Debug Info" && "$(CFG)" != "Editor - Win32 Bounds Checker" && "$(CFG)" != "Editor - Win32 Debug Demo" && "$(CFG)" != "Editor - Win32 Release Demo" && "$(CFG)" != "Editor - Win32 Demo Release with Debug Info" && "$(CFG)" != "Editor - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Editor.mak" CFG="Editor - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Editor - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Editor - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Editor - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Editor - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "Editor - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Editor - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Editor - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Editor - Win32 Demo Bounds Checker" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "Editor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /Ob2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "JA2DEMO" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Editor.lib" "$(OUTDIR)\Editor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Cursor Modes.obj"
	-@erase "$(INTDIR)\Cursor Modes.sbr"
	-@erase "$(INTDIR)\edit_sys.obj"
	-@erase "$(INTDIR)\edit_sys.sbr"
	-@erase "$(INTDIR)\Editor Callbacks.obj"
	-@erase "$(INTDIR)\Editor Callbacks.sbr"
	-@erase "$(INTDIR)\Editor Modes.obj"
	-@erase "$(INTDIR)\Editor Modes.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Creation.obj"
	-@erase "$(INTDIR)\Editor Taskbar Creation.sbr"
	-@erase "$(INTDIR)\Editor Taskbar Utils.obj"
	-@erase "$(INTDIR)\Editor Taskbar Utils.sbr"
	-@erase "$(INTDIR)\Editor Undo.obj"
	-@erase "$(INTDIR)\Editor Undo.sbr"
	-@erase "$(INTDIR)\EditorBuildings.obj"
	-@erase "$(INTDIR)\EditorBuildings.sbr"
	-@erase "$(INTDIR)\EditorItems.obj"
	-@erase "$(INTDIR)\EditorItems.sbr"
	-@erase "$(INTDIR)\EditorMapInfo.obj"
	-@erase "$(INTDIR)\EditorMapInfo.sbr"
	-@erase "$(INTDIR)\EditorMercs.obj"
	-@erase "$(INTDIR)\EditorMercs.sbr"
	-@erase "$(INTDIR)\EditorTerrain.obj"
	-@erase "$(INTDIR)\EditorTerrain.sbr"
	-@erase "$(INTDIR)\editscreen.obj"
	-@erase "$(INTDIR)\editscreen.sbr"
	-@erase "$(INTDIR)\Item Statistics.obj"
	-@erase "$(INTDIR)\Item Statistics.sbr"
	-@erase "$(INTDIR)\LoadScreen.obj"
	-@erase "$(INTDIR)\LoadScreen.sbr"
	-@erase "$(INTDIR)\messagebox.obj"
	-@erase "$(INTDIR)\messagebox.sbr"
	-@erase "$(INTDIR)\newsmooth.obj"
	-@erase "$(INTDIR)\newsmooth.sbr"
	-@erase "$(INTDIR)\popupmenu.obj"
	-@erase "$(INTDIR)\popupmenu.sbr"
	-@erase "$(INTDIR)\Road Smoothing.obj"
	-@erase "$(INTDIR)\Road Smoothing.sbr"
	-@erase "$(INTDIR)\Sector Summary.obj"
	-@erase "$(INTDIR)\Sector Summary.sbr"
	-@erase "$(INTDIR)\selectwin.obj"
	-@erase "$(INTDIR)\selectwin.sbr"
	-@erase "$(INTDIR)\SmartMethod.obj"
	-@erase "$(INTDIR)\SmartMethod.sbr"
	-@erase "$(INTDIR)\smooth.obj"
	-@erase "$(INTDIR)\smooth.sbr"
	-@erase "$(INTDIR)\Smoothing Utils.obj"
	-@erase "$(INTDIR)\Smoothing Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Editor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Editor.pch" /YX"Editor All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cursor Modes.sbr" \
	"$(INTDIR)\edit_sys.sbr" \
	"$(INTDIR)\Editor Callbacks.sbr" \
	"$(INTDIR)\Editor Modes.sbr" \
	"$(INTDIR)\Editor Taskbar Creation.sbr" \
	"$(INTDIR)\Editor Taskbar Utils.sbr" \
	"$(INTDIR)\Editor Undo.sbr" \
	"$(INTDIR)\EditorBuildings.sbr" \
	"$(INTDIR)\EditorItems.sbr" \
	"$(INTDIR)\EditorMapInfo.sbr" \
	"$(INTDIR)\EditorMercs.sbr" \
	"$(INTDIR)\EditorTerrain.sbr" \
	"$(INTDIR)\editscreen.sbr" \
	"$(INTDIR)\Item Statistics.sbr" \
	"$(INTDIR)\LoadScreen.sbr" \
	"$(INTDIR)\messagebox.sbr" \
	"$(INTDIR)\newsmooth.sbr" \
	"$(INTDIR)\popupmenu.sbr" \
	"$(INTDIR)\Road Smoothing.sbr" \
	"$(INTDIR)\Sector Summary.sbr" \
	"$(INTDIR)\selectwin.sbr" \
	"$(INTDIR)\SmartMethod.sbr" \
	"$(INTDIR)\smooth.sbr" \
	"$(INTDIR)\Smoothing Utils.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Editor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Cursor Modes.obj" \
	"$(INTDIR)\edit_sys.obj" \
	"$(INTDIR)\Editor Callbacks.obj" \
	"$(INTDIR)\Editor Modes.obj" \
	"$(INTDIR)\Editor Taskbar Creation.obj" \
	"$(INTDIR)\Editor Taskbar Utils.obj" \
	"$(INTDIR)\Editor Undo.obj" \
	"$(INTDIR)\EditorBuildings.obj" \
	"$(INTDIR)\EditorItems.obj" \
	"$(INTDIR)\EditorMapInfo.obj" \
	"$(INTDIR)\EditorMercs.obj" \
	"$(INTDIR)\EditorTerrain.obj" \
	"$(INTDIR)\editscreen.obj" \
	"$(INTDIR)\Item Statistics.obj" \
	"$(INTDIR)\LoadScreen.obj" \
	"$(INTDIR)\messagebox.obj" \
	"$(INTDIR)\newsmooth.obj" \
	"$(INTDIR)\popupmenu.obj" \
	"$(INTDIR)\Road Smoothing.obj" \
	"$(INTDIR)\Sector Summary.obj" \
	"$(INTDIR)\selectwin.obj" \
	"$(INTDIR)\SmartMethod.obj" \
	"$(INTDIR)\smooth.obj" \
	"$(INTDIR)\Smoothing Utils.obj"

"$(OUTDIR)\Editor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("Editor.dep")
!INCLUDE "Editor.dep"
!ELSE 
!MESSAGE Warning: cannot find "Editor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Editor - Win32 Release" || "$(CFG)" == "Editor - Win32 Debug" || "$(CFG)" == "Editor - Win32 Release with Debug Info" || "$(CFG)" == "Editor - Win32 Bounds Checker" || "$(CFG)" == "Editor - Win32 Debug Demo" || "$(CFG)" == "Editor - Win32 Release Demo" || "$(CFG)" == "Editor - Win32 Demo Release with Debug Info" || "$(CFG)" == "Editor - Win32 Demo Bounds Checker"
SOURCE=".\Cursor Modes.cpp"

"$(INTDIR)\Cursor Modes.obj"	"$(INTDIR)\Cursor Modes.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\edit_sys.cpp

"$(INTDIR)\edit_sys.obj"	"$(INTDIR)\edit_sys.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Editor Callbacks.cpp"

"$(INTDIR)\Editor Callbacks.obj"	"$(INTDIR)\Editor Callbacks.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Editor Modes.cpp"

"$(INTDIR)\Editor Modes.obj"	"$(INTDIR)\Editor Modes.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Editor Taskbar Creation.cpp"

"$(INTDIR)\Editor Taskbar Creation.obj"	"$(INTDIR)\Editor Taskbar Creation.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Editor Taskbar Utils.cpp"

"$(INTDIR)\Editor Taskbar Utils.obj"	"$(INTDIR)\Editor Taskbar Utils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Editor Undo.cpp"

"$(INTDIR)\Editor Undo.obj"	"$(INTDIR)\Editor Undo.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EditorBuildings.cpp

"$(INTDIR)\EditorBuildings.obj"	"$(INTDIR)\EditorBuildings.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EditorItems.cpp

"$(INTDIR)\EditorItems.obj"	"$(INTDIR)\EditorItems.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EditorMapInfo.cpp

"$(INTDIR)\EditorMapInfo.obj"	"$(INTDIR)\EditorMapInfo.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EditorMercs.cpp

"$(INTDIR)\EditorMercs.obj"	"$(INTDIR)\EditorMercs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EditorTerrain.cpp

"$(INTDIR)\EditorTerrain.obj"	"$(INTDIR)\EditorTerrain.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\editscreen.cpp

"$(INTDIR)\editscreen.obj"	"$(INTDIR)\editscreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Item Statistics.cpp"

"$(INTDIR)\Item Statistics.obj"	"$(INTDIR)\Item Statistics.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LoadScreen.cpp

"$(INTDIR)\LoadScreen.obj"	"$(INTDIR)\LoadScreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\messagebox.cpp

"$(INTDIR)\messagebox.obj"	"$(INTDIR)\messagebox.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\newsmooth.cpp

"$(INTDIR)\newsmooth.obj"	"$(INTDIR)\newsmooth.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\popupmenu.cpp

"$(INTDIR)\popupmenu.obj"	"$(INTDIR)\popupmenu.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Road Smoothing.cpp"

"$(INTDIR)\Road Smoothing.obj"	"$(INTDIR)\Road Smoothing.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Sector Summary.cpp"

"$(INTDIR)\Sector Summary.obj"	"$(INTDIR)\Sector Summary.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\selectwin.cpp

"$(INTDIR)\selectwin.obj"	"$(INTDIR)\selectwin.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SmartMethod.cpp

"$(INTDIR)\SmartMethod.obj"	"$(INTDIR)\SmartMethod.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\smooth.cpp

"$(INTDIR)\smooth.obj"	"$(INTDIR)\smooth.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Smoothing Utils.cpp"

"$(INTDIR)\Smoothing Utils.obj"	"$(INTDIR)\Smoothing Utils.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

