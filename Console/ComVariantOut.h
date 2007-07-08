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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    marko.bozikovic@alterbox.net
//    bozho@kset.org
/////////////////////////////////////////////////////////////////////////////

#ifndef _COMVARIANTOUT_H_
#define _COMVARIANTOUT_H_

/////////////////////////////////////////////////////////////////////////////
// CComVariantOut - a class that inherits from CComVariant and adds Out() 
//                  method that should be used instead of & operator for 
//                  'out' function parameters

class CComVariantOut : public CComVariant
{

public:
	CComVariantOut()
	{
	}
	~CComVariantOut()
	{
		CComVariant::~CComVariant();
	}

	CComVariantOut(const VARIANT& varSrc) : CComVariant(varSrc)
	{
	}

	CComVariantOut(const CComVariant& varSrc) : CComVariant(varSrc)
	{
	}

	CComVariantOut(BSTR bstrSrc) : CComVariant(bstrSrc)
	{
	}
	CComVariantOut(LPCOLESTR lpszSrc) : CComVariant(lpszSrc)
	{
	}

#ifndef OLE2ANSI
	CComVariantOut(LPCSTR lpszSrc) : CComVariant(lpszSrc)
	{
	}
#endif

	CComVariantOut(bool bSrc) : CComVariant(bSrc)
	{
	}

	CComVariantOut(int nSrc) : CComVariant(nSrc)
	{
	}
	CComVariantOut(BYTE nSrc) : CComVariant(nSrc)
	{
	}
	CComVariantOut(short nSrc) : CComVariant(nSrc)
	{
	}
	CComVariantOut(long nSrc, VARTYPE m_varSrc = VT_I4) : CComVariant(nSrc, m_varSrc)
	{
	}
	CComVariantOut(float fltSrc) : CComVariant(fltSrc)
	{
	}
	CComVariantOut(double dblSrc) : CComVariant(dblSrc)
	{
	}
	CComVariantOut(CY cySrc) : CComVariant(cySrc)
	{
	}
	CComVariantOut(IDispatch* pSrc) : CComVariant(pSrc)
	{
	}
	CComVariantOut(IUnknown* pSrc) : CComVariant(pSrc)
	{
	}

public:
	VARIANT* Out()
	{
		Clear();
		return this;
	}
};

/////////////////////////////////////////////////////////////////////////////


#endif // _COMVARIANTOUT_H_