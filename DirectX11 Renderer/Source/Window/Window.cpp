#include "Window.h"

#include <sstream>
#include <string>
#include <iostream>

#include "../../Resources/resource.h"

#pragma region WindowClass
Window::WindowClass Window::WindowClass::windowClass;
const wchar_t* Window::WindowClass::windowClassName = L"DirectX11 Renderer";

Window::WindowClass::WindowClass() : hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };

	// INFO: Set the Window Class Attributes
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 256, 256, 0));
	wc.hCursor = static_cast<HCURSOR>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 28, 36, 0));
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetWindowClassName();
	wc.hIconSm = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetWindowClassName(), GetInstance());
}
#pragma endregion

#pragma region WindowException
const char* Window::Exception::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl << "Error Code: " << hResult << std::endl << "Description: " << GetErrorDescription() << std::endl << GetOriginContext();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
std::string Window::Exception::TranslateErrorCode(HRESULT hResult)
{
	char* pMessageBuffer = nullptr;

	// INFO: Get the Error Message from Windows and Free the Allocated Memory when Done
	DWORD nMessageLength = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hResult, 0, reinterpret_cast<LPSTR>(&pMessageBuffer), 0, nullptr
	);

	if (nMessageLength == 0)
		return "Unidentified Error Code";

	// INFO: Copy the Error Message so the Windows Allocated Memory can be Released
	std::string errorDescription = pMessageBuffer;

	LocalFree(pMessageBuffer);

	return errorDescription;
}
#pragma endregion

#pragma region Window
Window::Window(const wchar_t* windowName, Info _windowInfo)
{
	windowInfo = _windowInfo;

	// INFO: Calculate Window Size based on Client Size
	RECT rect{};
	rect.right = windowInfo.width + rect.left;
	rect.bottom = windowInfo.height + rect.top;

	// INFO: Error Checking for Window Size Adjustment
	if (AdjustWindowRect(&rect, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, FALSE) == 0)
		throw WINDOW_THROW_LAST_EXCEPTION();

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

	// INFO: Error Checking for Window Creation
	if (hWnd == nullptr)
		throw WINDOW_THROW_LAST_EXCEPTION();

	// INFO: Show the Window
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	// INFO: Create the Graphics Renderer
	renderer = std::make_unique<Renderer>(hWnd);
}

Window::Window(const wchar_t* windowName, int width, int height, int x, int y) : Window(windowName, Info(width, height, x, y)) { }

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

void Window::SetupConsole()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout); // Redirect stdout to console
	std::cout.clear(); // Clear error state
	std::cout.sync_with_stdio(); // Synchronize with C I/O
}

void Window::SetTitle(const std::string& title) const
{
	if (SetWindowText(hWnd, std::wstring(title.begin(), title.end()).c_str()) == 0)
		throw WINDOW_THROW_LAST_EXCEPTION();
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	// INFO: Peek Message doesn't block loop from executing
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return static_cast<int>(msg.wParam);

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
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
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_KILLFOCUS:
	{
		keyboard.ClearKeyStates();
		break;
	}

#pragma region KEYBOARD MESSAGES
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		// INFO: If the Previous Key State is False
		if (!(lParam & 0x40000000))
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		// INFO: If the Previous Key State is True
		else if ((lParam & 0x40000000))
			keyboard.OnKeyHeld(static_cast<unsigned char>(wParam));
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		// INFO: If the Previous Key State is True
		if ((lParam & 0x40000000))
			keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}

	case WM_CHAR:
	{
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	}
#pragma endregion

#pragma region MOUSE MESSAGES
	case WM_MOUSEMOVE:
	{
		const POINTS point = MAKEPOINTS(lParam);
		// INFO: If in client window log move, and log enter + capture mouse (if not previously in window)
		if (point.x >= 0 && point.x < windowInfo.width && point.y >= 0 && point.y < windowInfo.height)
		{
			mouse.OnMouseMove(point.x, point.y);

			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// INFO: If not in client window but button is down log move
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(point.x, point.y);
			}
			// INFO: If button up, release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(point.x, point.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mouse.OnMiddlePressed(point.x, point.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mouse.OnRightPressed(point.x, point.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(point.x, point.y);

		// INFO: Release mouse if outside of window
		if (point.x < 0 || point.x >= windowInfo.width || point.y < 0 || point.y >= windowInfo.height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mouse.OnMiddleReleased(point.x, point.y);

		// INFO: Release mouse if outside of window
		if (point.x < 0 || point.x >= windowInfo.width || point.y < 0 || point.y >= windowInfo.height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mouse.OnRightReleased(point.x, point.y);

		// INFO: Release mouse if outside of window
		if (point.x < 0 || point.x >= windowInfo.width || point.y < 0 || point.y >= windowInfo.height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
#pragma endregion

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
#pragma endregion
