#include "Tile.h"

void Tile::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, int objnum)
{
	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(tileRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName() + std::to_string(objnum), true, tempTileRect);
	objSprite.SetTex(*objTex);

	objSprite.SetScale(scale);
	objSprite.origin = Vector2(0, 8);					// Swap between (0 ,0) or (0, 8) to test which looks better (fits all the tiles correctly on y axis)
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