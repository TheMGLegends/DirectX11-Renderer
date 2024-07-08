#include "Window/Window.h"

#include <iostream>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		// INFO: Set the Window Info (Centred by Default)
		Window::Info windowInfo{ 800, 600 };

		// INFO: Create the Window
		Window window(L"DirectX11 Renderer", windowInfo);

		// INFO: Setup Console for Debugging
		window.SetupConsole();

		MSG msg;
		BOOL result;

		// INFO: Pump Message Events
		while (result = (GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg); // INFO: Provides Translation for WM_CHAR Messages (One of the Jobs)
			DispatchMessage(&msg);

			if (window.keyboard.GetKeyHeld(0x41))
				std::cout << "A Held\n";

			if (window.keyboard.GetKeyPressed(0x41))
				std::cout << "A Pressed\n";

			if (window.keyboard.GetKeyReleased(0x41))
				std::cout << "A Released\n";
		}

		// INFO: Error Checking for GetMessage
		if (result == -1)
			throw WINDOW_THROW_LAST_EXCEPTION();

		return static_cast<int>(msg.wParam);
	}
	catch (const BaseException& exception)
	{
		std::string what = exception.what();
		std::wstring wWhatStr = std::wstring(what.begin(), what.end());
		const wchar_t* wWhat = wWhatStr.c_str();

		std::string type = exception.GetType();
		std::wstring wTypeStr = std::wstring(type.begin(), type.end());
		const wchar_t* wType = wTypeStr.c_str();

		MessageBox(nullptr, wWhat, wType, MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& exception)
	{
		std::string what = exception.what();
		std::wstring wWhatStr = std::wstring(what.begin(), what.end());
		const wchar_t* wWhat = wWhatStr.c_str();

		MessageBox(nullptr, wWhat, L"Standard Exception", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, L"Unknown Exception", L"Unknown Exception", MB_OK | MB_ICONERROR);
	}

	return -1;
}