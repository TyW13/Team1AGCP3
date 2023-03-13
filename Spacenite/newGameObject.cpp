#include "newGameObject.h"
#include "ResourceManager.h"
#include "Texture.h"

void GameObject::Init(Texture* _tex, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, Vector2 _objSize, std::string _objType, bool _isCollidable, RECTF _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);


	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(_objRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), _tex->GetPath(), _tex->GetName(), true); 
	objSprite.SetTex(*objTex, { 0,0,6,16 });
	objSprite.SetScale(_scale);
	objSprite.origin = Vector2(0, 0);
	isActive = _active;

	//objSprite.GetAnim().Init(0, 0, 0, true);				 // <----	

	objSprite.mPos = _position;
	objSize = _objSize;
	objType = _objType;
	isCollidable = _isCollidable;

	collisionBounds.left = objSprite.mPos.x;
	collisionBounds.top = objSprite.mPos.y;
	collisionBounds.right = objSprite.mPos.x + objSize.x * _scale.x;
	collisionBounds.bottom = objSprite.mPos.y + objSize.y * _scale.y;
}

void GameObject::Update(MyD3D& d3d, float dTime, ResourceManager& rManager)
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

void GameObject::SetObjectSize(Vector2 _objSize)
{
	objSize = _objSize;
}

void GameObject::SetIsCollidable(bool _isCollidable)
{
	isCollidable = _isCollidable;
}
