# Microsoft Developer Studio Generated NMAKE File, Based on JA2.dsp
!IF "$(CFG)" == ""
CFG=ja2 - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to ja2 - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "ja2 - Win32 Release" && "$(CFG)" != "ja2 - Win32 Debug" && "$(CFG)" != "ja2 - Win32 Release with Debug Info" && "$(CFG)" != "ja2 - Win32 Bounds Checker" && "$(CFG)" != "ja2 - Win32 Debug Demo" && "$(CFG)" != "ja2 - Win32 Release Demo" && "$(CFG)" != "ja2 - Win32 Demo Release with Debug Info" && "$(CFG)" != "ja2 - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JA2.mak" CFG="ja2 - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ja2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ja2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ja2 - Win32 Release with Debug Info" (based on "Win32 (x86) Application")
!MESSAGE "ja2 - Win32 Bounds Checker" (based on "Win32 (x86) Application")
!MESSAGE "ja2 - Win32 Debug Demo" (based on "Win32 (x86) Application")
!MESSAGE "ja2 - Win32 Release Demo" (based on "Win32 (x86) Application")
!MESSAGE "ja2 - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Application")
!MESSAGE "ja2 - Win32 Demo Bounds Checker" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ja2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\ja2_cvs.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Release" "Laptop - Win32 Release" "Editor - Win32 Release" "TacticalAI - Win32 Release" "Utils - Win32 Release" "Tactical - Win32 Release" "Standard Gaming Platform - Win32 Release" "TileEngine - Win32 Release" "..\ja2_cvs.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 ReleaseCLEAN" "Standard Gaming Platform - Win32 ReleaseCLEAN" "Tactical - Win32 ReleaseCLEAN" "Utils - Win32 ReleaseCLEAN" "TacticalAI - Win32 ReleaseCLEAN" "Editor - Win32 ReleaseCLEAN" "Laptop - Win32 ReleaseCLEAN" "Strategic - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2_cvs.map"
	-@erase "..\ja2_cvs.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MT /W3 /GX /Zi /O2 /Ob2 /I ".\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Editor" /I ".\Strategic" /I ".\laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=libexpatMT.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib mss32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2_cvs.map" /machine:I386 /out:"..\ja2_cvs.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Release\TileEngine.lib" \
	".\Standard Gaming Platform\Release\Standard Gaming Platform.lib" \
	".\Tactical\Release\Tactical.lib" \
	".\Utils\Release\Utils.lib" \
	".\TacticalAI\Release\TacticalAI.lib" \
	".\Editor\Release\Editor.lib" \
	".\Laptop\Release\Laptop.lib" \
	".\Strategic\Release\Strategic.lib"

"..\ja2_cvs.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\ja2_cvs.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Debug" "Laptop - Win32 Debug" "Editor - Win32 Debug" "TacticalAI - Win32 Debug" "Utils - Win32 Debug" "Tactical - Win32 Debug" "Standard Gaming Platform - Win32 Debug" "TileEngine - Win32 Debug" "..\ja2_cvs.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 DebugCLEAN" "Standard Gaming Platform - Win32 DebugCLEAN" "Tactical - Win32 DebugCLEAN" "Utils - Win32 DebugCLEAN" "TacticalAI - Win32 DebugCLEAN" "Editor - Win32 DebugCLEAN" "Laptop - Win32 DebugCLEAN" "Strategic - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2_cvs.map"
	-@erase "..\ja2_cvs.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Editor" /I ".\Strategic" /I ".\laptop" /I ".\\" /I "..\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=libexpatMT.lib mss32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2_cvs.map" /debug /machine:I386 /nodefaultlib:"libcmt" /out:"..\ja2_cvs.exe" /libpath:".\..\..\Standard Gaming Platform\\" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Debug\TileEngine.lib" \
	".\Standard Gaming Platform\Debug\Standard Gaming Platform.lib" \
	".\Tactical\Debug\Tactical.lib" \
	".\Utils\Debug\Utils.lib" \
	".\TacticalAI\Debug\TacticalAI.lib" \
	".\Editor\Debug\Editor.lib" \
	".\Laptop\Debug\Laptop.lib" \
	".\Strategic\Debug\Strategic.lib"

