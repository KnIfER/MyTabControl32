#include <windows.h>
#include "comctl32.h"

extern "C" {

extern void TAB_Register(void);

HWND _hWnd;

HWND   _hParent;

int cc;

void addTabItem(const TCHAR *text)
{
	TCITEM data;
	data.mask = TCIF_TEXT;
	data.pszText = (TCHAR *)text;
	SendMessage(_hWnd, TCM_INSERTITEM, cc++, reinterpret_cast<LPARAM>(&data));
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	MSG    msg;
	WNDCLASS wndclass;//WNDCLASSEX比WNDCLASS多两个结构成员－－cbSize(指定WNDCLASSEX结构的大小－－字节)  －－hIconSm(标识类的小图标)
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//白色//(HBRUSH)(COLOR_MENU  +1)界面灰
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	RegisterClass(&wndclass);

	_hParent = CreateWindow(szAppName,      // window class name
		TEXT("The Hello Program"),   // window caption
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX  | WS_MAXIMIZEBOX | WS_SIZEBOX,
		CW_USEDEFAULT,// initial x position
		CW_USEDEFAULT,// initial y position
		CW_USEDEFAULT,// initial x size
		CW_USEDEFAULT,// initial y size
		NULL,                 // parent window handle
		NULL,            // window menu handle
		hInstance,   // program instance handle
		NULL);      // creation parameters

	ShowWindow(_hParent, iCmdShow);
	UpdateWindow(_hParent);


	TAB_Register();


	int style = WS_CHILD | WS_VISIBLE ;

	_hWnd = ::CreateWindowEx(
		0,
		L"MyTabControl32",
		TEXT("Tab"),
		style,
		0, 0, 800, 100,
		_hParent,
		NULL,
		hInstance,
		0);

	addTabItem(L"happy");
	addTabItem(L"sadsdsfdsfsdfsdfsdfdss");


	SendMessage(_hParent, WM_SIZE, 0, 0);


	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR s[] = TEXT("Hello, Windows.");
	switch (message)
	{
	case WM_DRAWITEM:
	{
		break;
	}
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rect;
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		DrawText (hdc, s, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		if (_hWnd)
		{
			RECT rect;
			GetClientRect(_hParent, &rect);
			MoveWindow(_hWnd, rect.left, rect.top, rect.right, rect.bottom, true);
		}
		return 0;
	case WM_QUIT:
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

}