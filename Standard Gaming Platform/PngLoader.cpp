#include "PngLoader.h"
#include "FileMan.h"
#include "DEBUG.H"
#include "vobject.h"
#include "STIConvert.h"

#include "VFS/vfs.h"
#include "VFS/vfs_file_raii.h"
#include "VFS/Location/vfs_7z_library.h"
#include "VFS/Interface/vfs_location_interface.h"
#include "VFS/File/vfs_memory_file.h"

#include "XML_Parser.h"
namespace png
{
#include <png.h>
}

const utf8string::str_t CONST_DOTPNG(L".png");
const utf8string::str_t CONST_DOTXML(L".xml");

void Load24bppPNGImage(HIMAGE hImage, png::png_bytepp rows, png::png_infop info);
void Load32bppPNGImage(HIMAGE hImage, png::png_bytepp rows, png::png_infop info);
void LoadPalettedPNGImage(HIMAGE hImage, png::png_bytepp rows, png::png_infop info);


void user_read_data(png::png_structp png_ptr, png::png_bytep data, png::png_size_t length)
{
	TRYCATCH_RETHROW( static_cast<vfs::tReadableFile*>(png_ptr->io_ptr)->read((vfs::Byte*)data,length),
		L"error during png file reading");
}

/*******************************************************************************/
/*******************************************************************************/

class IndexedSTIImage
{
public:
	IndexedSTIImage();
	~IndexedSTIImage();
	
	bool setPalette(SGPPaletteEntry *pPal, int iSize);
	bool setPalette(png::png_colorp pPal, int iSize);
	bool addImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y, UINT8 *original_compressed=NULL, UINT32 original_compressed_size=0);

	bool addCompressedImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y);

	bool readAppDataFromXMLFile(HIMAGE hImage, vfs::tReadableFile* pFile);

	bool writeImage(vfs::tWritableFile* pFile);
	bool writeToHIMAGE(HIMAGE pImage);
private:
	STCIHeader							_header;
	STCIPaletteElement*					_palette;
	int									_pal_size;
	std::vector<STCISubImage>			_images;
	std::vector<std::vector<UINT8> >	_compressed_images;
};

IndexedSTIImage::IndexedSTIImage()
: _palette(NULL)
{
	memset(&_header, 0, sizeof(STCIHeader));
}

IndexedSTIImage::~IndexedSTIImage()
{
	if(_palette)
	{
		delete[] _palette;
	}
	for(unsigned int i=0; i < _compressed_images.size(); ++i)
	{
		//delete[] _compressed_images[i];
	}
}

bool IndexedSTIImage::setPalette(SGPPaletteEntry *pPal, int iSize)
{
	if(iSize < 0 ||iSize > 1024)
	{
		return false;
	}
	_pal_size = iSize;
	_palette = new STCIPaletteElement[iSize];
	for(int i=0; i<iSize;++i)
	{
		_palette[i].ubBlue = pPal[i].peBlue;
		_palette[i].ubGreen = pPal[i].peGreen;
		_palette[i].ubRed = pPal[i].peRed;
	}

	return true;
}
bool IndexedSTIImage::setPalette(png::png_colorp pPal, int iSize)
{
	if(iSize < 0 ||iSize > 1024)
	{
		return false;
	}
	_pal_size = iSize;
	_palette = new STCIPaletteElement[iSize];
	for(int i=0; i<iSize;++i)
	{
		_palette[i].ubBlue = pPal[i].blue;
		_palette[i].ubGreen = pPal[i].green;
		_palette[i].ubRed = pPal[i].red;
	}
	return true;
}

bool IndexedSTIImage::addCompressedImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y)
{
	if(!data || (data_size < 0) )
	{
		return false;
	}
	STCISubImage subimage;
	subimage.sOffsetX = image_offset_x;
	subimage.sOffsetY = image_offset_y;
	subimage.usWidth = image_width;
	subimage.usHeight = image_height;

	UINT8 *compressed = new UINT8[data_size];
	memcpy(compressed,data,data_size);

	subimage.uiDataLength = data_size;
	_header.uiStoredSize += data_size;
	_images.push_back(subimage);
//	_compressed_images.push_back(compressed);
	_header.uiOriginalSize += image_width * image_height;
	_header.Indexed.usNumberOfSubImages += 1;

	return true;
}


