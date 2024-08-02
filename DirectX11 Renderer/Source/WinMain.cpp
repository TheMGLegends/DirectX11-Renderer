#include "Application/App.h"

#include <iostream>
#include <sstream>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		// INFO: Set up Application
		App app(L"DirectX11 Renderer", 800, 600);

		return app.SystemUpdate();
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