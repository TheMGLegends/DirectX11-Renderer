#include "Renderer.h"

#pragma comment(lib, "d3d11.lib")

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

	// INFO: Create Device and Device Context
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);

	// INFO: Get the Back Buffer
	ID3D11Resource* backBuffer = nullptr;
	swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));

	// INFO: Create Render Target View
	if (backBuffer != NULL)
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

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
	swapChain->Present(1u, 0u);
}

void Renderer::ClearBuffer(float red, float green, float blue)
{
	const float colour[] = { red, green, blue, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView, colour);
}
