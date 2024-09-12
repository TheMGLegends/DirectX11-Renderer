#pragma once

#include "../Graphics/Renderer.h"

class BaseBindable
{
public:
	virtual void Bind(Renderer& renderer) = 0;
	virtual ~BaseBindable() = default; // Destructor must be virtual to allow for proper cleanup of derived classes when deleting a pointer to a base class.

protected:
	static ID3D11DeviceContext* GetContext(Renderer& renderer);
	static ID3D11Device* GetDevice(Renderer& renderer);
	static DxgiInfoManager& GetInfoManager(Renderer& renderer);
};