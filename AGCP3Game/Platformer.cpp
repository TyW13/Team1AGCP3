#include "stdafx.h"
//#include "Platformer.h"
//
//
//Platformer::Platformer() :
//    m_d3dDevice(nullptr),
//    m_commandList(nullptr),
//    m_clientWidth(0),
//    m_clientHeight(0)
//{
//}
//
//Platformer::~Platformer()
//{
//    for (auto sprite : m_sprites)
//    {
//        delete sprite;
//    }
//    m_sprites.clear();
//}
//
//HRESULT Platformer::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, UINT clientWidth, UINT clientHeight)
//{
//    m_d3dDevice = device;
//    m_commandList = commandList;
//    m_clientWidth = clientWidth;
//    m_clientHeight = clientHeight;
//
//    return S_OK;
//}
//
//void Platformer::Update(float deltaTime, float totalTime)
//{
//    for (auto sprite : m_sprites)
//    {
//        sprite->Update(deltaTime, totalTime);
//    }
//}
//
//void Platformer::Render()
//{
//    for (auto sprite : m_sprites)
//    {
//        sprite->Render(m_commandList);
//    }
//}
//
//void Platformer::AddSprite(Sprite* sprite)
//{
//    m_sprites.push_back(sprite);
//}
//
//Collision2D Platformer::CheckCollision(Sprite* sprite)
//{
//    Collision2D collision;
//    for (auto s : m_sprites)
//    {
//        if (s == sprite)
//        {
//            continue;
//        }
//
//        Collision2D c = s->CheckCollision(sprite);
//        if (c.type != CollisionType::None)
//        {
//            collision = c;
//            break;
//        }
//    }
//    return collision;
//}
