#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>


class Renderer;
class Texture;

class Sprite
{
public:
    Sprite(Renderer* renderer);
    ~Sprite();

    void LoadFromFile(const std::wstring& filename);
    void SetTexture(Texture* texture);
    void SetTransform(const DirectX::XMFLOAT2& position, float rotation, const DirectX::XMFLOAT2& scale);

    void Render();

private:
    std::unique_ptr<Texture> m_texture;
    DirectX::XMFLOAT2 m_position;
    float m_rotation;
    DirectX::XMFLOAT2 m_scale;

    Renderer* m_renderer;

    struct SpriteVertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    
    void CreateVertexBuffer();
    void CreatePipelineState();
    void CreateRootSignature();
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


