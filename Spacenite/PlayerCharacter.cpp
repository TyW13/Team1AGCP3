#define NOMINMAX

#include "D3DUtil.h"
#include "Game.h"
#include "PlayerCharacter.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Tile.h"

#include <algorithm>

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

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	objSprite.mPos = position;
}

void PlayerCharacter::Update(float dTime, ResourceManager& rManager)
{
	collisionPlayerRect.left = objSprite.mPos.x;
	collisionPlayerRect.right = objSprite.mPos.x + /*(objSprite.GetTexRect().right * objSprite.GetScale().x)*/36;
	collisionPlayerRect.top = objSprite.mPos.y;
	collisionPlayerRect.bottom = objSprite.mPos.y + /*(objSprite.GetTexRect().bottom * objSprite.GetScale().y)*/96;

	UpdateInput(dTime);

	objSprite.mPos += currentVel * dTime;

	//checkNextPos = objSprite.mPos;


	////newerCheckCollision(rManager, dTime);
	////objSprite.mPos += currentVel * dTime;
	//
	//if (!newerCheckCollision(rManager, dTime))
	//{
	//	objSprite.mPos += currentVel * dTime;
	//}


	// JAVIDX9 CODE
	// Sort collisions in order of distance
	Vector2 cp, cn;
	float t = 0, min_t = INFINITY;
	std::vector<std::pair<Tile*, float>> z;
	std::vector<Vector2> collisionNormals;
	bool movedX = false;
	bool movedY = false;
	// Work out collision point, add it to vector along with rect ID
	for (Tile* tile: rManager.GetTiles())
	{
		if (DynamicRectVsRect(tile, cp, cn, t, dTime))
		{
			z.push_back({ tile, t });
			collisionNormals.push_back(cn);
		}
	}
	// Do the sort
	std::sort(z.begin(), z.end(), [](const std::pair<Tile*, float>& a, const std::pair<Tile*, float>& b)
		{
			return a.second < b.second;
		});
	// Now resolve the collision in correct order 
	for (auto j : z)
	{
		for (int i = 0; i < collisionNormals.size(); i++)
		{
			if (collisionNormals[i] == Vector2(0, -1) && !movedY)
			{
				objSprite.mPos.y = j.first->GetCollisionBounds().top - 96;
				movedY = true;
			}
			else if (collisionNormals[i] == Vector2(0, 1) && !movedY)
			{
				objSprite.mPos.y = j.first->GetCollisionBounds().bottom + 96;
				movedY = true;
			}
			else if (collisionNormals[i] == Vector2(-1, 0) && !movedX)
			{
				objSprite.mPos.y = j.first->GetCollisionBounds().left - 36;
				movedX = true;

			}
			else if (collisionNormals[i] == Vector2(0, 1) && movedX)
			{
				objSprite.mPos.y = j.first->GetCollisionBounds().left + 36;
				movedX = true;
			}
		}
		//ResolveDynamicRectVsRect(dTime, j.first);
	}

	UpdateAnimation(dTime);

}

