#include "Sprite.h"

#include <DirectXHelpers.h>
#include <DirectXTex/DirectXTex.h>

#include "Renderer.h"
#include "d3dx12.h"
#include "pch.h"
#include "d3d12.h"
#include <filesystem>
#include <wincodec.h>
#include <rapidjson/rapidjson.h>
#include "DXSample.h"

#include "Game.h"

Sprite::Sprite(ID3D12Device* device, const wchar_t* textureFileName)
{
    // Create vertex buffer
    CreateVertexBuffer(device);

    // Create index buffer
    CreateIndexBuffer(device);

    // Create texture
    CreateTexture(device, textureFileName);

    // Create constant buffer
    const UINT constantBufferSize = sizeof(ConstantBuffer);
    DX::ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(m_constantBuffer.GetAddressOf())));
    DX::ThrowIfFailed(m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer)));
    ZeroMemory(m_mappedConstantBuffer, constantBufferSize);
    m_mappedConstantBuffer->Model = XMMatrixIdentity();
    m_mappedConstantBuffer->Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Sprite::Render(ID3D12GraphicsCommandList* commandList)
{
    // Set the vertex buffer
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

    // Set the index buffer
    commandList->IASetIndexBuffer(&m_indexBufferView);

    // Set the primitive topology
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set the descriptor heap and root signature
    ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap.Get() };
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    commandList->SetGraphicsRootSignature(g_rootSignature.Get());

    // Set the pipeline state
    commandList->SetPipelineState(g_pipelineState.Get());

    // Set the shader resource view
    CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
    commandList->SetGraphicsRootDescriptorTable(0, srvHandle);

    // Set the constant buffer
    commandList->SetGraphicsRootConstantBufferView(1, m_constantBuffer->GetGPUVirtualAddress());

    // Draw the sprite
    commandList->DrawIndexedInstanced(m_indexBufferSize / sizeof(UINT), 1, 0, 0, 0);
}

void Sprite::UpdateConstantBuffer(const XMFLOAT4X4& viewProjectionMatrix, const XMFLOAT2& position, const XMFLOAT2& size, const XMFLOAT4& color)
{

}

void Sprite::CreateVertexBuffer(ID3D12Device* device)
{
}

void Sprite::CreateIndexBuffer(ID3D12Device* device)
{
}

void Sprite::CreateTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const wchar_t* fileName)
{
   
}

void Sprite::LoadTexture(ID3D12Device* device, const std::wstring& spritePath)
{
  
}

void Sprite::CreateSRV(ID3D12Device* device)
{
}

void Sprite::SetTexRect(const RECTF& texRect)
{
    mTexRect = texRect;
}





