#include "Tile.h"

void Tile::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, int objnum)
{
	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(tileRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName() + std::to_string(objnum), true, tempTileRect); // TILE DISPLAY SOMETHING TO DO WITH &tempTileRect
	objSprite.SetTex(*objTex);																																			// OR GET ANIM FUNCTION BELOW FIRST 2 VALUES
	objSprite.origin = Vector2((tileRect.right + tileRect.left) / 2, (tileRect.top + tileRect.bottom) / 2);			// <----
	objSprite.SetScale(scale);
	isActive = active;

	objSprite.GetAnim().Init(0, 0, 0, true);				 // <----	

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
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