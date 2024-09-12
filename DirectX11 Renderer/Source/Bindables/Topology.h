#pragma once

#include "BaseBindable.h"

class Topology : public BaseBindable
{
public:
	Topology(Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Renderer& renderer) override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};

