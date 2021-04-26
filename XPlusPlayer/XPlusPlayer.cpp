#include <windows.h>
#include "XPlusWindow.h"
#include "TabLayout.h"
#include "XPlusPlayer.h"
#include <iostream>


struct DemoData
{
	const TCHAR* title;
	int image;
};

XPlusWindow XPP;

void hookMouseMove(MSG & msg)
{
	int yPos = msg.pt.y;
	RECT rc;
	GetClientRect(XPP.getHWND(), &rc);
	if (IsWindowVisible(XPP._hFullScreenBtmbar))
	{
		if (!XPP._seekbar._isSeeking && yPos<rc.bottom-XPP._barsHeight-12)
		{
			ShowWindow(XPP._hFullScreenBtmbar, SW_HIDE);
		}
	}
	else if (yPos>=rc.bottom-XPP._barsHeight) // -4
	{
		ShowWindow(XPP._hFullScreenBtmbar, SW_SHOW);
	}
}

void hookLButtonDown(MSG & msg)
{
	if(XPP.isFullScreen())
		return;
	if (msg.hwnd==XPP._toolbar.getHWND())
	{
		if (GET_X_LPARAM(msg.lParam)>XPP._toolbar.getHeight()*5)
		{
			ReleaseCapture();
			SendMessage(XPP.getHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		}
		return;
	}
	if (IsChild(XPP.GetMediaPlayerHWND(), msg.hwnd))
	{
		ReleaseCapture();
		SendMessage(XPP.getHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return;
	}
}

void hookMButtonClick(MSG & msg)
{
	if (msg.hwnd==XPP._toolbar.getHWND())
	{
		if (GET_X_LPARAM(msg.lParam)>XPP._toolbar.getHeight()*5)
		{
			XPP.ToggleFullScreen();
		}
		return;
	}
	if (IsChild(XPP.GetMediaPlayerHWND(), msg.hwnd))
	{
		XPP.ToggleFullScreen();
		return;
	}
}

int APIENTRY 
wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// 初始化公共空间
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc);

	XPP.init(hInstance, NULL);

	XPP.showWindow();

	MSG    msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{

		if (msg.message==WM_LBUTTONDOWN)
		{
			hookLButtonDown(msg);
		}
		if (msg.message==WM_MOUSEMOVE && XPP.isFullScreen())
		{
			hookMouseMove(msg);
		}
		if (msg.message==WM_MBUTTONUP)
		{
			hookMButtonClick(msg);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return msg.wParam;
}
