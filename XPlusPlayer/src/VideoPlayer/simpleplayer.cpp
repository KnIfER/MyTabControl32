
#include "simpleplayer.h"
#include "Resource.h"
#include "PlayerSink.h"
#include <CommCtrl.h>

#include <atlbase.h>
#include <atlhost.h>

#include "VideoPlayer.h"
#include "InsituDebug.h"

#define countof(x)			(sizeof(x)/sizeof(x[0]))

#define CONFIGID_PLAYRESULT		7		// Play result

#ifdef UNICODE
	#define lstrtoi		_wtoi
#else
	#define lstrtoi		atoi
#endif


/////////////////////////// chHANDLE_DLGMSG Macro /////////////////////////////


// The normal HANDLE_MSG macro in WindowsX.h does not work properly for dialog
// boxes because DlgProc returns a BOOL instead of an LRESULT (like
// WndProcs). This chHANDLE_DLGMSG macro corrects the problem:
#define chHANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg,     \
   HANDLE_##message((hWnd), (wParam), (lParam), (fn))))


CPlayerSink					g_APlayerSink;
HWND						g_hDialog = NULL;
HWND						g_hTextWnd = NULL;
HWND						g_hFullScreenWnd = NULL;

void EnterFullScreen(void);
void ExitFullScreen(void);


HRESULT VPlayerXunBo::OnMessage(LONG nMessage, LONG wParam, LONG lParam)
{
	switch(nMessage)
	{
	case WM_LBUTTONDBLCLK:
		{
			if(::IsWindowVisible(g_hFullScreenWnd))
				ExitFullScreen();
			else
				EnterFullScreen();
		}
		break;

	case WM_KEYDOWN:
		switch(LOWORD(wParam))
		{
		case VK_ESCAPE:
			ExitFullScreen();
			break;
		}
		break;
	}
	return S_OK;
}

int VPlayerXunBo::GetConfigInt(int nConfigId)
{
	USES_CONVERSION;
	_bstr_t strConfig = m_pAPlayer->GetConfig(nConfigId);
	LPTSTR pConfig = W2T(strConfig);
	int nResult = lstrtoi(pConfig);
	return nResult;
}

HRESULT VPlayerXunBo::OnStateChanged(LONG nOldState, LONG nNewState)
{
	TCHAR szText[MAX_PATH];
	if (nNewState==PS_CLOSING)
	{
		//m_pAPlayer->Play();
	}
	//HWND hWnd = GetDlgItem(g_hTextWnd, IDC_STATIC1);
	//
	//switch(nNewState)
	//{
	//case PS_READY:
	//	{
	//		DWORD dwPlayResult = g_MyPlayer.GetConfigInt(CONFIGID_PLAYRESULT);
	//		switch(dwPlayResult)
	//		{
	//		case S_OK:
	//			SetWindowText(hWnd, _T("播放完成！"));
	//			SetWindowText(GetDlgItem(g_hTextWnd, IDPAUSE), _T("播放"));
	//			break;
	//
	//		case S_FALSE:
	//			SetWindowText(hWnd, _T("准备就绪！"));
	//			break;
	//
	//		default:
	//			wsprintf(szText, _T("播放失败，错误码 0x%08X"), dwPlayResult);
	//			SetWindowText(hWnd, szText);
	//			break;
	//		}
	//	}
	//	break;
	//
	//case PS_OPENING:
	//	SetWindowText(hWnd, _T("正在打开..."));
	//	break;
	//
	//case PS_PAUSING:
	//	SetWindowText(hWnd, _T("正在暂停..."));
	//	break;
	//
	//case PS_PAUSED:
	//	break;
	//
	//case PS_PLAYING:
	//	SetWindowText(hWnd, _T("开始播放..."));
	//	break;
	//
	//case PS_PLAY:
	//	break;
	//
	//case PS_CLOSING:
	//	SetWindowText(hWnd, _T("正在关闭..."));
	//	break;
	//}

	return S_OK;
}

HRESULT VPlayerXunBo::OnOpenSucceeded()
{
	//SetWindowText(GetDlgItem(g_hTextWnd, IDPAUSE), _T("暂停"));
	m_nDuration = m_pAPlayer->GetDuration();
	SendMessage(_hParent, MM_PREPARED, m_nDuration, 0);
	return S_OK;
}

HRESULT VPlayerXunBo::OnSeekCompleted(LONG nPosition)
{
	return S_OK;
}

HRESULT VPlayerXunBo::OnBuffer(LONG nPercent)
{
	return S_OK;
}

