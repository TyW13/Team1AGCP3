#include "SpriteRenderer.h"
#include "Game.h"
#include "Sprite.h"
#include <DescriptorHeap.h>
#include "pch.h"
#include "MyGame.h"



using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;




SpriteRenderer::SpriteRenderer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, UINT width, UINT height)
    : m_width(width), m_height(height)
{
    InitializeResources(device);
    InitializeVertices();
    InitializeIndices();
    InitializeConstantBuffer(device);
    InitializeDescriptorHeap(device);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::SetPosition(float x, float y)
{
    SetTransform(x, y, m_transform._11, m_transform._22, DirectX::XMConvertToRadians(0.0f));
}

void SpriteRenderer::SetScale(float scaleX, float scaleY)
{
    SetTransform(m_transform._41, m_transform._42, scaleX, scaleY, DirectX::XMConvertToRadians(0.0f));
}

void SpriteRenderer::SetRotation(float angle)
{
    SetTransform(m_transform._41, m_transform._42, m_transform._11, m_transform._22, DirectX::XMConvertToRadians(angle));
}


void SpriteRenderer::Draw()
{
    auto commandList = D3D12_COMMAND_LIST_TYPE_DIRECT;

    // Set necessary state for sprite rendering
    commandList->SetGraphicsRootSignature(m_spriteRootSignature.Get());
    commandList->SetPipelineState(m_spritePipelineState.Get());
    commandList->SetGraphicsRootConstantBufferView(0, m_spriteConstantBuffer->GetGPUVirtualAddress());
    commandList->SetGraphicsRootDescriptorTable(1, m_spriteDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

    // Set sprite vertex buffer
    UINT vertexBufferSize = sizeof(SpriteVertex
        commandList->IASetVertexBuffers(0, 1, &m_spriteVertexBufferView);

    // Set sprite index buffer
    commandList->IASetIndexBuffer(&m_spriteIndexBufferView);

    // Draw sprite
    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void SpriteRenderer::InitializeResources(ID3D12Device* device)
{
    // Create root signature
    D3D12_ROOT_PARAMETER rootParameters[2];
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[0].Descriptor.ShaderRegister = 0;
    rootParameters[0].Descriptor.RegisterSpace = 0;
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParameters[1].DescriptorTable.pDescriptorRanges = &m_spriteDescriptorRange;
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = _countof(rootParameters);
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> signature;
    Microsoft::WRL::ComPtr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    if (FAILED(hr)) {
        OutputDebugStringA((char*)error->GetBufferPointer());
        assert(false);
    }

    device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_spriteRootSignature));

    // Create pipeline state object
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = m_spriteRootSignature.Get();
    psoDesc.VS = { SpriteVertexShader, sizeof(SpriteVertexShader) };
    psoDesc.PS = { SpritePixelShader, sizeof(SpritePixelShader) };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;


    device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_spritePipelineState));

    // Create vertex buffer
    VertexPositionTexture spriteVertices[] = {
        { { -0.5f, -0.5f, 0.0f },{ 0.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.0f },{ 1.0f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f },{ 1.0f, 1.0f } },
    };
    const UINT vertexBufferSize = sizeof(spriteVertices);

    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_spriteVertexBuffer)
    );
    m_spriteVertexBuffer->SetName(L"Sprite Vertex Buffer");

    void* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    m_spriteVertexBuffer->Map(0, &readRange, &pVertexDataBegin);
    memcpy(pVertexDataBegin, spriteVertices, vertexBufferSize);
    m_spriteVertexBuffer->Unmap(0, nullptr);

    m_spriteVertexBufferView.BufferLocation = m_spriteVertexBuffer->GetGPUVirtualAddress();
    m_spriteVertexBufferView.StrideInBytes = sizeof(VertexPositionTexture);
    m_spriteVertexBufferView.SizeInBytes = vertexBufferSize;

    // Create index buffer
    WORD spriteIndices[] = {
        0, 1, 2,
        0, 2, 3
    };
    const UINT indexBufferSize = sizeof(spriteIndices);

    resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_spriteIndexBuffer)
    );
    m_spriteIndexBuffer->SetName(L"Sprite Index Buffer");

    void* pIndexDataBegin;
    m_spriteIndexBuffer->Map(0, &readRange, &pIndexDataBegin);
    memcpy(pIndexDataBegin, spriteIndices, indexBufferSize);
    m_spriteIndexBuffer->Unmap(0, nullptr);

    m_spriteIndexBufferView.BufferLocation = m_spriteIndexBuffer->GetGPUVirtualAddress();
    m_spriteIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
    m_spriteIndexBufferView.SizeInBytes = indexBufferSize;

    // Create texture descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
    descriptorHeapDesc.NumDescriptors = 1;
    descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_spriteTextureDescriptorHeap));
    m_spriteTextureDescriptorHeap->SetName(L"Sprite Texture Descriptor Heap");

    // Load texture
    Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
    HRESULT hr = DirectX::CreateDDSTextureFromFile(device, L"my_sprite.dds",
        textureResource.GetAddressOf(),
        m_spriteTexture.ReleaseAndGetAddressOf());

    CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle(m_spriteTextureDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle(m_spriteTextureDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    device->CreateShaderResourceView(m_spriteTexture.Get(), nullptr, cpuDescriptorHandle);
    m_spriteTextureResource = textureResource.Get();

    // Create constant buffer
    resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ConstantBuffer));
    device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_spriteConstantBuffer)
    );
    m_spriteConstantBuffer->SetName(L"Sprite Constant Buffer");

    CD3DX12_RANGE writeRange(0, 0);
    m_spriteConstantBuffer->Map(0, &writeRange, reinterpret_cast<void**>(&m_pSpriteConstantBufferData));
    ZeroMemory(m_pSpriteConstantBufferData, sizeof(ConstantBuffer));
    m_spriteConstantBuffer->Unmap(0, nullptr);

    // Initialize constant buffer data
    m_spriteConstantBufferData->worldMatrix = DirectX::XMMatrixIdentity();
    m_spriteConstantBufferData->viewMatrix = DirectX::XMMatrixIdentity();
    m_spriteConstantBufferData->projectionMatrix = DirectX::XMMatrixIdentity();

    m_spriteConstantBufferData->color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up viewport and scissor rectangle
    m_spriteViewport = CD3DX12_VIEWPORT(
        0.0f,
        0.0f,
        static_cast<float>(m_windowWidth),
        static_cast<float>(m_windowHeight));

    m_spriteScissorRect = CD3DX12_RECT(0, 0, m_windowWidth, m_windowHeight);

}

