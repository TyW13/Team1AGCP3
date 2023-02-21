#include "newGameObject.h"
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

	collision_Bounds = collisionBounds;
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
