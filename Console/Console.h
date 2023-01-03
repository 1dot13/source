/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2001-2004 by Marko Bozikovic
// All rights reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.	I can be reached as follows:
//	marko.bozikovic@alterbox.net
//	bozho@kset.org
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Console.h - console class declaration

#pragma once

#include <string>
#include <vector>

using namespace std;

//#include "include/FreeImagePlus.h"

typedef basic_string<TCHAR>			tstring;

#define WM_INPUTREADY				WM_USER + 1

// tray icon message
#define WM_TRAY_NOTIFY				WM_USER + 0x123

#define TRAY_ICON_ID				1

// timer #defines
#define TIMER_REPAINT_CHANGE		42	// timer that is started after there 
										// were some changes in the console

#define TIMER_REPAINT_MASTER		43	// master timer (needed to repaint 
										// for some DOS programs, can be 
										// disabled for lower CPU usage)

#define TIMER_SHOW_HIDE_CONSOLE		100	// used to hide console window when 
										// starting shell process after a 
										// defined period of time (some 
										// shells, like 4NT need console 
										// window visible during startup for
										// all init options to work properly

// transparency #defines
#define TRANSPARENCY_NONE			0
#define TRANSPARENCY_ALPHA			1
#define TRANSPARENCY_COLORKEY		2
#define TRANSPARENCY_FAKE			3

// cusror style #defines
#define CURSOR_STYLE_NONE			0
#define CURSOR_STYLE_XTERM			1
#define CURSOR_STYLE_BLOCK			2
#define CURSOR_STYLE_NBBLOCK		3
#define CURSOR_STYLE_PULSEBLOCK		4
#define CURSOR_STYLE_BAR			5
#define CURSOR_STYLE_CONSOLE		6
#define CURSOR_STYLE_NBHLINE		7
#define CURSOR_STYLE_HLINE			8
#define CURSOR_STYLE_VLINE			9
#define CURSOR_STYLE_RECT			10
#define CURSOR_STYLE_NBRECT			11
#define CURSOR_STYLE_PULSERECT		12
#define CURSOR_STYLE_FADEBLOCK		13

// docking #defines
#define DOCK_NONE					0
#define DOCK_TOP_LEFT				1
#define DOCK_TOP_RIGHT				2
#define DOCK_BOTTOM_RIGHT			3
#define DOCK_BOTTOM_LEFT			4

// Z-order #defines
#define Z_ORDER_REGULAR				0
#define Z_ORDER_ONTOP				1
#define Z_ORDER_ONBOTTOM			2

// window border defines
#define BORDER_NONE					0
#define BORDER_REGULAR				1
#define BORDER_THIN					2

// window background style
#define	BACKGROUND_STYLE_RESIZE		0
#define	BACKGROUND_STYLE_CENTER		1
#define	BACKGROUND_STYLE_TILE		2

// taskbar button defines
#define TASKBAR_BUTTON_NORMAL		0
#define TASKBAR_BUTTON_HIDE			1
#define TASKBAR_BUTTON_TRAY			2

// new configuration auto-reload defines
#define RELOAD_NEW_CONFIG_PROMPT	0
#define RELOAD_NEW_CONFIG_YES		1
#define RELOAD_NEW_CONFIG_NO		2

#define TEXT_SELECTION_NONE			0
#define TEXT_SELECTION_SELECTING	1
#define TEXT_SELECTION_SELECTED		2


/////////////////////////////////////////////////////////////////////////////
// Console class

class Console {
	public: // ctor/dtor
		
		Console(LPCTSTR szConfigFile, LPCTSTR szShellCmdLine, LPCTSTR szConsoleTitle, LPCTSTR pszReloadNewConfig);
		~Console();

	public:

		// creates and shows Console window
		BOOL Create(HWND notify);

	private:

		////////////////////
		// message handlers
		////////////////////
		
		////////////////////////////////
		// windows destruction messages
		
		// destroys GDI stuff and posts a quit message
		void OnDestroy();
		// deletes Console object
		void OnNcDestroy();
		
		
		/////////////////////
		// painting messages
		
