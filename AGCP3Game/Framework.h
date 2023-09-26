#pragma once

#include "AudioManager.h"
#include "ResourceManager.h"
#include "DeviceManager.h"

class Framework
{
public:

	void Init(DeviceManager* dManager);
	void Update(DeviceManager* dManager, float elapsedTime);
	void Render(DeviceManager* dManager);
	void Terminate();
	void OnSuspending();
	void OnResuming();

private:

	AudioManager audioManager;
	ResourceManager rManager;
	DeviceManager* dManager;
};