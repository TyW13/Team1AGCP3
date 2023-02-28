#include "stdafx.h"

#include "Sprite.h"
#include "DDSTextureLoader.h"
#include "Texture.h"
#include "ResourceUploadBatch.h"

void Sprite::LoadFromFile(const std::wstring& filename, Renderer* renderer)
{
    m_texture = std::make_unique<Texture>(renderer);
    m_texture->LoadFromFile(filename);
}

void Sprite::LoadTexture(ID3D12Device* device, const wchar_t* filename)
{

    m_texture.LoadFromFile(device, filename);
    
}

void Sprite::SetTexture(Texture* texture)
{
    m_texture = std::unique_ptr<Texture>(texture);
}

void Sprite::SetTransform(const XMFLOAT2& position, float rotation, const XMFLOAT2& scale)
{
    m_position = position;
    m_rotation = rotation;
    m_scale = scale;
}


void Sprite::Draw(Renderer& renderer)
{
    // Bind the sprite's texture to the pipeline.
    renderer.SetTexture(m_texture);

    // Set the sprite's transform.
    XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
    float rotation = 0.0f;
    renderer.SetTransform(position, scale, rotation);

    // Draw the sprite.
    renderer.Draw(m_rect);
}

//Sprite::Sprite()
//    : m_texture(nullptr), m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_size(XMFLOAT2(0.0f, 0.0f))
//{
//    XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
//}
//
//Sprite::~Sprite()
//{
//    if (m_texture)
//    {
//        m_texture->Release();
//        m_texture = nullptr;
//    }
//
//    if (m_vertexBuffer)
//    {
//        m_vertexBuffer->Release();
//        m_vertexBuffer = nullptr;
//    }
//
//    if (m_indexBuffer)
//    {
//        m_indexBuffer->Release();
//        m_indexBuffer = nullptr;
//    }
//}
//
//bool Sprite::LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const std::wstring& fileName)
//{
//  
//    HRESULT hr = CreateDDSTextureFromFile(device, commandList, fileName.c_str(), &m_texture, &m_textureDescriptorHandle);
//    if (FAILED(hr))
//    {
//        return false;
//    }
//
//    return true;
//}
//
//void Sprite::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* heap, UINT descriptorSize, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState)
//{
//    // Set pipeline state and root signature
//    commandList->SetPipelineState(pipelineState);
//    commandList->SetGraphicsRootSignature(rootSignature);
//
//    // Set texture descriptor heap
//    ID3D12DescriptorHeap* descriptorHeaps[] = { heap };
//    commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
//    commandList->SetGraphicsRootDescriptorTable(0, m_textureDescriptorHandle);
//
//    // Set vertex and index buffers
//    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
//    commandList->IASetIndexBuffer(&m_indexBufferView);
//
//    // Set primitive topology
//    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    // Update constant buffer
//    XMMATRIX world = XMLoadFloat4x4(&m_transform);
//    XMMATRIX view = XMMatrixIdentity;
//
//
//}
//
//void Sprite::SetPosition(float x, float y)
//{
//}
//
//void Sprite::InitializeVertexBuffer()
//{
//}
//
//void Sprite::InitializeIndexBuffer()
//{
//}
