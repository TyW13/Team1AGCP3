#include "Render.h"

Render::Render(HWND hwnd, int width, int height)
{

}

void Render::RenderFrame()
{

}

void Render::CreateDevice()
{
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));

}

void Render::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue));
}

void Render::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.BufferCount = 2;
    desc.Width = m_width;
    desc.Height = m_height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;

    IDXGIFactory4* factory;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    factory->CreateSwapChainForHwnd(m_commandQueue, m_hwnd, &desc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&m_swapChain));

    factory->Release();
}

void Render::CreateRenderTargetView()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = 2;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
}

void Render::CreateCommandAllocator()
{

}

void Render::CreateCommandList()
{

}

void Render::PopulateCommandList()
{

}

void Render::WaitForPreviousFrame()
{

}
