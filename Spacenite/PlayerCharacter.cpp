#include "D3DUtil.h"
#include "Game.h"
#include "PlayerCharacter.h"
#include "Texture.h"
#include "ResourceManager.h"


using namespace DirectX::SimpleMath;

void PlayerCharacter::Init(Texture* tex, Vector2 scale, bool active)
{
	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), true);
	objSprite.SetTex(*objTex);
	objSprite.SetScale(scale);
	objSprite.origin = objSprite.GetTexData().dim / 2.0f;
	isActive = active;

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	//objSprite.mPos = Vector2(w / 2, h / 2);
	objSprite.mPos = Vector2(20, 20);
}

void PlayerCharacter::Update(float dTime)
{
	isBottom = false;
	isTop = false;
	//update player core movement
	objSprite.mPos.x += objSprite.mVel.x * dTime;
	objSprite.mPos.y += objSprite.mVel.y * dTime;
	objSprite.mPos.y += objSprite.mVel.y * dTime;

	//decrease velocity by gravity
	objSprite.mPos.y += GRAVITY * dTime;

	//if reaches max speed 
	if (objSprite.mVel.x > PLAYER_SPEED)
	{
		objSprite.mVel.x = PLAYER_SPEED;
	}
	if (objSprite.mVel.x < -PLAYER_SPEED)
	{
		objSprite.mVel.x = -PLAYER_SPEED;
	}
	if (objSprite.mVel.x < PLAYER_SPEED || objSprite.mVel.x > -PLAYER_SPEED)
	{
		objSprite.mVel.x *= DRAG;
	}
	if (objSprite.mVel.y < -0)
	{
		objSprite.mVel.y *= 0.995;
	}
	UpdateInput(dTime);
	CheckCollision();
}



void PlayerCharacter::UpdateInput(float dTime)
{
	if (Game::sMKIn.IsPressed(VK_D) == true)
	{
		objSprite.mVel.x += PLAYER_SPEED;
	}

	if (Game::sMKIn.IsPressed(VK_A) == true)
	{
		objSprite.mVel.x -= PLAYER_SPEED;
	}

	if (Game::sMKIn.IsPressed(VK_SPACE))
	{
		objSprite.mVel.y = -JUMP_SPEED;
		//isGrounded = false;
	}
}

																		// TEMP COLLISION FUNCTION //
void PlayerCharacter::CheckCollision()
{

	//for code clarity
	Vector2 playerOrigin = Vector2(((objSprite.GetTexData().dim.x / 2.f) * objSprite.GetScale().x, objSprite.GetTexData().dim.y / 2.f) * objSprite.GetScale().y);

	//check for player and screen borders collision

	//bottom
	if (objSprite.mPos.y > WinUtil::Get().GetClientHeight() + 60)
	{
		objSprite.mPos.y = playerOrigin.y;
		isBottom = true;
	}
	//top
	if (objSprite.mPos.y < playerOrigin.y)
	{
		objSprite.mPos.y = WinUtil::Get().GetClientHeight() + 30;
		isTop = true;
	}
	//right
	if (objSprite.mPos.x > WinUtil::Get().GetClientWidth() + 15)
	{
		objSprite.mPos.x = WinUtil::Get().GetClientWidth() + 14;
	}
	//left
	if (objSprite.mPos.x < -15)
	{
		objSprite.mPos.x = -14;
	}

	//if the player is on the bottom line (let's say it's the ground for now)
	if (objSprite.mPos.y == WinUtil::Get().GetClientHeight())
	{
		isGrounded = true;
	}
}

Sprite PlayerCharacter::GetSprite()
{
	return objSprite;
}

bool PlayerCharacter::GetActive()
{
	return isActive;
}

void PlayerCharacter::SetSprite(Sprite _sprite)
{
	objSprite = _sprite;
}

void PlayerCharacter::SetActive(bool _isActive)
{
	isActive = _isActive;
}
