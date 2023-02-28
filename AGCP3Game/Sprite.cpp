#include "Sprite.h"

#include <DirectXHelpers.h>
#include <DirectXTex/DirectXTex.h>
#include "Renderer.h"
#include "d3dx12.h"
#include "pch.h"


bool Sprite::LoadFromFile(const std::wstring& fileName)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(m_device.Get(), fileName.c_str(), nullptr, m_texture.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        return false;
    }

    m_textureWidth = m_texture->GetDesc().Width;
    m_textureHeight = m_texture->GetDesc().Height;
    m_textureFormat = m_texture->GetDesc().Format;

    return true;
}

void Sprite::SetTexture(ID3D12Resource* texture)
{
    // Get shader visible descriptor handle
    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());
    srvHandle.Offset(m_textureIndex, descriptorSize);

    // Create shader resource view
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = m_TextureFormat;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    m_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, srvHandle);

    // Set descriptor table
    m_DescriptorTable = srvHandle;
}

void Sprite::SetTransform(const DirectX::XMFLOAT2& position, float rotation, const DirectX::XMFLOAT2& scale)
{
    m_position = position;
    m_rotation = rotation;
    m_scale = scale;
}

void Sprite::Update(float deltaTime)
{
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
}

D3D12_GPU_DESCRIPTOR_HANDLE Sprite::GetSRV()
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
    srvHandle.Offset(m_srvIndex, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
    return srvHandle;
}

void Sprite::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap)
{
    // Set the pipeline state object.
    commandList->SetPipelineState(m_pipelineState.Get());

    // Set the vertex buffer.
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

    // Set the index buffer.
    commandList->IASetIndexBuffer(&m_indexBufferView);

    // Set the constant buffer.
    commandList->SetGraphicsRootDescriptorTable(0, descriptorHeap->GetGPUDescriptorHandleForHeapStart());

    // Set the texture.
    commandList->SetGraphicsRootDescriptorTable(1, m_texture->GetSRV());

    // Set the transform.
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixScaling(m_scale.x, m_scale.y, 1.0f) * DirectX::XMMatrixRotationZ(m_Rotation) * DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, 0.0f));
    commandList->SetGraphicsRoot32BitConstants(2, sizeof(DirectX::XMFLOAT4X4) / sizeof(uint32_t), &transform, 0);

    // Draw the sprite.
    commandList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);

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

