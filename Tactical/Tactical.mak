# Microsoft Developer Studio Generated NMAKE File, Based on Tactical.dsp
!IF "$(CFG)" == ""
CFG=Tactical - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to Tactical - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "Tactical - Win32 Release" && "$(CFG)" != "Tactical - Win32 Debug" && "$(CFG)" != "Tactical - Win32 Release with Debug Info" && "$(CFG)" != "Tactical - Win32 Bounds Checker" && "$(CFG)" != "Tactical - Win32 Debug Demo" && "$(CFG)" != "Tactical - Win32 Release Demo" && "$(CFG)" != "Tactical - Win32 Demo Release with Debug Info" && "$(CFG)" != "Tactical - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tactical - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Tactical.lib" "$(OUTDIR)\Tactical.bsc"


CLEAN :
	-@erase "$(INTDIR)\Air Raid.obj"
	-@erase "$(INTDIR)\Air Raid.sbr"
	-@erase "$(INTDIR)\Animation Cache.obj"
	-@erase "$(INTDIR)\Animation Cache.sbr"
	-@erase "$(INTDIR)\Animation Control.obj"
	-@erase "$(INTDIR)\Animation Control.sbr"
	-@erase "$(INTDIR)\Animation Data.obj"
	-@erase "$(INTDIR)\Animation Data.sbr"
	-@erase "$(INTDIR)\Arms Dealer Init.obj"
	-@erase "$(INTDIR)\Arms Dealer Init.sbr"
	-@erase "$(INTDIR)\ArmsDealerInvInit.obj"
	-@erase "$(INTDIR)\ArmsDealerInvInit.sbr"
	-@erase "$(INTDIR)\Auto Bandage.obj"
	-@erase "$(INTDIR)\Auto Bandage.sbr"
	-@erase "$(INTDIR)\Boxing.obj"
	-@erase "$(INTDIR)\Boxing.sbr"
	-@erase "$(INTDIR)\bullets.obj"
	-@erase "$(INTDIR)\bullets.sbr"
	-@erase "$(INTDIR)\Campaign.obj"
	-@erase "$(INTDIR)\Campaign.sbr"
	-@erase "$(INTDIR)\Civ Quotes.obj"
	-@erase "$(INTDIR)\Civ Quotes.sbr"
	-@erase "$(INTDIR)\Dialogue Control.obj"
	-@erase "$(INTDIR)\Dialogue Control.sbr"
	-@erase "$(INTDIR)\DisplayCover.obj"
	-@erase "$(INTDIR)\DisplayCover.sbr"
	-@erase "$(INTDIR)\Drugs And Alcohol.obj"
	-@erase "$(INTDIR)\Drugs And Alcohol.sbr"
	-@erase "$(INTDIR)\End Game.obj"
	-@erase "$(INTDIR)\End Game.sbr"
	-@erase "$(INTDIR)\Enemy Soldier Save.obj"
	-@erase "$(INTDIR)\Enemy Soldier Save.sbr"
	-@erase "$(INTDIR)\Faces.obj"
	-@erase "$(INTDIR)\Faces.sbr"
	-@erase "$(INTDIR)\fov.obj"
	-@erase "$(INTDIR)\fov.sbr"
	-@erase "$(INTDIR)\GAP.obj"
	-@erase "$(INTDIR)\GAP.sbr"
	-@erase "$(INTDIR)\Handle Doors.obj"
	-@erase "$(INTDIR)\Handle Doors.sbr"
	-@erase "$(INTDIR)\Handle Items.obj"
	-@erase "$(INTDIR)\Handle Items.sbr"
	-@erase "$(INTDIR)\Handle UI Plan.obj"
	-@erase "$(INTDIR)\Handle UI Plan.sbr"
	-@erase "$(INTDIR)\Handle UI.obj"
	-@erase "$(INTDIR)\Handle UI.sbr"
	-@erase "$(INTDIR)\Interface Control.obj"
	-@erase "$(INTDIR)\Interface Control.sbr"
	-@erase "$(INTDIR)\Interface Cursors.obj"
	-@erase "$(INTDIR)\Interface Cursors.sbr"
	-@erase "$(INTDIR)\Interface Dialogue.obj"
	-@erase "$(INTDIR)\Interface Dialogue.sbr"
	-@erase "$(INTDIR)\Interface Items.obj"
	-@erase "$(INTDIR)\Interface Items.sbr"
	-@erase "$(INTDIR)\Interface Panels.obj"
	-@erase "$(INTDIR)\Interface Panels.sbr"
	-@erase "$(INTDIR)\Interface Utils.obj"
	-@erase "$(INTDIR)\Interface Utils.sbr"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Interface.sbr"
	-@erase "$(INTDIR)\Inventory Choosing.obj"
	-@erase "$(INTDIR)\Inventory Choosing.sbr"
	-@erase "$(INTDIR)\Items.obj"
	-@erase "$(INTDIR)\Items.sbr"
	-@erase "$(INTDIR)\Keys.obj"
	-@erase "$(INTDIR)\Keys.sbr"
	-@erase "$(INTDIR)\LOS.obj"
	-@erase "$(INTDIR)\LOS.sbr"
	-@erase "$(INTDIR)\Map Information.obj"
	-@erase "$(INTDIR)\Map Information.sbr"
	-@erase "$(INTDIR)\Merc Entering.obj"
	-@erase "$(INTDIR)\Merc Entering.sbr"
	-@erase "$(INTDIR)\Merc Hiring.obj"
	-@erase "$(INTDIR)\Merc Hiring.sbr"
	-@erase "$(INTDIR)\Militia Control.obj"
	-@erase "$(INTDIR)\Militia Control.sbr"
	-@erase "$(INTDIR)\Morale.obj"
	-@erase "$(INTDIR)\Morale.sbr"
	-@erase "$(INTDIR)\opplist.obj"
	-@erase "$(INTDIR)\opplist.sbr"
	-@erase "$(INTDIR)\Overhead.obj"
	-@erase "$(INTDIR)\Overhead.sbr"
	-@erase "$(INTDIR)\PATHAI.obj"
	-@erase "$(INTDIR)\PATHAI.sbr"
	-@erase "$(INTDIR)\Points.obj"
	-@erase "$(INTDIR)\Points.sbr"
	-@erase "$(INTDIR)\QARRAY.obj"
	-@erase "$(INTDIR)\QARRAY.sbr"
	-@erase "$(INTDIR)\Real Time Input.obj"
	-@erase "$(INTDIR)\Real Time Input.sbr"
	-@erase "$(INTDIR)\Rotting Corpses.obj"
	-@erase "$(INTDIR)\Rotting Corpses.sbr"
	-@erase "$(INTDIR)\ShopKeeper Interface.obj"
	-@erase "$(INTDIR)\ShopKeeper Interface.sbr"
	-@erase "$(INTDIR)\SkillCheck.obj"
	-@erase "$(INTDIR)\SkillCheck.sbr"
	-@erase "$(INTDIR)\Soldier Add.obj"
	-@erase "$(INTDIR)\Soldier Add.sbr"
	-@erase "$(INTDIR)\Soldier Ani.obj"
	-@erase "$(INTDIR)\Soldier Ani.sbr"
	-@erase "$(INTDIR)\Soldier Control.obj"
	-@erase "$(INTDIR)\Soldier Control.sbr"
	-@erase "$(INTDIR)\Soldier Create.obj"
	-@erase "$(INTDIR)\Soldier Create.sbr"
	-@erase "$(INTDIR)\Soldier Find.obj"
	-@erase "$(INTDIR)\Soldier Find.sbr"
	-@erase "$(INTDIR)\Soldier Init List.obj"
	-@erase "$(INTDIR)\Soldier Init List.sbr"
	-@erase "$(INTDIR)\Soldier Profile.obj"
	-@erase "$(INTDIR)\Soldier Profile.sbr"
	-@erase "$(INTDIR)\Soldier Tile.obj"
	-@erase "$(INTDIR)\Soldier Tile.sbr"
	-@erase "$(INTDIR)\Spread Burst.obj"
	-@erase "$(INTDIR)\Spread Burst.sbr"
	-@erase "$(INTDIR)\Squads.obj"
	-@erase "$(INTDIR)\Squads.sbr"
	-@erase "$(INTDIR)\Strategic Exit GUI.obj"
	-@erase "$(INTDIR)\Strategic Exit GUI.sbr"
	-@erase "$(INTDIR)\Structure Wrap.obj"
	-@erase "$(INTDIR)\Structure Wrap.sbr"
	-@erase "$(INTDIR)\Tactical Save.obj"
	-@erase "$(INTDIR)\Tactical Save.sbr"
	-@erase "$(INTDIR)\Tactical Turns.obj"
	-@erase "$(INTDIR)\Tactical Turns.sbr"
	-@erase "$(INTDIR)\TeamTurns.obj"
	-@erase "$(INTDIR)\TeamTurns.sbr"
	-@erase "$(INTDIR)\Turn Based Input.obj"
	-@erase "$(INTDIR)\Turn Based Input.sbr"
	-@erase "$(INTDIR)\UI Cursors.obj"
	-@erase "$(INTDIR)\UI Cursors.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Vehicles.obj"
	-@erase "$(INTDIR)\Vehicles.sbr"
	-@erase "$(INTDIR)\Weapons.obj"
	-@erase "$(INTDIR)\Weapons.sbr"
	-@erase "$(INTDIR)\World Items.obj"
	-@erase "$(INTDIR)\World Items.sbr"
	-@erase "$(OUTDIR)\Tactical.bsc"
	-@erase "$(OUTDIR)\Tactical.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tactical.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Air Raid.sbr" \
	"$(INTDIR)\Animation Cache.sbr" \
	"$(INTDIR)\Animation Control.sbr" \
	"$(INTDIR)\Animation Data.sbr" \
	"$(INTDIR)\Arms Dealer Init.sbr" \
	"$(INTDIR)\ArmsDealerInvInit.sbr" \
	"$(INTDIR)\Auto Bandage.sbr" \
	"$(INTDIR)\Boxing.sbr" \
	"$(INTDIR)\bullets.sbr" \
	"$(INTDIR)\Campaign.sbr" \
	"$(INTDIR)\Civ Quotes.sbr" \
	"$(INTDIR)\Dialogue Control.sbr" \
	"$(INTDIR)\DisplayCover.sbr" \
	"$(INTDIR)\Drugs And Alcohol.sbr" \
	"$(INTDIR)\End Game.sbr" \
	"$(INTDIR)\Enemy Soldier Save.sbr" \
	"$(INTDIR)\Faces.sbr" \
	"$(INTDIR)\fov.sbr" \
	"$(INTDIR)\GAP.sbr" \
	"$(INTDIR)\Handle Doors.sbr" \
	"$(INTDIR)\Handle Items.sbr" \
	"$(INTDIR)\Handle UI Plan.sbr" \
	"$(INTDIR)\Handle UI.sbr" \
	"$(INTDIR)\Interface Control.sbr" \
	"$(INTDIR)\Interface Cursors.sbr" \
	"$(INTDIR)\Interface Dialogue.sbr" \
	"$(INTDIR)\Interface Items.sbr" \
	"$(INTDIR)\Interface Panels.sbr" \
	"$(INTDIR)\Interface Utils.sbr" \
	"$(INTDIR)\Interface.sbr" \
	"$(INTDIR)\Inventory Choosing.sbr" \
	"$(INTDIR)\Items.sbr" \
	"$(INTDIR)\Keys.sbr" \
	"$(INTDIR)\LOS.sbr" \
	"$(INTDIR)\Map Information.sbr" \
	"$(INTDIR)\Merc Entering.sbr" \
	"$(INTDIR)\Merc Hiring.sbr" \
	"$(INTDIR)\Militia Control.sbr" \
	"$(INTDIR)\Morale.sbr" \
	"$(INTDIR)\opplist.sbr" \
	"$(INTDIR)\Overhead.sbr" \
	"$(INTDIR)\PATHAI.sbr" \
	"$(INTDIR)\Points.sbr" \
	"$(INTDIR)\QARRAY.sbr" \
	"$(INTDIR)\Real Time Input.sbr" \
	"$(INTDIR)\Rotting Corpses.sbr" \
	"$(INTDIR)\ShopKeeper Interface.sbr" \
	"$(INTDIR)\SkillCheck.sbr" \
	"$(INTDIR)\Soldier Add.sbr" \
	"$(INTDIR)\Soldier Ani.sbr" \
	"$(INTDIR)\Soldier Control.sbr" \
	"$(INTDIR)\Soldier Create.sbr" \
	"$(INTDIR)\Soldier Find.sbr" \
	"$(INTDIR)\Soldier Init List.sbr" \
	"$(INTDIR)\Soldier Profile.sbr" \
	"$(INTDIR)\Soldier Tile.sbr" \
	"$(INTDIR)\Spread Burst.sbr" \
	"$(INTDIR)\Squads.sbr" \
	"$(INTDIR)\Strategic Exit GUI.sbr" \
	"$(INTDIR)\Structure Wrap.sbr" \
	"$(INTDIR)\Tactical Save.sbr" \
	"$(INTDIR)\Tactical Turns.sbr" \
	"$(INTDIR)\TeamTurns.sbr" \
	"$(INTDIR)\Turn Based Input.sbr" \
	"$(INTDIR)\UI Cursors.sbr" \
	"$(INTDIR)\Vehicles.sbr" \
	"$(INTDIR)\Weapons.sbr" \
	"$(INTDIR)\World Items.sbr"

