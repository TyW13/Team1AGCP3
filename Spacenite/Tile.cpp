#include "Tile.h"

void Tile::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect)
{
	std::vector<RECTF> tempTileRect;
	tempTileRect.push_back(tileRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), true, &tempTileRect); // TILE DISPLAY SOMETHING TO DO WITH &tempTileRect
	objSprite.SetTex(*objTex);																																			// OR GET ANIM FUNCTION BELOW FIRST 2 VALUES
	objSprite.SetScale(scale);
	objSprite.origin = Vector2(tileRect.left,tileRect.bottom);			// <----
	isActive = active;

	objSprite.GetAnim().Init(0, 0, 20, true);				 // <----	
	objSprite.GetAnim().Play(true);

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