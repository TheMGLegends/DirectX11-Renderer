#pragma once

#include "BaseBindable.h"

class InputLayout : public BaseBindable
{
public:
	InputLayout(Renderer& renderer,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* vertexShaderBytecode);
	void Bind(Renderer& renderer) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

