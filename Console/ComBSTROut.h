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

#ifndef _COMBSTROUT_H_
#define _COMBSTROUT_H_

/////////////////////////////////////////////////////////////////////////////
// CComBSTROut - a class that inherits from	CComBSTR and adds Out() method 
//				that should be used instead of & operator for 'out' 
//				function parameters

class CComBSTROut : public CComBSTR
{
public:

	CComBSTROut()
	{
	}
	/*explicit*/ CComBSTROut(int nSize) : CComBSTR(nSize)
	{
	}
	/*explicit*/ CComBSTROut(int nSize, LPCOLESTR sz) : CComBSTR(nSize, sz)
	{
	}
	/*explicit*/ CComBSTROut(LPCOLESTR pSrc) : CComBSTR(pSrc)
	{
	}
	/*explicit*/ CComBSTROut(const CComBSTROut& src) : CComBSTR(src)
	{
	}
	/*explicit*/ CComBSTROut(REFGUID src) : CComBSTR(src)
	{
	}

#ifndef OLE2ANSI
	CComBSTROut(LPCSTR pSrc) : CComBSTR(pSrc)
	{
	}
	
	CComBSTROut(int nSize, LPCSTR sz) : CComBSTR(nSize, sz)
	{
	}
#endif
	
	~CComBSTROut()
	{
		CComBSTR::~CComBSTR();
	}

	BSTR* Out()
	{
		Empty();
		return &m_str;
	}
};

/////////////////////////////////////////////////////////////////////////////


#endif // _COMBSTROUT_H_
