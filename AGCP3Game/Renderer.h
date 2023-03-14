
#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 Model;
	DirectX::XMFLOAT4X4 ViewProjection;
	DirectX::XMFLOAT4 Color;
};



class Renderer
{
public:
	Renderer(HWND hwnd, int width, int height);
	~Renderer();

	void RenderFrame();

private:
	void CreateDevice();
	void CreateFence();
	void CreateCommandQueue();
	void CreateCommandList();
	void CreateSwapChain();
	void CreateDescriptorHeaps();
	void CreateRenderTargetView();


	void CreateDepthStencilBuffer();
	void CreateCommandAllocator();
	
	void PopulateCommandList();
	void WaitForPreviousFrame();

	// Render Sprite
	void RenderSprite(const wchar_t* filename, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& scale);
	Microsoft::WRL::ComPtr<ID3D12Resource> LoadTexture(const wchar_t* filename);

	// Sprite related Resources

	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferUploadHeap;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	Microsoft::WRL::ComPtr <ID3D12RootSignature> g_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvHeap;


	Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBufferUploadHeap;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	UINT m_indexBufferSize;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_textureUploadHeap;
	

	Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;
	ConstantBuffer* m_mappedConstantBuffer;


	
	const int m_bufferCount = 2;
	int m_currentFrameIndex;


	DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer; // This is the depth stencil buffer resource.

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
	ID3D12DescriptorHeap* m_dsvDescriptorHeap;

	// An array of render targets 
	ID3D12Resource* m_renderTargets[2];

	// An array of command allocators 
	ID3D12CommandAllocator* m_commandAllocator;

	// The Command List used for recording rendering commands
	ID3D12GraphicsCommandList* m_commandList;



	// Frame Index
	UINT m_frameIndex;

	// The event used for waiting for the fence value to be signaled.
	HANDLE m_fenceEvent;

	// The Fence used for sychronization.
	ID3D12Fence* m_fence;

	// Fence Values - 
	UINT64 m_fenceValue;

	DXGI_FORMAT m_backBufferFormat;
	UINT m_backBufferCount;

	// The RTV desc Size
	UINT m_rtvDescriptorSize;
};

