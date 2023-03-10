#define NOMINMAX

#include "D3DUtil.h"
#include "Game.h"
#include "PlayerCharacter.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Tile.h"

#include <algorithm>

#include <windows.graphics.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//MouseAndKeys Game::sMKIn;
//Gamepads Game::sGamepads;

void PlayerCharacter::Init(Texture* tex, Vector2 position, Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)
{
	ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), tex->GetPath(), tex->GetName(), true);
	objSprite.SetTex(*objTex, spriteFrames[0]);
	objSprite.SetScale(scale);
	objSprite.origin = Vector2(0,0);
	isActive = active;

	objSprite.mPos = position;
	playerSize = { 6, 16 };
}

void PlayerCharacter::Update(float dTime, ResourceManager& rManager)
{
	collisionPlayerRect.left = objSprite.mPos.x;																			// Setting values of rect for player current position
	collisionPlayerRect.right = objSprite.mPos.x + playerSize.x * abs(objSprite.GetScale().x);								//
	collisionPlayerRect.top = objSprite.mPos.y;																				//
	collisionPlayerRect.bottom = objSprite.mPos.y + playerSize.y * abs(objSprite.GetScale().y);								//

	UpdateInput(dTime);

	CheckCollision(rManager, dTime);

	UpdateAnimation(dTime);
}

void PlayerCharacter::UpdateInput(float dTime)
{
	//--------- mouse
	if (Game::sMKIn.GetMouseButton(MouseAndKeys::LBUTTON) && detectMouseClick && !fired)
	{
		mousePos = Game::sMKIn.GetMousePos(true);
		//playerToMouseDist = Distance(character.mPos.x, mousePos.x, character.mPos.y, mousePos.y);

		//calculate the direction vector from the player to the mouse click
		direction = mousePos - objSprite.mPos;

		//reverse the direction vector
		direction *= -1;

		//normalize the direction vector
		direction /= sqrt(pow(direction.x, 2) + pow(direction.y, 2));

		//apply a jump force to the player character
		currentVel = (direction * 1500);

		fired = true;
		grounded = false;
		detectMouseClick = false;
	}
	if (!Game::sMKIn.GetMouseButton(MouseAndKeys::LBUTTON) && !detectMouseClick)
	{
		detectMouseClick = true;
	}

	//--------- x-axis
	//right
	if (Game::sMKIn.IsPressed(VK_D) && !deactivate_D)
	{
		currentVel.x = PLAYER_SPEED;
		animState = "Right";
	}
	//left
	else if (Game::sMKIn.IsPressed(VK_A) && !deactivate_A)
	{
		currentVel.x = -PLAYER_SPEED;
		animState = "Left";
	}
	//deceleration
	//
	//if on the ground
	else if (grounded)
	{
		currentVel.x *= DRAG_X;
		animState = "Stand";
	}
	//if in the air
	else if (!grounded)
	{
		currentVel.x *= DRAG_X_IN_AIR;
		animState = "Stand";
	}

	//--------- y-axis
	//up
	if (grounded)
	{
		if (Game::sMKIn.IsPressed(VK_SPACE)) 
		{
			detectSpaceKey = true;
		}
		//set initial velocity, start timer, record button pressed down during only the first frame
		if (detectSpaceKey)
		{
			start_time = std::chrono::high_resolution_clock::now();
			currentVel.y = -MAX_JUMP_VEL;	//set initial velocity to max velocity

			detectSpaceKey = false;
			recordJumpTime = true;
			grounded = false;
		}
	}
	if (!grounded)
	{
		//record how much time has been passed since pressing down and releasing space button
		if (recordJumpTime)
		{
			end_time = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_time - start_time;
			elapsed_time = elapsed_seconds.count();

			//if pressed for max time set to high jump straightaway
			if (elapsed_time >= HIGH_JUMP_TIME)
				elapsed_time = HIGH_JUMP_TIME;


		}

		//detect during which of two time frames the space button has been released and then set the jump type based on it 
		if (!Game::sMKIn.IsPressed(VK_SPACE) && !timeSpaceClickDetected || elapsed_time == HIGH_JUMP_TIME)
		{
			std::chrono::duration<double> elapsed_seconds = end_time - start_time;
			spaceClickElapsedTime = elapsed_seconds.count();

			//get the jump type
			if (spaceClickElapsedTime > LOW_JUMP_TIME)
			{
				//spaceClickElapsedTime = HIGH_JUMP_TIME;
				jumpType = "HighJump";

			}
			else
			{
				//spaceClickElapsedTime = LOW_JUMP_TIME;
				jumpType = "LowJump";

			}
			timeSpaceClickDetected = true;


		}

		//set the velocity to minimum jump vel if the space button was clicked during the former time frame 
		//OR 
		//set to maximum jump vel if was clicked during the latter time frame
		if ((!Game::sMKIn.IsPressed(VK_SPACE) && elapsed_time != 0 && elapsed_time <= HIGH_JUMP_TIME && !detectSpaceKey) || elapsed_time == HIGH_JUMP_TIME)
		{
			if (jumpType == "LowJump" && elapsed_time >= LOW_JUMP_TIME)
			{
				jumpType = "";
				elapsed_time = 0;
				currentVel.y = -MIN_JUMP_VEL;
				recordJumpTime = false;

			}
			else if (jumpType == "HighJump" && elapsed_time == HIGH_JUMP_TIME)
			{
				jumpType = "";
				elapsed_time = 0;
				currentVel.y = -MAX_JUMP_VEL;
				recordJumpTime = false;
			}
			else
			{
				recordJumpTime = false;
			}
		}

		//multiply the velocity by drag value to slown down the player moving upward
		if (currentVel.y < 0 && !recordJumpTime)
		{
			currentVel.y *= DRAG_Y;


		}




		//--wall sliding/climbing
		//
		//wall jump
		if (isWallSliding && Game::sMKIn.IsPressed(VK_SPACE))
		{
			//start_time_wall_jump = std::chrono::high_resolution_clock::now();
			currentVel.y = -currentVel.y;
			currentVel.x = -2 * currentVel.x;
			elapsedtime = 0;
			hasWallJumped = true;
		}/*
		else if (hasWallJumped)
		{
			elapsedtime += dTime;
			if (elapsedtime < 1)
			{
				deactivate_D = true;
			}
			else
			{
				deactivate_D = false;
				hasWallJumped = false;
			}
		}*/
		else
		{
			//climb upwards
			if (isWallSliding && Game::sMKIn.IsPressed(VK_W) && !Game::sMKIn.IsPressed(VK_SPACE))
			{
				currentVel.y = -CLIMB_VEL;
			}
			//climb downwards
			else if (isWallSliding && Game::sMKIn.IsPressed(VK_S) && !Game::sMKIn.IsPressed(VK_SPACE))
			{
				currentVel.y = CLIMB_VEL;
			}
			//slide down if none of above
			else if (isWallSliding && !Game::sMKIn.IsPressed(VK_W))
			{
				currentVel.y = SLIDE_DOWN_VEL;
			}
		}


		/*if (ungroundedTime > jumpForgiveness)
		{
			activateWallJump = false;
		}
		else
		{
			activateWallJump = true;
			ungroundedTime =+ dTime;
		}*/


		//if the player has slowed down moving upward activate the gravity to take them back down
		if (currentVel.y >= -40 && !isWallSliding)
		{
			if (currentVel.y > GRAVITY) { currentVel.y = GRAVITY; }
			else { currentVel.y += 1.01 * (GRAVITY / 20); }
		}
	}
}

