#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>
#include "GameObject.h"

using Microsoft::WRL::ComPtr;


class Player : public GameObject
{
public:
    Player();
    virtual ~Player();

    void Update(float deltaTime);
    void Move(float x, float y);

private:
    float m_speed;
    float m_positionX;
  
    DirectX::XMFLOAT2 m_velocity;
};
    
