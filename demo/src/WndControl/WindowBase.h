#pragma once
#include "windows.h"
#include "comctl32.h"

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
	HWND _hWnd;
	HWND _hParent;
};



