
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


	HWND m_hwnd;

	int m_width;
	int m_height;

	ID3D12Device* m_device;
	IDXGISwapChain4* m_swapChain;
	ID3D12CommandQueue* m_commandQueue;
	ID3D12DescriptorHeap* m_rtvDescriptorHeap;
	ID3D12Resource* m_renderTargets[2];
	ID3D12CommandAllocator* m_commandAllocator;
	ID3D12GraphicsCommandList* m_commandList;
	UINT m_rtvDescriptorSize;
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ID3D12Fence* m_fence;
	UINT64 m_fenceValue;
};

