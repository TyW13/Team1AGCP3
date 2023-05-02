#pragma once

#include "DeviceManager.h"

class ResourceManager;

class GameObject
{
public:
	GameObject(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, int collisionDirection, RECT objRect = { 0, 0, 0 ,0 })
	{
		Init(dManager, texPath, position, scale, active, objSize, objType, collisionDirection, objRect);
	}

	virtual void Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, int _collisionDirection, RECT objRect = { 0, 0, 0 ,0 });
	virtual void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime);
	virtual void Render(DeviceManager* dManager);

	virtual bool GetActive() { return isActive; }
	virtual DirectX::SimpleMath::Vector2 GetObjectSize() { return objSize; }
	virtual std::string GetObjectType() { return objType; }
	virtual int GetCollisionDirection() { return collisionDirection; }
	virtual RECT GetCollisionBounds() { return collisionBounds; }
	virtual DirectX::SimpleMath::Vector2 GetPosition() { return mPos; }
	virtual DirectX::SimpleMath::Vector2 GetScale() { return mScale; }

	virtual void SetActive(bool _isActive);
	virtual void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize);
	virtual void SetCollisionDirection(int _direction);
	virtual void SetRect(RECT _objRect);
	virtual void SetPosition(DirectX::SimpleMath::Vector2 _position);
	virtual void SetScale(DirectX::SimpleMath::Vector2 _scale);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
	bool isActive;
	DirectX::SimpleMath::Vector2 objSize;
	std::string objType;
	int collisionDirection;
	RECT objRect;
	DirectX::SimpleMath::Vector2 mPos;
	DirectX::SimpleMath::Vector2 mScale;
	DirectX::SimpleMath::Vector2 mOrigin = { 0,0 };

	RECT collisionBounds;
};

