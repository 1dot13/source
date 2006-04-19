# Microsoft Developer Studio Generated NMAKE File, Based on Laptop.dsp
!IF "$(CFG)" == ""
CFG=Laptop - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to Laptop - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "Laptop - Win32 Release" && "$(CFG)" != "Laptop - Win32 Debug" && "$(CFG)" != "Laptop - Win32 Release with Debug Info" && "$(CFG)" != "Laptop - Win32 Bounds Checker" && "$(CFG)" != "Laptop - Win32 Debug Demo" && "$(CFG)" != "Laptop - Win32 Release Demo" && "$(CFG)" != "Laptop - Win32 Demo Release with Debug Info" && "$(CFG)" != "Laptop - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Laptop - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I ".\\" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I ".\\" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Laptop - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Editor" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Laptop - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Laptop - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Editor" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Laptop.lib" "$(OUTDIR)\Laptop.bsc"


CLEAN :
	-@erase "$(INTDIR)\aim.obj"
	-@erase "$(INTDIR)\aim.sbr"
	-@erase "$(INTDIR)\AimArchives.obj"
	-@erase "$(INTDIR)\AimArchives.sbr"
	-@erase "$(INTDIR)\AimFacialIndex.obj"
	-@erase "$(INTDIR)\AimFacialIndex.sbr"
	-@erase "$(INTDIR)\AimHistory.obj"
	-@erase "$(INTDIR)\AimHistory.sbr"
	-@erase "$(INTDIR)\AimLinks.obj"
	-@erase "$(INTDIR)\AimLinks.sbr"
	-@erase "$(INTDIR)\AimMembers.obj"
	-@erase "$(INTDIR)\AimMembers.sbr"
	-@erase "$(INTDIR)\AimPolicies.obj"
	-@erase "$(INTDIR)\AimPolicies.sbr"
	-@erase "$(INTDIR)\AimSort.obj"
	-@erase "$(INTDIR)\AimSort.sbr"
	-@erase "$(INTDIR)\BobbyR.obj"
	-@erase "$(INTDIR)\BobbyR.sbr"
	-@erase "$(INTDIR)\BobbyRAmmo.obj"
	-@erase "$(INTDIR)\BobbyRAmmo.sbr"
	-@erase "$(INTDIR)\BobbyRArmour.obj"
	-@erase "$(INTDIR)\BobbyRArmour.sbr"
	-@erase "$(INTDIR)\BobbyRGuns.obj"
	-@erase "$(INTDIR)\BobbyRGuns.sbr"
	-@erase "$(INTDIR)\BobbyRMailOrder.obj"
	-@erase "$(INTDIR)\BobbyRMailOrder.sbr"
	-@erase "$(INTDIR)\BobbyRMisc.obj"
	-@erase "$(INTDIR)\BobbyRMisc.sbr"
	-@erase "$(INTDIR)\BobbyRShipments.obj"
	-@erase "$(INTDIR)\BobbyRShipments.sbr"
	-@erase "$(INTDIR)\BobbyRUsed.obj"
	-@erase "$(INTDIR)\BobbyRUsed.sbr"
	-@erase "$(INTDIR)\BrokenLink.obj"
	-@erase "$(INTDIR)\BrokenLink.sbr"
	-@erase "$(INTDIR)\CharProfile.obj"
	-@erase "$(INTDIR)\CharProfile.sbr"
	-@erase "$(INTDIR)\email.obj"
	-@erase "$(INTDIR)\email.sbr"
	-@erase "$(INTDIR)\files.obj"
	-@erase "$(INTDIR)\files.sbr"
	-@erase "$(INTDIR)\finances.obj"
	-@erase "$(INTDIR)\finances.sbr"
	-@erase "$(INTDIR)\florist Cards.obj"
	-@erase "$(INTDIR)\florist Cards.sbr"
	-@erase "$(INTDIR)\florist Gallery.obj"
	-@erase "$(INTDIR)\florist Gallery.sbr"
	-@erase "$(INTDIR)\florist Order Form.obj"
	-@erase "$(INTDIR)\florist Order Form.sbr"
	-@erase "$(INTDIR)\florist.obj"
	-@erase "$(INTDIR)\florist.sbr"
	-@erase "$(INTDIR)\funeral.obj"
	-@erase "$(INTDIR)\funeral.sbr"
	-@erase "$(INTDIR)\history.obj"
	-@erase "$(INTDIR)\history.sbr"
	-@erase "$(INTDIR)\IMP AboutUs.obj"
	-@erase "$(INTDIR)\IMP AboutUs.sbr"
	-@erase "$(INTDIR)\IMP Attribute Entrance.obj"
	-@erase "$(INTDIR)\IMP Attribute Entrance.sbr"
	-@erase "$(INTDIR)\IMP Attribute Finish.obj"
	-@erase "$(INTDIR)\IMP Attribute Finish.sbr"
	-@erase "$(INTDIR)\IMP Attribute Selection.obj"
	-@erase "$(INTDIR)\IMP Attribute Selection.sbr"
	-@erase "$(INTDIR)\IMP Begin Screen.obj"
	-@erase "$(INTDIR)\IMP Begin Screen.sbr"
	-@erase "$(INTDIR)\IMP Compile Character.obj"
	-@erase "$(INTDIR)\IMP Compile Character.sbr"
	-@erase "$(INTDIR)\IMP Confirm.obj"
	-@erase "$(INTDIR)\IMP Confirm.sbr"
	-@erase "$(INTDIR)\IMP Finish.obj"
	-@erase "$(INTDIR)\IMP Finish.sbr"
	-@erase "$(INTDIR)\IMP HomePage.obj"
	-@erase "$(INTDIR)\IMP HomePage.sbr"
	-@erase "$(INTDIR)\IMP MainPage.obj"
	-@erase "$(INTDIR)\IMP MainPage.sbr"
	-@erase "$(INTDIR)\IMP Personality Entrance.obj"
	-@erase "$(INTDIR)\IMP Personality Entrance.sbr"
	-@erase "$(INTDIR)\IMP Personality Finish.obj"
	-@erase "$(INTDIR)\IMP Personality Finish.sbr"
	-@erase "$(INTDIR)\IMP Personality Quiz.obj"
	-@erase "$(INTDIR)\IMP Personality Quiz.sbr"
	-@erase "$(INTDIR)\IMP Portraits.obj"
	-@erase "$(INTDIR)\IMP Portraits.sbr"
	-@erase "$(INTDIR)\IMP Text System.obj"
	-@erase "$(INTDIR)\IMP Text System.sbr"
	-@erase "$(INTDIR)\IMP Voices.obj"
	-@erase "$(INTDIR)\IMP Voices.sbr"
	-@erase "$(INTDIR)\IMPVideoObjects.obj"
	-@erase "$(INTDIR)\IMPVideoObjects.sbr"
	-@erase "$(INTDIR)\insurance Comments.obj"
	-@erase "$(INTDIR)\insurance Comments.sbr"
	-@erase "$(INTDIR)\insurance Contract.obj"
	-@erase "$(INTDIR)\insurance Contract.sbr"
	-@erase "$(INTDIR)\insurance Info.obj"
	-@erase "$(INTDIR)\insurance Info.sbr"
	-@erase "$(INTDIR)\insurance.obj"
	-@erase "$(INTDIR)\insurance.sbr"
	-@erase "$(INTDIR)\laptop.obj"
	-@erase "$(INTDIR)\laptop.sbr"
	-@erase "$(INTDIR)\mercs Account.obj"
	-@erase "$(INTDIR)\mercs Account.sbr"
	-@erase "$(INTDIR)\mercs Files.obj"
	-@erase "$(INTDIR)\mercs Files.sbr"
	-@erase "$(INTDIR)\mercs No Account.obj"
	-@erase "$(INTDIR)\mercs No Account.sbr"
	-@erase "$(INTDIR)\mercs.obj"
	-@erase "$(INTDIR)\mercs.sbr"
	-@erase "$(INTDIR)\personnel.obj"
	-@erase "$(INTDIR)\personnel.sbr"
	-@erase "$(INTDIR)\sirtech.obj"
	-@erase "$(INTDIR)\sirtech.sbr"
	-@erase "$(INTDIR)\Store Inventory.obj"
	-@erase "$(INTDIR)\Store Inventory.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Laptop.bsc"
	-@erase "$(OUTDIR)\Laptop.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Laptop.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\aim.sbr" \
	"$(INTDIR)\AimArchives.sbr" \
	"$(INTDIR)\AimFacialIndex.sbr" \
	"$(INTDIR)\AimHistory.sbr" \
	"$(INTDIR)\AimLinks.sbr" \
	"$(INTDIR)\AimMembers.sbr" \
	"$(INTDIR)\AimPolicies.sbr" \
	"$(INTDIR)\AimSort.sbr" \
	"$(INTDIR)\BobbyR.sbr" \
	"$(INTDIR)\BobbyRAmmo.sbr" \
	"$(INTDIR)\BobbyRArmour.sbr" \
	"$(INTDIR)\BobbyRGuns.sbr" \
	"$(INTDIR)\BobbyRMailOrder.sbr" \
	"$(INTDIR)\BobbyRMisc.sbr" \
	"$(INTDIR)\BobbyRShipments.sbr" \
	"$(INTDIR)\BobbyRUsed.sbr" \
	"$(INTDIR)\BrokenLink.sbr" \
	"$(INTDIR)\CharProfile.sbr" \
	"$(INTDIR)\email.sbr" \
	"$(INTDIR)\files.sbr" \
	"$(INTDIR)\finances.sbr" \
	"$(INTDIR)\florist Cards.sbr" \
	"$(INTDIR)\florist Gallery.sbr" \
	"$(INTDIR)\florist Order Form.sbr" \
	"$(INTDIR)\florist.sbr" \
	"$(INTDIR)\funeral.sbr" \
	"$(INTDIR)\history.sbr" \
	"$(INTDIR)\IMP AboutUs.sbr" \
	"$(INTDIR)\IMP Attribute Entrance.sbr" \
	"$(INTDIR)\IMP Attribute Finish.sbr" \
	"$(INTDIR)\IMP Attribute Selection.sbr" \
	"$(INTDIR)\IMP Begin Screen.sbr" \
	"$(INTDIR)\IMP Compile Character.sbr" \
	"$(INTDIR)\IMP Confirm.sbr" \
	"$(INTDIR)\IMP Finish.sbr" \
	"$(INTDIR)\IMP HomePage.sbr" \
	"$(INTDIR)\IMP MainPage.sbr" \
	"$(INTDIR)\IMP Personality Entrance.sbr" \
	"$(INTDIR)\IMP Personality Finish.sbr" \
	"$(INTDIR)\IMP Personality Quiz.sbr" \
	"$(INTDIR)\IMP Portraits.sbr" \
	"$(INTDIR)\IMP Text System.sbr" \
	"$(INTDIR)\IMP Voices.sbr" \
	"$(INTDIR)\IMPVideoObjects.sbr" \
	"$(INTDIR)\insurance Comments.sbr" \
	"$(INTDIR)\insurance Contract.sbr" \
	"$(INTDIR)\insurance Info.sbr" \
	"$(INTDIR)\insurance.sbr" \
	"$(INTDIR)\laptop.sbr" \
	"$(INTDIR)\mercs Account.sbr" \
	"$(INTDIR)\mercs Files.sbr" \
	"$(INTDIR)\mercs No Account.sbr" \
	"$(INTDIR)\mercs.sbr" \
	"$(INTDIR)\personnel.sbr" \
	"$(INTDIR)\sirtech.sbr" \
	"$(INTDIR)\Store Inventory.sbr"

