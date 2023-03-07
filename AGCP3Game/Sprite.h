#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>
#include "Renderer.h"



#include <string>

#pragma once

////
#include <SimpleMath.h>
#include <string>
#include <wrl/client.h>

class Sprite
{
public:
    Sprite(ID3D12Device* m_device);
    ~Sprite();

    bool LoadFromFile(const std::wstring& fileName);
    void SetTexture(ID3D12Resource* texture);
    // comment
    void setPosition(float x, float y);
    void SetVelocity(float x, float y);
    void SetScale(float x, float y);
    void SetRotation(float angle);


	void SetTransform(const DirectX::XMFLOAT2& position, float rotation = 0.0f, const DirectX::XMFLOAT2& scale = DirectX::XMFLOAT2(1.0f, 1.0f));
    void Update(float deltaTime);
    D3D12_GPU_DESCRIPTOR_HANDLE GetSRV();
    void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap);


    // Animation Getters - added 07/03/2023

    RECT mTexRect;

    DirectX::SimpleMath::Vector2 scale;

    void SetTexRect(const RECT& texRect);

    void SetScale(const DirectX::SimpleMath::Vector2& s) {
        scale = s;
    }

    const DirectX::SimpleMath::Vector2& GetScale() const {
        return scale;
    }

private:

    // Vertex Buffer holds Pos, tex coords
    void CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

    struct SpriteVertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texCoord;
        DirectX::XMFLOAT4 color;
    };

    struct ConstantBuffer
    {
        DirectX::XMFLOAT4X4 transform;

        DirectX::XMMATRIX modelMatrix;
        DirectX::XMMATRIX viewMatrix;
        DirectX::XMMATRIX projectionMatrix;
    };

    // The Command List used for recording rendering commands
    ID3D12GraphicsCommandList* m_commandList;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

    ID3D12Device* m_device;
    ID3D12Resource* m_texture;
    ID3D12Resource* m_vertexBuffer;
    ID3D12Resource* m_indexBuffer;
    ID3D12Resource* m_constantBuffer;

    ID3D12Resource* m_srvIndex;
    ID3D12Resource* m_srvHeap;
    ID3D12Resource* m_textureIndex;
    /*ID3D12Resource* m_srvDescriptorSize;*/

 
    int m_textureWidth;
    int m_textureHeight;
    DXGI_FORMAT m_textureFormat;


    // The render target view RTV heap used for storing render targets

    ID3D12DescriptorHeap* m_rtvDescriptorHeap;
    ID3D12DescriptorHeap* m_dsvDescriptorHeap;

    ID3D12DescriptorHeap& m_srvDescriptorSize;

    // The RTV desc Size
    UINT m_rtvDescriptorSize;

    DirectX::XMFLOAT2 m_position;
    DirectX::XMFLOAT2 m_size;
    DirectX::XMFLOAT4 m_color;

    DirectX::XMFLOAT2 m_scale;
    DirectX::XMFLOAT2 m_velocity;

    float m_positionX;
    float m_positionY;

    float m_scaleX;
    float m_ccaleY;

    float m_rotation;

    float m_velocityX;
    float m_velocityY;



    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_ConstantBuffer;
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_ConstantBufferView;

    ConstantBuffer m_constantBufferData;

    unsigned int m_vertexCount;
    unsigned int m_indexCount;

    

    UINT8* m_pConstantBufferDataBegin;

    D3D12_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;


 
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


