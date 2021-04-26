
#include "Frame.h"
#include "menu.h"
#include "resource.h"

void Frame_Register(HINSTANCE hInstance);


HWND WINAPI CreateRebar (HWND hwndOwner, HWND htoolbar)
{
	REBARINFO     rbi;
	REBARBANDINFO rbBand;
	RECT          rc;
	HWND   hwndRB;
	DWORD  dwBtnSize;

	hwndRB = CreateWindowExW(WS_EX_TOOLWINDOW,
		REBARCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN | RBS_VARHEIGHT |
		CCS_NODIVIDER,
		0, 0, 0, 0,
		hwndOwner,
		NULL,
		GetModuleHandleW(NULL),
		NULL);
	if (!hwndRB)
		return NULL;
	// Initialize and send the REBARINFO structure.
	rbi.cbSize = sizeof(REBARINFO);  // Required when using this
									 // structure.
	rbi.fMask = 0;
	rbi.himl = (HIMAGELIST)NULL;
	if (!SendMessage(hwndRB, RB_SETBARINFO, 0, (LPARAM)&rbi))
		return NULL;
	// Initialize structure members that both bands will share.
	rbBand.cbSize = sizeof(REBARBANDINFO);  // Required
	rbBand.fMask = RBBIM_COLORS | RBBIM_TEXT | RBBIM_BACKGROUND |
		RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE |
		RBBIM_SIZE;
	rbBand.fStyle = RBBS_CHILDEDGE | RBBS_FIXEDBMP;
	rbBand.hbmBack = NULL; 


	// Get the height of the _toolbar.
	dwBtnSize = SendMessage(htoolbar, TB_GETBUTTONSIZE, 0, 0);

	// Set values unique to the band with the _toolbar.
	rbBand.lpText = (LPWSTR)L"Tool Bar";
	rbBand.hwndChild = htoolbar;
	rbBand.cxMinChild = 0;
	rbBand.cyMinChild = HIWORD(dwBtnSize);
	rbBand.cx = 250;

	// Add the band that has the _toolbar.
	SendMessage(hwndRB, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
	return (hwndRB);
}


void Register(HINSTANCE hInstance)
{
	wchar_t lpszMenuName[] = L"Menu";

	auto MenuClassName = L"32768";
	MenuClassName = L"0x8000";
	MenuClassName = L"8000";

	static TCHAR szAppName[] = TEXT("Hello");
	MSG    msg;
	WNDCLASS wndclass;//WNDCLASSEX比WNDCLASS多两个结构成员－－cbSize(指定WNDCLASSEX结构的大小－－字节)  －－hIconSm(标识类的小图标)
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	//wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_MENU + 1);//白色 COLOR_WINDOW // COLOR_MENU 界面灰
	wndclass.hbrBackground = CreateSolidBrush (RGB(0,0,0));
	wndclass.lpszMenuName = lpszMenuName;
	wndclass.lpszClassName = MenuClassName;

	UnregisterClass(MenuClassName, hInstance);

	RegisterClass(&wndclass);
}

XPlusWindow::XPlusWindow(HINSTANCE hInstance)
{
	Frame_Register(hInstance);

	Register(hInstance);

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

	//_SysWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWL_WNDPROC, (LONG_PTR)XPlusWindow::WndProc);

	//_tabLayout = new TabLayout(hInstance, _hWnd);

	_toolbar = new ButtonList(hInstance, _hWnd);

	//CreateRebar(_hWnd, _toolbar->getHWND());

	RECT rect;
	GetClientRect(_hWnd, &rect);

	MoveWindow(_toolbar->getHWND(), rect.left, 100+rect.top, rect.right, 24, true);

	SetWindowPos(_toolbar->getHWND(), 0 , 100+rect.left
		, 100+rect.top, rect.right, 12, SWP_SHOWWINDOW);
}

void XPlusWindow::showWindow()
{
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);
}


static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	XPlusWindow* app = (XPlusWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	static TCHAR s[] = TEXT("Hello, Windows.");
	switch (msg)
	{
	//case WM_DRAWITEM:
	//{
	//	break;
	//}

	case WM_CONTEXTMENU:
	{
		test_init();

		test_menu_ownerdraw(hwnd, 0);

		return 0;
	}

	case WM_CREATE:
	{

		MENUINFO mi = { 0 }; 
		mi.cbSize = sizeof(mi); 
		mi.fMask = MIM_BACKGROUND|MIM_APPLYTOSUBMENUS; 
		mi.hbrBack =  CreateSolidBrush (RGB(0,0,0)); 
		mi.dwStyle = MNS_AUTODISMISS;

		HMENU hMenu = ::GetMenu(hwnd); 
		SetMenuInfo(hMenu, &mi); 
	
		//test_menu_add_string(hMenu);
		//test_menualign(0);

		
		//test_menu_ownerdraw(hwnd, GetSubMenu(hMenu, 0));

		//test_menu_ownerdraw(hwnd, 0);

		return 0;
	}


	//case WM_PAINT:
	//{
	//
	//	HDC hdc;
	//	PAINTSTRUCT ps;
	//	RECT rect;
	//	GetClientRect(hwnd, &rect);
	//	hdc = BeginPaint(hwnd, &ps);
	//	DrawText (hdc, s, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
	//	EndPaint(hwnd, &ps);
	//	return 0;
	//}


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		if (app->_tabLayout)
		{
			MoveWindow(app->_tabLayout->getHWND(), rect.left, rect.top, rect.right, rect.bottom, true);
		}
		if (app->_toolbar)
		{
			SendMessage(app->_toolbar->getHWND(), TB_AUTOSIZE, 0, 0);
			//MoveWindow(app->_toolbar->getHWND(), 1000+rect.left, rect.top, rect.right, rect.bottom, true);

			//SetWindowPos(app->_toolbar->getHWND(), 0
			//	, 1000+rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		}
		return 0;
	}

	case WM_QUIT:
		return 0;

	case WM_COMMAND:
	{
		switch (w)
		{
		case IDM_START:



			break;
		default:
			break;
		}
	
	}
		break;

	case WM_APP + 0x3F0E:       
	{

		::MessageBoxA(NULL, ("111"), (""), MB_OK);

		return true;
	}

	}

	//if (_SysWndProc)
	//{
	//	CallWindowProc(_SysWndProc, hwnd, msg, w, l);
	//}
	return DefWindowProc(hwnd, msg, w, l);
}

void Frame_Register(HINSTANCE hInstance)
{
	wchar_t lpszMenuName[] = L"Menu";

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
	wndclass.hbrBackground = (HBRUSH)(COLOR_MENU + 1);//白色 COLOR_WINDOW // COLOR_MENU 界面灰
	wndclass.hbrBackground = CreateSolidBrush (RGB(29,29,29));
	wndclass.lpszMenuName = lpszMenuName;
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = szAppName;

	RegisterClass(&wndclass);
}

void Frame_UnRegister()
{
}