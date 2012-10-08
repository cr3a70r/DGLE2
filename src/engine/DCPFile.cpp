/**
\author		Sivkov Ilya
\date		23.04.2012 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#include "DCPFile.h"
#include <fcntl.h>
#include <sys\stat.h>

CDCPFile::CDCPFile(uint uiInstIdx, const char* pcName, E_FILE_SYSTEM_OPEN_FLAGS eFlags, uint8 *pBuffer, uint32 ui32Size):
CInstancedObj(uiInstIdx), _ui32SeekPos(0)
{
	int mode = 0;
	
	std::string  file_name = GetFileName(pcName),
			file_path = GetFilePath(pcName);

	if (file_name.size() < MAX_PATH)
		strcpy(_acName, file_name.c_str());
	else
		strcpy(_acName, "\"file_name\" is too long.");

	if (file_path.size() < MAX_PATH)
		strcpy(_acPath, file_path.c_str());
	else
		strcpy(_acPath, "\"file_path\" is too long.");

	if (eFlags & FSOF_WRITE && eFlags & FSOF_READ)
		mode = mode | _O_RDWR;
	else
	{
		if (eFlags & FSOF_WRITE) mode = mode | _O_WRONLY;
		if (eFlags & FSOF_READ) mode = mode | _O_RDONLY;
	}
	
	if (eFlags & FSOF_TRUNC) mode = mode | _O_TRUNC | _O_CREAT;
	if (eFlags & FSOF_BINARY) mode = mode | _O_BINARY;

	int perms = 0;

	if (mode & _O_RDWR)
		perms = _S_IREAD | _S_IWRITE;
	else
		if(mode & _O_WRONLY)
			perms = _S_IWRITE;
		else
			perms = _S_IREAD;

	if ((_iFile = _open(pcName, mode, perms)) == -1)
		LOG(std::string("Can't open file \"") + pcName + "\".", LT_ERROR);

}

HRESULT DGLE2_API CDCPFile::Read(void *pBuffer, uint uiCount, uint &uiRead)
{	
	uiRead = uiCount > _ui32Size - _ui32SeekPos ? _ui32Size - _ui32SeekPos : uiCount;

	memcpy(pBuffer, (void*)&_pBuffer[_ui32SeekPos], uiRead);

	_ui32SeekPos += uiRead;

	return S_OK;
}

HRESULT DGLE2_API CDCPFile::Write(const void *pBuffer, uint uiCount, uint &uiWritten)
{
	_pBuffer = (uint8*)pBuffer;
	
	// create hash of file name
	uint32 hash = GetCRC32(_pBuffer,_ui32Size);

	// write to pack all data
	FileTableHeader.ui32CRC32 = hash;
	strcpy(FileTableHeader.cPackedFName, _acName);
	FileTableHeader.ui32CompressedSize = sizeof(_pBuffer);
	FileTableHeader.ui32Size = sizeof(_pBuffer)+sizeof(FileTableHeader);
	FileTableHeader.ui32Offset = uiCount;
	memcpy(FileTableHeader.cPackedFName,_pBuffer,sizeof(_pBuffer)+1);
	
	uiWritten = _write(_iFile, _pBuffer, uiCount);

	return S_OK;
}

HRESULT DGLE2_API CDCPFile::Seek(uint32 ui32Offset, E_FILE_SYSTEM_SEEK_FLAG eWay, uint32 &ui32Position)
{
	switch(eWay)
	{
	case FSSF_BEGIN: _ui32SeekPos =  ui32Offset; break;
	case FSSF_END: _ui32SeekPos = _ui32Size - ui32Offset; break;
	case FSSF_CURRENT: _ui32SeekPos += ui32Offset; break;
	}

	ui32Position = _ui32SeekPos;

	return S_OK;
}

HRESULT DGLE2_API CDCPFile::GetSize(uint32 &ui32Size)
{
	ui32Size = _ui32Size;
	return S_OK;
}

HRESULT DGLE2_API CDCPFile::IsOpen(bool &bOpened)
{
	if (_pBuffer)
		bOpened = true;
	else
		bOpened = false;

	return S_OK;
}

HRESULT DGLE2_API CDCPFile::GetName(char *pcName, uint uiCharsCount)
{
	if (uiCharsCount < strlen(_acName))
	{
		LOG("Too small \"pcName\" buffer size.", LT_ERROR);
		return E_INVALIDARG;
	}

	strcpy(pcName, _acName);

	return S_OK;
}

HRESULT DGLE2_API CDCPFile::GetPath(char *pcPath, uint uiCharsCount)
{
	if (uiCharsCount < strlen(_acPath))
	{
		LOG("Too small \"pcPath\" buffer size.", LT_ERROR);
		return E_INVALIDARG;
	}

	strcpy(pcPath, _acPath);

	return S_OK;
}

HRESULT DGLE2_API CDCPFile::Free()
{
	delete[] _pBuffer;
	delete this;
	
	return S_OK;
}