void PlayerCharacter::UpdateAnimation(float dTime)
{
	if (animState == "Right")
	{
		//calculate elapsed time
		float deltaTime = dTime;
		elapsedTime += deltaTime;

		//advance to the next frame if enough time has passed
		if (elapsedTime >= frameDuration)
		{
			currentFrame++;
			if (currentFrame >= 5)
			{
				currentFrame = 0;
			}
			elapsedTime -= frameDuration;
		}

		//flip back 
		objSprite.SetScale(Vector2(6, objSprite.GetScale().y));

		switch (currentFrame)
		{
		case 0:
			objSprite.SetTexRect(spriteFrames[0]);
			break;
		case 1:
			objSprite.SetTexRect(spriteFrames[1]);
			break;
		case 2:
			objSprite.SetTexRect(spriteFrames[2]);
			break;
		case 3:
			objSprite.SetTexRect(spriteFrames[3]);
			break;
		case 4:
			objSprite.SetTexRect(spriteFrames[4]);
			break;
		default:
			objSprite.SetTexRect(spriteFrames[0]);
			break;
		}
	}
	if (animState == "Left")
	{
		//calculate elapsed time
		float deltaTime = dTime;
		elapsedTime += deltaTime;

		//advance to the next frame if enough time has passed
		if (elapsedTime >= frameDuration)
		{
			currentFrame++;
			if (currentFrame >= 5)
			{
				currentFrame = 0;
			}
			elapsedTime -= frameDuration;
		}

		//flip
		objSprite.SetScale(Vector2(-6, objSprite.GetScale().y));

		switch (currentFrame)
		{
		case 0:
			objSprite.SetTexRect(flipped_spriteFrames[0]);
			break;
		case 1:
			objSprite.SetTexRect(flipped_spriteFrames[1]);
			break;
		case 2:
			objSprite.SetTexRect(flipped_spriteFrames[2]);
			break;
		case 3:
			objSprite.SetTexRect(flipped_spriteFrames[3]);
			break;
		case 4:
			objSprite.SetTexRect(flipped_spriteFrames[4]);
			break;
		default:
			objSprite.SetTexRect(flipped_spriteFrames[0]);
			break;
		}
	}
	if (animState == "Stand")
	{
		//flip back 
		objSprite.SetScale(Vector2(6, objSprite.GetScale().y));

		//set default frame back
		objSprite.SetTexRect(spriteFrames[0]);
	}
}

