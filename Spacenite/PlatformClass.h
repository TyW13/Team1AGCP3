#include <d3d11.h>
#include <DirectXMath.h>
#include "Sprite.h"
#include "WindowUtils.h"

using namespace DirectX;


// Vertex structure for a 2D platform
struct PlatformVertex
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
};

class Platform
{
public:

 
    // Create instances

    Platform(float xPos, float yPos, float width, float height)
        : X(xPos), Y(yPos), Width(width), Height(height)
    {}

    // Retrieve Position

    XMFLOAT2 GetPosition() const
    {
        return XMFLOAT2(X, Y);
    }

    // Get Size
    XMFLOAT2 GetSize() const
    {
        return XMFLOAT2(Width, Height);
    }

    void RenderPlatform()
    {
        // Create a platform instance
        Platform platform(100.0f, 200.0f, 300.0f, 50.0f);

        // Create the vertex buffer for the platform
        PlatformVertex vertices[] =
        {
            { XMFLOAT3(platform.GetPosition().x, platform.GetPosition().y, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(platform.GetPosition().x + platform.GetSize().x, platform.GetPosition().y, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(platform.GetPosition().x, platform.GetPosition().y + platform.GetSize().y, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(platform.GetPosition().x + platform.GetSize().x, platform.GetPosition().y + platform.GetSize().y, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
        };

        // Code to set up Direct3D device and rendering context...

        // Create the vertex buffer
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(PlatformVertex) * 4;
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
    }


    // Check for intersects with
    // Platform
    // Other - Player/Missile
    bool IntersectsWith(const Platform& other) const
    {
        return X < other.X + other.Width && X + Width > other.X &&
            Y < other.Y + other.Height && Y + Height > other.Y;
    }

    

private:
    float X, Y;
    float Width, Height;



};





class Platformer
{
private:
    XMFLOAT2 playerPosition;
    XMFLOAT2 platformPosition;
    XMFLOAT2 playerSize;
    XMFLOAT2 platformSize;

    bool CheckPlayerCollisionWithPlatform()
    {
        // Get the bounding box for the player
        float playerLeft = playerPosition.x - playerSize.x / 2.0f;
        float playerRight = playerPosition.x + playerSize.x / 2.0f;
        float playerTop = playerPosition.y + playerSize.y / 2.0f;
        float playerBottom = playerPosition.y - playerSize.y / 2.0f;

        // Get the bounding box for the platform
        float platformLeft = platformPosition.x - platformSize.x / 2.0f;
        float platformRight = platformPosition.x + platformSize.x / 2.0f;
        float platformTop = platformPosition.y + platformSize.y / 2.0f;
        float platformBottom = platformPosition.y - platformSize.y / 2.0f;

        // Check if the bounding boxes intersect
        if (playerRight > platformLeft && playerLeft < platformRight &&
            playerTop > platformBottom && playerBottom < platformTop)
        {
            return true;
        }

        return false;
    }

    bool OnGround()
    {
        // Check if the player is colliding with the platform
        if (CheckPlayerCollisionWithPlatform())
        {
            // Check if the player's bottom is above the platform's top
            float playerBottom = playerPosition.y - playerSize.y / 2.0f;
            float platformTop = platformPosition.y + platformSize.y / 2.0f;
            if (playerBottom < platformTop)
            {
                return true;
            }
        }

        return false;
    }

public:
    Platformer()
    {
        playerPosition = XMFLOAT2(0.0f, 0.0f);
        platformPosition = XMFLOAT2(0.0f, -10.0f);
        playerSize = XMFLOAT2(1.0f, 1.0f);
        platformSize = XMFLOAT2(10.0f, 2.0f);
    }

    void PlatformInit()
    {
        
    }


 
    void Update(float deltaTime)
    {
        if (OnGround())
        {
            
        }
    }
};


//
//// Check for player-platform collision
//if (CheckPlayerCollisionWithPlatform())
//{
//    // Handle the collision, for example, by adjusting the player's position or velocity
//}