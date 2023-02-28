#include "newGameObject.h"
#include "ResourceManager.h"
#include "Texture.h"

void GameObject::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)
{
	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(tileRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), "test_sheet.dds", "test_sheet" + std::to_string(objnum), true, tempTileRect); // TILE DISPLAY SOMETHING TO DO WITH &tempTileRect
	objSprite.SetTex(*objTex);																																			// OR GET ANIM FUNCTION BELOW FIRST 2 VALUES
	
	objSprite.SetScale(scale);
	objSprite.origin = Vector2(0, 8);
	isActive = active;

	objSprite.GetAnim().Init(0, 0, 0, true);				 // <----	

	objSprite.mPos = Vector2(position);

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
