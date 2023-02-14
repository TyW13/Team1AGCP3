#include "TileMap.h"

void TileMap::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active)
{
	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), true);
	objSprite.SetTex(*objTex);
	objSprite.SetScale(scale);
	objSprite.origin = objSprite.GetTexData().dim / 2.0f;
	isActive = active;

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	//objSprite.mPos = Vector2(w / 2, h / 2);
	objSprite.mPos = position;

}
