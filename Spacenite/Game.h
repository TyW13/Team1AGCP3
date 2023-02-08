#pragma once

#include <vector>
#include <fstream>

#include "Input.h"
#include "D3D.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Sprite.h"
#include "Background.h"
#include "GameObject.h"
#include "PlayMode.h"
#include "UI.h"

//#include "..\..\sqlite\sqlite3.h"

/*
Basic wrapper for a game
*/
class Game
{
public:
	enum class State { INTRO, PLAY, GETNAME, GETSCORES, END };
	static MouseAndKeys sMKIn;
	static Gamepads sGamepads;
	State state = State::INTRO;
	Game(MyD3D& d3d);


	void Release();
	void Update(float dTime);
	void Render(float dTime);

	void InitIntro(MyD3D& d3d);
	void InitEnd(MyD3D& d3d);
	void UpdateIntro();
	void UpdateEnd();
	void RenderIntro(DirectX::SpriteBatch& batch);
	void RenderName(DirectX::SpriteBatch& batch);
	void RenderEnd(DirectX::SpriteBatch& batch);
	std::string EncryptName(std::wstring pName, char encryptKey);
	std::string EncryptScore(int playerScore, char encryptKey);
	void ReadScoreFile(char encryptKey);
	void WriteScoreFile(char encryptKey);
	void CreateEndScreenString();
private:
	MyD3D& mD3D;
	//sqlite3* db;
	const std::string scoresTable;
	DirectX::SpriteBatch *mpSB = nullptr;
	DirectX::SpriteFont *mpSF = nullptr;
	//not much of a game, but this is it
	PlayMode mPMode;
	Sprite introSpr;
	Sprite endSpr;
	std::fstream playerScoresFile;
	std::stringstream scoresSS;
	std::wstring pName;
	int pScore = 1000;
	std::vector<std::string> topPlayers;
	std::vector<std::string> newTopPlayers;
	std::vector<int> highScores;
	std::vector<int> newHighScores;
	bool endGame = false;
	bool doneOnce = false;
};


