#include "stdafx.h"
#include "Renderer.h"

#include <stdexcept>
#include <fstream>
#include <wrl/client.h>
#include "pch.h"
#include "DXSampleHelper.h" // throw if failed
using namespace DirectX;

#include <DirectXMath.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "d3dcompiler.lib")

DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;




void Renderer::CreateDevice()
{
	// pAdapter = Specifies the display adapter we want the created device to rep.
	HRESULT WINAPI D3D12CreateDevice(
		IUnknown * pAdapter,
		// Minimum Feature Level 
		D3D_FEATURE_LEVEL MinimumFeatureLevel,
		// Riid = The COM ID of the ID3DDevice interface we want to create
		REFIID riid, // Expected: ID3D12DEVICE,
		// ppDevice = Returns the created device
		void** ppDevice);

	// Create DXGIFactory
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_factory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create DXGI factory");
    }

}

void Renderer::CreateFence()
{
	// Descriptor Sizes can vary across GPU so we must query this info.

	ThrowIfFailed(m_device->CreateFence(
		0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

	// RTV
	mRtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// DSV
	mDsvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// CBV
	mCbvSrvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	


}

void Renderer::Check4xMSSA()
{
	// Check for the supported quality level.
	// Because 4xMSSA is always supported, the returned quality should always be
	// greater than 0, therefore the asset.

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat; // back buffer index?
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(m_device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

	

}

void Renderer::CreateCommandList()
{
	// Command Queue Represented by the ID3D12CommandQueue (FL - 4.2.1)
	// Command Allocator Represented by the ID3D12CommandAllocator interface
	// Command List is Represented by the ID3D12GraphicsCommandList

	// Create the Command Queue

	ComPtr<ID3D12CommandQueue>        mCommandQueue;
	ComPtr<ID3D12CommandAllocator>    mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;


	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(m_device->CreateCommandQueue(
		&queueDesc, IID_PPV_ARGS(&mCommandQueue)));


	ThrowIfFailed(m_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())));

	ThrowIfFailed(m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(), // Associated Command allocator
		nullptr,                    // Initial Pipeline State Object (PSO)
		IID_PPV_ARGS(mCommandList.GetAddressOf())));

	// Start off in  closed state. This is because the first time we 
	// refer to the command list we reset it, and it needs to be 
	// closed before calling Reset.

	mCommandList->Close();


}


void Renderer::CreateSwapChain(HWND hWnd, int width, int height)
{
	// Create the Swap Chain

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = hWnd; // mhMainWnd
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note Swap Chain uses queue to perfom flush.
	ThrowIfFailed(m_factory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd,
		mSwapChain.GetAddressOf()));



}




void Renderer::WaitForPreviousFrame()
{
}




//Renderer::Renderer(HWND hWnd, int width, int height)
//{
//    // Create the device
//    CreateDevice();
//
//    // Create the swap chain
//    CreateSwapChain(hWnd, width, height);
//
//    // Create the command list
//    CreateCommandList();
//
//    // Create the fence
//    CreateFence();
//}
//
//Renderer::~Renderer()
//{
//    // Wait for the GPU to complete its commands
//    WaitForPreviousFrame();
//}
//
//void Renderer::Render()
//{
//    // Get the current render target
//    ComPtr<ID3D12Resource> renderTarget = m_renderTargets[m_frameIndex];
//
//    // Reset the command allocator and command list
//    m_commandAllocators[m_frameIndex]->Reset();
//    m_commandList->Reset(m_commandAllocators[m_frameIndex].Get(), nullptr);
//
//    // Transition the render target to the render target state
//    D3D12_RESOURCE_BARRIER barrierDesc = {};
//    barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//    barrierDesc.Transition.pResource = renderTarget.Get();
//    barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
//    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
//    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
//    m_commandList->ResourceBarrier(1, &barrierDesc);
//
//    // Set the viewport and scissor rect
//    D3D12_VIEWPORT viewport = {};
//    viewport.TopLeftX = 0.0f;
//    viewport.TopLeftY = 0.0f;
//    viewport.Width = static_cast<float>(m_renderTargets[0]->GetDesc().Width);
//    viewport.Height = static_cast<float>(m_renderTargets[0]->GetDesc().Height);
//    viewport.MinDepth = 0.0f;
//    viewport.MaxDepth = 1.0f;
//    m_commandList->RSSetViewports(1, &viewport);
//
//    D3D12_RECT scissorRect = {};
//    scissorRect.left = 0;
//    scissorRect.top = 0;
//    scissorRect.right = static_cast<LONG>(m_renderTargets[0]->GetDesc().Width);
//    scissorRect.bottom = static_cast<LONG>(m_renderTargets[0]->GetDesc().Height);
//
//}




//
//Renderer::Renderer(HWND hWnd, int width, int height)
//    : m_hWnd(hWnd), m_width(width), m_height(height), m_frameIndex(0)
//{
//    InitD3D(hWnd);
//    LoadAssets();
//}
//
//Renderer::~Renderer()
//{
//    WaitForPreviousFrame();
//}
//
//void Renderer::Update()
//{
//
//}
//
//void Renderer::Render()
//{
//    PopulateCommandList();
//
//    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
//    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
//
//    WaitForPreviousFrame();
//}
//
//void Renderer::Resize(int width, int height)
//{
//    if (m_swapChain)
//    {
//        WaitForPreviousFrame();
//
//        m_commandList->Reset(m_commandAllocator.Get(), nullptr);
//
//        for (UINT i = 0; i < _countof(m_renderTargets); i++)
//        {
//            m_renderTargets[i].Reset();
//        }
//        m_rtvHeap.Reset();
//
//        m_width = width;
//        m_height = height;
//
//        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//        m_swapChain->GetDesc(&swapChainDesc);
//        m_swapChain->ResizeBuffers(swapChainDesc.BufferCount, width, height, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags);
//
//        CreateDescriptorHeaps();
//    }
//}
//
//void Renderer::InitD3D(HWND hWnd)
//{
//    // Create DXGIFactory
//    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_factory));
//    if (FAILED(hr))
//    {
//        throw std::runtime_error("Failed to create DXGI factory");
//    }
//
//    // Create device
//    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
//    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_12_0 };
//    for (UINT i
//



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

void Renderer::CreateDevice()
{
}

void Renderer::CreateSwapChain(HWND hWnd, int width, int height)
{
}

void Renderer::CreateCommandList()
{
}

void Renderer::CreateFence()
{
}

void Renderer::WaitForPreviousFrame()
{
}
