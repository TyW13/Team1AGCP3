#include "Framework.h"

void Framework::Init(DeviceManager* dManager)
{
	rManager.Init(dManager);

}
void Framework::Update(DeviceManager* dManager, float elapsedTime)
{
	rManager.Update(dManager, elapsedTime);
}

void Framework::Render(DeviceManager* dManager)
{
	rManager.Render(dManager);
}

void Framework::Terminate()
{
}

void Framework::OnSuspending()
{
	audioManager.OnSuspending();
}

void Framework::OnResuming()
{
	audioManager.OnResuming();
}