HRESULT VPlayerXunBo::OnVideoSizeChanged(void)
{
	return S_OK;
}

HRESULT VPlayerXunBo::OnDownloadCodec(BSTR strCodecPath)
{
	// USES_CONVERSION;
	// TCHAR szText[MAX_PATH];
	// TCHAR szPath[MAX_PATH];
	// GetPathFromFullName(W2T(strCodecPath), szPath);
	// wsprintf(szText, _T("缺少解码器 %s, 下载后解压放在%s文件夹下。\n下载地址：http://aplayer.open.xunlei.com/codecs.zip"), W2T(strCodecPath), szPath);
	// MessageBox(g_hTextWnd, szText, _T("SimplePlayer"), MB_ICONINFORMATION);

	// 异步下载解码器模式，通知APlayer取消当前下载
	m_pAPlayer->SetConfig(19, _bstr_t("0"));
	return S_OK;
}

void VPlayerXunBo::Initialize()
{
	m_nPosition = 0;
	m_nDuration = 1000;
	m_pAPlayer = NULL;
	m_pConnectionPoint = NULL;
	m_dwCookie = 0;
	_hWnd = NULL;
	m_hModule = NULL;
}

void VPlayerXunBo::MillisecondToText(int nMs, TCHAR * pszText)
{
	int nSecond = nMs / 1000;
	int nMillisecond = nMs % 1000;

	int nHour = nSecond / 3600;
	nSecond = nSecond % 3600;
	int nMinute = nSecond / 60;
	nSecond = nSecond % 60;

	_stprintf(pszText, _T("%02d:%02d:%02d"), nHour, nMinute, nSecond);
}

BOOL VPlayerXunBo::GetPathFromFullName(const TCHAR * pcszFullName, TCHAR * pszPath)
{
	int nLength = lstrlen(pcszFullName);
	if(nLength >= MAX_PATH)
		return FALSE;
	lstrcpy(pszPath, pcszFullName);
	TCHAR * pEnd = pszPath + nLength - 1;
	while(pEnd > pszPath && *pEnd != '\\') pEnd--;
	*pEnd = '\0';
	return TRUE;
}

void VPlayerXunBo::OpenLocalFile(void)
{
	if (m_pAPlayer == NULL)
	{
		TCHAR szFullName[MAX_PATH];
		GetModuleFileNameW(NULL, szFullName, MAX_PATH);
		TCHAR szPath[MAX_PATH];
		GetPathFromFullName(szFullName, szPath);
		wsprintf(szFullName, _T("请先下载APlayer SDK，并将其放解压放在%s文件夹下。下载地址：http://aplayer.open.xunlei.com/codecs.zip"), szPath);
		MessageBox(g_hTextWnd, szFullName, _T("SimplePlayer"), MB_ICONINFORMATION);
		return ;
	}
	// The file path
	TCHAR szFileName[MAX_PATH];
	memset(szFileName, 0, sizeof(szFileName));

	// Build filter
	TCHAR szAllFormat[512];
	lstrcpy(szAllFormat, _T(""));
	lstrcat(szAllFormat, _T("*.wmv;*.wmp;*.wm;*.asf;*.wma;*.avi;*.wav;*.mpg;*.mpeg;*.dat;"));
	lstrcat(szAllFormat, _T("*.ts;*.mpa;*.mp2;*.vob;*.ifo;*.mp3;*.mid;*.ogm;*.ogg;*.cda;"));
	lstrcat(szAllFormat, _T("*.d2v;*.mp4;*.3gp;*.mkv;*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.mov;"));
	lstrcat(szAllFormat, _T("*.qt;*.amr;*.mpc;*.flv;*.swf;"));
	lstrcat(szAllFormat, _T("*.evo;*.tta;*.m4b;"));
	lstrcat(szAllFormat, _T("*.xv;*.xvx;*.xlmv"));

	TCHAR szFilter[512];
	TCHAR * pFilter = szFilter;
	wsprintf(pFilter, _T("支持的媒体文件(%s)"), szAllFormat);
	pFilter += (lstrlen(pFilter) + 1);
	lstrcpy(pFilter, szAllFormat);
	pFilter += (lstrlen(pFilter) + 1);
	lstrcpy(pFilter, _T("所有文件(*.*)"));
	pFilter += (lstrlen(pFilter) + 1);
	lstrcpy(pFilter, _T("*.*"));
	pFilter += (lstrlen(pFilter) + 1);
	*pFilter = 0; pFilter++;

	// Select file
	BOOL bResult = SelectFileDlg(g_hTextWnd, FALSE, _T("选择媒体文件"), szFilter,
		szFileName, countof(szFileName), NULL, 0);
	if(bResult)
		m_pAPlayer->Open(szFileName);
}

