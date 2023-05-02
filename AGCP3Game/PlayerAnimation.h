#pragma once
#include "GameObject.h"

constexpr int bufferMemory = 8192;

class PlayerAnimation
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
	float JumpDuration = 0.1f;
	float frameDuration = 0.05f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = Zero;
	float deltaTime;
	bool PlayIdle = false;
	int FramesTemp;
	int currentFrame = Zero;
	const int InitState = 0;
	RECT walkspriteSheet[50];
	const int PlayerFrames = 32;
	const int IdleFrames = 24;
	int Frames;
	int IdleOffset = 24;
	int JumpOffset = 27;
	const float delaytime = 5.0f;
};
