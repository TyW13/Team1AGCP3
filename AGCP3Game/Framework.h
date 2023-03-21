#pragma once

#include "Player.h"
//#include "GameRenderer.h"

//class GameRenderer;

#include "pch.h"
#include "SpriteBatch.h"
#include <Keyboard.h>
#include "AudioManager.h"
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

class Framework
{

public:
	void Initialise(HWND window, int width, int height);
	void Update(float elapsedTime);
	void Terminate();
	void OnSuspending();
	void OnResuming();


private:

	AudioManager audio;

    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;
    Player player;
};

