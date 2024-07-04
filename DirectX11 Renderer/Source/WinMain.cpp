#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"Renderer";
	const wchar_t WINDOW_NAME[] = L"DirectX11 Renderer";

	// INFO: Register a Window Class
	WNDCLASSEX wc = {0};

	// INFO: Set the Window Class Attributes
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	// INFO: Create a Window Instance
	HWND hWnd = CreateWindowEx(
		0, CLASS_NAME, WINDOW_NAME, 
		WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 
		nullptr, nullptr, hInstance, nullptr
	);

	// INFO: Show the Window
	ShowWindow(hWnd, SW_SHOW);

	// INFO: Pump Message Events
	MSG msg = {0};
	BOOL result = 0;
	while (result = (GetMessage(&msg, nullptr, 0, 0)) > 0) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// INFO: Error Encountered
	if (result == -1)
		return -1;
	else
		return msg.wParam;
}