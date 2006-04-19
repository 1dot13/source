# Microsoft Developer Studio Generated NMAKE File, Based on Utils.dsp
!IF "$(CFG)" == ""
CFG=Utils - Win32 Demo Bounds Checker
!MESSAGE No configuration specified. Defaulting to Utils - Win32 Demo Bounds Checker.
!ENDIF 

!IF "$(CFG)" != "Utils - Win32 Release" && "$(CFG)" != "Utils - Win32 Debug" && "$(CFG)" != "Utils - Win32 Release with Debug Info" && "$(CFG)" != "Utils - Win32 Bounds Checker" && "$(CFG)" != "Utils - Win32 Debug Demo" && "$(CFG)" != "Utils - Win32 Release Demo" && "$(CFG)" != "Utils - Win32 Demo Release with Debug Info" && "$(CFG)" != "Utils - Win32 Demo Bounds Checker"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Utils.mak" CFG="Utils - Win32 Demo Bounds Checker"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Utils - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Utils - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Utils - Win32 Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Utils - Win32 Bounds Checker" (based on "Win32 (x86) Static Library")
!MESSAGE "Utils - Win32 Debug Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Utils - Win32 Release Demo" (based on "Win32 (x86) Static Library")
!MESSAGE "Utils - Win32 Demo Release with Debug Info" (based on "Win32 (x86) Static Library")
!MESSAGE "Utils - Win32 Demo Bounds Checker" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "Utils - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Utils - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\Standard Gaming Platform" /I "..\TileEngine" /I "..\\" /I "..\Tactical" /I "..\tacticalai" /I "..\Editor" /I "..\strategic" /I "..\Laptop" /I ".\\" /D "CALLBACKTIMER" /D "PRECOMPILEDHEADERS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Utils - Win32 Release with Debug Info"

OUTDIR=.\Release with Debug
INTDIR=.\Release with Debug
# Begin Custom Macros
OutDir=.\Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\TileEngine" /I "..\strategic" /I "..\editor" /I ".\\" /D "NDEBUG" /D "RELEASE_WITH_DEBUG_INFO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /D "XML_STATIC" /D "CINTERFACE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Utils - Win32 Bounds Checker"

OUTDIR=.\Bounds Checker
INTDIR=.\Bounds Checker
# Begin Custom Macros
OutDir=.\Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\communications" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\editor" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "_VTUNE_PROFILING" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Utils - Win32 Debug Demo"

OUTDIR=.\Debug Demo
INTDIR=.\Debug Demo
# Begin Custom Macros
OutDir=.\Debug Demo
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\communications" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\editor" /D "_DEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Utils - Win32 Release Demo"

OUTDIR=.\Release Demo
INTDIR=.\Release Demo
# Begin Custom Macros
OutDir=.\Release Demo
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "\ja2\Build\Communications" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\editor" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Utils - Win32 Demo Release with Debug Info"

OUTDIR=.\Demo Release with Debug
INTDIR=.\Demo Release with Debug
# Begin Custom Macros
OutDir=.\Demo Release with Debug
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /I "..\Standard Gaming Platform" /I "..\\" /I "..\Tactical" /I "..\TileEngine" /I "..\strategic" /I "..\editor" /I ".\\" /D "RELEASE_WITH_DEBUG_INFO" /D "NDEBUG" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /D "XML_STATIC" /D "CINTERFACE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Utils - Win32 Demo Bounds Checker"

OUTDIR=.\Demo Bounds Checker
INTDIR=.\Demo Bounds Checker
# Begin Custom Macros
OutDir=.\Demo Bounds Checker
# End Custom Macros

ALL : "$(OUTDIR)\Utils.lib" "$(OUTDIR)\Utils.bsc"


