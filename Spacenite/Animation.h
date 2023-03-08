#pragma once
#include "Sprite.h"

constexpr int bufferMemory = 8192;
class Animation
{
public:
	Animation();
	~Animation();
	void Init(std::string, Sprite&, MyD3D& d3d);
	void Update(float, Sprite&, std::string);

private:
	void SwitchTex(Sprite&, int, std::string);
	void LoadAnimation(std::string);
	void LoadIdleAnimation(std::string);
	void CheckState(std::string jsonPath);

	ID3D11ShaderResourceView* p;
	ID3D11ShaderResourceView* p2;
	ID3D11ShaderResourceView* p3;
	enum class State {PLAYER};
	State animType = State::PLAYER;
	const int Zero = 0;
	const int kSizeUp = 6;
	const float frameDuration = 0.1f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = Zero;
	int currentFrame = Zero;
	const std::string InitState = "Stand";
	RECTF walkspriteSheet[5];
	RECTF idlespriteSheet[5];
	const int PlayerFrames = 5;
	const int IdleFrames = 4;
};

