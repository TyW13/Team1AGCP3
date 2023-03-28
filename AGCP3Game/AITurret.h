#pragma once
#include "GameObject.h"
#include "Bullet.h"



class ResourceManager;

class AITurret : public GameObject
{
public:
	AITurret(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, bool isCollidable, RECT objRect = {0,0,0,0})
		: GameObject(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect)
	{
		Init(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect);
	}




	void Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, bool isCollidable, RECT objRect = { 0, 0, 0 ,0 }) override;
	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Render(DeviceManager* dManager) override;




};










//class AITurret : public GameObject
//{
//public:
//	AITurret();
//	virtual ~AITurret();
//
//
//	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime);
//	void Shoot();
//
//private:
//
//	float m_shotTimer;
//	float m_shotDelay;
//
//
//
//
//};




//DirectX::SimpleMath::Vector2 GetVelocity() const;
//DirectX::SimpleMath::Vector2 SetVelocity(float x, float y);
//DirectX::SimpleMath::Vector2 m_bulletVelocity;