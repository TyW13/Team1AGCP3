

#include"stdafx.h"
#include "DDSTextureLoader.h"
#include "SpriteClass.h"
#include <stdexcept>


Sprite::Sprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
    : m_device(device), m_commandList(commandList), m_x(0.0f), m_y(0.0f), m_scaleX(1.0f), m_scaleY(1.0f)
{
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateConstantBuffer();
    CreateRootSignature();
    CreatePipelineState();
}

Sprite::~Sprite()
{
}

void Sprite::SetPosition(float x, float y)
{
    m_x = x;
    m_y = y;

    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixScaling(m_scaleX, m_scaleY, 1.0f) * DirectX::XMMatrixTranslation(m_x, m_y, 0.0f));
    m_commandList->SetGraphicsRoot32BitConstants(0, 16, &transform, 0);
}

void Sprite::SetScale(float x, float y)
{
    m_scaleX = x;
    m_scaleY = y;

    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixScaling(m_scaleX, m_scaleY, 1.0f) * DirectX::XMMatrixTranslation(m_x, m_y, 0.0f));
    m_commandList->SetGraphicsRoot32BitConstants(0, 16, &transform, 0);
}

void Sprite::SetTexture(ID3D12Resource* texture, D3D12_SRV_DIMENSION textureViewDimension)
{

}

void Sprite::Draw()
{
  
}



void Sprite::CreateVertexBuffer()
{
    // Create the vertex buffer
    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * 4);
    hr = device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create vertex buffer");
    }

    // Map the vertex buffer
    Vertex* vertices;
    CD3DX12_RANGE readRange(0, 0);
    hr = m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&vertices));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to map vertex buffer");
    }

    // Define the vertices of the sprite
    float left = static_cast<float>(-m_texture.GetMetadata().width) / 2.0f;
    float right = left + static_cast<float>(m_texture.GetMetadata().width);
    float top = static_cast<float>(m_texture.GetMetadata().height) / 2.0f;
    float bottom = top - static_cast<float>(m_texture.GetMetadata().height);
    vertices[0] = { DirectX::XMFLOAT3(left, top, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) };
    vertices[1] = { DirectX::XMFLOAT3(right, top, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) };
    vertices[2] = { DirectX::XMFLOAT3(left, bottom, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) };
    vertices[3] = { DirectX::XMFLOAT3(right, bottom, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) };

    // Unmap the vertex buffer
    m_vertexBuffer->Unmap(0, nullptr);

    // Create the vertex buffer view
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = sizeof(Vertex) * 4;
}

void Sprite::CreateIndexBuffer()
{

}

void Sprite::CreateConstantBuffer()
{

}

void Sprite::CreateRootSignature()
{

}

void Sprite::CreatePipelineState()
{

}
