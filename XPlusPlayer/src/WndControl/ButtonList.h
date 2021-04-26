#pragma once
#include "WindowBase.h"

class ButtonList : public WindowBase 
{
public:
	ButtonList(HINSTANCE hInstance, HWND hParent);

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((ButtonList*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}

	void showWindow();


	BOOL AddButtons(UINT buttonCount, LPTBBUTTON pButtonInfoArray) ;


	int GetCommandAt(int index);

	void AddIcon(int id);
	void ReplaceIcon(int pos, int id);



protected:
	HIMAGELIST _hImgList;

	int size;
};



