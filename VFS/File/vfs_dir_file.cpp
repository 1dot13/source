#include "vfs_dir_file.h"
#include "../Interface/vfs_directory_interface.h"
#include "../iteratedir.h"

vfs::Path vfs::CVFSFile::GetFullPath()
{
	if(_pLoc_)
	{
		return _pLoc_->GetFullPath() + m_sFileName;
	}
	else
	{
		return m_sFileName;
	}
}

bool vfs::CVFSFile::Delete()
{
	Close();
	vfs::Path fname;
	if(_pLoc_)
	{
		fname = static_cast<IDirectory<CVFSFile::write_type>*>(_pLoc_)->GetRealPath() + m_sFileName;
	}
	else
	{
		fname = m_sFileName;
	}
	return os::DeleteRealFile(fname);
}

bool vfs::CVFSFile::OpenRead()
{
//	std::cout << "[CVFSFile::OpenRead] open file <" << m_sFileName() << ">\n" ;
	if( !m_oFile.good() )
	{
		std::cout << "ERROR" << std::endl;
		return false;
	}
	if(m_bIsOpen_read)
	{
		return true;
	}	
	std::ios::openmode Mode;
	vfs::Path sFileName;
	if(_pLoc_)
	{
		sFileName = static_cast<IDirectory<CFile::write_type>*>(_pLoc_)->GetRealPath() + m_sFileName;
	}
	else
	{
		sFileName = m_sFileName;
	}
	// try to open 
	Mode = std::ios::in|std::ios::binary;
#ifdef WIN32
	m_oFile.open(sFileName().c_wcs().c_str(),Mode);
#else
	m_oFile.open(sFileName().utf8().c_str(),Mode);
#endif
	if( !(m_bIsOpen_read = m_oFile.is_open()) )
	{
		m_oFile.clear();
		return false;
	}
	return m_oFile.good();
}

bool vfs::CVFSFile::OpenWrite(bool bCreateWhenNotExist, bool bTruncate)
{
//	std::cout << "[CVFSFile::OpenWrite] opening file <" << m_sFileName << "> .. ";
	if(!m_oFile.good())
	{
//		std::cout << "ERROR" << std::endl;
		return false;
	}
	if(m_bIsOpen_write)
	{
//		std::cout << "already open" << std::endl;
		return m_oFile.good();
	}	
	vfs::Path sFileName;
	if(_pLoc_)
	{
		sFileName = static_cast<IDirectory<CFile::write_type>*>(_pLoc_)->GetRealPath() + m_sFileName;
	}
	else
	{
		sFileName = m_sFileName;
	}
	std::ios::openmode Mode;
	Mode = std::ios::in|std::ios::out|std::ios::binary;
	if(bTruncate)
	{
		Mode |= std::ios::trunc;
	}
#ifdef WIN32
	m_oFile.open(sFileName().c_wcs().c_str(),Mode);
#else
	m_oFile.open(sFileName().utf8().c_str(),Mode);
#endif
	if( !(m_bIsOpen_write = m_oFile.is_open()) )
	{
		m_oFile.clear();
		if(bCreateWhenNotExist)
		{
			// create file 
			Mode = std::ios::out|std::ios::binary;
			if(bTruncate)
			{
				Mode |= std::ios::trunc;
			}
#ifdef WIN32
			m_oFile.open(sFileName().c_wcs().c_str(),Mode);
#else
			m_oFile.open(sFileName().utf8().c_str(),Mode);
#endif
			if( !(m_bIsOpen_write = m_oFile.is_open()) )
			{
				return false;
			}
			m_oFile.close();
			Mode |= std::ios::in;
#ifdef WIN32
			m_oFile.open(sFileName().c_wcs().c_str(),Mode);
#else
			m_oFile.open(sFileName().utf8().c_str(),Mode);
#endif
			if( !(m_bIsOpen_write = m_oFile.is_open()) )
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return m_oFile.good();
}


vfs::Path vfs::CVFSTextFile::GetFullPath()
{
	if(_pLoc_)
	{
		return static_cast<IDirectory<CFile::write_type>*>(_pLoc_)->GetRealPath() + m_sFileName;
	}
	else
	{
		return m_sFileName;
	}
}

bool vfs::CVFSTextFile::OpenRead()
{
	if( !m_oFile.good() )
	{
		return false;
	}
	if(m_bIsOpen_read)
	{
		return true;
	}
	vfs::Path sFileName;
	if(_pLoc_)
	{
		sFileName = static_cast<IDirectory<CFile::write_type>*>(_pLoc_)->GetRealPath() + m_sFileName;
	}
	else
	{
		sFileName = m_sFileName;
	}
	std::ios::openmode Mode;
	Mode = std::ios::in;
#ifdef WIN32
	m_oFile.open(sFileName().c_wcs().c_str(),Mode);
#else
	m_oFile.open(sFileName().utf8().c_str(),Mode);
#endif
	if( !(m_bIsOpen_read = m_oFile.is_open()) )
	{
		m_oFile.clear();
		return false;
	}
	return m_oFile.good();
}
