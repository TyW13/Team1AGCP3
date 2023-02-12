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

    // Function to check if two rectangles are colliding
	static bool CheckCollision(const Rect& a, const Rect& b)
	{
		// If play > Asteroid
		// If Missile > Asteroid
		// If Player > Platform
		// If Asteroid > Platform

		//// Check collision between asteroid-player
		//if (((_mPlayer.mPos.x > asteroidSpr.mPos.x - radius) && (_mPlayer.mPos.x < asteroidSpr.mPos.x + 10))
		//	&& ((_mPlayer.mPos.y > asteroidSpr.mPos.y - radius) && (_mPlayer.mPos.y < asteroidSpr.mPos.y + 10)))
		//{
		//	float radius = 32.0f;
		//	active = false;
		//	_mPlayer.SetActive(false);
	
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

	
	int AsteroidCollision(float dTime, Sprite& _mPlayer, Sprite& _mMissile);
	// Asteroid - Player
	int BulletCollision();    // Bullet - Asteroid
	int PlatformCollision();  // Platform Collision

	// Collision Checks
	int CheckCollisionAABBAABB(AABB A, AABB B);
	




};



