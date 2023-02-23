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
	void CheckState(std::string jsonPath);

	enum class State {PLAYER};
	State animType = State::PLAYER;
	const int Zero = 0;
	const float frameDuration = 0.1f; //time in seconds per frame (regulates animation speed)
	float elapsedTime = Zero;
	int currentFrame = Zero;
	const std::string InitState = "Stand";
	RECTF spriteFrames[5] = { {0,0,16,16},{16,0,32,16},{32,0,48,16},{48,0,64,16},{64,0,80,16} }; //TEMPORARILY MAGIC NUMBERS UNTIL JSON IS INCLUDED ERASE ERASE ERASE
	const int PlayerFrames = 5;
};

