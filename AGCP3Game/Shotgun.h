#pragma once
#include "GameObject.h"
#include "AudioManager.h"
#include "ShotgunAnimation.h"

class Shotgun : public GameObject
{
public:
	Shotgun(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, int collisionDirection, RECT objRect = { 0, 0, 8 ,8 })
		: GameObject(dManager, texPath, position, scale, active, objSize, objType, collisionDirection, objRect)
	{
		Init(dManager, texPath, position, scale, active, objSize, objType, collisionDirection, objRect);
	}

	void Init(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, int collisionDirection, RECT objRect = { 0, 0, 8 ,8 }) override;
	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime);
	void Render(DeviceManager* dManager) override;

	bool GetActive() override { return isActive; }
	DirectX::SimpleMath::Vector2 GetObjectSize() override { return objSize; }
	std::string GetObjectType() override { return objType; }
	int GetCollisionDirection() override { return collisionDirection; }
	virtual DirectX::SimpleMath::Vector2 GetPosition() { return mPos; }
	virtual DirectX::SimpleMath::Vector2 GetScale() { return mScale; }

	void SetActive(bool _isActive) override;
	void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize) override;
	void SetCollisionDirection(int _direction);
	void SetRect(RECT _objRect) override;
	void SetPosition(DirectX::SimpleMath::Vector2 _position) override;
	void SetScale(DirectX::SimpleMath::Vector2 _scale) override;
private:
	float Rotation = 0;
	int AnimState = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
	bool isActive;
	DirectX::SimpleMath::Vector2 objSize;
	std::string objType;
	int collisionDirection;
	RECT objRect;
	DirectX::SimpleMath::Vector2 mPos;
	DirectX::SimpleMath::Vector2 mScale;
	DirectX::SimpleMath::Vector2 mOrigin = { 0,0 };
	ShotgunAnimation Anim;
	const DirectX::SimpleMath::Vector2 PosOffset = DirectX::SimpleMath::Vector2(25, 52); // Offset away from the player
	const float Rads180 = 3.14; //Adds 180 degrees in radians
};