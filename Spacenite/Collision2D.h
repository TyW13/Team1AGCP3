#pragma once
#include "GameObject.h"

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
        return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
    }

 
	
};



class Collision
{
public:
    struct Rect
    {
        float x;
        float y;
        float width;
        float height;
    };

    struct Player
    {
        Rect rect;
        float xSpeed;
        float ySpeed;
    };

    struct Platform
    {
        Rect rect;
    };

    void CheckCollision(Player& player, Platform& platform)
    {
        // Calculate the player's new position
        float newX = player.rect.x + player.xSpeed;
        float newY = player.rect.y + player.ySpeed;

        // Check for horizontal collision
        if (newX + player.rect.width > platform.rect.x && newX < platform.rect.x + platform.rect.width)
        {
            // Check for collision with the top of the platform
            if (newY + player.rect.height > platform.rect.y && newY < platform.rect.y)
            {
                player.ySpeed = 0.0f;
                player.rect.y = platform.rect.y - player.rect.height;
            }

            // Check for collision with the bottom of the platform
            if (newY > platform.rect.y + platform.rect.height && newY < platform.rect.y + platform.rect.height + player.rect.height)
            {
                player.ySpeed = 0.0f;
                player.rect.y = platform.rect.y + platform.rect.height;
            }
        }

        // Check for vertical collision
        if (newY + player.rect.height > platform.rect.y && newY < platform.rect.y + platform.rect.height)
        {
            // Check for collision with the left side of the platform
            if (newX + player.rect.width > platform.rect.x && newX < platform.rect.x)
            {
                player.xSpeed = 0.0f;
                player.rect.x = platform.rect.x - player.rect.width;
            }

            // Check for collision with the right side of the platform
            if (newX > platform.rect.x + platform.rect.width && newX < platform.rect.x + platform.rect.width + player.rect.width)
            {
                player.xSpeed = 0.0f;
                player.rect.x = platform.rect.x + platform.rect.width;
            }
        }

        // Update the player's position
        player.rect.x += player.xSpeed;
        player.rect.y += player.ySpeed;
    }
};
