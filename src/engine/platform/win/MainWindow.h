/**
\author		Korotkov Andrey aka DRON
\date		23.02.2012 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#pragma once

#include "..\Common.h"

class CMainWindow : public CInstancedObj, public IMainWindow
{
	HINSTANCE			_hInst;
	HWND				_hWnd;
	HDC					_hDC;
	TProcDelegate		*_pDelMainLoop;
	TMsgProcDelegate	*_pDelMessageProc;
	bool				_bFScreen;
	bool				_bIsLooping;

	int WINAPI				_wWinMain(HINSTANCE hInstance);	
	static LRESULT DGLE2_API _s_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void DGLE2_API	_s_ConsoleQuit(void *pParametr, const char *pcParam);

public:
	
	CMainWindow(uint uiInstIdx);
	~CMainWindow();

	HRESULT InitWindow(TWinHandle tHandle, const TCRendererInitResult &stRndrInitResults, TProcDelegate *pDelMainLoop, TMsgProcDelegate *pDelMsgProc);
	HRESULT SendMessage(const TWinMessage &stMsg);
	HRESULT GetWindowAccessType(E_WINDOW_ACCESS_TYPE &eType);
	HRESULT GetWindowHandle(TWinHandle &tHandle);
	HRESULT GetDrawContext(TWinDrawHandle &tHandle);
	HRESULT GetWinRect(int &iX, int &iY, int &iWidth, int &iHeight);
	HRESULT ScreenToClient(int &iX, int &iY);
	HRESULT ConfigureWindow(const TEngWindow &stWind, bool bSetFocus);
	HRESULT SetCaption(const char *pcTxt);
	HRESULT Minimize();
	HRESULT BeginMainLoop();
	HRESULT KillWindow();
	HRESULT Free();

};