"$(OUTDIR)\Laptop.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Laptop.lib" 
LIB32_OBJS= \
	"$(INTDIR)\aim.obj" \
	"$(INTDIR)\AimArchives.obj" \
	"$(INTDIR)\AimFacialIndex.obj" \
	"$(INTDIR)\AimHistory.obj" \
	"$(INTDIR)\AimLinks.obj" \
	"$(INTDIR)\AimMembers.obj" \
	"$(INTDIR)\AimPolicies.obj" \
	"$(INTDIR)\AimSort.obj" \
	"$(INTDIR)\BobbyR.obj" \
	"$(INTDIR)\BobbyRAmmo.obj" \
	"$(INTDIR)\BobbyRArmour.obj" \
	"$(INTDIR)\BobbyRGuns.obj" \
	"$(INTDIR)\BobbyRMailOrder.obj" \
	"$(INTDIR)\BobbyRMisc.obj" \
	"$(INTDIR)\BobbyRShipments.obj" \
	"$(INTDIR)\BobbyRUsed.obj" \
	"$(INTDIR)\BrokenLink.obj" \
	"$(INTDIR)\CharProfile.obj" \
	"$(INTDIR)\email.obj" \
	"$(INTDIR)\files.obj" \
	"$(INTDIR)\finances.obj" \
	"$(INTDIR)\florist Cards.obj" \
	"$(INTDIR)\florist Gallery.obj" \
	"$(INTDIR)\florist Order Form.obj" \
	"$(INTDIR)\florist.obj" \
	"$(INTDIR)\funeral.obj" \
	"$(INTDIR)\history.obj" \
	"$(INTDIR)\IMP AboutUs.obj" \
	"$(INTDIR)\IMP Attribute Entrance.obj" \
	"$(INTDIR)\IMP Attribute Finish.obj" \
	"$(INTDIR)\IMP Attribute Selection.obj" \
	"$(INTDIR)\IMP Begin Screen.obj" \
	"$(INTDIR)\IMP Compile Character.obj" \
	"$(INTDIR)\IMP Confirm.obj" \
	"$(INTDIR)\IMP Finish.obj" \
	"$(INTDIR)\IMP HomePage.obj" \
	"$(INTDIR)\IMP MainPage.obj" \
	"$(INTDIR)\IMP Personality Entrance.obj" \
	"$(INTDIR)\IMP Personality Finish.obj" \
	"$(INTDIR)\IMP Personality Quiz.obj" \
	"$(INTDIR)\IMP Portraits.obj" \
	"$(INTDIR)\IMP Text System.obj" \
	"$(INTDIR)\IMP Voices.obj" \
	"$(INTDIR)\IMPVideoObjects.obj" \
	"$(INTDIR)\insurance Comments.obj" \
	"$(INTDIR)\insurance Contract.obj" \
	"$(INTDIR)\insurance Info.obj" \
	"$(INTDIR)\insurance.obj" \
	"$(INTDIR)\laptop.obj" \
	"$(INTDIR)\mercs Account.obj" \
	"$(INTDIR)\mercs Files.obj" \
	"$(INTDIR)\mercs No Account.obj" \
	"$(INTDIR)\mercs.obj" \
	"$(INTDIR)\personnel.obj" \
	"$(INTDIR)\sirtech.obj" \
	"$(INTDIR)\Store Inventory.obj"

