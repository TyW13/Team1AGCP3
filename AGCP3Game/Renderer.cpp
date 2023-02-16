#include "stdafx.h"
#include "Renderer.h"







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