		// handles painting from off-screen buffers
		void OnPaint();
		
		// handles master and 'change' timers
		void OnPaintTimer();
		
		// handles cursor timer (for animated cursors)
		void OnCursorTimer();
		
		
		/////////////////////////
		// window state messages
		
		// handles window position changes (for snapping to desktop edges)
		void OnWindowPosChanging(WINDOWPOS* lpWndPos);
		
		// handles activation message (used for setting alpha transparency and cursor states)
		void OnActivateApp(BOOL bActivate, DWORD dwFlags);
		
		// handles vertical scrolling
		void OnVScroll(WPARAM wParam);
		
		// handles keyboard layout change, posts the same message to the windows console window
		void OnInputLangChangeRequest(WPARAM wParam, LPARAM lParam);
		
		
		//////////////////
		// mouse messages
		
		// handles text selection start and window mouse drag start
		void OnLButtonDown(UINT uiFlags, POINTS points);
		
		// handles text selection end, window mouse drag end and text copy
		void OnLButtonUp(UINT uiFlags, POINTS points);
		
		// toggles always on top flag
		void OnLButtonDblClick(UINT uiFlags, POINTS points);
		
		// pops up the Console menu
		void OnRButtonUp(UINT uiFlags, POINTS points);
		
		// pastes text from clipboard
		void OnMButtonDown(UINT uiFlags, POINTS points);
		
		// handles mouse movement for text selection and window mouse drag
		void OnMouseMove(UINT uiFlags, POINTS points);
		
		// handles start/stop mouse drag for window border
		void OnSetCursor(WORD wHitTest, WORD wMouseMessage);

		// handles text input
		void OnChar(WORD mychar);
		
		wstring Input;
		
		//////////////////
		// other messages
		
		// called before the Console menu or system menu pops up 
		// (populates config files submenu with filenames)
		void OnInitMenuPopup(HMENU hMenu, UINT uiPos, BOOL bSysMenu);
		
		// handles drag-n-dropped filenames
		void OnDropFiles(HDROP hDrop);
		
		// handles commands from the Console popup menu
		BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		
		// handles commands from the system popup menu
		BOOL OnSysCommand(WPARAM wParam, LPARAM lParam);
		
		// handles tray icon messages
		void OnTrayNotify(WPARAM wParam, LPARAM lParam);
		
		// handles WM_SETTINGCHANGE (we handle only wallpaper changes here)
//		void OnWallpaperChanged(const TCHAR* pszFilename);

		
	private:

		///////////////////////////////////////////
		// Console window creation/setup functions
		///////////////////////////////////////////
		
		// gets Console options
		BOOL GetOptions();
		
		// registers Console window classes
		BOOL RegisterWindowClasses();

		// adds stuff to system and popup menus
		BOOL SetupMenus();

		// creates Console window
		BOOL CreateConsoleWindow();
		
		// creates new Console font
		void CreateNewFont();
		
		// creates new background brush
		void CreateNewBrush();

		// creates the cursor
		void CreateCursor();
		
		// creates offscreen painting buffers
		void CreateOffscreenBuffers();
		
		// creates background bitmap
		void CreateBackgroundBitmap();

		// called by the ::EnumDisplayMonitors to create background for each display
//		static BOOL CALLBACK BackgroundEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
		
		// gets character width and height
		void GetTextSize();

		// gets window borders (X, Y and caption sizes)
		void GetBordersDimensions();

		// calculates window and client area sizes
		void CalcWindowSize();

		// sets up window transparency (alpha, color key, fake)
//		void SetWindowTransparency();

		// sets scrollbar stuff
		void SetScrollbarStuff();
		
		// sets default console colors
		void SetDefaultConsoleColors();
		
		// sets window size and position
		void SetWindowSizeAndPosition();
		
		// sets Console's big, small and tray icons
		void SetWindowIcons();
		
		// sets up traybar icon
		BOOL SetTrayIcon(DWORD dwMessage);

		// destroys Console cursor
		void DestroyCursor();

		// opens the configuration file in a text editor
		void EditConfigFile();
		
		// reloads Console settings
		void ReloadSettings();


