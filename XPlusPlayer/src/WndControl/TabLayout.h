#pragma once
#include "WindowBase.h"

class TabLayout : public WindowBase 
{
public:
	TabLayout()=default;
	void init(HINSTANCE hInstance, HWND hParent);
	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((TabLayout*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}

	void showWindow();

	void addTab(const TCHAR *text, int image=-1);

protected:
	WNDPROC _SysWndProc=NULL;

	int size;
};



