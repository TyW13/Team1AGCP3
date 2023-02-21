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

    void Render();
    void WaitForPreviousFrame(); // Wait for GPU?

private:

    void Init();
    void CreateCommandObjects();
    void CreateSwapChain(HWND hWnd, int width, int height));
    void CreateRenderTargets();
    void CreateDepthStencilBufferAndView();
    void CreateViewPortAndScissorRect();



  
 



    Microsoft::WRL::ComPtr<ID3D12Device> m_device; // FRNK LUNA (md3dDevice)
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[2];
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[2];
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

    /* Microsoft::WRL::ComPtr<ID3D12Descriptor*/

     // PlaceHolder Descriptor Sizes
    UINT mRtvDescriptorSize;
    UINT mDsvDescriptorSize;
    UINT mCbvSrvDescriptorSize;

    UINT64 m_fenceValue;
    HANDLE m_fenceEvent;
    UINT m_frameIndex;
};

