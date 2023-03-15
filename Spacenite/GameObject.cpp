#include "GameObject.h"
#include "Game.h"
#include "WindowUtils.h"
#include "Input.h"
#include "cmath"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

MouseAndKeys Game::sMKIn;
Gamepads Game::sGamepads;

//Asteroid class functions
Asteroid& Asteroid::operator=(const Asteroid& rhs)
{
	active = rhs.active;
	return *this;
}
void Asteroid::Init(MyD3D& d3d, bool honing)
{
	vector<RECTF> frames2;
	frames2.insert(frames2.begin(), 8 * 8, RECTF());
	const float ASS_SZ = 102;
	int c = 0;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			frames2[c++] = RECTF{ x * ASS_SZ, y * ASS_SZ, x * ASS_SZ + ASS_SZ, y * ASS_SZ + ASS_SZ };
		}
	}

	if (!honing)
	{
		ID3D11ShaderResourceView* p = asteroidSpr.GetD3D().GetCache().LoadTexture(&asteroidSpr.GetD3D().GetDevice(), "asteroid.dds", "asteroid", true, &frames2);

		asteroidSpr.SetTex(*p);
	}
	else
	{
		ID3D11ShaderResourceView* pp = asteroidSpr.GetD3D().GetCache().LoadTexture(&asteroidSpr.GetD3D().GetDevice(), "honingasteroid.dds", "hAsteroid", true, &frames2);

		asteroidSpr.SetTex(*pp);
	}

	asteroidSpr.GetAnim().Init(0, 31, 15, true);
	asteroidSpr.GetAnim().Play(true);
	asteroidSpr.SetScale(Vector2(0.5f, 0.5f));
	asteroidSpr.origin = Vector2(ASS_SZ / 2.0f, ASS_SZ / 2.0f);
	active = false;
	SetSprite(asteroidSpr);


}
void Asteroid::Update(float dTime, int& additionalScore, Sprite& _mPlayer, Sprite& _mMissile)
{
	if (active)
	{
		float radius = 32.0f;
		int w, h;
		WinUtil::Get().GetClientExtents(w, h);
		if (asteroidSpr.mPos.x < -radius)
		{
			active = false;
		}
		else if (asteroidSpr.mPos.y > h + radius) 
		{
			active = false;
		}
		asteroidSpr.GetAnim().Update(dTime);

		// from the right
		if (GetIsDown() == false)
		{
			if (GetHoning() == true)
			{
				if (asteroidSpr.mPos.x > Game::sMKIn.GetMousePos(true).x)
				{
					asteroidSpr.mPos.x -= 60.f * dTime;

					if (asteroidSpr.mPos.y < Game::sMKIn.GetMousePos(true).y + GetRandom(-50, 50))
					{
						asteroidSpr.mPos.y += 20.f * dTime;
					}
					else if (asteroidSpr.mPos.y > Game::sMKIn.GetMousePos(true).y + GetRandom(-50, 50))
					{
						asteroidSpr.mPos.y -= 20.f * dTime;
					}
				}
			}
			else if (GetHoning() == false)
			{
				asteroidSpr.mPos.x -= 70.f * dTime;
			}
		}

		// from above
		else if (GetIsDown() == true)
		{
			if (GetHoning() == true)
			{
				if (asteroidSpr.mPos.y < Game::sMKIn.GetMousePos(true).y)
				{
					asteroidSpr.mPos.y += 60.f * dTime;

					if (asteroidSpr.mPos.x < Game::sMKIn.GetMousePos(true).x + GetRandom(-50, 50))
					{
						asteroidSpr.mPos.x += 20.f * dTime;
					}
					else if (asteroidSpr.mPos.x > Game::sMKIn.GetMousePos(true).x + GetRandom(-50, 50))
					{
						asteroidSpr.mPos.x -= 20.f * dTime;
					}
				}
			}
			else if (GetHoning() == false)
			{
				asteroidSpr.mPos.y += 70.f * dTime;
			}
		}

		// Check collision between asteroid-ship
		if (((_mPlayer.mPos.x > asteroidSpr.mPos.x - radius) && (_mPlayer.mPos.x < asteroidSpr.mPos.x + 10))
			&& ((_mPlayer.mPos.y > asteroidSpr.mPos.y - radius) && (_mPlayer.mPos.y < asteroidSpr.mPos.y + 10)))
		{
			additionalScore += 10;
			active = false;
			_mPlayer.SetActive(false);
		}

		// Check collision between asteroid-bullet
		if (((_mMissile.mPos.x > asteroidSpr.mPos.x - radius) && (_mMissile.mPos.x < asteroidSpr.mPos.x + 10))
			&& ((_mMissile.mPos.y > asteroidSpr.mPos.y - radius) && (_mMissile.mPos.y < asteroidSpr.mPos.y + 10)))
		{
			additionalScore += 10;
			active = false;
			_mMissile.SetActive(false);
		}
	}
}
void Asteroid::Render(SpriteBatch& batch)
{
	/*if (active)
	{
		asteroidSpr.Draw(batch);
	}*/
}

