// Include necessary headers
#include <Windows.h>

#include "pch.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgi1_2.h>
#include <iostream>
#include <wrl/client.h>
using namespace Microsoft::WRL;
using Microsoft::WRL::ComPtr;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = nullptr; } }
#endif

// Define window class name
const wchar_t CLASS_NAME[] = L"DirectX12WindowClass";

// Declare global variables
HWND m_hWnd;
bool m_bQuit = false;

// Declare functions
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);
void InitD3D();
void PopulateCommandList();
void MainLoop();
void Cleanup();


// Function declaration
Microsoft::WRL::ComPtr<IDXGIAdapter1> GetHardwareAdapter(IDXGIFactory1* pFactory);

//m_pd3dDevice:  a pointer to the Direct3D device
//m_pd3dCommandList : a pointer to the Direct3D command list
//m_pd3dCommandAllocator : an array of command allocator objects for each frame in flight
//m_CommandQueue : a pointer to the Direct3D command queue
//m_BackBuffers : an array of pointers to the back buffers
//m_RTVHeap : a pointer to the render target view(RTV) heap
//m_SwapChain : a pointer to the Direct3D swap chain
//m_rtvDescriptorHeap : a pointer to the RTV descriptor heap
//m_rtvDescriptorSize : the size of an RTV descriptor in bytes
//m_renderTargets : an array of RTV objects for each frame in flight
//m_frameIndex : the index of the current frame in flight
//m_commandAllocators : an array of command allocator objects for each frame in flight
//m_fenceValue : the current value of the fence
//m_fenceEvent : a handle to the fence event
//FrameCount : the number of frames in flight

// Global variables

const int FrameCount = 3;



// Device resources
ComPtr<ID3D12Device>             m_device;
UINT                             FrameCount;
HANDLE                           m_fenceEvent;
UINT64                           m_fenceValue;
ComPtr<ID3D12CommandAllocator>   m_commandAllocators[FrameCount];
UINT                             m_frameIndex;
ComPtr<ID3D12Resource>           m_renderTargets[FrameCount];
UINT                             m_rtvDescriptorSize;
ComPtr<ID3D12DescriptorHeap>     m_rtvDescriptorHeap;
ComPtr<ID3D12GraphicsCommandList> m_pd3dCommandList;
ComPtr<ID3D12Device>             m_pd3dDevice;
ComPtr<ID3D12CommandAllocator>   m_pd3dCommandAllocator;
ComPtr<ID3D12CommandQueue>       m_pd3dCommandQueue;
ComPtr<ID3D12Fence>              m_fence;
ComPtr<IDXGISwapChain3>          m_swapChain;
ComPtr<ID3D12DescriptorHeap>     m_rtvHeap;
CD3DX12_CPU_DESCRIPTOR_HANDLE    m_rtvHandle;

// Window resources
int                              m_ClientWidth;
int                              m_ClientHeight;
HWND                             m_hwnd;

// DirectX 12 objects
ComPtr<IDXGIFactory4>            m_dxgiFactory;
ComPtr<IDXGIAdapter1>            m_adapter;

ComPtr<ID3D12CommandAllocator>   m_commandAllocator;
ComPtr<ID3D12CommandQueue>       m_commandQueue;
ComPtr<ID3D12RootSignature>      m_rootSignature;

ComPtr<ID3D12PipelineState>      m_pipelineState;
ComPtr<ID3D12GraphicsCommandList> m_commandList;



// Entry point for Windows applications
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create window
    CreateWindowHandle(hInstance, nCmdShow);

    // Initialize Direct3D 12
    InitD3D();

    // Main loop
    MainLoop();

    // Cleanup
    Cleanup();

    return 0;
}

// Window message handler
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

