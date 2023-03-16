//
// Game.cpp
//
#include "stdafx.h"

#include "pch.h"
#include "MyD3D.h"
#include "GraphicsMemory.h"
#include "SpriteBatch.h"
#include "ResourceUploadBatch.h"
#include "Framework.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

NewD3D::NewD3D() noexcept(false)
{
    deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    deviceResources->RegisterDeviceNotify(this);
}

NewD3D::~NewD3D()
{
    if (deviceResources)
    {
        deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void NewD3D::Initialize(HWND window, int width, int height)
{
    deviceResources->SetWindow(window, width, height);

    deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:

 /*   m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);*/

}

#pragma region Frame Update
// Executes the basic game loop.
void NewD3D::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

// Updates the world.
void NewD3D::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

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
void NewD3D::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    deviceResources->Prepare();

    auto commandList = deviceResources->GetCommandList();

    Clear();

    // Add rendering code here.

    float time = float(m_timer.GetTotalSeconds());

   
    ID3D12DescriptorHeap* heaps[] = { resourceDescriptors->Heap(), m_states->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    m_spriteBatch->Begin(commandList);

    m_spriteBatch->Draw(resourceDescriptors->GetGpuHandle(Descriptors::Background),
        GetTextureSize(m_background.Get()),
        m_fullscreenRect);

    m_spriteBatch->Draw(resourceDescriptors->GetGpuHandle(Descriptors::Cat),
        GetTextureSize(m_texture.Get()),
        m_screenPos, nullptr, Colors::White, 0.f, m_origin);

    m_spriteBatch->End();

    // Show the new frame.
    deviceResources->Present();
    m_graphicsMemory->Commit(deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void NewD3D::Clear()
{
    auto commandList = deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor = deviceResources->GetRenderTargetView();
    auto const dsvDescriptor = deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport = deviceResources->GetScreenViewport();
    auto const scissorRect = deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void NewD3D::OnActivated()
{
    // TODO: Game is becoming active window.
}

void NewD3D::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void NewD3D::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void NewD3D::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void NewD3D::OnWindowMoved()
{
    auto const r = deviceResources->GetOutputSize();
    deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void NewD3D::OnDisplayChange()
{
    deviceResources->UpdateColorSpace();
}

void NewD3D::OnWindowSizeChanged(int width, int height)
{
    if (!deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void NewD3D::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void NewD3D::CreateDeviceDependentResources()
{
    device = deviceResources->GetD3DDevice();



    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);


//m_graphicsMemory:

    // Tiling a Sprite
    m_states = std::make_unique<CommonStates>(device);

    resourceDescriptors = std::make_unique<DescriptorHeap>(device,
        Descriptors::Count);

    resourceUpload = std::make_unique<DirectX::ResourceUploadBatch>(device);

    resourceUpload.get()->Begin();

    auto uploadResourcesFinished = resourceUpload.get()->End(
        deviceResources->GetCommandQueue());

    uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void NewD3D::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    m_fullscreenRect = deviceResources->GetOutputSize();

    auto viewport = deviceResources->GetScreenViewport();

    auto size = deviceResources->GetOutputSize();
    m_screenPos.x = float(size.right) / 2.f;
    m_screenPos.y = float(size.bottom) / 2.f;

    // Stretch Sprite
    m_stretchRect.left = size.right / 4;
    m_stretchRect.top = size.bottom / 4;
    m_stretchRect.right = m_stretchRect.left + size.right / 2;
    m_stretchRect.bottom = m_stretchRect.top + size.bottom / 2;
}

void NewD3D::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory.reset();
    m_texture.Reset();
    resourceDescriptors.reset();
    m_states.reset();
    m_background.Reset();
}

void NewD3D::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion