#include "UI.h"

#include "Game.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

UserI::UserI(MyD3D& d3d)
	:PauseSpr(d3d)
{

}

UserI::~UserI()

{
}

void UserI::Init(MyD3D& d3d)
{
	//sets the texture for the background to the UI
	ID3D11ShaderResourceView* p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "backgroundlayers/mountains01_007.dds");
	PauseSpr.SetTex(*p);
	//Sets the font for the text
	mpFont = new SpriteFont(&d3d.GetDevice(), L"data/fonts/comicSansMS.spritefont");
}
void UserI::Update(int& pScore, float dTime)
{
	//NOT SURE IF NEEDED
	if (Paused == false)
	{
		Timer += dTime;
		if (Timer >= 1.0f)
		{
			pScore -= 10;
			Timer = 0.0f;
		}
	}

	//On key presses the game either ends or pauses
	if (Game::sMKIn.IsPressed(VK_P) == true)
	{
		Paused = !Paused;
	}
	if (Game::sMKIn.IsPressed(VK_H) == true)
	{
		EndScreen = true;
	}
}
void UserI::Render(float dTime, DirectX::SpriteBatch& batch)
{
	int w, h;
	w = 540;
	h = 270;
	//if the game is paused displays the appropriate background and text
	if (Paused == true)
	{
		string pause = "Paused";
		PauseSpr.origin = (Vector2(255, 127));
		PauseSpr.mPos = (Vector2(w * 0.5f, h * 0.5f));
		PauseSpr.Draw(batch);
		mpFont->DrawString(&batch, pause.c_str(), Vector2(w * 0.5f, h * 0.5f), Vector4(1, 1, 1, 1));
	}
	//if the game has ended displays the appropriate background and text
	if (EndScreen == true)
	{
		PauseSpr.origin = (Vector2(255, 127));
		PauseSpr.mPos = (Vector2(w * 0.5f, h * 0.5f));
		PauseSpr.Draw(batch);
		string pause = "Congratulations you win! \n Press space to continue";
		mpFont->DrawString(&batch, pause.c_str(), Vector2(w * 0.4f, h * 0.5f), Vector4(1, 1, 1, 1));
	}
}