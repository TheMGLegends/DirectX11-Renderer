#pragma once

// INFO: Custom Error Exception Macros for Window Class
#define WINDOW_THROW_EXCEPTION(hr) Window::Exception(__LINE__, __FILE__, hr)
#define WINDOW_THROW_LAST_EXCEPTION() Window::Exception(__LINE__, __FILE__, GetLastError())
#define WINDOW_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)