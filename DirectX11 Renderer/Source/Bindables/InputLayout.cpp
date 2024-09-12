#include "InputLayout.h"
#include "../Macros/RendererThrowMacros.h"

InputLayout::InputLayout(Renderer& renderer,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	ID3DBlob* vertexShaderBytecode)
{
	INFOMAN(renderer);

	GFX_THROW_INFO(GetDevice(renderer)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		vertexShaderBytecode->GetBufferPointer(),
		vertexShaderBytecode->GetBufferSize(),
		&inputLayout
	));
}

void InputLayout::Bind(Renderer& renderer)
{
	GetContext(renderer)->IASetInputLayout(inputLayout.Get());
}