		/////////////////////////////
		// windows console functions
		/////////////////////////////
		
		// allocates the console and starts the command shell
		BOOL StartShellProcess();
		
		// refreshes m_hStdOutFresh handle
		void RefreshStdOut();
		
		// gets a fresh console output
		void RefreshScreenBuffer();

		// sets initial windows console size
		void InitConsoleWndSize(DWORD dwColumns);
		
		// resizes the windows console
		void ResizeConsoleWindow();

		// Allocates the screen buffer
		void AllocateBuffer();
		
		
		//////////////////////
		// painting functions
		//////////////////////

		// repaints the memory hdc
		void RepaintWindow();
		
		// repaints the memory hdc (paint only changes)
		void RepaintWindowChanges();
		
		// draws the cursor
		void DrawCursor(BOOL bOnlyCursor = FALSE);
		
		// two helper functions for DrawCursor method

		// returns cursor rectangle
		inline void GetCursorRect(RECT& rectCursor);
		// draws cursor background and returns the cursor rectangle
		inline void DrawCursorBackground(RECT& rectCursor);

		// clears text selection
		void ClearSelection();
		
		// returns the console text change rate since the last painting
		// (using this value we decide whether to repaint entire window
		// or just the changes)
		DWORD GetChangeRate();


		//////////////////////////
		// window state functions
		//////////////////////////
		
		// shows/hides Console window
		void ShowHideWindow();
		
		// shows/hides windows console window
		void ShowHideConsole();

		// shows/hides windows console window after a timeout (used during
		// shell startup)
		void ShowHideConsoleTimeout();
		
		// toggles 'always on top' status
		void ToggleWindowOnTop();


		//////////////////
		// menu functions
		//////////////////

		// called by OnCommand and OnSysCommand to handle menu commands
		BOOL HandleMenuCommand(DWORD dwID);
		
		// updates popup and system menus for 'always on top' status
		void UpdateOnTopMenuItem();
		
		// updates popup and system menus for 'hide console' status
		void UpdateHideConsoleMenuItem();

		// updates configuration files submenu
		void UpdateConfigFilesSubmenu();


		///////////////////////
		// clipboard functions
		///////////////////////
		
		// copies selected text to the clipboard
		void CopyTextToClipboard();
		
		// pastes text from the clipboard
		void PasteClipoardText();
		
		//////////////////
		// misc functions
		//////////////////

		// shows Readme.txt file
		void ShowReadmeFile();

		// shows about dialog
		void About();

		// sends text to the windows console
		void SendTextToConsole(const wchar_t *pszText);

		// returns the full filename
		tstring GetFullFilename(const tstring& strFilename);

		// gets a desktop rectangle
		void GetDesktopRect(RECT& rectDesktop);

		// Pulls the text from the stdout or stderr handle (both are the same) and sends it to the console
		void AddOutput();


		///////////////////////
		// 'gearbox' functions
		///////////////////////
		
		// Console window procedure
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// shell activities monitor
		static DWORD WINAPI MonitorThreadStatic(LPVOID lpParam);
		DWORD MonitorThread();

		// CTRL+C, CTRL+BREAK handler
		static BOOL WINAPI CtrlHandler(DWORD dwCtrlType);


	public: // public data

		// Console window handle
		HWND	m_hWnd;

		HWND	m_notifyhWnd;

		// console colors
		COLORREF	m_arrConsoleColors[16];

	private: // private data

		// readem filename
		tstring	m_strReadmeFile;

		BOOL	m_bInitializing;
		BOOL	m_bReloading;
		
		tstring	m_strConfigFile;
		tstring	m_strConfigEditor;
		tstring m_strConfigEditorParams;

		// one of the RELOAD_NEW_CONFIG_* constants
		// - specifies relaod behaviour when a new configuration is selected
		// (auto reload, don't reload, ask user)
		DWORD	m_dwReloadNewConfigDefault;
		DWORD	m_dwReloadNewConfig;
		
		tstring	m_strShell;
		tstring	m_strShellCmdLine;
		
		// handle to invisible window - used for hiding the taskbar button in tray and 'hidden' modes
		HWND m_hwndInvisParent;

