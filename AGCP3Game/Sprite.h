#pragma once

#include <d3d12.h>
#include <DirectXMath.h>



#include <string>


using namespace DirectX;

class Sprite
{
public:
    Sprite();
    ~Sprite();

    bool LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const std::wstring& fileName);
    void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* heap, UINT descriptorSize, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState);
    void SetPosition(float x, float y);

private:
    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT2 uv;
    };

    ID3D12Resource* m_texture;
    D3D12_GPU_DESCRIPTOR_HANDLE m_textureDescriptorHandle;

    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    ID3D12Resource* m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

    XMFLOAT2 m_position;
    XMFLOAT2 m_size;
    XMFLOAT4X4 m_transform;

    void InitializeVertexBuffer();
    void InitializeIndexBuffer();
};


