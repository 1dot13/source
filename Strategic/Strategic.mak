# Microsoft Developer Studio Generated NMAKE File, Based on Strategic.dsp
!IF "$(CFG)" == ""
CFG=Strategic - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to Strategic - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "Strategic - Win32 Release" && "$(CFG)" != "Strategic - Win32 Debug" && "$(CFG)" != "Strategic - Win32 Release with Debug Info" && "$(CFG)" != "Strategic - Win32 Bounds Checker" && "$(CFG)" != "Strategic - Win32 Debug Demo" && "$(CFG)" != "Strategic - Win32 Release Demo" && "$(CFG)" != "Strategic - Win32 Demo Release with Debug Info" && "$(CFG)" != "Strategic - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Strategic.mak" CFG="Strategic - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Strategic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Strategic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Strategic - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Strategic - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "Strategic - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Strategic - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Strategic - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Strategic - Win32 Demo Bounds Checker" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "Strategic - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\Laptop" /I ".\\" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Strategic - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\Laptop" /I ".\\" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Strategic - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Tactical" /I "..\Laptop" /I "..\Editor" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Strategic - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\Laptop" /I "\ja2\build\editor" /I "\ja2\build\tactical" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Strategic - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\Laptop" /I "\ja2\build\editor" /I "\ja2\build\tactical" /D "_DEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Strategic - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\Laptop" /I "\ja2\build\Editor" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Strategic - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Tactical" /I "..\Laptop" /I "..\Editor" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Strategic - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Strategic.lib" "$(OUTDIR)\Strategic.bsc"