"$(OUTDIR)\Tactical.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tactical.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Air Raid.obj" \
	"$(INTDIR)\Animation Cache.obj" \
	"$(INTDIR)\Animation Control.obj" \
	"$(INTDIR)\Animation Data.obj" \
	"$(INTDIR)\Arms Dealer Init.obj" \
	"$(INTDIR)\ArmsDealerInvInit.obj" \
	"$(INTDIR)\Auto Bandage.obj" \
	"$(INTDIR)\Boxing.obj" \
	"$(INTDIR)\bullets.obj" \
	"$(INTDIR)\Campaign.obj" \
	"$(INTDIR)\Civ Quotes.obj" \
	"$(INTDIR)\Dialogue Control.obj" \
	"$(INTDIR)\DisplayCover.obj" \
	"$(INTDIR)\Drugs And Alcohol.obj" \
	"$(INTDIR)\End Game.obj" \
	"$(INTDIR)\Enemy Soldier Save.obj" \
	"$(INTDIR)\Faces.obj" \
	"$(INTDIR)\fov.obj" \
	"$(INTDIR)\GAP.obj" \
	"$(INTDIR)\Handle Doors.obj" \
	"$(INTDIR)\Handle Items.obj" \
	"$(INTDIR)\Handle UI Plan.obj" \
	"$(INTDIR)\Handle UI.obj" \
	"$(INTDIR)\Interface Control.obj" \
	"$(INTDIR)\Interface Cursors.obj" \
	"$(INTDIR)\Interface Dialogue.obj" \
	"$(INTDIR)\Interface Items.obj" \
	"$(INTDIR)\Interface Panels.obj" \
	"$(INTDIR)\Interface Utils.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\Inventory Choosing.obj" \
	"$(INTDIR)\Items.obj" \
	"$(INTDIR)\Keys.obj" \
	"$(INTDIR)\LOS.obj" \
	"$(INTDIR)\Map Information.obj" \
	"$(INTDIR)\Merc Entering.obj" \
	"$(INTDIR)\Merc Hiring.obj" \
	"$(INTDIR)\Militia Control.obj" \
	"$(INTDIR)\Morale.obj" \
	"$(INTDIR)\opplist.obj" \
	"$(INTDIR)\Overhead.obj" \
	"$(INTDIR)\PATHAI.obj" \
	"$(INTDIR)\Points.obj" \
	"$(INTDIR)\QARRAY.obj" \
	"$(INTDIR)\Real Time Input.obj" \
	"$(INTDIR)\Rotting Corpses.obj" \
	"$(INTDIR)\ShopKeeper Interface.obj" \
	"$(INTDIR)\SkillCheck.obj" \
	"$(INTDIR)\Soldier Add.obj" \
	"$(INTDIR)\Soldier Ani.obj" \
	"$(INTDIR)\Soldier Control.obj" \
	"$(INTDIR)\Soldier Create.obj" \
	"$(INTDIR)\Soldier Find.obj" \
	"$(INTDIR)\Soldier Init List.obj" \
	"$(INTDIR)\Soldier Profile.obj" \
	"$(INTDIR)\Soldier Tile.obj" \
	"$(INTDIR)\Spread Burst.obj" \
	"$(INTDIR)\Squads.obj" \
	"$(INTDIR)\Strategic Exit GUI.obj" \
	"$(INTDIR)\Structure Wrap.obj" \
	"$(INTDIR)\Tactical Save.obj" \
	"$(INTDIR)\Tactical Turns.obj" \
	"$(INTDIR)\TeamTurns.obj" \
	"$(INTDIR)\Turn Based Input.obj" \
	"$(INTDIR)\UI Cursors.obj" \
	"$(INTDIR)\Vehicles.obj" \
	"$(INTDIR)\Weapons.obj" \
	"$(INTDIR)\World Items.obj"

