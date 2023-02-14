#pragma once
#include "GameObject.h"
#include <iostream>

using namespace std;

class Collision2D
{
public:

    struct AABB 
    {
        int min;
        int max;
    };
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
        return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);

    }


    int PlatformCollision2D()
    {
        // Player RECT1
        // Platform RECT2


        // Create two rectangles
        Rect rect1(0.0f, 0.0f, 100.0f, 100.0f);
        Rect rect2(50.0f, 50.0f, 100.0f, 100.0f);

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
    int CheckCollisionAABBAABB(AABB A, AABB B)
    {
        // Exit with no intersection if seperated along an axis

        if (A.max[0] < B.min[0] || A.min[0] > B.max[0]) return 0;
        if (A.max[1] < B.min[1] || A.min[0] > B.max[1]) return 0;
        if (A.max[2] < B.min[2] || A.min[0] > B.max[2]) return 0;

        // Overlapping on all axes means AABB are intersecting

        return 1;

    }
 
	
};

//class Collision2D
//{
//public:
//    Collision2D() {}
//
//    // This function takes in two rectangles and returns true if they are intersecting.
//    bool CheckRectangleCollision(DirectX::XMFLOAT2 rect1Pos, DirectX::XMFLOAT2 rect1Size, DirectX::XMFLOAT2 rect2Pos, DirectX::XMFLOAT2 rect2Size)
//    {
//        float rect1Left = rect1Pos.x - rect1Size.x / 2.0f;
//        float rect1Right = rect1Pos.x + rect1Size.x / 2.0f;
//        float rect1Top = rect1Pos.y + rect1Size.y / 2.0f;
//        float rect1Bottom = rect1Pos.y - rect1Size.y / 2.0f;
//
//        float rect2Left = rect2Pos.x - rect2Size.x / 2.0f;
//        float rect2Right = rect2Pos.x + rect2Size.x / 2.0f;
//        float rect2Top = rect2Pos.y + rect2Size.y / 2.0f;
//        float rect2Bottom = rect2Pos.y - rect2Size.y / 2.0f;
//
//        if (rect1Right > rect2Left && rect1Left < rect2Right &&
//            rect1Top > rect2Bottom && rect1Bottom < rect2Top)
//        {
//            return true;
//        }
//
//        return false;
//    }
//};

//class Collision
//{
//public:
//    struct Rect
//    {
//        float x;
//        float y;
//        float width;
//        float height;
//    };
//
//
//    struct Platform
//    {
//        Rect rect;
//    };
//
//    void CheckCollision(Player& player, Platform& platform)
//    {
//        // Calculate the player's new position
//        float newX = player.rect.x + player.xSpeed;
//        float newY = player.rect.y + player.ySpeed;
//
//        // Check for horizontal collision
//        if (newX + player.rect.width > platform.rect.x && newX < platform.rect.x + platform.rect.width)
//        {
//            // Check for collision with the top of the platform
//            if (newY + player.rect.height > platform.rect.y && newY < platform.rect.y)
//            {
//                player.ySpeed = 0.0f;
//                player.rect.y = platform.rect.y - player.rect.height;
//            }
//
//            // Check for collision with the bottom of the platform
//            if (newY > platform.rect.y + platform.rect.height && newY < platform.rect.y + platform.rect.height + player.rect.height)
//            {
//                player.ySpeed = 0.0f;
//                player.rect.y = platform.rect.y + platform.rect.height;
//            }
//        }
//
//        // Check for vertical collision
//        if (newY + player.rect.height > platform.rect.y && newY < platform.rect.y + platform.rect.height)
//        {
//            // Check for collision with the left side of the platform
//            if (newX + player.rect.width > platform.rect.x && newX < platform.rect.x)
//            {
//                player.xSpeed = 0.0f;
//                player.rect.x = platform.rect.x - player.rect.width;
//            }
//
//            // Check for collision with the right side of the platform
//            if (newX > platform.rect.x + platform.rect.width && newX < platform.rect.x + platform.rect.width + player.rect.width)
//            {
//                player.xSpeed = 0.0f;
//                player.rect.x = platform.rect.x + platform.rect.width;
//            }
//        }
//
//        // Update the player's position
//        player.rect.x += player.xSpeed;
//        player.rect.y += player.ySpeed;
//    }
//
// 
//
//};






