// Include necessary headers
#include <Windows.h>

#include "pch.h"

// Define window class name
const wchar_t CLASS_NAME[] = L"DirectX12WindowClass";

// Declare global variables
HWND g_hWnd;
bool g_bQuit = false;

// Declare functions
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);
void InitD3D();
void PopulateCommandList();
void MainLoop();
void Cleanup();


// Global variables
Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
UINT FrameCount;
HANDLE g_fenceEvent;
UINT64 g_fenceValue;

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> g_commandAllocators[FrameCount];
UINT g_frameIndex;
Microsoft::WRL::ComPtr<ID3D12Resource> g_renderTargets[FrameCount];

UINT g_rtvDescriptorSize;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_rtvDescriptorHeap;
Microsoft::WRL::ComPtr<ID3D12CommandList> g_CommandList;
Microsoft::WRL::ComPtr<ID3D12CommandList> g_pd3dCommandList;

Microsoft::WRL::ComPtr<ID3D12Device> g_pd3dDevice;

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> g_pd3dCommandAllocator;

Microsoft::WRL::ComPtr<ID3D12Resource> g_BackBuffers[FrameCount];

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_RTVHeap;

Microsoft::WRL::ComPtr<IDXGISwapChain3> g_SwapChain;

Microsoft::WRL::ComPtr<ID3D12CommandQueue> g_CommandQueue;

// Function declaration
Microsoft::WRL::ComPtr<IDXGIAdapter1> GetHardwareAdapter(IDXGIFactory1* pFactory);

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
    g_hWnd = CreateWindowEx(
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
    if (g_hWnd == NULL)
    {
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Show window
    ShowWindow(g_hWnd, nCmdShow);
}

// Initializes Direct3D 12
void InitD3D()
{
    // TODO: Implement

    // 1. Create the device and swap chain

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Width = g_WindowWidth;
    swapChainDesc.Height = g_WindowHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;
    DX::ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory)));

    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    GetHardwareAdapter(dxgiFactory.Get(), &adapter);

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    DX::ThrowIfFailed(D3D12CreateDevice(adapter.Get(), featureLevel, IID_PPV_ARGS(&g_Device)));

    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    DX::ThrowIfFailed(g_Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&g_CommandQueue)));

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
    fullScreenDesc.RefreshRate.Numerator = 60;
    fullScreenDesc.RefreshRate.Denominator = 1;
    fullScreenDesc.Windowed = TRUE;

    DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
        g_CommandQueue.Get(),
        g_hWnd,
        &swapChainDesc,
        &fullScreenDesc,
        nullptr,
        &g_SwapChain
    ));

    // 2. Create a RTV descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    DX::ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_rtvDescriptorHeap)));



    // 3. Create the render target views

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < 2; i++)
    {
	    DX::ThrowIfFailed(g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_BackBuffers[i])));
        g_Device->CreateRenderTargetView(g_BackBuffers[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, g_RTVDescriptorSize);
    }


    // 4. Create the command allocator and command list

    DX::ThrowIfFailed(g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_CommandAllocator)));
    DX::ThrowIfFailed(g_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&g_CommandList)));

    // 5. Create the fence and event handle

    DX::ThrowIfFailed(g_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence)));
    g_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (g_FenceEvent == nullptr)
    {
	    DX::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }


    // 6. Create the command list
    HRESULT hr = g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_pd3dCommandAllocator, nullptr, IID_PPV_ARGS(&g_pd3dCommandList));
    if (FAILED(hr))
    {
        Cleanup();
        return false;
    }

    // Command lists are created in the recording state. Since there is nothing to
    // record right now and the main loop expects it to be closed, we close it now.
    hr = g_pd3dCommandList->Close();
    if (FAILED(hr))
    {
        Cleanup();
        return false;
    }

    // 7. Create frame resources.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV for each frame.
    for (UINT n = 0; n < FrameCount; n++)
    {
	    DX::ThrowIfFailed(g_SwapChain->GetBuffer(n, IID_PPV_ARGS(&g_BackBuffers[n])));
        g_pd3dDevice->CreateRenderTargetView(g_BackBuffers[n], nullptr, rtvHandle);
        rtvHandle.Offset(1, g_rtvDescriptorSize);

        DX::ThrowIfFailed(g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocators[n])));
    }


    // 8. Create a RTV for each frame
    // We need to create a RTV for each frame in the swap chain

    for (UINT n = 0; n < FrameCount; n++)
    {
	    DX::ThrowIfFailed(g_SwapChain->GetBuffer(n, IID_PPV_ARGS(&g_renderTargets[n])));
        g_Device->CreateRenderTargetView(g_renderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, g_rtvDescriptorSize);
    }


    // 9. Create a Command Allocator for each frame.
    // We need to create a command allocator for each frame to manage the memory that is used for
    // command lists.

    for (UINT n = 0; n < FrameCount; n++)
    {
	    DX::ThrowIfFailed(g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocators[n])));
    }

    // 10. Create the Command List and close it.
    // We need to create a command list and close it. The command list is used to store the
    // commands that will be executed by the GPU.

    DX::ThrowIfFailed(g_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocators[g_frameIndex].Get(), nullptr, IID_PPV_ARGS(&g_commandList)));
    DX::ThrowIfFailed(g_CommandList->Close());

    // 11. Create a Fence and event Handle
    // We need to create a fence to synchronize the CPU & GPU. The Fence is used to signal when the GPU has completed executing
    // a command list. We also need to create an event handle to wait for the fence to be signaled.

    DX::ThrowIfFailed(g_Device->CreateFence(g_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)));
    g_fenceValue++;
    g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (g_fenceEvent == nullptr)
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
    while (!g_bQuit)
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

// Cleans up Direct3D 12 resources
void Cleanup()
{
    // TODO: Implement
}
