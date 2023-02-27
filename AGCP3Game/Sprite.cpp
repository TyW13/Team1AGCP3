#include "stdafx.h"

#include "Sprite.h"
#include "DDSTextureLoader.h"

#include "ResourceUploadBatch.h"

Sprite::Sprite()
    : m_texture(nullptr), m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_size(XMFLOAT2(0.0f, 0.0f))
{
    XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
}

Sprite::~Sprite()
{
    if (m_texture)
    {
        m_texture->Release();
        m_texture = nullptr;
    }

    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }

    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = nullptr;
    }
}

bool Sprite::LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const std::wstring& fileName)
{
  
    HRESULT hr = CreateDDSTextureFromFile(device, commandList, fileName.c_str(), &m_texture, &m_textureDescriptorHandle);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

void Sprite::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* heap, UINT descriptorSize, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState)
{
    // Set pipeline state and root signature
    commandList->SetPipelineState(pipelineState);
    commandList->SetGraphicsRootSignature(rootSignature);

    // Set texture descriptor heap
    ID3D12DescriptorHeap* descriptorHeaps[] = { heap };
    commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
    commandList->SetGraphicsRootDescriptorTable(0, m_textureDescriptorHandle);

    // Set vertex and index buffers
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    commandList->IASetIndexBuffer(&m_indexBufferView);

    // Set primitive topology
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Update constant buffer
    XMMATRIX world = XMLoadFloat4x4(&m_transform);
    XMMATRIX view = XMMatrixIdentity;


}

void Sprite::SetPosition(float x, float y)
{
}

void Sprite::InitializeVertexBuffer()
{
}

void Sprite::InitializeIndexBuffer()
{
}
