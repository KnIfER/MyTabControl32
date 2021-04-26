#pragma once
#include "WindowBase.h"

class SeekBar : public WindowBase 
{
public:
	SeekBar()=default;
	void init(HINSTANCE hInstance, HWND hParent);

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((SeekBar*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}

	void showWindow();

	void SetPosition(LONG pos);
	LONG GetPosition();
	void SetMax(LONG val);
	LONG GetMax();
	void SetMin(LONG val);
	LONG GetMin();

	bool _isSeeking=false;
protected:
	WNDPROC _SysWndProc=NULL;

	int size;
};



