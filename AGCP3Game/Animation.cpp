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

void Animation::Init(std::string jsonPath, GameObject& Sprite)
{
	jsonPath = "Data/" + jsonPath;
	LoadAnimation(jsonPath)
	SwitchTex(Sprite, Zero, InitState);
}
void Animation::Update(float dTime, GameObject &Sprite, int animState)
{
	//calculate elapsed time
	switch (animType)
	{
	case State::PLAYER:
		deltaTime = dTime;
		FramesTemp = Zero;//Temporary magic numbers unil animation data loading is complete
		elapsedTime += deltaTime;
		if (animState == 1 || animState == 2)
		{
			FramesTemp = PlayerFrames;
		}
		if (animState == 0)
		{
			FramesTemp = IdleFrames;
		}
		//advance to the next frame if enough time has passed (Constantly changing frames)
		if (elapsedTime >= frameDuration)
		{
			++currentFrame;
			if (currentFrame >= FramesTemp)
			{
				currentFrame = Zero;
			}
			elapsedTime -= frameDuration;
		}
		SwitchTex(Sprite, currentFrame, animState);
		break;
	case State::SHOTGUN:
		deltaTime = dTime;
		elapsedTime += deltaTime;
		Frames = 4;//Temporary magic numbers unil animation data loading is complete
		if (animState == 1 && currentFrame <= Zero)
		{
			if (elapsedTime >= frameDuration)
			{
				if (currentFrame <= Frames)
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
		break;
	}
}

void Animation::SwitchTex(GameObject &Sprite, int currentFrame, int animState)
{
	switch (animType)
	{
	case State::PLAYER:
	
		if (animState == 0)
		{
			Sprite.SetScale(Vector2(kSizeUp, kSizeUp));
			Sprite.SetRect(walkspriteSheet[currentFrame]);
		}
		if (animState == 1)
		{
			//Sprite.SetScale(Vector2(kSizeUp, kSizeUp));
			//Sprite.SetRect(walkspriteSheet[currentFrame]);
		}
		if (animState == 2)
		{
			Sprite.SetScale(Vector2(-kSizeUp, kSizeUp));
			Sprite.SetRect(-walkspriteSheet[currentFrame]);
		}
		if (animState == 3)
		{
			Sprite.SetScale(Vector2(-kSizeUp, kSizeUp));
			//Sprite.SetRect(-walkspriteSheet[Zero]);
		}
		else
		{
			assert("Animation state invalid");
		}
		break;
	case State::SHOTGUN:
	
		Sprite.SetRect(shotgunSpriteSheet[currentFrame]);
		break;
	
	}
}

void Animation::LoadAnimation(std::string jsonPath, State Object )
{
	//From Joshua Moxon project 2
	//Need rapidjson to test
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
			switch (animType)
			{
			case State::PLAYER:
				walkspriteSheet[i] = TempRect;
				break;
			case State::SHOTGUN:
				shotgunSpriteSheet[i] = TempRect;
				break;
			}
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
//Temp Function until a single spritesheet is created
void Animation::LoadIdleAnimation(std::string jsonPath)
{
	//From Joshua Moxon project 2
	//Need rapidjson to test
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
			idlespriteSheet[i] = TempRect;
			++i;
		}
	}
}