CLEAN :
	-@erase "$(INTDIR)\_DutchText.obj"
	-@erase "$(INTDIR)\_DutchText.sbr"
	-@erase "$(INTDIR)\_EnglishText.obj"
	-@erase "$(INTDIR)\_EnglishText.sbr"
	-@erase "$(INTDIR)\_FrenchText.obj"
	-@erase "$(INTDIR)\_FrenchText.sbr"
	-@erase "$(INTDIR)\_GermanText.obj"
	-@erase "$(INTDIR)\_GermanText.sbr"
	-@erase "$(INTDIR)\_ItalianText.obj"
	-@erase "$(INTDIR)\_ItalianText.sbr"
	-@erase "$(INTDIR)\_Ja25EnglishText.obj"
	-@erase "$(INTDIR)\_Ja25EnglishText.sbr"
	-@erase "$(INTDIR)\_Ja25GermanText.obj"
	-@erase "$(INTDIR)\_Ja25GermanText.sbr"
	-@erase "$(INTDIR)\_PolishText.obj"
	-@erase "$(INTDIR)\_PolishText.sbr"
	-@erase "$(INTDIR)\_RussianText.obj"
	-@erase "$(INTDIR)\_RussianText.sbr"
	-@erase "$(INTDIR)\Animated ProgressBar.obj"
	-@erase "$(INTDIR)\Animated ProgressBar.sbr"
	-@erase "$(INTDIR)\Cinematics.obj"
	-@erase "$(INTDIR)\Cinematics.sbr"
	-@erase "$(INTDIR)\Cursors.obj"
	-@erase "$(INTDIR)\Cursors.sbr"
	-@erase "$(INTDIR)\Debug Control.obj"
	-@erase "$(INTDIR)\Debug Control.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\Encrypted File.obj"
	-@erase "$(INTDIR)\Encrypted File.sbr"
	-@erase "$(INTDIR)\Event Manager.obj"
	-@erase "$(INTDIR)\Event Manager.sbr"
	-@erase "$(INTDIR)\Event Pump.obj"
	-@erase "$(INTDIR)\Event Pump.sbr"
	-@erase "$(INTDIR)\Font Control.obj"
	-@erase "$(INTDIR)\Font Control.sbr"
	-@erase "$(INTDIR)\MapUtility.obj"
	-@erase "$(INTDIR)\MapUtility.sbr"
	-@erase "$(INTDIR)\MercTextBox.obj"
	-@erase "$(INTDIR)\MercTextBox.sbr"
	-@erase "$(INTDIR)\message.obj"
	-@erase "$(INTDIR)\message.sbr"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.obj"
	-@erase "$(INTDIR)\Multi Language Graphic Utils.sbr"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.obj"
	-@erase "$(INTDIR)\Multilingual Text Code Generator.sbr"
	-@erase "$(INTDIR)\Music Control.obj"
	-@erase "$(INTDIR)\Music Control.sbr"
	-@erase "$(INTDIR)\PopUpBox.obj"
	-@erase "$(INTDIR)\PopUpBox.sbr"
	-@erase "$(INTDIR)\Quantize Wrap.obj"
	-@erase "$(INTDIR)\Quantize Wrap.sbr"
	-@erase "$(INTDIR)\Quantize.obj"
	-@erase "$(INTDIR)\Quantize.sbr"
	-@erase "$(INTDIR)\Slider.obj"
	-@erase "$(INTDIR)\Slider.sbr"
	-@erase "$(INTDIR)\Sound Control.obj"
	-@erase "$(INTDIR)\Sound Control.sbr"
	-@erase "$(INTDIR)\STIConvert.obj"
	-@erase "$(INTDIR)\STIConvert.sbr"
	-@erase "$(INTDIR)\Text Input.obj"
	-@erase "$(INTDIR)\Text Input.sbr"
	-@erase "$(INTDIR)\Text Utils.obj"
	-@erase "$(INTDIR)\Text Utils.sbr"
	-@erase "$(INTDIR)\Timer Control.obj"
	-@erase "$(INTDIR)\Timer Control.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WordWrap.obj"
	-@erase "$(INTDIR)\WordWrap.sbr"
	-@erase "$(OUTDIR)\Utils.bsc"
	-@erase "$(OUTDIR)\Utils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "\ja2\build\communications" /I "\Standard Gaming Platform" /I "\ja2\Build" /I "\ja2\Build\Tactical" /I "\ja2\Build\TileEngine" /I "\ja2\build\strategic" /I "\ja2\build\editor" /D "_DEBUG" /D "BOUNDS_CHECKER" /D "JA2DEMO" /D "CALLBACKTIMER" /D "WIN32" /D "_WINDOWS" /D "JA2" /D "PRECOMPILEDHEADERS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Utils.pch" /YX"Utils All.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Utils.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_DutchText.sbr" \
	"$(INTDIR)\_EnglishText.sbr" \
	"$(INTDIR)\_FrenchText.sbr" \
	"$(INTDIR)\_GermanText.sbr" \
	"$(INTDIR)\_ItalianText.sbr" \
	"$(INTDIR)\_Ja25EnglishText.sbr" \
	"$(INTDIR)\_Ja25GermanText.sbr" \
	"$(INTDIR)\_PolishText.sbr" \
	"$(INTDIR)\_RussianText.sbr" \
	"$(INTDIR)\Animated ProgressBar.sbr" \
	"$(INTDIR)\Cinematics.sbr" \
	"$(INTDIR)\Cursors.sbr" \
	"$(INTDIR)\Debug Control.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\Encrypted File.sbr" \
	"$(INTDIR)\Event Manager.sbr" \
	"$(INTDIR)\Event Pump.sbr" \
	"$(INTDIR)\Font Control.sbr" \
	"$(INTDIR)\MapUtility.sbr" \
	"$(INTDIR)\MercTextBox.sbr" \
	"$(INTDIR)\message.sbr" \
	"$(INTDIR)\Multi Language Graphic Utils.sbr" \
	"$(INTDIR)\Multilingual Text Code Generator.sbr" \
	"$(INTDIR)\Music Control.sbr" \
	"$(INTDIR)\PopUpBox.sbr" \
	"$(INTDIR)\Quantize Wrap.sbr" \
	"$(INTDIR)\Quantize.sbr" \
	"$(INTDIR)\Slider.sbr" \
	"$(INTDIR)\Sound Control.sbr" \
	"$(INTDIR)\STIConvert.sbr" \
	"$(INTDIR)\Text Input.sbr" \
	"$(INTDIR)\Text Utils.sbr" \
	"$(INTDIR)\Timer Control.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\WordWrap.sbr"

