#pragma once
#include "GameObject.h"

constexpr int bufferMemory = 8192;
struct RECTF
{
	float left, top, right, bottom;
	operator RECT() {
		return RECT{ (int)left,(int)top,(int)right,(int)bottom };
	}
	RECTF operator-()
	{
		return RECTF{ -(float)left,(float)top,-(float)right,(float)bottom };
	}
};
class Animation
{
public:
	void Init(std::string,std::string, GameObject&);
	void Update(float, GameObject&);

private:
	void SwitchTex(GameObject&, int);
	void LoadAnimation(std::string);
	void LoadAnimationData(std::string);
	void LoadIdleAnimation(std::string);

	const int Zero = 0;
	const int kSizeUp = 6;
	float frameDuration = 0.07f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = Zero;
	float deltaTime;
	int FramesTemp;
	int currentFrame = Zero;
	RECTF SpriteSheet[4];
	int Frames = 5; //Temporary
};

