#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>
#include "Renderer.h"

class Renderer;
class Texture;


#include <string>

// Replaced 28/02/23 01:03
class Sprite
{
public:
    Sprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    virtual ~Sprite();

    bool LoadFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const std::wstring& fileName);

    void SetTexture(ID3D12Resource* texture);

    void SetTransform(const DirectX::XMFLOAT2& position, float rotation, const DirectX::XMFLOAT2& scale);

    void Update(float deltaTime);

    void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap);

private:
    struct SpriteVertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };

    struct ConstantBuffer
    {
	    DirectX::XMMATRIX transform;
    };

    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateConstantBuffer(ID3D12Device* device);
    void CreatePipelineState(ID3D12Device* device);

    ID3D12Device* m_device;
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    ID3D12Resource* m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    ID3D12Resource* m_texture;
    ID3D12DescriptorHeap* m_textureHeap;
    UINT m_textureWidth;
    UINT m_textureHeight;
    
    float m_rotation;
   
    ConstantBuffer m_constantBufferData;
    ID3D12Resource* m_constantBuffer;
    ID3D12Resource* m_mappedConstantBuffer;
    D3D12_GPU_VIRTUAL_ADDRESS m_constantBufferAddress;
    ID3D12PipelineState* m_pipelineState;

   
    DirectX::XMFLOAT2 m_position;
    DirectX::XMFLOAT2 m_scale;
    DirectX::XMMATRIX m_transformMatrix;


    UINT m_numIndices;
};





//class Sprite
//{
//public:
//    Sprite();
//    ~Sprite();
//
//    bool LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const std::wstring& fileName);
//    void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* heap, UINT descriptorSize, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState);
//    void SetPosition(float x, float y);
//
//private:
//    struct Vertex
//    {
//        XMFLOAT3 position;
//        XMFLOAT2 uv;
//    };
//
//    ID3D12Resource* m_texture;
//    D3D12_GPU_DESCRIPTOR_HANDLE m_textureDescriptorHandle;
//
//    ID3D12Resource* m_vertexBuffer;
//    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
//
//    ID3D12Resource* m_indexBuffer;
//    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
//
//    XMFLOAT2 m_position;
//    XMFLOAT2 m_size;
//    XMFLOAT4X4 m_transform;
//
//    void InitializeVertexBuffer();
//    void InitializeIndexBuffer();
//};


