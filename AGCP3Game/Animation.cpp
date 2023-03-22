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
	/*if (jsonPath == "TestSheet.json")
	{
		
	p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "test_chara_walk.dds");
	p2 = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "idle.dds");
	p3 = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "jump.dds");
	}
	else
	{
		p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "Shotgun.dds");
	}*/
	jsonPath = "data/" + jsonPath;
	CheckState(jsonPath);
	SwitchTex(Sprite, Zero, InitState);
}
void Animation::Update(float dTime, GameObject &Sprite, int animState)
{
	//calculate elapsed time
	switch (animType)
	{
	case State::PLAYER:
		float deltaTime = dTime;
		int Frames = Zero;//Temporary magic numbers unil animation data loading is complete
		elapsedTime += deltaTime;
		if (animState == 1 || 2)
		{
			Frames = PlayerFrames;
		}
		if (animState == 0)
		{
			Frames = IdleFrames;
		}
		//advance to the next frame if enough time has passed (Constantly changing frames)
		if (elapsedTime >= frameDuration)
		{
			++currentFrame;
			if (currentFrame >= Frames)
			{
				currentFrame = Zero;
			}
			elapsedTime -= frameDuration;
		}
		SwitchTex(Sprite, currentFrame, animState);
	case State::SHOTGUN:
		float deltaTime = dTime;
		elapsedTime += deltaTime;
		int Frames = 4;//Temporary magic numbers unil animation data loading is complete
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
	}
}

void Animation::SwitchTex(GameObject &Sprite, int currentFrame, int animState)
{
	switch (animType)
	{
	case State::PLAYER:
		if (animState == 0)
		{
			Sprite.SetTex(*p2);
			Sprite.SetScale(Vector2(0.5,0.5));//Temporary magic numbers for temporary sprite CHANGE
			Sprite.SetRect(idlespriteSheet[currentFrame]);
		}
		if (animState == 2)
		{
			Sprite.SetTex(*p);
			Sprite.SetScale(Vector2(kSizeUp, kSizeUp));
			Sprite.SetRect(walkspriteSheet[currentFrame]);
		}
		if (animState == 1)
		{
			Sprite.SetTex(*p);
			Sprite.SetScale(Vector2(-kSizeUp, kSizeUp));
			Sprite.SetRect(-walkspriteSheet[currentFrame]);
		}
		if (animState == 3)
		{
			Sprite.SetTex(*p3);
			Sprite.SetScale(Vector2(-kSizeUp, kSizeUp));
			Sprite.SetRect(-walkspriteSheet[Zero]);
		}
		else
		{
			assert("Animation state invalid");
		}
	case State::SHOTGUN:
		Sprite.SetRect(shotgunSpriteSheet[currentFrame]);
	}
}

void Animation::LoadAnimation(std::string jsonPath, State Object )
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
			RECT TempRect = { FrameWidth, FrameHeight,FrameOffsetW,FrameOffsetH };
			switch (animType)
			{
			case State::PLAYER:
				walkspriteSheet[i] = TempRect;
			case State::SHOTGUN:
				shotgunSpriteSheet[i] = TempRect;
			}
			++i;
		}
	}
}
void Animation::LoadAnimationData(std::string jsonPath)
{
	//Frameworks for any additional animation data that isnt created when creating a texture packer animation
	FILE* Animation = fopen(jsonPath.c_str(), "rb");
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
			RECT TempRect = { FrameWidth, FrameHeight,FrameOffsetW,FrameOffsetH };
			idlespriteSheet[i] = TempRect;
			++i;
		}
	}
}

void Animation::CheckState(std::string jsonPath)
{
	if (jsonPath == "data/TestSheet.json")
	{
		animType = State::PLAYER;
		LoadAnimation(jsonPath, animType);
		//LoadAnimation("data/jump.json");
		LoadIdleAnimation("data/idle.json");
		//LoadAnimationData("data/PlayerData.json);
	}
	
	if (jsonPath == "data/Shotgun.json")
	{
		animType = State::SHOTGUN;
		LoadAnimation(jsonPath, animType);
		//LoadAnimationData(data/ShotgunData.json);
	}
	else
	{
		assert("Animation does not exist");
	}
}