void Bullet::Init(MyD3D& d3d)
{
	ID3D11ShaderResourceView* p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "Laser.dds", "laser", true);

	bullet.SetTex(*p);
	bullet.SetScale(Vector2(0.5f, 0.5f));
	float laserXOrigin = 55.f / 2.f;
	float laserYOrigin = 55.f / 2.f;
	bullet.origin = Vector2(laserXOrigin, laserYOrigin);
	GetSprite().SetActive(false);
}
void Bullet::Render(SpriteBatch& batch)
{
	if (GetSprite().GetActive() == true)
	{
		bullet.Draw(batch);
	}
}
void Bullet::Update(float dTime)
{
	if (GetSprite().GetActive() == true)
	{
		bullet.mPos.x += MISSILE_SPEED * dTime;
		if (bullet.mPos.x > WinUtil::Get().GetClientWidth())
		{
			GetSprite().SetActive(false);
		}
	}
}

Player::Player(MyD3D& d3d)
	:character(d3d)
{

}

void Player::Init(MyD3D& mD3D)
{
	//load a orientate the player
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "c_walk.dds");
	character.SetTex(*p, spriteFrames[0]);
	character.SetScale(Vector2(6.f, 6.f));
	character.origin = Vector2(0, character.GetTexData().dim.y);
	character.mPos = Vector2(WinUtil::Get().GetClientWidth()/2, WinUtil::Get().GetClientHeight());
}

void Player::Update(float dTime, Sprite& _wallpad)
{
	//update player core movement
	character.mVel += currentVel * dTime;
	character.mPos += currentVel * dTime;


	UpdateInput(dTime);
	CheckCollision();
	UpdateAnimation(dTime);

	if (character.mPos.x > _wallpad.mPos.x)
	{
		elapsed_t_bouncepad = 0;
		character.mPos.x = _wallpad.mPos.x;
		currentVel.x = -7*currentVel.x;

		elapsed_t_bouncepad = +dTime;
	}
	if (elapsed_t_bouncepad > 0)
	{
		elapsed_t_bouncepad = +dTime;

		/*if (elapsed_t_bouncepad < 0.2)
		{
			deactivate_A = true;
			deactivate_D = true;
		}
		else
		{
			deactivate_A = false;
			deactivate_D = false;
		}*/
	}
}

void Player::Render(DirectX::SpriteBatch& batch)
{
	playerRender(batch);
}

void Player::UpdateInput(float dTime)
{
	//--------- mouse
	if (Game::sMKIn.GetMouseButton(MouseAndKeys::LBUTTON) && detectMouseClick)
	{
		mousePos = Game::sMKIn.GetMousePos(true);
		//playerToMouseDist = Distance(character.mPos.x, mousePos.x, character.mPos.y, mousePos.y);

		//calculate the direction vector from the player to the mouse click
		direction = mousePos - character.mPos;

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

		//--wall sliding
		//
		//wall jump
		if (isWallSliding && Game::sMKIn.IsPressed(VK_SPACE) && !hasWallJumped)
		{
			//start_time_wall_jump = std::chrono::high_resolution_clock::now();
			currentVel.y = -WALL_JUMP_VEL_Y;
			currentVel.x = -3 * currentVel.x;
			elapsedtime = 0;
			hasWallJumped = true;
			isWallSliding = false;
		}
		if (hasWallJumped)
		{
			elapsedtime += dTime;

			if (elapsedtime < 0.5)
			{

				currentVel *= 0.99;
				deactivate_A = true;
				deactivate_D = true;
			}
			else
			{
				hasWallJumped = false;
				deactivate_A = false;
				deactivate_D = false;
			}
		}
		else
		{
			if (isWallSliding && !Game::sMKIn.IsPressed(VK_W))
			{
				currentVel.y = SLIDE_DOWN_VEL;
			}
		}

		//if the player has slowed down moving upward activate the gravity to take them back down
		if (currentVel.y >= -40 && !isWallSliding)
		{
			if (currentVel.y > GRAVITY) { currentVel.y = GRAVITY; }
			else { currentVel.y += 1.01 * (GRAVITY / 20); }
		}
	}
}

