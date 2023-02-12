#pragma once

#include "DirectXHelpers.h"
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
#include <math.h>

#include "Sprite.h"


// Region R = { (x.y.z) | min.x < = x <= max.x, min,y <= y <=max.y, min.z <= z <= max.z }
struct AABB
{
	int min;
	int max;


};

struct BoundingBox
{
    static constexpr size_t CORNER_COUNT = 8;




};

class Collision2D
{
public:
    // Rectangle class for collisions
    class Rect
    {
    public:
        float x, y;
        float width, height;

        Rect(float _x, float _y, float _width, float _height) :
            x(_x), y(_y), width(_width), height(_height) {}
    };

	// This function takes in two rectangles and returns true if they are intersecting.
	bool CheckRectangleCollision(DirectX::XMFLOAT2 rect1Pos, DirectX::XMFLOAT2 rect1Size, DirectX::XMFLOAT2 rect2Pos, DirectX::XMFLOAT2 rect2Size)
	{
		float rect1Left = rect1Pos.x - rect1Size.x / 2.0f;
		float rect1Right = rect1Pos.x + rect1Size.x / 2.0f;
		float rect1Top = rect1Pos.y + rect1Size.y / 2.0f;
		float rect1Bottom = rect1Pos.y - rect1Size.y / 2.0f;

		float rect2Left = rect2Pos.x - rect2Size.x / 2.0f;
		float rect2Right = rect2Pos.x + rect2Size.x / 2.0f;
		float rect2Top = rect2Pos.y + rect2Size.y / 2.0f;
		float rect2Bottom = rect2Pos.y - rect2Size.y / 2.0f;

		if (rect1Right > rect2Left && rect1Left < rect2Right &&
			rect1Top > rect2Bottom && rect1Bottom < rect2Top)
		{
			return true;
		}

		return false;
	}
    // Function to check if two rectangles are colliding
	static bool CheckCollision(const Rect& a, const Rect& b)
	{

        return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
    }
};

class Collision
{
public :

	bool onGround;
	float xPos;
	float yPos;
	float xVel;
	float yVel;


	// Collision Impacts

	
	int AsteroidCollision(float dTime, Sprite& _mPlayer, Sprite& _mMissile)
	{
		float radius = 32.0f;

		// Check collision between asteroid-player
		if (((_mPlayer.mPos.x > asteroidSpr.mPos.x - radius) && (_mPlayer.mPos.x < asteroidSpr.mPos.x + 10))
			&& ((_mPlayer.mPos.y > asteroidSpr.mPos.y - radius) && (_mPlayer.mPos.y < asteroidSpr.mPos.y + 10)))
		{
			float radius = 32.0f;
			active = false;
			_mPlayer.SetActive(false);
		}
	}
	// Asteroid - Player
	int BulletCollision();    // Bullet - Asteroid
	int PlatformCollision();  // Platform Collision

	// Collision Checks
	int CheckCollisionAABBAABB(AABB A, AABB B);
	




};