"$(OUTDIR)\Utils.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Utils.lib" 
LIB32_OBJS= \
	"$(INTDIR)\_DutchText.obj" \
	"$(INTDIR)\_EnglishText.obj" \
	"$(INTDIR)\_FrenchText.obj" \
	"$(INTDIR)\_GermanText.obj" \
	"$(INTDIR)\_ItalianText.obj" \
	"$(INTDIR)\_Ja25EnglishText.obj" \
	"$(INTDIR)\_Ja25GermanText.obj" \
	"$(INTDIR)\_PolishText.obj" \
	"$(INTDIR)\_RussianText.obj" \
	"$(INTDIR)\Animated ProgressBar.obj" \
	"$(INTDIR)\Cinematics.obj" \
	"$(INTDIR)\Cursors.obj" \
	"$(INTDIR)\Debug Control.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\Encrypted File.obj" \
	"$(INTDIR)\Event Manager.obj" \
	"$(INTDIR)\Event Pump.obj" \
	"$(INTDIR)\Font Control.obj" \
	"$(INTDIR)\MapUtility.obj" \
	"$(INTDIR)\MercTextBox.obj" \
	"$(INTDIR)\message.obj" \
	"$(INTDIR)\Multi Language Graphic Utils.obj" \
	"$(INTDIR)\Multilingual Text Code Generator.obj" \
	"$(INTDIR)\Music Control.obj" \
	"$(INTDIR)\PopUpBox.obj" \
	"$(INTDIR)\Quantize Wrap.obj" \
	"$(INTDIR)\Quantize.obj" \
	"$(INTDIR)\Slider.obj" \
	"$(INTDIR)\Sound Control.obj" \
	"$(INTDIR)\STIConvert.obj" \
	"$(INTDIR)\Text Input.obj" \
	"$(INTDIR)\Text Utils.obj" \
	"$(INTDIR)\Timer Control.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\WordWrap.obj"

"$(OUTDIR)\Utils.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("Utils.dep")
!INCLUDE "Utils.dep"
!ELSE 
!MESSAGE Warning: cannot find "Utils.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Utils - Win32 Release" || "$(CFG)" == "Utils - Win32 Debug" || "$(CFG)" == "Utils - Win32 Release with Debug Info" || "$(CFG)" == "Utils - Win32 Bounds Checker" || "$(CFG)" == "Utils - Win32 Debug Demo" || "$(CFG)" == "Utils - Win32 Release Demo" || "$(CFG)" == "Utils - Win32 Demo Release with Debug Info" || "$(CFG)" == "Utils - Win32 Demo Bounds Checker"
SOURCE=.\_DutchText.cpp

