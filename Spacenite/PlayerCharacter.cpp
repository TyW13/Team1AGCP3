#include "PlayerCharacter.h"
#include "Texture.h"

void PlayerCharacter::Init(Texture* tex)
{
	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), false);
	objSprite.SetTex(*objTex);
}
