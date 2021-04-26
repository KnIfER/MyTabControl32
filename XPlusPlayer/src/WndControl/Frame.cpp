
#include "Frame.h"
#include "resource.h"
#include "SeekBar.h"
#include "VideoPlayer.h"
#include "InsituDebug.h"

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


	// Get the height of the toolbar.
	dwBtnSize = SendMessage(htoolbar, TB_GETBUTTONSIZE, 0, 0);

	// Set values unique to the band with the toolbar.
	rbBand.lpText = (LPWSTR)L"Tool Bar";
	rbBand.hwndChild = htoolbar;
	rbBand.cxMinChild = 0;
	rbBand.cyMinChild = HIWORD(dwBtnSize);
	rbBand.cx = 250;

	// Add the band that has the toolbar.
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

SeekBar* mSeekBar=0;

Frame::Frame(HINSTANCE hInstance)
{
	Frame_Register(hInstance);

	Register(hInstance);

	_hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, L"Hello",      // window class name
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
		this);      // creation parameters

	//_SysWndProc = (WNDPROC)GetWindowLongPtr(_hWnd, GWL_WNDPROC);

	//_SysWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWL_WNDPROC, (LONG_PTR)WndProc);

	//tabLayout = new TabLayout(hInstance, _hWnd);

	toolbar = new ButtonList(hInstance, _hWnd);

	//CreateRebar(_hWnd, toolbar->getHWND());

	RECT rect;
	GetClientRect(_hWnd, &rect);

	MoveWindow(toolbar->getHWND(), rect.left, 100+rect.top, rect.right, 24, true);

	SetWindowPos(toolbar->getHWND(), 0 , 100+rect.left
		, 100+rect.top, rect.right, 12, SWP_SHOWWINDOW);


	mSeekBar = new SeekBar(hInstance, _hWnd);



	mMediaPlayer0 = new VPlayerXunBo(hInstance, _hWnd);
}

void Frame::showWindow()
{
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);
}

HWND Frame::GetMediaPlayerHWND()
{
	return mMediaPlayer0?mMediaPlayer0->getHWND():NULL;
}

void Frame::Toggle()
{
	if (mMediaPlayer0)
	{
		if (_isPlaying=!_isPlaying)
		{
			mMediaPlayer0->Play();
		}
		else 
		{
			mMediaPlayer0->Pause();
		}
		toolbar->ReplaceIcon(0, _isPlaying?IDI_PAUSE:IDI_PLAY);
	}
}