CLEAN :
	-@erase "$(INTDIR)\AI Viewer.obj"
	-@erase "$(INTDIR)\AI Viewer.sbr"
	-@erase "$(INTDIR)\Assignments.obj"
	-@erase "$(INTDIR)\Assignments.sbr"
	-@erase "$(INTDIR)\Auto Resolve.obj"
	-@erase "$(INTDIR)\Auto Resolve.sbr"
	-@erase "$(INTDIR)\Campaign Init.obj"
	-@erase "$(INTDIR)\Campaign Init.sbr"
	-@erase "$(INTDIR)\Creature Spreading.obj"
	-@erase "$(INTDIR)\Creature Spreading.sbr"
	-@erase "$(INTDIR)\Game Clock.obj"
	-@erase "$(INTDIR)\Game Clock.sbr"
	-@erase "$(INTDIR)\Game Event Hook.obj"
	-@erase "$(INTDIR)\Game Event Hook.sbr"
	-@erase "$(INTDIR)\Game Events.obj"
	-@erase "$(INTDIR)\Game Events.sbr"
	-@erase "$(INTDIR)\Game Init.obj"
	-@erase "$(INTDIR)\Game Init.sbr"
	-@erase "$(INTDIR)\Hourly Update.obj"
	-@erase "$(INTDIR)\Hourly Update.sbr"
	-@erase "$(INTDIR)\Map Screen Helicopter.obj"
	-@erase "$(INTDIR)\Map Screen Helicopter.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Border.obj"
	-@erase "$(INTDIR)\Map Screen Interface Border.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.obj"
	-@erase "$(INTDIR)\Map Screen Interface Bottom.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map Inventory.sbr"
	-@erase "$(INTDIR)\Map Screen Interface Map.obj"
	-@erase "$(INTDIR)\Map Screen Interface Map.sbr"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.obj"
	-@erase "$(INTDIR)\Map Screen Interface TownMine Info.sbr"
	-@erase "$(INTDIR)\Map Screen Interface.obj"
	-@erase "$(INTDIR)\Map Screen Interface.sbr"
	-@erase "$(INTDIR)\mapscreen.obj"
	-@erase "$(INTDIR)\mapscreen.sbr"
	-@erase "$(INTDIR)\Meanwhile.obj"
	-@erase "$(INTDIR)\Meanwhile.sbr"
	-@erase "$(INTDIR)\Merc Contract.obj"
	-@erase "$(INTDIR)\Merc Contract.sbr"
	-@erase "$(INTDIR)\Player Command.obj"
	-@erase "$(INTDIR)\Player Command.sbr"
	-@erase "$(INTDIR)\PreBattle Interface.obj"
	-@erase "$(INTDIR)\PreBattle Interface.sbr"
	-@erase "$(INTDIR)\Queen Command.obj"
	-@erase "$(INTDIR)\Queen Command.sbr"
	-@erase "$(INTDIR)\Quest Debug System.obj"
	-@erase "$(INTDIR)\Quest Debug System.sbr"
	-@erase "$(INTDIR)\Quests.obj"
	-@erase "$(INTDIR)\Quests.sbr"
	-@erase "$(INTDIR)\QuestText.obj"
	-@erase "$(INTDIR)\QuestText.sbr"
	-@erase "$(INTDIR)\Scheduling.obj"
	-@erase "$(INTDIR)\Scheduling.sbr"
	-@erase "$(INTDIR)\Strategic AI.obj"
	-@erase "$(INTDIR)\Strategic AI.sbr"
	-@erase "$(INTDIR)\Strategic Event Handler.obj"
	-@erase "$(INTDIR)\Strategic Event Handler.sbr"
	-@erase "$(INTDIR)\Strategic Merc Handler.obj"
	-@erase "$(INTDIR)\Strategic Merc Handler.sbr"
	-@erase "$(INTDIR)\Strategic Mines.obj"
	-@erase "$(INTDIR)\Strategic Mines.sbr"
	-@erase "$(INTDIR)\Strategic Movement Costs.obj"
	-@erase "$(INTDIR)\Strategic Movement Costs.sbr"
	-@erase "$(INTDIR)\Strategic Movement.obj"
	-@erase "$(INTDIR)\Strategic Movement.sbr"
	-@erase "$(INTDIR)\Strategic Pathing.obj"
	-@erase "$(INTDIR)\Strategic Pathing.sbr"
	-@erase "$(INTDIR)\Strategic Status.obj"
	-@erase "$(INTDIR)\Strategic Status.sbr"
	-@erase "$(INTDIR)\Strategic Town Loyalty.obj"
	-@erase "$(INTDIR)\Strategic Town Loyalty.sbr"
	-@erase "$(INTDIR)\strategic town reputation.obj"
	-@erase "$(INTDIR)\strategic town reputation.sbr"
	-@erase "$(INTDIR)\Strategic Turns.obj"
	-@erase "$(INTDIR)\Strategic Turns.sbr"
	-@erase "$(INTDIR)\strategic.obj"
	-@erase "$(INTDIR)\strategic.sbr"
	-@erase "$(INTDIR)\strategicmap.obj"
	-@erase "$(INTDIR)\strategicmap.sbr"
	-@erase "$(INTDIR)\Town Militia.obj"
	-@erase "$(INTDIR)\Town Militia.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Strategic.bsc"
	-@erase "$(OUTDIR)\Strategic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Tactical" /I "\ja2\build\Laptop" /I "\ja2\build\editor" /I "\ja2\build\tactical" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Strategic.pch" /YX"Strategic All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Strategic.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AI Viewer.sbr" \
	"$(INTDIR)\Assignments.sbr" \
	"$(INTDIR)\Auto Resolve.sbr" \
	"$(INTDIR)\Campaign Init.sbr" \
	"$(INTDIR)\Creature Spreading.sbr" \
	"$(INTDIR)\Game Clock.sbr" \
	"$(INTDIR)\Game Event Hook.sbr" \
	"$(INTDIR)\Game Events.sbr" \
	"$(INTDIR)\Game Init.sbr" \
	"$(INTDIR)\Hourly Update.sbr" \
	"$(INTDIR)\Map Screen Helicopter.sbr" \
	"$(INTDIR)\Map Screen Interface Border.sbr" \
	"$(INTDIR)\Map Screen Interface Bottom.sbr" \
	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" \
	"$(INTDIR)\Map Screen Interface Map.sbr" \
	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" \
	"$(INTDIR)\Map Screen Interface.sbr" \
	"$(INTDIR)\mapscreen.sbr" \
	"$(INTDIR)\Meanwhile.sbr" \
	"$(INTDIR)\Merc Contract.sbr" \
	"$(INTDIR)\Player Command.sbr" \
	"$(INTDIR)\PreBattle Interface.sbr" \
	"$(INTDIR)\Queen Command.sbr" \
	"$(INTDIR)\Quest Debug System.sbr" \
	"$(INTDIR)\Quests.sbr" \
	"$(INTDIR)\QuestText.sbr" \
	"$(INTDIR)\Scheduling.sbr" \
	"$(INTDIR)\Strategic AI.sbr" \
	"$(INTDIR)\Strategic Event Handler.sbr" \
	"$(INTDIR)\Strategic Merc Handler.sbr" \
	"$(INTDIR)\Strategic Mines.sbr" \
	"$(INTDIR)\Strategic Movement Costs.sbr" \
	"$(INTDIR)\Strategic Movement.sbr" \
	"$(INTDIR)\Strategic Pathing.sbr" \
	"$(INTDIR)\Strategic Status.sbr" \
	"$(INTDIR)\Strategic Town Loyalty.sbr" \
	"$(INTDIR)\strategic town reputation.sbr" \
	"$(INTDIR)\Strategic Turns.sbr" \
	"$(INTDIR)\strategic.sbr" \
	"$(INTDIR)\strategicmap.sbr" \
	"$(INTDIR)\Town Militia.sbr"

