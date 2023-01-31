#include "newGameObject.h"
#include "Texture.h"

void GameObject::Init(Texture* tex)
{
    isActive = true;
    ID3D11ShaderResourceView* p = gSprite.GetD3D().GetCache().LoadTexture(&gSprite.GetD3D().GetDevice(), "asteroid.dds", tex->GetName(), false);

}

Sprite GameObject::GetSprite()
{
    return gSprite;
}

bool GameObject::GetActive()
{
    return isActive;
}

void GameObject::SetSprite(Sprite _sprite)
{
    gSprite = _sprite;
}

void GameObject::SetActive(bool _isActive)
{
    isActive = _isActive;
}
