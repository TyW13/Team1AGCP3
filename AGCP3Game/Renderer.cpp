
#include "Renderer.h"

#include <stdexcept>
#include <fstream>
#include <wrl/client.h>

using namespace DirectX;

Renderer::Renderer(HWND hWnd, int width, int height)
    : m_hWnd(hWnd), m_width(width), m_height(height), m_frameIndex(0)
{
    InitD3D(hWnd);
    LoadAssets();
}

Renderer::~Renderer()
{
    WaitForPreviousFrame();
}

void Renderer::Update()
{

}

void Renderer::Render()
{
    PopulateCommandList();

    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    WaitForPreviousFrame();
}

void Renderer::Resize(int width, int height)
{
    if (m_swapChain)
    {
        WaitForPreviousFrame();

        m_commandList->Reset(m_commandAllocator.Get(), nullptr);

        for (UINT i = 0; i < _countof(m_renderTargets); i++)
        {
            m_renderTargets[i].Reset();
        }
        m_rtvHeap.Reset();

        m_width = width;
        m_height = height;

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        m_swapChain->GetDesc(&swapChainDesc);
        m_swapChain->ResizeBuffers(swapChainDesc.BufferCount, width, height, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags);

        CreateDescriptorHeaps();
    }
}

void Renderer::InitD3D(HWND hWnd)
{
    // Create DXGIFactory
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_factory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create DXGI factory");
    }

    // Create device
    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_12_0 };
    for (UINT i




//void Renderer::CreateDevice()
//{
//	// Create a DX12 device
//	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
//
//}
//
//void Renderer::CreateSwapChain(HWND hwnd)
//{
//    // Create a swap chain
//    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
//    swapChainDesc.BufferCount = 2;
//    swapChainDesc.Width = 1920;
//    swapChainDesc.Height = 1080;
//    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//    swapChainDesc.SampleDesc.Count = 1;
//}
//
//void Renderer::CreateCommandObjects()
//{
//    // Create a command queue
//    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//    m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
//
//}
//
//void Renderer::CreateRenderTargetViews()
//{
//    
//    // Create render target view
//    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
//    for (int i = 0; i < 2; i++)
//    {
//        m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
//        m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
//        rtvHandle.Offset(1, m_rtvDescriptorSize);
//    }
//}
//
//void Renderer::CreateFence()
//{
//    // Create a fence
//    m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
//
//}
//
//void Renderer::WaitForPreviousFrame()
//{
//    // wait for GPU
//}


