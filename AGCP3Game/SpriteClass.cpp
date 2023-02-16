#include "SpriteClass.h"



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
