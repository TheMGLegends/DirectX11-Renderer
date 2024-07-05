#include "Window.h"

#include <string>

Window::WindowClass Window::WindowClass::windowClass;
const wchar_t* Window::WindowClass::windowClassName = L"DirectX11 Renderer";

Window::WindowClass::WindowClass()
{
	WNDCLASSEX wc = { 0 };

	// INFO: Set the Window Class Attributes
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetWindowClassName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
}

Window::Window(const wchar_t* windowName, WindowInfo _windowInfo)
{
	windowInfo = _windowInfo;

	// INFO: Calculate Window Size based on Client Size
	RECT rect{};
	rect.right = windowInfo.width + rect.left;
	rect.bottom = windowInfo.height + rect.top;

	AdjustWindowRect(&rect, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, FALSE);

	// INFO: Centre Window if Required
	if (windowInfo.x == CW_USECENTRE || windowInfo.y == CW_USECENTRE)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (windowInfo.x == CW_USECENTRE)
			windowInfo.x = (screenWidth - windowInfo.width) / 2;

		if (windowInfo.y == CW_USECENTRE)
			windowInfo.y = (screenHeight - windowInfo.height) / 2;
	}

	// INFO: Create the Window
	hWnd = CreateWindowEx(
		0, WindowClass::GetWindowClassName(), windowName,
		WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
		windowInfo.x, windowInfo.y, windowInfo.width, windowInfo.height,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// INFO: Show the Window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::Window(const wchar_t* windowName, int width, int height, int x, int y) : Window(windowName, WindowInfo(width, height, x, y)) { }

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::CentreWindow()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	windowInfo.x = (screenWidth - windowInfo.width) / 2;
	windowInfo.y = (screenHeight - windowInfo.height) / 2;

	SetWindowPos(hWnd, nullptr, windowInfo.x, windowInfo.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

LRESULT Window::HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		// INFO: Extract the Window Class from the Creation Data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// INFO: Store the Window Pointer in the User Data of the Window
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// INFO: Set the Message Procedure to Handle Message Repeated
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageRepeated));

		// INFO: Call the User Defined Message Handling
		return pWnd->HandleMessage(hWnd, uMsg, wParam, lParam);
	}

	// INFO: Default Message Handling for the Window
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessageRepeated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// INFO: Extract the Window Pointer from the User Data of the Window
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// INFO: Call the User Defined Message Handling
	return pWnd->HandleMessage(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	// INFO: Used for Key Presses (Player Movement with WASD etc.)
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		break;

	// INFO: Used for Character Input (Writing into a Text Box etc.)
	case WM_CHAR:
	{
		static std::string text;

		if (wParam == VK_BACK && !text.empty())
			text.pop_back();
		else
			text.push_back((char)wParam);

		SetWindowText(hWnd, std::wstring(text.begin(), text.end()).c_str());
	}
	break;

	// INFO: Used for Mouse Clicks (Selecting an Object etc.)
	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		std::string text = "X: " + std::to_string(pt.x) + " Y: " + std::to_string(pt.y);
		SetWindowText(hWnd, std::wstring(text.begin(), text.end()).c_str());
	}
	break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
