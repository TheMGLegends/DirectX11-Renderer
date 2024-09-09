#include "Renderer.h"

#include <sstream>
#include <d3dcompiler.h>

#include "../Dxerr/dxerr.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// INFO: Graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Renderer::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Renderer::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Renderer::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Renderer::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Renderer::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
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
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;
	GFX_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));

	// INFO: Create Render Target View
	GFX_THROW_INFO(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));
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
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), colour);
}

void Renderer::DrawTestTriangle()
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	HRESULT hr;

	struct Vertex
	{
		struct 
		{
			float x;
			float y;
		} position;

		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	Vertex vertices[] =
	{
		// INFO: Isosceles Triangle
		//{ 0.0f, 0.5f, 255, 0, 0, 0 },
		//{ 0.5f, -0.5f, 0, 255, 0, 0 },
		//{ -0.5f, -0.5f, 0, 0, 255, 0 }

		// INFO: Line List Triangle
		//{0.0f, 0.5f},
		//{0.5f, -0.5f},
		//
		//{0.5f, -0.5f},
		//{-0.5f, -0.5f},
		//
		//{-0.5f, -0.5f},
		//{0.0f, 0.5f}

		// INFO: Line Strip Triangle
		//{ 0.0f, 0.5f },
		//{ 0.5f, -0.5f },
		//{ -0.5f, -0.5f },
		//{ 0.0f, 0.5f }

		// INFO: Rectangle
		//{ -0.5f, 0.5f},
		//{ 0.5f, 0.5f},
		//{ 0.5f, -0.5f},
		//
		//{ 0.5f, -0.5f},
		//{ -0.5f, -0.5f},
		//{ -0.5f, 0.5f}

		// INFO: Hexagon
		{ 0.0f, 0.5f, 255, 0, 0, 0 },
		{ 0.5f, -0.5f, 0, 255, 0, 0 },
		{ -0.5f, -0.5f, 0, 0, 255, 0 },
		{ -0.3f, 0.3f, 0, 0, 255, 0 },
		{ 0.3f, 0.3f, 0, 255, 0, 0 },
		{ 0.0f, -1.8f, 255, 0, 0, 0 }
	};

	vertices[0].color.g = 255;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;

	GFX_THROW_INFO(device->CreateBuffer(&bufferDesc, &subresourceData, &vertexBuffer));

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	deviceContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	// INFO: Create Index Buffer
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		2, 1, 5
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	GFX_THROW_INFO(device->CreateBuffer(&ibd, &isd, &indexBuffer));

	// INFO: Bind Index Buffer
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;

	// INFO: Create Pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &shaderBlob));
	GFX_THROW_INFO(device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader));

	// INFO: Bind Pixel Shader
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0u);
	
	// INFO: Create Vertex Shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &shaderBlob));
	GFX_THROW_INFO(device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader));
	
	// INFO: Bind Vertex Shader
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

	// INFO: Create Input Layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	GFX_THROW_INFO(device->CreateInputLayout(ied, (UINT)std::size(ied), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout));

	// INFO: Bind Input Layout
	deviceContext->IASetInputLayout(inputLayout.Get());

	// INFO: Bind Render Target
	deviceContext->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), nullptr);

	// INFO: Set Primitive Topology (Triangle List)
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// INFO: Setup Viewport
	D3D11_VIEWPORT viewport = {};
	viewport.Width = 400;
	viewport.Height = 300;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 100;
	viewport.TopLeftY = 100;

	deviceContext->RSSetViewports(1u, &viewport);

	GFX_THROW_INFO_ONLY(deviceContext->DrawIndexed(std::size(indices), 0u, 0u));
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

#pragma region Renderer Info Exception
Renderer::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	: Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Renderer::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginContext();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Renderer::InfoException::GetType() const noexcept
{
	return "Graphics Info Exception";
}

std::string Renderer::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
#pragma endregion
