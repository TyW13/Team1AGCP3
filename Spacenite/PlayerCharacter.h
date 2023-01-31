#pragma once
#include "D3DUtil.h"
#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"
#include "newGameObject.h"
class PlayerCharacter : public GameObject
{
public:
	PlayerCharacter(MyD3D& d3d) 
		: GameObject(d3d), objSprite(d3d)
	{
		Init()
	}
	void Init(Texture* tex);
private:
	bool isAcitve = false;
	Sprite objSprite;
};

