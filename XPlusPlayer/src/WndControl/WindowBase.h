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
	HWND getHParent() {
		return _hParent;
	};
	void init(HINSTANCE hInstance, HWND hParent) {
		_hInst=hInstance;
		_hParent=hParent;
	};
	LONG getHeight() {
		RECT rc;
		GetClientRect(_hWnd, &rc);
		return rc.bottom;
	};
	LONG getWidth() {
		RECT rc;
		GetClientRect(_hWnd, &rc);
		return rc.right;
	};
protected:
	HINSTANCE _hInst;
	HWND _hWnd;
	HWND _hParent;
	WNDPROC _SysWndProc=NULL;
};