bool IndexedSTIImage::addImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y, UINT8 *original_compressed, UINT32 original_compressed_size)
{
	if(!data || (data_size != image_width*image_height) )
	{
		return false;
	}
	STCISubImage subimage;
	memset(&subimage,0,sizeof(STCISubImage));

	UINT32 RESIZE = 3*data_size;

	int comprSize = _compressed_images.size();
	_compressed_images.resize(comprSize+1);
	_compressed_images[comprSize].resize(RESIZE);
//	UINT8 *compressed = new UINT8[2*data_size];
	UINT8 *compressed = &_compressed_images[comprSize][0];

	UINT8 *compressed_start = compressed;
//	memset(compressed,0,RESIZE);
	UINT32 compressed_size = 0;

	subimage.usWidth = image_width;
	subimage.usHeight = image_height;

	INT32 etrle_size = ETRLECompressSubImage( compressed, RESIZE, data, image_width, image_height, &subimage);
	THROWIFFALSE( etrle_size > 0, L"not enough space reserved for ETRLE compression");
	subimage.sOffsetX = image_offset_x;
	subimage.sOffsetY = image_offset_y;

	//subimage.uiDataLength = compressed_size;
	//_header.uiStoredSize += compressed_size;
	subimage.uiDataLength = etrle_size;
	_header.uiStoredSize += etrle_size;
	_images.push_back(subimage);
//	_compressed_images.push_back(compressed_start);
	_header.uiOriginalSize += data_size;
	_header.Indexed.usNumberOfSubImages += 1;

	return true;

#if 0
	unsigned int uiBufferPos = 0;
	
	bool bZeroRun = false;
	UINT8 uiRunLength = 0;
	UINT8 *uiRunStartPosition = data;
	if(*data == 0)
	{
		bZeroRun = true;
	}
	bool done = false;
	UINT32 scanline = 0;
	while(!done)
	{
		if(bZeroRun)
		{
			while((*data == 0) && (uiRunLength < 128) && (uiBufferPos < data_size) && (scanline < image_width))
			{
				data++;
				uiRunLength++;
				uiBufferPos++;
				scanline++;
			}
			uiRunStartPosition = compressed;
			*compressed++ = uiRunLength | iCOMPRESS_TRANSPARENT;
			compressed_size += 1;
		}
		else
		{
			uiRunStartPosition = compressed++;
			while((*data != 0) && (uiRunLength < 128) && (uiBufferPos < data_size) && (scanline < image_width))
			{
				*compressed++ = *data++;
				uiRunLength++;
				uiBufferPos++;
				scanline++;
			}
			*uiRunStartPosition = uiRunLength;
			compressed_size += uiRunLength+1;
		}
		// prepare next run
		uiRunLength = 0;
		bZeroRun = (*data != 0) ? false : true;
		if(scanline == image_width)
		{
			scanline = 0;
			// "close" scanline with a zero
			*compressed++ = 0;
			compressed_size += 1;
		}
		if(uiBufferPos >= data_size)
		{
			done = true;
		}
	}
#endif
}




static inline void setFlag(UINT8 &flags, char const* sFlag)
{
	/*0x01*/ if     (strcmp(sFlag, "AUX_FULL_TILE") == 0)			flags |= AUX_FULL_TILE;
	/*0x02*/ else if(strcmp(sFlag, "AUX_ANIMATED_TILE") == 0)		flags |= AUX_ANIMATED_TILE;
	/*0x04*/ else if(strcmp(sFlag, "AUX_DYNAMIC_TILE") == 0)		flags |= AUX_DYNAMIC_TILE;
	/*0x08*/ else if(strcmp(sFlag, "AUX_INTERACTIVE_TILE") == 0)	flags |= AUX_INTERACTIVE_TILE;
	/*0x10*/ else if(strcmp(sFlag, "AUX_IGNORES_HEIGHT") == 0)		flags |= AUX_IGNORES_HEIGHT;
	/*0x20*/ else if(strcmp(sFlag, "AUX_USES_LAND_Z") == 0)			flags |= AUX_USES_LAND_Z;
	else
	{
		std::wstringstream wss;
		wss << L"Unknown flag [" << utf8string::as_utf16(sFlag) << L"]";
		THROWEXCEPTION(wss.str().c_str());
	}
}

class CAppDataParser : public IXMLParser
{
	enum DOM_OBJECT
	{
		DO_ELEMENT_NONE,
		DO_ELEMENT_ImageData,
		DO_ELEMENT_SubImage,
		DO_ELEMENT_offset,
		DO_ELEMENT_AuxData,
		DO_ELEMENT_flags,
		DO_ELEMENT_properties,
	};
public:
	CAppDataParser(XML_Parser &parser, IXMLParser* caller=NULL)
		: IXMLParser("ImageData", parser, caller), 
		current_state(DO_ELEMENT_NONE),
		m_hImage(NULL),
		m_iCurrentIndex(-1)
	{
	}
	virtual ~CAppDataParser() {};

	virtual void onStartElement(const XML_Char* name, const XML_Char** atts);
	virtual void onEndElement(const XML_Char* name);
	virtual void onTextElement(const XML_Char *str, int len);

