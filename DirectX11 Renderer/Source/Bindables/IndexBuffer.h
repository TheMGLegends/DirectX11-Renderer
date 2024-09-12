#pragma once

#include "BaseBindable.h"

class IndexBuffer : public BaseBindable
{
public:
	IndexBuffer(Renderer& renderer, const std::vector<unsigned short>& indices);
	void Bind(Renderer& renderer) override;
	UINT GetCount() const;

protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};