"$(OUTDIR)\Laptop.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("Laptop.dep")
!INCLUDE "Laptop.dep"
!ELSE 
!MESSAGE Warning: cannot find "Laptop.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Laptop - Win32 Release" || "$(CFG)" == "Laptop - Win32 Debug" || "$(CFG)" == "Laptop - Win32 Release with Debug Info" || "$(CFG)" == "Laptop - Win32 Bounds Checker" || "$(CFG)" == "Laptop - Win32 Debug Demo" || "$(CFG)" == "Laptop - Win32 Release Demo" || "$(CFG)" == "Laptop - Win32 Demo Release with Debug Info" || "$(CFG)" == "Laptop - Win32 Demo Bounds Checker"
SOURCE=.\aim.cpp

"$(INTDIR)\aim.obj"	"$(INTDIR)\aim.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AimArchives.cpp

"$(INTDIR)\AimArchives.obj"	"$(INTDIR)\AimArchives.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AimFacialIndex.cpp

"$(INTDIR)\AimFacialIndex.obj"	"$(INTDIR)\AimFacialIndex.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AimHistory.cpp

"$(INTDIR)\AimHistory.obj"	"$(INTDIR)\AimHistory.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AimLinks.cpp

"$(INTDIR)\AimLinks.obj"	"$(INTDIR)\AimLinks.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AimMembers.cpp

