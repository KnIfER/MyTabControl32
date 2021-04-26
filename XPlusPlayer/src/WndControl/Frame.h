#pragma once
#include "WindowBase.h"
#include "TabLayout.h"
#include "ButtonList.h"
#include "SimplePlayer.h"

class Frame : public WindowBase 
{
public:
	Frame(HINSTANCE hInst);

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void showWindow();

	void Toggle();
	HWND GetMediaPlayerHWND();

	TabLayout* tabLayout;
	ButtonList* toolbar;
	VPlayerXunBo* mMediaPlayer0=NULL;
protected:
	WNDPROC _SysWndProc=NULL;
	bool _isPlaying;
};



