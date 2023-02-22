#pragma once
#include "Sprite.h"

class Animation
{
public:
	Animation();
	~Animation();
	void Init(std::string, Sprite&);
	void Update(float, Sprite&, std::string);

private:
	void SwitchTex(Sprite&, int, std::string);
	void LoadAnimation(std::string);

	const int Zero = 0;
	const float frameDuration = 0.1f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = Zero;
	int currentFrame = Zero;
	const std::string InitState = "Stand";
	const RECTF spriteFrames[5] = { {0,0,16,16},{16,0,32,16},{32,0,48,16},{48,0,64,16},{64,0,80,16} }; //TEMPORARILY MAGIC NUMBERS UNTIL JSON IS INCLUDED ERASE ERASE ERASE
	const RECTF flipped_spriteFrames[5] = {
		{-spriteFrames[0].left, spriteFrames[0].top, -spriteFrames[0].right, spriteFrames[0].bottom},
		{-spriteFrames[1].left, spriteFrames[1].top, -spriteFrames[1].right, spriteFrames[1].bottom},
		{-spriteFrames[2].left, spriteFrames[2].top, -spriteFrames[2].right, spriteFrames[2].bottom},
		{-spriteFrames[3].left, spriteFrames[3].top, -spriteFrames[3].right, spriteFrames[3].bottom},
		{-spriteFrames[4].left, spriteFrames[4].top, -spriteFrames[4].right, spriteFrames[4].bottom} };
	const int PlayerFrames = 5;
};

