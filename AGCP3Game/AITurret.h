#pragma once
#include "GameObject.h"

class AITurret : public GameObject
{
public:
	AITurret();
	virtual ~AITurret();

	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Shoot();

private:

	float m_shotTimer;
	float m_shotDelay;


};