"$(INTDIR)\AimMembers.obj"	"$(INTDIR)\AimMembers.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AimPolicies.cpp

"$(INTDIR)\AimPolicies.obj"	"$(INTDIR)\AimPolicies.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\AimSort.cpp

"$(INTDIR)\AimSort.obj"	"$(INTDIR)\AimSort.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyR.cpp

"$(INTDIR)\BobbyR.obj"	"$(INTDIR)\BobbyR.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyRAmmo.cpp

"$(INTDIR)\BobbyRAmmo.obj"	"$(INTDIR)\BobbyRAmmo.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyRArmour.cpp

"$(INTDIR)\BobbyRArmour.obj"	"$(INTDIR)\BobbyRArmour.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyRGuns.cpp

"$(INTDIR)\BobbyRGuns.obj"	"$(INTDIR)\BobbyRGuns.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyRMailOrder.cpp

"$(INTDIR)\BobbyRMailOrder.obj"	"$(INTDIR)\BobbyRMailOrder.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyRMisc.cpp

"$(INTDIR)\BobbyRMisc.obj"	"$(INTDIR)\BobbyRMisc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyRShipments.cpp

"$(INTDIR)\BobbyRShipments.obj"	"$(INTDIR)\BobbyRShipments.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BobbyRUsed.cpp

