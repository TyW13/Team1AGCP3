#pragma once

#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"
#include "Texture.h"
#include "D3DUtil.h"

class ResourceManager;

class GameObject
{
public:
	GameObject(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, Vector2 objSize, std::string objType, bool isCollidable, RECTF objRect = { .0f,.0f ,.0f ,.0f })
		: objSprite(d3d)
	{
		Init(objTex, position, scale, active, objSize, objType, isCollidable, objRect);
	}

	virtual void Init(Texture* _tex, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, Vector2 _objSize, std::string _objType, bool _isCollidable, RECTF _objRect = { .0f,.0f ,.0f ,.0f });
	virtual void Update(MyD3D& d3d, float dTime, ResourceManager& rManager);

	virtual Sprite GetSprite();
	virtual bool GetActive();
	virtual Vector2 GetObjectSize() { return objSize; }
	virtual std::string GetObjectType() { return objType; }
	virtual bool GetIsCollidable() { return isCollidable; }
	RECTF GetCollisionBounds() { return collisionBounds; }

	virtual void SetSprite(Sprite _sprite);
	virtual void SetActive(bool _isActive);
	virtual void SetObjectSize(Vector2 _objSize);
	virtual void SetIsCollidable(bool _isCollidable);
	

private:
	Sprite objSprite;
	bool isActive;
	Vector2 objSize;
	std::string objType;
	bool isCollidable;

	RECTF collisionBounds;
};