"$(OUTDIR)\Tactical.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("Tactical.dep")
!INCLUDE "Tactical.dep"
!ELSE 
!MESSAGE Warning: cannot find "Tactical.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Tactical - Win32 Release" || "$(CFG)" == "Tactical - Win32 Debug" || "$(CFG)" == "Tactical - Win32 Release with Debug Info" || "$(CFG)" == "Tactical - Win32 Bounds Checker" || "$(CFG)" == "Tactical - Win32 Debug Demo" || "$(CFG)" == "Tactical - Win32 Release Demo" || "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info" || "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"
SOURCE=".\Air Raid.cpp"

"$(INTDIR)\Air Raid.obj"	"$(INTDIR)\Air Raid.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Animation Cache.cpp"

"$(INTDIR)\Animation Cache.obj"	"$(INTDIR)\Animation Cache.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Animation Control.cpp"

"$(INTDIR)\Animation Control.obj"	"$(INTDIR)\Animation Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Animation Data.cpp"

"$(INTDIR)\Animation Data.obj"	"$(INTDIR)\Animation Data.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Arms Dealer Init.cpp"

"$(INTDIR)\Arms Dealer Init.obj"	"$(INTDIR)\Arms Dealer Init.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ArmsDealerInvInit.cpp

"$(INTDIR)\ArmsDealerInvInit.obj"	"$(INTDIR)\ArmsDealerInvInit.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Auto Bandage.cpp"