void Sprite::Draw(ID3D12GraphicsCommandList* commandList)
{
    // Set necessary state
    commandList->SetGraphicsRootSignature(m_spriteRootSignature.Get());
    commandList->SetPipelineState(m_spritePipelineState.Get());
    // Set constant buffer data
    m_pSpriteConstantBufferData->worldMatrix = DirectX::XMMatrixTranspose(m_worldMatrix);
    m_pSpriteConstantBufferData->color = m_color;

    // Copy data to constant buffer
    CD3DX12_RANGE writeRange(0, 0);
    m_spriteConstantBuffer->Map(0, &writeRange, reinterpret_cast<void**>(&m_pSpriteConstantBufferData));
    memcpy(m_pMappedConstantBuffer, m_pSpriteConstantBufferData, sizeof(ConstantBuffer));
    m_spriteConstantBuffer->Unmap(0, nullptr);

    // Set constant buffer view
    D3D12_GPU_VIRTUAL_ADDRESS constantBufferAddress = m_spriteConstantBuffer->GetGPUVirtualAddress();
    commandList->SetGraphicsRootConstantBufferView(0, constantBufferAddress);

    // Set texture view
    commandList->SetGraphicsRootDescriptorTable(1, m_spriteTextureDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

    // Set vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    commandList->IASetVertexBuffers(0, 1, &m_spriteVertexBufferView);

    // Set index buffer
    commandList->IASetIndexBuffer(&m_spriteIndexBufferView);

    // Set primitive topology
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set viewport and scissor rectangle
    commandList->RSSetViewports(1, &m_spriteViewport);
    commandList->RSSetScissorRects(1, &m_spriteScissorRect);

    // Draw sprite
    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

}

//void SpriteRenderer::Initialize(ID3D12Device* m_device)
//{
//
//}
//
//void SpriteRenderer::CreateDeviceDependentResources()
//{
//	m_deviceResources = std::make_unique<DX::DeviceResources>();
//
//	
//	auto device = m_deviceResources->GetD3DDevice();
//
//	m_resourceDescriptors = std::make_unique<DescriptorHeap>(device,
//		Descriptors::Count);
//
//	ResourceUploadBatch resourceUpload(device);
//
//	resourceUpload.Begin();
//
//
//	DX::ThrowIfFailed(
//		DirectX::CreateDDSTextureFromFile(device, resourceUpload, L"cat.dds",
//			m_texture.ReleaseAndGetAddressOf()));
//
//	CreateShaderResourceView(device, m_texture.Get(),
//		m_resourceDescriptors->GetCpuHandle(Descriptors::Cat));
//
//
//	RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
//		m_deviceResources->GetDepthBufferFormat());
//
//	SpriteBatchPipelineStateDescription pd(rtState);
//	m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);
//
//	XMUINT2 catSize = GetTextureSize(m_texture.Get());
//
//	m_origin.x = float(catSize.x / 2);
//	m_origin.y = float(catSize.y / 2);
//
//
//	//m_origin.x = float(catSize.x * 2);
//	//m_origin.y = float(catSize.y * 2);
//
//	m_tileRect.left = catSize.x * 2;
//	m_tileRect.right = catSize.x * 6;
//	m_tileRect.top = catSize.y * 2;
//	m_tileRect.bottom = catSize.y * 6;
//
//	auto uploadResourcesFinished = resourceUpload.End(
//		m_deviceResources->GetCommandQueue());
//
//	uploadResourcesFinished.wait();
//
//}
//
//void SpriteRenderer::LoadTexture()
//{
//
//
//
//}
//
//void SpriteRenderer::Update()
//{
//
//}
//
//void SpriteRenderer::Render()
//{
//
//	// Don't try to render anything before the first Update.
//	if (m_timer.GetFrameCount() == 0)
//	{
//		return;
//	}
//
//	
//	// Prepare the command list to render a new frame.
//	m_deviceResources->Prepare();
//
//	auto commandList = m_deviceResources->GetCommandList();
//
//	
//
//	float time = float(m_timer.GetTotalSeconds());
//
//	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
//	commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);
//	m_spriteBatch->Begin(commandList);
//
//	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Background),
//		GetTextureSize(m_background.Get()),
//		m_fullscreenRect);
//
//	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
//		GetTextureSize(m_texture.Get()),
//		m_screenPos, nullptr, Colors::White, 0.f, m_origin);
//
//	m_spriteBatch->End();
//
//}
//
//void SpriteRenderer::OnDeviceLost()
//{
//	m_texture.Reset();
//	m_resourceDescriptors.reset();
//	m_spriteBatch.reset();
//}
//
//
//void SpriteRenderer::Draw()
//{
//
//}










//SpriteRenderer::SpriteRenderer()
//{
//    // Create an instance of the Render class
//    m_render = std::make_unique<Render>();
//
//    // Create a texture for the sprite
//    m_texture = std::make_unique<Texture>();
//    m_texture->LoadFromFile("sprite.dds");
//
//    // Create a sprite with the texture
//    m_sprite = std::make_unique<Sprite>(m_texture.get());
//    m_sprite->SetPosition(100, 100);
//}
//
//void SpriteRenderer::Update(float deltaTime)
//{
//    // Handle input and update sprite position
//    if (Input::IsKeyDown('W'))
//    {
//        m_sprite->Move(0, -50 * deltaTime);
//    }
//    if (Input::IsKeyDown('S'))
//    {
//        m_sprite->Move(0, 50 * deltaTime);
//    }
//    if (Input::IsKeyDown('A'))
//    {
//        m_sprite->Move(-50 * deltaTime, 0);
//    }
//    if (Input::IsKeyDown('D'))
//    {
//        m_sprite->Move(50 * deltaTime, 0);
//    }
//}
//
//void SpriteRenderer::Render()
//{
//    // Clear the render target
//    m_render->Clear();
//
//    // Draw the sprite
//    m_render->DrawSprite(m_sprite.get());
//
//    // Present the frame
//    m_render->Present();
//}

