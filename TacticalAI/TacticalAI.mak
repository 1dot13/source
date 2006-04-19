# Microsoft Developer Studio Generated NMAKE File, Based on TacticalAI.dsp
!IF "$(CFG)" == ""
CFG=TacticalAI - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to TacticalAI - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "TacticalAI - Win32 Release" && "$(CFG)" != "TacticalAI - Win32 Debug" && "$(CFG)" != "TacticalAI - Win32 Release with Debug Info" && "$(CFG)" != "TacticalAI - Win32 Bounds Checker" && "$(CFG)" != "TacticalAI - Win32 Debug Demo" && "$(CFG)" != "TacticalAI - Win32 Release Demo" && "$(CFG)" != "TacticalAI - Win32 Demo Release with Debug Info" && "$(CFG)" != "TacticalAI - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TacticalAI.mak" CFG="TacticalAI - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TacticalAI - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TacticalAI - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "TacticalAI - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "TacticalAI - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "TacticalAI - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "TacticalAI - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "TacticalAI - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "TacticalAI - Win32 Demo Bounds Checker" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "TacticalAI - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TacticalAI - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TacticalAI - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\laptop" /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\TileEngine" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TacticalAI - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\laptop" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TacticalAI - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\laptop" /D "_DEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TacticalAI - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TacticalAI - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\TileEngine" /I "..\strategic" /I "..\laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TacticalAI - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\TacticalAI.lib" "$(OUTDIR)\TacticalAI.bsc"


CLEAN :
	-@erase "$(INTDIR)\AIList.obj"
	-@erase "$(INTDIR)\AIList.sbr"
	-@erase "$(INTDIR)\AIMain.obj"
	-@erase "$(INTDIR)\AIMain.sbr"
	-@erase "$(INTDIR)\AIUtils.obj"
	-@erase "$(INTDIR)\AIUtils.sbr"
	-@erase "$(INTDIR)\Attacks.obj"
	-@erase "$(INTDIR)\Attacks.sbr"
	-@erase "$(INTDIR)\CreatureDecideAction.obj"
	-@erase "$(INTDIR)\CreatureDecideAction.sbr"
	-@erase "$(INTDIR)\DecideAction.obj"
	-@erase "$(INTDIR)\DecideAction.sbr"
	-@erase "$(INTDIR)\FindLocations.obj"
	-@erase "$(INTDIR)\FindLocations.sbr"
	-@erase "$(INTDIR)\Knowledge.obj"
	-@erase "$(INTDIR)\Knowledge.sbr"
	-@erase "$(INTDIR)\Medical.obj"
	-@erase "$(INTDIR)\Medical.sbr"
	-@erase "$(INTDIR)\Movement.obj"
	-@erase "$(INTDIR)\Movement.sbr"
	-@erase "$(INTDIR)\NPC.obj"
	-@erase "$(INTDIR)\NPC.sbr"
	-@erase "$(INTDIR)\PanicButtons.obj"
	-@erase "$(INTDIR)\PanicButtons.sbr"
	-@erase "$(INTDIR)\QuestDebug.obj"
	-@erase "$(INTDIR)\QuestDebug.sbr"
	-@erase "$(INTDIR)\Realtime.obj"
	-@erase "$(INTDIR)\Realtime.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TacticalAI.bsc"
	-@erase "$(OUTDIR)\TacticalAI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\Utils" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\laptop" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TacticalAI.pch" /YX"AI All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TacticalAI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AIList.sbr" \
	"$(INTDIR)\AIMain.sbr" \
	"$(INTDIR)\AIUtils.sbr" \
	"$(INTDIR)\Attacks.sbr" \
	"$(INTDIR)\CreatureDecideAction.sbr" \
	"$(INTDIR)\DecideAction.sbr" \
	"$(INTDIR)\FindLocations.sbr" \
	"$(INTDIR)\Knowledge.sbr" \
	"$(INTDIR)\Medical.sbr" \
	"$(INTDIR)\Movement.sbr" \
	"$(INTDIR)\NPC.sbr" \
	"$(INTDIR)\PanicButtons.sbr" \
	"$(INTDIR)\QuestDebug.sbr" \
	"$(INTDIR)\Realtime.sbr"

"$(OUTDIR)\TacticalAI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\TacticalAI.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AIList.obj" \
	"$(INTDIR)\AIMain.obj" \
	"$(INTDIR)\AIUtils.obj" \
	"$(INTDIR)\Attacks.obj" \
	"$(INTDIR)\CreatureDecideAction.obj" \
	"$(INTDIR)\DecideAction.obj" \
	"$(INTDIR)\FindLocations.obj" \
	"$(INTDIR)\Knowledge.obj" \
	"$(INTDIR)\Medical.obj" \
	"$(INTDIR)\Movement.obj" \
	"$(INTDIR)\NPC.obj" \
	"$(INTDIR)\PanicButtons.obj" \
	"$(INTDIR)\QuestDebug.obj" \
	"$(INTDIR)\Realtime.obj"

"$(OUTDIR)\TacticalAI.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("TacticalAI.dep")
!INCLUDE "TacticalAI.dep"
!ELSE 
!MESSAGE Warning: cannot find "TacticalAI.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TacticalAI - Win32 Release" || "$(CFG)" == "TacticalAI - Win32 Debug" || "$(CFG)" == "TacticalAI - Win32 Release with Debug Info" || "$(CFG)" == "TacticalAI - Win32 Bounds Checker" || "$(CFG)" == "TacticalAI - Win32 Debug Demo" || "$(CFG)" == "TacticalAI - Win32 Release Demo" || "$(CFG)" == "TacticalAI - Win32 Demo Release with Debug Info" || "$(CFG)" == "TacticalAI - Win32 Demo Bounds Checker"
SOURCE=.\AIList.cpp

"$(INTDIR)\AIList.obj"	"$(INTDIR)\AIList.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AIMain.cpp

"$(INTDIR)\AIMain.obj"	"$(INTDIR)\AIMain.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AIUtils.cpp

"$(INTDIR)\AIUtils.obj"	"$(INTDIR)\AIUtils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Attacks.cpp

"$(INTDIR)\Attacks.obj"	"$(INTDIR)\Attacks.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CreatureDecideAction.cpp

"$(INTDIR)\CreatureDecideAction.obj"	"$(INTDIR)\CreatureDecideAction.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DecideAction.cpp

"$(INTDIR)\DecideAction.obj"	"$(INTDIR)\DecideAction.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FindLocations.cpp

"$(INTDIR)\FindLocations.obj"	"$(INTDIR)\FindLocations.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Knowledge.cpp

"$(INTDIR)\Knowledge.obj"	"$(INTDIR)\Knowledge.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Medical.cpp

"$(INTDIR)\Medical.obj"	"$(INTDIR)\Medical.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Movement.cpp

"$(INTDIR)\Movement.obj"	"$(INTDIR)\Movement.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NPC.cpp

"$(INTDIR)\NPC.obj"	"$(INTDIR)\NPC.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PanicButtons.cpp

"$(INTDIR)\PanicButtons.obj"	"$(INTDIR)\PanicButtons.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\QuestDebug.cpp

"$(INTDIR)\QuestDebug.obj"	"$(INTDIR)\QuestDebug.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Realtime.cpp

"$(INTDIR)\Realtime.obj"	"$(INTDIR)\Realtime.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