		// memory device context that holds console output image
		HDC		m_hdcConsole;
		// memory device context for an off-screen window buffer (used to 
		// compose an image from m_hdcConsole and m_hdcSelection if text 
		// selection is active)
		HDC		m_hdcWindow;
		
		// a bitmap used for drawing in the console memory DC
		HBITMAP	m_hbmpConsole;
		HBITMAP	m_hbmpConsoleOld;

		// a bitmap used for drawing in the window memory DC
		HBITMAP	m_hbmpWindow;
		HBITMAP	m_hbmpWindowOld;
		
		// brush for painting background
		HBRUSH	m_hBkBrush;

		// master repaint timer interval (runs independent of changes in the 
		// console)
		DWORD	m_dwMasterRepaintInt;

		// change repaint timer interval (when a change occurs, repainting 
		// will be postponed for this interval)
		DWORD	m_dwChangeRepaintInt;
		
		// icon filename
		tstring	m_strIconFilename;
		// program icons
		HICON	m_hSmallIcon;
		HICON	m_hBigIcon;
		
		// popup menu
		HMENU	m_hPopupMenu;
		
		// system (taskbar button) menu
		HMENU	m_hSysMenu;

		// submenu for the XML (config) files
		HMENU	m_hConfigFilesMenu;

		// set to TRUE if the popup menu is disabled
		BOOL	m_bPopupMenuDisabled;
		
		// Console window title variables
		// holds the default console title ("console" or the one passed in the cmdline param)
		tstring m_strWindowTitleDefault;
		// holds the window title (default title, or the one from the config file)
		tstring	m_strWindowTitle;
		// holds the current window title
		tstring m_strWindowTitleCurrent;
		
		// font data
		tstring	m_strFontName;
		DWORD	m_dwFontSize;
		BOOL	m_bBold;
		BOOL	m_bItalic;
		BOOL	m_bUseFontColor;
		COLORREF m_crFontColor;
		HFONT	m_hFont;
		HFONT	m_hFontOld;
		
		// window X and Y positions
		int		m_nX;
		int		m_nY;

		// client area inside border (gives a more 'relaxed' look to windows)
		int		m_nInsideBorder;
		
		// window width and height
		int		m_nWindowWidth;
		int		m_nWindowHeight;

		// window border sizes
		int		m_nXBorderSize;
		int		m_nYBorderSize;
		int		m_nCaptionSize;

		// client area widht/height
		int		m_nClientWidth;
		int		m_nClientHeight;

		// char height and width (used in window repainting) 
		// Note: width is used only for fixed-pitch fonts to speed up 
		// repainting
		int		m_nCharHeight;
		int		m_nCharWidth;

		// window border type
		// 0 - none
		// 1 - regular window
		// 2 - 1-pixel thin border
		DWORD	m_dwWindowBorder;

		// scrollbar stuff
		BOOL	m_bShowScrollbar;
		int		m_nScrollbarStyle;
		COLORREF m_crScrollbarColor;
		int		m_nScrollbarWidth;
		int		m_nScrollbarButtonHeight;
		int		m_nScrollbarThunmbHeight;

		// what to do with the taskbar button
		// if the taskbar button is hidden, or placed in the traybar, you 
		// can't ALT-TAB to console (take care when using with color key 
		// transparency :-)
		// 0 - nothing
		// 1 - hide it
		// 2 - put icon to traybar
		DWORD	m_dwTaskbarButton;
		
		// set to TRUE if the window can be dragged by left-click hold
		BOOL	m_bMouseDragable;

		// snap distance
		int		m_nSnapDst;
		
		// window docking position
		// 0 - no dock
		// 1 - top left
		// 2 - top right
		// 3 - bottom right
		// 4 - bottom left
		DWORD	m_dwDocked;

		// window Z-ordering
		// 0 - regular
		// 1 - always on top
		// 2 - always on bottom
		DWORD	m_dwOriginalZOrder;
		DWORD	m_dwCurrentZOrder;

		// Win2000/XP transparency
		// 0 - none
		// 1 - alpha blending
		// 2 - colorkey
		// 3 - fake transparency
		DWORD	m_dwTransparency;