"$(OUTDIR)\Strategic.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Strategic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AI Viewer.obj" \
	"$(INTDIR)\Assignments.obj" \
	"$(INTDIR)\Auto Resolve.obj" \
	"$(INTDIR)\Campaign Init.obj" \
	"$(INTDIR)\Creature Spreading.obj" \
	"$(INTDIR)\Game Clock.obj" \
	"$(INTDIR)\Game Event Hook.obj" \
	"$(INTDIR)\Game Events.obj" \
	"$(INTDIR)\Game Init.obj" \
	"$(INTDIR)\Hourly Update.obj" \
	"$(INTDIR)\Map Screen Helicopter.obj" \
	"$(INTDIR)\Map Screen Interface Border.obj" \
	"$(INTDIR)\Map Screen Interface Bottom.obj" \
	"$(INTDIR)\Map Screen Interface Map Inventory.obj" \
	"$(INTDIR)\Map Screen Interface Map.obj" \
	"$(INTDIR)\Map Screen Interface TownMine Info.obj" \
	"$(INTDIR)\Map Screen Interface.obj" \
	"$(INTDIR)\mapscreen.obj" \
	"$(INTDIR)\Meanwhile.obj" \
	"$(INTDIR)\Merc Contract.obj" \
	"$(INTDIR)\Player Command.obj" \
	"$(INTDIR)\PreBattle Interface.obj" \
	"$(INTDIR)\Queen Command.obj" \
	"$(INTDIR)\Quest Debug System.obj" \
	"$(INTDIR)\Quests.obj" \
	"$(INTDIR)\QuestText.obj" \
	"$(INTDIR)\Scheduling.obj" \
	"$(INTDIR)\Strategic AI.obj" \
	"$(INTDIR)\Strategic Event Handler.obj" \
	"$(INTDIR)\Strategic Merc Handler.obj" \
	"$(INTDIR)\Strategic Mines.obj" \
	"$(INTDIR)\Strategic Movement Costs.obj" \
	"$(INTDIR)\Strategic Movement.obj" \
	"$(INTDIR)\Strategic Pathing.obj" \
	"$(INTDIR)\Strategic Status.obj" \
	"$(INTDIR)\Strategic Town Loyalty.obj" \
	"$(INTDIR)\strategic town reputation.obj" \
	"$(INTDIR)\Strategic Turns.obj" \
	"$(INTDIR)\strategic.obj" \
	"$(INTDIR)\strategicmap.obj" \
	"$(INTDIR)\Town Militia.obj"

"$(OUTDIR)\Strategic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("Strategic.dep")
!INCLUDE "Strategic.dep"
!ELSE 
!MESSAGE Warning: cannot find "Strategic.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Strategic - Win32 Release" || "$(CFG)" == "Strategic - Win32 Debug" || "$(CFG)" == "Strategic - Win32 Release with Debug Info" || "$(CFG)" == "Strategic - Win32 Bounds Checker" || "$(CFG)" == "Strategic - Win32 Debug Demo" || "$(CFG)" == "Strategic - Win32 Release Demo" || "$(CFG)" == "Strategic - Win32 Demo Release with Debug Info" || "$(CFG)" == "Strategic - Win32 Demo Bounds Checker"
SOURCE=".\AI Viewer.cpp"

"$(INTDIR)\AI Viewer.obj"	"$(INTDIR)\AI Viewer.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Assignments.cpp

"$(INTDIR)\Assignments.obj"	"$(INTDIR)\Assignments.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Auto Resolve.cpp"

"$(INTDIR)\Auto Resolve.obj"	"$(INTDIR)\Auto Resolve.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Campaign Init.cpp"

"$(INTDIR)\Campaign Init.obj"	"$(INTDIR)\Campaign Init.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Creature Spreading.cpp"

"$(INTDIR)\Creature Spreading.obj"	"$(INTDIR)\Creature Spreading.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Game Clock.cpp"

"$(INTDIR)\Game Clock.obj"	"$(INTDIR)\Game Clock.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Game Event Hook.cpp"

"$(INTDIR)\Game Event Hook.obj"	"$(INTDIR)\Game Event Hook.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Game Events.cpp"

