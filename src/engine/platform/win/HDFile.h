/**
\author		Korotkov Andrey aka DRON
\date		22.01.2010 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#pragma once

#include "..\Common.h"

class CInstancedObj;

class CHDFile: public CInstancedObj, public IFile
{
	int		_iFile;
	char	_acName[MAX_PATH],
			_acPath[MAX_PATH];

public:

	CHDFile(uint uiInstIdx, const char* pcName, E_FILE_SYSTEM_OPEN_FLAGS eFlags = FSOF_READ);
	~CHDFile();

	HRESULT DGLE2_API Read(void *pBuffer, uint uiCount, uint &uiRead);
	HRESULT DGLE2_API Write(const void *pBuffer, uint uiCount, uint &uiWritten);
	HRESULT DGLE2_API Seek(uint32 ui32Offset, E_FILE_SYSTEM_SEEK_FLAG eWay, uint32 &ui32Position);
	HRESULT DGLE2_API GetSize(uint32 &ui32Size);
	HRESULT DGLE2_API IsOpen(bool &bOpened);
	HRESULT DGLE2_API GetName(char *pcName, uint uiCharsCount);
	HRESULT DGLE2_API GetPath(char *pcPath, uint uiCharsCount);
	HRESULT DGLE2_API Free();

	IDGLE2_BASE_IMPLEMENTATION(IFile)
};