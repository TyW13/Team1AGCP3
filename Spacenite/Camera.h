#pragma once
#include <vector>

#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"

class Camera
{
public:
	Camera();
	~Camera();
	void Init();
	void Update(DirectX::SimpleMath::Vector2);

private:

};

Camera::Camera()
{
}

Camera::~Camera()

{
}

void Camera::Init()
{

}
void Camera::Update(DirectX::SimpleMath::Vector2 pPos)
{
	if (pPos.y >= 512)
	{
		pPos.y = 10;
	}
}
