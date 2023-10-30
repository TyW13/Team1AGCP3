#pragma once
#include "UIElement.h"


class ContinueButton : public UIElement
{
public:
	ContinueButton(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, DirectX::SimpleMath::Vector2 size, RECT rect = { 0, 0, 0, 0 })
		: UIElement(dManager, texPath, position, scale, size)
	{
		Init(dManager, texPath, position, scale, size, rect);
	}

	void Init(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, DirectX::SimpleMath::Vector2 size, RECT rect = { 0, 0, 0, 0 }) override;
	void Render(DeviceManager* dManager) override;

	void OnClicked(ResourceManager* rManger, DeviceManager* dManager) override;

	DirectX::SimpleMath::Vector2 GetPosition() const override { return mPos; }
	DirectX::SimpleMath::Vector2 GetScale() const override { return mScale; }
	DirectX::SimpleMath::Vector2 GetSize() const override { return mSize; }
	RECT GetRect() const override { return mRect; }

	RECT GetCollisionBounds() const override { return mCollisionBounds; }

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
	DirectX::SimpleMath::Vector2 mPos = {};
	DirectX::SimpleMath::Vector2 mScale = {};
	DirectX::SimpleMath::Vector2 mSize = {};
	RECT mRect = { 0, 0, 0, 0 };
	DirectX::SimpleMath::Vector2 mOrigin = { 0, 0 };

	RECT mCollisionBounds;
};