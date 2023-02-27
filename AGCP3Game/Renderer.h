
#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


class Renderer
{
public:
	Renderer(HWND hwnd, int width, int height);
	~Renderer();

	void RenderFrame();

private:
	void CreateDevice();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateCommandAllocator();
	void CreateCommandList();
	void PopulateCommandList();
	void WaitForPreviousFrame();

	// The handle to the window that the renderer will render to
	HWND m_hwnd;

	// Width and height of the window
	int m_width;
	int m_height;

	// The device used for rendering
	ID3D12Device* m_device;

	// The swap chain used for double buffering
	IDXGISwapChain4* m_swapChain;

	// The command queue used for submitting rendering commands to the GPU
	ID3D12CommandQueue* m_commandQueue;

	// The render target view RTV heap used for storing render targets

	ID3D12DescriptorHeap* m_rtvDescriptorHeap;

	// An array of render targets (One for Each back buffer in the swap chain)
	ID3D12Resource* m_renderTargets[2];

	// An array of command allocators (One for Each back buffer in the swap chain)
	ID3D12CommandAllocator* m_commandAllocators[2];

	// The Command List used for recording rendering commands
	ID3D12GraphicsCommandList* m_commandList;



	// Frame Index
	UINT m_frameIndex;

	// The event used for waiting for the fence value to be signaled.
	HANDLE m_fenceEvent;

	// The Fence used for sychronization.
	ID3D12Fence* m_fence;

	// Fence Values - An array of fence values (One for each back buffer in the swap chain)
	UINT64 m_fenceValues[2];

	DXGI_FORMAT m_backBufferFormat;
	UINT m_backBufferCount;

	// The RTV desc Size
	UINT m_rtvDescriptorSize;
};

