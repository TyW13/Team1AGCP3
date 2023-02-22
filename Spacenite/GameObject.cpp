#include "GameObject.h"
#include "Game.h"
#include "WindowUtils.h"
#include "Input.h"


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
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "test_chara_walk.dds");
	character.SetTex(*p);
	PlayerAnim.Init("test_chara_walk.json", character);
	character.SetScale(Vector2(6.f, 6.f));
	character.origin = Vector2(0, character.GetTexData().dim.y);
	character.mPos = Vector2(WinUtil::Get().GetClientWidth()/2, WinUtil::Get().GetClientHeight());
}

void Player::Update(float dTime)
{

	//update player core movement
	character.mPos.x += character.mVel.x * dTime;
	character.mPos.y += character.mVel.y * dTime;
	//decrease velocity by gravity
	character.mPos.y += gravity * dTime;
	
	gravity = GRAVITY_SPEED;

	//if velocity is is more than max speed 
	//
	//if player goes right
	if (character.mVel.x > PLAYER_SPEED && character.mVel.x != 0)
	{
		character.mVel.x = PLAYER_SPEED;
	}
	//if player goes left
	if (character.mVel.x < -PLAYER_SPEED && character.mVel.x != 0)
	{
		character.mVel.x = -PLAYER_SPEED;
	}
	if (character.mVel.x == 0)
	{
		
	}
	if (character.mVel.x < PLAYER_SPEED || character.mVel.x > -PLAYER_SPEED)
	{
		character.mVel.x *= DRAGX;
	}
	if (character.mVel.y < 0 && Game::sMKIn.IsPressed(VK_SPACE))
	{
		character.mVel.y *= DRAGY_Higher;	//jump higher when space is hold
	}
	else if (character.mVel.y < 0)
	{
		character.mVel.y *= DRAGY_Lower;	//jump lower when space is not hold anymore
	}
	else if (character.mVel.y >= 0)
	{
 		character.mVel.y *= DRAGY_Higher;	//make sure the character's falling down is smooth
	}


	UpdateInput(dTime);
	CheckCollision();
	PlayerAnim.Update(dTime, character, animState);
}

void Player::Render(DirectX::SpriteBatch& batch)
{
	playerRender(batch);
}


void Player::UpdateInput(float dTime)
{
	if (Game::sMKIn.IsPressed(VK_D) == true)
	{
		character.mVel.x += PLAYER_SPEED;

		animState = "Right";
	}
	else if (Game::sMKIn.IsPressed(VK_A) == true)
	{
		character.mVel.x -= PLAYER_SPEED;

		animState = "Left";
	}
	else
	{
		animState = "Stand";
	}
	
	if (Game::sMKIn.IsPressed(VK_SPACE) == true && isGrounded)
	{
		character.mVel.y = -JUMP_SPEED;
		gravity = 0;
		isGrounded = false;
	}
	if (Game::sMKIn.IsPressed(VK_SPACE) == false && character.mVel.y < 0)
	{
		isGrounded = false;
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
	if (character.mPos.x + playerOrigin.x > WinUtil::Get().GetClientWidth())
	{
		character.mPos.x = WinUtil::Get().GetClientWidth() - playerOrigin.x;
	}
	//left
	if (character.mPos.x - playerOrigin.x < 0)
	{
		character.mPos.x = playerOrigin.x;
	}

	//if the player is on the bottom line (let's say it's the ground for now)
	if (character.mPos.y == WinUtil::Get().GetClientHeight())
	{
		isGrounded = true;
	}
}


