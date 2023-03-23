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
	Animation();
	~Animation();
	void Init(std::string, GameObject&);
	void Update(float, GameObject&,int);

private:
	enum class State { PLAYER, SHOTGUN };
	void SwitchTex(GameObject&, int,int);
	void LoadAnimation(std::string, State);
	void LoadAnimationData(std::string);
	void LoadIdleAnimation(std::string);
	void CheckState(std::string jsonPath);

	//ID3D11ShaderResourceView* p;
	//ID3D11ShaderResourceView* p2;
	//ID3D11ShaderResourceView* p3;
	State animType = State::PLAYER;
	const int Zero = 0;
	const int kSizeUp = 6;
	float frameDuration = 0.07f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = Zero;
	float deltaTime;
	int FramesTemp;
	int currentFrame = Zero;
	const int InitState = 0;
	RECTF walkspriteSheet[5];
	RECTF idlespriteSheet[5];
	RECTF shotgunSpriteSheet[4];
	const int PlayerFrames = 5;
	const int IdleFrames = 4;
	int Frames;
};