	void setImage(HIMAGE hImage)
	{
		m_hImage = hImage;
		if(m_hImage)
		{
			m_vAppData.resize(m_hImage->usNumberOfObjects);
		}
	}
private:
	DOM_OBJECT					current_state;
	HIMAGE						m_hImage;
	struct SubImage{
		SubImage()
		{
			memset(&aux,0,sizeof(AuxObjectData));
			offset._override = false;
			offset.x = 0;
			offset.y = 0;
		};
		AuxObjectData aux;
		struct{
			int x,y;
			bool _override;
		} offset;
	};
	std::vector<SubImage>		m_vAppData;
	int							m_iCurrentIndex;			
};

void CAppDataParser::onStartElement(const XML_Char* name, const XML_Char** atts)
{
	if( current_state == DO_ELEMENT_NONE && strcmp(name, this->ElementName) == 0 )
	{
		current_state = DO_ELEMENT_ImageData;
	}
	else if(current_state == DO_ELEMENT_ImageData && strcmp(name, "SubImage") == 0)
	{
		int index = -1;
		THROWIFFALSE(getAttributeAsInt("index",atts,index), L"could not read attribute \"index\"");
		m_iCurrentIndex = index;
		if(index >= (int)m_vAppData.size())
		{
			m_vAppData.resize(index+1);
		}
		current_state = DO_ELEMENT_SubImage; 
	}
	else if(current_state == DO_ELEMENT_SubImage && strcmp(name, "offset") == 0)
	{
		int offset_x = 0, offset_y = 0;
		if(getAttributeAsInt("x",atts,offset_x))
		{
			m_vAppData[m_iCurrentIndex].offset.x = offset_x;
			m_vAppData[m_iCurrentIndex].offset._override = true;
		}
		if(getAttributeAsInt("y",atts,offset_y))
		{
			m_vAppData[m_iCurrentIndex].offset.y = offset_y;
			m_vAppData[m_iCurrentIndex].offset._override = true;
		}
		current_state = DO_ELEMENT_offset; 
	}
	else if(current_state == DO_ELEMENT_SubImage && strcmp(name, "AuxObjectData") == 0)
	{
		current_state = DO_ELEMENT_AuxData; 
	}
	else if(current_state == DO_ELEMENT_AuxData && strcmp(name, "flags") == 0)
	{
		current_state = DO_ELEMENT_flags;
	}
	else if(current_state == DO_ELEMENT_flags)
	{
		setFlag(m_vAppData[m_iCurrentIndex].aux.fFlags, name);
	}
	else if(current_state == DO_ELEMENT_AuxData && 
				(	strcmp(name, "ubCurrentFrame") == 0 || 
					strcmp(name, "ubNumberOfFrames") == 0 ||
					strcmp(name, "ubNumberOfTiles") == 0 ||
					strcmp(name, "ubWallOrientation") == 0 ||
					strcmp(name, "usTileLocIndex") == 0	)
			)
	{
		current_state = DO_ELEMENT_properties;
	}
	sCharData = "";
}
void CAppDataParser::onEndElement(const XML_Char* name)
{
	char *p;
	if( strcmp(name, "usTileLocIndex") == 0 && current_state == DO_ELEMENT_properties)
	{
		m_vAppData[m_iCurrentIndex].aux.usTileLocIndex = (UINT16)strtoul( sCharData.c_str(),&p,10);
		current_state = DO_ELEMENT_AuxData;
	}
	else if( strcmp(name, "ubWallOrientation") == 0 && current_state == DO_ELEMENT_properties)
	{
		m_vAppData[m_iCurrentIndex].aux.ubWallOrientation = (UINT8)strtoul(sCharData.c_str(),&p,10);
		current_state = DO_ELEMENT_AuxData;
	}
	else if( strcmp(name, "ubNumberOfTiles") == 0 && current_state == DO_ELEMENT_properties)
	{
		m_vAppData[m_iCurrentIndex].aux.ubNumberOfTiles = (UINT8)strtoul(sCharData.c_str(),&p,10);
		current_state = DO_ELEMENT_AuxData;
	}
	else if( strcmp(name, "ubNumberOfFrames") == 0 && current_state == DO_ELEMENT_properties)
	{
		m_vAppData[m_iCurrentIndex].aux.ubNumberOfFrames = (UINT)strtoul(sCharData.c_str(),&p,10);
		current_state = DO_ELEMENT_AuxData;
	}
	else if( strcmp(name, "ubCurrentFrame") == 0 && current_state == DO_ELEMENT_properties)
	{
		m_vAppData[m_iCurrentIndex].aux.ubCurrentFrame = (UINT8)strtoul(sCharData.c_str(),&p,10);
		current_state = DO_ELEMENT_AuxData;
	}
	else if( strcmp(name, "flags") == 0 && current_state == DO_ELEMENT_flags)
	{
		current_state = DO_ELEMENT_AuxData;
	}
	else if( strcmp(name, "offset") == 0 && current_state == DO_ELEMENT_offset)
	{
		current_state = DO_ELEMENT_SubImage;
	}
	else if( strcmp(name, "AuxObjectData") == 0 && current_state == DO_ELEMENT_AuxData)
	{
		current_state = DO_ELEMENT_SubImage;
	}
	else if( strcmp(name, "SubImage") == 0 && current_state == DO_ELEMENT_SubImage)
	{
		current_state = DO_ELEMENT_ImageData;
	}
	else if( strcmp(name, this->ElementName) == 0 && current_state == DO_ELEMENT_ImageData)
	{
		unsigned int sum_frames = 0;
		for(unsigned int i = 0; i < m_vAppData.size(); ++i)
		{
			sum_frames += m_vAppData[i].aux.ubNumberOfFrames;
		}
		THROWIFFALSE(sum_frames <= m_vAppData.size(), L"Sum of animation frames doesn't match the total number of frames");
		int iAppDataSize = m_vAppData.size()*sizeof(AuxObjectData);
		m_hImage->pAppData = (UINT8*) MemAlloc( iAppDataSize );
		for(unsigned int i = 0; i < m_vAppData.size(); ++i)
		{
			memcpy(&((AuxObjectData*)m_hImage->pAppData)[i],&m_vAppData[i],sizeof(AuxObjectData));
		}
		m_hImage->uiAppDataSize = iAppDataSize;
		unsigned int aaa = std::max<int>(m_hImage->usNumberOfObjects,m_vAppData.size());
		for(unsigned int i = 0; i < aaa; ++i)
		{
			if(m_vAppData[i].offset._override)
			{
				m_hImage->pETRLEObject[i].sOffsetX = m_vAppData[i].offset.x;
				m_hImage->pETRLEObject[i].sOffsetY = m_vAppData[i].offset.y;
			}
		}
		current_state = DO_ELEMENT_NONE;
	}
}
void CAppDataParser::onTextElement(const XML_Char *str, int len)
{
	// handle only this special case; everything else does not matter for now
	if(current_state == DO_ELEMENT_properties)
	{
		sCharData.append(str,len);
	}

}


