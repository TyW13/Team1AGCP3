#include "Sprite.h"

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

Sprite::Sprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const wchar_t* textureFileName)
    : m_commandList(commandList), mTexRect{}, scale{}, m_vertexBufferView{}, m_indexBufferView{}, m_indexBufferSize{}, m_mappedConstantBuffer{}
{
    // Initialize the vertex buffer and index buffer
    CreateVertexBuffer(device);
    CreateIndexBuffer(device);

    // Create texture resource and descriptor heap
    CreateTexture(device, commandList, textureFileName);
    CreateSRV(device);

    // Create constant buffer
    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ConstantBuffer));
    DX::ThrowIfFailed(device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)
    ));

    // Map the constant buffer
    DX::ThrowIfFailed(m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer)));

    // Initialize the constant buffer with default values
    XMFLOAT4X4 identity;
    XMStoreFloat4x4(&identity, XMMatrixIdentity());

    m_mappedConstantBuffer->Model = identity;
    m_mappedConstantBuffer->ViewProjection = identity;
    m_mappedConstantBuffer->Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

}

Sprite::~Sprite()
{
    if (m_constantBuffer)
    {
        m_constantBuffer->Unmap(0, nullptr);
    }
}

// Responsible for setting the vertex buffer, index buffer, primitive topology,
// Descriptor heap, root signature, pipline state, shader resource view and constant buffer,
// Calls "DrawindexedInstanced" to render the sprite on screen

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

// Update the constant buffer with the given view-projection matrix, position, size, and color

void Sprite::UpdateConstantBuffer(const XMFLOAT4X4& viewProjectionMatrix, const XMFLOAT2& position, const XMFLOAT2& size, const XMFLOAT4& color)
{

    XMMATRIX model = XMMatrixTranslation(position.x, position.y, 0.0f) * XMMatrixScaling(size.x, size.y, 1.0f);

    XMStoreFloat4x4(&m_mappedConstantBuffer->Model, XMMatrixTranspose(model));
    XMStoreFloat4x4(&m_mappedConstantBuffer->ViewProjection, XMMatrixTranspose(XMLoadFloat4x4(&viewProjectionMatrix)));
    m_mappedConstantBuffer->Color = color;

}



void Sprite::CreateVertexBuffer(ID3D12Device* device)
{

    // Define the vertex data for the sprite
    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT2 texCoord;
    };

    // Define the vertex buffer data layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, position), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texCoord), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA },
    };

    Vertex vertices[] =
    {
        { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT3(-0.5f,  0.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(0.5f,  0.5f, 0.0f), XMFLOAT2(1.0f, 0.0f) }
    };

    const UINT vertexBufferSize = sizeof(vertices);

    // Create the vertex buffer
    DX::ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)));

    // Create the upload heap for the vertex buffer
    DX::ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBufferUploadHeap)));

    // Copy the vertex data to the upload heap
    D3D12_SUBRESOURCE_DATA vertexData = {};
    vertexData.pData = reinterpret_cast<BYTE*>(vertices);
    vertexData.RowPitch = vertexBufferSize;
    vertexData.SlicePitch = vertexBufferSize;

    UpdateSubresources(m_commandList, m_vertexBuffer.Get(), m_vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);

    // Describe and create a vertex buffer view
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
}

void Sprite::CreateIndexBuffer(ID3D12Device* device)
{
    // Define the indices for a square (two triangles)
    unsigned int indices[] =
    {
        // Triangle 1
        0, 1, 2,
        // Triangle 2
        0, 2, 3
    };

    m_indexBufferSize = sizeof(indices);

    // Create the index buffer
    DX::ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(m_indexBufferSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_indexBuffer)));

    // Create the upload heap for the index buffer
    DX::ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(m_indexBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_indexBufferUploadHeap)));

    // Describe the data we want to copy to the index buffer
    D3D12_SUBRESOURCE_DATA indexData = {};
    indexData.pData = reinterpret_cast<BYTE*>(indices);
    indexData.RowPitch = m_indexBufferSize;
    indexData.SlicePitch = m_indexBufferSize;

    // Copy the data to the upload heap
    UpdateSubresources(m_commandList, m_indexBuffer.Get(), m_indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
    // Transition the index buffer to the vertex buffer state
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

    // Initialize the index buffer view
    m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
    m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    m_indexBufferView.SizeInBytes = m_indexBufferSize;
}

