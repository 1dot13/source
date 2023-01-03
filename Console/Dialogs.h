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
// Dialogs.h - dialog classes
//
// About dialog class, etc...

#pragma once

/////////////////////////////////////////////////////////////////////////////
// About dialog class

class CAboutDlg {
	public:
		CAboutDlg(HWND hwndParent);
		~CAboutDlg();

	// public methods
	public:

		// shows About dialog
		BOOL DoModal();

	// public data
	public:
		// parent window handle
		HWND	m_hWndParent;

	// private methods
	private:
		/////////////////////////
		// message handlers

		// dialog window procedure
		static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// private data
	private:
};

/////////////////////////////////////////////////////////////////////////////