bool IndexedSTIImage::readAppDataFromXMLFile(HIMAGE hImage, vfs::tReadableFile* pFile)
{
	if(!pFile)
	{
		return false;
	}
	std::vector<char> vBuffer;
	UINT32 uiSize = 0;
	try
	{
		vfs::COpenReadFile oFile(pFile);
		uiSize = oFile.file().getSize();
		vBuffer.resize(uiSize+1);

		THROWIFFALSE(uiSize == oFile.file().read(&vBuffer[0],uiSize), L"Could not read XML file");
		vBuffer[uiSize] = 0;
		oFile.file().close();
	}
	catch(CBasicException& ex)
	{
		RETHROWEXCEPTION(L"", &ex);
	}

	XML_Parser	parser = XML_ParserCreate(NULL);

	CAppDataParser adp(parser);
	adp.grabParser();
	adp.setImage(hImage);

	try
	{
		if(!XML_Parse(parser, &vBuffer[0], uiSize, TRUE))
		{
			std::wstringstream wss;
			wss << L"XML Parser Error in Groups.xml: " 
				<< utf8string(XML_ErrorString(XML_GetErrorCode(parser))).c_wcs()
				<< L" at line " 
				<< XML_GetCurrentLineNumber(parser);
			THROWEXCEPTION(wss.str().c_str());
		}
	}
	catch(CBasicException &ex)
	{
		RETHROWEXCEPTION(L"Could not load xml file", &ex);
	}
	return true;
}


