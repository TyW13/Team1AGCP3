#include "Animation.h"
#include "D3D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
Animation::Animation()
{

}

Animation::~Animation()
{
}

void Animation::Init(std::string jsonPath, Sprite& Sprite)
{
	LoadAnimation(jsonPath);
	SwitchTex(Sprite, Zero, InitState);
}
void Animation::Update(float dTime, Sprite &Sprite, std::string animState)
{
	//calculate elapsed time
	float deltaTime = dTime;
	elapsedTime += deltaTime;

	//advance to the next frame if enough time has passed
		if (elapsedTime >= frameDuration)
		{
			currentFrame++;
			if (currentFrame >= PlayerFrames)
			{
				currentFrame = Zero;
			}
			elapsedTime -= frameDuration;
		}
	SwitchTex(Sprite, currentFrame, animState);

}

void Animation::SwitchTex(Sprite &Player, int currentFrame, std::string animState)
{
	if (animState == "Stand")
	{
		Player.SetScale(Vector2(6, Player.GetScale().y));
		Player.SetTexRect(spriteFrames[Zero]);
	}
	if (animState == "Right")
	{
		Player.SetScale(Vector2(6, Player.GetScale().y));
		Player.SetTexRect(spriteFrames[currentFrame]);
	}
	if (animState == "Left")
	{
		Player.SetScale(Vector2(-6, Player.GetScale().y));
		Player.SetTexRect(flipped_spriteFrames[currentFrame]);
	}
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