LRESULT Frame::RunProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static TCHAR s[] = TEXT("Hello, Windows.");
	switch (msg)
	{
		//case WM_DRAWITEM:
		//{
		//	break;
		//}
	case WM_DROPFILES:
	{
		HDROP hDropInfo = (HDROP)wParam;
		UINT  nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
		TCHAR szFileName[_MAX_PATH] = _T("");
		DWORD dwAttribute;

		// 获取拖拽进来文件和文件夹
		for (UINT i = 0; i < nFileCount; i++)
		{
			::DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
			dwAttribute = ::GetFileAttributes(szFileName);

			// 是否为文件夹
			if (dwAttribute & FILE_ATTRIBUTE_DIRECTORY)
			{          
			}
			else
			{
				// szFileName
				mMediaPlayer0->PlayVideoFile(szFileName);
			}
		}

		::DragFinish(hDropInfo);
		return true;
	
	};
		break;

	case WM_CONTEXTMENU:
	{

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
		if (tabLayout)
		{
			MoveWindow(tabLayout->getHWND(), rect.left, rect.top, rect.right, rect.bottom, true);
		}
		int videoHeightAvailable = rect.bottom - rect.top;
		int toolbarHeight = 0;
		int seekbarHeight = 0;
		if (toolbar)
		{
			SendMessage(toolbar->getHWND(), TB_AUTOSIZE, 0, 0);
			RECT toolbarRect;
			GetClientRect(toolbar->getHWND(), &toolbarRect);
			toolbarHeight = toolbarRect.bottom-toolbarRect.top;
			//MoveWindow(toolbar->getHWND(), 1000+rect.left, rect.top, rect.right, rect.bottom, true);

			//SetWindowPos(toolbar->getHWND(), 0
			//	, 1000+rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		}
		if (mSeekBar)
		{
			seekbarHeight = 24;
			MoveWindow(mSeekBar->getHWND(), rect.left, rect.bottom-toolbarHeight-seekbarHeight, rect.right, seekbarHeight, true);

			//SetWindowPos(toolbar->getHWND(), 0
			//	, 1000+rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		}
		if (mMediaPlayer0)
		{
			//mMediaPlayer0->SetAPlayerWindowSize(hwnd);
			::SetWindowPos(mMediaPlayer0->getHWND(), HWND_TOP, 
				rect.left, 
				rect.top, 
				rect.right - rect.left, 
				videoHeightAvailable-toolbarHeight-seekbarHeight, 
				SWP_SHOWWINDOW);
		}
		return 0;
	}

	case WM_TIMER:
	{
		if (wParam != 1 || mMediaPlayer0->m_pAPlayer == NULL)
		{
			break;
		}
		
		if (mMediaPlayer0->m_pAPlayer->GetState() == PS_PLAY || 
			mMediaPlayer0->m_pAPlayer->GetState() == PS_PAUSED)
		{
			mMediaPlayer0->m_nPosition = mMediaPlayer0->m_pAPlayer->GetPosition();
		
			//TCHAR szPosition[64];
			//TCHAR szDuration[64];
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nPosition, szPosition);
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nDuration, szDuration);
		
			//lstrcat(szPosition, _T("/"));
			//lstrcat(szPosition, szDuration);
		
			int nPos =  int (mMediaPlayer0->m_nPosition / (double)mMediaPlayer0->m_nDuration * 1000);
			//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nPos);
			
			if (mSeekBar && !mSeekBar->_isSeeking)
			{
				mSeekBar->SetPosition(mMediaPlayer0->m_nPosition);
			}

			LogIs(3, "setPosition:: %d %d max=%d curr=%d\n", mMediaPlayer0->m_nPosition, mMediaPlayer0->m_nDuration, mSeekBar->GetMax(), mSeekBar->GetPosition());

			//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nPos);
			//SetWindowText(GetDlgItem(hwnd, IDC_STATIC1), szPosition);
		}

	}
		return 0;


	case WM_HSCROLL:
	{
		//LogIs("SetTime GETPOS:: %ld \n", mSeekBar->GetPosition());
		if ((HWND)lParam==mSeekBar->getHWND())
		{
			if (mMediaPlayer0->m_pAPlayer != NULL && mSeekBar->_isSeeking)
			{
				if (mMediaPlayer0->m_pAPlayer->GetState() == PS_PLAY || 
					mMediaPlayer0->m_pAPlayer->GetState() == PS_PAUSED)
				{
					mMediaPlayer0->m_pAPlayer->SetPosition(wParam);
				}
			}
		}
	}
	break;

	case WM_QUIT:
		return 0;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_SPACE:
			Toggle();
			RECT rc;
			GetClientRect(toolbar->getHWND(), &rc);
			rc.right = rc.bottom;
			InvalidateRect(toolbar->getHWND(), &rc, TRUE);
		break;
		default:
			break;
		}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDM_START:
			Toggle();
			break;
		case IDM_STOP:
			if (mMediaPlayer0)
				mMediaPlayer0->Stop();
			break;
		case IDM_OPEN:
			if (mMediaPlayer0)
				mMediaPlayer0->OpenLocalFile();
			_isPlaying = true;
			break;
		default:
			break;
		}

	}
	break;
	case MM_PREPARED:
	{
		LogIs("MPM_PREPARED\n");
		if (hwnd==_hWnd && mSeekBar)
		{
			mSeekBar->SetMax(wParam);
		}
		toolbar->ReplaceIcon(0, IDI_PAUSE);
		break;
	}
	case WM_NOTIFY:
	{
		switch (((NMHDR FAR *) lParam)->code) 
		{
		case PSN_HELP:
		{
			break;
		}
		}
	}
	break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK Frame::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!hwnd)
	{
		return FALSE;
	}
	else if(message==WM_NCCREATE)
	{
		Frame* app = (Frame*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		app->_hWnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	return ((Frame*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
}

void Frame_Register(HINSTANCE hInstance)
{
	wchar_t lpszMenuName[] = L"Menu";

	static TCHAR szAppName[] = TEXT("Hello");
	MSG    msg;
	WNDCLASS wndclass;//WNDCLASSEX比WNDCLASS多两个结构成员－－cbSize(指定WNDCLASSEX结构的大小－－字节)  －－hIconSm(标识类的小图标)
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = Frame::WndProc;
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