"..\ja2_cvs.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\ja2_cvs.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Release with Debug Info" "Laptop - Win32 Release with Debug Info" "Editor - Win32 Release with Debug Info" "TacticalAI - Win32 Release with Debug Info" "Utils - Win32 Release with Debug Info" "Tactical - Win32 Release with Debug Info" "Standard Gaming Platform - Win32 Release with Debug Info" "TileEngine - Win32 Release with Debug Info" "..\ja2_cvs.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 Release with Debug InfoCLEAN" "Standard Gaming Platform - Win32 Release with Debug InfoCLEAN" "Tactical - Win32 Release with Debug InfoCLEAN" "Utils - Win32 Release with Debug InfoCLEAN" "TacticalAI - Win32 Release with Debug InfoCLEAN" "Editor - Win32 Release with Debug InfoCLEAN" "Laptop - Win32 Release with Debug InfoCLEAN" "Strategic - Win32 Release with Debug InfoCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2_cvs.map"
	-@erase "..\ja2_cvs.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MT /W4 /GX /Zi /O2 /I ".\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Communications" /I ".\Editor" /I ".\Strategic" /I ".\laptop" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=libexpatMT.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib mss32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2_cvs.map" /debug /machine:I386 /out:"..\ja2_cvs.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Release with Debug\TileEngine.lib" \
	".\Standard Gaming Platform\Release with Debug Info\Standard Gaming Platform.lib" \
	".\Tactical\Release with Debug\Tactical.lib" \
	".\Utils\Release with Debug\Utils.lib" \
	".\TacticalAI\Release with Debug\TacticalAI.lib" \
	".\Editor\Release with Debug\Editor.lib" \
	".\Laptop\Release with Debug\Laptop.lib" \
	".\Strategic\Release with Debug\Strategic.lib"

"..\ja2_cvs.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Bounds Checker" "Laptop - Win32 Bounds Checker" "Editor - Win32 Bounds Checker" "TacticalAI - Win32 Bounds Checker" "Utils - Win32 Bounds Checker" "Tactical - Win32 Bounds Checker" "Standard Gaming Platform - Win32 Bounds Checker" "TileEngine - Win32 Bounds Checker" "..\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 Bounds CheckerCLEAN" "Standard Gaming Platform - Win32 Bounds CheckerCLEAN" "Tactical - Win32 Bounds CheckerCLEAN" "Utils - Win32 Bounds CheckerCLEAN" "TacticalAI - Win32 Bounds CheckerCLEAN" "Editor - Win32 Bounds CheckerCLEAN" "Laptop - Win32 Bounds CheckerCLEAN" "Strategic - Win32 Bounds CheckerCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2.map"
	-@erase "..\ja2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Communications" /I ".\Editor" /I ".\Strategic" /I ".\Laptop" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=vtuneapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib mss32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2.map" /debug /machine:I386 /out:"\ja2\ja2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Bounds Checker\TileEngine.lib" \
	".\Standard Gaming Platform\Standar0\Standard Gaming Platform.lib" \
	".\Tactical\Bounds Checker\Tactical.lib" \
	".\Utils\Bounds Checker\Utils.lib" \
	".\TacticalAI\Bounds Checker\TacticalAI.lib" \
	".\Editor\Bounds Checker\Editor.lib" \
	".\Laptop\Bounds Checker\Laptop.lib" \
	".\Strategic\Bounds Checker\Strategic.lib"

"..\ja2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\ja2demo\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Debug Demo" "Laptop - Win32 Debug Demo" "Editor - Win32 Debug Demo" "TacticalAI - Win32 Debug Demo" "Utils - Win32 Debug Demo" "Tactical - Win32 Debug Demo" "Standard Gaming Platform - Win32 Debug Demo" "TileEngine - Win32 Debug Demo" "..\..\ja2demo\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 Debug DemoCLEAN" "Standard Gaming Platform - Win32 Debug DemoCLEAN" "Tactical - Win32 Debug DemoCLEAN" "Utils - Win32 Debug DemoCLEAN" "TacticalAI - Win32 Debug DemoCLEAN" "Editor - Win32 Debug DemoCLEAN" "Laptop - Win32 Debug DemoCLEAN" "Strategic - Win32 Debug DemoCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2.map"
	-@erase "..\..\ja2demo\ja2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Communications" /I ".\Editor" /I ".\Strategic" /I ".\Laptop" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib mss32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2.map" /debug /machine:I386 /out:"d:\ja2demo\ja2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Debug Demo\TileEngine.lib" \
	".\Standard Gaming Platform\Standard\Standard Gaming Platform.lib" \
	".\Tactical\Debug Demo\Tactical.lib" \
	".\Utils\Debug Demo\Utils.lib" \
	".\TacticalAI\Debug Demo\TacticalAI.lib" \
	".\Editor\Debug Demo\Editor.lib" \
	".\Laptop\Debug Demo\Laptop.lib" \
	".\Strategic\Debug Demo\Strategic.lib"