bool IndexedSTIImage::writeToHIMAGE(HIMAGE pImage)
{
	if(!pImage)
	{
		return false;
	}
	pImage->usNumberOfObjects = _compressed_images.size();
	pImage->usWidth = 640;
	pImage->usHeight = 480;
	pImage->ubBitDepth = 8;
//	pImage->fFlags = STCI_ETRLE_COMPRESSED | STCI_INDEXED | STCI_TRANSPARENT;
	pImage->fFlags |= IMAGE_TRLECOMPRESSED;
	pImage->fFlags |= IMAGE_BITMAPDATA;

	//strncat(pImage->ImageFile,"test",4);
	pImage->iFileLoader = -1;
	pImage->pui16BPPPalette = NULL;


	pImage->pPalette = new SGPPaletteEntry[256];
	for(int j=0; j<256; ++j)
	{
		pImage->pPalette[j].peRed   = _palette[j].ubRed;
		pImage->pPalette[j].peGreen = _palette[j].ubGreen;
		pImage->pPalette[j].peBlue  = _palette[j].ubBlue;
		pImage->pPalette[j].peFlags = 0;
	}

	pImage->pETRLEObject = new ETRLEObject[pImage->usNumberOfObjects];
	UINT32 offset=0; // == size
	for(unsigned int i=0; i<_images.size(); ++i)
	{
		pImage->pETRLEObject[i].sOffsetX = _images[i].sOffsetX;
		pImage->pETRLEObject[i].sOffsetY = _images[i].sOffsetY;
		pImage->pETRLEObject[i].usHeight = _images[i].usHeight;
		pImage->pETRLEObject[i].usWidth  = _images[i].usWidth;
		pImage->pETRLEObject[i].uiDataOffset = offset;
		pImage->pETRLEObject[i].uiDataLength = _images[i].uiDataLength;
		offset += _images[i].uiDataLength;
	}

	pImage->uiSizePixData = offset;
	pImage->pPixData8 = new UINT8[offset];
	// write sub-images
	for(unsigned int i=0; i<_compressed_images.size(); ++i)
	{
		memcpy(
			&pImage->pPixData8[pImage->pETRLEObject[i].uiDataOffset],
			&_compressed_images[i][0],
			pImage->pETRLEObject[i].uiDataLength);
	}

	//pImage->uiAppDataSize = pImage->usNumberOfObjects * sizeof(AuxObjectData);
	//pImage->pAppData = (UINT8*)(new AuxObjectData[pImage->usNumberOfObjects]);
	//memset(pImage->pAppData,0,pImage->uiAppDataSize);

	//// write application data
	//int frames = _compressed_images.size()/8; // 8 = number of directions
	//for(unsigned int i=0; i<pImage->usNumberOfObjects; ++i)
	//{
	//	if(i % frames == 0)
	//	{
	//		AuxObjectData &aod = *((AuxObjectData*)(&pImage->pAppData[i*sizeof(AuxObjectData)]));
	//		aod.ubNumberOfFrames = frames; 
	//		aod.fFlags = AUX_ANIMATED_TILE;
	//	}
	//}
	return true;
}

/*******************************************************************************/
/*******************************************************************************/

class LoadPngFile
{
public:
	LoadPngFile(vfs::Path const& sFile)
		: _file(NULL), _struct(NULL), _info(NULL), _row_ptr(NULL)
	{
		// Open and read in the file
		vfs::COpenReadFile oFile(sFile);
		_file = &oFile.file();
		oFile.release();
	}
	LoadPngFile(vfs::tReadableFile* pFile)
		: _file(pFile), _struct(NULL), _info(NULL), _row_ptr(NULL)
	{
		THROWIFFALSE(_file, L"file pointer is NULL");
	}
	bool Load()
	{
		png::png_error_ptr	user_error_ptr = NULL;
		png::png_error_ptr	user_error_fn = NULL;
		png::png_error_ptr	user_warning_fn = NULL;

		_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png::png_voidp)user_error_ptr, user_error_fn, user_warning_fn);
		if (!_struct)
		{
			return false;
		}

		_info = png_create_info_struct(_struct);
		if (!_info)
		{
			png_destroy_read_struct(&_struct, (png::png_infopp)NULL, (png::png_infopp)NULL);
			return false;
		}

		png::png_infop end_info = png_create_info_struct(_struct);
		if (!end_info)
		{
			png_destroy_read_struct(&_struct, &_info, (png::png_infopp)NULL);
			return false;
		}
		
		// push file handle
		png_set_read_fn(_struct, _file, user_read_data);

		int png_transforms = PNG_TRANSFORM_IDENTITY;
		png_read_png(_struct, _info, png_transforms, NULL);

//		png_read_end(_struct,_info);

		_row_ptr = png_get_rows(_struct, _info);

		return _row_ptr != NULL;
	}
	~LoadPngFile()
	{
		if(_file)
		{
			_file->close();
		}
		if(_struct && _info)
		{
			png_destroy_info_struct(_struct, &_info);
			_info = NULL;
		}
		if(_struct)
		{
			png_destroy_read_struct(&_struct, (png::png_infopp)_info, (png::png_infopp)NULL);
		}
		//if(_row_ptr)
		//{
		//	int i=0;
		//}
	}

	png::png_bytepp Rows()
	{
		return _row_ptr;
	}
	png::png_infop Info()
	{
		return _info;
	}
private:
	vfs::tReadableFile* _file;
	png::png_structp	_struct;
	png::png_infop		_info;
	png::png_bytepp		_row_ptr;
	png::png_bytep		_data;
};

