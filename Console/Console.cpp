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

#ifdef UNICODE
#error "Unicode defined"
#endif

/////////////////////////////////////////////////////////////////////////////
// Console.cpp - console class implementation
#include "builddefines.h"
#undef CINTERFACE

//#include "stdafx.h"
//#include <atlbase.h>
#include <tchar.h>
#include <msxml.h>
#include <shellapi.h>
#include <commctrl.h>
#include <memory>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include "resource.h"
#include "FileStream.h"
//#include "ComBSTROut.h"
//#include "ComVariantOut.h"
#include "Cursors.h"
#include "Dialogs.h"
#include "Console.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HINSTANCE ghInstance;

/////////////////////////////////////////////////////////////////////////////
// Console class


/////////////////////////////////////////////////////////////////////////////
// Console window class names
const TCHAR Console::m_szConsoleClass[] = _T("Console Main Command Window");
const TCHAR Console::m_szHiddenParentClass[] = _T("Console Invisible Parent Window");


/////////////////////////////////////////////////////////////////////////////
// win console window title
const tstring Console::m_strWinConsoleTitle(_T("Console Command Window"));


/////////////////////////////////////////////////////////////////////////////
// ctor/dtor

Console::Console(LPCTSTR pszConfigFile, LPCTSTR pszShellCmdLine, LPCTSTR pszConsoleTitle, LPCTSTR pszReloadNewConfig)
: m_hWnd(NULL)
, m_bInitializing(TRUE)
, m_bReloading(FALSE)
, m_strConfigEditor(_T("notepad.exe"))
, m_strConfigEditorParams(_T(""))
, m_strShell(_T(""))
, m_strShellCmdLine(pszShellCmdLine)
, m_hwndInvisParent(NULL)
, m_hdcConsole(NULL)
, m_hdcWindow(NULL)
, m_hbmpConsole(NULL)
, m_hbmpConsoleOld(NULL)
, m_hbmpWindow(NULL)
, m_hbmpWindowOld(NULL)
, m_hBkBrush(NULL)
, m_dwMasterRepaintInt(500)
, m_dwChangeRepaintInt(50)
, m_strIconFilename(_T(""))
, m_hSmallIcon(NULL)
, m_hBigIcon(NULL)
, m_hPopupMenu(NULL)
, m_hSysMenu(NULL)
, m_hConfigFilesMenu(NULL)
, m_bPopupMenuDisabled(FALSE)
, m_strWindowTitleDefault(_tcslen(pszConsoleTitle) == 0 ? _T("console") : pszConsoleTitle)
, m_strWindowTitle(m_strWindowTitleDefault)
, m_strWindowTitleCurrent(m_strWindowTitleDefault)
, m_strFontName(_T("Courier New"))
, m_dwFontSize(10)
, m_bBold(FALSE)
, m_bItalic(FALSE)
, m_bUseFontColor(FALSE)
, m_crFontColor(RGB(0, 0, 0))
, m_hFont(NULL)
, m_hFontOld(NULL)
, m_nX(0)
, m_nY(0)
, m_nInsideBorder(0)
, m_nWindowWidth(0)
, m_nWindowHeight(0)
, m_nXBorderSize(5)
, m_nYBorderSize(5)
, m_nCaptionSize(0)
, m_nClientWidth(0)
, m_nClientHeight(0)
, m_nCharHeight(0)
, m_nCharWidth(0)
, m_dwWindowBorder(BORDER_REGULAR)
, m_bShowScrollbar(TRUE)
, m_nScrollbarStyle(FSB_REGULAR_MODE)
, m_crScrollbarColor(::GetSysColor(COLOR_3DHILIGHT))
, m_nScrollbarWidth(::GetSystemMetrics(SM_CXVSCROLL))
, m_nScrollbarButtonHeight(::GetSystemMetrics(SM_CYVSCROLL))
, m_nScrollbarThunmbHeight(::GetSystemMetrics(SM_CYVTHUMB))
, m_dwTaskbarButton(TASKBAR_BUTTON_NORMAL)
, m_bMouseDragable(TRUE)
, m_nSnapDst(10)
, m_dwDocked(DOCK_NONE)
, m_dwOriginalZOrder(Z_ORDER_REGULAR)
, m_dwCurrentZOrder(Z_ORDER_REGULAR)
, m_dwTransparency(TRANSPARENCY_NONE)
, m_byAlpha(150)
, m_byInactiveAlpha(150)
, m_crBackground(RGB(0, 0, 0))
, m_crConsoleBackground(RGB(0, 0, 0))
, m_bTintSet(FALSE)
, m_byTintOpacity(50)
, m_byTintR(0)
, m_byTintG(0)
, m_byTintB(0)
, m_bBitmapBackground(FALSE)
, m_strBackgroundFile(_T(""))
, m_hdcBackground(NULL)
, m_hbmpBackground(NULL)
, m_hbmpBackgroundOld(NULL)
, m_dwBackgroundStyle(BACKGROUND_STYLE_RESIZE)
, m_bRelativeBackground(FALSE)
, m_bExtendBackground(FALSE)
, m_nBackgroundOffsetX(0)
, m_nBackgroundOffsetY(0)
, m_bHideWindow(FALSE)
, m_bHideConsole(TRUE)
, m_dwHideConsoleTimeout(0)
, m_bStartMinimized(FALSE)
, m_dwCursorStyle(CURSOR_STYLE_CONSOLE)
, m_crCursorColor(RGB(255, 255, 255))
, m_bCursorVisible(TRUE)
, m_pCursor(NULL)
, m_hWndConsole(NULL)
, m_hStdOut(NULL)
, m_hStdOutFresh(NULL)
, m_hQuitEvent(NULL)
, m_hConsoleProcess(NULL)
, m_hMonitorThread(NULL)
, m_dwRows(50)
, m_dwColumns(80)
, m_dwBufferRows(500)
, m_bUseTextBuffer(TRUE)
, m_nTextSelection(TEXT_SELECTION_NONE)
, m_bCopyOnSelect(FALSE)
, m_bInverseShift(FALSE)
, m_hdcSelection(NULL)
, m_hbmpSelection(NULL)
, m_hbmpSelectionOld(NULL)
, m_hbrushSelection(::CreateSolidBrush(RGB(0xff, 0xff, 0xff)))
, m_pScreenBuffer(NULL)
, m_pScreenBufferNew(NULL)
, m_nTextColor(15)
, m_nTextBgColor(0)
{

#if 0
	m_strConfigFile = GetFullFilename(pszConfigFile);

	if (!_tcsicmp(pszReloadNewConfig, _T("yes"))) {
		m_dwReloadNewConfigDefault = RELOAD_NEW_CONFIG_YES;
	} else if (!_tcsicmp(pszReloadNewConfig, _T("no"))) {
		m_dwReloadNewConfigDefault = RELOAD_NEW_CONFIG_NO;
	} else {
		m_dwReloadNewConfigDefault = RELOAD_NEW_CONFIG_PROMPT;
	}
	m_dwReloadNewConfig = m_dwReloadNewConfigDefault;
#endif

	m_mouseCursorOffset.x = 0;
	m_mouseCursorOffset.y = 0;

	m_coordSelOrigin.X = 0;
	m_coordSelOrigin.Y = 0;

	::ZeroMemory(&m_rectSelection, sizeof(RECT));

#if 0
	// get Console.exe directory
	TCHAR szPathName[MAX_PATH];
	::ZeroMemory(szPathName, sizeof(szPathName));
	::GetModuleFileName(ghInstance, szPathName, MAX_PATH);

	tstring	strExeDir(szPathName);

	strExeDir = strExeDir.substr(0, strExeDir.rfind(_T("\\")));
	strExeDir += TCHAR('\\');

	// if no config file is given, get console.xml from the startup directory
	if (m_strConfigFile.length() == 0) {

		m_strConfigFile = strExeDir + tstring(_T("console.xml"));
	}

	// get readme filename
	m_strReadmeFile = strExeDir + tstring(_T("Readme.txt"));
#endif

	::ZeroMemory(&m_csbiCursor, sizeof(CONSOLE_SCREEN_BUFFER_INFO));
	::ZeroMemory(&m_csbiConsole, sizeof(CONSOLE_SCREEN_BUFFER_INFO));

	SetDefaultConsoleColors();

	AllocateBuffer();

	// Set the stdout and stderr to this window
	HANDLE myOut;
	::CreatePipe( &m_hStdOut, &myOut, NULL, 0);
	::SetStdHandle( STD_OUTPUT_HANDLE, myOut);
	::SetStdHandle( STD_ERROR_HANDLE, myOut);

	int hConHandle = _open_osfhandle((long)myOut, _O_BINARY); //_O_WTEXT);
 	FILE *fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	*stderr = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// create text monitor thread
	DWORD dwThreadID;
	m_hMonitorThread = ::CreateThread(NULL, 0, Console::MonitorThreadStatic, this, CREATE_SUSPENDED, &dwThreadID);
	::ResumeThread(m_hMonitorThread);
}

