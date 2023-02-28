#pragma once
//
#include <memory>
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"
#include "Game.h"

class SpriteRenderer
{
public:

    void Init();
    void LoadTexture();
    void Update();
    void Draw();
    void CreateDeviceDependentResources();

private:

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;

    enum Descriptors
    {
        Cat,
        Count
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