"$(INTDIR)\Auto Bandage.obj"	"$(INTDIR)\Auto Bandage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Boxing.cpp

"$(INTDIR)\Boxing.obj"	"$(INTDIR)\Boxing.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bullets.cpp

"$(INTDIR)\bullets.obj"	"$(INTDIR)\bullets.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Campaign.cpp

"$(INTDIR)\Campaign.obj"	"$(INTDIR)\Campaign.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Civ Quotes.cpp"

"$(INTDIR)\Civ Quotes.obj"	"$(INTDIR)\Civ Quotes.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Dialogue Control.cpp"

"$(INTDIR)\Dialogue Control.obj"	"$(INTDIR)\Dialogue Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DisplayCover.cpp

"$(INTDIR)\DisplayCover.obj"	"$(INTDIR)\DisplayCover.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Drugs And Alcohol.cpp"

"$(INTDIR)\Drugs And Alcohol.obj"	"$(INTDIR)\Drugs And Alcohol.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\End Game.cpp"

"$(INTDIR)\End Game.obj"	"$(INTDIR)\End Game.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Enemy Soldier Save.cpp"

"$(INTDIR)\Enemy Soldier Save.obj"	"$(INTDIR)\Enemy Soldier Save.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Faces.cpp

"$(INTDIR)\Faces.obj"	"$(INTDIR)\Faces.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\fov.cpp

