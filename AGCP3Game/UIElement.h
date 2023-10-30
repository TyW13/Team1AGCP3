#pragma once
#include "DeviceManager.h"

class ResourceManager;

class UIElement
{
public:

	UIElement(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, DirectX::SimpleMath::Vector2 objSize, RECT objRect = { 0, 0, 0, 0 })
	{
		Init(dManager, texPath, position, scale, objSize, objRect);
	}

	virtual void Init(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, DirectX::SimpleMath::Vector2 objSize, RECT objRect = { 0, 0, 0, 0 });
	virtual void Render(DeviceManager* dManager);

	virtual void OnClicked(ResourceManager* rManger, DeviceManager* dManager);

	virtual DirectX::SimpleMath::Vector2 GetPosition() const { return mPos; }
	virtual DirectX::SimpleMath::Vector2 GetScale() const { return mScale; }
	virtual DirectX::SimpleMath::Vector2 GetSize() const { return mSize; }
	virtual RECT GetRect() const { return mRect; }

	virtual RECT GetCollisionBounds() const { return mCollisionBounds; }

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
	DirectX::SimpleMath::Vector2 mPos = {};
	DirectX::SimpleMath::Vector2 mScale = {};
	DirectX::SimpleMath::Vector2 mSize = {};
	RECT mRect = { 0, 0, 0, 0 };
	DirectX::SimpleMath::Vector2 mOrigin = { 0, 0 };

	RECT mCollisionBounds;
};

