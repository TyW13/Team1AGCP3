#include "newGameObject.h"
#include "Texture.h"

void GameObject::Init()
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