"..\..\ja2demo\ja2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\ja2demo\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Release Demo" "Laptop - Win32 Release Demo" "Editor - Win32 Release Demo" "TacticalAI - Win32 Release Demo" "Utils - Win32 Release Demo" "Tactical - Win32 Release Demo" "Standard Gaming Platform - Win32 Release Demo" "TileEngine - Win32 Release Demo" "..\..\ja2demo\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 Release DemoCLEAN" "Standard Gaming Platform - Win32 Release DemoCLEAN" "Tactical - Win32 Release DemoCLEAN" "Utils - Win32 Release DemoCLEAN" "TacticalAI - Win32 Release DemoCLEAN" "Editor - Win32 Release DemoCLEAN" "Laptop - Win32 Release DemoCLEAN" "Strategic - Win32 Release DemoCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2.map"
	-@erase "..\..\ja2demo\ja2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MT /W4 /GX /Zi /O2 /I "\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Communications" /I ".\Editor" /I ".\Strategic" /I ".\laptop" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib mss32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2.map" /debug /machine:I386 /out:"d:\ja2demo\ja2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Release Demo\TileEngine.lib" \
	".\Standard Gaming Platform\Standar1\Standard Gaming Platform.lib" \
	".\Tactical\Release Demo\Tactical.lib" \
	".\Utils\Release Demo\Utils.lib" \
	".\TacticalAI\Release Demo\TacticalAI.lib" \
	".\Editor\Release Demo\Editor.lib" \
	".\Laptop\Release Demo\Laptop.lib" \
	".\Strategic\Release Demo\Strategic.lib"

"..\..\ja2demo\ja2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\ja2demo\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Demo Release with Debug Info" "Laptop - Win32 Demo Release with Debug Info" "Editor - Win32 Demo Release with Debug Info" "TacticalAI - Win32 Demo Release with Debug Info" "Utils - Win32 Demo Release with Debug Info" "Tactical - Win32 Demo Release with Debug Info" "Standard Gaming Platform - Win32 Demo Release with Debug Info" "TileEngine - Win32 Demo Release with Debug Info" "..\..\ja2demo\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 Demo Release with Debug InfoCLEAN" "Standard Gaming Platform - Win32 Demo Release with Debug InfoCLEAN" "Tactical - Win32 Demo Release with Debug InfoCLEAN" "Utils - Win32 Demo Release with Debug InfoCLEAN" "TacticalAI - Win32 Demo Release with Debug InfoCLEAN" "Editor - Win32 Demo Release with Debug InfoCLEAN" "Laptop - Win32 Demo Release with Debug InfoCLEAN" "Strategic - Win32 Demo Release with Debug InfoCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2.map"
	-@erase "..\..\ja2demo\ja2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MT /W4 /GX /Zi /O2 /I ".\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Communications" /I ".\Editor" /I ".\Strategic" /I ".\laptop" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /O2b2 /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib mss32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2.map" /debug /machine:I386 /out:"d:\ja2demo\ja2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Demo Release with Debug\TileEngine.lib" \
	".\Standard Gaming Platform\Standar2\Standard Gaming Platform.lib" \
	".\Tactical\Demo Release with Debug\Tactical.lib" \
	".\Utils\Demo Release with Debug\Utils.lib" \
	".\TacticalAI\Demo Release with Debug\TacticalAI.lib" \
	".\Editor\Demo Release with Debug\Editor.lib" \
	".\Laptop\Demo Release with Debug\Laptop.lib" \
	".\Strategic\Demo Release with Debug\Strategic.lib"

