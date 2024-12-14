#ifndef __LANGUAGE_DEFINES_H
#define __LANGUAGE_DEFINES_H

#pragma once

/* ============================================================================
 * ONLY ONE OF THESE LANGUAGES CAN BE DEFINED AT A TIME!
 * BUT now You can define it _here_ by uncommenting one _or_ (better):
 * You can comment them all out and then set it _global_ in "Preprocessor 
 * options" (do it for ALL projects in the workspace and both debug & release)
 *   _or_
 * give it do Your MAKEFILE, f.i. make ENGLISH, but keep in mind that some 
 * weird make tools will require 'make ENGLISH=1' instead
 * 
 * using one of the two later methods will keep this SVN file unchanged for the
 * future, only Your private project files (workspace/solution) will differ 
 * from the SVN stuff.
 *											 (2006-10-10, Sergeant_Kolja)
 */


/* The recommend approach for VS2010 multi-language builds is to use the command line or the ja2.props file.

    By default the language is ENGLISH and the Language Prefix is EN.

	There are 3 ways you can build the JA2 1.13 executable file:

	// -------------------------------------------------------
	// 1. Using the command line
	// -------------------------------------------------------

    For example, where msbuild is located in %SystemRoot%\Microsoft.NET\Framework\v4.0.30319\ if not on your path
	msbuild.exe /p:Configuration=Release ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=PL /p:JA2Language=POLISH ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=DE /p:JA2Language=GERMAN ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=RU /p:JA2Language=RUSSIAN ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=NL /p:JA2Language=DUTCH ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=FR /p:JA2Language=FRENCH ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=IT /p:JA2Language=ITALIAN ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=CN /p:JA2Language=CHINESE ja2_VS2010.sln

	Note: If you want to build "Unfinished Business" version, just append the /p:JA2Config=JA2UB in the command line
	msbuild.exe /p:Configuration=Release /p:JA2Config=JA2UB ja2_VS2010.sln
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=DE /p:JA2Language=GERMAN /p:JA2Config=JA2UB ja2_VS2010.sln
	
	Note2: You can also specify the target output name with the parameter /p:TargetName
	msbuild.exe /p:Configuration=Release /p:JA2LangPrefix=DE /p:JA2Language=GERMAN /p:JA2Config=JA2UB /p:TargetName="JA2UB_113" ja2_VS2010.sln

	// --------------------------------------------------------
	// 2. Editing the ja2.props file and then build in VS 2010 
	// -------------------------------------------------------
	
	1. Open the "ja2.props" file in a text editor and set the <BuildMacro> tags to your likeing.
	
	For example: If you want to build Russian Version (normal JA2, not UB) then set the following:

	<BuildMacro Include="JA2Config">
      <Value></Value>
    </BuildMacro>
    <BuildMacro Include="JA2LangPrefix">
      <Value>RU</Value>
    </BuildMacro>
    <BuildMacro Include="JA2Language">
      <Value>RUSSIAN</Value>
    </BuildMacro>

	2. Save the file
	3. Build the project in Visual Studio 2010

	// --------------------------------------------------------
	// 3. The "old" way for building the executable
	// -------------------------------------------------------

	1. Enable the "#undef ENGLISH" define below, so English will not be used anymore
	2. Set the desired language below
	3. If you want to build "Unfinished Business" version, enable "#define JA2UB" and "#define JA2UBMAPS" in builddefines.h"
	4. Build the executable in VS 2005 / 2008 / 2010
	5. The output will be placed in the "Build\bin\" folder
*/

// Only enable this "undef", if you use the 3. way of building the executable!
#undef ENGLISH

#if !defined(ENGLISH) && !defined(GERMAN) && !defined(RUSSIAN) && !defined(DUTCH) && !defined(POLISH) && !defined(FRENCH) && !defined(ITALIAN) && !defined(CHINESE)
/* please set one manually here (by uncommenting) if not willingly to set Workspace wide */

#define ENGLISH
//#define GERMAN
//#define RUSSIAN
//#define DUTCH
//#define FRENCH
//#define ITALIAN
//#define POLISH

// INFO: For Chinese 1.13 version, you also have to set USE_WINFONTS = 1 in ja2.ini inside your JA2 installation directory!
//#define CHINESE

#endif

//**ddd direct link libraries
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "advapi32.lib")
#pragma comment (lib, "shell32.lib")

/* ====================================================================
 * Regardless of if we did it Workspace wide or by uncommenting above,
 * HERE we must see, what language was selected. If one, we
 */
#if !defined(ENGLISH) && !defined(GERMAN) && !defined(RUSSIAN) && !defined(DUTCH) && !defined(POLISH) && !defined(FRENCH) && !defined(ITALIAN) && !defined(CHINESE)
#  error "At least You have to specify a Language somewhere. See comments above."
#endif

//if the language represents words as single chars
/*#ifdef TAIWAN
	#define	SINGLE_CHAR_WORDS
#endif*/

#endif