bool LoadPNGFileToImage(HIMAGE hImage, UINT16 fContents)
{
	LoadPngFile fpng(hImage->ImageFile);
	if(!fpng.Load())
	{
		return false;
	}

	if(fpng.Info()->channels == 4 && fpng.Info()->bit_depth == 8)
	{
		Load32bppPNGImage(hImage, fpng.Rows(), fpng.Info());
	}
	else if(fpng.Info()->channels == 3 && fpng.Info()->bit_depth == 8)
	{
		Load24bppPNGImage(hImage, fpng.Rows(), fpng.Info());
	}
	else if(fpng.Info()->channels == 1 && fpng.Info()->bit_depth == 8)
	{
		LoadPalettedPNGImage(hImage, fpng.Rows(), fpng.Info());
	}
	else
	{
		std::wstringstream wss;
		wss << L"Unknown image datatype : channels = " << (int)(fpng.Info()->channels) 
			<< L", bit depth = " << (int)(fpng.Info()->bit_depth);
		THROWEXCEPTION(wss.str().c_str());
	}

	// data was copied to hImage, need to clean up here
	// ...
	return true;
}


bool LoadJPCFileToImage(HIMAGE hImage, UINT16 fContents)
{
	if(!getVFS()->fileExists(hImage->ImageFile))
	{
		return false;
	}
	vfs::CMemoryFile oBuffer("");

	vfs::COpenReadFile oFile(hImage->ImageFile);
	TRYCATCH_RETHROW(oBuffer.copyToBuffer(oFile.file()), L"Could not copy file to buffer");
	oBuffer.close();

	//vfs::CUncompressed7zLibrary oLib(&oFile.file(),"");
	ObjBlockAllocator<vfs::CLibFile> allocator(128);
	vfs::CUncompressed7zLibrary oLib(vfs::tReadableFile::cast(&oBuffer),"",false, &allocator);

	if(!oLib.init())
	{
		return false;
	}

	IndexedSTIImage image;

	std::vector<vfs::IBaseFile*> vFiles;
	int count_files = 0;
	vfs::CUncompressed7zLibrary::Iterator it = oLib.begin();
	for(; !it.end(); it.next())
	{
		count_files++;
	}
	vFiles.resize(count_files);
	it = oLib.begin();
	vfs::tReadableFile* appdata_file = NULL;
	for(; !it.end(); it.next())
	{
		// check extension
		utf8string::str_t const& fname = it.value()->getName().c_wcs();
		utf8string::size_t dot = fname.find_last_of(vfs::Const::DOT());
		if(dot != utf8string::str_t::npos)
		{
			if( StrCmp::Equal(fname.substr(dot,fname.length()-dot), CONST_DOTPNG) )
			{
				std::wstringstream bss;
				bss.str( fname.substr(0,dot) );
				int index;
				bss >> index;
				if(index >= 0 && index < count_files)
				{
					vFiles[index] = it.value();
				}
				else
				{
					THROWEXCEPTION(L"invalid index");
				}
			}
			else if (StrCmp::Equal(fname.substr(dot,fname.length()-dot), CONST_DOTXML) )
			{
				appdata_file = vfs::tReadableFile::cast(it.value());
			}
		}
	}

	bool bHasPalette = false;
	std::vector<vfs::IBaseFile*>::iterator fit = vFiles.begin();
	if(vFiles.size() == 1)
	{
		try
		{
			LoadPngFile lpng( vfs::tReadableFile::cast(vFiles[0]) );

			bool bLoadS = lpng.Load();
	
			if(bLoadS)
			{
				if(lpng.Info()->channels == 4 && lpng.Info()->bit_depth == 8)
				{
					Load32bppPNGImage(hImage, lpng.Rows(), lpng.Info());
				}
				else if(lpng.Info()->channels == 3 && lpng.Info()->bit_depth == 8)
				{
					Load24bppPNGImage(hImage, lpng.Rows(), lpng.Info());
				}
				else if(lpng.Info()->channels == 1 && lpng.Info()->bit_depth == 8)
				{
					LoadPalettedPNGImage(hImage, lpng.Rows(), lpng.Info());
				}
				else
				{
					std::wstringstream wss;
					wss << L"Unknown image datatype : channels = " << (int)(lpng.Info()->channels)
						<< L", bit depth = " << (int)(lpng.Info()->bit_depth);
					THROWEXCEPTION(wss.str().c_str());
				}
				return true;
			}
		}
		catch(CBasicException &ex)
		{
			std::wstringstream wss;
			wss << L"Loading PNG image from file '"
				<< oFile.file().getPath().c_wcs()
				<< L"' failed";
			RETHROWEXCEPTION(wss.str().c_str(),&ex);
		}
		return false;
	}
	else
	{
		for(int findex = 0; fit != vFiles.end(); ++fit, ++findex)
		{
			if(*fit == NULL)
			{
				// probably not a PNG file
				continue;
			}
			try
			{
				vfs::CMemoryFile oTempFile("");
				oTempFile.copyToBuffer( *vfs::tReadableFile::cast(*fit) );

//				LoadPngFile lpng( vfs::tReadableFile::cast(*fit) );
				LoadPngFile lpng( vfs::tReadableFile::cast(&oTempFile) );
	
				bool bLoadS = lpng.Load();
				if(bLoadS)
				{
					if(lpng.Info()->channels == 1 && lpng.Info()->bit_depth == 8)
					{
						if(!bHasPalette)
						{
							THROWIFFALSE(lpng.Info()->num_palette == 256, L"size of palette is not 256");
							image.setPalette(lpng.Info()->palette, lpng.Info()->num_palette);
							bHasPalette = true;
						}
						UINT32 SIZE = lpng.Info()->height * lpng.Info()->width;
						std::vector<UINT8> data(SIZE,0);
						for(unsigned int i = 0; i < lpng.Info()->height; ++i)
						{
							memcpy(&data[i*lpng.Info()->width],lpng.Rows()[i],lpng.Info()->width);
						}
						image.addImage(&data[0], SIZE, lpng.Info()->width, lpng.Info()->height, lpng.Info()->x_offset, lpng.Info()->y_offset);
					}
					else
					{
						std::wstringstream wss;
						wss << L"PNG file '" << (*fit)->getName()() << L" @ " << oFile.file().getPath()() << L"' is not a paletted image";
						THROWEXCEPTION(wss.str().c_str());
					}
				}
			}
			catch(CBasicException& ex)
			{
				std::wstringstream wss;
				wss << L"Loading PNG image [" << findex << L"] from file '"	<< oFile.file().getPath().c_wcs() << L"' failed";
				RETHROWEXCEPTION(wss.str().c_str(),&ex);
			}
		}
	}
	bool success = image.writeToHIMAGE(hImage);
	if(appdata_file)
	{
		success &= image.readAppDataFromXMLFile(hImage, appdata_file);
	}
	return success;
}


