#include "Renderer.h"

#include <sstream>

#include "../Dxerr/dxerr.h"

#pragma comment(lib, "d3d11.lib")

// INFO: Graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Renderer::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Renderer::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Renderer::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#else
#define GFX_EXCEPT(hr) Renderer::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

#pragma region Renderer
Renderer::Renderer(HWND hWnd) : device(nullptr), deviceContext(nullptr), swapChain(nullptr), renderTargetView(nullptr)
{
	// INFO: Swap Chain Description
	DXGI_SWAP_CHAIN_DESC swapChainDescription = {};
	swapChainDescription.BufferDesc.Width = 0;
	swapChainDescription.BufferDesc.Height = 0;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 1;
	swapChainDescription.OutputWindow = hWnd;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// INFO: For checking results of d3d functions
	HRESULT hr;

	// INFO: Create Device and Device Context
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	));

	// INFO: Get the Back Buffer
	ID3D11Resource* backBuffer = nullptr;
	GFX_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer)));

	// INFO: Create Render Target View
	GFX_THROW_INFO(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView));

	// INFO: Release the Back Buffer
	backBuffer->Release();
}

Renderer::~Renderer()
{
	if (renderTargetView != nullptr)
		renderTargetView->Release();

	if (swapChain != nullptr)
		swapChain->Release();

	if (deviceContext != nullptr)
		deviceContext->Release();

	if (device != nullptr)
		device->Release();
}

void Renderer::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = swapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(device->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void Renderer::ClearBuffer(float red, float green, float blue)
{
	const float colour[] = { red, green, blue, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView, colour);
}
#pragma endregion

#pragma region Renderer Exceptions
Renderer::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs)
	: Exception(line, file), hr(hr)
{
	// INFO: Merge all messages into a single string
	for (const std::string& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}

	// INFO: Remove the final newline if there is one
	if (!info.empty())
		info.pop_back();
}

const char* Renderer::HrException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}

	oss << GetOriginContext();
	whatBuffer = oss.str();

	return whatBuffer.c_str();
}

inline HRESULT Renderer::HrException::GetErrorCode() const
{
	return hr;
}

std::string Renderer::HrException::GetErrorString() const
{
	return DXGetErrorStringA(hr);
}

std::string Renderer::HrException::GetErrorDescription() const
{
	wchar_t buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf) / sizeof(wchar_t));
	std::string string((const char*)buf);
	return string;
}

std::string Renderer::HrException::GetErrorInfo() const
{
	return info;
}
#pragma endregion

#pragma region Renderer Device Removed Exception
const char* Renderer::DeviceRemovedException::GetType() const
{
	return "Renderer Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
#pragma endregion
