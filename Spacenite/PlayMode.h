#if !defined(PlayModeH)
#define PlayModeH

#include <fstream>

#include "Input.h"
#include "D3D.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Sprite.h"
#include "Background.h"
#include "GameObject.h"



class PlayMode
{
public:
	PlayMode(MyD3D& d3d);
	void Release();
	void Update(float dTime, bool& _endGame);
	void Render(float dTime, int& pScore, DirectX::SpriteBatch& batch);


private:

	const int ASTEROID_CACHE = 32;	//PMODE Exclusive

	MyD3D& mD3D;					//PMODE Exclusive
	DirectX::SpriteFont* mpFont = nullptr;//PMODE Exclusive
	std::vector<Asteroid> mAsteroids;//PMODE Exclusive
	float mSpawnRateSec = 0.5f;		//Asteroid spawn rate //PMODE Exclusive
	float mLastSpawn = 0;		// used in asteroid spawn timing //PMODE Exclusive
	/*int spawnCheck = 0;*/
	int asteroidID = 0;//PMODE Exclusive
	/*bool shipActive = true;*/
	int additionalScore = 0;							//PMODE Exclusive

	Bullet mMissile;	//weapon, only one at once		//PMODE Exclusive
	Background bGround;	//paralax scrolling background	//PMODE Exclusive
	Player Player;
	WallPad wallPad;

	//make it move, reset it once it leaves the screen, only one at once
	void UpdateMissile(float dTime);


	//move the ship by keyboard, gamepad or mouse

	//Asteroid Function. TW
	//A specific set of functions used to handle the 32 instances of asteroids used in Playmode. MH
	Asteroid* SpawnAsteroid();
	Asteroid* CheckAsteroidColl(Asteroid& asteroid);
	void InitAsteroids();
	void UpdateAsteroids(float dTime);
	void RenderAsteroids(DirectX::SpriteBatch& batch);
};


#endif