void Load32bppPNGImage(HIMAGE hImage, png::png_bytepp rows, png::png_infop info)
{
	hImage->usHeight   = (UINT16)info->height;
	hImage->usWidth    = (UINT16)info->width;
	hImage->ubBitDepth = 32;

#if 1
	UINT32 SIZE = info->height * info->width;
	hImage->p32BPPData = new UINT32[SIZE];
	memset(hImage->p32BPPData, 0, SIZE*sizeof(UINT32));
	UINT32* dest_row = NULL;
	UINT32 rgbcolor = 0;
	for(unsigned int i=0; i<info->height; ++i)
	{
		dest_row = &(hImage->p32BPPData[i*info->width]);
		png::png_bytep row_i = rows[i];
		memcpy(dest_row,row_i,info->width * sizeof(UINT32));
	}
#else
	UINT32 SIZE = info->height * info->width;
	hImage->p16BPPData = new UINT16[SIZE];
	memset(hImage->p16BPPData, 0, SIZE*sizeof(UINT16));
	UINT16* dest_row = NULL;
	UINT32 rgbcolor = 0;
	for(unsigned int i=0; i<info->height; ++i)
	{
		dest_row = &(hImage->p16BPPData[i*info->width]);
		png_bytep row_i = rows[i];
		for(unsigned int sx = 0, dx = 0; sx < 4*info->width; sx+=4, dx+=1)
		{
			if(row_i[sx+3] == 255)
			{
				rgbcolor = FROMRGB(row_i[sx], row_i[sx+1], row_i[sx+2]);
				if(rgbcolor == 0)
				{
					// since we already use rgb(0,0,0) as a fully transparent color,
					// the color black will be mapped to rgb(0,1,0), because green has the most bits
					//rgbcolor = 1 << 8;
					rgbcolor = FROMRGB(0,1,0);
				}
				dest_row[dx] = Get16BPPColor(rgbcolor);
			}
			else
			{
				dest_row[dx] = Get16BPPColor(0);
			}
		}
	}
#endif
}


void Load24bppPNGImage(HIMAGE hImage, png::png_bytepp rows, png::png_infop info)
{
	hImage->usHeight   = (UINT16)info->height;
	hImage->usWidth    = (UINT16)info->width;
	hImage->ubBitDepth = 16;
	
	UINT32 SIZE = info->height * info->width;
	hImage->p16BPPData = new UINT16[SIZE];
	memset(hImage->p16BPPData, 0, SIZE*sizeof(UINT16));

	UINT16* dest_row = NULL;
	UINT32 rgbcolor = 0;
	for(unsigned int i=0; i<info->height; ++i)
	{
		dest_row = &(hImage->p16BPPData[i*info->width]);
		png::png_bytep row_i = rows[i];
		for(unsigned int sx = 0, dx = 0; sx < 3*info->width; sx+=3, dx+=1)
		{
			dest_row[dx] = Get16BPPColor( FROMRGB(row_i[sx], row_i[sx+1], row_i[sx+2]) );
		}
	}
}

