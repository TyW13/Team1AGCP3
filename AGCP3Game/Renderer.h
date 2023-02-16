#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include <wrl/client.h>

class Renderer
{
public:
    Renderer(HWND hWnd, int width, int height);
    ~Renderer();

    void Update();
    void Render();

    void Resize(int width, int height);

private:
    HWND m_hWnd;
    int m_width;
    int m_height;

    Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[2];
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;

    UINT m_frameIndex;

    void InitD3D(HWND hWnd);
    void CreateCommandObjects();
    void CreateSwapChain(HWND hWnd);
    void CreateDescriptorHeaps();
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();
};
