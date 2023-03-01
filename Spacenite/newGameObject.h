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
	GameObject(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)
		: objSprite(d3d)
	{
		Init(objTex, position, scale, active, tileRect, collisionBounds, objnum);
	}

	virtual void Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum);
	virtual void Update(float dTime, ResourceManager& rManager);

	void Terminate();

	virtual Sprite GetSprite();
	virtual bool GetActive();
	virtual void SetSprite(Sprite _sprite);
	virtual void SetActive(bool _isActive);

	
	RECT GetCollisionBounds() { return collision_Bounds; }
private:
	Sprite objSprite;
	bool isActive;

	RECT collision_Bounds;

	Vector2 collisionDimensions;
};

