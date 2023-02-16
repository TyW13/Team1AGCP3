#pragma once
#include <vector>

#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"
#include <chrono>

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

	Player(MyD3D& d3d);
	Sprite character;		//											//PLAYER EXCLUSIVE
	void Init(MyD3D& d3d); //THIS IS CALLED
	//This needs to follow where its called "mMissile.Init(mD3D);"
	void Update(float dTime);											//PLAYER EXCLUSIVE
	void Render(DirectX::SpriteBatch& batch);

private:

	//------ movement variables
	std::chrono::time_point<std::chrono::steady_clock> start_time;
	std::chrono::time_point<std::chrono::steady_clock> end_time;
	DirectX::SimpleMath::Vector2 currentVel = currentVel.Zero;
	const float JUMP_VEL = 500.0f;
	const float GRAVITY = 2000.f;
	const float PLAYER_MAX_SPEED = 350;
	const float DRAG_X = 0.985;									//for deceleration in x-axis
	float currentPlayerSpeed = 0;
	float jumpSpeed = 0;
	double elapsed_time = 0;

	bool higher_jump, lower_jump = false;
	bool isGrounded = false;
	bool isJumping = false;

	//------ animation variables 
	const float frameDuration = 0.07f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = 0.0f;
	int currentFrame = 0;
	std::string animState;
	RECTF spriteFrames[5] = {{0,0,16,16},{16,0,32,16},{32,0,48,16},{48,0,64,16},{64,0,80,16}};
	RECTF flipped_spriteFrames[5] = { 
		{-spriteFrames[0].left, spriteFrames[0].top, -spriteFrames[0].right, spriteFrames[0].bottom},
		{-spriteFrames[1].left, spriteFrames[1].top, -spriteFrames[1].right, spriteFrames[1].bottom},
		{-spriteFrames[2].left, spriteFrames[2].top, -spriteFrames[2].right, spriteFrames[2].bottom},
		{-spriteFrames[3].left, spriteFrames[3].top, -spriteFrames[3].right, spriteFrames[3].bottom},
		{-spriteFrames[4].left, spriteFrames[4].top, -spriteFrames[4].right, spriteFrames[4].bottom}};

	//------ animation functions
	void UpdateAnimation(float dTime);
	

	void UpdateInput(float dTime);									//PLAYER EXCLUSIVE
	void playerRender(DirectX::SpriteBatch& batch);

	//for my little solution only, Kieron can swap it for his collision once merged
	void CheckCollision();	
};