"..\..\ja2demo\ja2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ELSE 

ALL : "Strategic - Win32 Demo Bounds Checker" "Laptop - Win32 Demo Bounds Checker" "Editor - Win32 Demo Bounds Checker" "TacticalAI - Win32 Demo Bounds Checker" "Utils - Win32 Demo Bounds Checker" "Tactical - Win32 Demo Bounds Checker" "TileEngine - Win32 Demo Bounds Checker" "..\..\ja2.exe" "$(OUTDIR)\JA2.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"TileEngine - Win32 Demo Bounds CheckerCLEAN" "Tactical - Win32 Demo Bounds CheckerCLEAN" "Utils - Win32 Demo Bounds CheckerCLEAN" "TacticalAI - Win32 Demo Bounds CheckerCLEAN" "Editor - Win32 Demo Bounds CheckerCLEAN" "Laptop - Win32 Demo Bounds CheckerCLEAN" "Strategic - Win32 Demo Bounds CheckerCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\aniviewscreen.obj"
	-@erase "$(INTDIR)\aniviewscreen.sbr"
	-@erase "$(INTDIR)\Credits.obj"
	-@erase "$(INTDIR)\Credits.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Fade Screen.obj"
	-@erase "$(INTDIR)\Fade Screen.sbr"
	-@erase "$(INTDIR)\GameInitOptionsScreen.obj"
	-@erase "$(INTDIR)\GameInitOptionsScreen.sbr"
	-@erase "$(INTDIR)\gameloop.obj"
	-@erase "$(INTDIR)\gameloop.sbr"
	-@erase "$(INTDIR)\gamescreen.obj"
	-@erase "$(INTDIR)\gamescreen.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GameVersion.obj"
	-@erase "$(INTDIR)\GameVersion.sbr"
	-@erase "$(INTDIR)\HelpScreen.obj"
	-@erase "$(INTDIR)\HelpScreen.sbr"
	-@erase "$(INTDIR)\Init.obj"
	-@erase "$(INTDIR)\Init.sbr"
	-@erase "$(INTDIR)\Intro.obj"
	-@erase "$(INTDIR)\Intro.sbr"
	-@erase "$(INTDIR)\JA2 Splash.obj"
	-@erase "$(INTDIR)\JA2 Splash.sbr"
	-@erase "$(INTDIR)\ja2.res"
	-@erase "$(INTDIR)\jascreens.obj"
	-@erase "$(INTDIR)\jascreens.sbr"
	-@erase "$(INTDIR)\Loading Screen.obj"
	-@erase "$(INTDIR)\Loading Screen.sbr"
	-@erase "$(INTDIR)\MainMenuScreen.obj"
	-@erase "$(INTDIR)\MainMenuScreen.sbr"
	-@erase "$(INTDIR)\MessageBoxScreen.obj"
	-@erase "$(INTDIR)\MessageBoxScreen.sbr"
	-@erase "$(INTDIR)\Options Screen.obj"
	-@erase "$(INTDIR)\Options Screen.sbr"
	-@erase "$(INTDIR)\SaveLoadGame.obj"
	-@erase "$(INTDIR)\SaveLoadGame.sbr"
	-@erase "$(INTDIR)\SaveLoadScreen.obj"
	-@erase "$(INTDIR)\SaveLoadScreen.sbr"
	-@erase "$(INTDIR)\SCREENS.obj"
	-@erase "$(INTDIR)\SCREENS.sbr"
	-@erase "$(INTDIR)\Sys Globals.obj"
	-@erase "$(INTDIR)\Sys Globals.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\JA2.bsc"
	-@erase "$(OUTDIR)\ja2.map"
	-@erase "..\..\ja2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "\Standard Gaming Platform" /I ".\TileEngine" /I ".\Tactical" /I ".\Utils" /I ".\TacticalAI" /I ".\Communications" /I ".\Editor" /I ".\Strategic" /I ".\Laptop" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\JA2.pch" /YX"JA2 All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ja2.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\JA2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\aniviewscreen.sbr" \
	"$(INTDIR)\Credits.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Fade Screen.sbr" \
	"$(INTDIR)\GameInitOptionsScreen.sbr" \
	"$(INTDIR)\gameloop.sbr" \
	"$(INTDIR)\gamescreen.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GameVersion.sbr" \
	"$(INTDIR)\HelpScreen.sbr" \
	"$(INTDIR)\Init.sbr" \
	"$(INTDIR)\Intro.sbr" \
	"$(INTDIR)\JA2 Splash.sbr" \
	"$(INTDIR)\jascreens.sbr" \
	"$(INTDIR)\Loading Screen.sbr" \
	"$(INTDIR)\MainMenuScreen.sbr" \
	"$(INTDIR)\MessageBoxScreen.sbr" \
	"$(INTDIR)\Options Screen.sbr" \
	"$(INTDIR)\SaveLoadGame.sbr" \
	"$(INTDIR)\SaveLoadScreen.sbr" \
	"$(INTDIR)\SCREENS.sbr" \
	"$(INTDIR)\Sys Globals.sbr"

