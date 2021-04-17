#pragma once
#include "windows.h"

class WindowBase 
{
public:
	HWND getHWND() {
		return _hWnd;
	};
	HWND getHParent(){
		return _hParent;
	};
private:
	HWND _hWnd;
	HWND _hParent;
};