"$(INTDIR)\fov.obj"	"$(INTDIR)\fov.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GAP.cpp

"$(INTDIR)\GAP.obj"	"$(INTDIR)\GAP.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Handle Doors.cpp"

!IF  "$(CFG)" == "Tactical - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Z7 /Od /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Handle Doors.obj"	"$(INTDIR)\Handle Doors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=".\Handle Items.cpp"

"$(INTDIR)\Handle Items.obj"	"$(INTDIR)\Handle Items.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Handle UI Plan.cpp"

"$(INTDIR)\Handle UI Plan.obj"	"$(INTDIR)\Handle UI Plan.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Handle UI.cpp"

"$(INTDIR)\Handle UI.obj"	"$(INTDIR)\Handle UI.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Interface Control.cpp"

"$(INTDIR)\Interface Control.obj"	"$(INTDIR)\Interface Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Interface Cursors.cpp"

"$(INTDIR)\Interface Cursors.obj"	"$(INTDIR)\Interface Cursors.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Interface Dialogue.cpp"

"$(INTDIR)\Interface Dialogue.obj"	"$(INTDIR)\Interface Dialogue.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Interface Items.cpp"

"$(INTDIR)\Interface Items.obj"	"$(INTDIR)\Interface Items.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Interface Panels.cpp"

