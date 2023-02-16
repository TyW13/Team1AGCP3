
#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
}

GameObject::GameObject(float x, float y, float width, float height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

GameObject::~GameObject()
{

}

void GameObject::Update(float deltaTime)
{
    // Implement in child classes as needed
}

void GameObject::Render()
{
    // Implement in child classes as needed
}

bool GameObject::IsColliding(GameObject* other)
{
    float left1 = m_x - m_width / 2.0f;
    float right1 = m_x + m_width / 2.0f;
    float top1 = m_y - m_height / 2.0f;
    float bottom1 = m_y + m_height / 2.0f;

    float left2 = other->GetX() - other->GetWidth() / 2.0f;
    float right2 = other->GetX() + other->GetWidth() / 2.0f;
    float top2 = other->GetY() - other->GetHeight() / 2.0f;
    float bottom2 = other->GetY() + other->GetHeight() / 2.0f;

    return !(left1 > right2 || left2 > right1 || top1 > bottom2 || top2 > bottom1);
}