void LoadPalettedPNGImage(HIMAGE hImage, png::png_bytepp rows, png::png_infop info)
{
	UINT32 SIZE = info->height * info->width;
	UINT8 *data = new UINT8[SIZE];
	memset(data,0,SIZE);
	for(unsigned int i = 0; i < info->height; ++i)
	{
		memcpy(&data[i*info->width],rows[i],info->width);
	}

	hImage->usNumberOfObjects = 1;
	hImage->pETRLEObject = new ETRLEObject[hImage->usNumberOfObjects];
	memset(hImage->pETRLEObject,0, hImage->usNumberOfObjects * sizeof(ETRLEObject));

	ETRLEObject &subimage = hImage->pETRLEObject[0];
	subimage.sOffsetX = (INT16)  info->x_offset;;
	subimage.sOffsetY = (INT16)  info->y_offset;
	subimage.usWidth  = (UINT16) info->width;
	subimage.usHeight = (UINT16) info->height;

	// why allocate so much? because in in the worst case we need one TAG and one DATAVALUE per pixel
	UINT32 RESIZE = 3*SIZE;
	hImage->p8BPPData = new UINT8[RESIZE];
	UINT8 *compressed = hImage->p8BPPData;
	memset(hImage->p8BPPData,0,RESIZE);
	UINT32 compressed_size = 0;

	STCISubImage subim;
	memset(&subim,0,sizeof(STCISubImage));

	subim.uiDataLength = subimage.uiDataLength;
	subim.uiDataOffset = subimage.uiDataOffset;
	subim.usHeight = subimage.usHeight;
	subim.usWidth = subimage.usWidth;

	UINT32 etrle_size = ETRLECompressSubImage( hImage->p8BPPData, RESIZE, data, (UINT16)info->width, (UINT16)info->height, &subim);
	THROWIFFALSE( etrle_size > 0, L"ETRLE compression of PNG image failed" );
	subim.sOffsetX = subimage.sOffsetX;
	subim.sOffsetY = subimage.sOffsetY;
#if 0
	unsigned int uiBufferPos = 0;
	
	bool bZeroRun = false;
	UINT8 uiRunLength = 0;
	UINT8 *uiRunStartPosition = data;
	if(*data == 0)
	{
		bZeroRun = true;
	}
	bool done = false;
	UINT32 scanline = 0;
	while(!done)
	{
		if(bZeroRun)
		{
			while((*data == 0) && (uiRunLength < 128) && (uiBufferPos < SIZE) && (scanline < info->width))
			{
				data++;
				uiRunLength++;
				uiBufferPos++;
				scanline++;
			}
			uiRunStartPosition = compressed;
			*compressed++ = uiRunLength | COMPRESS_TRANSPARENT;
			compressed_size += 1;
			*compressed++ = 0;
			compressed_size += 1;
		}
		else
		{
			uiRunStartPosition = compressed++;
			while((*data != 0) && (uiRunLength < 128) && (uiBufferPos < SIZE) && (scanline < info->width))
			{
				*compressed++ = *data++;
				uiRunLength++;
				uiBufferPos++;
				scanline++;
			}
			*uiRunStartPosition = uiRunLength | COMPRESS_NON_TRANSPARENT;
			compressed_size += uiRunLength+1;
		}
		// prepare next run
		uiRunLength = 0;
		bZeroRun = (*data != 0) ? false : true;
		if(scanline == info->width)
		{
			scanline = 0;
			// "close" scanline with a zero
			*compressed++ = 0;
			compressed_size += 1;
		}
		if(uiBufferPos >= SIZE)
		{
			done = true;
		}
	}
	UINT32 etrle_size = compressed_size;
#endif
	//subimage.uiDataLength = compressed_size;
	subimage.uiDataLength = etrle_size;
	subimage.uiDataOffset = 0;

	hImage->ubBitDepth = 8;
	//hImage->uiSizePixData = compressed_size;
	hImage->uiSizePixData = etrle_size;

	hImage->usHeight = (UINT16)info->height;
	hImage->usWidth = (UINT16)info->width;

	hImage->fFlags |= IMAGE_TRLECOMPRESSED;
	hImage->fFlags |= IMAGE_BITMAPDATA;
	
	// palette
	THROWIFFALSE(info->num_palette == 256, L"palette has size != 256");

	hImage->pPalette = new SGPPaletteEntry[256];
	for(int i=0; i<256; ++i)
	{
		hImage->pPalette[i].peRed   = info->palette[i].red;
		hImage->pPalette[i].peGreen = info->palette[i].green;
		hImage->pPalette[i].peBlue  = info->palette[i].blue;
		hImage->pPalette[i].peFlags = 0;
	}
	hImage->fFlags |= IMAGE_PALETTE;
}