void Player::UpdateAnimation(float dTime)
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
			if (currentFrame >= 4)
			{
				currentFrame = 0;
			}
			elapsedTime -= frameDuration;
		}

		//flip back 
		character.SetScale(Vector2(6, character.GetScale().y));

		switch (currentFrame)
		{
		case 0:
			character.SetTexRect(spriteFrames[0]);
			break;
		case 1:
			character.SetTexRect(spriteFrames[1]);
			break;
		case 2:
			character.SetTexRect(spriteFrames[2]);
			break;
		case 3:
			character.SetTexRect(spriteFrames[3]);
			break;
		/*case 4:
			character.SetTexRect(spriteFrames[4]);
			break;*/
		default:
			character.SetTexRect(spriteFrames[0]);
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
			if (currentFrame >= 4)
			{
				currentFrame = 0;
			}
			elapsedTime -= frameDuration;
		}

		//flip
		character.SetScale(Vector2(-6, character.GetScale().y));

		switch (currentFrame)
		{
		case 0:
			character.SetTexRect(flipped_spriteFrames[0]);
			break;
		case 1:
			character.SetTexRect(flipped_spriteFrames[1]);
			break;
		case 2:
			character.SetTexRect(flipped_spriteFrames[2]);
			break;
		case 3:
			character.SetTexRect(flipped_spriteFrames[3]);
			break;
		case 4:
			character.SetTexRect(flipped_spriteFrames[4]);
			break;
		default:
			character.SetTexRect(flipped_spriteFrames[0]);
			break;
		}
	}
	if (animState == "Stand")
	{
		//flip back 
		character.SetScale(Vector2(6, character.GetScale().y));

		//set default frame back
		character.SetTexRect(spriteFrames[0]);
	}
}

void Player::playerRender(DirectX::SpriteBatch& batch)
{
	character.Draw(batch);
}

void Player::CheckCollision()
{

	//for code clarity
	Vector2 playerOrigin = Vector2(((character.GetTexData().dim.x / 2.f) * character.GetScale().x, character.GetTexData().dim.y / 2.f) * character.GetScale().y);

	//check for player and screen borders collision

	//bottom
	if (character.mPos.y > WinUtil::Get().GetClientHeight())
	{
		character.mPos.y = WinUtil::Get().GetClientHeight();
	}
	//top
	if (character.mPos.y < playerOrigin.y * 2)
	{
		character.mPos.y = playerOrigin.y * 2;
	}
	//right
	if (character.mPos.x + playerOrigin.x*2 > WinUtil::Get().GetClientWidth())
	{
		if (Game::sMKIn.IsPressed(VK_D) && !deactivate_D)
		{
			isWallSliding = true;
		}
		else
		{
			isWallSliding = false;
		}
		character.mPos.x = WinUtil::Get().GetClientWidth() - playerOrigin.x *2;
	}
	//left
	else if (character.mPos.x - playerOrigin.x * 2 < 0)
	{
		if (Game::sMKIn.IsPressed(VK_A) && !deactivate_A)
		{
			isWallSliding = true;
		}
		else
		{
			isWallSliding = false;
		}
		character.mPos.x = playerOrigin.x * 2;
	}
	else
	{
		isWallSliding = false;
	}

	//if the player is on the bottom line (let's say it's the ground for now)
	if (character.mPos.y == WinUtil::Get().GetClientHeight())
	{
		detectSpaceKey = true;
		grounded = true;
	}
}

void WallPad::Init(MyD3D& d3d)
{
	ID3D11ShaderResourceView* p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "wallpad.dds", "wallpad", true);

	wallpadSpr.SetTex(*p);
	wallpadSpr.SetScale(Vector2(0.5f, 0.5f));
	wallpadSpr.origin = Vector2(wallpadSpr.GetTexData().dim.x / 2, wallpadSpr.GetTexData().dim.y / 2);
	wallpadSpr.mPos = Vector2(WinUtil::Get().GetClientWidth() - 350, WinUtil::Get().GetClientHeight() - 100);
}

void WallPad::Update(float dTime)
{   
}

void WallPad::Render(DirectX::SpriteBatch& batch)
{
	wallpadSpr.Draw(batch);
}
