#pragma once

#include "BaseBindable.h"
#include "../Macros/RendererThrowMacros.h"

template<typename C>
class ConstantBuffer : public BaseBindable
{
public:
	void Update(Renderer& renderer, const C& consts)
	{
		INFOMAN(renderer);

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(GetContext(renderer)->Map(
			constantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		));
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(renderer)->Unmap(constantBuffer.Get(), 0u);
	}
	ConstantBuffer(Renderer& renderer, const C& consts)
	{
		INFOMAN(renderer);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(renderer)->CreateBuffer(&cbd, &csd, &constantBuffer));
	}
	ConstantBuffer(Renderer& renderer)
	{
		INFOMAN(renderer);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(renderer)->CreateBuffer(&cbd, nullptr, &constantBuffer));
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::constantBuffer;
	using BaseBindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer; // Inherit constructor from ConstantBuffer, without the need to redefine it.
	void Bind(Renderer& renderer) override
	{
		GetContext(renderer)->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::constantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer; // Inherit constructor from ConstantBuffer, without the need to redefine it.
	void Bind(Renderer& renderer) override
	{
		GetContext(renderer)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	}
};

