//
// Game.cpp
//
#include "stdafx.h"

#include "DeviceManager.h"
#include "Framework.h"

#include <Keyboard.h>
#include <Mouse.h>

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

DeviceManager::DeviceManager() noexcept(false)
{
    deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    deviceResources->RegisterDeviceNotify(this);
}

DeviceManager::~DeviceManager()
{
    if (deviceResources)
    {
        deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void DeviceManager::Init(HWND window, int width, int height)
{
    deviceResources->SetWindow(window, width, height);

    deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    assert(deviceResources);
    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:

 /*   m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);*/

    //game logic, will move later

    m_keyboard = std::make_unique<DirectX::Keyboard>();
    m_mouse = std::make_unique<DirectX::Mouse>();
    m_mouse->SetWindow(window);
}

/////
#pragma region Frame Render

// Helper method to clear the back buffers.
void DeviceManager::Clear()
{
    commandList = deviceResources->GetCommandList();
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

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void DeviceManager::CreateDeviceDependentResources()
{
    device = deviceResources.get()->GetD3DDevice();

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

    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    m_states = std::make_unique<CommonStates>(device);

    resourceDescriptors = std::make_unique<DescriptorHeap>(device,
        Descriptors::Count);

    resourceUpload = std::make_unique<ResourceUploadBatch>(device);

    resourceUpload.get()->Begin();

    RenderTargetState rtState(deviceResources.get()->GetBackBufferFormat(),
        deviceResources.get()->GetDepthBufferFormat());

    auto sampler = m_states->PointWrap();
    SpriteBatchPipelineStateDescription pd(
        rtState, nullptr, nullptr, nullptr, &sampler);
    ResourceUploadBatch* rUpload = resourceUpload.get();
    spriteBatch = std::make_unique<SpriteBatch>(device, *rUpload, pd);

    auto uploadResourcesFinished = resourceUpload.get()->End(
        deviceResources.get()->GetCommandQueue());

    uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void DeviceManager::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    m_fullscreenRect = deviceResources->GetOutputSize();

    auto viewport = deviceResources->GetScreenViewport();
    spriteBatch.get()->SetViewport(viewport);

    auto size = deviceResources->GetOutputSize();
    m_screenPos.x = float(size.right) / 2.f;
    m_screenPos.y = float(size.bottom) / 2.f;

    // Stretch Sprite
    m_stretchRect.left = size.right / 4;
    m_stretchRect.top = size.bottom / 4;
    m_stretchRect.right = m_stretchRect.left + size.right / 2;
    m_stretchRect.bottom = m_stretchRect.top + size.bottom / 2;
}

void DeviceManager::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    deviceResources.reset();
    resourceUpload.reset();
    resourceDescriptors.reset();
    spriteBatch.reset();
    m_graphicsMemory.reset();
    m_states.reset();
}

void DeviceManager::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
void DeviceManager::BeginRender()
{
    // Don't try to render anything before the first Update.
    //if (m_timer.GetFrameCount() == 0)
    //{
    //    return;
    //}

    // Prepare the command list to render a new frame.
    deviceResources->Prepare();

    commandList = deviceResources->GetCommandList();

    Clear();

    // Add rendering code here.

    float time = float(m_timer.GetTotalSeconds());


    ID3D12DescriptorHeap* heaps[] = { resourceDescriptors->Heap(), m_states->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);
}
void DeviceManager::EndRender()
{
    deviceResources->Present();
    m_graphicsMemory->Commit(deviceResources->GetCommandQueue());
}
#pragma endregion