#include "Animation.h"
#include "D3D.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
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
	jsonPath = "data/" + jsonPath;
	CheckState(jsonPath);
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
	switch (animType)
	{
	case State::PLAYER:
		if (animState == "Stand")
		{
			Player.SetScale(Vector2(kSizeUp, Player.GetScale().y));
			Player.SetTexRect(spriteSheet[Zero]);
		}
		if (animState == "Right")
		{
			Player.SetScale(Vector2(kSizeUp, Player.GetScale().y));
			Player.SetTexRect(spriteSheet[currentFrame]);
		}
		if (animState == "Left")
		{
			Player.SetScale(Vector2(-kSizeUp, Player.GetScale().y));
			Player.SetTexRect(-spriteSheet[currentFrame]);
		}
		else
		{
			assert("Animation state invalid");
		}
	}
}

void Animation::LoadAnimation(std::string jsonPath)
{
	//From Joshua Moxon project 2
	//Need rapidjson to test
	FILE* Animation = fopen(jsonPath.c_str(), "rb");
	char readBuffer[bufferMemory];
	FileReadStream is(Animation, readBuffer, sizeof(readBuffer));
	Document AnimationDoc;
	AnimationDoc.ParseStream(is);
	int i = 0;
	https://stackoverflow.com/questions/41857273/rapidjson-get-member-name-of-value
	GenericArray fullArray = AnimationDoc["frames"].GetArray();
	for (Value::ConstValueIterator itr = fullArray.Begin(); itr != fullArray.End(); ++itr)
	{
		auto obj = itr->GetObj();
		if (obj.HasMember("frame"))
		{
			auto oneFrame = obj.FindMember("frame");
			auto const& FrameData = oneFrame->value;
			float FrameWidth = FrameData["x"].GetInt();
			float FrameHeight = FrameData["y"].GetInt();
			float FrameOffsetW = FrameData["w"].GetInt();
			float FrameOffsetH = FrameData["h"].GetInt();
			RECTF TempRect = { FrameWidth, FrameHeight,FrameOffsetW,FrameOffsetH };
			spriteSheet[i] = TempRect;
			++i;
		}
	}
}

void Animation::CheckState(std::string jsonPath)
{
	if (jsonPath == "data/test_chara_walk.json")
	{
		animType = State::PLAYER;
	}
	else
	{
		assert("Animation does not exist");
	}
}

