#pragma once

#include "newGameObject.h"
#include <chrono>

class Sprite;
class ResourceManager;


class PlayerCharacter : public GameObject
{
public:
	PlayerCharacter(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)	// Default constructor
		: GameObject(d3d, objTex, position, scale, active, tileRect, collisionBounds, objnum), objSprite(d3d)
	{
		Init(objTex, position, scale, active, tileRect, collisionBounds, objnum);
	}

	void Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum) override;
	void Update(float dTime, ResourceManager& rManager) override;
	void Terminate();

	Sprite GetSprite() override;
	bool GetActive() override;
	void SetSprite(Sprite _sprite) override;
	void SetActive(bool _isActive) override;

	RECTF GetPlayerCollisionBounds() { return collisionPlayerRect; }

private:
	bool isActive;
	Sprite objSprite;

	// Collision functions
	bool PointVsRect(const Vector2& point, const RECTF& rect)											// Return true if point is inside rect
	{
		return(point.x >= rect.left &&
			point.y >= rect.top &&
			point.x < rect.right&&
			point.y < rect.bottom);
	}
	bool RectVsRect(const RECTF& r1, const RECTF& r2)													// Returns true if any part of rect1 and rect2 intersect
	{
		return(r1.left <= r2.right &&
			r1.top <= r2.bottom &&
			r1.right > r2.left &&
			r1.bottom > r2.top);
	}

	// See "Arbitary rectangle Collision detection & Resolution - Complete!" - javidx9 on youtube.com
	bool RayVsRect(const Vector2& rayOrigin, const Vector2& rayDir, const RECTF& targetRect, Vector2& contactPoint, Vector2& contactNormal, float& t_hit_near);
	bool DynamicRectVsRect(Tile* obj2, Vector2& contactPoint, Vector2& contactNormal, float& contactTime, float dTime);
	bool ResolveDynamicRectVsRect(const float dTime, Tile* tile);

	void CheckCollision(ResourceManager& rManager, float dTime);											// temporary collision function, Kieron can swap it for his collision once merged
	bool newCheckCollision(Tile* tile, ResourceManager& rManager, float dTime);
	bool newerCheckCollision(ResourceManager& rManager, float dTime);
	bool newerCheckCollision(ResourceManager& rManager, float dTime);
	RECTF collisionPlayerRect;
	RECTF oldCollisionPRect;

	Vector2 collisionDimensions;



	// KACPER STUFF
	//------ movement variables
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
	std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time_wall_jump;
	std::chrono::time_point<std::chrono::high_resolution_clock> end_time_wall_jump;

	Vector2 checkNextVel = checkNextVel.Zero;
	Vector2 checkNextPos;

	Vector2 oldPos;

	DirectX::SimpleMath::Vector2 currentVel = currentVel.Zero;
	DirectX::SimpleMath::Vector2 mousePos = mousePos.Zero;
	DirectX::SimpleMath::Vector2 direction = direction.Zero;

	const float MAX_JUMP_VEL = 400;
	const float MIN_JUMP_VEL = MAX_JUMP_VEL / 2;
	const float CLIMB_VEL = 170;
	const float SLIDE_DOWN_VEL = 80;
	const float GRAVITY = 300;
	const float PLAYER_SPEED = 350;
	const float DRAG_X = 0.985;				//for deceleration in x-axis on the ground
	const float DRAG_X_IN_AIR = 0.994;				//for deceleration in x-axis in air
	const float	DRAG_Y = 0.990;				//for deceleration in y-axis
	const float HIGH_JUMP_TIME = 0.20;					//how much time it takes to do a higher jump
	const float LOW_JUMP_TIME = HIGH_JUMP_TIME / 2;	//how much time it takes to do a lower jump

	double elapsed_time = 0;					//measure how much time has elapsed between starting and ending time counting
	float ungroundedTime = 0;
	float jumpForgiveness = 1;

	std::string jumpType;

	bool grounded = false;				//
	bool timeSpaceClickDetected = false;				//if space button has been released stop measuring time for picking either high or low jump
	bool recordJumpTime = false;				//start/stop recording jump time
	bool detectSpaceKey = true;					//start detecting the space button pressed down
	bool detectMouseClick = true;					//

	//------ animation variables 
	const float frameDuration = 0.07f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = 0.0f;
	float spaceClickElapsedTime = 0.f;
	int currentFrame = 0;
	std::string animState;
	RECTF spriteFrames[5] = { {0,0,6,16},{6,0,12,16},{12,0,18,16},{18,0,24,16},{24,0,30,16} };
	RECTF flipped_spriteFrames[5] = {
		{-spriteFrames[0].left, spriteFrames[0].top, -spriteFrames[0].right, spriteFrames[0].bottom},
		{-spriteFrames[1].left, spriteFrames[1].top, -spriteFrames[1].right, spriteFrames[1].bottom},
		{-spriteFrames[2].left, spriteFrames[2].top, -spriteFrames[2].right, spriteFrames[2].bottom},
		{-spriteFrames[3].left, spriteFrames[3].top, -spriteFrames[3].right, spriteFrames[3].bottom},
		{-spriteFrames[4].left, spriteFrames[4].top, -spriteFrames[4].right, spriteFrames[4].bottom} };

	//------ animation functions
	void UpdateAnimation(float dTime);


	void UpdateInput(float dTime);									//PLAYER EXCLUSIVE
	Vector2 playerPreviousPos;
	Vector2 playerPreviousVel;

	//for my little solution only, Kieron can swap it for his collision once merged

	//------ simple "collisions"
	double elapsedtime = 0;
	bool isWallSliding = false;
	bool deactivate_A = false;
	bool deactivate_D = false;
	bool hasWallJumped = false;
	/*bool collidesFromLeft = false;
	bool collidesFromRight = false;*/
};