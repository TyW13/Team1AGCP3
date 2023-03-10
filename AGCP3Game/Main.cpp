#include <d3d12.h>
#include <DirectXHelpers.h>
#include <dxgi1_4.h>
#include <windows.h>

#include "d3dx12.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Global variables
IDXGISwapChain3* g_pSwapChain = nullptr;
ID3D12Device* g_pDevice = nullptr;
ID3D12CommandQueue* g_pCommandQueue = nullptr;
ID3D12DescriptorHeap* g_pRTVDescriptorHeap = nullptr;
ID3D12Resource* g_pRenderTarget[2] = { nullptr, nullptr };
UINT g_nRTVDescriptorSize = 0;
int g_nBackBufferIndex = 0;



// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Entry point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register window class
    const wchar_t CLASS_NAME[] = L"HACKY2D";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,                          // Optional window styles.
        CLASS_NAME,                 // Window class
        L"DirectX12 Window",        // Window text
        WS_OVERLAPPEDWINDOW,        // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,  // Position and size
        nullptr,                    // Parent window
        nullptr,                    // Menu
        hInstance,                  // Instance handle
        nullptr                     // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Create device and swap chain
    CreateDevice();
    CreateCommandQueue();
    CreateSwapChain(hwnd);
    CreateDescriptorHeap();
    CreateRenderTarget();

    // Show window
    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Render frame
        PopulateCommandList();
        WaitForPreviousFrame();
    }

    return 0;
}


// Create device
void CreateDevice()
{
    // Create device
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&g_pDevice));
}

// Create command queue
void CreateCommandQueue()
{
    // Create command queue
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    g_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pCommandQueue));
}

// Create swap chain
void CreateSwapChain(HWND hwnd)
{
    // Get DXGI factory
    IDXGIFactory4* pFactory = nullptr;
    CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));

    // Create swap chain
    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.BufferCount = 2;
    desc.Width = WINDOW_WIDTH;
    desc.Height = WINDOW_HEIGHT;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;

    IDXGISwapChain1* pSwapChain = nullptr;
    pFactory->CreateSwapChainForHwnd(
        g_pCommandQueue,
        hwnd,
        &desc,
        nullptr,
        nullptr,
        &pSwapChain
    );

    // Upgrade swap chain to IDXGISwapChain3
    pSwapChain->QueryInterface(IID_PPV_ARGS(&g_pSwapChain));
    pSwapChain->Release();

    // Release factory
    pFactory->Release();

}

// Create descriptor heap
void CreateDescriptorHeap()
{
    // Get descriptor heap size
    g_nRTVDescriptorSize = g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


    // Create descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = 2;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    g_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pRTVDescriptorHeap));


}

// Create render target
void CreateRenderTarget()
{
    // Get current back buffer index
    g_nBackBufferIndex = g_pSwapChain->GetCurrentBackBufferIndex();

    // Create render target for each back buffer
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_pRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < 2; i++)
    {
        g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&g_pRenderTarget[i]));
        g_pDevice->CreateRenderTargetView(g_pRenderTarget[i], nullptr, rtvHandle);
        rtvHandle.Offset(1, g_nRTVDescriptorSize);
    }

}

// Populate command list
void PopulateCommandList()
{
    // Reset command allocator and command list
    ID3D12CommandAllocator* pCommandAllocator = nullptr;
    ID3D12GraphicsCommandList* pCommandList = nullptr;
    g_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAllocator));
    g_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCommandAllocator, nullptr, IID_PPV_ARGS(&pCommandList));
    pCommandAllocator->Reset();
    pCommandList->Reset(pCommandAllocator, nullptr);

    // Set render target
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_pRenderTarget[g_nBackBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    pCommandList->ResourceBarrier(1, &barrier);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_pRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), g_nBackBufferIndex, g_nRTVDescriptorSize);
    pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Clear render target
    const float clearColor[] = { 0.0f, 0.0f,0.0f };
    
	// Render to back buffer
    pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    pCommandList->DrawInstanced(3, 1, 0, 0);

    // Set back buffer to present state
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(g_pRenderTarget[g_nBackBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    pCommandList->ResourceBarrier(1, &barrier);

    // Close command list
    pCommandList->Close();

    // Execute command list
    ID3D12CommandList* ppCommandLists[] = { pCommandList };
    g_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present frame
    g_pSwapChain->Present(1, 0);

    // Wait for command queue to finish
    WaitForPreviousFrame();

    // Release command allocator and command list
    pCommandAllocator->Release();
    pCommandList->Release();


}


void InitD3D()
{
    // Create the device
    CreateDevice();

    // Create the command queue
    CreateCommandQueue();

    // Create the swap chain
    CreateSwapChain();

    // Create the render target 
    CreateRenderTarget();

    // Create the command allocator
    CreateCommandAllocator();

    // Create the command list
    CreateCommandList();

    // Create the fence and event handle
    CreateFenceAndEvent();

    // Wait for the fence to be signaled
    WaitForPreviousFrame();
}


bool g_bQuit = false;

// Main loop
void MainLoop()
{
    while (!g_bQuit)
    {
        MSG msg;
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            PopulateCommandList();
        }
    }
}

// Cleanup
void Cleanup()
{
    // Wait for command queue to finish
    WaitForPreviousFrame();

    // Release resources
    g_pRenderTarget[0]->Release();
    g_pRenderTarget[1]->Release();
    g_pRTVDescriptorHeap->Release();
    g_pSwapChain->Release();
    g_pCommandQueue->Release();
    g_pDevice->Release();

}


//// WinMain entry point
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//    // Create window
//    CreateWindowHandle(hInstance, nShowCmd);
//
//    // Initialize Direct3D 12
//    InitD3D();
//
//    // Main loop
//    MainLoop();
//
//    // Cleanup
//    Cleanup();
//
//    return 0;
//
//}
