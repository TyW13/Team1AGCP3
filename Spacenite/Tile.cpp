#include "Tile.h"

void Tile::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect)
{
	std::vector<RECTF> tempTileRect;
	tempTileRect.push_back(tileRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), true, &tempTileRect);
	objSprite.SetTex(*objTex);
	objSprite.SetScale(scale);
	objSprite.origin = Vector2(0,0);
	isActive = active;

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	//objSprite.mPos = Vector2(w / 2, h / 2);
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
