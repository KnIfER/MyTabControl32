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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

	Frame* app = new Frame(hInstance);

	app->showWindow();

	for (size_t i = 0; i < 15; i++)
	{
		DemoData & dd = demoData[i];
		app->tabLayout
			->addTab(dd.title, dd.image);
	}

	TCHAR buffer[64];
	for (size_t i = 0; i < 80; i++)
	{
		swprintf_s(buffer, L"happy#%d", i);
		app->tabLayout->addTab(buffer, 0);
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
