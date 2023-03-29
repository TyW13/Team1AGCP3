#pragma once
#include "GameObject.h"

class Tile : public GameObject
{
public:
	Tile(DeviceManager* dManager, int texId, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, bool isCollidable, RECT objRect = { 0, 0, 8 ,8 })
		: GameObject(dManager, texId, position, scale, active, objSize, objType, isCollidable, objRect)
	{
		Init(dManager, texId, position, scale, active, objSize, objType, isCollidable, objRect);
	}

	void Init(DeviceManager* dManager, int texId, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, bool isCollidable, RECT objRect = { 0, 0, 8 ,8 }) override;
	void Render(DeviceManager* dManager, ResourceManager& resourceManager) override;

	bool GetActive() override { return isActive; }
	DirectX::SimpleMath::Vector2 GetObjectSize() override { return objSize; }
	std::string GetObjectType() override { return objType; }
	bool GetIsCollidable() override { return isCollidable; }
	RECT GetCollisionBounds() override { return collisionBounds; }
	virtual DirectX::SimpleMath::Vector2 GetPosition() { return mPos; }
	virtual DirectX::SimpleMath::Vector2 GetScale() { return mScale; }
	virtual int GetTexId() { return texId; }

	void SetActive(bool _isActive) override;
	void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize) override;
	void SetIsCollidable(bool _isCollidable) override;
	void SetRect(RECT _objRect) override;
	void SetPosition(DirectX::SimpleMath::Vector2 _position) override;
	void SetScale(DirectX::SimpleMath::Vector2 _scale) override;

private:
	bool isActive;
	DirectX::SimpleMath::Vector2 objSize;
	std::string objType;
	bool isCollidable;
	RECT objRect;
	DirectX::SimpleMath::Vector2 mPos;
	DirectX::SimpleMath::Vector2 mScale;
	DirectX::SimpleMath::Vector2 mOrigin = { 0,0 };
	int texId;

	RECT collisionBounds;
};