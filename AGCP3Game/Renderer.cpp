
#include "stdafx.h"
#include "Renderer.h"

#include <stdexcept>
#include <fstream>
#include <wrl/client.h>

#include "DXSampleHelper.h" // throw if failed
using namespace DirectX;

#include <DirectXMath.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "d3dcompiler.lib")

DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;




Renderer::Renderer(HWND hWnd, int width, int height)
{
}

Renderer::~Renderer()
{
}



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
	



}


void Renderer::Render()
{
}

void Renderer::WaitForPreviousFrame()
{
}

void Renderer::Init()
{
}

void Renderer::CreateCommandObjects()
{
}

void Renderer::CreateRenderTargets()
{
}

void Renderer::CreateDepthStencilBufferAndView()
{
}

void Renderer::CreateViewPortAndScissorRect()
{
}