void VPlayerXunBo::Release()
{
	if (m_pAPlayer != NULL)
	{
		m_pAPlayer->Close();

		if (m_dwCookie != 0)
		{
			m_pConnectionPoint->Unadvise(m_dwCookie);
			m_dwCookie = 0;
		}

		m_pAPlayer->Release();
		m_pAPlayer = NULL;

		if (IsWindow(_hWnd))
		{
			DestroyWindow(_hWnd);
			_hWnd = NULL;
		}

		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

HRESULT VPlayerXunBo::CreateInstanceFromFile(const TCHAR * pcszPath,
											REFCLSID rclsid,
											REFIID riid,
											IUnknown * pUnkOuter,
											LPVOID * ppv)
{
	m_hModule = ::LoadLibraryEx(pcszPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hModule == NULL)
		return TYPE_E_CANTLOADLIBRARY;

	typedef HRESULT (STDAPICALLTYPE *_pfnDllGetClassObject)(REFCLSID, REFIID, LPVOID*);
	_pfnDllGetClassObject pfnDllGetClassObject = (_pfnDllGetClassObject)GetProcAddress(m_hModule, "DllGetClassObject");
	if(pfnDllGetClassObject == NULL)
		return CO_E_ERRORINDLL;

	CComPtr<IClassFactory> spCF;
	HRESULT hr = (*pfnDllGetClassObject)(rclsid, IID_IClassFactory, (LPVOID*)&spCF);
	if(FAILED(hr))
		return hr;

	hr = spCF->CreateInstance(pUnkOuter, riid, ppv);
	if(FAILED(hr))
		return hr;

	return S_OK;
}

BOOL VPlayerXunBo::CreateAPlayerWindow()
{
	TCHAR szFullName[MAX_PATH];
	GetModuleFileNameW(NULL, szFullName, MAX_PATH);
	TCHAR szPath[MAX_PATH];
	GetPathFromFullName(szFullName, szPath);
	wsprintf(szPath, _T("%s\\APlayer.dll"), szPath);
	HRESULT hr = CreateInstanceFromFile(szPath, 
										__uuidof(APlayer3Lib::Player), 
										__uuidof(APlayer3Lib::IPlayer), 
										NULL, 
										(void **)&m_pAPlayer);

	if (SUCCEEDED(hr))
	{
		_hWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW, _T(ATLAXWIN_CLASS), _T(""),
										WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
										CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
										g_hTextWnd, NULL, NULL, NULL);
	}

	if (::IsWindow(_hWnd))
	{
		hr = AtlAxAttachControl(m_pAPlayer, _hWnd, NULL);
		//_SysWndProc = SetWindowLong(_hWnd, GWLP_WNDPROC, );
	}

	CComQIPtr<IConnectionPointContainer> spContainer;
	if (SUCCEEDED(hr))
	{
		hr = m_pAPlayer->QueryInterface(__uuidof(IConnectionPointContainer), 
										(void **)&spContainer);
	}

	if (SUCCEEDED(hr))
	{
		hr = spContainer->FindConnectionPoint(__uuidof(APlayer3Lib::_IPlayerEvents), 
												&m_pConnectionPoint);
	}

	if (SUCCEEDED(hr))
	{
		g_APlayerSink.SetEventHandler(this);
		hr = m_pConnectionPoint->Advise(&g_APlayerSink, &m_dwCookie);
	}

	// http://aplayer.open.xunlei.com/bbs/read.php?tid=468712
	m_pAPlayer->SetConfig(119, _bstr_t("1")) ;

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}


void VPlayerXunBo::PlayVideoFile(TCHAR* path)
{
	m_pAPlayer->Open(path);
}

void VPlayerXunBo::Play()
{
	if (m_pAPlayer != NULL)
	{
		m_pAPlayer->Play();
	}
}

void VPlayerXunBo::Pause()
{
	if (m_pAPlayer != NULL)
	{
		m_pAPlayer->Pause();
	}
}

bool VPlayerXunBo::IsPlaying()
{
	return m_pAPlayer!=NULL && m_pAPlayer->GetState()==PS_PLAY;
}

void VPlayerXunBo::Stop()
{
	if (m_pAPlayer != NULL 
		&& (m_pAPlayer->GetState()==PS_PLAY||m_pAPlayer->GetState()==PS_PAUSED)
		)
	{
		m_pAPlayer->Close();
		{
			
			//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
			//SetWindowText(GetDlgItem(hwnd, IDC_STATIC1), _T("00:00:00/00:00:00"));
			//SetWindowText(GetDlgItem(g_hTextWnd, IDPAUSE), _T("播放"));
		}
	}
}

BOOL VPlayerXunBo::SetAPlayerWindowSize(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	BOOL bResult = FALSE;
	if (::IsWindow(_hWnd))
	{
		bResult = ::SetWindowPos(_hWnd, 
								HWND_TOP, 
								rect.left, 
								rect.top, 
								rect.right - rect.left, 
								rect.bottom - rect.top - 60, 
								SWP_SHOWWINDOW);
	}

	return bResult;
}

// Sets the dialog box icons
inline void chSETDLGICONS(HWND hWnd, int idi)
{
	SendMessage(hWnd, WM_SETICON, ICON_BIG,  (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{

	return(TRUE);
}

BOOL VPlayerXunBo::SelectFileDlg(HWND hOwner,
				   BOOL bIsSave,
				   const TCHAR * pcszTitle,
				   const TCHAR * pcszFilter,
				   TCHAR * pszFilePath,
				   int nMaxFilePath,
				   TCHAR * pszFileTitle,
				   int nMaxFileTitle)
{
	OPENFILENAME ofn;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hOwner;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = pcszFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = pszFilePath;
	ofn.nMaxFile = nMaxFilePath;
	ofn.lpstrFileTitle = pszFileTitle;
	ofn.nMaxFileTitle = nMaxFileTitle;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = pcszTitle;
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	BOOL bResult = 0;
	if(bIsSave)
		bResult = GetSaveFileName(&ofn);
	else
		bResult = GetOpenFileName(&ofn);
	return bResult;
}



LRESULT WINAPI FullScreenWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	VPlayerXunBo* player;
	if (uMsg==WM_CREATE)
	{
		player = (VPlayerXunBo*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)player);
	}
	else
	{
		player = (VPlayerXunBo*)GetWindowLongPtr(hWnd, GWL_USERDATA);
	}
	if (player)
	{
		switch(uMsg)
		{
		case WM_CLOSE:
		{
			player->ExitFullScreen();
			return 0;
		}
		break;

		case WM_LBUTTONDBLCLK:
			player->ExitFullScreen();
			break;

		case WM_KEYDOWN:
			switch(LOWORD(wParam))
			{
			case VK_ESCAPE:
				player->ExitFullScreen();
				break;
			}
			break;
		}
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void VPlayerXunBo::EnterFullScreen()
{
	if(0)
	if(g_hFullScreenWnd == NULL)
	{
		// Register window class
		WNDCLASSEX wc;
		wc.cbSize = sizeof(wc);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = NULL;
		wc.lpfnWndProc = FullScreenWindowProc;
		wc.lpszClassName = _T("FullScreenContainer");
		wc.lpszMenuName = 0;
		wc.style = CS_DBLCLKS;

		if(!::RegisterClassEx(&wc))
		{
			int nError = GetLastError();
			if(nError != ERROR_CLASS_ALREADY_EXISTS)
				return;
		}

		int w = ::GetSystemMetrics(SM_CXSCREEN);
		int h = ::GetSystemMetrics(SM_CYSCREEN);
		g_hFullScreenWnd = ::CreateWindowEx(WS_EX_TOPMOST
			, wc.lpszClassName
			, _T(""), WS_POPUP
			, 0, 0, w, h,
			_hParent, NULL, NULL, this);
	}
	DWORD style = GetWindowLong(_hParent, GWL_STYLE);
	SetWindowLong(_hParent, GWL_STYLE
		, style|CS_DBLCLKS
	);
	style = GetWindowLong(_hParent, GWL_EXSTYLE);
	SetWindowLong(_hParent, GWL_EXSTYLE, style|WS_EX_TOPMOST);

	int w = ::GetSystemMetrics(SM_CXSCREEN);
	int h = ::GetSystemMetrics(SM_CYSCREEN);
	::SetWindowPos(_hParent, NULL, 0, 0, w, h, 0);
	::SetParent(_hWnd, _hParent);

	//::ShowWindow(g_hFullScreenWnd, SW_SHOW);
	//::SetParent(_hWnd, g_hFullScreenWnd);
	//
	//// Set Position
	//RECT rc;
	//::GetClientRect(g_hFullScreenWnd, &rc);
	//::SetWindowPos(_hWnd, NULL, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, 0);
}


void VPlayerXunBo::ExitFullScreen()
{
	::SetParent(_hWnd, g_hDialog);
	SetAPlayerWindowSize(g_hDialog);
	::ShowWindow(g_hFullScreenWnd, SW_HIDE);
}


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

	switch (id)
	{
	//case IDM_STOP:
	//	KillTimer(hwnd, 1);
	//	g_MyPlayer.Release();
	//	EndDialog(hwnd, id);
	//	break;

	case IDM_OPEN:
		//g_MyPlayer.OpenLocalFile();
		break;

	case IDM_PAUSE:
		//if (g_MyPlayer.m_pAPlayer != NULL)
		//{
		//	if (g_MyPlayer.m_pAPlayer->GetState() == PS_PLAY)
		//	{
		//		g_MyPlayer.m_pAPlayer->Pause();
		//		//SetWindowText(GetDlgItem(hwnd, IDPAUSE), _T("播放"));
		//	}
		//	else if (g_MyPlayer.m_pAPlayer->GetState() == PS_PAUSED)
		//	{
		//		g_MyPlayer.m_pAPlayer->Play();
		//		//SetWindowText(GetDlgItem(hwnd, IDPAUSE), _T("暂停"));
		//	}
		//}
		break;

	case IDM_STOP:
		break;

	//case IDFULLSCREEN:
	//	::EnterFullScreen();
	//	break;
	//
	//case IDABOUT:
	//	MessageBox(hwnd, _T("作者：hehaifeng@xunlei.com"), _T("关于"), MB_ICONINFORMATION);
	//	break;
	}
}



void WINAPI Dlg_OnTimer(HWND hwnd, UINT id)
{
	//if (id != 1 || g_MyPlayer.m_pAPlayer == NULL)
	//{
	//	return;
	//}
	//
	//if (g_MyPlayer.m_pAPlayer->GetState() == PS_PLAY || 
	//	g_MyPlayer.m_pAPlayer->GetState() == PS_PAUSED)
	//{
	//	g_MyPlayer.m_nPosition = g_MyPlayer.m_pAPlayer->GetPosition();
	//
	//	TCHAR szPosition[64];
	//	TCHAR szDuration[64];
	//	g_MyPlayer.MillisecondToText(g_MyPlayer.m_nPosition, szPosition);
	//	g_MyPlayer.MillisecondToText(g_MyPlayer.m_nDuration, szDuration);
	//
	//	lstrcat(szPosition, _T("/"));
	//	lstrcat(szPosition, szDuration);
	//
	//	int nPos =  int (g_MyPlayer.m_nPosition / (double)g_MyPlayer.m_nDuration * 1000);
	//	//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nPos);
	//	//SetWindowText(GetDlgItem(hwnd, IDC_STATIC1), szPosition);
	//}
}

void Dlg_OnHScroll(HWND hwnd, HWND hwndCtl, WPARAM wParam, LPARAM lParam)
{
	//if (g_MyPlayer.m_pAPlayer != NULL)
	//{
	//	if (g_MyPlayer.m_pAPlayer->GetState() == PS_PLAY || 
	//		g_MyPlayer.m_pAPlayer->GetState() == PS_PAUSED)
	//	{
	//		//int nPos = (int)SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
	//		//nPos = int (nPos / 1000.0 * g_MyPlayer.m_nDuration);
	//		//g_MyPlayer.m_pAPlayer->SetPosition(nPos);
	//	}
	//}

	return;
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

		chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
		chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
		chHANDLE_DLGMSG(hwnd, WM_TIMER,      Dlg_OnTimer);
		chHANDLE_DLGMSG(hwnd, WM_HSCROLL,    Dlg_OnHScroll);
	}

	return(FALSE);
}

CComModule _module;

VPlayerXunBo::VPlayerXunBo(HINSTANCE hInstance, HWND hParent)
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	_module.Init(NULL, hInstance);

	AtlAxWinInit();

	_hParent = hParent;
	//g_hDialog = hwnd;

	//chSETDLGICONS(hwnd, IDI_ICON1);
	//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 1000));

	SetTimer(hParent, 1, 1000, NULL);

	g_hTextWnd = hParent;
	Initialize();
	if (CreateAPlayerWindow())
	{
		SetAPlayerWindowSize(hParent);
	}
	else
	{
		//HWND hWnd = GetDlgItem(hwnd, IDC_STATIC1);
		//SetWindowText(hWnd, _T("加载APlayer失败！"));
	}


	//DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_PLAYER), 
	//				NULL, Dlg_Proc, _ttoi(lpCmdLine));

}