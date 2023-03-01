#include "Tile.h"

void Tile::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)
{
	std::string tileRectsString = std::to_string(tileRect.left) + std::to_string(tileRect.top) + std::to_string(tileRect.right) + std::to_string(tileRect.bottom);

	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(tileRect);

	std::string a = tex->GetPath();
	std::string b = tex->GetName();

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName() + tileRectsString, true, tempTileRect);
	objSprite.SetTex(*objTex);

	objSprite.SetScale(scale);
	objSprite.origin = Vector2(0, 0);					// Swap between (0 ,0) or (0, 8) to test which looks better (fits all the tiles correctly on y axis)
	isActive = active;

	objSprite.GetAnim().Init(0, 0, 0, true);

	objSprite.mPos = position;
}

Sprite Tile::GetSprite()
{
	return objSprite;
}

bool Tile::GetActive()
{
	return isActive;
}

void Tile::SetSprite(Sprite _sprite)
{
	objSprite = _sprite;
}

void Tile::SetActive(bool _isActive)
{
	isActive = _isActive;
}