"$(INTDIR)\Interface Panels.obj"	"$(INTDIR)\Interface Panels.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Interface Utils.cpp"

"$(INTDIR)\Interface Utils.obj"	"$(INTDIR)\Interface Utils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Interface.cpp

"$(INTDIR)\Interface.obj"	"$(INTDIR)\Interface.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Inventory Choosing.cpp"

"$(INTDIR)\Inventory Choosing.obj"	"$(INTDIR)\Inventory Choosing.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Items.cpp

"$(INTDIR)\Items.obj"	"$(INTDIR)\Items.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Keys.cpp

"$(INTDIR)\Keys.obj"	"$(INTDIR)\Keys.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LOS.cpp

!IF  "$(CFG)" == "Tactical - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /Od /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /Od /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /Od /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\LOS.obj"	"$(INTDIR)\LOS.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=".\Map Information.cpp"

"$(INTDIR)\Map Information.obj"	"$(INTDIR)\Map Information.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Merc Entering.cpp"

"$(INTDIR)\Merc Entering.obj"	"$(INTDIR)\Merc Entering.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Merc Hiring.cpp"

"$(INTDIR)\Merc Hiring.obj"	"$(INTDIR)\Merc Hiring.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Militia Control.cpp"

"$(INTDIR)\Militia Control.obj"	"$(INTDIR)\Militia Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Morale.cpp

"$(INTDIR)\Morale.obj"	"$(INTDIR)\Morale.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\opplist.cpp

"$(INTDIR)\opplist.obj"	"$(INTDIR)\opplist.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Overhead.cpp

!IF  "$(CFG)" == "Tactical - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Z7 /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Overhead.obj"	"$(INTDIR)\Overhead.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\PATHAI.cpp