		// alpha value for alpha blending (Win2000 and later only!)
		BYTE	m_byAlpha;

		// alpha value for inactive window
		BYTE	m_byInactiveAlpha;

		COLORREF m_crBackground;
		COLORREF m_crConsoleBackground;

		// this is used for tinting background images and fake transparencies
		BOOL	m_bTintSet;
		BYTE	m_byTintOpacity;
		BYTE	m_byTintR;
		BYTE	m_byTintG;
		BYTE	m_byTintB;
		
		// used when background is an image
		BOOL	m_bBitmapBackground;
		tstring	m_strBackgroundFile;
		HDC		m_hdcBackground;
		HBITMAP	m_hbmpBackground;
		HBITMAP	m_hbmpBackgroundOld;

		// background attributes

		// one of BACKGROUND_STYLE_ #defines
		DWORD	m_dwBackgroundStyle;
		// set to true for relative background
		BOOL	m_bRelativeBackground;
		// set to true to extend the background to all monitors
		BOOL	m_bExtendBackground;
		
		// offsets used for multiple monitors and relative backgrounds (fake transparency, too)
		int		m_nBackgroundOffsetX;
		int		m_nBackgroundOffsetY;

		// used for showing/hiding main window
		BOOL	m_bHideWindow;
		// set to TRUE when the real console is hidden
		BOOL	m_bHideConsole;

		// timeout used when hiding console window for the first time (some
		// shells need console window visible during startup)
		DWORD	m_dwHideConsoleTimeout;

		// if set to TRUE, Console will be started minimized
		BOOL	m_bStartMinimized;
		
		
		// cursor style
		// 0 - none
		// 1 - XTerm
		// 2 - block cursor
		// 3 - bar cursor
		// 4 - console cursor
		// 5 - horizontal line
		// 6 - vertical line
		// 7 - pulse rect
		// 8 - fading block
		DWORD	m_dwCursorStyle;

		COLORREF m_crCursorColor;
		
		// console screen buffer info for cursor
		CONSOLE_SCREEN_BUFFER_INFO m_csbiCursor;

		// wether console cursor is visible or not
		BOOL	m_bCursorVisible;

		class Cursor*	m_pCursor;

		// mouse cursor offset within the window (used for moving the window)
		POINT	m_mouseCursorOffset;
		
		/////////////////////
		// console stuff

		// console window handle
		HWND	m_hWndConsole;

		// console screen buffer info for console repainting
		CONSOLE_SCREEN_BUFFER_INFO m_csbiConsole;
		
		// console stdouts
		HANDLE	m_hStdOut;
		HANDLE	m_hStdOutFresh;
		
		// set when quitting the application
		HANDLE	m_hQuitEvent;

		// set by monitor thread when detects console process exit
		HANDLE	m_hConsoleProcess;

		// handle to monitor thread
		HANDLE	m_hMonitorThread;

		// console rows & columns
		DWORD	m_dwRows;
		DWORD	m_dwColumns;
		DWORD	m_dwBufferRows;
		BOOL	m_bUseTextBuffer;

		// set to one of TEXT_SELECTION_ #defines
		int		m_nTextSelection;

		// X Windows style copy-on-select
		BOOL	m_bCopyOnSelect;

		// Inverse the shift behaviour for selecting and dragging
		BOOL	m_bInverseShift;
		
		COORD	m_coordSelOrigin;
		RECT	m_rectSelection;
		HDC		m_hdcSelection;
		HBITMAP	m_hbmpSelection;
		HBITMAP	m_hbmpSelectionOld;
		HBRUSH	m_hbrushSelection;

		CHAR_INFO*	m_pScreenBuffer;
		CHAR_INFO*	m_pScreenBufferNew;

		DWORD m_nTextColor;
		DWORD m_nTextBgColor;

		// Console window class names
		static const TCHAR m_szConsoleClass[];
		static const TCHAR m_szHiddenParentClass[];

		// win console window title
		static const tstring m_strWinConsoleTitle;
};



/////////////////////////////////////////////////////////////////////////////
