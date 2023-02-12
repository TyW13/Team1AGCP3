#pragma once
#include <d2d1.h>

#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"
class Asteroid 
{
public:
	Asteroid(MyD3D& d3d)
		: asteroidSpr(d3d)
	{
		active = false;
	}
	Asteroid& operator=(const Asteroid& rhs);
	void Init(MyD3D& d3d, bool honing);
	void Update(float dTime, int& additionalScore, Sprite& _mPlayer, Sprite& _mMissile);
	void Render(DirectX::SpriteBatch& batch);
	bool GetActive()
	{
		return active;
	}
	void SetActive(bool _active)
	{
		active = _active;
	}
	Sprite& GetSprite()
	{
		return asteroidSpr;
	}
	void SetSprite(Sprite _spr)
	{
		asteroidSpr = _spr;
	}
	void SetSpriteID(int _id)
	{
		spriteID = _id;
	}
	void SetHoning(bool isTrue)
	{
		isHoning = isTrue;
	}
	bool GetHoning()
	{
		return isHoning;
	}
	void SetIsDown(bool isDown) 
	{
		isGoingDown = isDown;
	}
	bool GetIsDown()
	{
		return isGoingDown;
	}
private:
	Sprite asteroidSpr;
	bool active;
	int spriteID = 0;
	const float ASTEROID_SPEED = 100;
	const int ASTEROID_SPAWN_TRIES = 50;
	bool isHoning = false;
	bool isGoingDown;
};

class Bullet
{
public:
	Bullet(MyD3D& d3d)
		:bullet(d3d)
	{}

	void Init(MyD3D& d3d);
	void Render(DirectX::SpriteBatch& batch);
	void Update(float dTime);

	Sprite& GetSprite()
	{
		return bullet;
	}
private:
	Sprite bullet;
	const float MISSILE_SPEED = 300;
};

class Player
{
public:

	bool onGround;
	float xPos;
	float yPos;
	float xVel;
	float yVel;



	Player(MyD3D& d3d);

	Sprite player;		// Player											//PLAYER EXCLUSIVE
	void Init(MyD3D& d3d); //THIS IS CALLED
	//This needs to follow where its called "mMissile.Init(mD3D);"
	void Update(float dTime);											//PLAYER EXCLUSIVE
	void Render(DirectX::SpriteBatch& batch);

private:
	const float SPEED = 250;											//PLAYER EXCLUSIVE
	const float MOUSE_SPEED = 5000;										//PLAYER EXCLUSIVE
	const float PAD_SPEED = 500;										//PLAYER EXCLUSIVE
	RECTF playArea;		//don't go outside this							//PLAYER EXCLUSIVE
	bool isHoning;
	void UpdateInput(float dTime);										//PLAYER EXCLUSIVE
	void playerRender(DirectX::SpriteBatch& batch);
};


class Platform
{
public:

	float xPos;
	float yPos;
	float xVel;
	float yVel;
	int scale;

	Platform(MyD3D& d3d);
	Sprite platform;
	void Init(MyD3D& d3d);
	void Update(float dTime, Sprite& _mPlayer);
	void Render(DirectX::SpriteBatch& batch);

private :

	void platformRender(DirectX::SpriteBatch& batch);

};


