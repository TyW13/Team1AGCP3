#include "ShotgunAnimation.h"
#include "D3D.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void Animation::Init(std::string jsonPath, GameObject& Sprite)
{
	jsonPath = "Data/" + jsonPath;
	if (jsonPath == "Data/Shotgun.json")
	{
		LoadAnimation(jsonPath);
		//LoadAnimationData(Data/ShotgunData.json);
		SwitchTex(Sprite, Zero, InitState);
	}
	else
	{
		assert("Animation does not exist");
	}
}
void Animation::Update(float dTime, GameObject& Sprite, int animState)
{
	//calculate elapsed time
	deltaTime = dTime;
	elapsedTime += deltaTime;
	Frames = ShotgunFrames;//Temporary magic numbers unil animation data loading is complete
	if (animState == 1 && currentFrame <= Zero)
	{
		if (elapsedTime >= frameDuration)
		{
			if (currentFrame < Frames)
			{
				++currentFrame;
			}
			elapsedTime -= frameDuration;
		}
	}
	if (animState == Zero && currentFrame >= Zero)
	{
		if (elapsedTime >= frameDuration)
		{
			if (currentFrame >= Zero)
			{
				--currentFrame;
			}
			elapsedTime -= frameDuration;
		}
	}
	SwitchTex(Sprite, currentFrame, animState);
}

void Animation::SwitchTex(GameObject& Sprite, int currentFrame, int animState)
{
	Sprite.SetRect(shotgunSpriteSheet[currentFrame]);
}

void Animation::LoadAnimation(std::string jsonPath)
{
	//From Joshua Moxon project 2
	FILE* Animation;
	errno_t levelsStatus = fopen_s(&Animation, jsonPath.c_str(), "rb");
	if (levelsStatus != 0)
	{
		printf("ERROR: Could not open file!");
		assert(false);
	}
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
			shotgunSpriteSheet[i] = TempRect;
			++i;
		}
	}
}
void Animation::LoadAnimationData(std::string jsonPath)
{
	//Frameworks for any additional animation data that isnt created when creating a texture packer animation
	FILE* Animation;
	errno_t levelsStatus = fopen_s(&Animation, jsonPath.c_str(), "rb");
	if (levelsStatus != 0)
	{
		printf("ERROR: Could not open file!");
		assert(false);
	}
	char readBuffer[bufferMemory];
	FileReadStream is(Animation, readBuffer, sizeof(readBuffer));
	Document AnimationDoc;
	AnimationDoc.ParseStream(is);
	frameDuration = AnimationDoc["Duration"].GetFloat();
	Frames = AnimationDoc["FrameCount"].GetInt();
}