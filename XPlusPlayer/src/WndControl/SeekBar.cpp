
#include "SeekBar.h"

#include "resource.h"
#include "InsituDebug.h"


extern void TRACKBAR_Register (void);

void SeekBar::init(HINSTANCE hInstance, HWND hParent)
{
	TRACKBAR_Register();

	_hWnd = CreateWindowEx(0
		, true?L"my_trackbar32":TRACKBAR_CLASS, nullptr
		, WS_CHILD 
		| TBS_HORZ  | WS_VISIBLE
		| TBS_FOCUSNEVER
		| TBS_BOOKMARKTICS
		| TBS_SEEKBAR
		, 20, 50, 200, 23, hParent
		, nullptr, nullptr, nullptr);

	//SendWndMessage(TBM_SETTHUMBLENGTH, 100, 0);
 
	SendWndMessage(TBM_SETRANGEMAX, false, 10000000);

	//SetPosition(20);
	//LogIs(2, "GetMax :: %d", GetMax());
	//LogIs(2, "GetMax :: %d", GetPosition());

	//SendWndMessage(TBM_SETTIC , 0, 200);


	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);


	_SysWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWL_WNDPROC, (LONG_PTR)SeekBar::WndProc);


}

void SeekBar::SetPosition(LONG pos)
{
	SendWndMessage(TBM_SETPOS, true, pos);
}

LONG SeekBar::GetPosition()
{
	return (LONG)SendWndMessage(TBM_GETPOS, 0, 0);
}

void SeekBar::SetMax(LONG val)
{
	SendWndMessage(TBM_SETRANGEMAX, true, val);
}

LONG SeekBar::GetMax()
{
	return (LONG)SendWndMessage(TBM_GETRANGEMAX, 0, 0);
}

void SeekBar::SetMin(LONG val)
{
	SendWndMessage(TBM_SETRANGEMIN, true, val);
}

LONG SeekBar::GetMin()
{
	return (LONG)SendWndMessage(TBM_GETRANGEMIN, 0, 0);
}

void SeekBar::showWindow()
{
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);
}

LRESULT SeekBar::RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		_isSeeking = true;
	break;
	case WM_LBUTTONUP:
		_isSeeking = false;
	break;
	default:
		break;
	}
	if (_SysWndProc)
	{
		::CallWindowProc(_SysWndProc, hwnd, msg, w, l);
	}
	return ::DefWindowProc(hwnd, msg, w, l);
}