// Creates the window
void CreateWindowHandle(HINSTANCE hInstance, int nCmdShow)
{
    // Create window
    m_hWnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"DirectX 12 Window",           // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        CW_USEDEFAULT, CW_USEDEFAULT,   // Size
        NULL,                           // Parent window    
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    // If window creation failed, exit the application
    if (m_hWnd == NULL)
    {
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Show window
    ShowWindow(m_hWnd, nCmdShow);
}

void GetHardwareAdapter(ComPtr<IDXGIFactory4>::InterfaceType* get, const Details::ComPtrRef<ComPtr<IDXGIAdapter1>>& adapter);

// Initializes Direct3D 12
void InitD3D()
{
    // TODO: Implement

    // 1. Create the device and swap chain

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Width = 800;
    swapChainDesc.Height =600;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;
    DX::ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory)));

    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    GetHardwareAdapter(dxgiFactory.Get(), &adapter);

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    DX::ThrowIfFailed(D3D12CreateDevice(adapter.Get(), featureLevel, IID_PPV_ARGS(&m_device)));

    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    DX::ThrowIfFailed(m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue)));

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
    fullScreenDesc.RefreshRate.Numerator = 60;
    fullScreenDesc.RefreshRate.Denominator = 1;
    fullScreenDesc.Windowed = TRUE;

    DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),
        m_hWnd,
        &swapChainDesc,
        &fullScreenDesc,
        nullptr,
        &m_swapChain
    ));

    // 2. Create a RTV descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    DX::ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvDescriptorHeap)));



    // 3. Create the render target views

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < 2; i++)
    {
	    DX::ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i])));
        m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);
    }


    // 4. Create the command allocator and command list

    DX::ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
    DX::ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator->Get(), nullptr, IID_PPV_ARGS(&m_CommandList)));

    // 5. Create the fence and event handle

    DX::ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
    {
	    DX::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }


    // 6. Create the command list
    HRESULT hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators, nullptr, IID_PPV_ARGS(&m_pd3dCommandList));
    if (FAILED(hr))
    {
        Cleanup();
        return false;
    }

    // Command lists are created in the recording state. Since there is nothing to
    // record right now and the main loop expects it to be closed, we close it now.
    hr = m_commandList->Close();
    if (FAILED(hr))
    {
        Cleanup();
        return false;
    }

    // 7. Create frame resources.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV for each frame.
    for (UINT n = 0; n < FrameCount; n++)
    {
	    DX::ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_backBuffers[n])));
        m_device->CreateRenderTargetView(m_backBuffers[n], nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);

        DX::ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
    }


    // 8. Create a RTV for each frame
    // We need to create a RTV for each frame in the swap chain

    for (UINT n = 0; n < FrameCount; n++)
    {
	    DX::ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
        m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);
    }


    // 9. Create a Command Allocator for each frame.
    // We need to create a command allocator for each frame to manage the memory that is used for
    // command lists.

    for (UINT n = 0; n < FrameCount; n++)
    {
	    DX::ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
    }

    // 10. Create the Command List and close it.
    // We need to create a command list and close it. The command list is used to store the
    // commands that will be executed by the GPU.

    DX::ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
    DX::ThrowIfFailed(m_commandList->Close());

    // 11. Create a Fence and event Handle
    // We need to create a fence to synchronize the CPU & GPU. The Fence is used to signal when the GPU has completed executing
    // a command list. We also need to create an event handle to wait for the fence to be signaled.

    DX::ThrowIfFailed(m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    m_fenceValue++;
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
    {
	    DX::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }


}

// Populates the command list
void PopulateCommandList()
{
    // TODO: Implement
}

// Main game loop
void MainLoop()
{
    // Enter the message loop
    MSG msg = {};
    while (!m_bQuit)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Render the frame
        PopulateCommandList();

        // TODO: Implement rendering
    }
}


void WaitForPreviousFrame()
{
    // Wait for the fence value for the current frame to be signaled
    const UINT64 fence = m_fenceValue[m_frameIndex];
    if (m_fence->GetCompletedValue() < fence)
    {
        m_fence->SetEventOnCompletion(fence, m_fenceEvent);
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    // Increment the fence value for the current frame
    m_fenceValue[m_frameIndex]++;
}

void Close()
{
    // Wait for the GPU to complete all previous operations
    WaitForPreviousFrame();

    // Close the fence event
    CloseHandle(m_fenceEvent);

    // Release the resources used by Direct3D
    for (int i = 0; i < FrameCount; i++)
    {
        SAFE_RELEASE(m_renderTargets[i]);
        SAFE_RELEASE(m_backBuffers[i]);
        SAFE_RELEASE(m_commandAllocators[i]);
    }

    SAFE_RELEASE(m_rtvDescriptorHeap);
    SAFE_RELEASE(m_commandList);
    SAFE_RELEASE(m_commandAllocator);
    SAFE_RELEASE(m_commandQueue);
    SAFE_RELEASE(m_device);
    SAFE_RELEASE(m_swapChain);
}

// Cleans up Direct3D 12 resources
void Cleanup()
{
    // TODO: Implement
}