"$(INTDIR)\BobbyRUsed.obj"	"$(INTDIR)\BobbyRUsed.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BrokenLink.cpp

"$(INTDIR)\BrokenLink.obj"	"$(INTDIR)\BrokenLink.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CharProfile.cpp

"$(INTDIR)\CharProfile.obj"	"$(INTDIR)\CharProfile.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\email.cpp

"$(INTDIR)\email.obj"	"$(INTDIR)\email.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\files.cpp

"$(INTDIR)\files.obj"	"$(INTDIR)\files.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\finances.cpp

"$(INTDIR)\finances.obj"	"$(INTDIR)\finances.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\florist Cards.cpp"

"$(INTDIR)\florist Cards.obj"	"$(INTDIR)\florist Cards.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\florist Gallery.cpp"

"$(INTDIR)\florist Gallery.obj"	"$(INTDIR)\florist Gallery.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\florist Order Form.cpp"

"$(INTDIR)\florist Order Form.obj"	"$(INTDIR)\florist Order Form.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\florist.cpp

"$(INTDIR)\florist.obj"	"$(INTDIR)\florist.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\funeral.cpp

"$(INTDIR)\funeral.obj"	"$(INTDIR)\funeral.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\history.cpp

"$(INTDIR)\history.obj"	"$(INTDIR)\history.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP AboutUs.cpp"

"$(INTDIR)\IMP AboutUs.obj"	"$(INTDIR)\IMP AboutUs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Attribute Entrance.cpp"

"$(INTDIR)\IMP Attribute Entrance.obj"	"$(INTDIR)\IMP Attribute Entrance.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Attribute Finish.cpp"

"$(INTDIR)\IMP Attribute Finish.obj"	"$(INTDIR)\IMP Attribute Finish.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Attribute Selection.cpp"

"$(INTDIR)\IMP Attribute Selection.obj"	"$(INTDIR)\IMP Attribute Selection.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Begin Screen.cpp"

