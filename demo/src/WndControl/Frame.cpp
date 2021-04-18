
#include "Frame.h"

void Frame_Register(HINSTANCE hInstance);

Frame::Frame(HINSTANCE hInstance)
{
	Frame_Register(hInstance);
	_hWnd = CreateWindow(L"Hello",      // window class name
		TEXT("The Hello Program"),   // window caption
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX  
		| WS_MAXIMIZEBOX | WS_SIZEBOX
		| WS_CLIPCHILDREN 
		,
		CW_USEDEFAULT,// initial x position
		CW_USEDEFAULT,// initial y position
		CW_USEDEFAULT,// initial x size
		CW_USEDEFAULT,// initial y size
		NULL,                 // parent window handle
		NULL,            // window menu handle
		hInstance,   // program instance handle
		NULL);      // creation parameters

	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	//_SysWndProc = (WNDPROC)GetWindowLongPtr(_hWnd, GWL_WNDPROC);

	//_SysWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWL_WNDPROC, (LONG_PTR)Frame::WndProc);

	tabLayout = new TabLayout(hInstance, _hWnd);


}

void Frame::showWindow()
{
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);
}


static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	Frame* app = (Frame*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	static TCHAR s[] = TEXT("Hello, Windows.");
	switch (msg)
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
		if (app->tabLayout)
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			MoveWindow(app->tabLayout->getHWND(), rect.left, rect.top, rect.right, rect.bottom, true);
		}
		return 0;
	case WM_QUIT:
		return 0;
	}
	//if (_SysWndProc)
	//{
	//	CallWindowProc(_SysWndProc, hwnd, msg, w, l);
	//}
	return DefWindowProc(hwnd, msg, w, l);
}

void Frame_Register(HINSTANCE hInstance)
{
	static TCHAR szAppName[] = TEXT("Hello");
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
}

void Frame_UnRegister()
{
}