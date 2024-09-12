#pragma once

#include "BaseBindable.h"

class PixelShader : public BaseBindable
{
public:
	PixelShader(Renderer& renderer, const std::wstring& path);
	void Bind(Renderer& renderer) override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
};