void PlayerCharacter::CheckCollision(ResourceManager& rManager, float dTime)
{
	float maxVel = 1.8f;																// 
	if (currentVel.x * dTime >= maxVel)													// Making sure speed doesnt exceed max speed
	{
		currentVel.x = maxVel / dTime;
	}
	else if (currentVel.x * dTime <= -maxVel)
	{
		currentVel.x = -maxVel / dTime;
	}
	if (currentVel.y * dTime >= maxVel)
	{
		currentVel.y = maxVel / dTime;
	}
	else if (currentVel.y * dTime <= -maxVel)
	{
		currentVel.y = -maxVel / dTime;
	}

	Vector2 nextPos = objSprite.mPos + currentVel * dTime;															// Player future position after forces applied

	RECTF nextPosRect = RECTF{																						// Rect for players future position
		nextPos.x,
		nextPos.y,
		nextPos.x + playerSize.x * abs(objSprite.GetScale().x),														// Issue with player rect (right) being equal to current anim frames
		nextPos.y + playerSize.y * abs(objSprite.GetScale().y) };													// right value (6, 12, 18, 24, 30), prob fixed with josh animation code

	bool collided = false;
	std::vector<Tile*> collidedTiles;

	for (Tile* tile : rManager.GetTiles())
	{
		if (nextPosRect.left <= tile->GetCollisionBounds().right &&
			nextPosRect.right >= tile->GetCollisionBounds().left &&
			nextPosRect.top <= tile->GetCollisionBounds().bottom &&
			nextPosRect.bottom >= tile->GetCollisionBounds().top)
		{
			collidedTiles.emplace_back(tile);
		}
	}

	float collisionPosOffset = 1;																															// Value to offset player by when they collide with an object
	for (Tile* tile : collidedTiles)
	{
		if (collisionPlayerRect.bottom < tile->GetCollisionBounds().top && nextPosRect.bottom >= tile->GetCollisionBounds().top && !collidedTop)			// Collided from top, moving down
		{
			objSprite.mPos.y = tile->GetCollisionBounds().top - (playerSize.y * abs(objSprite.GetScale().y) + collisionPosOffset);							// Setting position to just outside tile
			objSprite.mPos.x += currentVel.x * dTime;																										// Only adding velocity on non colliding axis

			collided = true;
			collidedTop = true;
			grounded = true;
			fired = false;
		}
		else if (collisionPlayerRect.top > tile->GetCollisionBounds().bottom && nextPosRect.top <= tile->GetCollisionBounds().bottom && !collidedBottom)	// Collided from bottom, moving up
		{
			objSprite.mPos.y = tile->GetCollisionBounds().bottom + collisionPosOffset;																		// Setting position to just outside tile
			objSprite.mPos.x += currentVel.x * dTime;																										// Only adding velocity on non colliding axis

			collided = true;
			collidedBottom = true;
		}
		if (collisionPlayerRect.right < tile->GetCollisionBounds().left && nextPosRect.right >= tile->GetCollisionBounds().left && !collidedLeft)			// Collided from left, moving right
		{
			objSprite.mPos.x = tile->GetCollisionBounds().left - (playerSize.x * abs(objSprite.GetScale().x) + collisionPosOffset);							// Setting position to just outside tile
			objSprite.mPos.y += currentVel.y * dTime;																										// Only adding velocity on non colliding axis

			collided = true;
			collidedLeft = true;
		}
		else if (collisionPlayerRect.left > tile->GetCollisionBounds().right && nextPosRect.left <= tile->GetCollisionBounds().right && !collidedRight)		// Collided from right, moving left
		{
			objSprite.mPos.x = tile->GetCollisionBounds().right + collisionPosOffset;																		// Setting position to just outside tile
			objSprite.mPos.y += currentVel.y * dTime;																										// Only adding velocity on non colliding axis

			collided = true;
			collidedRight = true;
		}
	}

	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	if (!collided)																																			// If player didnt collide with anything
	{
		objSprite.mPos += currentVel * dTime;
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
