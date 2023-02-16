#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <DirectXMath.h>

class Sprite
{
public:
    Sprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const wchar_t* fileName, int spriteWidth, int spriteHeight);
    ~Sprite();

    void Update(float deltaTime);
    void Draw();

    void SetPosition(float x, float y);
    void SetVelocity(float x, float y);
    void SetScale(float scale);
    void Move(float dx, float dy);

    //bool LoadTextureFromFile(ID3D12Device* device, const wchar_t* fileName);

private:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };

    void CreateDeviceDependentResources(const wchar_t* fileName);
    void CreateTextureResource(const wchar_t* fileName);
    void CreateVertexBuffer(int spriteWidth, int spriteHeight);

    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvDescriptorHeap;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

    DirectX::XMFLOAT2 m_position;
    DirectX::XMFLOAT2 m_velocity;
    float m_scale;
    float m_rotation;
    
    float m_x, m_y;

    bool m_loadingComplete;

    const int m_vertexBufferSize = 6;
    const int m_numFrames = 1;
    int m_currentFrame = 0;

    ID3D12Device* m_device;
};