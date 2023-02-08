#include "UI.h"

UserI::UserI()
{
}

UserI::~UserI()

{
}

void UserI::Init()
{

}
void UserI::Update(int& pScore, float dTime)
{
	Timer += dTime;
	if (Timer >= 1.0f)
	{
		pScore -= 10;
		Timer = 0.0f;
	}
}