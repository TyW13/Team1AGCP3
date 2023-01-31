#pragma once

#include "D3DUtil.h"
#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"

class Texture;

class GameObject
{
public:
	GameObject(MyD3D& d3d)
		: gSprite(d3d)
	{
		//Init();
	}
	void Init(Texture*);
	void Update();
	void Terminate();

	Sprite GetSprite();
	bool GetActive();
	void SetSprite(Sprite _sprite);
	void SetActive(bool _isActive);
private:
	Sprite gSprite;
	bool isActive;
};

