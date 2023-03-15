#include "ReloadGem.h"

void ReloadGem::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, Vector2 objSize, std::string _objType, bool isCollidable, RECTF objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(_objRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), _tex->GetPath(), _tex->GetName() + tileRectsString, true, tempTileRect);
	objSprite.SetTex(*objTex);

	objSprite.SetScale(_scale);
	objSprite.origin = Vector2(0, 0);					// Swap between (0 ,0) or (0, 8) to test which looks better (fits all the tiles correctly on y axis)
	isActive = _active;

	objSprite.GetAnim().Init(0, 0, 0, true);

	objSprite.mPos = _position;
	objSize = _objSize;
	objType = _objType;
	isCollidable = _isCollidable;

	collisionBounds.left = objSprite.mPos.x;
	collisionBounds.top = objSprite.mPos.y;
	collisionBounds.right = objSprite.mPos.x + objSize.x * _scale.x;
	collisionBounds.bottom = objSprite.mPos.y + objSize.y * _scale.y;
}

Sprite ReloadGem::GetSprite()
{
	return objSprite;
}

bool ReloadGem::GetActive()
{
    return false;
}

void ReloadGem::SetSprite(Sprite _sprite)
{
	objSprite = _sprite;
}

void ReloadGem::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void ReloadGem::SetObjectSize(Vector2 _objSize)
{
	objSize = _objSize;
}

void ReloadGem::SetIsCollidable(bool _isCollidable)
{
	isCollidable = _isCollidable;
}