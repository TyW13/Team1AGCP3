#include "newGameObject.h"
#include "ResourceManager.h"
#include "Texture.h"

void GameObject::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)
{
	std::string tileRectsString = std::to_string(tileRect.left) + std::to_string(tileRect.top) + std::to_string(tileRect.right) + std::to_string(tileRect.bottom);


	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(tileRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), true); 
	objSprite.SetTex(*objTex, { 0,0,6,16 });
	objSprite.SetScale(scale);
	objSprite.origin = Vector2(0, 0);
	isActive = active;

	//objSprite.GetAnim().Init(0, 0, 0, true);				 // <----	

	objSprite.mPos = position;

	collisionDimensions.x = collisionBounds.x;
	collisionDimensions.y = collisionBounds.y;

	collision_Bounds.left = objSprite.mPos.x;
	collision_Bounds.top = objSprite.mPos.y;
	collision_Bounds.right = objSprite.mPos.x + collisionDimensions.x;
	collision_Bounds.bottom = objSprite.mPos.y + collisionDimensions.y;
}

void GameObject::Update(float dTime, ResourceManager& rManager)
{
	collision_Bounds.left = objSprite.mPos.x;
	collision_Bounds.top = objSprite.mPos.y;
	collision_Bounds.right = objSprite.mPos.x + collisionDimensions.x;
	collision_Bounds.bottom = objSprite.mPos.y + collisionDimensions.y;
	DBOUT(std::to_string(objSprite.mPos.x) + " " + std::to_string(objSprite.mPos.y));
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
