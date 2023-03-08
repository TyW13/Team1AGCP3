#include "MyGame.h"

#include <algorithm>
#include <DirectXColors.h>
#include <memory>

#include "DDSTextureLoader.h"

MyGame::MyGame()
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

void MyGame::LoadTextures()
{
    // Load textures using DirectX Tool Kit (example)
    HRESULT hr;

    // Create a texture loader
    std::unique_ptr<DirectX::ResourceUploadBatch> uploadBatch = std::make_unique<DirectX::ResourceUploadBatch>(m_device.Get());
    uploadBatch->Begin();

    // Load a texture from a file
    std::unique_ptr<DirectX::Texture2D> texture = std::make_unique<DirectX::Texture2D>();
    hr = DirectX::CreateDDSTextureFromFile(m_device.Get(), uploadBatch.get(), L"texture.dds", texture->GetAddressOf());
    if (FAILED(hr))
    {
        // Handle the error
    }

    // Upload the texture to the GPU
    uploadBatch->End(m_commandQueue.Get());
    m_commandQueue->Signal(m_fence.Get(), ++m_fenceValue);
    m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent.Get());
    WaitForSingleObject(m_fenceEvent.Get(), INFINITE);

    // Store the texture in a member variable
    m_texture = std::move(texture);
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
    m_renderer->Begin(m_commandList.Get(), m_viewMatrix, m_projMatrix);

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
