#pragma once

#include "../Generics/MinimalWindows.h"
#include <d3d11.h>

class Renderer
{
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

