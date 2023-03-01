#include "D3DUtil.h"
#include "Game.h"
#include "PlayerCharacter.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Tile.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void PlayerCharacter::Init(Texture* tex, Vector2 position, Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)
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

	// Initialize the players collision rect. This is in Init() and Update() to ensure no random values get put in based on order of function calling 
	collisionPlayerRect.left = objSprite.mPos.x;
	collisionPlayerRect.right = objSprite.mPos.x + objSprite.GetTexData().dim.x;
	collisionPlayerRect.top = objSprite.mPos.y;
	collisionPlayerRect.bottom = objSprite.mPos.y + objSprite.GetTexData().dim.y;
}

void PlayerCharacter::Update(float dTime, ResourceManager& rManager)
{
	isBottom = false;
	isTop = false;
	//update player core movement
	objSprite.mPos.x += objSprite.mVel.x * dTime;
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

	// Initializes the players collision rect 
	collisionPlayerRect.left = objSprite.mPos.x;
	collisionPlayerRect.right = objSprite.mPos.x + objSprite.GetTexData().dim.x;
	collisionPlayerRect.top = objSprite.mPos.y;
	collisionPlayerRect.bottom = objSprite.mPos.y + objSprite.GetTexData().dim.y;

	CheckCollision(rManager);
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

																		// PLAYER COLLISION CHECK FUNCTION
void PlayerCharacter::CheckCollision(ResourceManager& rManager)
{
	Vector2 currentPlayerPosition = objSprite.mPos;				// grabs current players position

	for (Tile* tile : rManager.GetTiles())					    // loops through all tiles. Ideally only loops through tiles that have collision bounds set in TILED editors
	{
		if (this->GetPlayerCollisionBounds().left < tile->GetCollisionBounds().right)				// AABB collision checks between the player and tile rects
		{																							// if colliding, sets players position to just before, not allowing them to go through the wall/tile
			objSprite.mPos.x = currentPlayerPosition.x;
		}
		if (this->GetPlayerCollisionBounds().right < tile->GetCollisionBounds().left)
		{
			objSprite.mPos.x = currentPlayerPosition.x;

		}
		if (this->GetPlayerCollisionBounds().top > tile->GetCollisionBounds().bottom)
		{
			objSprite.mPos.y = currentPlayerPosition.y;

		}
		if (this->GetPlayerCollisionBounds().bottom < tile->GetCollisionBounds().top)
		{
			objSprite.mPos.y = currentPlayerPosition.y;
		}
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
