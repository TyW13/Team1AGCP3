#pragma once
#include "GameObject.h"

class ShotgunAnimation
{
public:
	void Init(std::string, GameObject&);
	void Update(float, GameObject&, int);
private:
	void SwitchTex(GameObject&, int, int);
	void LoadAnimation(std::string);
	void LoadAnimationData(std::string);
	const int Zero = 0;
	const int kSizeUp = 6;
	float frameDuration = 0.07f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = Zero;
	float deltaTime;
	int currentFrame = 6;
	const int InitState = 1;
	RECT shotgunSpriteSheet[11];
	int Frames;
};