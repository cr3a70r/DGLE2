/**
\author		Korotkov Andrey aka DRON
\date		23.04.2012 (c)Sivkov Ilya

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#pragma once

#include "Common.h"

class CDCPFileIterator: public CInstancedObj, public IFileIterator
{
	char _acName[MAX_PATH];
	std::string _strMask; 

	std::vector<std::string> _clNameList;
	std::vector<std::string>::iterator _clNameListIter;

public:
	CDCPFileIterator(uint uiInstIdx, const std::vector<std::string> &clNameList);

	HRESULT DGLE2_API FileName(char *pcName, uint uiCharsCount);
	HRESULT DGLE2_API Next();
	HRESULT DGLE2_API Free();

	IDGLE2_BASE_IMPLEMENTATION(IFileIterator);	
};

struct TDCPFileInfo
{
	uint32	ui32CRC32;
	uint32	ui32Size;
	uint32	ui32CompressedSize;
	uint32	ui32Offset;
	char	cPackedFName[256];
};

class CDCPFileSystem :public CInstancedObj, public IFileSystem
{

	IMainFileSystem *_pFS;
	TDCPFileInfo *_pInfoTable;

	IFile *_pPack;
	std::string _strPackName;	
	uint32 _ui32DataSize, _ui32FilesCount;
	std::vector<std::string> _clFindedFiles;

	bool _ReadFileInfo();
	uint32 _GetTableNumber(const std::string &strName) const;
	bool _OpenPack(const std::string &strPackName);
	void _Clean();
	void _ReplaceChInStr(std::string &outStr, const std::string &findCh, const std::string &repCh);
	void _ConvertFormatFromDirToRegEx(std::string &outStr, const std::string &inStr);

public:

	CDCPFileSystem(uint uiInstIdx, IMainFileSystem *pFS);
	~CDCPFileSystem();

	HRESULT DGLE2_API OpenFile(const char *pcName, E_FILE_SYSTEM_OPEN_FLAGS eFlags, IFile *&prFile);	
	HRESULT DGLE2_API DeleteFile(const char *pcName);
	HRESULT DGLE2_API FileExists(const char *pcName, bool &bExists);
	HRESULT DGLE2_API Find(const char *pcMask, E_FIND_FLAGS eFlags, IFileIterator *&prIterator);
	HRESULT DGLE2_API SendCommand(const char *pcCommand, char *pcResult, uint uiCharsCount);

	IDGLE2_BASE_IMPLEMENTATION(IFileSystem);
};