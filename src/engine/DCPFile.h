/**
\author		Sivkov Ilya
\date		23.04.2012 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#pragma once

#include "Common.h"

//TODO:
//Should be defined somewhere else besides there.
//The same declaration belongs to DCPFileSystem.h
//Struct needed to Write function
struct TDCPFileInfo
{
	uint32	ui32CRC32;
	uint32	ui32Size;
	uint32	ui32CompressedSize;
	uint32	ui32Offset;
	char	cPackedFName[256];
};


class CDCPFile : public CInstancedObj, public IFile
{
	uint8	*_pBuffer;
	uint32	_ui32Size;
	uint32	_ui32SeekPos;
	char	_acName[MAX_PATH], _acPath[MAX_PATH];
	int		_iFile;
	TDCPFileInfo FileTableHeader;
public:
	CDCPFile(uint uiInstIdx, const char* pcName, E_FILE_SYSTEM_OPEN_FLAGS eFlags, uint8 *pBuffer, uint32 ui32Size);

	HRESULT DGLE2_API Read(void *pBuffer, uint uiCount, uint &uiRead);
	HRESULT DGLE2_API Write(const void *pBuffer, uint uiCount, uint &uiWritten);
	HRESULT DGLE2_API Seek(uint32 ui32Offset, E_FILE_SYSTEM_SEEK_FLAG eWay, uint32 &ui32Position);
	HRESULT DGLE2_API GetSize(uint32 &ui32Size);
	HRESULT DGLE2_API IsOpen(bool &bOpened);
	HRESULT DGLE2_API GetName(char *pcName, uint uiCharsCount);
	HRESULT DGLE2_API GetPath(char *pcPath, uint uiCharsCount);
	HRESULT DGLE2_API Free();

	IDGLE2_BASE_IMPLEMENTATION(IFile);
};