#include "Sprite.h"

void Sprite::Draw(DirectX::SpriteBatch& batch)
{
	//batch.Draw()
}

void Sprite::SetTex(ID3D12Resource& tex, const RECT& texRect)
{
	mpTex = &tex;
	mTexRect = texRect;

	if (mTexRect.left == mTexRect.right && mTexRect.top == mTexRect.bottom)
	{
		SetTexRect(RECT{0,0,dimx, dimy})
	}
}

void Sprite::SetTexRect(const RECT& texRect)
{
	mTexRect = texRect;
}
