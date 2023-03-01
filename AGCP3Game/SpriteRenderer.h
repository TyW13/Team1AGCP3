#pragma once
//
#include <memory>
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"
#include "Game.h"
#include "SimpleMath.h"

class SpriteRenderer
{
public:

    void Initialize(ID3D12Device* m_device);
    void LoadTexture();
    void Update();
    void Render();
    void Draw();
    void CreateDeviceDependentResources();

    void OnDeviceLost();

private:


    // Sprite - CAT
    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;

    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;
    RECT m_tileRect;

    std::unique_ptr<DirectX::CommonStates> m_states;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    RECT m_fullscreenRect;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_background;


    enum Descriptors
    {
        Cat,
        Count,
        Background,
    };
};


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
