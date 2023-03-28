#pragma once
#include "GameObject.h"
#include "Bullet.h"



class ResourceManager;

class AITurret : public GameObject
{
public:
	AITurret(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, bool isCollidable, RECT objRect = { 0,0,0,0 })
		: GameObject(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect)
	{
		Init(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect);
	}




	void Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, bool isCollidable, RECT objRect = { 0, 0, 0 ,0 }) override;
	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Render(DeviceManager* dManager) override;


	//void UpdateInput


	bool GetActive() override { return isActive; }
	DirectX::SimpleMath::Vector2 GetObjectSize() override { return objSize; }
	std::string GetObjectType() override { return objType; }
	bool GetIsCollidable() override { return isCollidable; }
	RECT GetCollisionBounds() override { return collisionBounds; }
	virtual DirectX::SimpleMath::Vector2 GetPosition() { return mPos; }
	virtual DirectX::SimpleMath::Vector2 GetScale() { return mScale; }



	void SetActive(bool _isActive) override;
	void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize) override;
	void SetIsCollidable(bool _isCollidable) override;
	void SetRect(RECT _objRect) override;
	void SetPosition(DirectX::SimpleMath::Vector2 _position) override;
	void SetScale(DirectX::SimpleMath::Vector2 _scale) override;


private:
	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
	bool isActive;
	DirectX::SimpleMath::Vector2 objSize;
	std::string objType;


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