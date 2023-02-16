#include "newGameObject.h"
#include "Texture.h"

void GameObject::Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, int objnum)
{
	std::vector<RECTF>* tempTileRect = new std::vector<RECTF>;
	tempTileRect->push_back(tileRect);

	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), "test_sheet.dds", "test_sheet" + std::to_string(objnum), true, tempTileRect); // TILE DISPLAY SOMETHING TO DO WITH &tempTileRect
	objSprite.SetTex(*objTex);																																			// OR GET ANIM FUNCTION BELOW FIRST 2 VALUES
	objSprite.origin = Vector2(tileRect.left, tileRect.top);			// <----
	objSprite.SetScale(scale);
	isActive = true;

	objSprite.GetAnim().Init(0, 0, 0, true);				 // <----	

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	objSprite.mPos = Vector2(position);
}

void GameObject::Update(float dTime)
{
}

Sprite GameObject::GetSprite()
{
    return objSprite;
}

bool GameObject::GetActive()
{
    return isActive;
}

void GameObject::SetSprite(Sprite _sprite)
{
    objSprite = _sprite;
}

void GameObject::SetActive(bool _isActive)
{
    isActive = _isActive;
}
