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
// Cursors.h - cursor classes

#pragma once

#define CURSOR_TIMER		45

extern BOOL g_bWin2000;

/////////////////////////////////////////////////////////////////////////////
// A base class for all the cursors

class Cursor {
	public:
		Cursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor) 
			: m_hwndParent(hwndParent)
			, m_hdcWindow(hdcWindow)
			, m_crCursorColor(crCursorColor)
			, m_bActive(TRUE)
            , m_uiTimer(0)
		{};
		
		virtual ~Cursor(){};

		void SetState(BOOL bActive) {m_bActive = bActive;};
		BOOL GetState() { return m_bActive; }

		// used to draw current frame of the cursor
		virtual void Draw(LPRECT pRect)	= 0;
		// used to prepare the next frame of cursor animation
		virtual void PrepareNext()		= 0;

	protected:

		HWND		m_hwndParent;
		HDC			m_hdcWindow;
		COLORREF	m_crCursorColor;
		
		BOOL		m_bActive;
		UINT		m_uiTimer;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// XTermCursor

class XTermCursor : public Cursor {
	public:
		XTermCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~XTermCursor();

		void Draw(LPRECT pRect);
		void PrepareNext();

	private:
		HBRUSH	m_hActiveBrush;
		HBRUSH	m_hInactiveBrush;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// BlockCursor

class BlockCursor : public Cursor {
	public:
		BlockCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~BlockCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HBRUSH	m_hActiveBrush;
		BOOL	m_bVisible;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// NBBlockCursor

class NBBlockCursor : public Cursor {
	public:
		NBBlockCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~NBBlockCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HBRUSH	m_hActiveBrush;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// PulseBlockCursor

class PulseBlockCursor : public Cursor {
	public:
		PulseBlockCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~PulseBlockCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HBRUSH	m_hActiveBrush;
		RECT	m_rect;
		int		m_nSize;
		int		m_nMaxSize;
		int		m_nStep;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// BarCursor

class BarCursor : public Cursor {
	public:
		BarCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~BarCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HPEN	m_hPen;
		BOOL	m_bVisible;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// ConsoleCursor

class ConsoleCursor : public Cursor {
	public:
		ConsoleCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~ConsoleCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HANDLE	m_hStdOut;
		HBRUSH	m_hActiveBrush;
		BOOL	m_bVisible;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// NBHLineCursor

class NBHLineCursor : public Cursor {
public:
	NBHLineCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
	~NBHLineCursor();
	
	void Draw(LPRECT pRect);
	void PrepareNext();
	
private:
	HPEN	m_hPen;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// HLineCursor

class HLineCursor : public Cursor {
	public:
		HLineCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~HLineCursor();
			
		void Draw(LPRECT pRect);
		void PrepareNext();
			
	private:
		HPEN	m_hPen;
		int		m_nSize;
		int		m_nPosition;
		int		m_nStep;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// VLineCursor

class VLineCursor : public Cursor {
	public:
		VLineCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~VLineCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HPEN	m_hPen;
		int		m_nSize;
		int		m_nPosition;
		int		m_nStep;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// RectCursor

class RectCursor : public Cursor {
	public:
		RectCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~RectCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HBRUSH	m_hActiveBrush;
		BOOL	m_bVisible;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// NBRectCursor

class NBRectCursor : public Cursor {
	public:
		NBRectCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~NBRectCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HBRUSH	m_hActiveBrush;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// PulseRectCursor

class PulseRectCursor : public Cursor {
	public:
		PulseRectCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor);
		~PulseRectCursor();
		
		void Draw(LPRECT pRect);
		void PrepareNext();
		
	private:
		HBRUSH	m_hActiveBrush;
		RECT	m_rect;
		int		m_nSize;
		int		m_nMaxSize;
		int		m_nStep;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// FadeBlockCursor

#define FADE_STEPS			20
#define ALPHA_STEP			12

#define BLEND_BMP_WIDTH		10
#define BLEND_BMP_HEIGHT	20

class FadeBlockCursor : public Cursor {
	public:
		FadeBlockCursor(HWND hwndParent, HDC hdcWindow, COLORREF crCursorColor, COLORREF crBkColor);
		~FadeBlockCursor();

		void Draw(LPRECT pRect);
		void PrepareNext();

	private:

		void FakeBlend();

		COLORREF	m_crCursorColor;
		int			m_nStep;
		
		// used under WinNT 4.0
		COLORREF	m_crBkColor;
		COLORREF	m_arrColors[FADE_STEPS+1];
		int			m_nIndex;

		// these are used under Win2000 only
		HMODULE			m_hUser32;
		BLENDFUNCTION	m_bfn;
		HDC				m_hMemDC;
};

/////////////////////////////////////////////////////////////////////////////
