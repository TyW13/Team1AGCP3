//
// Game.cpp
//
#include "stdafx.h"

#include "pch.h"
#include "GameRenderer.h"
#include "GraphicsMemory.h"
#include "SpriteBatch.h"
#include "ResourceUploadBatch.h"
#include "DDSTextureLoader.h"
#include "Framework.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameRenderer::GameRenderer(NewD3D& _mD3D) noexcept(false)
    : mD3D(_mD3D)
{
    //m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    //mD3D.GetDeviceResources()->RegisterDeviceNotify(this);
}

GameRenderer::~GameRenderer()
{
    delete m_pFramework;

    if (mD3D.GetDeviceResources())
    {
        mD3D.GetDeviceResources()->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void GameRenderer::Initialize(HWND window, int width, int height)
{
    m_pFramework = new Framework;
    m_pFramework->Initialise();

    mD3D.GetDeviceResources()->SetWindow(window, width, height);

    //mD3D.GetDeviceResources()->CreateDeviceResources();
    CreateDeviceDependentResources();

    mD3D.GetDeviceResources()->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    audio.Init();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:

 /*   m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);*/

}

#pragma region Frame Update
// Executes the basic game loop.
void GameRenderer::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

// Updates the world.
void GameRenderer::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

    audio.Update(elapsedTime);

    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// This is our game logic all here!
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    m_pFramework->Update(elapsedTime);
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////


    PIXEndEvent();
}
#pragma endregion
/////
#pragma region Frame Render
// Draws the scene.
void GameRenderer::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    mD3D.GetDeviceResources()->Prepare();

    auto commandList = mD3D.GetDeviceResources()->GetCommandList();

    Clear();

    // Add rendering code here.

    float time = float(m_timer.GetTotalSeconds());

   
    ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    m_spriteBatch->Begin(commandList);

    m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Background),
        GetTextureSize(m_background.Get()),
        m_fullscreenRect);

    //m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
    //    GetTextureSize(m_texture.Get()),
    //    m_screenPos, nullptr, Colors::White, 0.f, m_origin);

    m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Tile),
        GetTextureSize(m_texture.Get()),
        m_screenPos, nullptr, Colors::White, 0.f, m_origin);

    m_spriteBatch->End();

  

    // Show the new frame.
    mD3D.GetDeviceResources()->Present();
    m_graphicsMemory->Commit(mD3D.GetDeviceResources()->GetCommandQueue());
}

