#pragma once
#include "GameObject.h"
#include "Bullet.h"


class AITurret : public GameObject
{
public:
	AITurret();
	virtual ~AITurret();


	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime);
	void Shoot();

private:

	float m_shotTimer;
	float m_shotDelay;




};




//DirectX::SimpleMath::Vector2 GetVelocity() const;
//DirectX::SimpleMath::Vector2 SetVelocity(float x, float y);
//DirectX::SimpleMath::Vector2 m_bulletVelocity;