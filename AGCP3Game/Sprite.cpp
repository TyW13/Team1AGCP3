#include "Sprite.h"

#include <DirectXHelpers.h>
#include <DirectXTex/DirectXTex.h>
#include "Renderer.h"
#include "d3dx12.h"
#include "pch.h"

Sprite::Sprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList) :
    m_device(device),
    m_texture(nullptr),
    m_constantBuffer(nullptr),
    m_mappedConstantBuffer(nullptr),
    m_rotation(0.0f),
    m_position(DirectX::XMFLOAT2(0.0f, 0.0f)),
    m_scale(DirectX::XMFLOAT2(1.0f, 1.0f)),
    m_transformMatrix(DirectX::XMMatrixIdentity())
{
    LoadFromFile(device, commandList, L"default.dds");
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateConstantBuffer(device);
    CreatePipelineState(device);
}


Sprite::~Sprite()
{
    if (m_texture)
        m_texture->Release();

    if (m_constantBuffer)
        m_constantBuffer->Release();
}

bool Sprite::LoadFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const std::wstring& fileName)
{
    std::unique_ptr<DirectX::ScratchImage> image = std::make_unique<DirectX::ScratchImage>();
    HRESULT hr = DirectX::LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *image);
    if (FAILED(hr))
        return false;

    hr = CreateShaderResourceView(device, image->GetImages(), image->GetImageCount(), image->GetMetadata(), &m_texture);
    if (FAILED(hr))
        return false;

    return true;
}


void Sprite::SetTexture(ID3D12Resource* texture)
{
    m_texture = texture;
}

void Sprite::SetTransform(const DirectX::XMFLOAT2& position, float rotation, const DirectX::XMFLOAT2& scale)
{
    m_position = position;
    m_scale = scale;
    m_rotation = rotation;

    DirectX::XMMATRIX transform = DirectX::XMMatrixScaling(scale.x, scale.y, 1.0f) * DirectX::XMMatrixRotationZ(rotation) * DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
    DirectX::XMStoreFloat4x4(&m_transformMatrix, transform);

}

void Sprite::Update(float deltaTime)
{
	// Any sprite-specific update logic can go here
}

void Sprite::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap)
{
    // Set the pipeline state object and root signature
    commandList->SetPipelineState(mPipelineState.Get());
    commandList->SetGraphicsRootSignature(mRootSignature.Get());

    // Set the vertex buffer
    commandList->IASetVertexBuffers(0, 1, &mVertexBufferView);

    // Set the descriptor heap and bind the texture
    ID3D12DescriptorHeap* heaps[] = { mDescriptorHeap.Get() };
    commandList->SetDescriptorHeaps(_countof(heaps), heaps);
    CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorHandle(mDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    mRenderer->GetDevice()->CopyDescriptorsSimple(1, descriptorHandle, mTexture->GetShaderResourceView(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Set the transform matrix constant buffer
    struct TransformConstants
    {
	    DirectX::XMFLOAT4X4 transform;
    };
    TransformConstants transformConstants;
    transformConstants.transform = m_transformMatrix;
    commandList->SetGraphicsRoot32BitConstants(0, sizeof(transformConstants) / 4, &transformConstants, 0);

    // Draw the sprite
    commandList->DrawInstanced(mNumVertices, 1, 0, 0);
}

void Sprite::CreateVertexBuffer()
{
    // Define the geometry for a sprite.
    SpriteVertex spriteVertices[] =
    {
        // Top left
        { { -0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f } },
        // Top right
        { { 0.5f, 0.5f, 0.0f },{ 1.0f, 0.0f } },
        // Bottom left
        { { -0.5f, -0.5f, 0.0f },{ 0.0f, 1.0f } },
        // Bottom right
        { { 0.5f, -0.5f, 0.0f },{ 1.0f, 1.0f } }
    };

    const UINT vertexBufferSize = sizeof(spriteVertices);

    // Create default heap for the vertex buffer
    HRESULT hr = m_device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer));

    assert(SUCCEEDED(hr));

    // Create upload heap for the vertex buffer
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
    hr = m_device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBufferUploadHeap));

    assert(SUCCEEDED(hr));

    // Copy vertex buffer data to the upload heap
    D3D12_SUBRESOURCE_DATA vertexData = {};
    vertexData.pData = reinterpret_cast<BYTE*>(spriteVertices);
    vertexData.RowPitch = vertexBufferSize;
    vertexData.SlicePitch = vertexBufferSize;

    UpdateSubresources(m_commandList.Get(), m_vertexBuffer.Get(), vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);

    // Transition the vertex buffer to the vertex buffer state
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

    // Initialize the vertex buffer view
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(SpriteVertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
}

void Sprite::CreateIndexBuffer()
{

}

void Sprite::CreateConstantBuffer(ID3D12Device* device)
{

}

void Sprite::CreatePipelineState(ID3D12Device* device)
{
    // define pipeline state object
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { m_inputElementDescs.data(), (UINT)m_inputElementDescs.size() };
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_pixelShader.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    // create pipeline state object
    HRESULT hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create pipeline state object.");
    }

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
