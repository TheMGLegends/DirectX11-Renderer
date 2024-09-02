#pragma once

#include <d3d11.h>

#include "../Generics/MinimalWindows.h"
#include "../Base Classes/Exceptions/BaseException.h"

class Renderer
{
public:
	// INFO: Custom Renderer Exception Classes for Error Handling
	class Exception : public BaseException
	{
		using BaseException::BaseException;
	};

	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs);
		const char* what() const override;
		const char* GetType() const override;
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
		std::string GetErrorDescription() const;
		std::string GetErrorInfo() const;
	};

public:
	Renderer(HWND hWnd);
	~Renderer();

	void EndFrame();
	void ClearBuffer(float red, float green, float blue);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
};

