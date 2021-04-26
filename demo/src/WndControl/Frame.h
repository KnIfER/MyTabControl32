#pragma once
#include "WindowBase.h"
#include "TabLayout.h"
#include "ButtonList.h"

class XPlusWindow : public WindowBase 
{
public:
	XPlusWindow(HINSTANCE hInst);

	//LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	//
	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	//{
	//	return ((XPlusWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	//}

	void showWindow();

	TabLayout* _tabLayout;
	ButtonList* _toolbar;
protected:
	WNDPROC _SysWndProc=NULL;

};



