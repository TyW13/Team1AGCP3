#include "Collision.h"
#include "GameObject.h"
#include "Game.h"
#include <iostream>

//int Collision::AsteroidCollision(float dTime, Sprite& _mPlayer, Sprite& _mMissile)
//{
//	float radius = 32.0f;
//
	//// Check collision between asteroid-player
	//if (((_mPlayer.mPos.x > asteroidSpr.mPos.x - radius) && (_mPlayer.mPos.x < asteroidSpr.mPos.x + 10))
	//	&& ((_mPlayer.mPos.y > asteroidSpr.mPos.y - radius) && (_mPlayer.mPos.y < asteroidSpr.mPos.y + 10)))
	//{
	//	float radius = 32.0f;
	//	active = false;
	//	_mPlayer.SetActive(false);
	//}
//}
//
//int Collision::BulletCollision()
//{
//	// Check collision between asteroid-bullet
//	if (((_mMissile.mPos.x > asteroidSpr.mPos.x - radius) && (_mMissile.mPos.x < asteroidSpr.mPos.x + 10))
//		&& ((_mMissile.mPos.y > asteroidSpr.mPos.y - radius) && (_mMissile.mPos.y < asteroidSpr.mPos.y + 10)))
//	{
//		additionalScore += 10;
//		active = false;
//		_mMissile.SetActive(false);
//	}
//}

int Collision::PlatformCollision()
{
	// Player RECT1
	// Platform RECT2
	
	
	// Create two rectangles
	Collision2D::Rect rect1(0.0f, 0.0f, 100.0f, 100.0f);
	Collision2D::Rect rect2(50.0f, 50.0f, 100.0f, 100.0f);

	// Check if they are colliding
	bool isColliding = Collision2D::CheckCollision(rect1, rect2);

	// Player Collision

	//bool isColliding = Collision2D::CheckCollision(player);

	// Collision Check

	if (isColliding)
	{
		// Output to state collision
		std::cout << "The rectangles are colliding!" << std::endl;
	}
	else
	{
		// Output to state NO collision
		std::cout << "The rectangles are not colliding." << std::endl;
	}

	return 0;
}

// Axis Aligned Bounding Box > AABB
int Collision::CheckCollisionAABBAABB(AABB A, AABB B)
{
	// Exit with no intersection if seperated along an axis

	if (A.max[0] < B.min[0] || A.min[0] > B.max[0]) return 0;
	if (A.max[1] < B.min[1] || A.min[0] > B.max[1]) return 0;
	if (A.max[2] < B.min[2] || A.min[0] > B.max[2]) return 0;

	// Overlapping on all axes means AABB are intersecting

	return 1;
	
}