"$(OUTDIR)\JA2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib smackw32.lib mss32.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\ja2.map" /debug /machine:I386 /out:"..\..\ja2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\aniviewscreen.obj" \
	"$(INTDIR)\Credits.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Fade Screen.obj" \
	"$(INTDIR)\GameInitOptionsScreen.obj" \
	"$(INTDIR)\gameloop.obj" \
	"$(INTDIR)\gamescreen.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GameVersion.obj" \
	"$(INTDIR)\HelpScreen.obj" \
	"$(INTDIR)\Init.obj" \
	"$(INTDIR)\Intro.obj" \
	"$(INTDIR)\JA2 Splash.obj" \
	"$(INTDIR)\jascreens.obj" \
	"$(INTDIR)\Loading Screen.obj" \
	"$(INTDIR)\MainMenuScreen.obj" \
	"$(INTDIR)\MessageBoxScreen.obj" \
	"$(INTDIR)\Options Screen.obj" \
	"$(INTDIR)\SaveLoadGame.obj" \
	"$(INTDIR)\SaveLoadScreen.obj" \
	"$(INTDIR)\SCREENS.obj" \
	"$(INTDIR)\Sys Globals.obj" \
	"$(INTDIR)\ja2.res" \
	".\TileEngine\Demo Bounds Checker\TileEngine.lib" \
	".\Tactical\Demo Bounds Checker\Tactical.lib" \
	".\Utils\Demo Bounds Checker\Utils.lib" \
	".\TacticalAI\Demo Bounds Checker\TacticalAI.lib" \
	".\Editor\Demo Bounds Checker\Editor.lib" \
	".\Laptop\Demo Bounds Checker\Laptop.lib" \
	".\Strategic\Demo Bounds Checker\Strategic.lib"

"..\..\ja2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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
!IF EXISTS("JA2.dep")
!INCLUDE "JA2.dep"
!ELSE 
!MESSAGE Warning: cannot find "JA2.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ja2 - Win32 Release" || "$(CFG)" == "ja2 - Win32 Debug" || "$(CFG)" == "ja2 - Win32 Release with Debug Info" || "$(CFG)" == "ja2 - Win32 Bounds Checker" || "$(CFG)" == "ja2 - Win32 Debug Demo" || "$(CFG)" == "ja2 - Win32 Release Demo" || "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info" || "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"
SOURCE=.\Utils\_Ja25GermanText.cpp

"$(INTDIR)\_Ja25GermanText.obj"	"$(INTDIR)\_Ja25GermanText.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\aniviewscreen.cpp

"$(INTDIR)\aniviewscreen.obj"	"$(INTDIR)\aniviewscreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Credits.cpp

"$(INTDIR)\Credits.obj"	"$(INTDIR)\Credits.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Utils\Cursors.cpp

"$(INTDIR)\Cursors.obj"	"$(INTDIR)\Cursors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\Fade Screen.cpp"

"$(INTDIR)\Fade Screen.obj"	"$(INTDIR)\Fade Screen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameInitOptionsScreen.cpp

"$(INTDIR)\GameInitOptionsScreen.obj"	"$(INTDIR)\GameInitOptionsScreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gameloop.cpp

"$(INTDIR)\gameloop.obj"	"$(INTDIR)\gameloop.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gamescreen.cpp

"$(INTDIR)\gamescreen.obj"	"$(INTDIR)\gamescreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameSettings.cpp

