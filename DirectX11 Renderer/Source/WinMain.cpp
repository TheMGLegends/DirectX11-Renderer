#include "Window/Window.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// INFO: Set the Window Info (Centred by Default)
	WindowInfo windowInfo(800, 600);

	// INFO: Create the Window
	Window window(L"DirectX11 Renderer", windowInfo);

	MSG msg;
	BOOL result;

	// INFO: Pump Message Events
	while (result = (GetMessage(&msg, nullptr, 0, 0)) > 0) 
	{
		TranslateMessage(&msg); // INFO: Provides Translation for WM_CHAR Messages (One of the Jobs)
		DispatchMessage(&msg);
	}

	// INFO: Error Encountered
	if (result == -1)
		return -1;
	else
		return static_cast<int>(msg.wParam);
}