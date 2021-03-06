/**
\author		Korotkov Andrey aka DRON
\date		25.02.2012 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#include "HookedWindow.h"

CHookedWindow::CHookedWindow(uint uiInstIdx):
CInstancedObj(uiInstIdx), _bNoMloopHook(false),
_stOldWindowProc(NULL), _stOldRootWindowProc(NULL),
_hWnd(NULL), _hDC(NULL), _hRootHWnd(NULL),
_uiUpdateTimer(-1)
{}

LRESULT DGLE2_API CHookedWindow::_s_RootWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CHookedWindow *this_ptr = (CHookedWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (msg == WM_CLOSE && CallWindowProc(this_ptr->_stOldRootWindowProc, hWnd, msg, wParam, lParam) == 0)
	{
		this_ptr->_KillWindow();
		return 0;
	}
	else
	if (msg == WM_DESTROY)
	{
		CallWindowProc(this_ptr->_stOldRootWindowProc, hWnd, msg, wParam, lParam );
		this_ptr->_KillWindow();
		return 0;
	}
	else
	{
		switch(msg)
		{
		case WM_KILLFOCUS:
		case WM_SETFOCUS:
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_CHAR:
		case WM_MOUSEWHEEL:
			this_ptr->_pDelMessageProc->Invoke(WinAPIMsgToEngMsg(msg, wParam, lParam));
			break;

		case WM_SIZING:
			RECT r;
			GetWindowRect(this_ptr->_hWnd, &r);
			this_ptr->_pDelMessageProc->Invoke(WinAPIMsgToEngMsg(WM_SIZING, wParam, (LPARAM)&r));
			break;
		}

		if (this_ptr->_stOldRootWindowProc)
			return CallWindowProc(this_ptr->_stOldRootWindowProc, hWnd, msg, wParam, lParam );
		else
			return DefWindowProc(hWnd, msg, wParam, lParam );
	}
}

LRESULT DGLE2_API CHookedWindow::_s_WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CHookedWindow *this_ptr = (CHookedWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (msg == WM_DESTROY)
	{
		this_ptr->_KillWindow();
		return DefWindowProc(hWnd, msg, wParam, lParam );
	}
	else
		this_ptr->_pDelMessageProc->Invoke(WinAPIMsgToEngMsg(msg, wParam, lParam));

	if(!this_ptr->_stOldWindowProc || (msg == WM_PAINT || msg == WM_SETTEXT || msg == WM_NCPAINT|| msg == WM_ERASEBKGND))
		return DefWindowProc(hWnd, msg, wParam, lParam );
	else
		return CallWindowProc(this_ptr->_stOldWindowProc, hWnd, msg, wParam, lParam);
}

HRESULT CHookedWindow::InitWindow(TWinHandle tHandle, const TCRendererInitResult &stRndrInitResults, TProcDelegate *pDelMainLoop, TMsgProcDelegate *pDelMsgProc)
{
	if(_stOldWindowProc || _stOldRootWindowProc)
		return E_FAIL;

	_hWnd = tHandle;
	_pDelMainLoop = pDelMainLoop;
	_pDelMessageProc = pDelMsgProc;
	_bNoMloopHook = (Core()->EngWindow()->uiFlags & EWF_DONT_HOOK_MLOOP) != 0;

	if (IsWindow(tHandle) == FALSE)
	{
		LOG("Received HWND is not window based control handle.", LT_FATAL);
		return E_INVALIDARG;
	}

	_hRootHWnd = GetAncestor(_hWnd, GA_ROOT);

	if (_hRootHWnd == NULL)
		LOG("Can't get root window handle.", LT_ERROR);
	else
	{
		if (_hRootHWnd == _hWnd)
		{
			LOG("Engine can't be assigned to the root window.", LT_FATAL);
			return E_INVALIDARG;
		}

		if (Core()->EngWindow()->uiFlags & EWF_DONT_HOOK_ROOT_WIN)
			_stOldRootWindowProc = NULL;
		else
		{
			_stOldRootWindowProc = (WNDPROC)SetWindowLongPtr(_hRootHWnd, GWLP_WNDPROC, (LONG_PTR)_s_RootWindowProc);

			if (_stOldRootWindowProc == NULL)
				LOG("Failed to set root window message hook.", LT_ERROR);
			else
			{
				SetWindowLongPtr(_hRootHWnd, GWLP_USERDATA, (LONG_PTR)this);

				Console()->RegComProc("quit", "Closes owner root window, quits engine and releases all resources.", &_s_ConsoleQuit, (void*)this);
			}
		}
	}

	_stOldWindowProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWLP_WNDPROC, (LONG_PTR)_s_WindowProc);
	
	if (_stOldWindowProc != NULL)
	{
		SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	
		LOG("Window control message hook has been set successfully.", LT_INFO);
		
		if (!(_hDC = GetDC(_hWnd)))
		{
			LOG("Can't get window Draw Context.", LT_FATAL);
			return E_FAIL;
		}

		return S_OK;
	}
	else
	{
		LOG("Failed to set window control message hook.", LT_ERROR);
		return E_FAIL;
	}
}

HRESULT CHookedWindow::SendMessage(const TWinMessage &stMsg)
{
	if (!_hWnd)
		return E_FAIL;

	UINT msg; WPARAM wparam; LPARAM lparam;
	EngMsgToWinAPIMsg(stMsg, msg, wparam, lparam);
	::SendMessage(_hWnd, msg, wparam, lparam);

	return S_OK;
}

HRESULT CHookedWindow::GetWindowAccessType(E_WINDOW_ACCESS_TYPE &eType)
{
	eType = WAT_RESTRICTED_ACCESS;

	return S_OK;
}

HRESULT CHookedWindow::GetWindowHandle(TWinHandle &stHandle)
{
	stHandle = _hWnd;

	return S_OK;
}

HRESULT CHookedWindow::GetDrawContext(TWinDrawHandle &tHandle)
{
	if (!_hDC)
		return E_FAIL;

	tHandle = _hDC;

	return S_OK;
}

HRESULT CHookedWindow::GetWinRect(int &iX, int &iY, int &iWidth, int &iHeight)
{
	if (!_hWnd)
		return E_FAIL;

	RECT rect;
	
	if (GetClientRect(_hWnd, &rect) == FALSE)
	{
		iX = iY = iWidth = iHeight = 0;
		LOG("Can't get window client rect.", LT_ERROR);
		return E_FAIL;
	}

	POINT lt, rb;
	
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;

	ClientToScreen(_hWnd, &lt);
	ClientToScreen(_hWnd, &rb);

	iX = lt.x;
	iY = rb.x;
	iWidth = rb.x - lt.x;
	iHeight = rb.y - lt.y;

	return S_OK;
}

HRESULT CHookedWindow::ScreenToClient(int &iX, int &iY)
{
	if (!_hWnd)
		return E_FAIL;

	POINT p;

	p.x = iX; p.y = iY;
	::ScreenToClient(_hWnd, &p);
	iX = p.x; iY = p.y;

	return S_OK;
}

HRESULT CHookedWindow::ConfigureWindow(const TEngWindow &stWind, bool bSetFocus)
{
	return E_NOTIMPL;
}

HRESULT CHookedWindow::SetCaption(const char *pcTxt)
{
	return E_NOTIMPL;
}

HRESULT CHookedWindow::Minimize()
{
	return E_NOTIMPL;
}

HRESULT CHookedWindow::BeginMainLoop()
{
	if (!_hWnd)
		return E_FAIL;

	if (!_stOldWindowProc && !_stOldRootWindowProc)
		return E_FAIL;

	if (!_bNoMloopHook && (_uiUpdateTimer = CreateTimer(0, _pDelMainLoop), _uiUpdateTimer != -1))
	{
		LOG("Can't set update timer.", LT_FATAL);
		return E_ABORT;
	}

	LOG("**Entering main loop**",LT_INFO);

	return S_OK;
}

void CHookedWindow::_KillWindow()
{
	if (!_stOldWindowProc && !_stOldRootWindowProc)
		return;

	LogWrite(InstIdx(), "**Exiting main loop**", LT_INFO, __FILE__, __LINE__);

	_pDelMessageProc->Invoke(TWinMessage(WMT_CLOSE));
	_pDelMessageProc->Invoke(TWinMessage(WMT_DESTROY));

	if (!_bNoMloopHook && !ReleaseTimer(_uiUpdateTimer))
		LOG("Can't kill update timer.", LT_ERROR);

	if(
		SetWindowLongPtr(_hWnd, GWLP_WNDPROC, (LONG_PTR)_stOldWindowProc)!=NULL &&
		(_stOldRootWindowProc == NULL || SetWindowLongPtr(_hRootHWnd, GWLP_WNDPROC, (LONG_PTR)_stOldRootWindowProc) != NULL)
		)
		LOG("Window controls message hooks unset successfully.", LT_INFO);
	else 
		LOG("Can't unset window control message hooks.", LT_ERROR);

	_stOldWindowProc = NULL;
	_stOldRootWindowProc = NULL;

	_pDelMessageProc->Invoke(TWinMessage(WMT_RELEASED));
}

HRESULT CHookedWindow::KillWindow()
{
	if (_hDC && !ReleaseDC(_hWnd, _hDC))
	{
		LOG("Failed to release Device Context.", LT_ERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CHookedWindow::Free()
{
	delete this;
	return S_OK;
}

void DGLE2_API CHookedWindow::_s_ConsoleQuit(void *pParametr, const char *pcParam)
{
	if (strlen(pcParam)!=0)
		CON(CHookedWindow, "No parametrs expected.");
	else 
		::SendMessage(PTHIS(CHookedWindow)->_hRootHWnd, WM_CLOSE, NULL, NULL);
}