#pragma once

#include "../Base Classes/Exceptions/BaseException.h"
#include "../Keyboard/Keyboard.h"
#include "../Mouse/Mouse.h"
#include "../Graphics/Renderer.h"

#include "../Generics/MinimalWindows.h"

#include <optional>
#include <memory>

// INFO: Custom Window Class Macros for Window Positioning
constexpr auto CW_USECENTRE = ((int)0x80000001);

// INFO: Custom Error Exception Macros for Window Class
#define WINDOW_THROW_EXCEPTION(hr) Window::Exception(__LINE__, __FILE__, hr)
#define WINDOW_THROW_LAST_EXCEPTION() Window::Exception(__LINE__, __FILE__, GetLastError())

class Window
{
private:
	// INFO: Handles the Registering and Unregistering (Cleanup) of Window Class
	class WindowClass
	{
	private:
		static WindowClass windowClass;
		static const wchar_t* windowClassName;
		HINSTANCE hInstance;

	public:
		static inline const wchar_t* GetWindowClassName() { return windowClassName; }
		static inline HINSTANCE GetInstance() { return windowClass.hInstance; }

	private:
		/// @brief Handles the Registering of Window Class
		WindowClass();

		/// @brief Handles the Unregistering (Cleanup) of Window Class
		~WindowClass();

		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	};

public:
	// INFO: Window Information Structure for Window Creation
	struct Info
	{
		int width;
		int height;
		int x;
		int y;

		Info() : width(800), height(600), x(CW_USECENTRE), y(CW_USECENTRE) {}

		Info(int _width, int _height, int _x = CW_USECENTRE, int _y = CW_USECENTRE)
			: width(_width), height(_height), x(_x), y(_y) {}
	};

	// INFO: Custom Window Exception Class for Error Handling
	class Exception : public BaseException
	{
	private:
		HRESULT hResult;

	public:
		inline Exception(int _line, const char* _file, HRESULT _hResult) : BaseException(_line, _file), hResult(_hResult) {}
		virtual const char* what() const override;
		static std::string TranslateErrorCode(HRESULT hResult);

		inline virtual const char* GetType() const override { return "Window Exception"; }
		inline HRESULT GetErrorCode() const { return hResult; }
		inline std::string GetErrorDescription() const { return TranslateErrorCode(hResult); }
	};

private:
	Info windowInfo;
	HWND hWnd;
	std::unique_ptr<Renderer> renderer;

public:
	Keyboard keyboard;
	Mouse mouse;

public:
	Window(const wchar_t* windowName, Info _windowInfo);
	Window(const wchar_t* windowName, int width, int height, int x = CW_USECENTRE, int y = CW_USECENTRE);
	~Window();

	void CentreWindow();
	void SetupConsole();
	void SetTitle(const std::string& title) const;

	static std::optional<int> ProcessMessages();

	inline Renderer& GetRenderer() { return *renderer; }

private:
	/// @brief Handles the Setup of Message Handling for the Window
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// @brief Handles the Repeated Message Handling for the Window
	static LRESULT CALLBACK HandleMessageRepeated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// @brief User Defined Message Handling for the Window
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

