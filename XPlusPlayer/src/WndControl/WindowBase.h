#pragma once
#include "windows.h"
#include "comctl32.h"

#define SendWndMessage(Msg, wParam, lParam) SendMessage(_hWnd, Msg, wParam, lParam)

class WindowBase 
{
public:
	HWND getHWND() {
		return _hWnd;
	};
	HWND getHParent(){
		return _hParent;
	};
protected:
	HINSTANCE _hInst;
	HWND _hWnd;
	HWND _hParent;
	WNDPROC _SysWndProc=NULL;
};



