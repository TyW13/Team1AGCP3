#include "stdafx.h"
#include "Player.h"



Player::Player() : GameObject()
{
    m_speed = 100.0f;
    m_velocity = DirectX::XMFLOAT2(0.0f, 0.0f);
   
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    // Update position based on velocity and delta time
    //m_position.x += m_velocity.x * deltaTime;
    //m_position.y += m_velocity.y * deltaTime;

    // Call parent class update function
    GameObject::Update(deltaTime);
}

void Player::Move(float x, float y)
{
    // Update player velocity
    m_velocity.x = x * m_speed;
    m_velocity.y = y * m_speed;
}