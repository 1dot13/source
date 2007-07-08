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


/////////////////////////////////////////////////////////////////////////////
// FileStream.h		- implementation of IStream interface on any file 
//					  (not really COM, I'm faking it here a bit :-)
//

#pragma once


/////////////////////////////////////////////////////////////////////////////
// CreateFileStream	- opens/creates a file and returns an IStream interface

HRESULT CreateFileStream(
  LPCTSTR lpFileName,							// file name
  DWORD dwDesiredAccess,						// access mode
  DWORD dwShareMode,							// share mode
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,	// SD
  DWORD dwCreationDisposition,					// how to create
  DWORD dwFlagsAndAttributes,					// file attributes
  HANDLE hTemplateFile,							// handle to template file
  IStream** ppStream							// pointer to IStream interface
);


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// FileStream class

class FileStream : public IStream {
	public:
		FileStream(HANDLE hFile);
		~FileStream();

		// IUnknown methods
		STDMETHOD(QueryInterface)(REFIID riid, void ** ppv) {
			if (riid == IID_IUnknown) {
				*ppv = static_cast<IStream*>(this);
			} else if (riid == IID_IStream) {
				*ppv = static_cast<IStream*>(this);
			} else {
				*ppv = NULL;
				return E_NOINTERFACE;
			}

			static_cast<IUnknown*>(*ppv)->AddRef();
			return S_OK;
		};

		STDMETHOD_(ULONG, AddRef)(void) {
			return ::InterlockedIncrement(&m_lRefCount);
		};

		STDMETHOD_(ULONG, Release)(void) {
			long l = ::InterlockedDecrement(&m_lRefCount);
			if (l == 0) delete this;
			return l; 
		};

		// IStream methods
		STDMETHOD(Read)(void *pv, ULONG cb, ULONG *pcbRead);
		STDMETHOD(Write)(void const *pv, ULONG cb, ULONG *pcbWritten);
		STDMETHOD(Seek)(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);
		STDMETHOD(SetSize)(ULARGE_INTEGER libNewSize);
		STDMETHOD(CopyTo)(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);
		STDMETHOD(Commit)(DWORD grfCommitFlags);
		STDMETHOD(Revert)(void);
		STDMETHOD(LockRegion)(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
		STDMETHOD(UnlockRegion)(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
		STDMETHOD(Stat)(STATSTG *pstatstg, DWORD grfStatFlag);
		STDMETHOD(Clone)(IStream **ppstm);

	private:	// private data

		// reference count
		long	m_lRefCount;

		// file handle
		HANDLE	m_hFile;
};

/////////////////////////////////////////////////////////////////////////////
