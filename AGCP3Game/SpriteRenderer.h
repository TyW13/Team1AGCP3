#pragma once
//
#include <memory>
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"
#include "Game.h"
#include "SimpleMath.h"


#include <d3d12.h>
#include <DirectXMath.h>

class SpriteRenderer {
public:
    SpriteRenderer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, UINT width, UINT height);
    ~SpriteRenderer();

    void SetPosition(float x, float y);
    void SetScale(float scaleX, float scaleY);
    void SetRotation(float angle);
    void Draw();

private:
    struct SpriteVertex {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texCoord;
    };

    struct SpriteConstantBuffer {
        DirectX::XMFLOAT4X4 transform;
    };

    Microsoft::WRL::ComPtr<ID3D12Resource> m_spriteVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_spriteIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_spriteConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_spriteDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_spriteRootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_spritePipelineState;

    UINT m_width;
    UINT m_height;

    SpriteVertex m_vertices[4];
    WORD m_indices[6];

    DirectX::XMFLOAT4X4 m_transform;

    void InitializeResources(ID3D12Device* device);
    void InitializeVertices();
    void InitializeIndices();
    void InitializeConstantBuffer(ID3D12Device* device);
    void InitializeDescriptorHeap(ID3D12Device* device);

    void UpdateConstantBuffer();
    void SetTransform(float x, float y, float scaleX, float scaleY, float angle);
    void UpdateVertices();
};


//class SpriteRenderer
//{
//public:
//
//    void Initialize(ID3D12Device* m_device);
//    void LoadTexture();
//    void Update();
//    void Render();
//    void Draw();
//    void CreateDeviceDependentResources();
//
//    void OnDeviceLost();
//
//private:
//
//
//    // Sprite - CAT
//    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
//    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
//
//    std::unique_ptr<DX::DeviceResources>        m_deviceResources;
//
//    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
//    DirectX::SimpleMath::Vector2 m_screenPos;
//    DirectX::SimpleMath::Vector2 m_origin;
//    RECT m_tileRect;
//
//    std::unique_ptr<DirectX::CommonStates> m_states;
//
//    // Rendering loop timer.
//    DX::StepTimer                               m_timer;
//
//    RECT m_fullscreenRect;
//    Microsoft::WRL::ComPtr<ID3D12Resource> m_background;
//
//
//    enum Descriptors
//    {
//        Cat,
//        Count,
//        Background,
//    };
//};


// 
//class SpriteRenderer
//{
//public:
//    SpriteRenderer();
//    void Update(float deltaTime);
//    void Render();
//
//private:
//    std::unique_ptr<Render> m_render;
//    std::unique_ptr<Texture> m_texture;
//    std::unique_ptr<Sprite> m_sprite;
//
//};
