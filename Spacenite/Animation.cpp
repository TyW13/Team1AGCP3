#include "Animation.h"

void Animation::Init(std::string jsonPath, Sprite& Sprite)
{
	LoadAnimation(jsonPath);
	SwitchTex(Sprite, Zero);
}
void Animation::Update(float dTime, Sprite &Sprite, std::string animState)
{
	//calculate elapsed time
	float deltaTime = dTime;
	elapsedTime += deltaTime;

	//advance to the next frame if enough time has passed
	if (animState == "Right" || animState == "Left")
	{
		if (elapsedTime >= frameDuration)
		{
			currentFrame++;
			if (currentFrame >= PlayerFrames)
			{
				currentFrame = Zero;
			}
			elapsedTime -= frameDuration;
			SwitchTex(Sprite, currentFrame);
		}
	}

	if (animState == "Stand")
	{
		SwitchTex(Sprite, Zero);
	}
}

void Animation::SwitchTex(Sprite &Player, int currentFrame)
{
	Player.SetTexRect(spriteFrames[currentFrame]);
}

void Animation::LoadAnimation(std::string jsonPath)
{
	//Need rapidjson to test
	/*FILE* Animation = fopen(jsonPath, "rb");
	char readBuffer[4096];
	FileReadStream is(Animation, readBuffer, sizeof(readBuffer));
	Document AnimationDoc;
	AnimationDoc.ParseStream(is);*/
}