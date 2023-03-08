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

void Animation::Init(std::string jsonPath, Sprite& Sprite, MyD3D& d3d)
{
	p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "test_chara_walk.dds");
	p2 = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "idle.dds");
	p3 = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "jump.dds");
	jsonPath = "data/" + jsonPath;
	CheckState(jsonPath);
	SwitchTex(Sprite, Zero, InitState);
}
void Animation::Update(float dTime, Sprite &Sprite, std::string animState)
{
	//calculate elapsed time
	float deltaTime = dTime;
	int Frames = 0;
	elapsedTime += deltaTime;
	if (animState == "Left" || "Right")
	{
		Frames = PlayerFrames;
	}
	if (animState == "Stand")
	{
		Frames = IdleFrames;
	}
	//advance to the next frame if enough time has passed
		if (elapsedTime >= frameDuration)
		{
			currentFrame++;
			if (currentFrame >= Frames)
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
			Player.origin = Vector2(0, Player.GetTexData().dim.y);
			Player.SetTex(*p2);
			Player.SetScale(Vector2(0.5,0.5));//Temporary magic numbers for temporary sprite CHANGE
			Player.SetTexRect(idlespriteSheet[currentFrame]);
		}
		if (animState == "Right")
		{
			Player.origin = Vector2(0, Player.GetTexData().dim.y);
			Player.SetTex(*p);
			Player.SetScale(Vector2(kSizeUp, kSizeUp));
			Player.SetTexRect(walkspriteSheet[currentFrame]);
		}
		if (animState == "Left")
		{
			Player.origin = Vector2(0, Player.GetTexData().dim.y);
			Player.SetTex(*p);
			Player.SetScale(Vector2(-kSizeUp, kSizeUp));
			Player.SetTexRect(-walkspriteSheet[currentFrame]);
		}
		if (animState == "Jump")
		{
			Player.origin = Vector2(0, Player.GetTexData().dim.y);
			Player.SetTex(*p3);
			Player.SetScale(Vector2(-kSizeUp, kSizeUp));
			Player.SetTexRect(-walkspriteSheet[Zero]);
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
 			walkspriteSheet[i] = TempRect;
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
			RECTF TempRect = { FrameWidth, FrameHeight,FrameOffsetW,FrameOffsetH };
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
		LoadAnimation(jsonPath);
		//LoadAnimation("data/jump.json");
		LoadIdleAnimation("data/idle.json");
		//LoadAnimationData("data/PlayerData.json);
	}
	/*
	* if (jsonPath == "data/Shotgun.json")
	*	animType = State::Shotgun;
		LoadAnimation(jsonPath);
		LoadAnimationData(data/ShotgunData.json);
	*/
	else
	{
		assert("Animation does not exist");
	}
}

