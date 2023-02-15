#include "Background.h"

#include "WindowUtils.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
void Background::Init(MyD3D& mD3D)
{
	//a sprite for each layer
	//assert(bGround.empty());
	//bGround.insert(bGround.begin(), BGND_LAYERS, Sprite(mD3D));



	//a neat way to package pairs of things (nicknames and filenames)
	pair<string, string> files[BGND_LAYERS]{
		{ "bgnd0","backgroundlayers/mountains01_007.dds" },
		{ "bgnd1","backgroundlayers/mountains01_005.dds" },
		{ "bgnd2","backgroundlayers/mountains01_004.dds" },
		{ "bgnd3","backgroundlayers/mountains01_003.dds" },
		{ "bgnd4","backgroundlayers/mountains01_002.dds" },
		{ "bgnd5","backgroundlayers/mountains01_001.dds" },
		{ "bgnd6","backgroundlayers/mountains01_000.dds" },
		{ "bgnd7","backgroundlayers/mountains01_006.dds" }
	};
	int x = 0;
	int y = 0;
	for (auto& f : files)
	{
		//set each texture layer
		ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), f.second, f.first);
		if (!p)
			assert(false);
		bGroundTest[x][y].SetTex(*p);
		++x;
		if (x == 2)
		{
			++y;
			x = 0;
		}
	}
}

void Background::Update(float dTime, Vector2& Pos)
{
	//Reset all booleans on each update loop
	IsTop = false;
	IsBottom = false;
	IsLeft = false;
	IsRight = false;
	//Check the player to see where they are (Needs moving back to GameObject)
	CheckBounds(Pos);
	//Change the x and y position of background array
	Increase(IsTop, IsBottom);
	//Move the backgrounds(Not Needed after levels are included)
	bGroundTest[currentx][currenty].Scroll(dTime * SCROLL_SPEED, 0);
}

void Background::Render(DirectX::SpriteBatch& batch)
{
	bGroundTest[currentx][currenty].Draw(batch);
}

void Background::Increase(bool IsTop, bool IsBottom)
{
	if (IsTop == true)
	{
		++currenty;
		if (currenty >= 3)
		{
			IsMaxY = true;
			currenty = 3;
		}
		else
		{
			IsMaxY = false;
		}
	}
	if (IsBottom == true)
	{
		--currenty;
		if (currenty <= 0)
		{
			IsMinY = true;
			currenty = 0;
		}
		else
		{
			IsMinY = false;
		}
	}
	if (IsLeft == true)
	{
		++currentx;
		if (currentx >= 1)
		{
			IsMaxX = true;
			currentx = 1;
		}
		else
		{
			IsMaxX = false;
		}

	}
	if (IsRight == true)
	{
		--currentx;
		if (currentx <= 0)
		{
			IsMinX = true;
			currentx = 0;
		}
		else
		{
			IsMinX = false;
		}
	}
}

void Background::CheckBounds(Vector2& mPos)
{
	//for code clarity
	Vector2 playerOrigin = Vector2((38.4, 38.4));

	//check for player and screen borders collision

	//bottom
	if (mPos.y > WinUtil::Get().GetClientHeight() + 60)
	{
		if (IsMinY == true)
		{
			mPos.y = WinUtil::Get().GetClientHeight() + 59;
		}
		else
		{
			mPos.y = playerOrigin.y;
			IsBottom = true;
			IsMaxY = false;
		}
	}
	//top
	if (mPos.y < playerOrigin.y)
	{
		if (IsMaxY == true)
		{
			mPos.y = playerOrigin.y + 1;
		}
		else
		{
			mPos.y = WinUtil::Get().GetClientHeight() + 30;
			IsTop = true;
			IsMinY = false;
		}
	}
	//right
	if (mPos.x > WinUtil::Get().GetClientWidth() + 15)
	{
		if (IsMinX == true)
		{
			mPos.x = WinUtil::Get().GetClientWidth() + 14;
		}
		else
		{
			mPos.x = -14;
			IsRight = true;
			IsMaxX = false;
		}
	}
	//left
	if (mPos.x < -15)
	{
		if (IsMaxX == true)
		{
			mPos.x = -14;
		}
		else
		{
			mPos.x = WinUtil::Get().GetClientWidth() + 14;
			IsLeft = true;
			IsMinX = false;
		}
	}
}