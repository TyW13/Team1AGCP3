#include "Graphics.h"

Graphics::Graphics(HWND hWnd, int width, int height)
{
    // Constructor implementation
    // ...
}

Graphics::~Graphics()
{
    // Destructor implementation
    // ...
}

void Graphics::BeginFrame()
{
    // BeginFrame implementation
    // ...
}

void Graphics::EndFrame()
{
    // EndFrame implementation
    // ...
}

void Graphics::DrawDebugLine(DirectX::SimpleMath::Vector3 startPoint, DirectX::SimpleMath::Vector3 endPoint, DirectX::SimpleMath::Vector4 color)
{
    // DrawDebugLine implementation
    // ...
    //{
    //    // Create a vertex buffer for the line
    //    SimpleVertex vertices[] =
    //    {
    //        { startPoint, color },
    //        { endPoint, color }
    //    };
    //    const UINT vertexBufferSize = sizeof(vertices);

    //    ComPtr<ID3D12Resource> vertexBuffer;
    //    ThrowIfFailed(m_device->CreateCommittedResource(
    //        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
    //        D3D12_HEAP_FLAG_NONE,
    //        &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
    //        D3D12_RESOURCE_STATE_GENERIC_READ,
    //        nullptr,
    //        IID_PPV_ARGS(vertexBuffer.GetAddressOf())));

    //    void* mappedBuffer = nullptr;
    //    CD3DX12_RANGE readRange(0, 0);
    //    ThrowIfFailed(vertexBuffer->Map(0, &readRange, &mappedBuffer));
    //    memcpy(mappedBuffer, vertices, vertexBufferSize);
    //    vertexBuffer->Unmap(0, nullptr);

    //    // Draw the line
    //    ID3D12CommandList* commandLists[] = { m_commandList.Get() };
    //    m_commandAllocator->Reset();
    //    m_commandList->Reset(m_commandAllocator.Get(), nullptr);

    //    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    //    m_commandList->SetPipelineState(m_pipelineState.Get());

    //    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    //    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    //    vertexBufferView.StrideInBytes = sizeof(SimpleVertex);
    //    vertexBufferView.SizeInBytes = vertexBufferSize;

    //    m_commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
    //    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
    //    m_commandList->DrawInstanced(2, 1, 0, 0);

    //    ThrowIfFailed(m_commandList->Close());
    //    m_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    //    WaitForGpu();
    //}
}