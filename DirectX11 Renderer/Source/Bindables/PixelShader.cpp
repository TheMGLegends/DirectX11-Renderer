#include "PixelShader.h"
#include "../Macros/RendererThrowMacros.h"

PixelShader::PixelShader(Renderer& renderer, const std::wstring& path)
{
	INFOMAN(renderer);

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &blob));
	GFX_THROW_INFO(GetDevice(renderer)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
}

void PixelShader::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->PSSetShader(pixelShader.Get(), nullptr, 0u);
}
