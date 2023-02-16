#pragma once


using namespace Microsoft::WRL;
using namespace Windows::Foundation;

using namespace DirectX;

class Renderer {
public:
    Renderer(ID3D12Device* device, HWND hwnd, int width, int height);
    ~Renderer();

    void Render();

private:
    // DirectX objects
    ID3D12Device*                 m_device;
    IDXGISwapChain3*              m_swapChain;
    ID3D12CommandQueue*           m_commandQueue;

    ID3D12DescriptorHeap*         m_rtvDescriptorHeap;
    UINT				          m_rtvDescriptorSize;


    ID3D12DescriptorHeap*         m_dsvDescriptorHeap;
 
    ID3D12Resource*               m_backBufferRenderTarget[2];

    ID3D12GraphicsCommandList*    m_commandList;

    ID3D12CommandAllocator*       m_commandAllocator;

    D3D12_RENDER_TARGET_VIEW_DESC m_renderTargets;


    ComPtr<ID3D12Fence>					fence;
    UINT64								fenceValue = 0;

    HANDLE								fenceEvent;



    //

    //// Window size
    //int m_width;
    //int m_height;

    //// Frame synchronization
    //UINT m_frameIndex;
    //HANDLE m_fenceEvent;
    //ID3D12Fence* m_fence;
    //UINT64 m_fenceValue;

    //// Initialization methods
    //void CreateDevice();
    //void CreateSwapChain(HWND hwnd);
    //void CreateCommandObjects();
    //void CreateRenderTargetViews();
    //void CreateFence();
    //void WaitForPreviousFrame();
};

