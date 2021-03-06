/**
\author		Korotkov Andrey aka DRON
\date		31.03.2012 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#pragma once

#include <DGLE2.h>
#include "../FuncDelegate.h"

class CConsole;

namespace DGLE2
{

enum E_WINDOW_ACCESS_TYPE
{
	WAT_FULL_ACCESS = 0,	/**< Indicates that engine owns window. */
	WAT_RESTRICTED_ACCESS,	/**< Indicates that engine is rendering to some window control of some other window. */
	WAT_NO_ACCESS			/**< Indicates that engine is working with some control without handle or that is not hookable. */
};

// {0D3C63FA-D14B-4190-BFB6-92F611506171}
static const GUID IID_IMainWindow = 
{ 0xd3c63fa, 0xd14b, 0x4190, { 0xbf, 0xb6, 0x92, 0xf6, 0x11, 0x50, 0x61, 0x71 } };

class IMainWindow
{
public:
	virtual HRESULT InitWindow(TWinHandle tHandle, const TCRendererInitResult &stRndrInitResults, TProcDelegate *pDelMainLoop, TMsgProcDelegate *pDelMsgProc) = 0;
	virtual HRESULT SendMessage(const TWinMessage &stMsg) = 0;
	virtual HRESULT GetWindowAccessType(E_WINDOW_ACCESS_TYPE &eType) = 0;
	virtual HRESULT GetWindowHandle(TWinHandle &tHandle) = 0;
	virtual HRESULT GetDrawContext(TWinDrawHandle &tHandle) = 0;
	virtual HRESULT GetWinRect(int &iX, int &iY, int &iWidth, int &iHeight) = 0;
	virtual HRESULT ScreenToClient(int &iX, int &iY) = 0;
	virtual HRESULT ConfigureWindow(const TEngWindow &stWind, bool bSetFocus) = 0;
	virtual HRESULT SetCaption(const char *pcTxt) = 0;
	virtual HRESULT Minimize() = 0;
	virtual HRESULT BeginMainLoop() = 0;
	virtual HRESULT KillWindow() = 0;
	virtual HRESULT Free() = 0;
};

// {FB6FAC15-B60F-4F96-98A3-93465429750F}
static const GUID IID_ISpalshWindow = 
{ 0xfb6fac15, 0xb60f, 0x4f96, { 0x98, 0xa3, 0x93, 0x46, 0x54, 0x29, 0x75, 0xf } };

class ISplashWindow
{
public:
	virtual HRESULT InitWindow(bool bSeparateThread, const char *pcBmpFileName) = 0;
	virtual HRESULT SetOwnerWindow(TWinHandle tOwnerHwnd) = 0;
	virtual HRESULT Free() = 0;
};

// {67B449F3-CFF6-4778-8232-B81084527A51}
static const GUID IID_IConsoleWindow = 
{ 0x67b449f3, 0xcff6, 0x4778, { 0x82, 0x32, 0xb8, 0x10, 0x84, 0x52, 0x7a, 0x51 } };

class IConsoleWindow
{
public:
	virtual HRESULT InitWindow(bool bSeparateThread, void (DGLE2_API *pOnCmdExec)(CConsole *pConsole, const char *pcCommand), void (DGLE2_API *pOnCmdComplete)(CConsole *pConsole, const char *pcCommand), CConsole *pConsole) = 0;
	virtual HRESULT Visible(bool bVisible) = 0;
	virtual HRESULT SetSizeAndPos(int iX, int iY, int iWidth, int iHeight) = 0;
	virtual HRESULT GetSizeAndPos(int &iX, int &iY, int &iWidth, int &iHeight) = 0;
	virtual HRESULT GetWindowHandle(TWinHandle &tHandle) = 0;
	virtual HRESULT GetThreadId(uint32 &ui32Id) = 0;
	virtual HRESULT OutputTxt(const char *pcTxt, bool bToPrevLine) = 0;
	virtual HRESULT GetEditTxt(char *pcTxt, uint uiBufferSize) = 0;
	virtual HRESULT SetEditTxt(const char *pcTxt) = 0;
	virtual HRESULT Clear() = 0;
	virtual HRESULT ResetSizeAndPos() = 0;
	virtual HRESULT EnterThreadSafeSec() = 0;
	virtual HRESULT LeaveThreadSafeSec() = 0;
	virtual HRESULT Free() = 0;
};

// {628B089E-DA74-4096-B638-9B4145832E0E}
static const GUID IID_IEngineCoreWrapper = 
{ 0x628b089e, 0xda74, 0x4096, { 0xb6, 0x38, 0x9b, 0x41, 0x45, 0x83, 0x2e, 0xe } };

class IEngineCoreWrapper
{
public:
	virtual HRESULT DGLE2_API SetWindowAPI(IMainWindow *pMainWindow) = 0;
	virtual HRESULT DGLE2_API TranslateMessage(const TWinMessage &stWinMsg) = 0;
	virtual HRESULT DGLE2_API KillEngine() = 0;
};

#ifdef ENGINE_PLATFORM_BASE

class IBaseRenderGL
{
public:
	bool Prepare(TCRendererInitResult &stResults);
	bool Initialize(TCRendererInitResult &stResults);
	bool Finalize();
	bool AdjustMode(TEngWindow &stNewWin);
	bool MakeCurrent();
	void Present();
};

class IBaseSound
{
public:
};

class IBaseInput
{
public:
	void ShowCursor(bool bVisible);
	void GetCursorPos(int &x, int &y);
	void SetCursorPos(int x, int y);
	void ClipCursor(uint left, uint top, uint right, uint bottom);
	bool IsJoystickImplemented();
	uint JoysticksCount();
	std::string GetJoystickName(uint id);
	TJoystickStates GetJoystickStates(uint id);
};

#ifdef PLATFORM_WINDOWS

TWinMessage WinAPIMsgToEngMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
void EngMsgToWinAPIMsg(const TWinMessage &msg, UINT &Msg, WPARAM &wParam, LPARAM &lParam);

#endif

struct TSysTimeAndDate
{
	uint16 ui16Year;
	uint16 ui16Month;
	uint16 ui16Day;
	uint16 ui16Hour;
	uint16 ui16Minute;
	uint16 ui16Second;
	uint16 ui16Milliseconds;
};

bool PlatformInit();
void GetDisplaySize(uint &width, uint &height);
void GetLocalTimaAndDate(TSysTimeAndDate &time);
uint64 GetPerfTimer();
void OutputDebugTxt(const char *pcTxt);
uint CreateTimer(uint uiInterval, TProcDelegate *pDelOnTimer);
bool ReleaseTimer(uint id);
void Suspend(uint uiMilliseconds);
TDynLibHandle LoadDynamicLib(const char *pcFileName);
void* GetFuncAddress(TDynLibHandle tHandle, const char *pcFuncName);
bool ReleaseDynamicLib(TDynLibHandle tHandle);
void ShowModalUserAlert(const char *pcTxt, const char *pcCaption);
void GetEngineFilePath(std::string &strPath);
bool FindFilesInDir(const char* pcMask, std::vector<std::string> &strs, bool bInnerFolders);
void GetSystemInformation(std::string &strInfo, TSystemInfo &stSysInfo);
void Terminate();

#endif //ENGINE_PLATFORM_BASE

}