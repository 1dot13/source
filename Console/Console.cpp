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


	m_mouseCursorOffset.x = 0;
	m_mouseCursorOffset.y = 0;

	m_coordSelOrigin.X = 0;
	m_coordSelOrigin.Y = 0;

	::ZeroMemory(&m_rectSelection, sizeof(RECT));


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


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

BOOL Console::GetOptions() {

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

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////


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
	DWORD	dwScreenWidth	= ::GetSystemMetrics(SM_CXSCREEN);
	DWORD	dwScreenHeight	= ::GetSystemMetrics(SM_CYSCREEN);
	DWORD	dwTop			= ::GetSystemMetrics(0);
	DWORD	dwLeft			= ::GetSystemMetrics(0);

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


	// this is a little hack needed to support columns greater than standard 80
	RefreshStdOut();
	InitConsoleWndSize(80);
	ResizeConsoleWindow();


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::RefreshStdOut() {
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


	// ... and draw it
	if (m_bCursorVisible) {

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
		std::unique_ptr<wchar_t>	pszLine(new wchar_t[m_csbiCursor.dwCursorPosition.X + 2]);
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


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::UpdateOnTopMenuItem() {

}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::UpdateHideConsoleMenuItem() {

}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

void Console::UpdateConfigFilesSubmenu() {

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


