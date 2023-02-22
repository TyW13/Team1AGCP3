#include "Animation.h"

void Animation::Init()
{
	//LoadAnimation("Test.json");
}
void Animation::Update(float dTime, Sprite &Player, std::string animState)
{
	//calculate elapsed time
	float deltaTime = dTime;
	elapsedTime += deltaTime;

	//advance to the next frame if enough time has passed
	if (animState == "Right")
	{
		if (elapsedTime >= frameDuration)
		{
			currentFrame++;
			if (currentFrame >= 5)
			{
				currentFrame = 0;
			}
			elapsedTime -= frameDuration;
			SwitchTex(Player, currentFrame);
		}
	}
}

void Animation::SwitchTex(Sprite &Player, int currentFrame)
{
	Player.SetTexRect(spriteFrames[currentFrame]);
}

void Animation::LoadAnimation(std::string jsonPath)
{
	/*FILE* Animation = fopen("Test.json", "rb");
char readBuffer[4096];
FileReadStream is(Animation, readBuffer, sizeof(readBuffer));
Document AnimationDoc;
AnimationDoc.ParseStream(is);*/
}