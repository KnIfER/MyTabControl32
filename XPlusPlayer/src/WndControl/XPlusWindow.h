#pragma once
#include "WindowBase.h"
#include "TabLayout.h"
#include "ButtonList.h"
#include "SimplePlayer.h"
#include "SeekBar.h"

class XPlusWindow : public WindowBase 
{
public:
	XPlusWindow()=default;
	void init(HINSTANCE hInstance, HWND hParent);

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void showWindow();

	void Toggle();
	HWND GetMediaPlayerHWND();
	void ToggleFullScreen();
	bool isFullScreen();
	void MarkPlaying(bool playing=true);
	bool PickFile();

	TabLayout _tabLayout;
	ButtonList _toolbar;
	SeekBar _seekbar;
	VPlayerXunBo* mMediaPlayer0=NULL;
	int _barsHeight=10;
	HWND _hFullScreenBtmbar;
protected:
	WNDPROC _SysWndProc=NULL;
	bool _isPlaying;
	bool _isFullScreen=false;
};



