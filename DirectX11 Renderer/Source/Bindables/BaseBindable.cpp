#include "BaseBindable.h"

ID3D11DeviceContext* BaseBindable::GetContext(Renderer& renderer)
{
	return renderer.deviceContext.Get();
}

ID3D11Device* BaseBindable::GetDevice(Renderer& renderer)
{
	return renderer.device.Get();
}

DxgiInfoManager& BaseBindable::GetInfoManager(Renderer& renderer)
{
#ifndef NDEBUG
	return renderer.infoManager;
#else
	throw std::logic_error("YouFuckedUp! (tried to access gfx.infoManager in Release config)");
#endif
}
