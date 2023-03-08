#include "MyGame.h"

#include <algorithm>
#include <DirectXColors.h>
#include <memory>
#include <DirectXTex/DirectXTex.h>

#include "d3dx12.h"
#include "DDSTextureLoader.h"
#include "pch.h"




MyGame::MyGame(HWND hwnd)
	:
    m_render(m_device, m_commandList, m_rootSignature, m_pipelineState, m_rtvDescriptorHeap)
{

}

MyGame::~MyGame()
{

}

void MyGame::Initialize(HWND hwnd)
{

    // Create the DirectX 12 device
    CreateDevice();

    // Create the command queue, allocator, and list
    CreateCommandQueue();
    CreateCommandAllocator();
    CreateCommandList();

}

void MyGame::CreateDevice()
{
    HRESULT hr;

    // Create the DirectX 12 device
    hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
    if (FAILED(hr))
    {
        // Handle the error
    }
}

void MyGame::CreateCommandQueue()
{
    HRESULT hr;

    // Create the command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
    if (FAILED(hr))
    {
        // Handle the error
    }
}

void MyGame::CreateCommandAllocator()
{
    HRESULT hr;

    // Create the command allocator
    hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
    if (FAILED(hr))
    {
        // Handle the error
    }
}

void MyGame::CreateCommandList()
{
    HRESULT hr;

    // Create the command list
    hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList));
    if (FAILED(hr))
    {
        // Handle the error
    }

    // Close the command list to prepare for recording commands
    hr = m_commandList->Close();
    if (FAILED(hr))
    {
        // Handle the error
    }
}

void MyGame::LoadContent()
{
    // Load game resources
    LoadTextures();
   
}

void MyGame::UpdatePlayer(float deltaTime)
{
    // Update the player's position based on input and deltaTime
    m_playerPosition.x += m_playerVelocity.x * deltaTime;
    m_playerPosition.y += m_playerVelocity.y * deltaTime;
}

//void MyGame::UpdateEnemies(float deltaTime)
//{
//    // Update enemy positions and behaviors based on game state and deltaTime
//    for (auto& enemy : m_enemies)
//    {
//        enemy.Update(deltaTime);
//    }
//}

void MyGame::Update(float deltaTime)
{
    // Update game logic and objects
    UpdatePlayer(deltaTime);
   /* UpdateEnemies(deltaTime);*/
}

void LoadImageDataFromFile(std::unique_ptr<uint8_t[]>& imageData, int& width, int& height, const wchar_t* fileName)
{
    ////////////// DDS ////////////////////////////////////////////////////

	// Load image file
    std::unique_ptr<uint8_t[]> decodedData;
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;
    HRESULT hr = DirectX::LoadFromDDSFile(fileName, DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);
    if (FAILED(hr))
    {
        // Handle error
        return;
    }

    // Get texture information
    const DirectX::Image* pImage = scratchImage.GetImage(0, 0, 0);
    width = static_cast<int>(pImage->width);
    height = static_cast<int>(pImage->height);
    size_t rowPitch = pImage->rowPitch;
    size_t imageSize = pImage->slicePitch;

    // Copy texture data to output buffer
    imageData = std::make_unique<uint8_t[]>(imageSize);
    memcpy(imageData.get(), pImage->pixels, imageSize);



}
void MyGame::LoadTextures()
{
    // Load texture from file
    std::wstring texturePath = L"my_texture.png";
    std::unique_ptr<uint8_t[]> textureData;
    int textureWidth = 0, textureHeight = 0;
    LoadImageDataFromFile(textureData, textureWidth, textureHeight, texturePath.c_str());

    // Create texture resource
    CD3DX12_RESOURCE_DESC textureDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        0,
        textureWidth,
        textureHeight,
        1,
        1,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        1,
        0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    DX::ThrowIfFailed(m_device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_textureResource)));

    // Upload texture data to the GPU
    UINT64 textureUploadBufferSize;
    m_device->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

    CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC textureUploadDesc = CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize);

    DX::ThrowIfFailed(m_device->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &textureUploadDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_textureUploadHeap)));

    D3D12_SUBRESOURCE_DATA textureDataUpload = {};
    textureDataUpload.pData = textureData.get();
    textureDataUpload.RowPitch = textureWidth * 4;
    textureDataUpload.SlicePitch = textureDataUpload.RowPitch * textureHeight;

    UpdateSubresources(m_commandList.Get(), m_textureResource.Get(), m_textureUploadHeap.Get(), 0, 0, 1, &textureDataUpload);
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_textureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void MyGame::ClearViews()
{
    // Clear the render target and depth stencil views using DirectX Tool Kit (example)
    m_commandList->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), TRUE, m_depthStencilView.Get());
    m_commandList->ClearRenderTargetView(m_renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
    m_commandList->ClearDepthStencilView(m_depthStencilView.Get(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MyGame::RenderObjects()
{
    // Render game objects using a custom renderer (example)
    m_renderer->Begin(m_commandList.Get(), m_viewMatrix, m_projectionMatrix);

    // Draw the player mesh
    m_renderer->DrawMesh(m_mesh.get(), m_playerPosition, m_playerRotation, m_playerScale);

    // Draw the enemy meshes
    for (auto& enemy : m_enemies)
    {
        m_renderer->DrawMesh(enemy.GetMesh(), enemy.GetPosition(), enemy.GetRotation(), enemy.GetScale());
    }

    m_renderer->End();
}

void MyGame::RenderUI()
{
    // Render UI using DirectX Tool Kit (example)
    m_spriteBatch->Begin(m_commandList.Get());

    // Draw UI elements, such as text and images
    m_spriteBatch->Draw(m_texture.Get(), m_playerPosition, nullptr, DirectX::Colors::White, m_playerRotation, m_playerOrigin, m_playerScale);

    m_spriteBatch->End();
}

void MyGame::Present()
{
    // Present the back buffer to the screen
    m_swapChain->Present(1, 0);
}


void MyGame::Render()
{
    // Clear the render target and depth stencil views
    ClearViews();

    // Render game objects and UI
    RenderObjects();
    RenderUI();

    // Present the back buffer to the screen
    Present();
}
