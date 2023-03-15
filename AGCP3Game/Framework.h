#pragma once


//#include "GameRenderer.h"

//class GameRenderer;

#include "SpriteBatch.h"
#include <Keyboard.h>

class Framework
{



public:
	void Initialise();
	void Update(float elapsedTime);

	void Terminate();



private:

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//DirectX::SimpleMath::Vector2 m_screenPos;
	//DirectX::SimpleMath::Vector2 m_origin;

	//GameRenderer* m_pRenderer;

	//std::unique_ptr<Keyboard> keyboard;

};

