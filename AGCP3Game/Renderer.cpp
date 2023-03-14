#include "Renderer.h"

#include "d3dx12.h"
#include "pch.h"
#include "DXSampleHelper.h"
//#include "Sprite.h"
#include "DirectXTex/DirectXTex.h"

Renderer::Renderer(HWND hwnd, int width, int height)
	: m_hwnd(hwnd), m_width(width), m_height(height), m_device(nullptr), m_swapChain(nullptr), 
	  m_commandQueue(nullptr), m_rtvDescriptorHeap(nullptr), m_dsvDescriptorHeap(nullptr),
	  m_commandAllocator(nullptr), m_commandList(nullptr), m_frameIndex(0), m_fenceEvent(nullptr), 
	  m_fence(nullptr), m_fenceValue(0), m_backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM), 
	  m_backBufferCount(2), m_rtvDescriptorSize(0), m_currentFrameIndex(0),
      m_renderTargets{}
{
	CreateDevice();
	CreateFence();
	CreateCommandQueue();
	CreateCommandList();
	CreateSwapChain();
	CreateDescriptorHeaps();
	CreateRenderTargetView();
	CreateDepthStencilBuffer();
	CreateCommandAllocator();
}

Renderer::~Renderer()
{
}



void Renderer::CreateDevice()
{
  
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device));

}

void Renderer::CreateFence()
{
	DX::ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

   
}

void Renderer::CreateCommandQueue()
{
    // Create the command queue

    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue));

    
}


void Renderer::CreateSwapChain()
{
    // Describe the Swap Chain

    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.BufferCount = 2;

    // Screen Width & Height
    desc.Width = m_width;
    desc.Height = m_height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;

    // Create the Swap chain
 /*   Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;*/
    IDXGIFactory4* factory;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    factory->CreateSwapChainForHwnd(m_commandQueue, m_hwnd, &desc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&m_swapChain));

    factory->Release();
}

void Renderer::CreateDescriptorHeaps()
{
    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = m_bufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    DX::ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvDescriptorHeap)));

    // Describe and create a depth stencil view (DSV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    DX::ThrowIfFailed(m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvDescriptorHeap)));

    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void Renderer::CreateRenderTargetView()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = 2;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvDescriptorHeap));

    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    for (int i = 0; i < 2; i++)
    {
        m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        m_device->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);
    }
}

void Renderer::CreateDepthStencilBuffer()
{
    //// Create the depth stencil

    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = m_width;
    depthStencilDesc.Height = m_height;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = m_depthStencilFormat;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = m_depthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    DX::ThrowIfFailed(m_device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(m_depthStencilBuffer.GetAddressOf())));

}



void Renderer::CreateCommandAllocator()
{
    m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
    DX::ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}

void Renderer::CreateCommandList()
{
    DX::ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)));
    m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList));
    m_commandList->Close();
   
   
}

void Renderer::PopulateCommandList()
{
  /*  float clearColor[] = { 0, 0,255, 0 };
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &barrier);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &barrier);
    m_commandList->Close();*/

    // Reset the command list
    DX::ThrowIfFailed(m_commandList->Reset(m_commandAllocator, nullptr));

    // Transition the render target to the correct state
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentFrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &barrier);

    // Set the render target and depth stencil views
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrameIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    m_commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect
    D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, 1.0f };
    D3D12_RECT scissorRect = { 0, 0, m_width, m_height };
    m_commandList->RSSetViewports(1, &viewport);
    m_commandList->RSSetScissorRects(1, &scissorRect);

    // Indicate that the back buffer will be used as a render target
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // Close the command list
    DX::ThrowIfFailed(m_commandList->Close());
}