//Sprite::Sprite(ID3D12Device* m_device)
//{
//
//}
//
//Sprite::~Sprite()
//{
//}
//
//bool Sprite::LoadFromFile(const std::wstring& fileName)
//{
//    
//    
//}
//
//void Sprite::SetTexture(ID3D12Resource* texture)
//{
//    // Load DDS texture using DirectXTex library
//    DirectX::TexMetadata metadata;
//    DirectX::ScratchImage scratchImage;
//    HRESULT hr = DirectX::LoadFromDDSFile(filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);
//    if (FAILED(hr))
//    {
//        throw std::exception("Failed to load DDS texture.");
//    }
//
//    // Create texture resource
//    Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
//    hr = m_device->CreateCommittedResource(
//        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
//        D3D12_HEAP_FLAG_NONE,
//        &CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, metadata.height, 1, 1, metadata.arraySize, metadata.mipLevels, D3D12_RESOURCE_FLAG_NONE),
//        D3D12_RESOURCE_STATE_COPY_DEST,
//        nullptr,
//        IID_PPV_ARGS(textureResource.GetAddressOf()));
//    if (FAILED(hr))
//    {
//        throw std::exception("Failed to create texture resource.");
//    }
//
//    // Upload texture data to texture resource
//    Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadHeap;
//    hr = m_device->CreateCommittedResource(
//        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//        D3D12_HEAP_FLAG_NONE,
//        &CD3DX12_RESOURCE_DESC::Buffer(scratchImage.GetPixelsSize()),
//        D3D12_RESOURCE_STATE_GENERIC_READ,
//        nullptr,
//        IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));
//    if (FAILED(hr))
//    {
//        throw std::exception("Failed to create texture upload heap.");
//    }
//
//    D3D12_SUBRESOURCE_DATA textureData = {};
//    textureData.pData = scratchImage.GetPixels();
//    textureData.RowPitch = scratchImage.GetImages()->rowPitch;
//    textureData.SlicePitch = scratchImage.GetImages()->slicePitch;
//
//    UpdateSubresources(m_commandList, textureResource.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
//
//    // Transition texture resource to shader resource state
//    CD3DX12_RESOURCE_BARRIER textureBarrier = CD3DX12_RESOURCE_BARRIER::Transition(textureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
//    m_commandList->ResourceBarrier(1, &textureBarrier);
//
//    // Create shader resource view (SRV)
//    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(m_srvHeap->GetCPUDescriptorHandleForHeapStart(), m_srvIndex, m_srvDescriptorSize);
//    m_device->CreateShaderResourceView(textureResource.Get(), nullptr, srvHandle);
//
//    // Set sprite texture index
//    m_textureIndex = m_srvIndex;
//
//    // Increment SRV index
//    m_srvIndex++;
//
//    /////////////////////////////////////////////////////////////////
//    // Get shader visible descriptor handle
//    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());
//    srvHandle.Offset(m_textureIndex, m_rtvDescriptorSize /*descriptorSize*/);
//
//    // Create shader resource view
//    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//    srvDesc.Format = m_textureFormat;
//    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//    srvDesc.Texture2D.MipLevels = 1;
//    m_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, srvHandle);
//
//    // Set descriptor table
//    m_DescriptorTable = srvHandle;
//}
//
//void Sprite::setPosition(float x, float y)
//{
//    m_position = DirectX::XMFLOAT2(x, y);
//  
//}
//
//
//void Sprite::SetScale(float x, float y)
//{
//
//}
//
//void Sprite::SetRotation(float angle)
//{
//    
//}
//
//void Sprite::SetTransform(const DirectX::XMFLOAT2& position, float rotation, const DirectX::XMFLOAT2& scale)
//{
//    m_position = position;
//    m_rotation = rotation;
//    m_scale = scale;
//}
//
//void Sprite::Update(float deltaTime)
//{
//    m_position.x += m_velocity.x * deltaTime;
//    m_position.y += m_velocity.y * deltaTime;
//}
////
//D3D12_GPU_DESCRIPTOR_HANDLE Sprite::GetSRV()
//{
//    CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
//    srvHandle.Offset(m_srvIndex, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
//    return srvHandle;
//}
//
//void Sprite::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap)
//{
//    // Set the pipeline state object.
//    commandList->SetPipelineState(m_pipelineState.Get());
//
//    // Set the vertex buffer.
//    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
//
//    // Set the index buffer.
//    commandList->IASetIndexBuffer(&m_indexBufferView);
//
//    // Set the constant buffer.
//    commandList->SetGraphicsRootDescriptorTable(0, descriptorHeap->GetGPUDescriptorHandleForHeapStart());
//
//    // Set the texture.
//    commandList->SetGraphicsRootDescriptorTable(1, m_texture->GetSRV());
//
//    // Set the transform.
//    DirectX::XMFLOAT4X4 transform;
//    DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixScaling(m_scale.x, m_scale.y, 1.0f) * DirectX::XMMatrixRotationZ(m_rotation) * DirectX::XMMatrixTranslation(m_position.x, m_position.y, 0.0f));
//    commandList->SetGraphicsRoot32BitConstants(2, sizeof(DirectX::XMFLOAT4X4) / sizeof(uint32_t), &transform, 0);
//
//    // Draw the sprite.
//    commandList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
//
//}
//void Sprite::SetTexRect(const RECTF& texRect)
//{
//    mTexRect = texRect;
//}
//// test
//
//void Sprite::CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
//{
//    // Define the vertices for the sprite
//    std::vector<SpriteVertex> vertices =
//    {
//        { { m_position.x, m_position.y, 0.0f }, { 0.0f, 0.0f }, m_color },
//        { { m_position.x + m_size.x, m_position.y, 0.0f }, { 1.0f, 0.0f }, m_color },
//        { { m_position.x + m_size.x, m_position.y + m_size.y, 0.0f }, { 1.0f, 1.0f }, m_color },
//        { { m_position.x, m_position.y + m_size.y, 0.0f }, { 0.0f, 1.0f }, m_color },
//    };
//
//    // Create the vertex buffer
//    const UINT vertexBufferSize = static_cast<UINT>(vertices.size() * sizeof(SpriteVertex));
//    DX::ThrowIfFailed(device->CreateCommittedResource(
//        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//        D3D12_HEAP_FLAG_NONE,
//        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
//        D3D12_RESOURCE_STATE_GENERIC_READ,
//        nullptr,
//        IID_PPV_ARGS(&m_vertexBuffer)));
//
//    // Copy the vertex data to the vertex buffer
//    UINT8* pVertexDataBegin;
//    CD3DX12_RANGE readRange(0, 0);
//    DX::ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
//    memcpy(pVertexDataBegin, vertices.data(), vertexBufferSize);
//    m_vertexBuffer->Unmap(0, nullptr);
//
//    // Initialize the vertex buffer view
//    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
//    m_vertexBufferView.StrideInBytes = sizeof(SpriteVertex);
//    m_vertexBufferView.SizeInBytes = vertexBufferSize;
//
//    // Update the command list with the necessary commands to set up the vertex buffer
//    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
//    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//}
//



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