// Helper method to clear the back buffers.
void GameRenderer::Clear()
{
    auto commandList = mD3D.GetDeviceResources()->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor = mD3D.GetDeviceResources()->GetRenderTargetView();
    auto const dsvDescriptor = mD3D.GetDeviceResources()->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport = mD3D.GetDeviceResources()->GetScreenViewport();
    auto const scissorRect = mD3D.GetDeviceResources()->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void GameRenderer::OnActivated()
{
    // TODO: Game is becoming active window.
}

void GameRenderer::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void GameRenderer::OnSuspending()
{
    audio.OnSuspending();

    // TODO: Game is being power-suspended (or minimized).
}

void GameRenderer::OnResuming()
{
    m_timer.ResetElapsedTime();

    audio.OnResuming();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void GameRenderer::OnWindowMoved()
{
    auto const r = mD3D.GetDeviceResources()->GetOutputSize();
    mD3D.GetDeviceResources()->WindowSizeChanged(r.right, r.bottom);
}

void GameRenderer::OnDisplayChange()
{
    mD3D.GetDeviceResources()->UpdateColorSpace();
}

void GameRenderer::OnWindowSizeChanged(int width, int height)
{
    if (!mD3D.GetDeviceResources()->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void GameRenderer::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void GameRenderer::CreateDeviceDependentResources()
{
    //auto device = m_deviceResources->GetD3DDevice();
    //device = std::make_unique<ID3D12Device>();
    //device = m_deviceResources->GetD3DDevice();


    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(mD3D.GetDevice()->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = std::make_unique<GraphicsMemory>(mD3D.GetDevice());


//m_graphicsMemory:



    // Tiling a Sprite
    m_states = std::make_unique<CommonStates>(mD3D.GetDevice());

    m_resourceDescriptors = std::make_unique<DescriptorHeap>(mD3D.GetDevice(),
        Descriptors::Count);

    //resourceUpload = std::make_unique<ResourceUploadBatch>(mD3D.GetDevice());
    //ResourceUploadBatch rUpload(device);
    
    mD3D.GetResourceUpload()->Begin();


    // WIC

    //DX::ThrowIfFailed(
    //    CreateWICTextureFromFile(device, resourceUpload, L"cat.png",
    //        m_texture.ReleaseAndGetAddressOf()));

    // DDS

    // CAT

    //DX::ThrowIfFailed(
    //    CreateDDSTextureFromFile(device, resourceUpload, L"Data/cat.dds",
    //        m_texture.ReleaseAndGetAddressOf()));


    //CreateShaderResourceView(device, m_texture.Get(),
    //    m_resourceDescriptors->GetCpuHandle(Descriptors::Cat));

    // Tile
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(mD3D.GetDevice(), *mD3D.GetResourceUpload(), L"Data/test_sheet2.dds",
            m_texture.ReleaseAndGetAddressOf()));

    DirectX::CreateShaderResourceView(mD3D.GetDevice(), m_texture.Get(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Tile));

    // BACKGROUND

    DX::ThrowIfFailed(
        CreateWICTextureFromFile(mD3D.GetDevice(), *mD3D.GetResourceUpload(), L"Data/sunset.jpg",
            m_background.ReleaseAndGetAddressOf()));


    DirectX::CreateShaderResourceView(mD3D.GetDevice(), m_background.Get(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Background));

    RenderTargetState rtState(mD3D.GetDeviceResources()->GetBackBufferFormat(),
        mD3D.GetDeviceResources()->GetDepthBufferFormat());

    //SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);
    //m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

    auto sampler = m_states->LinearWrap();
    SpriteBatchPipelineStateDescription pd(
        rtState, nullptr, nullptr, nullptr, &sampler);
    m_spriteBatch = std::make_unique<SpriteBatch>(mD3D.GetDevice(), *mD3D.GetResourceUpload(), pd);

    XMUINT2 catSize = GetTextureSize(m_texture.Get());

    m_origin.x = float(catSize.x / 2);
    m_origin.y = float(catSize.y / 2);

    //m_origin.x = float(catSize.x * 2);
    //m_origin.y = float(catSize.y * 2);

    m_tileRect.left = catSize.x * 2;
    m_tileRect.right = catSize.x * 6;
    m_tileRect.top = catSize.y * 2;
    m_tileRect.bottom = catSize.y * 6;

    auto uploadResourcesFinished = mD3D.GetResourceUpload()->End(
        mD3D.GetDeviceResources()->GetCommandQueue());

    uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void GameRenderer::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    m_fullscreenRect = mD3D.GetDeviceResources()->GetOutputSize();

    auto viewport = mD3D.GetDeviceResources()->GetScreenViewport();
    m_spriteBatch->SetViewport(viewport);

    auto size = mD3D.GetDeviceResources()->GetOutputSize();
    m_screenPos.x = float(size.right) / 2.f;
    m_screenPos.y = float(size.bottom) / 2.f;

    // Stretch Sprite
    m_stretchRect.left = size.right / 4;
    m_stretchRect.top = size.bottom / 4;
    m_stretchRect.right = m_stretchRect.left + size.right / 2;
    m_stretchRect.bottom = m_stretchRect.top + size.bottom / 2;
}

void GameRenderer::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory.reset();
    m_texture.Reset();
    m_resourceDescriptors.reset();
    m_spriteBatch.reset();
    m_states.reset();
    m_background.Reset();
}

void GameRenderer::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion