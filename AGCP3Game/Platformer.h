#pragma once

#include <vector>
#include "Collision2D.h"
#include "SpriteClass.h"

class Platformer2D
{
public:
    Platformer2D(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvDescriptorHeap, int clientWidth, int clientHeight);
    ~Platformer2D();

    void Update(float deltaTime);
    void Render();

    
    void LoadLevel(const char* levelFileName);

private:
    void CreateBoundingBoxes();

    ID3D12Device* m_d3dDevice;
    ID3D12GraphicsCommandList* m_commandList;
    ID3D12DescriptorHeap* m_srvDescriptorHeap;

    int m_clientWidth;
    int m_clientHeight;

    Sprite* m_background;
    std::vector<Sprite*> m_sprites;
    std::vector<Collision2D*> m_collisionBoxes;

    Sprite* m_player;
    Collision2D* m_playerCollisionBox;
};
