#pragma once

#include <d3d11.h>
#include <vector>
#include <wrl.h>

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

	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};

public:
	Renderer(HWND hWnd);
	~Renderer() = default;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue);

	void DrawTestTriangle(float angle, float x, float y);

private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};

