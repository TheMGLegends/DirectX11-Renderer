#pragma once

#include <d3d11.h>
#include <vector>

#include "../Generics/MinimalWindows.h"
#include "../Base Classes/Exceptions/BaseException.h"
#include "../DXGI/DxgiInfoManager.h"

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
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {});

		const char* what() const override;

		inline virtual const char* GetType() const override { return "Renderer Exception"; }
		inline HRESULT GetErrorCode() const;
		inline std::string GetErrorString() const;
		std::string GetErrorDescription() const;
		std::string GetErrorInfo() const;

	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	public:
		const char* GetType() const override;

	private:
		std::string reason;
	};

public:
	Renderer(HWND hWnd);
	~Renderer();

	void EndFrame();
	void ClearBuffer(float red, float green, float blue);

private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
};

