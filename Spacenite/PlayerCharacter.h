#pragma once
#include "D3DUtil.h"
#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"
#include "newGameObject.h"

class ResourceManager;

class PlayerCharacter : public GameObject
{
public:
	PlayerCharacter(MyD3D& d3d, ResourceManager* rManager, Texture* objTex, DirectX::SimpleMath::Vector2 scale)
		: GameObject(d3d), objSprite(d3d)
	{
		Init(rManager, objTex, scale);
	}
	void Init(ResourceManager* rManager, Texture* tex, DirectX::SimpleMath::Vector2 scale);
private:
	bool isAcitve = false;
	Sprite objSprite;
};

