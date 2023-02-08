#pragma once
#include <vector>

#include "Sprite.h"

class UserI
{
public:
	UserI();
	~UserI();
	void Init();
	void Update(int&, float, bool&, bool&);

private:
	float Timer = 0.0f;
};

