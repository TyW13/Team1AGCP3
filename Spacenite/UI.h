#pragma once
#include <vector>
#include "Sprite.h"
#include <SpriteFont.h>
#include "D3D.h"
#include "D3DUtil.h"
#include "Input.h"
#include "WindowUtils.h"

class UserI
{
public:
	UserI(MyD3D& d3d);
	~UserI();
	void Init(MyD3D& d3d);
	void Update(int&, float);
	void Render(float dTime, DirectX::SpriteBatch& batch);
	bool Paused = false;
	bool EndScreen = false;

private:
	float Timer = 0.0f;
	Sprite PauseSpr;
	DirectX::SpriteFont* mpFont = nullptr;
};