void Renderer::RenderFrame()
{
    PopulateCommandList();

    ID3D12CommandList* commandList[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(_countof(commandList), commandList);

    m_swapChain->Present(1, 0);
    DX::ThrowIfFailed(m_swapChain->Present(1, 0));

    WaitForPreviousFrame();

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void Renderer::WaitForPreviousFrame()
{
    //// Signal and increment the fence value.
    //const UINT64 fence = m_fenceValue;
    //m_commandQueue->Signal(m_fence, fence);
    //m_fenceValue++;

    ////
    //if (m_fence->GetCompletedValue() < fence)
    //{
    //    m_fence->SetEventOnCompletion(fence, m_fenceEvent);
    //    WaitForSingleObject(m_fenceEvent, INFINITE);
    //}

    // Signal and increment the fence value.
    const UINT64 fence = m_fenceValue;
    DX::ThrowIfFailed(m_commandQueue->Signal(m_fence(), fence));
    m_fenceValue++;

    // Wait until the previous frame is finished.
    if (m_fence->GetCompletedValue() < fence)
    {
        DX::ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    // Update the frame index.
    m_currentFrameIndex = m_swapChain->GetCurrentBackBufferIndex();
}



void Renderer::RenderSprite(const wchar_t* filename, const DirectX::XMFLOAT2 & position, const DirectX::XMFLOAT2 & scale)
{
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = m_commandList();

    // Load the texture
    Microsoft::WRL::ComPtr<ID3D12Resource> texture = LoadTexture(filename);

    // Set the pipeline state object
    commandList->SetPipelineState(m_pipelineState.Get());

    // Set the vertex buffer and index buffer
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    commandList->IASetIndexBuffer(&m_indexBufferView);

    // Set the texture
    commandList->SetGraphicsRootDescriptorTable(0, m_srvHeap->GetGPUDescriptorHandleForHeapStart());

    // Set the constant buffer
    ConstantBuffer constantBufferData{};
    /*constantBufferData.Model = 
        DirectX::XMMatrixScaling(scale.x, scale.y, 1.0f) *
        DirectX::XMMatrixRotationRollPitchYaw(0,0,0)    *
        DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);*/

    //#TODO - Figure out how to store the XMMatrix -> XMFloat4x4
    //XMStoreFloat4x4

    //constantBufferData.ViewProjection = DirectX::XMMatrixTranspose(m_view);

    //constantBufferData.Color    = DirectX::XMMatrixTranspose(m_view);
    commandList->SetGraphicsRoot32BitConstants(1, sizeof(ConstantBuffer) / 4, &constantBufferData, 0);

    // Draw the sprite
    commandList->DrawIndexedInstanced(m_indexBufferSize / sizeof (UINT), 1, 0, 0, 0);
}

Microsoft::WRL::ComPtr<ID3D12Resource> Renderer::LoadTexture(const wchar_t* filename)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;

    // Load the texture using DirectXTex
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;
    HRESULT hr = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
    if (SUCCEEDED(hr))
    {
        // Create the texture resource
        hr = m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, metadata.height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_NONE),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&textureResource)
        );
        if (SUCCEEDED(hr))
        {
            // Upload the texture data to the GPU
            Microsoft::WRL::ComPtr<ID3D12Resource> uploadResource;
            hr = m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(scratchImage.GetPixelsSize()),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&uploadResource)
            );
            if (SUCCEEDED(hr))
            {
                D3D12_SUBRESOURCE_DATA textureData = {};
                textureData.pData = scratchImage.GetPixels();
                textureData.RowPitch = scratchImage.GetImages()->rowPitch;
                textureData.SlicePitch = scratchImage.GetImages()->slicePitch;

                UpdateSubresources(m_commandList(), textureResource.Get(), uploadResource.Get(), 0, 0, 1, &textureData);
                m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(textureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
            }
        }
    }

    return textureResource;
}

// Create DepthStencilBuffer
// Create descriptor to mip level 0 of entire resource using the 
// format of the resource.

//    m_device->CreateDepthStencilView(
//        m_depthStencilBuffer.Get(),
//        nulltptr
//    )
//}