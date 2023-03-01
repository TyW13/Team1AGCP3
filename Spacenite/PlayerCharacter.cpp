#include "D3DUtil.h"
#include "Game.h"
#include "PlayerCharacter.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Tile.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MouseAndKeys Game::sMKIn;
Gamepads Game::sGamepads;

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
	// Initializes the players collision rect 
	collisionPlayerRect.left = objSprite.mPos.x;
	collisionPlayerRect.right = objSprite.mPos.x + objSprite.GetTexData().dim.x;
	collisionPlayerRect.top = objSprite.mPos.y;
	collisionPlayerRect.bottom = objSprite.mPos.y + objSprite.GetTexData().dim.y;

	UpdateInput(dTime);

	CheckCollision(rManager);
}

//void PlayerCharacter::UpdateInput(float dTime)
//{
//	if (Game::sMKIn.IsPressed(VK_D) == true)
//	{
//		objSprite.mVel.x += PLAYER_SPEED;
//	}
//
//	if (Game::sMKIn.IsPressed(VK_A) == true)
//	{
//		objSprite.mVel.x -= PLAYER_SPEED;
//	}
//
//	if (Game::sMKIn.IsPressed(VK_SPACE))
//	{
//		objSprite.mVel.y = -JUMP_SPEED;
//		//isGrounded = false;
//	}
//}


void PlayerCharacter::UpdateInput(float dTime)
{
	//--------- x-axis
	//right
	if (Game::sMKIn.IsPressed(VK_D))
	{
		currentVel.x = PLAYER_SPEED;
		animState = "Right";
	}
	//left
	else if (Game::sMKIn.IsPressed(VK_A))
	{
		currentVel.x = -PLAYER_SPEED;
		animState = "Left";
	}
	//deceleration
	else
	{
		currentVel.x *= DRAG_X;
		animState = "Stand";
	}

	//--------- y-axis
	//up

	if (grounded)
	{
		//set initial velocity, start timer, record button pressed down during only the first frame
		if (Game::sMKIn.IsPressed(VK_SPACE) && !stopDetectSpaceKey)
		{
			start_time = std::chrono::steady_clock::now();
			currentVel.y = -MAX_JUMP_VEL;	//set initial velocity to max velocity

			stopDetectSpaceKey = true;
			recordJumpTime = true;
			grounded = false;
		}


	}
	if (!grounded)
	{
		//record how much time has been passed since pressing down and releasing space button
		if (recordJumpTime)
		{
			end_time = std::chrono::steady_clock::now();
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
		if ((!Game::sMKIn.IsPressed(VK_SPACE) && elapsed_time != 0 && elapsed_time <= HIGH_JUMP_TIME && stopDetectSpaceKey) || elapsed_time == HIGH_JUMP_TIME)
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

		//if the player has slowed down moving upward activate the gravity to take them back down
		if (currentVel.y >= -40)
		{
			if (currentVel.y > GRAVITY) { currentVel.y = GRAVITY; }
			else { currentVel.y += 1.01 * (GRAVITY / 20); }


		}
	}

	//--------- mouse
	if (Game::sMKIn.GetMouseButton(MouseAndKeys::LBUTTON) && !stopDetectMouseClick)
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
		currentVel = (direction * 1000);

		stopDetectMouseClick = true;
	}
	if (!Game::sMKIn.GetMouseButton(MouseAndKeys::LBUTTON) && stopDetectMouseClick)
	{
		stopDetectMouseClick = false;
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