void PlayerCharacter::UpdateInput(float dTime)
{
	//--------- mouse
	if (Game::sMKIn.GetMouseButton(MouseAndKeys::LBUTTON) && detectMouseClick)
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
		//set initial velocity, start timer, record button pressed down during only the first frame
		if (Game::sMKIn.IsPressed(VK_SPACE) && detectSpaceKey)
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



bool PlayerCharacter::RayVsRect(const Vector2& rayOrigin, const Vector2& rayDir, const RECTF& targetRect, Vector2& contactPoint, Vector2& contactNormal, float& t_hit_near)
{
	contactNormal = { 0,0 };
	contactPoint = { 0,0 };

	Vector2 invdir;
	invdir.x = 1.0f / rayDir.x;
	invdir.y = 1.0f / rayDir.y;

	float t_nearX = (targetRect.left - rayOrigin.x) * invdir.x;										// Getting near and far points on x and y where line intercects object rect (both sides)
	float t_nearY = (targetRect.top - rayOrigin.y) * invdir.y;
	Vector2 t_near = { t_nearX, t_nearY };

	float t_farX = ((targetRect.left + 36) - rayOrigin.x) * invdir.x;
	float t_farY = ((targetRect.top + 96) - rayOrigin.y) * invdir.y;
	Vector2 t_far = { t_farX, t_farY };

	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

	if (t_near.x > t_far.x) { std::swap(t_near.x, t_far.x); }
	if (t_near.y > t_far.y) { std::swap(t_near.y, t_far.y); }

	if (t_near.x > t_far.y || t_near.y > t_far.x) { return false; }										// Will be false if the ray is not intersecting with object rect

	t_hit_near = std::max(t_near.x, t_near.y);															// Calculate "times" from origin point to near and far collisions
	float t_hit_far = std::max(t_far.x, t_far.y);
	if (t_hit_far < 0) { return false; }													// Return false for intersections in the opposite direction of ray and after max distance of ray

	contactPoint = rayOrigin + t_hit_near * rayDir;														// Location of contact point


	// Obtaining contact normal
	if (t_near.x > t_near.y)																			// If ray hits object rect from side
	{
		if (invdir.x < 0)																				// if ray is coming from right
		{
			contactNormal = { 1,0 };
		}
		else																							// If ray is coming from left
		{
			contactNormal = { -1, 0 };
		}
	}
	else if (t_near.x < t_near.y)																		// If ray hits object rect from top/bottom
	{
		if (invdir.y < 0)																				// If ray is coming from bottom
		{
			contactNormal = { 0,1 };
		}
		else
		{
			contactNormal = { 0,-1 };																	// If ray is coming from top
		}
	}

	return true;
}

bool PlayerCharacter::DynamicRectVsRect(Tile* obj2, Vector2& contactPoint, Vector2& contactNormal, float& contactTime, float dTime)
{
	if (currentVel.x == .0f && currentVel.y == .0f)
	{
		return false;
	}

	RECTF expandedTargetRect;
	expandedTargetRect.left = obj2->GetCollisionBounds().left - 18;
	expandedTargetRect.right = obj2->GetCollisionBounds().right + 18;
	expandedTargetRect.top = obj2->GetCollisionBounds().top - 48;
	expandedTargetRect.bottom = obj2->GetCollisionBounds().bottom + 48;

	if (RayVsRect({ collisionPlayerRect.left + 18, collisionPlayerRect.top + 48 },
		currentVel * dTime, expandedTargetRect, contactPoint, contactNormal, contactTime))
	{
		if (contactTime <= 1.0f)
		{
			contactPoint;
			return true;
		}
	}

	return false;
}

bool PlayerCharacter::ResolveDynamicRectVsRect(const float dTime, Tile* tile)
{
	Vector2 contact_point, contact_normal;
	float contact_time = 0.0f;
	if (DynamicRectVsRect(tile, contact_point, contact_normal, contact_time, dTime))
	{
		//if (contact_normal.y > 0) r_dynamic->contact[0] = r_static; else nullptr;					// Couldnt figure out what contact was
		//if (contact_normal.x < 0) r_dynamic->contact[1] = r_static; else nullptr;
		//if (contact_normal.y < 0) r_dynamic->contact[2] = r_static; else nullptr;
		//if (contact_normal.x > 0) r_dynamic->contact[3] = r_static; else nullptr;
		Vector2 aaa = contact_normal * Vector2(fabs(currentVel.x), fabs(currentVel.y));
		float bbb = (1 - contact_time);
		Vector2 ccc = aaa * bbb;


		currentVel += contact_normal * Vector2(fabs(currentVel.x), fabs(currentVel.y)) * (1 - contact_time);
		return true;
	}

	return false;
}

// PLAYER COLLISION CHECK FUNCTION
void PlayerCharacter::CheckCollision(ResourceManager& rManager, float dTime)
{
	collisionPlayerRect.left = objSprite.mPos.x;
	collisionPlayerRect.right = objSprite.mPos.x + /*(objSprite.GetTexRect().right * objSprite.GetScale().x)*/36;
	collisionPlayerRect.top = objSprite.mPos.y;
	collisionPlayerRect.bottom = objSprite.mPos.y + /*(objSprite.GetTexRect().bottom * objSprite.GetScale().y)*/96;

	for (Tile* tile : rManager.GetTiles())					    // loops through all tiles. Ideally only loops through tiles that have collision bounds set in TILED editors
	{
		if (collisionPlayerRect.left < tile->GetCollisionBounds().right &&
			collisionPlayerRect.right > tile->GetCollisionBounds().left &&
			collisionPlayerRect.top < tile->GetCollisionBounds().bottom &&
			collisionPlayerRect.bottom > tile->GetCollisionBounds().top)
		{
			RECTF ww = collisionPlayerRect;
			RECTF pp = tile->GetCollisionBounds();


			float topDist = fabs(collisionPlayerRect.top - tile->GetCollisionBounds().bottom);									// Distance from tile rect top
			float botDist = fabs(collisionPlayerRect.bottom - tile->GetCollisionBounds().top);

			if (topDist < botDist)																						// If player is closer to top of tile
			{
				objSprite.mPos.y += topDist;
				//collisionsY -= topDist;
				//currentVel.y = 0;
			}
			else if (botDist < topDist)																					// If player is closer to bottom of tile
			{
				objSprite.mPos.y -= botDist;
				//collisionsY += botDist;
				//currentVel.y = 0;
			}

			collisionPlayerRect.left = objSprite.mPos.x;
			collisionPlayerRect.right = objSprite.mPos.x + /*(objSprite.GetTexRect().right * objSprite.GetScale().x)*/36;
			collisionPlayerRect.top = objSprite.mPos.y;
			collisionPlayerRect.bottom = objSprite.mPos.y + /*(objSprite.GetTexRect().bottom * objSprite.GetScale().y)*/96;
		}

		if (collisionPlayerRect.left < tile->GetCollisionBounds().right &&
			collisionPlayerRect.right > tile->GetCollisionBounds().left &&
			collisionPlayerRect.top < tile->GetCollisionBounds().bottom &&
			collisionPlayerRect.bottom > tile->GetCollisionBounds().top)
		{
			float leftDist = fabs(collisionPlayerRect.left - tile->GetCollisionBounds().right);
			float rightDist = fabs(collisionPlayerRect.right - tile->GetCollisionBounds().left);

			if (leftDist < rightDist)
			{
				objSprite.mPos.x += leftDist;
				//collisionsX -= leftDist;
				//currentVel.x = 0;
			}
			else if (rightDist < leftDist)
			{
				objSprite.mPos.x -= rightDist;
				//collisionsX += rightDist;
				//currentVel.x = 0;
			}
		}

		collisionPlayerRect.left = objSprite.mPos.x;
		collisionPlayerRect.right = objSprite.mPos.x + /*(objSprite.GetTexRect().right * objSprite.GetScale().x)*/36;
		collisionPlayerRect.top = objSprite.mPos.y;
		collisionPlayerRect.bottom = objSprite.mPos.y + /*(objSprite.GetTexRect().bottom * objSprite.GetScale().y)*/96;
	}
}

bool PlayerCharacter::newCheckCollision(Tile* tile, ResourceManager& rManager, float dTime)
{
	collisionPlayerRect.left = checkNextPos.x;
	collisionPlayerRect.right = checkNextPos.x + (objSprite.GetTexRect().right * objSprite.GetScale().x);
	collisionPlayerRect.top = checkNextPos.y;
	collisionPlayerRect.bottom = checkNextPos.y + (objSprite.GetTexRect().bottom * objSprite.GetScale().y);

	Vector2 contactPoint{ 0,0 };
	Vector2 contactNormal{ 0, 0};
	float contactTime = 0.f;

	if (DynamicRectVsRect(tile, contactPoint, contactNormal, contactTime, dTime))
	{
		currentVel += contactNormal * Vector2(fabs(currentVel.x), fabs(currentVel.y) * 1 - contactTime);
		return true;
	}

	return false;
}

bool PlayerCharacter::newerCheckCollision(ResourceManager& rManager, float dTime)
{
	collisionPlayerRect.left = checkNextPos.x;
	collisionPlayerRect.right = checkNextPos.x + /*(objSprite.GetTexRect().right * objSprite.GetScale().x)*/36;
	collisionPlayerRect.top = checkNextPos.y;
	collisionPlayerRect.bottom = checkNextPos.y + /*(objSprite.GetTexRect().bottom * objSprite.GetScale().y)*/96;

	bool collided = false;
	std::vector<Tile*> collidingTiles;
	Vector2 collectiveVel{ 0,0 };

	for (Tile* tile : rManager.GetTiles())					    // loops through all tiles. Ideally only loops through tiles that have collision bounds set in TILED editors
	{
		Vector2 dir{ 0,0 };

		float topDist = fabs(collisionPlayerRect.top - tile->GetCollisionBounds().bottom);
		float botDist = fabs(collisionPlayerRect.bottom - tile->GetCollisionBounds().top);
		float leftDist = fabs(collisionPlayerRect.left - tile->GetCollisionBounds().right);
		float rightDist = fabs(collisionPlayerRect.right - tile->GetCollisionBounds().left);

		if (collisionPlayerRect.left < tile->GetCollisionBounds().right &&
			collisionPlayerRect.right > tile->GetCollisionBounds().left &&
			collisionPlayerRect.top < tile->GetCollisionBounds().bottom &&
			collisionPlayerRect.bottom > tile->GetCollisionBounds().top)
		{
			if (botDist < topDist)
			{
				//collectiveVel.y -= botDist + 1;
				objSprite.mPos.y -= botDist + 1;
			}
			else if (topDist < botDist)
			{
				//collectiveVel.y += topDist + 1;
				objSprite.mPos.y += topDist + 1;
			}
			if (leftDist < rightDist)
			{
				//collectiveVel.x += leftDist + 1;
				objSprite.mPos.x += leftDist + 1;

			}
			else if (rightDist < leftDist)
			{
				//collectiveVel.x -= rightDist + 1;
				objSprite.mPos.x -= rightDist + 1;
			}
			return true;
			//collidingTiles.emplace_back(tile);
			//collided = true;
		}
	}

	//for (Tile* tile : collidingTiles)
	//{
	//	float topDist = fabs(collisionPlayerRect.top - tile->GetCollisionBounds().bottom);
	//	float botDist = fabs(collisionPlayerRect.bottom - tile->GetCollisionBounds().top);
	//	float leftDist = fabs(collisionPlayerRect.left - tile->GetCollisionBounds().right);
	//	float rightDist = fabs(collisionPlayerRect.right - tile->GetCollisionBounds().left);

	//	if (botDist < topDist)
	//	{
	//		//collectiveVel.y -= botDist + 1;
	//		currentVel.y -= currentVel.y + 1;
	//	}
	//	else if (topDist < botDist)
	//	{
	//		//collectiveVel.y += topDist + 1;
	//		currentVel.y += currentVel.y + 1;
	//	}
	//	if (leftDist < rightDist)
	//	{
	//		//collectiveVel.x += leftDist + 1;
	//		currentVel.x += currentVel.x + 1;

	//	}
	//	else if (rightDist < leftDist)
	//	{
	//		//collectiveVel.x -= rightDist + 1;
	//		currentVel.x -=  currentVel.x + 1;
	//	}
	//}
	//if (collided)
	//{
	//	objSprite.mPos += collectiveVel;
	//	return true;
	//}

	return false;
}

bool PlayerCharacter::newerCheckCollision(ResourceManager& rManager, float dTime)
{
	currentVel *= dTime;
	for (Tile* tile : rManager.GetTiles())
	{
		Vector2 diff = Vector2(objSprite.mPos.x + currentVel.x - tile->GetCollisionBounds().left, objSprite.mPos.y + currentVel.y - tile->GetCollisionBounds().top);

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