UINT Sprite::GetBytesPerPixel(DXGI_FORMAT format)
{
    switch (format)
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return 4;
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        return 4;
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        return 4;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return 4;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return 16;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
        return 8;
    default:
        throw std::exception("Unsupported format");

    }
}

void Sprite::CreateTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const wchar_t* fileName)
{
    // Load texture from file
    std::wstring spritePath(fileName);
    m_texture = LoadTexture(device, spritePath);

    // Create texture upload heap
    const UINT subresourceCount = m_texture->GetDesc().MipLevels;
    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, subresourceCount);

    DX::ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_textureUploadHeap)));

    // Copy texture data to upload heap
    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.RowPitch = m_texture->GetDesc().Width * GetBytesPerPixel(m_texture->GetDesc().Format);
    textureData.SlicePitch = textureData.RowPitch * m_texture->GetDesc().Height;
    UINT8* textureDataBegin = nullptr;
    DX::ThrowIfFailed(m_textureUploadHeap->Map(0, nullptr, reinterpret_cast<void**>(&textureDataBegin)));
    UpdateSubresources(commandList, m_texture.Get(), m_textureUploadHeap.Get(), 0, 0, subresourceCount, &textureData);
    m_textureUploadHeap->Unmap(0, nullptr);

    // Transition texture to shader resource state
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_texture.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    commandList->ResourceBarrier(1, &barrier);

  

}



ComPtr<ID3D12Resource> Sprite::LoadTexture(ID3D12Device* device, const std::wstring& filePath)
{
    // Load texture from file
    ComPtr<ID3D12Resource> texture = nullptr;
    ComPtr<ID3D12Resource> textureUploadHeap = nullptr;
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;

    HRESULT hr = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to load texture from file.");
    }

    const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

    // Create texture
    const CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
    const CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        metadata.format,
        metadata.width,
        metadata.height,
        1, // Array size
        static_cast<UINT16>(metadata.mipLevels),
        1, // Sample count
        0, // Sample quality
        D3D12_RESOURCE_FLAG_NONE,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        0 // Alignment
    );

    DX::ThrowIfFailed(device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&texture)));

    // Create texture upload heap
    const UINT subresourceCount = textureDesc.DepthOrArraySize * textureDesc.MipLevels;
    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, subresourceCount);

    DX::ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&textureUploadHeap)));

    // Copy texture data to upload heap
    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = image->pixels;
    textureData.RowPitch = metadata.width * GetBytesPerPixel(metadata.format);
    textureData.SlicePitch = textureData.RowPitch * metadata.height;

    UpdateSubresources(nullptr, texture.Get(), textureUploadHeap.Get(), 0, 0, subresourceCount, &textureData);

    // Transition texture to shader resource state
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        texture.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    // Create shader resource view (SRV)
    CreateSRV(device);

    return texture;

    // Now we can load a texture by passing its file patch to the 'LoadTexture' method
    // ComPtr<ID3D12Resource> textureResource = LoadTexture(device, L"cat.dds");
}

void Sprite::CreateSRV(ID3D12Device* device)
{
    // Get descriptor heap from device
    ComPtr<ID3D12DescriptorHeap> descriptorHeap;
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = 1;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    DX::ThrowIfFailed(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap)));

    // Create shader resource view (SRV)
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = m_texture->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = m_texture->GetDesc().MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.PlaneSlice = 0;
    srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
    device->CreateShaderResourceView(m_texture.Get(), &srvDesc, descriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // Store descriptor heap and descriptor size
    m_descriptorHeap = descriptorHeap;
    m_descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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

