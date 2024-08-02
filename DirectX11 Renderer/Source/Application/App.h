#pragma once

#include "../Window/Window.h"

class App
{
public:
	App(const wchar_t* windowName, Window::Info _windowInfo) : window(windowName, _windowInfo) {}
	App(const wchar_t* windowName, int width, int height, int x = CW_USECENTRE, int y = CW_USECENTRE) : window(windowName, width, height, x, y) {}

	int SystemUpdate();
	void CustomUpdate();

private:
	Window window;
};

