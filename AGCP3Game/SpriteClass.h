#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <DirectXMath.h>

#include "d3dx12.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texCoord;
};

class Sprite
{
public:
    Sprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    ~Sprite();

    void SetPosition(float x, float y);
    void SetScale(float x, float y);
    void SetTexture(ID3D12Resource* texture, D3D12_SRV_DIMENSION textureViewDimension);
    void Draw();

private:
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateConstantBuffer();
    void CreateRootSignature();
    void CreatePipelineState();

    struct ConstantBufferData
    {
        DirectX::XMFLOAT4X4 transform;
    };

    ID3D12Device* m_device;
    ID3D12GraphicsCommandList* m_commandList;

    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

    ComPtr<ID3D12Resource> m_constantBuffer;
    ConstantBufferData m_constantBufferData;
    UINT8* m_mappedConstantBuffer;

    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12PipelineState> m_pipelineState;

    ComPtr<ID3D12Resource> m_texture;
    D3D12_SHADER_RESOURCE_VIEW_DESC m_textureSRVDesc;

    float m_x;
    float m_y;
    float m_scaleX;
    float m_scaleY;
};