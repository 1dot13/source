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
//#define POLISH
//#define FRENCH
//#define ITALIAN
//#define TAIWANESE
//#define CHINESE

#ifdef CHINESE
	#define WINFONTS
#endif

#endif



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