"$(INTDIR)\GameSettings.obj"	"$(INTDIR)\GameSettings.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameVersion.cpp

"$(INTDIR)\GameVersion.obj"	"$(INTDIR)\GameVersion.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\HelpScreen.cpp

"$(INTDIR)\HelpScreen.obj"	"$(INTDIR)\HelpScreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Init.cpp

"$(INTDIR)\Init.obj"	"$(INTDIR)\Init.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Intro.cpp

"$(INTDIR)\Intro.obj"	"$(INTDIR)\Intro.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\JA2 Splash.cpp"

"$(INTDIR)\JA2 Splash.obj"	"$(INTDIR)\JA2 Splash.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Res\ja2.rc

!IF  "$(CFG)" == "ja2 - Win32 Release"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"


"$(INTDIR)\ja2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\ja2.res" /i "Res" /i ".\Res" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=.\jascreens.cpp

"$(INTDIR)\jascreens.obj"	"$(INTDIR)\jascreens.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Loading Screen.cpp"

"$(INTDIR)\Loading Screen.obj"	"$(INTDIR)\Loading Screen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MainMenuScreen.cpp

"$(INTDIR)\MainMenuScreen.obj"	"$(INTDIR)\MainMenuScreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MessageBoxScreen.cpp

"$(INTDIR)\MessageBoxScreen.obj"	"$(INTDIR)\MessageBoxScreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Options Screen.cpp"

"$(INTDIR)\Options Screen.obj"	"$(INTDIR)\Options Screen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SaveLoadGame.cpp

"$(INTDIR)\SaveLoadGame.obj"	"$(INTDIR)\SaveLoadGame.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SaveLoadScreen.cpp

"$(INTDIR)\SaveLoadScreen.obj"	"$(INTDIR)\SaveLoadScreen.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SCREENS.cpp

"$(INTDIR)\SCREENS.obj"	"$(INTDIR)\SCREENS.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Sys Globals.cpp"

"$(INTDIR)\Sys Globals.obj"	"$(INTDIR)\Sys Globals.sbr" : $(SOURCE) "$(INTDIR)"


!IF  "$(CFG)" == "ja2 - Win32 Release"

"TileEngine - Win32 Release" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Release" 
   cd ".."

"TileEngine - Win32 ReleaseCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"TileEngine - Win32 Debug" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Debug" 
   cd ".."

"TileEngine - Win32 DebugCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"TileEngine - Win32 Release with Debug Info" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Release with Debug Info" 
   cd ".."

"TileEngine - Win32 Release with Debug InfoCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"TileEngine - Win32 Bounds Checker" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Bounds Checker" 
   cd ".."

"TileEngine - Win32 Bounds CheckerCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"TileEngine - Win32 Debug Demo" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Debug Demo" 
   cd ".."

"TileEngine - Win32 Debug DemoCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"TileEngine - Win32 Release Demo" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Release Demo" 
   cd ".."

"TileEngine - Win32 Release DemoCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"TileEngine - Win32 Demo Release with Debug Info" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Demo Release with Debug Info" 
   cd ".."

"TileEngine - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

"TileEngine - Win32 Demo Bounds Checker" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Demo Bounds Checker" 
   cd ".."

"TileEngine - Win32 Demo Bounds CheckerCLEAN" : 
   cd ".\TileEngine"
   $(MAKE) /$(MAKEFLAGS) /F .\TileEngine.mak CFG="TileEngine - Win32 Demo Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "ja2 - Win32 Release"

"Standard Gaming Platform - Win32 Release" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Release" 
   cd ".."

"Standard Gaming Platform - Win32 ReleaseCLEAN" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"Standard Gaming Platform - Win32 Debug" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Debug" 
   cd ".."

"Standard Gaming Platform - Win32 DebugCLEAN" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"Standard Gaming Platform - Win32 Release with Debug Info" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Release with Debug Info" 
   cd ".."

"Standard Gaming Platform - Win32 Release with Debug InfoCLEAN" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"Standard Gaming Platform - Win32 Bounds Checker" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Bounds Checker" 
   cd ".."

"Standard Gaming Platform - Win32 Bounds CheckerCLEAN" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"Standard Gaming Platform - Win32 Debug Demo" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Debug Demo" 
   cd ".."

