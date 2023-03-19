#include "CompanionAI.h"
#include "pch.h"

CompanionAI::CompanionAI(ID3D12Device* device)
	: m_position(0.0f,0.0f,0.f), m_rotation(0.0f), m_scale(1.0f)
{
	// Create the Constant Buffer

	const UINT constantBufferSize = sizeof(ConstantBuffer);
	DX::ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // Upload heap
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer
		nullptr,
		IID_PPV_ARGS(&m_constantBuffer));

	// Map the Constant Buffer

	DX::ThrowIfFailed(m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer)));


}	

void CompanionAI::Update(float deltatime, const DirectX::XMFLOAT3& playerPosition)
{
}

void CompanionAI::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* cbvHeap, UINT cbvOffset)
{
}