"$(INTDIR)\Game Events.obj"	"$(INTDIR)\Game Events.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Game Init.cpp"

"$(INTDIR)\Game Init.obj"	"$(INTDIR)\Game Init.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Hourly Update.cpp"

"$(INTDIR)\Hourly Update.obj"	"$(INTDIR)\Hourly Update.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Screen Helicopter.cpp"

"$(INTDIR)\Map Screen Helicopter.obj"	"$(INTDIR)\Map Screen Helicopter.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Screen Interface Border.cpp"

"$(INTDIR)\Map Screen Interface Border.obj"	"$(INTDIR)\Map Screen Interface Border.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Screen Interface Bottom.cpp"

"$(INTDIR)\Map Screen Interface Bottom.obj"	"$(INTDIR)\Map Screen Interface Bottom.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Screen Interface Map Inventory.cpp"

"$(INTDIR)\Map Screen Interface Map Inventory.obj"	"$(INTDIR)\Map Screen Interface Map Inventory.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Screen Interface Map.cpp"

"$(INTDIR)\Map Screen Interface Map.obj"	"$(INTDIR)\Map Screen Interface Map.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Screen Interface TownMine Info.cpp"

"$(INTDIR)\Map Screen Interface TownMine Info.obj"	"$(INTDIR)\Map Screen Interface TownMine Info.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Map Screen Interface.cpp"

"$(INTDIR)\Map Screen Interface.obj"	"$(INTDIR)\Map Screen Interface.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mapscreen.cpp

"$(INTDIR)\mapscreen.obj"	"$(INTDIR)\mapscreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Meanwhile.cpp

"$(INTDIR)\Meanwhile.obj"	"$(INTDIR)\Meanwhile.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Merc Contract.cpp"

"$(INTDIR)\Merc Contract.obj"	"$(INTDIR)\Merc Contract.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Player Command.cpp"

"$(INTDIR)\Player Command.obj"	"$(INTDIR)\Player Command.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\PreBattle Interface.cpp"

"$(INTDIR)\PreBattle Interface.obj"	"$(INTDIR)\PreBattle Interface.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Queen Command.cpp"

"$(INTDIR)\Queen Command.obj"	"$(INTDIR)\Queen Command.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Quest Debug System.cpp"

"$(INTDIR)\Quest Debug System.obj"	"$(INTDIR)\Quest Debug System.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Quests.cpp

"$(INTDIR)\Quests.obj"	"$(INTDIR)\Quests.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\QuestText.cpp

"$(INTDIR)\QuestText.obj"	"$(INTDIR)\QuestText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Scheduling.cpp

"$(INTDIR)\Scheduling.obj"	"$(INTDIR)\Scheduling.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic AI.cpp"

"$(INTDIR)\Strategic AI.obj"	"$(INTDIR)\Strategic AI.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Event Handler.cpp"

"$(INTDIR)\Strategic Event Handler.obj"	"$(INTDIR)\Strategic Event Handler.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Merc Handler.cpp"

"$(INTDIR)\Strategic Merc Handler.obj"	"$(INTDIR)\Strategic Merc Handler.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Mines.cpp"

"$(INTDIR)\Strategic Mines.obj"	"$(INTDIR)\Strategic Mines.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Movement Costs.cpp"

"$(INTDIR)\Strategic Movement Costs.obj"	"$(INTDIR)\Strategic Movement Costs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Movement.cpp"

"$(INTDIR)\Strategic Movement.obj"	"$(INTDIR)\Strategic Movement.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Pathing.cpp"

"$(INTDIR)\Strategic Pathing.obj"	"$(INTDIR)\Strategic Pathing.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Status.cpp"

"$(INTDIR)\Strategic Status.obj"	"$(INTDIR)\Strategic Status.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Town Loyalty.cpp"

"$(INTDIR)\Strategic Town Loyalty.obj"	"$(INTDIR)\Strategic Town Loyalty.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\strategic town reputation.cpp"

"$(INTDIR)\strategic town reputation.obj"	"$(INTDIR)\strategic town reputation.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Turns.cpp"

"$(INTDIR)\Strategic Turns.obj"	"$(INTDIR)\Strategic Turns.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\strategic.cpp

"$(INTDIR)\strategic.obj"	"$(INTDIR)\strategic.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\strategicmap.cpp

"$(INTDIR)\strategicmap.obj"	"$(INTDIR)\strategicmap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Town Militia.cpp"

"$(INTDIR)\Town Militia.obj"	"$(INTDIR)\Town Militia.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

