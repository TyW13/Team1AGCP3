#pragma once
#include "newGameObject.h"

class ReloadGem : public GameObject
{
public:
	ReloadGem(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, Vector2 objSize, std::string objType, bool isCollidable, RECTF objRect = { .0f,.0f ,.0f ,.0f })
		: GameObject(d3d, objTex, position, scale, active, objSize, objType, isCollidable, objRect), objSprite(d3d)
	{
		Init(objTex, position, scale, active, objSize, objType, isCollidable, objRect);
	}

	void Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, Vector2 objSize, std::string _objType, bool isCollidable, RECTF objRect = { .0f,.0f ,.0f ,.0f }) override;

	Sprite GetSprite() override;
	bool GetActive();
	Vector2 GetObjectSize() override { return objSize; }
	bool GetIsCollidable() override { return isCollidable; }
	RECTF GetCollisionBounds() { return collisionBounds; }

	void SetSprite(Sprite _sprite) override;
	void SetActive(bool _isActive) override;
	void SetObjectSize(Vector2 _objSize) override;
	void SetIsCollidable(bool _isCollidable) override;
private:
	Sprite objSprite;
	bool isActive;
	Vector2 objSize;
	std::string objType;
	bool isCollidable;

	RECTF collisionBounds;
};

