#include "Game.h"
#include "PlayMode.h"
#include "WindowUtils.h"
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "PlayerCharacter.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


Asteroid* PlayMode::CheckAsteroidColl(Asteroid& asteroid)
{
	assert(!mAsteroids.empty());
	float radius = mAsteroids[0].GetSprite().GetScreenSize().Length() / 2.f;
	size_t i = 0;
	Asteroid* pColl = nullptr;
	while (i < mAsteroids.size() && !pColl)
	{
		Asteroid& collider = mAsteroids[i];
		if ((&asteroid != &collider) && (collider.GetActive() && ((collider.GetSprite().mPos - asteroid.GetSprite().mPos).Length() < (radius * 2))))
		{
			pColl = &mAsteroids[i];
		}
		++i;
	}
	return pColl;
}

Asteroid* PlayMode::SpawnAsteroid()
{
	assert(!mAsteroids.empty());
	size_t i = 0;
	Asteroid* p = nullptr;
	while (i < mAsteroids.size() && !p)
	{
		if (!mAsteroids[i].GetActive())
		{
			p = &mAsteroids[i];
		}
		i++;
	}

	if (p)
	{
		int w, h;
		WinUtil::Get().GetClientExtents(w, h);
		float radius = mAsteroids[0].GetSprite().GetScreenSize().Length() / 2.0f;
		Vector2& pos = p->GetSprite().mPos;
		int rnum = GetRandom(1, 4);
		switch (rnum)
		{
		case(1):
		case(2):
			pos.y = (float)GetRandom(radius, h - radius);
			pos.x = (float)(w + radius);
			p->SetIsDown(false);
			break;
		case(3):
		case(4):
			pos.y = (float)(-radius);
			pos.x = (float)GetRandom(radius, w - radius);
			p->SetIsDown(true);
		}

		bool collision = false;
		p->SetActive(true);

		return p;
	}
}

void PlayMode::InitAsteroids()
{
	assert(mAsteroids.empty());
	Asteroid asteroid(mD3D);
	mAsteroids.insert(mAsteroids.begin(), ASTEROID_CACHE, asteroid);

	for (auto& asteroid : mAsteroids)
	{
		if (GetRandom(0, 2) >= 0.25f)
		{
			asteroid.SetHoning(false);
		}
		else
		{
			asteroid.SetHoning(true);
		}

		if (asteroid.GetHoning() == false)
		{
			asteroid.Init(mD3D, false);
		}
		else if (asteroid.GetHoning() == true)
		{
			asteroid.Init(mD3D, true);
		}

		asteroid.SetSpriteID(asteroidID);
		asteroidID++;

		if (GetRandom(0, 1) == 0)
		{
			asteroid.GetSprite().GetAnim().Init(0, 31, GetRandom(10.0f, 20.f), true);
		}
		else
		{
			asteroid.GetSprite().GetAnim().Init(0, 31, GetRandom(10.0f, 20.0f), true);
		}

		asteroid.GetSprite().GetAnim().SetFrame(GetRandom(asteroid.GetSprite().GetAnim().GetStart(), asteroid.GetSprite().GetAnim().GetEnd()));
	}
}

void PlayMode::UpdateAsteroids(float dTime)
{
	//PlayMode pMode = *this;
	assert(!mAsteroids.empty());
	for (auto& asteroid : mAsteroids)
	{
		asteroid.Update(dTime, additionalScore, Player.character, mMissile.GetSprite()); //need to add mPlayer.ship here
	}
	if ((GetClock() - mLastSpawn) > mSpawnRateSec)		// Spawn timer
	{
		if (SpawnAsteroid())
		{
			mLastSpawn = GetClock();
		}

		if (mSpawnRateSec > 0.2f) 
		{
			mSpawnRateSec -= 0.0001f;
		}
	}
}

void PlayMode::RenderAsteroids(DirectX::DX11::SpriteBatch& batch)
{
	for (Asteroid& asteroid : mAsteroids)
	{
		asteroid.Render(batch);
	}
}
//------------------------------------------------------------------------------- Asteroid Functions end


PlayMode::PlayMode(MyD3D& d3d)
	:mD3D(d3d), Player(d3d), mMissile(d3d), rManager(d3d)
{
	rManager.Init(d3d);

	bGround.Init(d3d);
	Player.Init(d3d);

	mpFont = new SpriteFont(&d3d.GetDevice(), L"data/fonts/comicSansMS.spritefont");
	assert(mpFont);	
}

void PlayMode::Release()
{
	delete mpFont;
	mpFont = nullptr;
}

void PlayMode::UpdateMissile(float dTime)
{
	if ((mMissile.GetSprite().GetActive() == false) && Game::sMKIn.IsPressed(VK_SPACE))
	{
		mMissile.GetSprite().SetActive(true);
		mMissile.GetSprite().mPos = Vector2(Player.character.mPos.x + Player.character.GetScreenSize().x / 2.f, Player.character.mPos.y); //need to add mPlayer.ship here
	}
	mMissile.Update(dTime);
}


void PlayMode::Update(MyD3D& d3d, float dTime, bool& _endGame)
{
	bGround.Update(dTime, IsTop, IsBottom);
	rManager.Update(d3d, dTime);
}

void PlayMode::Render(float dTime, int& pScore, DirectX::DX11::SpriteBatch& batch)
{
	bGround.Render(batch);
	//if (Player.character.GetActive())
	//{
	//	Player.Render(batch);
	//	//mMissile.Render(batch);

	//}
	rManager.Render(batch);

	//RenderAsteroids(batch);


	// Increase score over time
	pScore = (int)GetClock() * 10 + additionalScore;
	stringstream ss;
	//ss << std::setfill('0') << std::setw(3) << pScore;
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, ss.str().c_str(), Vector2(w * 0.86f, h * 0.85f), Vector4(1, 1, 1, 1));
}