!IF  "$(CFG)" == "Tactical - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Z7 /Od /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PATHAI.obj"	"$(INTDIR)\PATHAI.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Points.cpp

"$(INTDIR)\Points.obj"	"$(INTDIR)\Points.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\QARRAY.cpp

"$(INTDIR)\QARRAY.obj"	"$(INTDIR)\QARRAY.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Real Time Input.cpp"

"$(INTDIR)\Real Time Input.obj"	"$(INTDIR)\Real Time Input.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Rotting Corpses.cpp"

"$(INTDIR)\Rotting Corpses.obj"	"$(INTDIR)\Rotting Corpses.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\ShopKeeper Interface.cpp"

"$(INTDIR)\ShopKeeper Interface.obj"	"$(INTDIR)\ShopKeeper Interface.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SkillCheck.cpp

"$(INTDIR)\SkillCheck.obj"	"$(INTDIR)\SkillCheck.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Soldier Add.cpp"

"$(INTDIR)\Soldier Add.obj"	"$(INTDIR)\Soldier Add.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Soldier Ani.cpp"

"$(INTDIR)\Soldier Ani.obj"	"$(INTDIR)\Soldier Ani.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Soldier Control.cpp"

"$(INTDIR)\Soldier Control.obj"	"$(INTDIR)\Soldier Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Soldier Create.cpp"

"$(INTDIR)\Soldier Create.obj"	"$(INTDIR)\Soldier Create.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Soldier Find.cpp"

"$(INTDIR)\Soldier Find.obj"	"$(INTDIR)\Soldier Find.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Soldier Init List.cpp"

!IF  "$(CFG)" == "Tactical - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Z7 /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Init List.obj"	"$(INTDIR)\Soldier Init List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=".\Soldier Profile.cpp"

"$(INTDIR)\Soldier Profile.obj"	"$(INTDIR)\Soldier Profile.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Soldier Tile.cpp"

!IF  "$(CFG)" == "Tactical - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Z7 /Od /I "..\TacticalAI" /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Tactical - Win32 Demo Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\TacticalAI" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\strategic" /I "\ja2\build\Laptop" /I "\ja2\build" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "PRECOMPILEDHEADERS" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Tactical.pch" /YX"Tactical All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Soldier Tile.obj"	"$(INTDIR)\Soldier Tile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=".\Spread Burst.cpp"

"$(INTDIR)\Spread Burst.obj"	"$(INTDIR)\Spread Burst.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Squads.cpp

"$(INTDIR)\Squads.obj"	"$(INTDIR)\Squads.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Strategic Exit GUI.cpp"

"$(INTDIR)\Strategic Exit GUI.obj"	"$(INTDIR)\Strategic Exit GUI.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Structure Wrap.cpp"

"$(INTDIR)\Structure Wrap.obj"	"$(INTDIR)\Structure Wrap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Tactical Save.cpp"

"$(INTDIR)\Tactical Save.obj"	"$(INTDIR)\Tactical Save.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Tactical Turns.cpp"

"$(INTDIR)\Tactical Turns.obj"	"$(INTDIR)\Tactical Turns.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TeamTurns.cpp

"$(INTDIR)\TeamTurns.obj"	"$(INTDIR)\TeamTurns.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Turn Based Input.cpp"

"$(INTDIR)\Turn Based Input.obj"	"$(INTDIR)\Turn Based Input.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\UI Cursors.cpp"

"$(INTDIR)\UI Cursors.obj"	"$(INTDIR)\UI Cursors.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Vehicles.cpp

"$(INTDIR)\Vehicles.obj"	"$(INTDIR)\Vehicles.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Weapons.cpp

"$(INTDIR)\Weapons.obj"	"$(INTDIR)\Weapons.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\World Items.cpp"

"$(INTDIR)\World Items.obj"	"$(INTDIR)\World Items.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