"$(INTDIR)\IMP Begin Screen.obj"	"$(INTDIR)\IMP Begin Screen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Compile Character.cpp"

"$(INTDIR)\IMP Compile Character.obj"	"$(INTDIR)\IMP Compile Character.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Confirm.cpp"

"$(INTDIR)\IMP Confirm.obj"	"$(INTDIR)\IMP Confirm.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Finish.cpp"

"$(INTDIR)\IMP Finish.obj"	"$(INTDIR)\IMP Finish.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP HomePage.cpp"

"$(INTDIR)\IMP HomePage.obj"	"$(INTDIR)\IMP HomePage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP MainPage.cpp"

"$(INTDIR)\IMP MainPage.obj"	"$(INTDIR)\IMP MainPage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Personality Entrance.cpp"

"$(INTDIR)\IMP Personality Entrance.obj"	"$(INTDIR)\IMP Personality Entrance.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Personality Finish.cpp"

"$(INTDIR)\IMP Personality Finish.obj"	"$(INTDIR)\IMP Personality Finish.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Personality Quiz.cpp"

"$(INTDIR)\IMP Personality Quiz.obj"	"$(INTDIR)\IMP Personality Quiz.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Portraits.cpp"

"$(INTDIR)\IMP Portraits.obj"	"$(INTDIR)\IMP Portraits.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Text System.cpp"

"$(INTDIR)\IMP Text System.obj"	"$(INTDIR)\IMP Text System.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\IMP Voices.cpp"

"$(INTDIR)\IMP Voices.obj"	"$(INTDIR)\IMP Voices.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IMPVideoObjects.cpp

"$(INTDIR)\IMPVideoObjects.obj"	"$(INTDIR)\IMPVideoObjects.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\insurance Comments.cpp"

"$(INTDIR)\insurance Comments.obj"	"$(INTDIR)\insurance Comments.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\insurance Contract.cpp"

"$(INTDIR)\insurance Contract.obj"	"$(INTDIR)\insurance Contract.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\insurance Info.cpp"

"$(INTDIR)\insurance Info.obj"	"$(INTDIR)\insurance Info.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\insurance.cpp

"$(INTDIR)\insurance.obj"	"$(INTDIR)\insurance.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\laptop.cpp

"$(INTDIR)\laptop.obj"	"$(INTDIR)\laptop.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\mercs Account.cpp"

"$(INTDIR)\mercs Account.obj"	"$(INTDIR)\mercs Account.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\mercs Files.cpp"

"$(INTDIR)\mercs Files.obj"	"$(INTDIR)\mercs Files.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\mercs No Account.cpp"

"$(INTDIR)\mercs No Account.obj"	"$(INTDIR)\mercs No Account.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mercs.cpp

"$(INTDIR)\mercs.obj"	"$(INTDIR)\mercs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\personnel.cpp

!IF  "$(CFG)" == "Laptop - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I ".\\" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\Utils" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I ".\\" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Laptop - Win32 Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Editor" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Laptop - Win32 Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Laptop - Win32 Debug Demo"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Laptop - Win32 Release Demo"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Release with Debug Info"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\TileEngine" /I "..\Utils" /I "..\TacticalAI" /I "..\Editor" /I "..\Tactical" /I "..\strategic" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Laptop - Win32 Demo Bounds Checker"

CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\TileEngine" /I "\ja2\Build\Utils" /I "\ja2\build\TacticalAI" /I "\ja2\build\Communications" /I "\ja2\build\Editor" /I "\ja2\build\Tactical" /I "\ja2\build\strategic" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Laptop.pch" /YX"Laptop All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\personnel.obj"	"$(INTDIR)\personnel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\sirtech.cpp

"$(INTDIR)\sirtech.obj"	"$(INTDIR)\sirtech.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Store Inventory.cpp"

"$(INTDIR)\Store Inventory.obj"	"$(INTDIR)\Store Inventory.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

