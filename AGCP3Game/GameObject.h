#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class GameObject
{
public:
    GameObject();
    GameObject(float x, float y, float width, float height);
    virtual ~GameObject();

    // Getters and setters
    float GetX() const { return m_x; }
    void SetX(float x) { m_x = x; }
    float GetY() const { return m_y; }
    void SetY(float y) { m_y = y; }
    float GetWidth() const { return m_width; }
    void SetWidth(float width) { m_width = width; }
    float GetHeight() const { return m_height; }
    void SetHeight(float height) { m_height = height; }

    // Update and render
    virtual void Update(float deltaTime);
    virtual void Render();

    // Check for collision with another game object
    bool IsColliding(GameObject* other);

protected:
    float m_x, m_y;
    float m_width, m_height;
};