"$(INTDIR)\_DutchText.obj"	"$(INTDIR)\_DutchText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_EnglishText.cpp

"$(INTDIR)\_EnglishText.obj"	"$(INTDIR)\_EnglishText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_FrenchText.cpp

"$(INTDIR)\_FrenchText.obj"	"$(INTDIR)\_FrenchText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_GermanText.cpp

"$(INTDIR)\_GermanText.obj"	"$(INTDIR)\_GermanText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_ItalianText.cpp

"$(INTDIR)\_ItalianText.obj"	"$(INTDIR)\_ItalianText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_Ja25EnglishText.cpp

"$(INTDIR)\_Ja25EnglishText.obj"	"$(INTDIR)\_Ja25EnglishText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_Ja25GermanText.cpp

"$(INTDIR)\_Ja25GermanText.obj"	"$(INTDIR)\_Ja25GermanText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_PolishText.cpp

"$(INTDIR)\_PolishText.obj"	"$(INTDIR)\_PolishText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\_RussianText.cpp

"$(INTDIR)\_RussianText.obj"	"$(INTDIR)\_RussianText.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Animated ProgressBar.cpp"

"$(INTDIR)\Animated ProgressBar.obj"	"$(INTDIR)\Animated ProgressBar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Cinematics.cpp

"$(INTDIR)\Cinematics.obj"	"$(INTDIR)\Cinematics.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Cursors.cpp

"$(INTDIR)\Cursors.obj"	"$(INTDIR)\Cursors.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Debug Control.cpp"

"$(INTDIR)\Debug Control.obj"	"$(INTDIR)\Debug Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dsutil.cpp

"$(INTDIR)\dsutil.obj"	"$(INTDIR)\dsutil.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Encrypted File.cpp"

"$(INTDIR)\Encrypted File.obj"	"$(INTDIR)\Encrypted File.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Event Manager.cpp"

"$(INTDIR)\Event Manager.obj"	"$(INTDIR)\Event Manager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Event Pump.cpp"

"$(INTDIR)\Event Pump.obj"	"$(INTDIR)\Event Pump.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Font Control.cpp"

"$(INTDIR)\Font Control.obj"	"$(INTDIR)\Font Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MapUtility.cpp

"$(INTDIR)\MapUtility.obj"	"$(INTDIR)\MapUtility.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MercTextBox.cpp

"$(INTDIR)\MercTextBox.obj"	"$(INTDIR)\MercTextBox.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\message.cpp

"$(INTDIR)\message.obj"	"$(INTDIR)\message.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Multi Language Graphic Utils.cpp"

"$(INTDIR)\Multi Language Graphic Utils.obj"	"$(INTDIR)\Multi Language Graphic Utils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Multilingual Text Code Generator.cpp"

"$(INTDIR)\Multilingual Text Code Generator.obj"	"$(INTDIR)\Multilingual Text Code Generator.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Music Control.cpp"

"$(INTDIR)\Music Control.obj"	"$(INTDIR)\Music Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PopUpBox.cpp

"$(INTDIR)\PopUpBox.obj"	"$(INTDIR)\PopUpBox.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Quantize Wrap.cpp"

"$(INTDIR)\Quantize Wrap.obj"	"$(INTDIR)\Quantize Wrap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Quantize.cpp

"$(INTDIR)\Quantize.obj"	"$(INTDIR)\Quantize.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Slider.cpp

"$(INTDIR)\Slider.obj"	"$(INTDIR)\Slider.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Sound Control.cpp"

"$(INTDIR)\Sound Control.obj"	"$(INTDIR)\Sound Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\STIConvert.cpp

"$(INTDIR)\STIConvert.obj"	"$(INTDIR)\STIConvert.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Text Input.cpp"

"$(INTDIR)\Text Input.obj"	"$(INTDIR)\Text Input.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Text Utils.cpp"

"$(INTDIR)\Text Utils.obj"	"$(INTDIR)\Text Utils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\Timer Control.cpp"

"$(INTDIR)\Timer Control.obj"	"$(INTDIR)\Timer Control.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Utilities.cpp

"$(INTDIR)\Utilities.obj"	"$(INTDIR)\Utilities.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WordWrap.cpp

"$(INTDIR)\WordWrap.obj"	"$(INTDIR)\WordWrap.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

