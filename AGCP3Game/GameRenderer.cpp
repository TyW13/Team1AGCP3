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

}

GameRenderer::~GameRenderer()
{
    if (mD3D.GetDeviceResources())
    {
        mD3D.GetDeviceResources()->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void GameRenderer::Initialize(HWND window, int width, int height)
{
    audio.Init();
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
    mD3D.BeginRender();

    m_spriteBatch->Begin(mD3D.GetCommandList());

    m_spriteBatch->Draw(mD3D.GetResourceDescriptors()->GetGpuHandle(2),
        GetTextureSize(m_background.Get()),
        m_fullscreenRect);

    //m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
    //    GetTextureSize(m_texture.Get()),
    //    m_screenPos, nullptr, Colors::White, 0.f, m_origin);

    m_spriteBatch->Draw(mD3D.GetResourceDescriptors()->GetGpuHandle(0),
        GetTextureSize(m_texture.Get()),
        m_screenPos, nullptr, Colors::White, 0.f, m_origin);

    m_spriteBatch->End();

    mD3D.EndRender();
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

    mD3D.CreateWindowSizeDependentResources();

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

    mD3D.CreateDeviceDependentResources();

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


//m_graphicsMemory:


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
        mD3D.GetResourceDescriptors()->GetCpuHandle(Descriptors::Tile));

    // BACKGROUND

    DX::ThrowIfFailed(
        CreateWICTextureFromFile(mD3D.GetDevice(), *mD3D.GetResourceUpload(), L"Data/sunset.jpg",
            m_background.ReleaseAndGetAddressOf()));


    DirectX::CreateShaderResourceView(mD3D.GetDevice(), m_background.Get(),
        mD3D.GetResourceDescriptors()->GetCpuHandle(Descriptors::Background));

    RenderTargetState rtState(mD3D.GetDeviceResources()->GetBackBufferFormat(),
        mD3D.GetDeviceResources()->GetDepthBufferFormat());

    //SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);
    //m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

    SpriteBatchPipelineStateDescription pd(
        rtState, nullptr, nullptr, nullptr, mD3D.GetSampler());
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
#pragma endregion