"Standard Gaming Platform - Win32 Debug DemoCLEAN" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"Standard Gaming Platform - Win32 Release Demo" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Release Demo" 
   cd ".."

"Standard Gaming Platform - Win32 Release DemoCLEAN" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"Standard Gaming Platform - Win32 Demo Release with Debug Info" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Demo Release with Debug Info" 
   cd ".."

"Standard Gaming Platform - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\Standard Gaming Platform"
   $(MAKE) /$(MAKEFLAGS) /F ".\Standard Gaming Platform.mak" CFG="Standard Gaming Platform - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

!ENDIF 

!IF  "$(CFG)" == "ja2 - Win32 Release"

"Tactical - Win32 Release" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Release" 
   cd ".."

"Tactical - Win32 ReleaseCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"Tactical - Win32 Debug" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Debug" 
   cd ".."

"Tactical - Win32 DebugCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"Tactical - Win32 Release with Debug Info" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Release with Debug Info" 
   cd ".."

"Tactical - Win32 Release with Debug InfoCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"Tactical - Win32 Bounds Checker" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Bounds Checker" 
   cd ".."

"Tactical - Win32 Bounds CheckerCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"Tactical - Win32 Debug Demo" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Debug Demo" 
   cd ".."

"Tactical - Win32 Debug DemoCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"Tactical - Win32 Release Demo" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Release Demo" 
   cd ".."

"Tactical - Win32 Release DemoCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"Tactical - Win32 Demo Release with Debug Info" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Demo Release with Debug Info" 
   cd ".."

"Tactical - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

"Tactical - Win32 Demo Bounds Checker" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Demo Bounds Checker" 
   cd ".."

"Tactical - Win32 Demo Bounds CheckerCLEAN" : 
   cd ".\Tactical"
   $(MAKE) /$(MAKEFLAGS) /F .\Tactical.mak CFG="Tactical - Win32 Demo Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "ja2 - Win32 Release"

"Utils - Win32 Release" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Release" 
   cd ".."

"Utils - Win32 ReleaseCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"Utils - Win32 Debug" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Debug" 
   cd ".."

"Utils - Win32 DebugCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"Utils - Win32 Release with Debug Info" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Release with Debug Info" 
   cd ".."

"Utils - Win32 Release with Debug InfoCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"Utils - Win32 Bounds Checker" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Bounds Checker" 
   cd ".."

"Utils - Win32 Bounds CheckerCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"Utils - Win32 Debug Demo" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Debug Demo" 
   cd ".."

"Utils - Win32 Debug DemoCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"Utils - Win32 Release Demo" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Release Demo" 
   cd ".."

"Utils - Win32 Release DemoCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"Utils - Win32 Demo Release with Debug Info" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Demo Release with Debug Info" 
   cd ".."

"Utils - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

"Utils - Win32 Demo Bounds Checker" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Demo Bounds Checker" 
   cd ".."

"Utils - Win32 Demo Bounds CheckerCLEAN" : 
   cd ".\Utils"
   $(MAKE) /$(MAKEFLAGS) /F .\Utils.mak CFG="Utils - Win32 Demo Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "ja2 - Win32 Release"

"TacticalAI - Win32 Release" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Release" 
   cd ".."

"TacticalAI - Win32 ReleaseCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"TacticalAI - Win32 Debug" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Debug" 
   cd ".."

"TacticalAI - Win32 DebugCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"TacticalAI - Win32 Release with Debug Info" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Release with Debug Info" 
   cd ".."

"TacticalAI - Win32 Release with Debug InfoCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"TacticalAI - Win32 Bounds Checker" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Bounds Checker" 
   cd ".."

"TacticalAI - Win32 Bounds CheckerCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"TacticalAI - Win32 Debug Demo" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Debug Demo" 
   cd ".."

"TacticalAI - Win32 Debug DemoCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"TacticalAI - Win32 Release Demo" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Release Demo" 
   cd ".."

"TacticalAI - Win32 Release DemoCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"TacticalAI - Win32 Demo Release with Debug Info" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Demo Release with Debug Info" 
   cd ".."

"TacticalAI - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

"TacticalAI - Win32 Demo Bounds Checker" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Demo Bounds Checker" 
   cd ".."

"TacticalAI - Win32 Demo Bounds CheckerCLEAN" : 
   cd ".\TacticalAI"
   $(MAKE) /$(MAKEFLAGS) /F .\TacticalAI.mak CFG="TacticalAI - Win32 Demo Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "ja2 - Win32 Release"

"Editor - Win32 Release" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Release" 
   cd ".."

"Editor - Win32 ReleaseCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"Editor - Win32 Debug" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Debug" 
   cd ".."

"Editor - Win32 DebugCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"Editor - Win32 Release with Debug Info" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Release with Debug Info" 
   cd ".."

"Editor - Win32 Release with Debug InfoCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"Editor - Win32 Bounds Checker" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Bounds Checker" 
   cd ".."

"Editor - Win32 Bounds CheckerCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"Editor - Win32 Debug Demo" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Debug Demo" 
   cd ".."

"Editor - Win32 Debug DemoCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"Editor - Win32 Release Demo" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Release Demo" 
   cd ".."

"Editor - Win32 Release DemoCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"Editor - Win32 Demo Release with Debug Info" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Demo Release with Debug Info" 
   cd ".."

"Editor - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

"Editor - Win32 Demo Bounds Checker" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Demo Bounds Checker" 
   cd ".."

"Editor - Win32 Demo Bounds CheckerCLEAN" : 
   cd ".\Editor"
   $(MAKE) /$(MAKEFLAGS) /F .\Editor.mak CFG="Editor - Win32 Demo Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "ja2 - Win32 Release"

"Laptop - Win32 Release" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Release" 
   cd ".."

"Laptop - Win32 ReleaseCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"Laptop - Win32 Debug" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Debug" 
   cd ".."

"Laptop - Win32 DebugCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"Laptop - Win32 Release with Debug Info" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Release with Debug Info" 
   cd ".."

"Laptop - Win32 Release with Debug InfoCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"Laptop - Win32 Bounds Checker" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Bounds Checker" 
   cd ".."

"Laptop - Win32 Bounds CheckerCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"Laptop - Win32 Debug Demo" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Debug Demo" 
   cd ".."

"Laptop - Win32 Debug DemoCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"Laptop - Win32 Release Demo" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Release Demo" 
   cd ".."

"Laptop - Win32 Release DemoCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"Laptop - Win32 Demo Release with Debug Info" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Demo Release with Debug Info" 
   cd ".."

"Laptop - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

"Laptop - Win32 Demo Bounds Checker" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Demo Bounds Checker" 
   cd ".."

"Laptop - Win32 Demo Bounds CheckerCLEAN" : 
   cd ".\Laptop"
   $(MAKE) /$(MAKEFLAGS) /F .\Laptop.mak CFG="Laptop - Win32 Demo Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "ja2 - Win32 Release"

"Strategic - Win32 Release" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Release" 
   cd ".."

"Strategic - Win32 ReleaseCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug"

"Strategic - Win32 Debug" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Debug" 
   cd ".."

"Strategic - Win32 DebugCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release with Debug Info"

"Strategic - Win32 Release with Debug Info" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Release with Debug Info" 
   cd ".."

"Strategic - Win32 Release with Debug InfoCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Bounds Checker"

"Strategic - Win32 Bounds Checker" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Bounds Checker" 
   cd ".."

"Strategic - Win32 Bounds CheckerCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Debug Demo"

"Strategic - Win32 Debug Demo" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Debug Demo" 
   cd ".."

"Strategic - Win32 Debug DemoCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Debug Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Release Demo"

"Strategic - Win32 Release Demo" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Release Demo" 
   cd ".."

"Strategic - Win32 Release DemoCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Release Demo" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Release with Debug Info"

"Strategic - Win32 Demo Release with Debug Info" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Demo Release with Debug Info" 
   cd ".."

"Strategic - Win32 Demo Release with Debug InfoCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Demo Release with Debug Info" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "ja2 - Win32 Demo Bounds Checker"

"Strategic - Win32 Demo Bounds Checker" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Demo Bounds Checker" 
   cd ".."

"Strategic - Win32 Demo Bounds CheckerCLEAN" : 
   cd ".\Strategic"
   $(MAKE) /$(MAKEFLAGS) /F .\Strategic.mak CFG="Strategic - Win32 Demo Bounds Checker" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 


!ENDIF 

