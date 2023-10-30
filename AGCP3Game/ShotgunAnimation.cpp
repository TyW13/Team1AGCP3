#include "ShotgunAnimation.h"
#include "D3D.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void ShotgunAnimation::Init(std::string jsonPath, GameObject& Sprite)
{
	//Grabs all the necessary data
	jsonPath = "Data/" + jsonPath;
	if (jsonPath == "Data/Shotgun/Shotgun.json")
	{
		LoadAnimation(jsonPath);
		LoadAnimationData("Data/Shotgun/Shotgun_AnimData.json");
		SwitchTex(Sprite, 6, InitState);
	}
	else
	{
		assert("Animation does not exist");
	}
}
void ShotgunAnimation::Update(float dTime, GameObject& Sprite, int animState)
{
	//calculate elapsed time
	deltaTime = dTime;
	elapsedTime += deltaTime;

	//Checks to make sure the gun has fired before animating
	if (currentFrame < 5)
	{
		animState = 1;
	}
	//Checks for when the gun is appearing/reloading
	if (animState == 0)
	{
		if (currentFrame <= 5)
		{
			currentFrame = 6;
		}
		if (elapsedTime >= frameDuration)
		{
			if (currentFrame < Frames)
			{
				++currentFrame;
			}
			elapsedTime -= frameDuration;
		}
	}
	//Checks to make the firing animation move along 
	if (animState == 1)
	{
		if (currentFrame >= 6)
		{
			currentFrame = Zero;
		}
		if (elapsedTime >= frameDuration)
		{
			if (currentFrame <= 4)
			{
				++currentFrame;
			}
			elapsedTime -= frameDuration;
		}
	}
	SwitchTex(Sprite, currentFrame, animState);
}

void ShotgunAnimation::SwitchTex(GameObject& Sprite, int currentFrame, int animState)
{
	//Sets the necessary RECT
	Sprite.SetRect(shotgunSpriteSheet[currentFrame]);
}

void ShotgunAnimation::LoadAnimation(std::string jsonPath)
{
	constexpr int bufferMemory = 8192;
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
	
	rapidjson::Value fullArray = AnimationDoc["frames"].GetArray();
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
			RECT TempRect = { FrameWidth, FrameHeight,FrameOffsetW,FrameOffsetH };
			shotgunSpriteSheet[i] = TempRect;
			++i;
		}
	}
}
void ShotgunAnimation::LoadAnimationData(std::string jsonPath)
{
	constexpr int bufferMemory = 8192;
	//Grabs for any additional animation data that isnt created when creating a texture packer animation such as frame duration
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