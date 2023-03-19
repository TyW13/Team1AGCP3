//
// Game.cpp
//
#include "stdafx.h"

#include "pch.h"
#include "DeviceManager.h"
#include "GraphicsMemory.h"
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
void NewD3D::Init(HWND window, int width, int height)
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

}

/////
#pragma region Frame Render

// Helper method to clear the back buffers.
void NewD3D::Clear()
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
void NewD3D::CreateDeviceDependentResources()
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

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);


    //m_graphicsMemory:


    // Tiling a Sprite
    m_states = std::make_unique<CommonStates>(device);

    resourceDescriptors = std::make_unique<DescriptorHeap>(device,
        Descriptors::Count);

    resourceUpload = std::make_unique<ResourceUploadBatch>(device);

    resourceUpload.get()->Begin();


    // WIC

    //DX::ThrowIfFailed(
    //    CreateWICTextureFromFile(device, resourceUpload, L"cat.png",
    //        m_texture.ReleaseAndGetAddressOf()));

    // DDS

    // CAT

    //DX::ThrowIfFailed(
    //    CreateDDSTextureFromFile(device, *resourceUpload.get(), L"Data/test_sheet2.dds",
    //        m_texture.ReleaseAndGetAddressOf()));

    //CreateShaderResourceView(device, m_texture.Get(),
    //    resourceDescriptors.get()->GetCpuHandle(Descriptors::Tile));

    //DX::ThrowIfFailed(
    //    CreateWICTextureFromFile(device, *resourceUpload.get(), L"Data/sunset.jpg",
    //        m_background.ReleaseAndGetAddressOf()));

    //// BACKGROUND

    //CreateShaderResourceView(device, m_background.Get(),
    //    resourceDescriptors.get()->GetCpuHandle(Descriptors::Background));

    RenderTargetState rtState(deviceResources.get()->GetBackBufferFormat(),
        deviceResources.get()->GetDepthBufferFormat());

    auto sampler = m_states->PointWrap();
    SpriteBatchPipelineStateDescription pd(
        rtState, nullptr, nullptr, nullptr, &sampler);
    ResourceUploadBatch* rUpload = resourceUpload.get();
    spriteBatch = std::make_unique<SpriteBatch>(device, *rUpload, pd);

    //XMUINT2 catSize = GetTextureSize(m_texture.Get());

    //m_origin.x = float(catSize.x / 2);
    //m_origin.y = float(catSize.y / 2);

    ////m_origin.x = float(catSize.x * 2);
    ////m_origin.y = float(catSize.y * 2);

    //m_tileRect.left = catSize.x * 2;
    //m_tileRect.right = catSize.x * 6;
    //m_tileRect.top = catSize.y * 2;
    //m_tileRect.bottom = catSize.y * 6;

    auto uploadResourcesFinished = resourceUpload.get()->End(
        deviceResources.get()->GetCommandQueue());

    uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void NewD3D::CreateWindowSizeDependentResources()
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

void NewD3D::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    deviceResources.reset();
    resourceUpload.reset();
    resourceDescriptors.reset();
    m_texture.Reset();
    m_background.Reset();
    spriteBatch.reset();
    m_graphicsMemory.reset();
    m_states.reset();
}

void NewD3D::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
void NewD3D::BeginRender()
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
void NewD3D::EndRender()
{
    deviceResources->Present();
    m_graphicsMemory->Commit(deviceResources->GetCommandQueue());
}
#pragma endregion