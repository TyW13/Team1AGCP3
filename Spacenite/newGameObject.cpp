#include "newGameObject.h"
#include "Texture.h"

void GameObject::Init(Texture* tex, DirectX::SimpleMath::Vector2 scale, bool active)
{
}

void GameObject::Update(float dTime)
{
}

Sprite GameObject::GetSprite()
{
    return objSprite;
}

bool GameObject::GetActive()
{
    return isActive;
}

void GameObject::SetSprite(Sprite _sprite)
{
    objSprite = _sprite;
}

void GameObject::SetActive(bool _isActive)
{
    isActive = _isActive;
}