Console::~Console() {

	delete[] m_pScreenBuffer;
	delete[] m_pScreenBufferNew;

	if (m_hSmallIcon) ::DestroyIcon(m_hSmallIcon);
	if (m_hBigIcon) ::DestroyIcon(m_hBigIcon);

//	::CloseHandle(m_hConsoleProcess);
//	::CloseHandle(m_hQuitEvent);

	// shutdown console process
//	::CloseHandle(m_hStdOut);
//	::CloseHandle(m_hStdOutFresh);
//	if (m_hWndConsole) ::SendMessage(m_hWndConsole, WM_CLOSE, 0, 0);
//	::FreeConsole();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Public methods

/////////////////////////////////////////////////////////////////////////////
// creates and shows Console window

BOOL Console::Create(HWND notify) {

	m_notifyhWnd = notify;

	if (m_hWnd)
	{
		::ShowWindow( m_hWnd, SW_SHOW);
		return TRUE;
	}

	if (!GetOptions()) return FALSE;

	if (!RegisterWindowClasses()) return FALSE;

	if (!CreateConsoleWindow()) return FALSE;

	// create window DC
	HDC hdcDesktop = ::GetDCEx(m_hWnd, NULL, 0);
	m_hdcConsole = ::CreateCompatibleDC(hdcDesktop);
	m_hdcWindow = ::CreateCompatibleDC(hdcDesktop);
	::ReleaseDC(m_hWnd, hdcDesktop);

	// create selection DC
	m_hdcSelection = ::CreateCompatibleDC(m_hdcWindow);

	if (!SetupMenus()) return FALSE;

	CreateNewFont();
	CreateNewBrush();

//	SetWindowTransparency();
	SetWindowIcons();

	GetTextSize();
	GetBordersDimensions();
	CreateCursor();

	// now we can start the monitor thread
	StartShellProcess();

	// set the long repaint timer
	if (m_dwMasterRepaintInt) ::SetTimer(m_hWnd, TIMER_REPAINT_MASTER, m_dwMasterRepaintInt, NULL);

	m_bInitializing = FALSE;

	// register for files/folders drag-n-drop
	::DragAcceptFiles(m_hWnd, TRUE);

	RefreshScreenBuffer();
	::CopyMemory(m_pScreenBuffer, m_pScreenBufferNew, sizeof(CHAR_INFO) * m_dwRows * m_dwColumns);
	RepaintWindow();
	::UpdateWindow(m_hWnd);

	if (m_bStartMinimized) {
		if (m_dwTaskbarButton > TASKBAR_BUTTON_NORMAL) {
			m_bHideWindow = TRUE;
		} else {
			::ShowWindow(m_hWnd, SW_MINIMIZE);
		}
	} else {
		::ShowWindow(m_hWnd, SW_SHOW);
	}

	::SetForegroundWindow(m_hWnd);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Message handlers

/////////////////////////////////////////////////////////////////////////////

void Console::OnDestroy() {

	if (!m_bInitializing) {
		::SetEvent(m_hQuitEvent);
		::WaitForSingleObject(m_hMonitorThread, 2000);
		::CloseHandle(m_hMonitorThread);

		// kill timers
		::KillTimer(m_hWnd, TIMER_REPAINT_CHANGE);
		if (m_dwMasterRepaintInt) ::KillTimer(m_hWnd, TIMER_REPAINT_MASTER);

		DestroyCursor();

		// cleanup graphics objects
		if (m_hFontOld) ::SelectObject(m_hdcConsole, m_hFontOld);
		if (m_hFont) ::DeleteObject(m_hFont);

		if (m_hBkBrush) ::DeleteObject(m_hBkBrush);

		if (m_hbmpConsoleOld) ::SelectObject(m_hdcConsole, m_hbmpConsoleOld);
		if (m_hbmpConsole) ::DeleteObject(m_hbmpConsole);

		if (m_hbmpWindowOld) ::SelectObject(m_hdcWindow, m_hbmpWindowOld);
		if (m_hbmpWindow) ::DeleteObject(m_hbmpWindow);

		if (m_hbmpBackgroundOld) ::SelectObject(m_hdcBackground, m_hbmpBackgroundOld);
		if (m_hbmpBackground) ::DeleteObject(m_hbmpBackground);
		if (m_hdcBackground) ::DeleteDC(m_hdcBackground);

		if (m_hbmpSelectionOld) ::SelectObject(m_hdcSelection, m_hbmpSelectionOld);
		if (m_hbmpSelection)::DeleteObject(m_hbmpSelection);
		if (m_hdcSelection) ::DeleteDC(m_hdcSelection);

		if (m_hbrushSelection) ::DeleteObject(m_hbrushSelection);

		::DestroyMenu(m_hConfigFilesMenu);
		::DestroyMenu(m_hPopupMenu);

		SetTrayIcon(NIM_DELETE);

		::DeleteDC(m_hdcConsole);
		::DeleteDC(m_hdcWindow);
		::PostQuitMessage(0);
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnNcDestroy() {
	if (!m_bInitializing) delete this;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnPaint() {

	PAINTSTRUCT	paintStruct;
	HDC hdc = ::BeginPaint(m_hWnd, &paintStruct);

	if (!m_nTextSelection) {
		// if there's no selection, just blit console image
		::BitBlt(
			hdc,
			paintStruct.rcPaint.left,
			paintStruct.rcPaint.top,
			paintStruct.rcPaint.right - paintStruct.rcPaint.left + 1,
			paintStruct.rcPaint.bottom - paintStruct.rcPaint.top + 1,
			m_hdcConsole,
			paintStruct.rcPaint.left,
			paintStruct.rcPaint.top,
			SRCCOPY);
	} else {
		// there's a text selection, first compose the image in an off-screen
		// buffer, and then blit it
		::BitBlt(
			m_hdcWindow,
			paintStruct.rcPaint.left,
			paintStruct.rcPaint.top,
			paintStruct.rcPaint.right - paintStruct.rcPaint.left + 1,
			paintStruct.rcPaint.bottom - paintStruct.rcPaint.top + 1,
			m_hdcConsole,
			paintStruct.rcPaint.left,
			paintStruct.rcPaint.top,
			SRCCOPY);

		::BitBlt(
			m_hdcWindow,
			m_rectSelection.left,
			m_rectSelection.top,
			m_rectSelection.right - m_rectSelection.left,
			m_rectSelection.bottom - m_rectSelection.top,
			m_hdcSelection,
			0,
			0,
			SRCINVERT);
		::BitBlt(
			hdc,
			paintStruct.rcPaint.left,
			paintStruct.rcPaint.top,
			paintStruct.rcPaint.right - paintStruct.rcPaint.left + 1,
			paintStruct.rcPaint.bottom - paintStruct.rcPaint.top + 1,
			m_hdcWindow,
			paintStruct.rcPaint.left,
			paintStruct.rcPaint.top,
			SRCCOPY);
	}

	::EndPaint(m_hWnd, &paintStruct);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnPaintTimer() {

	::KillTimer(m_hWnd, TIMER_REPAINT_CHANGE);
	RefreshStdOut();
	RefreshScreenBuffer();

	if (GetChangeRate() > 15) {
		::CopyMemory(m_pScreenBuffer, m_pScreenBufferNew, sizeof(CHAR_INFO) * m_dwRows * m_dwColumns);
		RepaintWindow();
	} else {
		RepaintWindowChanges();
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnCursorTimer() {
	if (m_pCursor) {
		m_pCursor->PrepareNext();
		DrawCursor();
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnWindowPosChanging(WINDOWPOS* lpWndPos) {

	if (!(lpWndPos->flags & SWP_NOMOVE)) {
		if (m_nSnapDst >= 0) {
			// we'll snap Console window to the desktop edges
			RECT rectDesktop;

			GetDesktopRect(rectDesktop);

			m_dwDocked = DOCK_NONE;
			DWORD	dwLeftRight = 0;
			DWORD	dwTopBottom = 0;

			// now, see if we're close to the edges
			if (lpWndPos->x <= rectDesktop.left + m_nSnapDst) {
				lpWndPos->x = rectDesktop.left;
				dwLeftRight = 1;
			}

			if (lpWndPos->x >= rectDesktop.right - m_nWindowWidth - m_nSnapDst) {
				lpWndPos->x = rectDesktop.right - m_nWindowWidth;
				dwLeftRight = 2;
			}

			if (lpWndPos->y <= rectDesktop.top + m_nSnapDst) {
				lpWndPos->y = rectDesktop.top;
				dwTopBottom = 1;
			}

			if (lpWndPos->y >= rectDesktop.bottom - m_nWindowHeight - m_nSnapDst) {
				lpWndPos->y = rectDesktop.bottom - m_nWindowHeight;
				dwTopBottom = 2;
			}

			// now, see if the window is docked
			if (dwLeftRight == 1) {
				// left edge
				if (dwTopBottom == 1) {
					// top left
					m_dwDocked = DOCK_TOP_LEFT;
				} else if (dwTopBottom == 2) {
					// bottom left
					m_dwDocked = DOCK_BOTTOM_LEFT;
				}
			} else if (dwLeftRight == 2) {
				// right edge
				if (dwTopBottom == 1) {
					// top right
					m_dwDocked = DOCK_TOP_RIGHT;
				} else if (dwTopBottom == 2) {
					// bottom right
					m_dwDocked = DOCK_BOTTOM_RIGHT;
				}
			}
		}

		m_nX = lpWndPos->x;
		m_nY = lpWndPos->y;

//		TRACE(_T("Win pos: %ix%i\n"), m_nX, m_nY);

		// we need to repaint for relative backgrounds
//		if (m_bRelativeBackground && !m_bInitializing) RepaintWindow();
	}

	if (m_dwCurrentZOrder == Z_ORDER_ONBOTTOM) lpWndPos->hwndInsertAfter = HWND_BOTTOM;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnActivateApp(BOOL bActivate, DWORD dwFlags) {

	if (m_pCursor) {
		m_pCursor->SetState(bActivate);
		DrawCursor();
	}

#if 0
	if ((m_dwTransparency == TRANSPARENCY_ALPHA) && (m_byInactiveAlpha > 0)) {
		if (bActivate) {
			g_pfnSetLayeredWndAttr(m_hWnd, m_crBackground, m_byAlpha, LWA_ALPHA);
		} else {
			g_pfnSetLayeredWndAttr(m_hWnd, m_crBackground, m_byInactiveAlpha, LWA_ALPHA);
		}

	}
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnVScroll(WPARAM wParam) {

//	TRACE(_T("VScroll\n"));

	int nCurrentPos = ::GetScrollPos(m_hWnd, SB_VERT);
	int nDelta = 0;

	switch(LOWORD (wParam)) {

	case SB_PAGEUP:
		nDelta = -5;
		break;

	case SB_PAGEDOWN:
		nDelta = 5;
		break;

	case SB_LINEUP:
		nDelta = -1;
		break;

	case SB_LINEDOWN:
		nDelta = 1;
		break;

	case SB_THUMBTRACK:
		nDelta = HIWORD(wParam) - nCurrentPos;
		break;

	case SB_ENDSCROLL:

		return;

	default:
		return;
	}

	if ((nDelta = max(-nCurrentPos, min(nDelta, (int)(m_dwBufferRows-m_dwRows) - nCurrentPos))) != 0) {

		nCurrentPos += nDelta;

		SMALL_RECT sr;
		sr.Top = (short) nCurrentPos;
//		sr.Bottom = nDelta;
		sr.Left = sr.Right = 0;
		m_csbiConsole.srWindow = sr;
//		::SetConsoleWindowInfo(m_hStdOutFresh, FALSE, &sr);

		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask	= SIF_POS;
		si.nPos	= nCurrentPos;
		::FlatSB_SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);

		// this seems to work/look much better than direct repainting...
		::SetTimer(m_hWnd, TIMER_REPAINT_CHANGE, m_dwChangeRepaintInt, NULL);
	}

}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnInputLangChangeRequest(WPARAM wParam, LPARAM lParam) {
	::PostMessage(m_hWndConsole, WM_INPUTLANGCHANGEREQUEST, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnLButtonDown(UINT uiFlags, POINTS points) {
#if 0
	RECT windowRect;
	::GetCursorPos(&m_mouseCursorOffset);
	::GetWindowRect(m_hWnd, &windowRect);
	m_mouseCursorOffset.x -= windowRect.left;
	m_mouseCursorOffset.y -= windowRect.top;

	if (!m_bMouseDragable ||
		(m_bInverseShift == !(uiFlags & MK_SHIFT))) {

		if (m_nCharWidth) {

			if (m_nTextSelection == TEXT_SELECTION_SELECTED) return;

			// fixed-width characters
			// start copy text selection
			::SetCapture(m_hWnd);

			if (!m_nTextSelection) {
				RECT rect;
				rect.left = 0;
				rect.top = 0;
				rect.right = m_nClientWidth;
				rect.bottom = m_nClientHeight;
				::FillRect(m_hdcSelection, &rect, m_hbrushSelection);
			}

			m_nTextSelection = TEXT_SELECTION_SELECTING;

			m_coordSelOrigin.X = min(max(points.x - m_nInsideBorder, 0) / m_nCharWidth, m_dwColumns-1);
			m_coordSelOrigin.Y = min(max(points.y - m_nInsideBorder, 0) / m_nCharHeight, m_dwRows-1);

			m_rectSelection.left = m_rectSelection.right = m_coordSelOrigin.X * m_nCharWidth + m_nInsideBorder;
			m_rectSelection.top = m_rectSelection.bottom = m_coordSelOrigin.Y * m_nCharHeight + m_nInsideBorder;

			TRACE(_T("Starting point: %ix%i\n"), m_coordSelOrigin.X, m_coordSelOrigin.Y);
		}

	} else {
		if (m_nTextSelection) {
			return;
		} else if (m_bMouseDragable) {
			// start to drag window
			::SetCapture(m_hWnd);
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnLButtonUp(UINT uiFlags, POINTS points) {

	if ((m_nTextSelection == TEXT_SELECTION_SELECTED) ||
		// X Windows select/copy style
		((m_nTextSelection == TEXT_SELECTION_SELECTING) && (m_bCopyOnSelect))) {

		// if the user clicked inside the selection rectangle, copy data
		if ((points.x >= m_rectSelection.left) &&
			(points.x <= m_rectSelection.right) &&
			(points.y >= m_rectSelection.top) &&
			(points.y <= m_rectSelection.bottom)) {

			CopyTextToClipboard();
		}

		if (m_bCopyOnSelect) ::ReleaseCapture();
		ClearSelection();

	} else if (m_nTextSelection == TEXT_SELECTION_SELECTING) {

		if ((m_rectSelection.left == m_rectSelection.right) &&
			(m_rectSelection.top == m_rectSelection.bottom)) {

			m_nTextSelection = TEXT_SELECTION_NONE;

		} else {
			m_nTextSelection = TEXT_SELECTION_SELECTED;
		}
		::ReleaseCapture();

	} else if (m_bMouseDragable) {
		// end window drag
		::ReleaseCapture();
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnLButtonDblClick(UINT uiFlags, POINTS points) {

//	ToggleWindowOnTop();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnRButtonUp(UINT uiFlags, POINTS points) {

	if (uiFlags & MK_SHIFT) {
		PasteClipoardText();
	} else {

		if (m_bPopupMenuDisabled) return;

		POINT	point;
		point.x = points.x;
		point.y = points.y;
		::ClientToScreen(m_hWnd, &point);

		HMENU	hPopup = ::GetSubMenu(m_hPopupMenu, 0);

		// show popup menu
		::TrackPopupMenu(hPopup, 0, point.x, point.y, 0, m_hWnd, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnMButtonDown(UINT uiFlags, POINTS points) {

	PasteClipoardText();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnMouseMove(UINT uiFlags, POINTS points) {

#if 0
	RECT	windowRect;
	int		deltaX, deltaY;
	POINT	point;

	if (uiFlags & MK_LBUTTON) {

		::GetWindowRect(m_hWnd, &windowRect);

		point.x = points.x;
		point.y = points.y;

		::ClientToScreen(m_hWnd, &point);

		deltaX = point.x - windowRect.left - m_mouseCursorOffset.x;
		deltaY = point.y - windowRect.top - m_mouseCursorOffset.y;

		if (deltaX | deltaY) {

//			TRACE(_T("m_nTextSelection: %i, Delta X: %i Delta Y: %i\n"), m_nTextSelection, deltaX, deltaY);

			if (m_nTextSelection) {
				if ((!m_bMouseDragable) || (m_bInverseShift == !(uiFlags & MK_SHIFT))) {

					// some text has been selected, just return
					if (m_nTextSelection == TEXT_SELECTION_SELECTED) return;

					// selecting text for copy/paste
					COORD coordSel;

					::InvalidateRect(m_hWnd, &m_rectSelection, FALSE);

					coordSel.X = min(max(points.x - m_nInsideBorder, 0) / m_nCharWidth, m_dwColumns-1);
					coordSel.Y = min(max(points.y - m_nInsideBorder, 0) / m_nCharHeight, m_dwRows-1);

//					TRACE(_T("End point: %ix%i\n"), coordSel.X, coordSel.Y);

					if (coordSel.X >= m_coordSelOrigin.X) {
						m_rectSelection.left = m_coordSelOrigin.X * m_nCharWidth + m_nInsideBorder;
						m_rectSelection.right = (coordSel.X + 1) * m_nCharWidth + m_nInsideBorder;
					} else {
						m_rectSelection.left = coordSel.X * m_nCharWidth + m_nInsideBorder;
						m_rectSelection.right = (m_coordSelOrigin.X + 1) * m_nCharWidth + m_nInsideBorder;
					}

					if (coordSel.Y >= m_coordSelOrigin.Y) {
						m_rectSelection.top = m_coordSelOrigin.Y * m_nCharHeight + m_nInsideBorder;
						m_rectSelection.bottom = (coordSel.Y + 1) * m_nCharHeight + m_nInsideBorder;
					} else {
						m_rectSelection.top = coordSel.Y * m_nCharHeight + m_nInsideBorder;
						m_rectSelection.bottom = (m_coordSelOrigin.Y + 1) * m_nCharHeight + m_nInsideBorder;
					}

//					TRACE(_T("Selection rect: %i,%i x %i,%i\n"), m_rectSelection.left, m_rectSelection.top, m_rectSelection.right, m_rectSelection.bottom);

					::InvalidateRect(m_hWnd, &m_rectSelection, FALSE);
				}

			} else if (m_bMouseDragable) {

				// moving the window
				HWND hwndZ;
				switch (m_dwCurrentZOrder) {
					case Z_ORDER_REGULAR	: hwndZ = HWND_NOTOPMOST; break;
					case Z_ORDER_ONTOP		: hwndZ = HWND_TOPMOST; break;
					case Z_ORDER_ONBOTTOM	: hwndZ = HWND_BOTTOM; break;
				}

				::SetWindowPos(
					m_hWnd,
					hwndZ,
					windowRect.left + deltaX,
					windowRect.top + deltaY,
					0,
					0,
					SWP_NOSIZE);

				::PostMessage(m_hWnd, WM_PAINT, 0, 0);
			}
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnChar(WORD myChar) {
	wchar_t text[16];

	if (myChar == 8)
	{
		int l = Input.length();
		if (l > 0)
		{
			wchar_t last = Input[ l - 1];
			if (last == '\n')
			{
				return;
			}

			text[0] = 8;
			text[1] = L' ';
			text[2] = 8;
			text[3] = 0;

			Input.resize( l - 1);
		}
		else
		{
			return;
		}
	}
	else if (myChar == '\r')
	{
		text[0] = '\n';
		text[1] = '\r';
		text[2] = 0;
		Input += '\n';
		SendTextToConsole( text);
		SendMessage( m_notifyhWnd, WM_INPUTREADY, 0, (LPARAM) &Input);
		return;
	}
	else
	{
		text[0] = myChar;
		text[1] = 0;
		Input += text;
	}

	SendTextToConsole( text);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnSetCursor(WORD wHitTest, WORD wMouseMessage) {

	if (wHitTest == HTBORDER) {
		if (wMouseMessage == WM_LBUTTONDOWN) {
			if (m_bMouseDragable) {
				// start to drag window
				RECT windowRect;
				::GetCursorPos(&m_mouseCursorOffset);
				::GetWindowRect(m_hWnd, &windowRect);
				m_mouseCursorOffset.x -= windowRect.left;
				m_mouseCursorOffset.y -= windowRect.top;
				::SetCapture(m_hWnd);
			}
		} else if (wMouseMessage == WM_LBUTTONUP) {
			if (m_bMouseDragable) {
				// end window drag
				::ReleaseCapture();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnInitMenuPopup(HMENU hMenu, UINT uiPos, BOOL bSysMenu) {

	if ((hMenu != ::GetSubMenu(m_hPopupMenu, 0)) && (hMenu != m_hSysMenu)) return;

	// update configuration files submenu
	UpdateConfigFilesSubmenu();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnDropFiles(HDROP hDrop) {

	UINT	uiFilesCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	tstring	strFilenames(_T(""));

	// concatenate all filenames
	for (UINT i = 0; i < uiFilesCount; ++i) {
		TCHAR	szFilename[MAX_PATH];
		::ZeroMemory(szFilename, sizeof(szFilename));

		::DragQueryFile(hDrop, i, szFilename, MAX_PATH);

		tstring strFilename(szFilename);

		// if there are spaces in the filename, put quotes around it
		if (strFilename.find(_T(" ")) != tstring::npos) strFilename = tstring(_T("\"")) + strFilename + tstring(_T("\""));

		if (i > 0) strFilenames += _T(" ");
		strFilenames += strFilename;

	}
	::DragFinish(hDrop);

	// This function takes text, not file names
//	SendTextToConsole(strFilenames.c_str());
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == 0) {
		// popup menu
		return HandleMenuCommand(LOWORD(wParam));
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::OnSysCommand(WPARAM wParam, LPARAM lParam) {

	return HandleMenuCommand(wParam);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::OnTrayNotify(WPARAM wParam, LPARAM lParam) {

	switch (lParam) {
		case WM_RBUTTONUP: {

			if (m_bPopupMenuDisabled) return;

			POINT	posCursor;

			::GetCursorPos(&posCursor);
			// show popup menu
			::SetForegroundWindow(m_hWnd);
			::TrackPopupMenu(::GetSubMenu(m_hPopupMenu, 0), 0, posCursor.x, posCursor.y, 0, m_hWnd, NULL);
			::PostMessage(m_hWnd, WM_NULL, 0, 0);

			return;
						}

		case WM_LBUTTONDOWN:
			m_bHideWindow = false;
			ShowHideWindow();
			::SetForegroundWindow(m_hWnd);
			return;

		case WM_LBUTTONDBLCLK:
			m_bHideWindow = !m_bHideWindow;
			ShowHideWindow();
			::SetForegroundWindow(m_hWnd);
			return;

		default : return;
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

#if 0
void Console::OnWallpaperChanged(const TCHAR* pszFilename) {

	if (m_dwTransparency == TRANSPARENCY_FAKE) {
		SetWindowTransparency();
		CreateBackgroundBitmap();
		RepaintWindow();
	}

}
#endif

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::GetOptions() {

#if 0
	class XmlException {
	public: XmlException(BOOL bRet) : m_bRet(bRet){};
		BOOL m_bRet;
	};

	BOOL bRet = FALSE;

	IStream*				pFileStream			= NULL;
	IXMLDocument*			pConfigDoc			= NULL;
	IPersistStreamInit*		pPersistStream		= NULL;
	IXMLElement*			pRootElement		= NULL;
	IXMLElementCollection*	pColl				= NULL;
	IXMLElement*			pFontElement		= NULL;
	IXMLElement*			pPositionElement	= NULL;
	IXMLElement*			pAppearanceElement	= NULL;
	IXMLElement*			pScrollbarElement	= NULL;
	IXMLElement*			pBackgroundElement	= NULL;
	IXMLElement*			pCursorElement		= NULL;
	IXMLElement*			pBehaviorElement	= NULL;

	try {
		::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

		USES_CONVERSION;

		// open file stream
		if (!SUCCEEDED(CreateFileStream(
			m_strConfigFile.c_str(),
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL,
			&pFileStream))) {

			throw XmlException(FALSE);
		}

		// create XML document instance
		if (!SUCCEEDED(::CoCreateInstance(
			CLSID_XMLDocument,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IXMLDocument,
			(void**)&pConfigDoc))) {

			throw XmlException(FALSE);
		}

		// load the configuration file
		pConfigDoc->QueryInterface(IID_IPersistStreamInit, (void **)&pPersistStream);

		if (!SUCCEEDED(pPersistStream->Load(pFileStream))) throw XmlException(FALSE);

		// see if we're dealing with the skin
		if (!SUCCEEDED(pConfigDoc->get_root(&pRootElement))) throw XmlException(FALSE);

		CComVariantOut	varAttValue;
		CComBSTROut		bstr;
		CComBSTROut		strText;
		tstring			strTempText(_T(""));

		// root element must be CONSOLE
		pRootElement->get_tagName(bstr.Out());
		bstr.ToUpper();

		if (!(bstr == CComBSTR(_T("CONSOLE")))) throw XmlException(FALSE);

		pRootElement->getAttribute(CComBSTR(_T("title")), varAttValue.Out());
		if (varAttValue.vt == VT_BSTR) {
			m_strWindowTitle = OLE2T(varAttValue.bstrVal);
			m_strWindowTitleCurrent = m_strWindowTitle;
		}

		pRootElement->getAttribute(CComBSTR(_T("refresh")), varAttValue.Out());
		if (varAttValue.vt == VT_BSTR) m_dwMasterRepaintInt = _ttol(OLE2T(varAttValue.bstrVal));

		pRootElement->getAttribute(CComBSTR(_T("change_refresh")), varAttValue.Out());
		if (varAttValue.vt == VT_BSTR) m_dwChangeRepaintInt = _ttol(OLE2T(varAttValue.bstrVal));
		if ((int)m_dwChangeRepaintInt < 5) m_dwChangeRepaintInt = 5;

		pRootElement->getAttribute(CComBSTR(_T("shell")), varAttValue.Out());
		if (varAttValue.vt == VT_BSTR) m_strShell = OLE2T(varAttValue.bstrVal);

		pRootElement->getAttribute(CComBSTR(_T("editor")), varAttValue.Out());
		if (varAttValue.vt == VT_BSTR) m_strConfigEditor = OLE2T(varAttValue.bstrVal);

		pRootElement->getAttribute(CComBSTR(_T("editor_params")), varAttValue.Out());
		if (varAttValue.vt == VT_BSTR) m_strConfigEditorParams = OLE2T(varAttValue.bstrVal);

		pRootElement->get_children(&pColl);
		if (!pColl) throw XmlException(TRUE);

		// get font settings
		IXMLElementCollection*	pFontColl = NULL;
		if (!SUCCEEDED(pColl->item(CComVariant(_T("font")), CComVariant(0), (IDispatch**)&pFontElement))) throw XmlException(FALSE);
		if (pFontElement) {
			if (!SUCCEEDED(pFontElement->get_children(&pFontColl))) throw XmlException(FALSE);

			if (pFontColl) {
				IXMLElement* pFontSubelement = NULL;

				if (!SUCCEEDED(pFontColl->item(CComVariant(_T("size")), CComVariant(0), (IDispatch**)&pFontSubelement))) throw XmlException(FALSE);
				if (pFontSubelement) {
					pFontSubelement->get_text(strText.Out());
					if (strText.Length() > 0) m_dwFontSize = _ttoi(OLE2T(strText));
				}
				SAFERELEASE(pFontSubelement);

				if (!SUCCEEDED(pFontColl->item(CComVariant(_T("italic")), CComVariant(0), (IDispatch**)&pFontSubelement))) throw XmlException(FALSE);
				if (pFontSubelement) {
					pFontSubelement->get_text(strText.Out());
					m_bItalic = !_tcsicmp(OLE2T(strText), _T("true"));
				}
				SAFERELEASE(pFontSubelement);

				if (!SUCCEEDED(pFontColl->item(CComVariant(_T("bold")), CComVariant(0), (IDispatch**)&pFontSubelement))) throw XmlException(FALSE);
				if (pFontSubelement) {
					pFontSubelement->get_text(strText.Out());
					m_bBold = !_tcsicmp(OLE2T(strText), _T("true"));
				}
				SAFERELEASE(pFontSubelement);

				if (!SUCCEEDED(pFontColl->item(CComVariant(_T("name")), CComVariant(0), (IDispatch**)&pFontSubelement))) throw XmlException(FALSE);
				if (pFontSubelement) {
					pFontSubelement->get_text(strText.Out());
					if (strText.Length() > 0) m_strFontName = OLE2T(strText);
				}
				SAFERELEASE(pFontSubelement);

				if (!SUCCEEDED(pFontColl->item(CComVariant(_T("color")), CComVariant(0), (IDispatch**)&pFontSubelement))) throw XmlException(FALSE);
				if (pFontSubelement) {
					BYTE r = 0;
					BYTE g = 0;
					BYTE b = 0;

					varAttValue.Clear();
					pFontSubelement->getAttribute(CComBSTR(_T("r")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) r = _ttoi(OLE2T(varAttValue.bstrVal));
					varAttValue.Clear();
					pFontSubelement->getAttribute(CComBSTR(_T("g")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) g = _ttoi(OLE2T(varAttValue.bstrVal));
					varAttValue.Clear();
					pFontSubelement->getAttribute(CComBSTR(_T("b")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) b = _ttoi(OLE2T(varAttValue.bstrVal));

					m_bUseFontColor = TRUE;
					m_crFontColor = RGB(r, g, b);
				}
				SAFERELEASE(pFontSubelement);

				// get font color mapping
				if (!SUCCEEDED(pFontColl->item(CComVariant(_T("colors")), CComVariant(0), (IDispatch**)&pFontSubelement))) throw XmlException(FALSE);
				if (pFontSubelement) {

					IXMLElementCollection*	pColorsColl = NULL;

					if (!SUCCEEDED(pFontSubelement->get_children(&pColorsColl))) throw XmlException(FALSE);

					if (pColorsColl) {

						for (int i = 0; i < 16; ++i) {
							IXMLElement* pColorSubelement = NULL;
							TCHAR szColorName[32];

							_sntprintf(szColorName, sizeof(szColorName)/sizeof(TCHAR), _T("color_%02i"), i);

							if (!SUCCEEDED(pColorsColl->item(CComVariant(szColorName), CComVariant(0), (IDispatch**)&pColorSubelement))) throw XmlException(FALSE);
							if (pColorSubelement) {

								BYTE r = 0;
								BYTE g = 0;
								BYTE b = 0;

								varAttValue.Clear();
								pColorSubelement->getAttribute(CComBSTR(_T("r")), varAttValue.Out());
								if (varAttValue.vt == VT_BSTR) r = _ttoi(OLE2T(varAttValue.bstrVal));
								varAttValue.Clear();
								pColorSubelement->getAttribute(CComBSTR(_T("g")), varAttValue.Out());
								if (varAttValue.vt == VT_BSTR) g = _ttoi(OLE2T(varAttValue.bstrVal));
								varAttValue.Clear();
								pColorSubelement->getAttribute(CComBSTR(_T("b")), varAttValue.Out());
								if (varAttValue.vt == VT_BSTR) b = _ttoi(OLE2T(varAttValue.bstrVal));

								Console::m_arrConsoleColors[i] = RGB(r, g, b);
							}

							SAFERELEASE(pColorSubelement);
						}
					}
					SAFERELEASE(pColorsColl);
				}
				SAFERELEASE(pFontSubelement);
			}
			SAFERELEASE(pFontColl);
		}

		// get position settings
		IXMLElementCollection*	pPositionColl = NULL;
		if (!SUCCEEDED(pColl->item(CComVariant(_T("position")), CComVariant(0), (IDispatch**)&pPositionElement))) throw XmlException(FALSE);
		if (pPositionElement) {
			if (!SUCCEEDED(pPositionElement->get_children(&pPositionColl))) throw XmlException(FALSE);

			if (pPositionColl) {
				IXMLElement* pPositionSubelement = NULL;

				if (!m_bReloading) {
					if (!SUCCEEDED(pPositionColl->item(CComVariant(_T("x")), CComVariant(0), (IDispatch**)&pPositionSubelement))) throw XmlException(FALSE);
					if (pPositionSubelement) {
						pPositionSubelement->get_text(strText.Out());
						if (strText.Length() > 0) m_nX = _ttoi(OLE2T(strText));
					}
					SAFERELEASE(pPositionSubelement);

					if (!SUCCEEDED(pPositionColl->item(CComVariant(_T("y")), CComVariant(0), (IDispatch**)&pPositionSubelement))) throw XmlException(FALSE);
					if (pPositionSubelement) {
						pPositionSubelement->get_text(strText.Out());
						if (strText.Length() > 0) m_nY = _ttoi(OLE2T(strText));
					}
					SAFERELEASE(pPositionSubelement);
				}

				if (!SUCCEEDED(pPositionColl->item(CComVariant(_T("docked")), CComVariant(0), (IDispatch**)&pPositionSubelement))) throw XmlException(FALSE);
				if (pPositionSubelement) {
					pPositionSubelement->get_text(strText.Out());
					strTempText = OLE2T(strText);

					if (!_tcsicmp(strTempText.c_str(), _T("top left"))) {
						m_dwDocked = DOCK_TOP_LEFT;
					} else if (!_tcsicmp(strTempText.c_str(), _T("top right"))) {
						m_dwDocked = DOCK_TOP_RIGHT;
					} else if (!_tcsicmp(strTempText.c_str(), _T("bottom right"))) {
						m_dwDocked = DOCK_BOTTOM_RIGHT;
					} else if (!_tcsicmp(strTempText.c_str(), _T("bottom left"))) {
						m_dwDocked = DOCK_BOTTOM_LEFT;
					} else {
						m_dwDocked = DOCK_NONE;
					}
				}
				SAFERELEASE(pPositionSubelement);

				if (!SUCCEEDED(pPositionColl->item(CComVariant(_T("snap_distance")), CComVariant(0), (IDispatch**)&pPositionSubelement))) throw XmlException(FALSE);
				if (pPositionSubelement) {
					pPositionSubelement->get_text(strText.Out());
					if (strText.Length() > 0) m_nSnapDst = _ttoi(OLE2T(strText));
				}
				SAFERELEASE(pPositionSubelement);

				if (!SUCCEEDED(pPositionColl->item(CComVariant(_T("z_order")), CComVariant(0), (IDispatch**)&pPositionSubelement))) throw XmlException(FALSE);
				if (pPositionSubelement) {
					pPositionSubelement->get_text(strText.Out());
					strTempText = OLE2T(strText);

					if (!_tcsicmp(strTempText.c_str(), _T("regular"))) {
						m_dwCurrentZOrder = Z_ORDER_REGULAR;
						m_dwOriginalZOrder = Z_ORDER_REGULAR;
					} else if (!_tcsicmp(strTempText.c_str(), _T("on top"))) {
						m_dwCurrentZOrder = Z_ORDER_ONTOP;
						m_dwOriginalZOrder = Z_ORDER_ONTOP;
					} else if (!_tcsicmp(strTempText.c_str(), _T("on bottom"))) {
						m_dwCurrentZOrder = Z_ORDER_ONBOTTOM;
						m_dwOriginalZOrder = Z_ORDER_ONBOTTOM;
					} else {
						m_dwCurrentZOrder = Z_ORDER_REGULAR;
						m_dwOriginalZOrder = Z_ORDER_REGULAR;
					}
				}
				SAFERELEASE(pPositionSubelement);
			}

			SAFERELEASE(pPositionColl);
		}

		// get appearance settings
		IXMLElementCollection*	pAppearanceColl = NULL;
		if (!SUCCEEDED(pColl->item(CComVariant(_T("appearance")), CComVariant(0), (IDispatch**)&pAppearanceElement))) throw XmlException(FALSE);
		if (pAppearanceElement) {
			if (!SUCCEEDED(pAppearanceElement->get_children(&pAppearanceColl))) throw XmlException(FALSE);

			if (pAppearanceColl) {
				IXMLElement* pAppearanaceSubelement = NULL;

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("hide_console")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("true"))) {
						m_bHideConsole = TRUE;
					} else {
						m_bHideConsole = FALSE;
					}
				}
				SAFERELEASE(pAppearanaceSubelement);

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("hide_console_timeout")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->get_text(strText.Out());
					if (strText.Length() > 0) m_dwHideConsoleTimeout = _ttoi(OLE2T(strText));
				}
				SAFERELEASE(pAppearanaceSubelement);

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("start_minimized")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("true"))) {
						m_bStartMinimized = TRUE;
					} else {
						m_bStartMinimized = FALSE;
					}
				}
				SAFERELEASE(pAppearanaceSubelement);

				IXMLElementCollection*	pScrollbarColl = NULL;
				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("scrollbar")), CComVariant(0), (IDispatch**)&pScrollbarElement))) throw XmlException(FALSE);
				if (pScrollbarElement) {
					if (!SUCCEEDED(pScrollbarElement->get_children(&pScrollbarColl))) throw XmlException(FALSE);

					if (pScrollbarColl) {
						IXMLElement* pScrollbarSubelement = NULL;

						if (!SUCCEEDED(pScrollbarColl->item(CComVariant(_T("color")), CComVariant(0), (IDispatch**)&pScrollbarSubelement))) throw XmlException(FALSE);
						if (pScrollbarSubelement) {
							BYTE r = 0;
							BYTE g = 0;
							BYTE b = 0;

							pScrollbarSubelement->getAttribute(CComBSTR(_T("r")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) r = _ttoi(OLE2T(varAttValue.bstrVal));
							pScrollbarSubelement->getAttribute(CComBSTR(_T("g")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) g = _ttoi(OLE2T(varAttValue.bstrVal));
							pScrollbarSubelement->getAttribute(CComBSTR(_T("b")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) b = _ttoi(OLE2T(varAttValue.bstrVal));

							m_crScrollbarColor = RGB(r, g, b);
						}
						SAFERELEASE(pScrollbarSubelement);

						if (!SUCCEEDED(pScrollbarColl->item(CComVariant(_T("style")), CComVariant(0), (IDispatch**)&pScrollbarSubelement))) throw XmlException(FALSE);
						if (pScrollbarSubelement) {
							pScrollbarSubelement->get_text(strText.Out());
							strTempText = OLE2T(strText);

							if (!_tcsicmp(strTempText.c_str(), _T("regular"))) {
								m_nScrollbarStyle = FSB_REGULAR_MODE;
							} else if (!_tcsicmp(strTempText.c_str(), _T("flat"))) {
								m_nScrollbarStyle = FSB_FLAT_MODE;
							} else if (!_tcsicmp(strTempText.c_str(), _T("encarta"))) {
								m_nScrollbarStyle = FSB_ENCARTA_MODE;
							}
						}
						SAFERELEASE(pScrollbarSubelement);


						if (!SUCCEEDED(pScrollbarColl->item(CComVariant(_T("width")), CComVariant(0), (IDispatch**)&pScrollbarSubelement))) throw XmlException(FALSE);
						if (pScrollbarSubelement) {
							pScrollbarSubelement->get_text(strText.Out());
							if (strText.Length() > 0) m_nScrollbarWidth = _ttoi(OLE2T(strText));
						}
						SAFERELEASE(pScrollbarSubelement);

						if (!SUCCEEDED(pScrollbarColl->item(CComVariant(_T("button_height")), CComVariant(0), (IDispatch**)&pScrollbarSubelement))) throw XmlException(FALSE);
						if (pScrollbarSubelement) {
							pScrollbarSubelement->get_text(strText.Out());
							if (strText.Length() > 0) m_nScrollbarButtonHeight = _ttoi(OLE2T(strText));
						}
						SAFERELEASE(pScrollbarSubelement);

						if (!SUCCEEDED(pScrollbarColl->item(CComVariant(_T("thumb_height")), CComVariant(0), (IDispatch**)&pScrollbarSubelement))) throw XmlException(FALSE);
						if (pScrollbarSubelement) {
							pScrollbarSubelement->get_text(strText.Out());
							if (strText.Length() > 0) m_nScrollbarThunmbHeight = _ttoi(OLE2T(strText));
						}
						SAFERELEASE(pScrollbarSubelement);
					}
					SAFERELEASE(pScrollbarColl);
				}

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("border")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("true")) || !_tcsicmp(OLE2T(strText), _T("regular"))) {
						m_dwWindowBorder = BORDER_REGULAR;
					} else if (!_tcsicmp(OLE2T(strText), _T("thin"))) {
						m_dwWindowBorder = BORDER_THIN;
					} else {
						m_dwWindowBorder = BORDER_NONE;
					}
				}
				SAFERELEASE(pAppearanaceSubelement);

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("inside_border")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->get_text(strText.Out());
					if (strText.Length() > 0) m_nInsideBorder = _ttoi(OLE2T(strText));
				}
				SAFERELEASE(pAppearanaceSubelement);

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("taskbar_button")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("hide"))) {
						m_dwTaskbarButton = TASKBAR_BUTTON_HIDE;
					} else if (!_tcsicmp(OLE2T(strText), _T("tray"))) {
						m_dwTaskbarButton = TASKBAR_BUTTON_TRAY;
					} else {
						m_dwTaskbarButton = TASKBAR_BUTTON_NORMAL;
					}
				}
				SAFERELEASE(pAppearanaceSubelement);

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("size")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {

					pAppearanaceSubelement->getAttribute(CComBSTR(_T("rows")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) {
						if (_tcsicmp(OLE2T(varAttValue.bstrVal), _T("max")) == 0) {
							m_dwRows = 0;
						} else {
							m_dwRows = _ttoi(OLE2T(varAttValue.bstrVal));
						}
					}

					pAppearanaceSubelement->getAttribute(CComBSTR(_T("columns")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) {
						if (_tcsicmp(OLE2T(varAttValue.bstrVal), _T("max")) == 0) {
							m_dwColumns = 0;
						} else {
							m_dwColumns = _ttoi(OLE2T(varAttValue.bstrVal));
						}
					}

					pAppearanaceSubelement->getAttribute(CComBSTR(_T("buffer_rows")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) {
						m_dwBufferRows = _ttoi(OLE2T(varAttValue.bstrVal));
						m_bUseTextBuffer = TRUE;
					} else {
						m_dwBufferRows = m_dwRows;
					}
				}
				SAFERELEASE(pAppearanaceSubelement);

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("transparency")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->getAttribute(CComBSTR(_T("alpha")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) m_byAlpha = (BYTE)_ttoi(OLE2T(varAttValue.bstrVal));
					pAppearanaceSubelement->getAttribute(CComBSTR(_T("inactive_alpha")), varAttValue.Out());
					if (varAttValue.vt == VT_BSTR) m_byInactiveAlpha = (BYTE)_ttoi(OLE2T(varAttValue.bstrVal));

					pAppearanaceSubelement->get_text(strText.Out());
					strTempText = OLE2T(strText);

					if (!_tcsicmp(strTempText.c_str(), _T("none"))) {
						m_dwTransparency = TRANSPARENCY_NONE;
					} else if (!_tcsicmp(strTempText.c_str(), _T("alpha"))) {
						m_dwTransparency = TRANSPARENCY_ALPHA;
					} else if (!_tcsicmp(strTempText.c_str(), _T("color key"))) {
						m_dwTransparency = TRANSPARENCY_COLORKEY;
					} else if (!_tcsicmp(strTempText.c_str(), _T("fake"))) {
						m_dwTransparency = TRANSPARENCY_FAKE;
					}

				}
				SAFERELEASE(pAppearanaceSubelement);

				// get background settings
				IXMLElementCollection*	pBackgroundColl = NULL;
				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("background")), CComVariant(0), (IDispatch**)&pBackgroundElement))) throw XmlException(FALSE);
				if (pBackgroundElement) {
					if (!SUCCEEDED(pBackgroundElement->get_children(&pBackgroundColl))) throw XmlException(FALSE);

					if (pBackgroundColl) {
						IXMLElement* pBackgroundSubelement = NULL;

						if (!SUCCEEDED(pBackgroundColl->item(CComVariant(_T("color")), CComVariant(0), (IDispatch**)&pBackgroundSubelement))) throw XmlException(FALSE);
						if (pBackgroundSubelement) {
							BYTE r = 0;
							BYTE g = 0;
							BYTE b = 0;

							pBackgroundSubelement->getAttribute(CComBSTR(_T("r")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) r = _ttoi(OLE2T(varAttValue.bstrVal));
							pBackgroundSubelement->getAttribute(CComBSTR(_T("g")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) g = _ttoi(OLE2T(varAttValue.bstrVal));
							pBackgroundSubelement->getAttribute(CComBSTR(_T("b")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) b = _ttoi(OLE2T(varAttValue.bstrVal));

							m_crBackground			= RGB(r, g, b);
						}
						SAFERELEASE(pBackgroundSubelement);

						if (!SUCCEEDED(pBackgroundColl->item(CComVariant(_T("console_color")), CComVariant(0), (IDispatch**)&pBackgroundSubelement))) throw XmlException(FALSE);
						if (pBackgroundSubelement) {
							BYTE r = 0;
							BYTE g = 0;
							BYTE b = 0;

							pBackgroundSubelement->getAttribute(CComBSTR(_T("r")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) r = _ttoi(OLE2T(varAttValue.bstrVal));
							pBackgroundSubelement->getAttribute(CComBSTR(_T("g")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) g = _ttoi(OLE2T(varAttValue.bstrVal));
							pBackgroundSubelement->getAttribute(CComBSTR(_T("b")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) b = _ttoi(OLE2T(varAttValue.bstrVal));

							m_crConsoleBackground = RGB(r, g, b);
						}
						SAFERELEASE(pBackgroundSubelement);

						if (!SUCCEEDED(pBackgroundColl->item(CComVariant(_T("tint")), CComVariant(0), (IDispatch**)&pBackgroundSubelement))) throw XmlException(FALSE);
						if (pBackgroundSubelement) {
							BYTE r = 0;
							BYTE g = 0;
							BYTE b = 0;

							pBackgroundSubelement->getAttribute(CComBSTR(_T("r")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) m_byTintR = _ttoi(OLE2T(varAttValue.bstrVal));
							pBackgroundSubelement->getAttribute(CComBSTR(_T("g")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) m_byTintG = _ttoi(OLE2T(varAttValue.bstrVal));
							pBackgroundSubelement->getAttribute(CComBSTR(_T("b")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) m_byTintB = _ttoi(OLE2T(varAttValue.bstrVal));
							pBackgroundSubelement->getAttribute(CComBSTR(_T("opacity")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) m_byTintOpacity = _ttoi(OLE2T(varAttValue.bstrVal));

							if (m_byTintOpacity > 100) m_byTintOpacity = 50;
							m_bTintSet = TRUE;
						}
						SAFERELEASE(pBackgroundSubelement);

						if (!SUCCEEDED(pBackgroundColl->item(CComVariant(_T("image")), CComVariant(0), (IDispatch**)&pBackgroundSubelement))) throw XmlException(FALSE);
						if (pBackgroundSubelement) {

							pBackgroundSubelement->getAttribute(CComBSTR(_T("style")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) {
								if (!_tcsicmp(OLE2T(varAttValue.bstrVal), _T("resize"))) {
									m_dwBackgroundStyle = BACKGROUND_STYLE_RESIZE;
								} else if (!_tcsicmp(OLE2T(varAttValue.bstrVal), _T("center"))) {
									m_dwBackgroundStyle = BACKGROUND_STYLE_CENTER;
								} else if (!_tcsicmp(OLE2T(varAttValue.bstrVal), _T("tile"))) {
									m_dwBackgroundStyle = BACKGROUND_STYLE_TILE;
								}
							}

							pBackgroundSubelement->getAttribute(CComBSTR(_T("relative")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) {
								if (!_tcsicmp(OLE2T(varAttValue.bstrVal), _T("true"))) {
									m_bRelativeBackground = TRUE;
								} else {
									m_bRelativeBackground = FALSE;
								}
							}

							pBackgroundSubelement->getAttribute(CComBSTR(_T("extend")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) {
								if (!_tcsicmp(OLE2T(varAttValue.bstrVal), _T("true"))) {
									m_bExtendBackground = TRUE;
								} else {
									m_bExtendBackground = FALSE;
								}
							}

							pBackgroundSubelement->get_text(strText.Out());
							m_strBackgroundFile = OLE2T(strText);
							m_bBitmapBackground = TRUE;
						}
						SAFERELEASE(pBackgroundSubelement);
					}
					SAFERELEASE(pBackgroundColl);
				}

				IXMLElementCollection*	pCursorColl = NULL;
				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("cursor")), CComVariant(0), (IDispatch**)&pCursorElement))) throw XmlException(FALSE);
				if (pCursorElement) {
					if (!SUCCEEDED(pCursorElement->get_children(&pCursorColl))) throw XmlException(FALSE);

					if (pCursorColl) {
						IXMLElement* pCursorSubelement = NULL;

						if (!SUCCEEDED(pCursorColl->item(CComVariant(_T("color")), CComVariant(0), (IDispatch**)&pCursorSubelement))) throw XmlException(FALSE);
						if (pCursorSubelement) {
							BYTE r = 0;
							BYTE g = 0;
							BYTE b = 0;

							pCursorSubelement->getAttribute(CComBSTR(_T("r")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) r = _ttoi(OLE2T(varAttValue.bstrVal));
							pCursorSubelement->getAttribute(CComBSTR(_T("g")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) g = _ttoi(OLE2T(varAttValue.bstrVal));
							pCursorSubelement->getAttribute(CComBSTR(_T("b")), varAttValue.Out());
							if (varAttValue.vt == VT_BSTR) b = _ttoi(OLE2T(varAttValue.bstrVal));

							m_crCursorColor = RGB(r, g, b);
						}
						SAFERELEASE(pCursorSubelement);

						if (!SUCCEEDED(pCursorColl->item(CComVariant(_T("style")), CComVariant(0), (IDispatch**)&pCursorSubelement))) throw XmlException(FALSE);
						if (pCursorSubelement) {
							pCursorSubelement->get_text(strText.Out());
							strTempText = OLE2T(strText);

							if (!_tcsicmp(strTempText.c_str(), _T("none"))) {
								m_dwCursorStyle = CURSOR_STYLE_NONE;
							} else if (!_tcsicmp(strTempText.c_str(), _T("XTerm"))) {
								m_dwCursorStyle = CURSOR_STYLE_XTERM;
							} else if (!_tcsicmp(strTempText.c_str(), _T("block"))) {
								m_dwCursorStyle = CURSOR_STYLE_BLOCK;
							} else if (!_tcsicmp(strTempText.c_str(), _T("noblink block"))) {
								m_dwCursorStyle = CURSOR_STYLE_NBBLOCK;
							} else if (!_tcsicmp(strTempText.c_str(), _T("pulse block"))) {
								m_dwCursorStyle = CURSOR_STYLE_PULSEBLOCK;
							} else if (!_tcsicmp(strTempText.c_str(), _T("bar"))) {
								m_dwCursorStyle = CURSOR_STYLE_BAR;
							} else if (!_tcsicmp(strTempText.c_str(), _T("console"))) {
								m_dwCursorStyle = CURSOR_STYLE_CONSOLE;
							} else if (!_tcsicmp(strTempText.c_str(), _T("noblink line"))) {
								m_dwCursorStyle = CURSOR_STYLE_NBHLINE;
							} else if (!_tcsicmp(strTempText.c_str(), _T("horizontal line"))) {
								m_dwCursorStyle = CURSOR_STYLE_HLINE;
							} else if (!_tcsicmp(strTempText.c_str(), _T("vertical line"))) {
								m_dwCursorStyle = CURSOR_STYLE_VLINE;
							} else if (!_tcsicmp(strTempText.c_str(), _T("rect"))) {
								m_dwCursorStyle = CURSOR_STYLE_RECT;
							} else if (!_tcsicmp(strTempText.c_str(), _T("noblink rect"))) {
								m_dwCursorStyle = CURSOR_STYLE_NBRECT;
							} else if (!_tcsicmp(strTempText.c_str(), _T("pulse rect"))) {
								m_dwCursorStyle = CURSOR_STYLE_PULSERECT;
							} else if (!_tcsicmp(strTempText.c_str(), _T("fading block"))) {
								m_dwCursorStyle = CURSOR_STYLE_FADEBLOCK;
							}
						}
						SAFERELEASE(pCursorSubelement);
					}
					SAFERELEASE(pCursorColl);
				}

				if (!SUCCEEDED(pAppearanceColl->item(CComVariant(_T("icon")), CComVariant(0), (IDispatch**)&pAppearanaceSubelement))) throw XmlException(FALSE);
				if (pAppearanaceSubelement) {
					pAppearanaceSubelement->get_text(strText.Out());
					if (strText.Length() > 0) m_strIconFilename = OLE2T(strText);
				}
				SAFERELEASE(pAppearanaceSubelement);

			}
			SAFERELEASE(pAppearanceColl);
		}

		// get behaviour settings
		IXMLElementCollection*	pBehaviorColl = NULL;
		if (!SUCCEEDED(pColl->item(CComVariant(_T("behaviour")), CComVariant(0), (IDispatch**)&pBehaviorElement))) {
			if (!SUCCEEDED(pColl->item(CComVariant(_T("behavior")), CComVariant(0), (IDispatch**)&pBehaviorElement))) throw XmlException(FALSE);
		}

		if (pBehaviorElement) {
			if (!SUCCEEDED(pBehaviorElement->get_children(&pBehaviorColl))) throw XmlException(FALSE);

			if (pBehaviorColl) {
				IXMLElement* pBehaviourSubelement = NULL;

				if (!SUCCEEDED(pBehaviorColl->item(CComVariant(_T("mouse_drag")), CComVariant(0), (IDispatch**)&pBehaviourSubelement))) throw XmlException(FALSE);
				if (pBehaviourSubelement) {
					pBehaviourSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("true"))) {
						m_bMouseDragable = TRUE;
					} else {
						m_bMouseDragable = FALSE;
					}
				}
				SAFERELEASE(pBehaviourSubelement);

				if (!SUCCEEDED(pBehaviorColl->item(CComVariant(_T("copy_on_select")), CComVariant(0), (IDispatch**)&pBehaviourSubelement))) throw XmlException(FALSE);
				if (pBehaviourSubelement) {
					pBehaviourSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("true"))) {
						m_bCopyOnSelect = TRUE;
					} else {
						m_bCopyOnSelect = FALSE;
					}
				}
				SAFERELEASE(pBehaviourSubelement);

				if (!SUCCEEDED(pBehaviorColl->item(CComVariant(_T("inverse_shift")), CComVariant(0), (IDispatch**)&pBehaviourSubelement))) throw XmlException(FALSE);
				if (pBehaviourSubelement) {
					pBehaviourSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("true"))) {
						m_bInverseShift = TRUE;
					} else {
						m_bInverseShift = FALSE;
					}
				}
				SAFERELEASE(pBehaviourSubelement);

				if (!SUCCEEDED(pBehaviorColl->item(CComVariant(_T("reload_new_config")), CComVariant(0), (IDispatch**)&pBehaviourSubelement))) throw XmlException(FALSE);
				if (pBehaviourSubelement) {
					pBehaviourSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("yes"))) {
						m_dwReloadNewConfig = RELOAD_NEW_CONFIG_YES;
					} else if (!_tcsicmp(OLE2T(strText), _T("no"))) {
						m_dwReloadNewConfig = RELOAD_NEW_CONFIG_NO;
					} else {
						m_dwReloadNewConfig = RELOAD_NEW_CONFIG_PROMPT;
					}
				}
				SAFERELEASE(pBehaviourSubelement);

				if (!SUCCEEDED(pBehaviorColl->item(CComVariant(_T("disable_menu")), CComVariant(0), (IDispatch**)&pBehaviourSubelement))) throw XmlException(FALSE);
				if (pBehaviourSubelement) {
					pBehaviourSubelement->get_text(strText.Out());
					if (!_tcsicmp(OLE2T(strText), _T("true"))) {
						m_bPopupMenuDisabled = TRUE;
					} else {
						m_bPopupMenuDisabled = FALSE;
					}
				}
				SAFERELEASE(pBehaviourSubelement);

			}
			SAFERELEASE(pBehaviorColl);
		}

		bRet = TRUE;

	} catch (const XmlException& e) {
		bRet = e.m_bRet;
	}

	SAFERELEASE(pBehaviorElement);
	SAFERELEASE(pCursorElement);
	SAFERELEASE(pBackgroundElement);
	SAFERELEASE(pScrollbarElement);
	SAFERELEASE(pAppearanceElement);
	SAFERELEASE(pPositionElement);
	SAFERELEASE(pFontElement);
	SAFERELEASE(pColl);
	SAFERELEASE(pRootElement);
	SAFERELEASE(pPersistStream);
	SAFERELEASE(pConfigDoc);
	SAFERELEASE(pFileStream);

	::CoUninitialize();
	return bRet;
#endif
	return 1;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::RegisterWindowClasses() {
	WNDCLASSEX wcx;

	// register the Console window class
	::ZeroMemory(&wcx, sizeof(WNDCLASSEX));
	wcx.cbSize			= sizeof(wcx);
	wcx.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wcx.lpfnWndProc		= Console::WindowProc;
	wcx.cbClsExtra		= 0;
	wcx.cbWndExtra		= sizeof(void*);
	wcx.hInstance		= ghInstance;
	wcx.hIcon			= ::LoadIcon(ghInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcx.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground	= ::CreateSolidBrush(RGB(0, 0, 0));
	wcx.lpszMenuName	=	NULL;
	wcx.lpszClassName	= Console::m_szConsoleClass;
	wcx.hIconSm			= NULL;

	if ((::RegisterClassEx(&wcx) == 0) && (::GetLastError() != ERROR_CLASS_ALREADY_EXISTS)) return FALSE;

	// register the hidden parent window class
	::ZeroMemory(&wcx, sizeof(WNDCLASSEX));
	wcx.cbSize			= sizeof(wcx);
	wcx.style			= 0;
	wcx.lpfnWndProc		= ::DefWindowProc;
	wcx.cbClsExtra		= 0;
	wcx.cbWndExtra		= 0;
	wcx.hInstance		= ghInstance;
	wcx.hIcon			= NULL;
	wcx.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground	= NULL;
	wcx.lpszMenuName	= NULL;
	wcx.lpszClassName	= Console::m_szHiddenParentClass;
	wcx.hIconSm			= NULL;

	if ((::RegisterClassEx(&wcx) == 0) && (::GetLastError() != ERROR_CLASS_ALREADY_EXISTS)) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::SetupMenus() {

	MENUITEMINFO	mii;

	// setup popup menu
	if ((m_hPopupMenu = ::LoadMenu(ghInstance, MAKEINTRESOURCE(IDR_POPUP_MENU))) == NULL) return FALSE;
	if ((m_hConfigFilesMenu = ::CreateMenu()) == NULL) return FALSE;

	// add stuff to the system menu
	TCHAR*			arrMenuItems[] = {_T("&Copy"), _T("&Paste")};
	DWORD			arrMenuTypes[] = {MFT_STRING, MFT_STRING};
	DWORD			arrMenuIDs[] = { ID_COPY, ID_PASTE};

	m_hSysMenu = ::GetSystemMenu(m_hWnd, FALSE);

	for (int i = 0; i < sizeof(arrMenuIDs)/sizeof(arrMenuIDs[0]); ++i) {
		::ZeroMemory(&mii, sizeof(MENUITEMINFO));
		mii.cbSize		= sizeof(MENUITEMINFO);
		mii.fMask		= MIIM_TYPE | MIIM_ID;
		mii.fType		= arrMenuTypes[i];
		if (mii.fType == MFT_STRING) {
			mii.wID			= arrMenuIDs[i];
			mii.dwTypeData	= arrMenuItems[i];
			mii.cch			= _tcslen(arrMenuItems[i]);
		} else {
			mii.wID			= 0;
		}
		::InsertMenuItem(m_hSysMenu, SC_CLOSE, FALSE, &mii);
	}

	// set config files menu as a submenu of "Select configuration file" item
	// in the popup and system menus
	::ZeroMemory(&mii, sizeof(MENUITEMINFO));
	mii.cbSize		= sizeof(MENUITEMINFO);
	mii.fMask		= MIIM_SUBMENU;
	mii.hSubMenu	= m_hConfigFilesMenu;

//	::SetMenuItemInfo(m_hPopupMenu, ID_SEL_CONFIG_FILE, FALSE, &mii);
//	::SetMenuItemInfo(m_hSysMenu, ID_SEL_CONFIG_FILE, FALSE, &mii);

	// load the initial list (so the list can be used by external programs)
	UpdateConfigFilesSubmenu();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::CreateConsoleWindow() {

	// create the window
	DWORD dwWindowStyle = WS_VSCROLL;
	if (m_dwTaskbarButton == TASKBAR_BUTTON_NORMAL) dwWindowStyle |= WS_MINIMIZEBOX;

	switch (m_dwWindowBorder) {
	case BORDER_NONE :
		dwWindowStyle |= WS_POPUPWINDOW & ~WS_BORDER;
		break;
	case BORDER_REGULAR :
		dwWindowStyle |= WS_CAPTION|WS_SYSMENU;
		break;

	case BORDER_THIN :
		dwWindowStyle |= WS_POPUPWINDOW;
		break;

	default: dwWindowStyle |= WS_POPUPWINDOW & ~WS_BORDER;
	}

	// If the taskbar style is set to hide or tray, we create an invisible window
	// to parent the main window to. This hides the taskbar button while allowing
	// us to use the normal title bar.
	if (m_dwTaskbarButton > TASKBAR_BUTTON_NORMAL) {
		if ((m_hwndInvisParent = ::CreateWindowEx(
			WS_EX_TOOLWINDOW,
			Console::m_szHiddenParentClass,
			_T(""),
			WS_POPUP,
			0,
			0,
			0,
			0,
			NULL,
			NULL,
			ghInstance,
			NULL)) == NULL) {

			return FALSE;
		}

		// don't let the user minimize it if there's no way to restore it
		if (m_dwTaskbarButton == TASKBAR_BUTTON_HIDE) dwWindowStyle &= ~WS_MINIMIZEBOX;
	}


	if ((m_hWnd = ::CreateWindowEx(
		0, //m_dwTaskbarButton > TASKBAR_BUTTON_NORMAL ? WS_EX_TOOLWINDOW : 0,
		Console::m_szConsoleClass,
		m_strWindowTitle.c_str(),
		dwWindowStyle,
		((m_nX == -1) || (m_nY == -1)) ? CW_USEDEFAULT : 0,
		0,
		0,
		0,
		m_hwndInvisParent,
		NULL,
		ghInstance,
		NULL)) == NULL) {

		return FALSE;
	}

	::SetWindowLongPtr( m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	if ((m_nX == -1) || (m_nY == -1)) {
		RECT rectWindow;

		::GetWindowRect(m_hWnd, &rectWindow);
		m_nX = (int)rectWindow.left;
		m_nY = (int)rectWindow.top;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::CreateNewFont() {

	// create a new font
	if (m_hFontOld) ::SelectObject(m_hdcConsole, m_hFontOld);
	if (m_hFont) ::DeleteObject(m_hFont);

	m_hFont = ::CreateFont(
		-MulDiv(m_dwFontSize, ::GetDeviceCaps(m_hdcConsole, LOGPIXELSY), 72),
		0,
		0,
		0,
		m_bBold ? FW_BOLD : 0,
		m_bItalic,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		m_strFontName.c_str());

	m_hFontOld = (HFONT)::SelectObject(m_hdcConsole, m_hFont);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::CreateNewBrush() {

	// create a new background brush
	if (m_hBkBrush) ::DeleteObject(m_hBkBrush);
	m_hBkBrush = ::CreateSolidBrush(m_crBackground);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::CreateCursor() {
	switch (m_dwCursorStyle) {
		case CURSOR_STYLE_XTERM :
			m_pCursor = (Cursor*)new XTermCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_BLOCK :
			m_pCursor = (Cursor*)new BlockCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_NBBLOCK :
			m_pCursor = (Cursor*)new NBBlockCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_PULSEBLOCK :
			m_pCursor = (Cursor*)new PulseBlockCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_BAR :
			m_pCursor = (Cursor*)new BarCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_CONSOLE :
			m_pCursor = (Cursor*)new ConsoleCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_NBHLINE :
			m_pCursor = (Cursor*)new NBHLineCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_HLINE :
			m_pCursor = (Cursor*)new HLineCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_VLINE :
			m_pCursor = (Cursor*)new VLineCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_RECT :
			m_pCursor = (Cursor*)new RectCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_NBRECT :
			m_pCursor = (Cursor*)new NBRectCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_PULSERECT :
			m_pCursor = (Cursor*)new PulseRectCursor(m_hWnd, m_hdcConsole, m_crCursorColor);
			break;
		case CURSOR_STYLE_FADEBLOCK :
			m_pCursor = (Cursor*)new FadeBlockCursor(m_hWnd, m_hdcConsole, m_crCursorColor, m_crBackground);
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::CreateOffscreenBuffers() {

	HDC hdcDesktop = ::GetDCEx(m_hWnd, NULL, 0);

	// create new window bitmap
	if (m_hbmpConsoleOld) ::SelectObject(m_hdcConsole, m_hbmpConsoleOld);
	if (m_hbmpConsole) ::DeleteObject(m_hbmpConsole);

	m_hbmpConsole = ::CreateCompatibleBitmap(hdcDesktop, m_nClientWidth, m_nClientHeight);
	m_hbmpConsoleOld = (HBITMAP)::SelectObject(m_hdcConsole, m_hbmpConsole);

	if (m_hbmpWindowOld) ::SelectObject(m_hdcWindow, m_hbmpWindowOld);
	if (m_hbmpWindow) ::DeleteObject(m_hbmpWindow);

	m_hbmpWindow = ::CreateCompatibleBitmap(hdcDesktop, m_nClientWidth, m_nClientHeight);
	m_hbmpWindowOld = (HBITMAP)::SelectObject(m_hdcWindow, m_hbmpWindow);

	// create selection bitmap
	if (m_hbmpSelectionOld) ::SelectObject(m_hdcSelection, m_hbmpSelectionOld);
	if (m_hbmpSelection) ::DeleteObject(m_hbmpSelection);

	m_hbmpSelection = ::CreateCompatibleBitmap(m_hdcWindow, m_nClientWidth, m_nClientHeight);
	m_hbmpSelectionOld = (HBITMAP)::SelectObject(m_hdcSelection, m_hbmpSelection);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_nClientWidth;
	rect.bottom = m_nClientHeight;
	::FillRect(m_hdcSelection, &rect, m_hbrushSelection);

	::ReleaseDC(m_hWnd, hdcDesktop);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
#if 0
void Console::CreateBackgroundBitmap() {

	USES_CONVERSION;

	if (m_hbmpBackgroundOld) ::SelectObject(m_hdcBackground, m_hbmpBackgroundOld);
	if (m_hbmpBackground) ::DeleteObject(m_hbmpBackground);
	if (m_hdcBackground) ::DeleteDC(m_hdcBackground);

	if (!m_bBitmapBackground) return;

	// determine the total size of the background bitmap
	DWORD	dwPrimaryDisplayWidth	= ::GetSystemMetrics(SM_CXSCREEN);
	DWORD	dwPrimaryDisplayHeight	= ::GetSystemMetrics(SM_CYSCREEN);

	DWORD	dwBackgroundWidth		= 0;
	DWORD	dwBackgroundHeight		= 0;

	if (m_bRelativeBackground) {

		if (g_bWin2000) {
			// Win2K and later can handle multiple monitors
			dwBackgroundWidth	= ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
			dwBackgroundHeight	= ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

			// get offsets for virtual display
			m_nBackgroundOffsetX = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
			m_nBackgroundOffsetY = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
		} else {
			// WinNT compatibility (hope it works, I didn't test it)
			dwBackgroundWidth	= dwPrimaryDisplayWidth;
			dwBackgroundHeight	= dwBackgroundHeight;
		}

	} else {
		dwBackgroundWidth	= m_nClientWidth;
		dwBackgroundHeight	= m_nClientHeight;
	}

	// now, load the image...
	fipImage	image;
	IMAGE_DATA	imageData;

	if (!image.load(T2A(m_strBackgroundFile.c_str()))) {
		m_bBitmapBackground = FALSE;
		return;
	}

	imageData.hdcImage		= NULL;
	imageData.dwImageWidth	= image.getWidth();
	imageData.dwImageHeight = image.getHeight();

	image.convertTo24Bits();

	// ... if needed, tint the background image
	if (m_bTintSet) {

		BYTE*	pPixels = image.accessPixels();
		BYTE*	pPixelsEnd =	pPixels + 3*image.getWidth()*image.getHeight();
		BYTE*	pPixelSubel = pPixels;

		while (pPixelSubel < pPixelsEnd) {

			*pPixelSubel = (BYTE) ((unsigned long)(*pPixelSubel * (100 - m_byTintOpacity) + m_byTintB*m_byTintOpacity)/100);
			++pPixelSubel;
			*pPixelSubel = (BYTE) ((unsigned long)(*pPixelSubel * (100 - m_byTintOpacity) + m_byTintG*m_byTintOpacity)/100);
			++pPixelSubel;
			*pPixelSubel = (BYTE) ((unsigned long)(*pPixelSubel * (100 - m_byTintOpacity) + m_byTintR*m_byTintOpacity)/100);
			++pPixelSubel;
		}
	}

	// create the basic image
	HBITMAP		hbmpImage		= NULL;
	HBITMAP		hbmpImageOld	= NULL;

	if (m_dwBackgroundStyle == BACKGROUND_STYLE_RESIZE) {

		if (m_bRelativeBackground) {
			if (m_bExtendBackground) {
				imageData.dwImageWidth	= dwBackgroundWidth;
				imageData.dwImageHeight	= dwBackgroundHeight;
			} else {
				imageData.dwImageWidth	= dwPrimaryDisplayWidth;
				imageData.dwImageHeight	= dwPrimaryDisplayHeight;
			}
		} else {
			imageData.dwImageWidth	= (DWORD)m_nClientWidth;
			imageData.dwImageHeight	= (DWORD)m_nClientHeight;
		}

		if ((image.getWidth() != imageData.dwImageWidth) || (image.getHeight() != imageData.dwImageHeight)) {
			image.rescale(imageData.dwImageWidth, imageData.dwImageHeight, FILTER_LANCZOS3);
		}
	}


	// now, create a DC compatible with the screen and create the basic bitmap
	HDC hdcDesktop		= ::GetDCEx(m_hWnd, NULL, 0);
	imageData.hdcImage	= ::CreateCompatibleDC(hdcDesktop);
	hbmpImage			= ::CreateDIBitmap(
								hdcDesktop,
								image.getInfoHeader(),
								CBM_INIT,
								image.accessPixels(),
								image.getInfo(),
								DIB_RGB_COLORS);
	hbmpImageOld= (HBITMAP)::SelectObject(imageData.hdcImage, hbmpImage);
	::ReleaseDC(m_hWnd, hdcDesktop);

	// create the background image
	m_hdcBackground	= ::CreateCompatibleDC(imageData.hdcImage);
	m_hbmpBackground = ::CreateCompatibleBitmap(imageData.hdcImage, dwBackgroundWidth, dwBackgroundHeight);
	m_hbmpBackgroundOld = (HBITMAP)::SelectObject(m_hdcBackground, m_hbmpBackground);

	RECT rectBackground;
	rectBackground.left		= 0;
	rectBackground.top		= 0;
	rectBackground.right	= dwBackgroundWidth;
	rectBackground.bottom	= dwBackgroundHeight;

	// fill the background with the proper background color in case the
	// bitmap doesn't cover the entire background,
	COLORREF crBackground;

	if (m_dwTransparency == TRANSPARENCY_FAKE) {
		// get desktop background color
		HKEY hkeyColors;
		if (::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Control Panel\\Colors"), 0, KEY_READ, &hkeyColors) == ERROR_SUCCESS) {

			TCHAR	szData[MAX_PATH];
			DWORD	dwDataSize = MAX_PATH;

			BYTE	r = 0;
			BYTE	g = 0;
			BYTE	b = 0;

			::ZeroMemory(szData, sizeof(szData));
			::RegQueryValueEx(hkeyColors, _T("Background"), NULL, NULL, (BYTE*)szData, &dwDataSize);

			_stscanf(szData, _T("%i %i %i"), &r, &g, &b);
			crBackground = RGB(r, g, b);

			::RegCloseKey(hkeyColors);
		}
	} else {
		::CopyMemory(&crBackground, &m_crBackground, sizeof(COLORREF));
	}

	HBRUSH hBkBrush = ::CreateSolidBrush(crBackground);
	::FillRect(m_hdcBackground, &rectBackground, hBkBrush);
	::DeleteObject(hBkBrush);

	if (m_dwBackgroundStyle == BACKGROUND_STYLE_TILE) {

		// we're tiling the image, starting at coordinates (0, 0) of the virtual screen
		DWORD dwX = 0;
		DWORD dwY = 0;

		DWORD dwImageOffsetX = 0;
		DWORD dwImageOffsetY = imageData.dwImageHeight + (m_nBackgroundOffsetY - (int)imageData.dwImageHeight*(m_nBackgroundOffsetY/(int)imageData.dwImageHeight));

		while (dwY < dwBackgroundHeight) {

			dwX				= 0;
			dwImageOffsetX	= imageData.dwImageWidth + (m_nBackgroundOffsetX - (int)imageData.dwImageWidth*(m_nBackgroundOffsetX/(int)imageData.dwImageWidth));

			while (dwX < dwBackgroundWidth) {

				::BitBlt(
					m_hdcBackground,
					dwX,
					dwY,
					imageData.dwImageWidth,
					imageData.dwImageHeight,
					imageData.hdcImage,
					dwImageOffsetX,
					dwImageOffsetY,
					SRCCOPY);

				dwX += imageData.dwImageWidth - dwImageOffsetX;
				dwImageOffsetX = 0;
			}

			dwY += imageData.dwImageHeight - dwImageOffsetY;
			dwImageOffsetY = 0;
		}

	} else if (m_bExtendBackground || !m_bRelativeBackground) {

		switch (m_dwBackgroundStyle) {
			case BACKGROUND_STYLE_RESIZE :
				::BitBlt(
					m_hdcBackground,
					0,
					0,
					dwBackgroundWidth,
					dwBackgroundHeight,
					imageData.hdcImage,
					0,
					0,
					SRCCOPY);
				break;

			case BACKGROUND_STYLE_CENTER :
				::BitBlt(
					m_hdcBackground,
					(dwBackgroundWidth <= imageData.dwImageWidth) ? 0 : (dwBackgroundWidth - imageData.dwImageWidth)/2,
					(dwBackgroundHeight <= imageData.dwImageHeight) ? 0 : (dwBackgroundHeight - imageData.dwImageHeight)/2,
					imageData.dwImageWidth,
					imageData.dwImageHeight,
					imageData.hdcImage,
					(dwBackgroundWidth < imageData.dwImageWidth) ? (imageData.dwImageWidth - dwBackgroundWidth)/2 : 0,
					(dwBackgroundHeight < imageData.dwImageHeight) ? (imageData.dwImageHeight - dwBackgroundHeight)/2 : 0,
					SRCCOPY);
					break;
		}
	} else {
		::EnumDisplayMonitors(NULL, NULL, Console::BackgroundEnumProc, (DWORD)&imageData);
	}

	::SelectObject(imageData.hdcImage, hbmpImageOld);
	::DeleteObject(hbmpImage);
	::DeleteDC(imageData.hdcImage);
}
#endif

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

#if 0
BOOL CALLBACK Console::BackgroundEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {

	IMAGE_DATA* pImageData = (IMAGE_DATA*)dwData;

	DWORD	dwDisplayWidth	= lprcMonitor->right - lprcMonitor->left;
	DWORD	dwDisplayHeight	= lprcMonitor->bottom - lprcMonitor->top;

	DWORD	dwPrimaryDisplayWidth	= ::GetSystemMetrics(SM_CXSCREEN);
	DWORD	dwPrimaryDisplayHeight	= ::GetSystemMetrics(SM_CYSCREEN);

	// center the image according to current display's size and position
	switch (g_pConsole->m_dwBackgroundStyle) {

		case BACKGROUND_STYLE_RESIZE :
			::BitBlt(
				g_pConsole->m_hdcBackground,
				(dwDisplayWidth <= dwPrimaryDisplayWidth) ? lprcMonitor->left-g_pConsole->m_nBackgroundOffsetX : lprcMonitor->left-g_pConsole->m_nBackgroundOffsetX + (dwDisplayWidth - dwPrimaryDisplayWidth)/2,
				(dwDisplayHeight <= dwPrimaryDisplayHeight) ? lprcMonitor->top-g_pConsole->m_nBackgroundOffsetY : lprcMonitor->top-g_pConsole->m_nBackgroundOffsetY + (dwDisplayHeight - dwPrimaryDisplayHeight)/2,
				dwDisplayWidth,
				dwDisplayHeight,
				pImageData->hdcImage,
				(dwDisplayWidth < dwPrimaryDisplayWidth) ? (dwPrimaryDisplayWidth - dwDisplayWidth)/2 : 0,
				(dwDisplayHeight < dwPrimaryDisplayHeight) ? (dwPrimaryDisplayHeight - dwDisplayHeight)/2 : 0,
				SRCCOPY);

			break;

		case BACKGROUND_STYLE_CENTER :
			::BitBlt(
				g_pConsole->m_hdcBackground,
				(dwDisplayWidth <= pImageData->dwImageWidth) ? lprcMonitor->left-g_pConsole->m_nBackgroundOffsetX : lprcMonitor->left-g_pConsole->m_nBackgroundOffsetX + (dwDisplayWidth - pImageData->dwImageWidth)/2,
				(dwDisplayHeight <= pImageData->dwImageHeight) ? lprcMonitor->top-g_pConsole->m_nBackgroundOffsetY : lprcMonitor->top-g_pConsole->m_nBackgroundOffsetY + (dwDisplayHeight - pImageData->dwImageHeight)/2,
				dwDisplayWidth,
				dwDisplayHeight,
				pImageData->hdcImage,
				(dwDisplayWidth < pImageData->dwImageWidth) ? (pImageData->dwImageWidth - dwDisplayWidth)/2 : 0,
				(dwDisplayHeight < pImageData->dwImageHeight) ? (pImageData->dwImageHeight - dwDisplayHeight)/2 : 0,
				SRCCOPY);

			break;
	}

	return TRUE;
}
#endif

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::GetTextSize() {

	TEXTMETRIC	textMetric;
	::GetTextMetrics(m_hdcConsole, &textMetric);

	m_nCharHeight = textMetric.tmHeight;

	if (!(textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH)) {
		// fixed pitch font (TMPF_FIXED_PITCH is cleared!!!)
		m_nCharWidth = textMetric.tmAveCharWidth;
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::GetBordersDimensions() {

	WINDOWINFO wndInfo;
	TITLEBARINFO titlebarInfo;

	::ZeroMemory(&wndInfo, sizeof(WINDOWINFO));
	::ZeroMemory(&titlebarInfo, sizeof(TITLEBARINFO));

	wndInfo.cbSize		= sizeof(WINDOWINFO);
	titlebarInfo.cbSize = sizeof(TITLEBARINFO);

	::GetWindowInfo(m_hWnd, &wndInfo);
	::GetTitleBarInfo(m_hWnd, &titlebarInfo);

	m_nXBorderSize = wndInfo.cxWindowBorders;
	m_nYBorderSize = wndInfo.cyWindowBorders;
	m_nCaptionSize = (titlebarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE) ? 0 : titlebarInfo.rcTitleBar.bottom - titlebarInfo.rcTitleBar.top;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::CalcWindowSize() {

	m_nClientHeight	= m_dwRows * m_nCharHeight + 2*m_nInsideBorder;

	if (m_nCharWidth > 0) {
		// fixed pitch font
		m_nClientWidth	= m_dwColumns * m_nCharWidth + 2*m_nInsideBorder;
	} else {
		// variable pitch font
		int nCharWidth;
		::GetCharWidth32(m_hdcConsole, _TCHAR('m'), _TCHAR('m'), &nCharWidth);
		m_nClientWidth	= m_dwColumns * nCharWidth;
	}

	switch (m_dwWindowBorder) {
		case BORDER_NONE :
			m_nWindowHeight	= m_nClientHeight;
			m_nWindowWidth	= m_nClientWidth;
			break;

		case BORDER_REGULAR : {

			m_nWindowHeight	= m_nClientHeight + m_nCaptionSize + 2*m_nYBorderSize;
			m_nWindowWidth	= m_nClientWidth + 2 * m_nXBorderSize;

			break;
							}

		case BORDER_THIN :

			m_nWindowHeight	= m_nClientHeight + 2*m_nYBorderSize;
			m_nWindowWidth	= m_nClientWidth + 2*m_nXBorderSize;
	}

	if (m_bShowScrollbar) m_nWindowWidth += m_nScrollbarWidth;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

#if 0
void Console::SetWindowTransparency() {
	// set alpha transparency (Win2000 and later only!)
	if (g_bWin2000 && ((m_dwTransparency == TRANSPARENCY_ALPHA) || (m_dwTransparency == TRANSPARENCY_COLORKEY))) {

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		g_pfnSetLayeredWndAttr(m_hWnd, m_crBackground, m_byAlpha, m_dwTransparency == TRANSPARENCY_ALPHA ? LWA_ALPHA : LWA_COLORKEY);

	} else if (m_dwTransparency == TRANSPARENCY_FAKE) {
		// get wallpaper settings
		HKEY hkeyDesktop;
		if (::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Control Panel\\Desktop"), 0, KEY_READ, &hkeyDesktop) == ERROR_SUCCESS) {
			TCHAR	szData[MAX_PATH];
			DWORD	dwDataSize = MAX_PATH;

			DWORD	dwWallpaperStyle;
			DWORD	dwTileWallpaper;

			::ZeroMemory(szData, sizeof(szData));
			::RegQueryValueEx(hkeyDesktop, _T("Wallpaper"), NULL, NULL, (BYTE*)szData, &dwDataSize);

			if (_tcslen(szData) > 0) {
				m_bBitmapBackground = TRUE;
				m_strBackgroundFile		= szData;
				m_bRelativeBackground	= TRUE;
				m_bExtendBackground		= FALSE;

				// get wallpaper style and tile flag
				dwDataSize = MAX_PATH;
				::ZeroMemory(szData, sizeof(szData));
				::RegQueryValueEx(hkeyDesktop, _T("WallpaperStyle"), NULL, NULL, (BYTE*)szData, &dwDataSize);

				dwWallpaperStyle = _ttoi(szData);

				dwDataSize = MAX_PATH;
				::ZeroMemory(szData, sizeof(szData));
				::RegQueryValueEx(hkeyDesktop, _T("TileWallpaper"), NULL, NULL, (BYTE*)szData, &dwDataSize);

				dwTileWallpaper = _ttoi(szData);

				if (dwTileWallpaper == 1) {
					m_dwBackgroundStyle = BACKGROUND_STYLE_TILE;
				} else {

					if (dwWallpaperStyle == 0) {
						m_dwBackgroundStyle = BACKGROUND_STYLE_CENTER;
					} else {
						m_dwBackgroundStyle = BACKGROUND_STYLE_RESIZE;
					}
				}
			}

			::RegCloseKey(hkeyDesktop);
		}
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::SetScrollbarStuff() {

	if (m_bUseTextBuffer) {
		m_bShowScrollbar = m_dwBufferRows > m_dwRows;
	} else {
		m_bShowScrollbar = FALSE;
	}

	// we don't call InitializeFlatSB due to some problems on Win2k, Windowblinds and new-style scrollbars
	if (m_nScrollbarStyle != FSB_REGULAR_MODE) ::InitializeFlatSB(m_hWnd);
	::FlatSB_ShowScrollBar(m_hWnd, SB_VERT, m_bShowScrollbar);
	::FlatSB_SetScrollRange(m_hWnd, SB_VERT, 0, m_dwBufferRows-m_dwRows, FALSE);

	// set scrollbar properties
	::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_VSTYLE, m_nScrollbarStyle, FALSE);
	::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_VBKGCOLOR, m_crScrollbarColor, FALSE);
	::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_CXVSCROLL , m_nScrollbarWidth, FALSE);
	::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_CYVSCROLL, m_nScrollbarButtonHeight, FALSE);
	::FlatSB_SetScrollProp(m_hWnd, WSB_PROP_CYVTHUMB, m_nScrollbarThunmbHeight, TRUE);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::SetDefaultConsoleColors() {

	m_arrConsoleColors[0]	= 0x000000;
	m_arrConsoleColors[1]	= 0x800000;
	m_arrConsoleColors[2]	= 0x008000;
	m_arrConsoleColors[3]	= 0x808000;
	m_arrConsoleColors[4]	= 0x000080;
	m_arrConsoleColors[5]	= 0x800080;
	m_arrConsoleColors[6]	= 0x008080;
	m_arrConsoleColors[7]	= 0xC0C0C0;
	m_arrConsoleColors[8]	= 0x808080;
	m_arrConsoleColors[9]	= 0xFF0000;
	m_arrConsoleColors[10]	= 0x00FF00;
	m_arrConsoleColors[11]	= 0xFFFF00;
	m_arrConsoleColors[12]	= 0x0000FF;
	m_arrConsoleColors[13]	= 0xFF00FF;
	m_arrConsoleColors[14]	= 0x00FFFF;
	m_arrConsoleColors[15]	= 0xFFFFFF;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::SetWindowSizeAndPosition() {

	// set window position
#if 0
	DWORD	dwScreenWidth	= ::GetSystemMetrics(g_bWin2000 ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
	DWORD	dwScreenHeight	= ::GetSystemMetrics(g_bWin2000 ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);
	DWORD	dwTop			= ::GetSystemMetrics(g_bWin2000 ? SM_YVIRTUALSCREEN : 0);
	DWORD	dwLeft			= ::GetSystemMetrics(g_bWin2000 ? SM_XVIRTUALSCREEN : 0);
#else
	DWORD	dwScreenWidth	= ::GetSystemMetrics(SM_CXSCREEN);
	DWORD	dwScreenHeight	= ::GetSystemMetrics(SM_CYSCREEN);
	DWORD	dwTop			= ::GetSystemMetrics(0);
	DWORD	dwLeft			= ::GetSystemMetrics(0);
#endif

	switch (m_dwDocked) {
	case DOCK_TOP_LEFT:
		// top left
		m_nX = dwLeft;
		m_nY = dwTop;
		break;

	case DOCK_TOP_RIGHT:
		// top right
		m_nX = dwScreenWidth - m_nWindowWidth;
		m_nY = dwTop;
		break;

	case DOCK_BOTTOM_RIGHT:
		// bottom right
		m_nX = dwScreenWidth - m_nWindowWidth;
		m_nY = dwScreenHeight - m_nWindowHeight;
		break;

	case DOCK_BOTTOM_LEFT:
		// bottom left
		m_nX = dwLeft;
		m_nY = dwScreenHeight - m_nWindowHeight;
		break;
	}

	HWND hwndZ = 0;
	switch (m_dwCurrentZOrder) {
	case Z_ORDER_REGULAR	: hwndZ = HWND_NOTOPMOST; break;
	case Z_ORDER_ONTOP		: hwndZ = HWND_TOPMOST; break;
	case Z_ORDER_ONBOTTOM	: hwndZ = HWND_BOTTOM; break;
	default                 : return;
	}

	::SetWindowPos(
		m_hWnd,
		hwndZ,
		m_nX,
		m_nY,
		m_nWindowWidth,
		m_nWindowHeight,
		0);

	UpdateOnTopMenuItem();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::SetWindowIcons() {

	HICON hOldIcon	= NULL;

	if (m_strIconFilename.length() > 0) {

		// custom icon, try loading it...
		if ((m_hSmallIcon = (HICON)::LoadImage(NULL, m_strIconFilename.c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE)) != NULL) {

			// icon file exists, load and set the big icon as well
			m_hBigIcon = (HICON)::LoadImage(NULL, m_strIconFilename.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			hOldIcon = (HICON)::SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)m_hBigIcon);
			if (hOldIcon) ::DestroyIcon(hOldIcon);

		} else {
			m_hSmallIcon = (HICON)::LoadImage(ghInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0);
		}

	} else {
		m_hSmallIcon = (HICON)::LoadImage(ghInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0);
	}

	hOldIcon = (HICON)::SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)m_hSmallIcon);
	if (hOldIcon) ::DestroyIcon(hOldIcon);

	SetTrayIcon(m_dwTaskbarButton == TASKBAR_BUTTON_TRAY ? NIM_ADD : NIM_DELETE);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::SetTrayIcon(DWORD dwMessage) {

	NOTIFYICONDATA tnd;
	tstring	strToolTip(m_strWindowTitle);

	tnd.cbSize				= sizeof(NOTIFYICONDATA);
	tnd.hWnd				= m_hWnd;
	tnd.uID					= TRAY_ICON_ID;
	tnd.uFlags				= NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnd.uCallbackMessage	= WM_TRAY_NOTIFY;
	tnd.hIcon				= m_hSmallIcon;

	if (strToolTip.length() > 63) {
		strToolTip.resize(59);
		strToolTip += _T(" ...");
//		DWORD dw = strToolTip.length();
	}

	_tcscpy(tnd.szTip, strToolTip.c_str());
	return ::Shell_NotifyIcon(dwMessage, &tnd);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::DestroyCursor() {

	if (m_pCursor)
	{
		delete m_pCursor;
		m_pCursor = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::EditConfigFile() {

	// prepare editor parameters
	tstring strParams(m_strConfigEditorParams);

	if (strParams.length() == 0) {
		// no params, just use the config file
		strParams = m_strConfigFile;
	} else {
		tstring::size_type nPos = strParams.find(_T("%f"));

		if (nPos == tstring::npos) {
			// no '%f' in editor params, concatenate config file name
			strParams += _T(" ");
			strParams += m_strConfigFile;
		} else {
			// replace '%f' with config file name
			strParams = strParams.replace(nPos, 2, m_strConfigFile);
		}
	}

	// start editor
	::ShellExecute(NULL, NULL, m_strConfigEditor.c_str(), strParams.c_str(), NULL, SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::ReloadSettings() {

	m_bInitializing = TRUE;
	m_bReloading	= TRUE;

	// suspend console monitor thread
	::SuspendThread(m_hMonitorThread);
	::KillTimer(m_hWnd, TIMER_REPAINT_CHANGE);
	if (m_dwMasterRepaintInt) ::KillTimer(m_hWnd, TIMER_REPAINT_MASTER);

	// hide Windows console
	::ShowWindow(m_hWndConsole, SW_HIDE);

	// destroy icons
	SetTrayIcon(NIM_DELETE);
	if (m_hSmallIcon) ::DestroyIcon(m_hSmallIcon);
	if (m_hBigIcon) ::DestroyIcon(m_hBigIcon);

	// destroy cursor
	DestroyCursor();

	// destory menus
	::DestroyMenu(m_hConfigFilesMenu);
	::DestroyMenu(m_hPopupMenu);

	// uninitialize flat scrollbars
	if (m_nScrollbarStyle != FSB_REGULAR_MODE) ::UninitializeFlatSB(m_hWnd);

	// destory window
	::DestroyWindow(m_hWnd);
	if (m_hwndInvisParent) ::DestroyWindow(m_hwndInvisParent);

	// set default values
	m_strShell				= _T("");
	m_strConfigEditor		= _T("notepad.exe");
	m_strConfigEditorParams	= _T("");
	m_dwReloadNewConfig		= m_dwReloadNewConfigDefault;
	m_hwndInvisParent		= NULL;
	m_dwMasterRepaintInt	= 500;
	m_dwChangeRepaintInt	= 50;
	m_strIconFilename		= _T("");
	m_hSmallIcon			= NULL;
	m_hBigIcon				= NULL;
	m_hPopupMenu			= NULL;
	m_hConfigFilesMenu		= NULL;
	m_bPopupMenuDisabled	= FALSE;
	m_strWindowTitle		= m_strWindowTitleDefault;
	m_strWindowTitleCurrent	= m_strWindowTitleDefault;
	m_strFontName			= _T("Courier New");
	m_dwFontSize			= 10;
	m_bBold					= FALSE;
	m_bItalic				= FALSE;
	m_bUseFontColor			= FALSE;
	m_crFontColor			= RGB(0, 0, 0);
	m_nWindowWidth			= 0;
	m_nWindowHeight			= 0;
	m_nXBorderSize			= 5;
	m_nYBorderSize			= 5;
	m_nCaptionSize			= 0;
	m_nClientWidth			= 0;
	m_nClientHeight			= 0;
	m_nCharHeight			= 0;
	m_nCharWidth			= 0;
	m_dwWindowBorder		= BORDER_REGULAR;
	m_bShowScrollbar		= TRUE;
	m_nScrollbarStyle		= FSB_REGULAR_MODE;
	m_crScrollbarColor		= ::GetSysColor(COLOR_3DHILIGHT);
	m_nScrollbarWidth		= ::GetSystemMetrics(SM_CXVSCROLL);
	m_nScrollbarButtonHeight= ::GetSystemMetrics(SM_CYVSCROLL);
	m_nScrollbarThunmbHeight= ::GetSystemMetrics(SM_CYVTHUMB);
	m_dwTaskbarButton		= TASKBAR_BUTTON_NORMAL;
	m_bMouseDragable		= TRUE;
	m_nSnapDst				= 10;
	m_dwDocked				= DOCK_NONE;
	m_dwOriginalZOrder		= Z_ORDER_REGULAR;
	m_dwCurrentZOrder		= Z_ORDER_REGULAR;
	m_dwTransparency		= TRANSPARENCY_NONE;
	m_byAlpha				= 150;
	m_byInactiveAlpha		= 150;
	m_crBackground			= RGB(0, 0, 0);
	m_crConsoleBackground	= RGB(0, 0, 0);
	m_bTintSet				= FALSE;
	m_byTintOpacity			= 50;
	m_byTintR				= 0;
	m_byTintG				= 0;
	m_byTintB				= 0;
	m_bBitmapBackground		= FALSE;
	m_strBackgroundFile		= _T("");
	m_dwBackgroundStyle		= BACKGROUND_STYLE_RESIZE;
	m_bRelativeBackground	= FALSE;
	m_bExtendBackground		= FALSE;
	m_bHideWindow			= FALSE;
	m_bHideConsole			= TRUE;
	m_dwCursorStyle			= CURSOR_STYLE_CONSOLE;
	m_bStartMinimized		= FALSE;
	m_crCursorColor			= RGB(255, 255, 255);
	m_bCursorVisible		= TRUE;
	m_dwRows				= 50;
	m_dwColumns				= 80;
	m_dwBufferRows			= 500;
	m_bUseTextBuffer		= TRUE;
	m_bCopyOnSelect			= FALSE;

	m_nTextColor			= 15;
	m_nTextBgColor			= 0;

	SetDefaultConsoleColors();

	GetOptions();

	CreateConsoleWindow();

	SetupMenus();

	CreateNewFont();
	CreateNewBrush();

//	SetWindowTransparency();
	SetWindowIcons();

	CreateCursor();

	RefreshStdOut();
	GetTextSize();
	GetBordersDimensions();
	InitConsoleWndSize(m_dwColumns);
	ResizeConsoleWindow();
//	ShowHideConsole();

	m_bInitializing = FALSE;
	m_bReloading	= FALSE;

	RefreshScreenBuffer();
	::CopyMemory(m_pScreenBuffer, m_pScreenBufferNew, sizeof(CHAR_INFO) * m_dwRows * m_dwColumns);
	RepaintWindow();

	if (m_dwMasterRepaintInt) ::SetTimer(m_hWnd, TIMER_REPAINT_MASTER, m_dwMasterRepaintInt, NULL);

	if (m_bStartMinimized) {
		if (m_dwTaskbarButton > TASKBAR_BUTTON_NORMAL) {
			m_bHideWindow = TRUE;
		} else {
			::ShowWindow(m_hWnd, SW_MINIMIZE);
		}
	} else {
		::ShowWindow(m_hWnd, SW_SHOW);
	}

	::SetForegroundWindow(m_hWnd);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::StartShellProcess() {

#if 0
	if (m_strShell.length() == 0) {
		TCHAR	szComspec[MAX_PATH];

		if (::GetEnvironmentVariable(_T("COMSPEC"), szComspec, MAX_PATH) > 0) {
			m_strShell = szComspec;
		} else {
			m_strShell = _T("cmd.exe");
		}
	}

	tstring	strShellCmdLine(m_strShell);
	if (m_strShellCmdLine.length() > 0) {
		strShellCmdLine += _T(" ");
		strShellCmdLine += m_strShellCmdLine;
	}

//	strShellCmdLine = "cmd.exe";

	// create the console window
	TCHAR	szConsoleTitle[MAX_PATH];
	::AllocConsole();
	// we use this to avoid possible problems with multiple console instances running
	_stprintf(szConsoleTitle, _T("%i"), ::GetCurrentThreadId());
	::SetConsoleTitle(szConsoleTitle);
	m_hStdOut	= ::GetStdHandle(STD_OUTPUT_HANDLE);
	while ((m_hWndConsole = ::FindWindow(NULL, szConsoleTitle)) == NULL) ::Sleep(50);
	::SetConsoleTitle(m_strWinConsoleTitle.c_str());
#endif

	// this is a little hack needed to support columns greater than standard 80
	RefreshStdOut();
	InitConsoleWndSize(80);
	ResizeConsoleWindow();

#if 0
	::SetConsoleCtrlHandler(Console::CtrlHandler, TRUE);

	// setup the start up info struct
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	::ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!::CreateProcess(
		NULL,
		(TCHAR*)strShellCmdLine.c_str(),
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si,
		&pi)) {

		return FALSE;
	}

	if (m_dwHideConsoleTimeout > 0) {
		::ShowWindow(m_hWndConsole, SW_MINIMIZE);
		::SetTimer(m_hWnd, TIMER_SHOW_HIDE_CONSOLE, m_dwHideConsoleTimeout, NULL);
	} else {
		ShowHideConsole();
	}

	// close main thread handle
	::CloseHandle(pi.hThread);

	// set handles
	m_hQuitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hConsoleProcess = pi.hProcess;
#endif

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::RefreshStdOut() {
#if 0
	if (m_hStdOutFresh) ::CloseHandle(m_hStdOutFresh);
	m_hStdOutFresh = ::CreateFile(_T("CONOUT$"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::RefreshScreenBuffer() {
	// if we're initializing, do nothing
	if (m_bInitializing) return;

//	::GetConsoleScreenBufferInfo(m_hStdOutFresh, &m_csbiConsole);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask	= SIF_POS;
	si.nPos	= (int)m_csbiConsole.srWindow.Top;
	::FlatSB_SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);

#if 0
	if ((m_csbiConsole.srWindow.Right - m_csbiConsole.srWindow.Left + 1 != m_dwColumns) || (m_csbiConsole.srWindow.Bottom - m_csbiConsole.srWindow.Top + 1 != m_dwRows) || (m_csbiConsole.dwSize.Y != m_dwBufferRows)) {
		m_dwColumns = m_csbiConsole.srWindow.Right - m_csbiConsole.srWindow.Left + 1;
		m_dwRows	= m_csbiConsole.srWindow.Bottom - m_csbiConsole.srWindow.Top + 1;
		ResizeConsoleWindow();
	}

	COORD		coordBufferSize;
	COORD		coordStart;
	SMALL_RECT	srRegion;

	coordStart.X		= 0;
	coordStart.Y		= 0;

	coordBufferSize.X	= m_dwColumns;
	coordBufferSize.Y	= m_dwRows;

	srRegion.Top		= m_csbiConsole.srWindow.Top;
	srRegion.Left		= 0;
	srRegion.Bottom		= m_csbiConsole.srWindow.Top + m_dwRows - 1;
	srRegion.Right		= m_dwColumns - 1;

	DEL_ARR(m_pScreenBufferNew);
	m_pScreenBufferNew = new CHAR_INFO[m_dwRows * m_dwColumns];
	::ZeroMemory( m_pScreenBufferNew, m_dwRows * m_dwColumns * sizeof(CHAR_INFO));
	for (int i=0; i<m_dwRows*m_dwColumns; i++) { m_pScreenBufferNew[i].Char.AsciiChar = '0' + i%10; m_pScreenBufferNew[i].Attributes = 15; }

	::ReadConsoleOutput(m_hStdOutFresh, m_pScreenBufferNew, coordBufferSize, coordStart, &srRegion);

	// set console window title
	TCHAR szWinConsoleTitle[MAX_PATH+1];
	::GetConsoleTitle(szWinConsoleTitle, MAX_PATH);

	tstring strWinConsoleTitle(szWinConsoleTitle);
	tstring strConsoleTitle(_T(""));

	// Here we decide about updating Console window title.
	// There are 2 possibilities:

	if (m_strWinConsoleTitle.compare(0, m_strWinConsoleTitle.length(), strWinConsoleTitle, 0, m_strWinConsoleTitle.length()) == 0) {
		// 1. Windows console title starts with the original title, just see if
		//	windows titles differ, and if they do, update it.
		if ((m_strWindowTitle.length() == 0)	&&
			(strWinConsoleTitle[m_strWinConsoleTitle.length()] == ' ') &&
			(strWinConsoleTitle[m_strWinConsoleTitle.length()+1] == '-')) {

			strConsoleTitle = strWinConsoleTitle.substr(m_strWinConsoleTitle.length()+3);
		} else {
			strConsoleTitle = m_strWindowTitle + strWinConsoleTitle.substr(m_strWinConsoleTitle.length());
		}

	} else {
		// 2. Windows console title is completely changed. To set Console title,
		//	we need to get Windows console title and concatenate it to our
		//	original Console title (if it changed since the last update)
		if (m_strWindowTitle.length() == 0) {
			strConsoleTitle = strWinConsoleTitle;
		} else {
			strConsoleTitle = m_strWindowTitle + tstring(_T(" - ")) + strWinConsoleTitle;
		}
	}

	if (m_strWindowTitleCurrent.compare(strConsoleTitle) != 0) {
		m_strWindowTitleCurrent = strConsoleTitle;
		::SetWindowText(m_hWnd, m_strWindowTitleCurrent.c_str());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::InitConsoleWndSize(DWORD dwColumns) {

	if (m_nTextSelection) ClearSelection();

	// get max console size
//	COORD		coordMaxSize;
//	coordMaxSize = ::GetLargestConsoleWindowSize(m_hStdOutFresh);

	// calc max window size
	RECT rectDesktop;
	GetDesktopRect(rectDesktop);

	DWORD dwMaxColumns	= (rectDesktop.right - rectDesktop.left - 2*m_nInsideBorder - 2*m_nXBorderSize - m_nScrollbarWidth)/m_nCharWidth;
	DWORD dwMaxRows		= (rectDesktop.bottom - rectDesktop.top - 2*m_nInsideBorder - m_nYBorderSize - m_nCaptionSize)/m_nCharHeight;

//	if (m_dwColumns == 0) m_dwColumns = coordMaxSize.X < dwMaxColumns ? coordMaxSize.X : dwMaxColumns;
//	if (m_dwRows == 0) m_dwRows = coordMaxSize.Y < dwMaxRows ? coordMaxSize.Y : dwMaxRows;
	if (m_dwColumns == 0) m_dwColumns = dwMaxColumns;
	if (m_dwRows == 0) m_dwRows = dwMaxRows;
	if (m_dwBufferRows < m_dwRows) m_dwBufferRows = m_dwRows;

	COORD coordConsoleSize;
	coordConsoleSize.X = (SHORT)dwColumns;
	coordConsoleSize.Y = (SHORT)m_dwBufferRows;

	SMALL_RECT	srConsoleRect;
	srConsoleRect.Top	= srConsoleRect.Left =0;
//	srConsoleRect.Right	= dwColumns - 1;
//	srConsoleRect.Bottom= m_dwRows - 1;

#if 0
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	::GetConsoleScreenBufferInfo(m_hStdOutFresh, &csbi);

	if ((DWORD) csbi.dwSize.X * csbi.dwSize.Y > (DWORD) dwColumns * m_dwBufferRows) {
		::SetConsoleWindowInfo(m_hStdOutFresh, TRUE, &srConsoleRect);
		::SetConsoleScreenBufferSize(m_hStdOutFresh, coordConsoleSize);
	} else if (((DWORD)csbi.dwSize.X < dwColumns) || ((DWORD)csbi.dwSize.Y < m_dwBufferRows) || ((DWORD)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1) != m_dwRows)) {
		::SetConsoleScreenBufferSize(m_hStdOutFresh, coordConsoleSize);
		::SetConsoleWindowInfo(m_hStdOutFresh, TRUE, &srConsoleRect);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::ResizeConsoleWindow() {

	m_bInitializing = TRUE;

	if (m_nTextSelection) ClearSelection();

//	CONSOLE_SCREEN_BUFFER_INFO csbi;
//	::GetConsoleScreenBufferInfo(m_hStdOutFresh, &csbi);

	// in case buffer rows increased since last resize, and we're not using text buffer
	if (!m_bUseTextBuffer) m_dwBufferRows = m_dwRows;

	COORD		coordBuffersSize;
	coordBuffersSize.X = (SHORT)m_dwColumns;
	coordBuffersSize.Y = (SHORT)m_dwBufferRows;

	SMALL_RECT	srConsoleRect;
	srConsoleRect.Top	= srConsoleRect.Left =0;
//	srConsoleRect.Right	= m_dwColumns - 1;
//	srConsoleRect.Bottom= m_dwRows - 1;

#if 0
	// order of setting window size and screen buffer size depends on current and desired dimensions
	if ((DWORD) csbi.dwSize.X * csbi.dwSize.Y > (DWORD) m_dwColumns * m_dwBufferRows) {

		if (m_bUseTextBuffer && (csbi.dwSize.Y > m_dwBufferRows)) {
			coordBuffersSize.Y = m_dwBufferRows = csbi.dwSize.Y;
		}

		::SetConsoleWindowInfo(m_hStdOutFresh, TRUE, &srConsoleRect);
		::SetConsoleScreenBufferSize(m_hStdOutFresh, coordBuffersSize);

		//	} else if (((DWORD)csbi.dwSize.X < m_dwColumns) || ((DWORD)csbi.dwSize.Y < m_dwBufferRows) || ((DWORD)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1) != m_dwRows)) {
	} else if ((DWORD) csbi.dwSize.X * csbi.dwSize.Y < (DWORD) m_dwColumns * m_dwBufferRows) {

		if (csbi.dwSize.Y < m_dwBufferRows) {
			m_dwBufferRows = coordBuffersSize.Y = csbi.dwSize.Y;
		}

		::SetConsoleScreenBufferSize(m_hStdOutFresh, coordBuffersSize);
		::SetConsoleWindowInfo(m_hStdOutFresh, TRUE, &srConsoleRect);
	}
#endif

	SetScrollbarStuff();
	CalcWindowSize();
	SetWindowSizeAndPosition();
	CreateOffscreenBuffers();

//	if (m_bBitmapBackground) CreateBackgroundBitmap();
}

void Console::AllocateBuffer()
{
	// resize screen buffer
	delete[] m_pScreenBuffer;
	m_pScreenBuffer = new CHAR_INFO[m_dwRows * m_dwColumns];
	m_pScreenBufferNew = new CHAR_INFO[m_dwBufferRows * m_dwColumns];
	::ZeroMemory( m_pScreenBufferNew, m_dwBufferRows * m_dwColumns * sizeof(CHAR_INFO));

	m_bInitializing = FALSE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::RepaintWindow() {

	RECT rect;
	rect.top	= 0;
	rect.left	= 0;
	rect.bottom	= m_nClientHeight;
	rect.right	= m_nClientWidth;

#if 0
	if (m_bBitmapBackground) {
			if (m_bRelativeBackground) {
			::BitBlt(m_hdcConsole, 0, 0, m_nClientWidth, m_nClientHeight, m_hdcBackground, m_nX+m_nXBorderSize-m_nBackgroundOffsetX, m_nY+m_nCaptionSize+m_nYBorderSize-m_nBackgroundOffsetY, SRCCOPY);
		} else {
			::BitBlt(m_hdcConsole, 0, 0, m_nClientWidth, m_nClientHeight, m_hdcBackground, 0, 0, SRCCOPY);
		}
	} else {
#endif
		::FillRect(m_hdcConsole, &rect, m_hBkBrush);
//	}

	DWORD dwX			= m_nInsideBorder;
	DWORD dwY			= m_nInsideBorder;
	DWORD dwOffset		= m_csbiConsole.srWindow.Top * m_dwColumns;

	WORD attrBG;

	// stuff used for caching
	int			nBkMode		= TRANSPARENT;
	COLORREF	crBkColor	= RGB(0, 0, 0);
	COLORREF	crTxtColor	= RGB(0, 0, 0);

//	int			nNewBkMode		= TRANSPARENT;
//	COLORREF	crNewBkColor	= RGB(0, 0, 0);
//	COLORREF	crNewTxtColor	= RGB(0, 0, 0);

	bool		bTextOut		= false;

	wstring		strText(L"");

	memcpy(m_pScreenBuffer, m_pScreenBufferNew + dwOffset, sizeof(CHAR_INFO)*m_dwRows * m_dwColumns);
	dwOffset = 0;

	if (m_nCharWidth > 0) {
		// fixed pitch font
		for (DWORD i = 0; i < m_dwRows; ++i) {

			dwX = m_nInsideBorder;
			dwY = i*m_nCharHeight + m_nInsideBorder;

			nBkMode			= TRANSPARENT;
			crBkColor		= RGB(0, 0, 0);
			crTxtColor		= RGB(0, 0, 0);

			bTextOut		= false;

			attrBG = m_pScreenBuffer[dwOffset].Attributes >> 4;

			// here we decide how to paint text over the backgound
			if (m_arrConsoleColors[attrBG] == m_crConsoleBackground) {
				::SetBkMode(m_hdcConsole, TRANSPARENT);
				nBkMode		= TRANSPARENT;
			} else {
				::SetBkMode(m_hdcConsole, OPAQUE);
				nBkMode		= OPAQUE;
				::SetBkColor(m_hdcConsole, m_arrConsoleColors[attrBG]);
				crBkColor	= m_arrConsoleColors[attrBG];
			}

			::SetTextColor(m_hdcConsole, m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwOffset].Attributes & 0xF]);
			crTxtColor		= m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwOffset].Attributes & 0xF];

			strText = m_pScreenBuffer[dwOffset].Char.UnicodeChar;
			++dwOffset;

			for (DWORD j = 1; j < m_dwColumns; ++j) {

				attrBG = m_pScreenBuffer[dwOffset].Attributes >> 4;

				if (m_arrConsoleColors[attrBG] == m_crConsoleBackground) {
					if (nBkMode != TRANSPARENT) {
						nBkMode = TRANSPARENT;
						bTextOut = true;
					}
				} else {
					if (nBkMode != OPAQUE) {
						nBkMode = OPAQUE;
						bTextOut = true;
					}
					if (crBkColor != m_arrConsoleColors[attrBG]) {
						crBkColor = m_arrConsoleColors[attrBG];
						bTextOut = true;
					}
				}

				if (crTxtColor != (m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwOffset].Attributes & 0xF])) {
					crTxtColor = m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwOffset].Attributes & 0xF];
					bTextOut = true;
				}

				if (bTextOut) {

					::TextOutW(m_hdcConsole, dwX, dwY, strText.c_str(), strText.length());
					dwX += strText.length() * m_nCharWidth;

					::SetBkMode(m_hdcConsole, nBkMode);
					::SetBkColor(m_hdcConsole, crBkColor);
					::SetTextColor(m_hdcConsole, crTxtColor);

					strText = m_pScreenBuffer[dwOffset].Char.UnicodeChar;

				} else {
					strText += m_pScreenBuffer[dwOffset].Char.UnicodeChar;
				}

				++dwOffset;
			}

			if (strText.length() > 0) {
				::TextOutW(m_hdcConsole, dwX, dwY, strText.c_str(), strText.length());
			}
		}

	} else {

		// variable pitch font
		for (DWORD i = 0; i < m_dwRows; ++i) {

			dwX = m_nInsideBorder;
			dwY = i*m_nCharHeight + m_nInsideBorder;

			for (DWORD j = 0; j < m_dwColumns; ++j) {

				attrBG = m_pScreenBuffer[dwOffset].Attributes >> 4;

				// here we decide how to paint text over the backgound
				if (m_arrConsoleColors[attrBG] == m_crConsoleBackground) {
					::SetBkMode(m_hdcConsole, TRANSPARENT);
				} else {
					::SetBkMode(m_hdcConsole, OPAQUE);
					::SetBkColor(m_hdcConsole, m_arrConsoleColors[attrBG]);
				}

				::SetTextColor(m_hdcConsole, m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwOffset].Attributes & 0xF]);
				::TextOutW(m_hdcConsole, dwX, dwY, &(m_pScreenBuffer[dwOffset].Char.UnicodeChar), 1);
				int nWidth;
				::GetCharWidth32(m_hdcConsole, m_pScreenBuffer[dwOffset].Char.UnicodeChar, m_pScreenBuffer[dwOffset].Char.UnicodeChar, &nWidth);
				dwX += nWidth;
				++dwOffset;
			}
		}
	}

	if (m_pCursor) DrawCursor(TRUE);

	::InvalidateRect(m_hWnd, NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::RepaintWindowChanges() {

	DWORD dwX			= m_nInsideBorder;
	DWORD dwY			= m_nInsideBorder;
	DWORD dwDestOffset	= 0;
	DWORD dwSrcOffset	= m_csbiConsole.srWindow.Top * m_dwColumns;

	WORD attrBG;

	if (m_nCharWidth > 0) {

		// fixed pitch font
		for (DWORD i = 0; i < m_dwRows; ++i) {

			dwX = m_nInsideBorder;
			dwY = i*m_nCharHeight + m_nInsideBorder;

			for (DWORD j = 0; j < m_dwColumns; ++j) {

				if (memcmp(&(m_pScreenBuffer[dwDestOffset]), &(m_pScreenBufferNew[dwSrcOffset]), sizeof(CHAR_INFO))) {

					memcpy(&(m_pScreenBuffer[dwDestOffset]), &(m_pScreenBufferNew[dwSrcOffset]), sizeof(CHAR_INFO));

					RECT rect;
					rect.top	= dwY;
					rect.left	= dwX;
					rect.bottom	= dwY + m_nCharHeight;
					rect.right	= dwX + m_nCharWidth;

#if 0
					if (m_bBitmapBackground) {
						if (m_bRelativeBackground) {
							::BitBlt(m_hdcConsole, dwX, dwY, m_nCharWidth, m_nCharHeight, m_hdcBackground, m_nX+m_nXBorderSize-m_nBackgroundOffsetX+(int)dwX, m_nY+m_nCaptionSize+m_nYBorderSize-m_nBackgroundOffsetY+(int)dwY, SRCCOPY);
						} else {
							::BitBlt(m_hdcConsole, dwX, dwY, m_nCharWidth, m_nCharHeight, m_hdcBackground, dwX, dwY, SRCCOPY);
						}
					} else {
#endif
						::FillRect(m_hdcConsole, &rect, m_hBkBrush);
//					}


					attrBG = m_pScreenBuffer[dwDestOffset].Attributes >> 4;

					// here we decide how to paint text over the backgound
					if (m_arrConsoleColors[attrBG] == m_crConsoleBackground) {
						::SetBkMode(m_hdcConsole, TRANSPARENT);
					} else {
						::SetBkMode(m_hdcConsole, OPAQUE);
						::SetBkColor(m_hdcConsole, m_arrConsoleColors[attrBG]);
					}

					::SetTextColor(m_hdcConsole, m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwDestOffset].Attributes & 0xF]);
					::TextOutW(m_hdcConsole, dwX, dwY, &(m_pScreenBuffer[dwDestOffset].Char.UnicodeChar), 1);

					::InvalidateRect(m_hWnd, &rect, FALSE);
				}

				dwX += m_nCharWidth;
				++dwDestOffset;
				++dwSrcOffset;
			}
		}

	} else {

		// variable pitch font
		if (memcmp(m_pScreenBuffer, m_pScreenBufferNew + dwDestOffset, sizeof(CHAR_INFO)*m_dwRows * m_dwColumns)) {
		memcpy(m_pScreenBuffer, m_pScreenBufferNew + dwDestOffset, sizeof(CHAR_INFO)*m_dwRows * m_dwColumns);

		RECT rect;
		rect.top	= 0;
		rect.left	= 0;
		rect.bottom	= m_nClientHeight;
		rect.right	= m_nClientWidth;

#if 0
		if (m_bBitmapBackground) {
			if (m_bRelativeBackground) {
				::BitBlt(m_hdcConsole, 0, 0, m_nClientWidth, m_nClientHeight, m_hdcBackground, m_nX+m_nXBorderSize-m_nBackgroundOffsetX, m_nY+m_nCaptionSize+m_nYBorderSize-m_nBackgroundOffsetY, SRCCOPY);
			} else {
				::BitBlt(m_hdcConsole, 0, 0, m_nClientWidth, m_nClientHeight, m_hdcBackground, 0, 0, SRCCOPY);
			}
		} else {
#endif
			::FillRect(m_hdcConsole, &rect, m_hBkBrush);
//		}

		for (DWORD i = 0; i < m_dwRows; ++i) {

			dwX = m_nInsideBorder;
			dwY = i*m_nCharHeight + m_nInsideBorder;

			for (DWORD j = 0; j < m_dwColumns; ++j) {

				attrBG = m_pScreenBuffer[dwDestOffset].Attributes >> 4;

				// here we decide how to paint text over the backgound
				if (m_arrConsoleColors[attrBG] == m_crConsoleBackground) {
					::SetBkMode(m_hdcConsole, TRANSPARENT);
				} else {
					::SetBkMode(m_hdcConsole, OPAQUE);
					::SetBkColor(m_hdcConsole, m_arrConsoleColors[attrBG]);
				}

				::SetTextColor(m_hdcConsole, m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwDestOffset].Attributes & 0xF]);
				::TextOutW(m_hdcConsole, dwX, dwY, &(m_pScreenBuffer[dwDestOffset].Char.UnicodeChar), 1);
				int nWidth;
				::GetCharWidth32(m_hdcConsole, m_pScreenBuffer[dwDestOffset].Char.UnicodeChar, m_pScreenBuffer[dwDestOffset].Char.UnicodeChar, &nWidth);
				dwX += nWidth;
				++dwDestOffset;
			}
		}

		::InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}

	if (m_pCursor) DrawCursor();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::DrawCursor(BOOL bOnlyCursor) {

	// first, erase old cursor image if needed
	if (!bOnlyCursor && m_bCursorVisible) {
		RECT rectCursorOld;
		DrawCursorBackground(rectCursorOld);
		::InvalidateRect(m_hWnd, &rectCursorOld, FALSE);
	}

#if 0
	// now, see if the cursor is visible...
	CONSOLE_CURSOR_INFO	cinf;
	::GetConsoleCursorInfo(m_hStdOutFresh, &cinf);

	m_bCursorVisible = cinf.bVisible;
#endif

	// ... and draw it
	if (m_bCursorVisible) {
#if 0
		::GetConsoleScreenBufferInfo(m_hStdOutFresh, &m_csbiCursor);

		if (m_csbiCursor.dwCursorPosition.Y < m_csbiCursor.srWindow.Top || m_csbiCursor.dwCursorPosition.Y > m_csbiCursor.srWindow.Bottom) {
			m_bCursorVisible = FALSE;
			return;
		}

		// set proper cursor offset
		m_csbiCursor.dwCursorPosition.Y -= m_csbiCursor.srWindow.Top;
#endif

		RECT rectCursor;
		if (!bOnlyCursor) {
			DrawCursorBackground(rectCursor);
		} else {
			GetCursorRect(rectCursor);
		}
		m_pCursor->Draw(&rectCursor);
		::InvalidateRect(m_hWnd, &rectCursor, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

inline void Console::GetCursorRect(RECT& rectCursor) {

	::ZeroMemory(&rectCursor, sizeof(RECT));

	if (m_nCharWidth > 0) {
		// fixed pitch

		rectCursor.left = m_csbiCursor.dwCursorPosition.X * m_nCharWidth + m_nInsideBorder;
		rectCursor.top = m_csbiCursor.dwCursorPosition.Y * m_nCharHeight + m_nInsideBorder;
		rectCursor.right = m_csbiCursor.dwCursorPosition.X * m_nCharWidth + m_nCharWidth + m_nInsideBorder;
		rectCursor.bottom = m_csbiCursor.dwCursorPosition.Y * m_nCharHeight + m_nCharHeight + m_nInsideBorder;

	} else {

		// variable pitch, we do a little joggling here :-)
		RECT			rectLine;
		int				nLastCharWidth;
		auto_ptr<wchar_t>	pszLine(new wchar_t[m_csbiCursor.dwCursorPosition.X + 2]);
		::ZeroMemory(pszLine.get(), (m_csbiCursor.dwCursorPosition.X + 2)*sizeof(wchar_t));

		for (short i = 0; i <= m_csbiCursor.dwCursorPosition.X; ++i) pszLine.get()[i] = m_pScreenBuffer[m_csbiCursor.dwCursorPosition.Y * m_dwColumns + i].Char.UnicodeChar;

		rectLine.left	= rectLine.right	= 0;
		rectLine.top	= rectLine.bottom	= m_csbiCursor.dwCursorPosition.Y * m_nCharHeight;

		::DrawTextW(
			m_hdcConsole,
			pszLine.get(),
			-1,
			&rectLine,
			DT_CALCRECT);

		if (!::GetCharWidth32(
			m_hdcConsole,
			pszLine.get()[m_csbiCursor.dwCursorPosition.X],
			pszLine.get()[m_csbiCursor.dwCursorPosition.X],
			&nLastCharWidth)) {

			return;
		}

		rectCursor.left		= rectLine.right - (DWORD)nLastCharWidth + m_nInsideBorder;
		rectCursor.top		= rectLine.top + m_nInsideBorder;
		rectCursor.right	= rectLine.right + m_nInsideBorder;
		rectCursor.bottom	= rectLine.bottom + m_nInsideBorder;
	}

	rectCursor.top -= m_csbiConsole.srWindow.Top * m_nCharHeight;
	rectCursor.bottom -= m_csbiConsole.srWindow.Top * m_nCharHeight;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

inline void Console::DrawCursorBackground(RECT& rectCursor) {

	GetCursorRect(rectCursor);

	if (m_csbiCursor.dwCursorPosition.Y < m_csbiConsole.srWindow.Top ||
		m_csbiCursor.dwCursorPosition.Y >= m_csbiConsole.srWindow.Top + (SHORT) m_dwRows) {
			return;
	}

	DWORD dwOffset = (m_csbiCursor.dwCursorPosition.Y-m_csbiConsole.srWindow.Top)*m_dwColumns + m_csbiCursor.dwCursorPosition.X;
	WORD attrBG = m_pScreenBuffer[dwOffset].Attributes >> 4;

	// here we decide how to paint text over the backgound

	if (m_arrConsoleColors[attrBG] == m_crConsoleBackground) {
		::SetBkMode(m_hdcConsole, TRANSPARENT);
	} else {
		::SetBkMode(m_hdcConsole, OPAQUE);
		::SetBkColor(m_hdcConsole, m_arrConsoleColors[attrBG]);
	}

	::SetTextColor(m_hdcConsole, m_bUseFontColor ? m_crFontColor : m_arrConsoleColors[m_pScreenBuffer[dwOffset].Attributes & 0xF]);

#if 0
	if (m_bBitmapBackground) {
		if (m_bRelativeBackground) {
			::BitBlt(
				m_hdcConsole,
				rectCursor.left,
				rectCursor.top,
				rectCursor.right - rectCursor.left,
				rectCursor.bottom - rectCursor.top,
				m_hdcBackground,
				m_nX+m_nXBorderSize-m_nBackgroundOffsetX + rectCursor.left,
				m_nY+m_nCaptionSize+m_nYBorderSize-m_nBackgroundOffsetY + rectCursor.top,
				SRCCOPY);

		} else {
			::BitBlt(
				m_hdcConsole,
				rectCursor.left,
				rectCursor.top,
				rectCursor.right - rectCursor.left,
				rectCursor.bottom - rectCursor.top,
				m_hdcBackground,
				rectCursor.left,
				rectCursor.top, SRCCOPY);
		}
	} else {
#endif
		::FillRect(m_hdcConsole, &rectCursor, m_hBkBrush);
//	}

	::TextOutW(
		m_hdcConsole,
		rectCursor.left,
		rectCursor.top,
		&(m_pScreenBuffer[dwOffset].Char.UnicodeChar),
		1);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::ClearSelection() {

	m_nTextSelection = TEXT_SELECTION_NONE;
	m_coordSelOrigin.X = 0;
	m_coordSelOrigin.Y = 0;
	::ZeroMemory(&m_rectSelection, sizeof(RECT));

	RepaintWindow();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

DWORD Console::GetChangeRate() {

	DWORD dwCount				= m_dwRows * m_dwColumns;
	DWORD dwChangedPositions	= 0;

	for (DWORD i = 0; i < dwCount; ++i) {
		if (m_pScreenBuffer[i].Char.UnicodeChar != m_pScreenBufferNew[i].Char.UnicodeChar) ++dwChangedPositions;
	}

	return dwChangedPositions*100/dwCount;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::ShowHideWindow() {
	::ShowWindowAsync(m_hWnd, m_bHideWindow ? SW_HIDE : SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::ShowHideConsole() {
	::ShowWindow(m_hWndConsole, m_bHideConsole ? SW_HIDE : SW_SHOWNORMAL);
	if (!m_bHideConsole) ::SetForegroundWindow(m_hWndConsole);
	UpdateHideConsoleMenuItem();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

#if 0
void Console::ShowHideConsoleTimeout() {

	::KillTimer(m_hWnd, TIMER_SHOW_HIDE_CONSOLE);
	ShowHideConsole();
}
#endif

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::ToggleWindowOnTop() {

	if (m_dwCurrentZOrder == m_dwOriginalZOrder) {
		m_dwCurrentZOrder	= Z_ORDER_ONTOP;
	} else {
		m_dwCurrentZOrder = m_dwOriginalZOrder;
	}

	HWND hwndZ = 0;
	switch (m_dwCurrentZOrder) {
	case Z_ORDER_REGULAR	: hwndZ = HWND_NOTOPMOST; break;
	case Z_ORDER_ONTOP		: hwndZ = HWND_TOPMOST; break;
	case Z_ORDER_ONBOTTOM	: hwndZ = HWND_BOTTOM; break;
	default                 : return;
	}

	::SetWindowPos(
		m_hWnd,
		hwndZ,
		0,
		0,
		0,
		0,
		SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);

	UpdateOnTopMenuItem();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::HandleMenuCommand(DWORD dwID) {

	// check if it's one of the main menu commands
	switch (dwID) {

/*
	case ID_SHOW_README_FILE:
		ShowReadmeFile();
		return FALSE;

	case ID_ABOUT:
		About();
		return FALSE;
*/

	case ID_COPY:
		CopyTextToClipboard();
		return FALSE;

	case ID_PASTE:
		PasteClipoardText();
		return FALSE;

#if 0
	case ID_HIDE_CONSOLE:
		m_bHideConsole = !m_bHideConsole;
		ShowHideConsole();
		return FALSE;

	case ID_EDIT_CONFIG_FILE:
		EditConfigFile();
		return FALSE;

	case ID_RELOAD_SETTINGS:
		ReloadSettings();
		return FALSE;

	case ID_TOGGLE_ONTOP:
		ToggleWindowOnTop();
		return FALSE;
#endif

	case ID_EXIT_CONSOLE:
		::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		return FALSE;

	case SC_MINIMIZE:
		if (m_dwTaskbarButton > TASKBAR_BUTTON_NORMAL) {
			m_bHideWindow = !m_bHideWindow;
			ShowHideWindow();
			::SetForegroundWindow(m_hWnd);
			return FALSE;
		}
		return TRUE;
	}

#if 0
	// check if it's one of config file submenu items
	if ((dwID >= ID_FIRST_XML_FILE) &&
		(dwID <= ID_LAST_XML_FILE)) {

		TCHAR	szFilename[MAX_PATH];
		::ZeroMemory(szFilename, sizeof(szFilename));
		::GetMenuString(m_hConfigFilesMenu, dwID, szFilename, MAX_PATH, MF_BYCOMMAND);
		m_strConfigFile = tstring(szFilename);

		if (m_dwReloadNewConfig == RELOAD_NEW_CONFIG_PROMPT) {
			if (::MessageBox(
					m_hWndConsole,
					_T("Load new settings?"),
					_T("New configuration selected"),
					MB_YESNO|MB_ICONQUESTION) == IDYES) {

				ReloadSettings();
			}
		} else if (m_dwReloadNewConfig == RELOAD_NEW_CONFIG_YES) {
			ReloadSettings();
		}

		return FALSE;
	}
#endif

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::UpdateOnTopMenuItem() {

#if 0
	::CheckMenuItem(::GetSubMenu(m_hPopupMenu, 0), ID_TOGGLE_ONTOP, MF_BYCOMMAND | ((m_dwCurrentZOrder == Z_ORDER_ONTOP) ? MF_CHECKED : MF_UNCHECKED));
	::CheckMenuItem(m_hSysMenu, ID_TOGGLE_ONTOP, MF_BYCOMMAND | ((m_dwCurrentZOrder == Z_ORDER_ONTOP) ? MF_CHECKED : MF_UNCHECKED));
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::UpdateHideConsoleMenuItem() {

#if 0
	::CheckMenuItem(::GetSubMenu(m_hPopupMenu, 0), ID_HIDE_CONSOLE, MF_BYCOMMAND | (m_bHideConsole ? MF_CHECKED : MF_UNCHECKED));
	::CheckMenuItem(m_hSysMenu, ID_HIDE_CONSOLE, MF_BYCOMMAND | (m_bHideConsole ? MF_CHECKED : MF_UNCHECKED));
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::UpdateConfigFilesSubmenu() {

#if 0
	// populate m_hConfigFilesMenu

	// first, delete old items
	while (::GetMenuItemCount(m_hConfigFilesMenu) != 0) ::DeleteMenu(m_hConfigFilesMenu, 0, MF_BYPOSITION);

	// then, enumerate the files
	WIN32_FIND_DATA	wfd;
	HANDLE			hWfd = NULL;
	BOOL			bMoreFiles = TRUE;
	DWORD			dwID = ID_FIRST_XML_FILE;

	::ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));

	// create the search mask...
	int		nBackslashPos = m_strConfigFile.rfind(_TCHAR('\\'));
	tstring	strConfigFileDir(m_strConfigFile.substr(0, nBackslashPos+1));
	tstring	strSearchFileMask(strConfigFileDir + tstring(_T("*.xml")));

	// ... and enumearate files
	hWfd = ::FindFirstFile(strSearchFileMask.c_str(), &wfd);
	while ((hWfd != INVALID_HANDLE_VALUE) && bMoreFiles) {

		MENUITEMINFO	mii;
		TCHAR			szFilename[MAX_PATH];

		_sntprintf(szFilename, MAX_PATH, _T("%s"), (strConfigFileDir + tstring(wfd.cFileName)).c_str());

		::ZeroMemory(&mii, sizeof(MENUITEMINFO));
		mii.cbSize		= sizeof(MENUITEMINFO);
		mii.fMask		= MIIM_TYPE | MIIM_ID | MIIM_STATE;
		mii.fType		= MFT_RADIOCHECK | MFT_STRING;
		mii.wID			= dwID++;
		mii.dwTypeData	= szFilename;
		mii.cch			= _tcslen(wfd.cFileName);

		if (_tcsicmp(szFilename, m_strConfigFile.c_str()) == 0) {
			mii.fState	= MFS_CHECKED;
		} else {
			mii.fState	= MFS_UNCHECKED;
		}

		::InsertMenuItem(m_hConfigFilesMenu, dwID-ID_FIRST_XML_FILE, TRUE, &mii);

		bMoreFiles = ::FindNextFile(hWfd, &wfd);
	}

	::FindClose(hWfd);
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::CopyTextToClipboard() {

	int nSelX = (m_rectSelection.left - m_nInsideBorder)/m_nCharWidth;
	int nSelY = (m_rectSelection.top - m_nInsideBorder)/m_nCharHeight;
	int nSelColumns = (m_rectSelection.right-m_rectSelection.left)/m_nCharWidth;
	int nSelRows = (m_rectSelection.bottom-m_rectSelection.top)/m_nCharHeight;

	// nothing selected, just return
	if ((nSelColumns == 0) || (nSelRows == 0)) {
		ClearSelection();
		return;
	}

	if (!::OpenClipboard(m_hWnd)) {
		ClearSelection();
		return;
	}

	::EmptyClipboard();

	HGLOBAL hglbData = ::GlobalAlloc(GMEM_MOVEABLE, (nSelColumns+2)*nSelRows*sizeof(TCHAR));
	if (hglbData == NULL) {
		::CloseClipboard();
		ClearSelection();
		return;
	}

	// lock the handle and copy the text to the buffer.
	wchar_t* pszData = (wchar_t*)::GlobalLock(hglbData);

    if (pszData)
	{
		int i;
		for (i = 0; i < nSelRows; ++i) {
			for (int j = 0; j < nSelColumns; ++j) {
				pszData[i*(nSelColumns+2) + j] = m_pScreenBuffer[(nSelY+i)*m_dwColumns + nSelX + j].Char.UnicodeChar;
			}
			// at the end of each row we put \r\n (except for the last one where we put \0)
			pszData[i*(nSelColumns+2) + nSelColumns] = _TCHAR('\r');
			pszData[i*(nSelColumns+2) + nSelColumns + 1] = _TCHAR('\n');
		}
		pszData[(i-1)*(nSelColumns+2) + nSelColumns] = _TCHAR('\x0');
	}

	::GlobalUnlock(hglbData);

	if (::SetClipboardData(CF_UNICODETEXT, hglbData) == NULL) {
		// we need to global-free data only if copying failed
	}
	::CloseClipboard();
	// !!! No call to GlobalFree here. Next app that uses clipboard will call EmptyClipboard to free the data

	ClearSelection();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::PasteClipoardText() {

	if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) return;


	if (::OpenClipboard(m_hWnd)) {
		HANDLE	hData = ::GetClipboardData(CF_UNICODETEXT);

		SendTextToConsole((wchar_t*)::GlobalLock(hData));

		::GlobalUnlock(hData);
		::CloseClipboard();
	}

}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::ShowReadmeFile() {

	// prepare editor parameters
	tstring strParams(m_strConfigEditorParams);

	if (strParams.length() == 0) {
		// no params, just use the readme file
		strParams = m_strReadmeFile;
	} else {
		tstring::size_type nPos = strParams.find(_T("%f"));

		if (nPos == tstring::npos) {
			// no '%f' in editor params, concatenate readme file name
			strParams += _T(" ");
			strParams += m_strReadmeFile;
		} else {
			// replace '%f' with readme file name
			strParams = strParams.replace(nPos, 2, m_strReadmeFile);
		}
	}

	::ShellExecute(NULL, NULL, m_strConfigEditor.c_str(), strParams.c_str(), NULL, SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// About	- shows an about dialog

void Console::About()
{
	CAboutDlg dlg(m_hWnd);

	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::SendTextToConsole(const wchar_t *pszText)
{

	if (!pszText || (wcslen(pszText) == 0)) return;

	BOOL c_state = FALSE;
	if (m_pCursor)
	{
		c_state = (m_pCursor != NULL && m_pCursor->GetState());
		m_pCursor->SetState(FALSE);
		DrawCursor();
	}

	int idx = m_csbiCursor.dwCursorPosition.X + m_csbiCursor.dwCursorPosition.Y * m_dwColumns;

	RECT rectInval;
	int cursorX;
	int cursorY;
	WORD attr = (WORD)(m_nTextColor + (m_nTextBgColor << 4));

	cursorX = m_csbiCursor.dwCursorPosition.X * m_nCharWidth + m_nInsideBorder;
	cursorY = m_csbiCursor.dwCursorPosition.Y * m_nCharHeight + m_nInsideBorder;
	rectInval.left = cursorX;
	rectInval.top = cursorY;
	rectInval.right = cursorX;
	rectInval.bottom = cursorY + m_nCharHeight;

	for (; *pszText; pszText++) {
		int charWidth;
		::GetCharWidth32( m_hdcConsole, *pszText, *pszText, &charWidth);
		if (cursorX + m_nCharWidth > rectInval.right) {
			rectInval.right = cursorX + m_nCharWidth;
		}
		if (cursorX < rectInval.left) {
			rectInval.left = cursorX;
		}
		if (cursorY + m_nCharHeight > rectInval.bottom) {
			rectInval.bottom = cursorY + m_nCharHeight;
		}
		if (cursorY < rectInval.top) {
			rectInval.top = cursorY;
		}

		if (*pszText == 8) {
			if (m_csbiCursor.dwCursorPosition.X) {
				m_csbiCursor.dwCursorPosition.X--;
				cursorX -= m_nCharWidth;
				idx--;
			} else if (m_csbiCursor.dwCursorPosition.Y) {
				if (m_csbiCursor.dwCursorPosition.Y == m_csbiConsole.srWindow.Top) {
					OnVScroll( SB_LINEUP);
				}
				m_csbiCursor.dwCursorPosition.Y--;
				m_csbiCursor.dwCursorPosition.X = (SHORT)m_dwColumns - 1;
				cursorX = m_nCharWidth * m_csbiCursor.dwCursorPosition.X;
				idx--;
			}
		} else if (*pszText != '\n' &&
			*pszText != '\r') {
			m_pScreenBufferNew[ idx].Attributes = attr;
			m_pScreenBufferNew[ idx].Char.UnicodeChar = *pszText;
			idx++;
			cursorX += m_nCharWidth;
			m_csbiCursor.dwCursorPosition.X++;
		}

		if (m_csbiCursor.dwCursorPosition.X >= (SHORT) m_dwColumns || *pszText == '\r' || *pszText == '\n' ) {
			m_csbiCursor.dwCursorPosition.X = 0;
			cursorX = m_nInsideBorder;
			if (*pszText != '\r') {
				m_csbiCursor.dwCursorPosition.Y++;
				if (m_csbiCursor.dwCursorPosition.Y >= (SHORT)m_dwBufferRows) {
					m_csbiCursor.dwCursorPosition.Y--;
					idx = m_csbiCursor.dwCursorPosition.X + m_csbiCursor.dwCursorPosition.Y * m_dwColumns;
					::CopyMemory( m_pScreenBufferNew, m_pScreenBufferNew + m_dwColumns, idx * sizeof( CHAR_INFO));
					::ZeroMemory( m_pScreenBufferNew + idx, m_dwColumns * sizeof( CHAR_INFO));
				} else if (m_csbiCursor.dwCursorPosition.Y == m_csbiConsole.srWindow.Top + (SHORT)m_dwRows) {
					OnVScroll( SB_LINEDOWN);
				} else {
					cursorY += m_nCharHeight;
				}
			}
			idx = m_csbiCursor.dwCursorPosition.X + m_csbiCursor.dwCursorPosition.Y * m_dwColumns;
		}
	}

	::InvalidateRect( m_hWnd, &rectInval, TRUE);
	::SetTimer(m_hWnd, TIMER_REPAINT_CHANGE, m_dwChangeRepaintInt, NULL);

	if (m_pCursor)
	{
		m_pCursor->SetState(c_state);
	}

#if 0
	HANDLE hStdIn = ::CreateFile(_T("CONIN$"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	DWORD	dwTextLen		= _tcslen(pszText);
	DWORD	dwTextWritten	= 0;

	INPUT_RECORD* pKeyEvents = new INPUT_RECORD[dwTextLen];
	::ZeroMemory(pKeyEvents, sizeof(INPUT_RECORD)*dwTextLen);

	for (DWORD i = 0; i < dwTextLen; ++i) {
		pKeyEvents[i].EventType = KEY_EVENT;
		pKeyEvents[i].Event.KeyEvent.bKeyDown = TRUE;
		pKeyEvents[i].Event.KeyEvent.wRepeatCount = 1;
		pKeyEvents[i].Event.KeyEvent.wVirtualKeyCode = 0;
		pKeyEvents[i].Event.KeyEvent.wVirtualScanCode = 0;
		pKeyEvents[i].Event.KeyEvent.uChar.UnicodeChar = pszText[i];
		pKeyEvents[i].Event.KeyEvent.dwControlKeyState = 0;
	}
	::WriteConsoleInput(hStdIn, pKeyEvents, dwTextLen, &dwTextWritten);

	DEL_ARR(pKeyEvents);
	::CloseHandle(hStdIn);
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

tstring Console::GetFullFilename(const tstring& strFilename) {

	TCHAR			szFilePath[MAX_PATH];
	TCHAR*			pszFilename;

	::ZeroMemory(szFilePath, sizeof(szFilePath));
	::GetFullPathName(strFilename.c_str(), MAX_PATH, szFilePath, &pszFilename);

	return tstring(szFilePath);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::GetDesktopRect(RECT& rectDesktop) {

//	if (g_bWin2000) {
		POINT pt;
		if (::GetCursorPos(&pt)) {
			MONITORINFO monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFO);
			if (GetMonitorInfo(MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST), &monitorInfo)) {
				rectDesktop = monitorInfo.rcWork;
			}

		} else {
			rectDesktop.left	= ::GetSystemMetrics(SM_XVIRTUALSCREEN);
			rectDesktop.top		= ::GetSystemMetrics(SM_YVIRTUALSCREEN);
			rectDesktop.right	= rectDesktop.left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
			rectDesktop.bottom	= rectDesktop.top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
		}

#if 0
} else {
		// we keep this for WinNT compatibility

		rectDesktop.left= 0;
		rectDesktop.top	= 0;
		rectDesktop.right	= ::GetSystemMetrics(SM_CXSCREEN);
		rectDesktop.bottom	= ::GetSystemMetrics(SM_CYSCREEN);

		RECT rectTaskbar = {0, 0, 0, 0};
		HWND hWndTaskbar = ::FindWindow(_T("Shell_TrayWnd"), _T(""));


		if (hWndTaskbar) {

			::GetWindowRect(hWndTaskbar, &rectTaskbar);

			if ((rectTaskbar.top <= rectDesktop.top) && (rectTaskbar.left <= rectDesktop.left) && (rectTaskbar.right >= rectDesktop.right)) {
				// top taskbar
				rectDesktop.top += rectTaskbar.bottom;

			} else if ((rectTaskbar.top > rectDesktop.top) && (rectTaskbar.left <= rectDesktop.left)) {
				// bottom taskbar
				rectDesktop.bottom = rectTaskbar.top;

			} else if ((rectTaskbar.top <= rectDesktop.top) && (rectTaskbar.left > rectDesktop.left)) {
				// right taskbar
				rectDesktop.right = rectTaskbar.left;

			} else {
				// left taskbar
				rectDesktop.left += rectTaskbar.right;
			}
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK Console::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	Console *myself = (Console*) ::GetWindowLongPtr( hwnd, GWLP_USERDATA );

	switch (uMsg) {
		case WM_CREATE:
			return 0;

		case WM_PAINT:
			// paint the window
			myself->OnPaint();
			return 0;

		case WM_ERASEBKGND:
			return -1;

		case WM_SIZE:
			// set the size and position of the window
			return 0;

		case WM_CLOSE:
//			::DestroyWindow(myself->m_hWnd);
			// Probably want to just hide the window if the X is hit.	That way it can continue
			// to update on script output
			ShowWindow( hwnd, FALSE);
			return 0;

		case WM_DESTROY:
			myself->OnDestroy();
			return 0;

		case WM_NCDESTROY:
			myself->OnNcDestroy();
			return 0;

		case WM_LBUTTONDOWN:
			myself->OnLButtonDown((UINT)wParam, MAKEPOINTS(lParam));
			return 0;

		case WM_LBUTTONUP:
			myself->OnLButtonUp((UINT)wParam, MAKEPOINTS(lParam));
			return 0;

		case WM_LBUTTONDBLCLK:
			myself->OnLButtonDblClick((UINT)wParam, MAKEPOINTS(lParam));
			return 0;

		case WM_RBUTTONUP:
			myself->OnRButtonUp((UINT)wParam, MAKEPOINTS(lParam));
			return 0;

		case WM_MBUTTONDOWN:
			myself->OnMButtonDown((UINT)wParam, MAKEPOINTS(lParam));
			return 0;

		case WM_MOUSEMOVE:
			myself->OnMouseMove((UINT)wParam, MAKEPOINTS(lParam));
			return 0;

		case WM_SETCURSOR:
			myself->OnSetCursor(LOWORD(lParam), HIWORD(lParam));
			::DefWindowProc(myself->m_hWnd, uMsg, wParam, lParam);
			return 0;

		case WM_INITMENUPOPUP:
			myself->OnInitMenuPopup((HMENU)wParam, LOWORD(lParam), (BOOL)HIWORD(lParam));
			return 0;

		case WM_DROPFILES:
			myself->OnDropFiles((HDROP)wParam);
			return 0;

		case WM_WINDOWPOSCHANGING:
			myself->OnWindowPosChanging((LPWINDOWPOS)lParam);
			return 0;

		case WM_ACTIVATEAPP:
			myself->OnActivateApp((BOOL)wParam, (DWORD)lParam);
			return 0;

#if 0
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			MSG	msg;

			::ZeroMemory(&msg, sizeof(MSG));

			msg.hwnd	= myself->m_hWnd;
			msg.message	= uMsg;
			msg.wParam	= wParam;
			msg.lParam	= lParam;

			::TranslateMessage(&msg);
			::PostMessage(myself->m_hWndConsole, uMsg, wParam, lParam);
			return 0;
#endif

		case WM_CHAR:
			myself->OnChar( (WORD) wParam);
			break;

		case WM_KEYDOWN:
		case WM_KEYUP:
//		case WM_MOUSEWHEEL:
//			::PostMessage(myself->m_hWndConsole, uMsg, wParam, lParam);
			return 0;


		case WM_VSCROLL:
			myself->OnVScroll(wParam);
			return 0;

		case WM_TIMER:
			switch (wParam) {
				case TIMER_REPAINT_CHANGE:
				case TIMER_REPAINT_MASTER:
					myself->OnPaintTimer();
					return 0;

				case CURSOR_TIMER:
					myself->OnCursorTimer();
					return 0;

#if 0
				case TIMER_SHOW_HIDE_CONSOLE:
					myself->ShowHideConsoleTimeout();
					return 0;
#endif

				default:
					return 1;
			}

		case WM_COMMAND:
			if (myself->OnCommand(wParam, lParam)) {
				return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
			} else {
				return 0;
			}

		case WM_SYSCOMMAND:
			if (myself->OnSysCommand(wParam, lParam)) {
				return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
			} else {
				return 0;
			}

		case WM_TRAY_NOTIFY:
			myself->OnTrayNotify(wParam, lParam);
			return 0;

//		case WM_SETTINGCHANGE:
//
//			myself->OnWallpaperChanged((TCHAR*)lParam);
//			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);

		case WM_INPUTLANGCHANGEREQUEST:
			myself->OnInputLangChangeRequest(wParam, lParam);
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);

			// process other messages
		default:
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// MonitorThread	- shell activities monitor

DWORD WINAPI Console::MonitorThreadStatic(LPVOID lpParam) {
	return ((Console*)lpParam)->MonitorThread();
}

DWORD Console::MonitorThread() {

//	HANDLE	arrHandles[] = {m_hConsoleProcess, m_hQuitEvent, m_hStdOut};
//	HANDLE	arrHandles[] = { m_hStdOut};

	for (;;) { // Infinite loop
#if 0
		DWORD dwWait = ::WaitForMultipleObjects(1, arrHandles, FALSE, INFINITE);

		if (dwWait == WAIT_OBJECT_0) {
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			break;
		} else if (dwWait == WAIT_OBJECT_0 + 1) {
			break;
		} else if (dwWait == WAIT_OBJECT_0 + 2) {
#endif
			AddOutput();
			::SetTimer(m_hWnd, TIMER_REPAINT_CHANGE, m_dwChangeRepaintInt, NULL);
			// we sleep here for a while, to prevent 'flooding' of m_hStdOut events
			::Sleep(m_dwChangeRepaintInt);
			::ResetEvent(m_hStdOut);
//		}
	}

//	return 0;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
void Console::AddOutput() {
	DWORD bytesRead;
	char buf[801];
	wchar_t wbuf[801];

	::ReadFile( m_hStdOut, buf, sizeof(buf)/sizeof(wchar_t)-1, &bytesRead, NULL);
	if (bytesRead == 0) {
		return;
	}
	buf[bytesRead] = 0;
	MultiByteToWideChar( CP_UTF8, 0, buf, -1, wbuf, sizeof(wbuf)/sizeof(wbuf[0]));
	SendTextToConsole( wbuf);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL WINAPI Console::CtrlHandler(DWORD dwCtrlType) {
	if ((dwCtrlType == CTRL_C_EVENT) || (dwCtrlType == CTRL_BREAK_EVENT)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////


