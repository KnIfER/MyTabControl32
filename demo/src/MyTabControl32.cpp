#include <windows.h>
#include "Frame.h"
#include "TabLayout.h"
#include "MyTabControl32.h"
#include <iostream>

struct DemoData
{
	const TCHAR* title;
	int image;
};

DemoData demoData[]{
	{L"happy", 0}
	,{L"happy for what", 1}
	,{L"sad", 0}
	,{L"sad for what", 1}
	,{L"Values，何为价值观", 2}
	,{L"成功秘诀.pdf", 3}
	,{L"happy", 0}
	,{L"happy", 0}
	,{L"SecretToHappiness.pdf", 4}
	,{L"MasterCPP.pdf", 4}
	,{L"happy", 0}
	,{L"happy", 0}
	,{L"Your photo.png", 5}
	,{L"happy", 0}
	,{L"happy", 0}
};

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

	XPlusWindow* app = new XPlusWindow(hInstance);

	app->showWindow();


	if (app->_tabLayout)
	{
		for (size_t i = 0; i < 15; i++)
		{
			DemoData & dd = demoData[i];
			app->_tabLayout
				->addTab(dd.title, dd.image);
		}

		TCHAR buffer[64];
		for (size_t i = 0; i < 80; i++)
		{
			swprintf_s(buffer, L"happy#%d", i);
			app->_tabLayout->addTab(buffer, 0);
		}
	}

	SendMessage(app->getHWND(), WM_SIZE, 0, 0);


	MSG    msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return msg.wParam;
}
