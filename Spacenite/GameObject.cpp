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
	if (active)
	{
		asteroidSpr.Draw(batch);
	}
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
	:ship(d3d)
{

}

void Player::Init(MyD3D& mD3D)
{
	//load a orientate the ship
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "ship.dds");
	ship.SetTex(*p);
	ship.SetScale(Vector2(0.1f, 0.1f));
	ship.origin = ship.GetTexData().dim / 2.f;

	//setup the play area
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	playArea.left = ship.GetScreenSize().x * 0.6f;
	playArea.top = ship.GetScreenSize().y * 0.6f;
	playArea.right = w - playArea.left;
	playArea.bottom = h * 0.75f;
 	ship.mPos = Vector2(150, (playArea.bottom - playArea.top) / 2.f);
}

void Player::Update(float dTime)
{
	UpdateInput(dTime);
}

void Player::Render(DirectX::SpriteBatch& batch)
{
	shipRender(batch);
}

void Player::UpdateInput(float dTime)
{
	Vector2 mouse{ Game::sMKIn.GetMousePos(false) };
	bool keypressed = Game::sMKIn.IsPressed(VK_UP) || Game::sMKIn.IsPressed(VK_DOWN) ||
		Game::sMKIn.IsPressed(VK_RIGHT) || Game::sMKIn.IsPressed(VK_LEFT);
	bool sticked = false;
	if (Game::sGamepads.IsConnected(0) &&
		(Game::sGamepads.GetState(0).leftStickX != 0 || Game::sGamepads.GetState(0).leftStickX != 0))
		sticked = true;

	if (keypressed || (mouse.Length() > VERY_SMALL) || sticked)
	{
		//move the ship around
		Vector2 pos(0, 0);

		if (sticked)
		{
			DBOUT("left stick x=" << Game::sGamepads.GetState(0).leftStickX << " y=" << Game::sGamepads.GetState(0).leftStickY);
			pos.x += Game::sGamepads.GetState(0).leftStickX * PAD_SPEED * dTime;
			pos.y -= Game::sGamepads.GetState(0).leftStickY * PAD_SPEED * dTime;
		}

		//keep it within the play area
		pos += ship.mPos;
		if (pos.x < playArea.left)
			pos.x = playArea.left;
		else if (pos.x > playArea.right)
			pos.x = playArea.right;
		if (pos.y < playArea.top)
			pos.y = playArea.top;
		else if (pos.y > playArea.bottom)
			pos.y = playArea.bottom;

		ship.mPos = pos;
	}

	double rotationInRads = -(atan2(Game::sMKIn.GetMousePos(true).y - ship.mPos.y, ship.mPos.x - Game::sMKIn.GetMousePos(true).x) + PI / 2);
	ship.rotation = rotationInRads;

	if (ship.mPos.x + 10 < Game::sMKIn.GetMousePos(true).x)
	{
		ship.mPos.x += 100.f * dTime;
	}
	else if (ship.mPos.x - 10 > Game::sMKIn.GetMousePos(true).x)
	{
		ship.mPos.x -= 100.f * dTime;
	}
	if (ship.mPos.y + 10 < Game::sMKIn.GetMousePos(true).y)
	{
		ship.mPos.y += 100.f * dTime;
	}
	else if (ship.mPos.y - 10 > Game::sMKIn.GetMousePos(true).y)
	{
		ship.mPos.y -= 100.f * dTime;
	}
}

void Player::shipRender(DirectX::SpriteBatch& batch)
{
	ship.Draw(batch);
}
