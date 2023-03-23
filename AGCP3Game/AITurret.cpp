#include "AITurret.h"


AITurret::AITurret() :
    GameObject()
{
    // Set initial values
    m_shotTimer = 0.0f;
    m_shotDelay = 1.5f;
}

AITurret::~AITurret()
{
}



void AITurret::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
    // Update shot timer
    m_shotTimer += dTime;

    // If shot timer has expired, shoot
    if (m_shotTimer >= m_shotDelay)
    {
        Shoot();

        // Reset shot timer
        m_shotTimer = 0.0f;
    }
}

void AITurret::Shoot()
{
    // Create a new bullet object
    Bullet* bullet = new Bullet();
    bullet->SetPosition(GetPosition());
    bullet->SetVelocity(DirectX::SimpleMath::Vector2(0.0f, -1.0f));
}