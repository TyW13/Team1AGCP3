#include "UI.h"
#include "Input.h"
#include "Game.h"
#include <thread>

using namespace std;

UserI::UserI()
{
}

UserI::~UserI()

{
}

void UserI::Init()
{

}
void UserI::Update(int& pScore, float dTime, bool& Paused)
{
	if (Paused == false)
	{
		Timer += dTime;
		if (Timer >= 1.0f)
		{
			pScore -= 10;
			Timer = 0.0f;
		}
	}
	if (Paused == false)
	{

	}

	if (Game::sMKIn.IsPressed(VK_P) == true)
	{
		this_thread::sleep_for(chrono::milliseconds(80)); // delay to stop it instantly unpausing
		Paused = !Paused;
	}
}