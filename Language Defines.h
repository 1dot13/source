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


#if !defined(ENGLISH) && !defined(GERMAN) && !defined(RUSSIAN) && !defined(DUTCH) && !defined(POLISH) && !defined(FRENCH) && !defined(ITALIAN) && !defined(TAIWANESE) && !defined(CHINESE)
/* please set one manually here (by uncommenting) if not willingly to set Workspace wide */

#define ENGLISH
//#define GERMAN
//#define RUSSIAN
//#define DUTCH
//#define FRENCH
//#define ITALIAN
//#define TAIWANESE

// WANNE: The pragma and the exclusive VS2008/VS2010 compilation should not be needed anymore for Polish version, after we set the encoding to UTF8 
// WANNE: If we like to build Polish version, we have to do this in VS2008/VS2010, because in VS2005 we get weird behavior (progress bar not moving when loading sector, ...). 
// To get correct display of Polish letters, it is needed to set "#pragma setlocale("POLISH")" in "_PolishText.cpp" and "_"Ja25PolishText.cpp.
// Unfortunatelly setting the "pragma" causes the weird behavior when building in VS2005, this is why we should build in VS2008/VS2010.
//#define POLISH			

// WANNE: For CHINESE language we also have to define WINFONTS! So when building Chinese version, enable both defines.
//#define CHINESE
//#define WINFONTS

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
#if !defined(ENGLISH) && !defined(GERMAN) && !defined(RUSSIAN) && !defined(DUTCH) && !defined(POLISH) && !defined(FRENCH) && !defined(ITALIAN) && !defined(TAIWANESE) && !defined(CHINESE)
#  error "At least You have to specify a Language somewhere. See comments above."
#endif

//if the language represents words as single chars
#ifdef TAIWAN
	#define	SINGLE_CHAR_WORDS
#endif

#endif
