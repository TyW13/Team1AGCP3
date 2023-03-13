#include "Respawner.h"

void Respawner::Init(Texture* _tex, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, Vector2 _objSize, std::string _objType, bool _isCollidable, RECTF _objRect)
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

Sprite Respawner::GetSprite()
{
	return objSprite;
}

bool Respawner::GetActive()
{
	return isActive;
}

void Respawner::SetSprite(Sprite _sprite)
{
	objSprite = _sprite;
}

void Respawner::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void Respawner::SetObjectSize(Vector2 _objSize)
{
	objSize = _objSize;
}

void Respawner::SetIsCollidable(bool _isCollidable)
{
	isCollidable = _isCollidable;
}