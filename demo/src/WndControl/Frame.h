#pragma once
#include "WindowBase.h"
#include "TabLayout.h"

class Frame : public WindowBase 
{
public:
	Frame(HINSTANCE hInst);

	//LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	//
	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	//{
	//	return ((Frame*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	//}

	void showWindow();

	TabLayout* tabLayout;
protected:
	WNDPROC _SysWndProc=NULL;

};



