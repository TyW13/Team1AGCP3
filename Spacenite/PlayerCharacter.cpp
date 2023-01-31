#include "D3DUtil.h"
#include "PlayerCharacter.h"
#include "Texture.h"
#include "ResourceManager.h"


using namespace DirectX::SimpleMath;

void PlayerCharacter::Init(ResourceManager* rManager, Texture* tex, Vector2 scale)
{
	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), false);
	objSprite.SetTex(*objTex);
	objSprite.SetScale(scale);
	objSprite.origin = objSprite.GetTexData().dim / 2.0f;

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	objSprite.mPos = Vector2(w / 2, h / 2);


	rManager->AddGameObject(this);
}
