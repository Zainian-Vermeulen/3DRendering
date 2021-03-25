#pragma once
#include <Windows.h>

class Window
{
public:
	int init(HINSTANCE hInstance, INT width, INT height, int nCmdShow);
	bool run();

	HWND getWindowHandle() const { return _hWnd; }

private:
	HWND _hWnd;
};

