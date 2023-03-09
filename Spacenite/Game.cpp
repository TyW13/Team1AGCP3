#include "Game.h"
#include "WindowUtils.h"
#include "D3Dutil.h"
#include "CommonStates.h"

//#include "..\..\sqlite\sqlite3.h"

#include <thread>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <fstream>


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


Game::Game(MyD3D& d3d)
	: mPMode(d3d), mD3D(d3d), mpSB(nullptr), introSpr(d3d), endSpr(d3d)
{
	sMKIn.Initialise(WinUtil::Get().GetMainWnd(), true, false);
	sGamepads.Initialise();
	InitIntro(d3d);
	InitEnd(d3d);
	mpSB = new DirectX::DX11::SpriteBatch(&mD3D.GetDeviceCtx());
	mpSF = new SpriteFont(&d3d.GetDevice(), L"data/fonts/comicSansMS.spritefont");
}
//any memory or resources we made need releasing at the end
void Game::Release()
{
	delete mpSB;
	mpSB = nullptr;
	delete mpSF;
	mpSF = nullptr;
	mPMode.Release();
}
//called over and over, use it to update game logic
void Game::Update(MyD3D& d3d, float dTime)
{
	sGamepads.Update();
	switch (state)
	{
		case State::INTRO:
		{
			UpdateIntro();
			break;
		}
		case State::PLAY:
		{
			GameTimer += dTime;
			if (GameTimer >= 1)
			{
				++Seconds;
				GameTimer = 0;
			}
			if (Seconds >= 60)
			{
				++Minutes;
				Seconds = 0;
			}
			mPMode.Update(d3d, dTime, endGame, pScore);
			if (endGame == true)
			{
				state = State::GETNAME;
			}
			break;
		}
		case State::GETNAME:
		{
			wstring currentChar;
			sMKIn.GetPressedKeyNames(currentChar);
			int charInInteger = static_cast<int>(currentChar.c_str()[0]);
	
			if (GetKeyState(charInInteger) && currentChar.length() == 2)
			{
				if (!doneOnce)
				{
					pName += currentChar;
					doneOnce = true;
				}
			}
			else
			{
				doneOnce = false;
			}

			if (sMKIn.IsPressed(VK_RETURN) || pName.size() > 15) //////////////////////////////might need changing (size)
			{
				state = State::GETSCORES;
			}
			break;
		}
		case State::GETSCORES:
		{
			char encryptKey = 'E';

			// Adding the new score to the player score file
			playerScoresFile.open("data/HighScores.txt", ios::in | fstream::app); // Opening text file so it can be used
			assert(playerScoresFile.is_open());
			playerScoresFile << EncryptName(pName, encryptKey) << ",";
			playerScoresFile << EncryptScore(pScore, encryptKey);
			playerScoresFile << endl;
			playerScoresFile.close();

			// Reading the players scores from the txt file
			ReadScoreFile(encryptKey);

			// If the size of the vector is higher than 15, then erase the last element
			if (highScores.size() > 10)
			{
				highScores.pop_back();
			}

			// Writing sorted scores back to the txt file
			WriteScoreFile(encryptKey);

			// Putting 5 elements from the reordered list into a string to display at the end
			CreateEndScreenString();

			state = State::END;
			break;
		}
		case State::END:
		{
			UpdateEnd();
			break;
		}
	}
}
//called over and over, use it to render things
void Game::Render(float dTime)
{
	mD3D.BeginRender(Colours::Black);


	CommonStates dxstate(&mD3D.GetDevice());
	mpSB->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &mD3D.GetWrapSampler());

	switch (state)
	{
	case State::INTRO:
		RenderIntro(*mpSB);
		break;
	case State::PLAY:
		mPMode.Render(dTime, pScore, *mpSB);
		break;
	case State::GETNAME:
		RenderName(*mpSB);
		break;
	case State::END:
		RenderEnd(*mpSB);
		break;
	}

	mpSB->End();


	mD3D.EndRender();
	sMKIn.PostProcess();
}

void Game::InitIntro(MyD3D& d3d)
{
	ID3D11ShaderResourceView* ppp = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "BasicIntroScreen.dds", "IntroScreen", true);
	introSpr.SetTex(*ppp);
}
void Game::UpdateIntro()
{
	ResetClock();
	if (sMKIn.IsPressed(VK_SPACE))
	{
		this_thread::sleep_for(chrono::milliseconds(80)); // delay to stop missile from spawning
		state = State::PLAY;
	}
}
void Game::RenderIntro(DirectX::DX11::SpriteBatch& batch)
{
	introSpr.Draw(batch);
}

void Game::RenderName(DirectX::DX11::SpriteBatch& batch)
{
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);

	stringstream playersScoreSS;
	playersScoreSS << "Score: " << pScore;
	mpSF->DrawString(&batch, playersScoreSS.str().c_str(), Vector2(w * 0.4f, h * 0.1f), Vector4(1, 1, 1, 1));

	stringstream nameSS;
	nameSS << pName.c_str();
	mpSF->DrawString(&batch, "Enter your name: ", Vector2(w * 0.16f, h * 0.25f), Vector4(1, 1, 1, 1));
	mpSF->DrawString(&batch, pName.c_str(), Vector2(w * 0.5f, h * 0.25f), Vector4(1, 1, 1, 1)); 
}

void Game::InitEnd(MyD3D& d3d)
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "EndScreen.dds");
	endSpr.SetTex(*p);
}
void Game::UpdateEnd()
{
	ResetClock();
}
void Game::RenderEnd(DirectX::DX11::SpriteBatch& batch)
{
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	endSpr.Draw(batch);
	mpSF->DrawString(&batch, scoresSS.str().c_str(), Vector2(w * 0.3f, h * 0.4f), Vector4(1, 1, 1, 1));
}

// encrypting each individual character of the current players inputted name and inserting it into a new vector/string stream to be returned
std::string Game::EncryptName(std::wstring playerName, char encryptKey)
{
	stringstream encryptedNameSS;
	std::string nameString;
	for (int i = 0; i < playerName.size(); i+=2)
	{
		nameString.insert(nameString.end(), playerName[i]);
	}
	
	for (int i = 0; i < nameString.size(); i++)
	{
		char encryptedChar;
		encryptedChar = nameString[i] + encryptKey;
		encryptedNameSS << encryptedChar;
	}
	
	return encryptedNameSS.str();
}

// encrypting each individual character of the current players score and inserting it into a new vector/stringstream to be returned
std::string Game::EncryptScore(int playerScore, char encryptKey)
{
	stringstream encryptedScoreSS;
	stringstream scoreString;
	scoreString << playerScore;
	for (int i = 0; i < scoreString.str().size(); i++)
	{
		char encryptedChar;
		encryptedChar = scoreString.str()[i] ^ encryptKey;
		encryptedScoreSS << encryptedChar;
	}

	return encryptedScoreSS.str();
}

// getting both the names and scores of each player and decrypting them to be put in a vector
void Game::ReadScoreFile(char encryptKey)
{
	// Updating highscore and topPlayers vectors with data from text file
	playerScoresFile.open("data/HighScores.txt");
	std::string line;

	while (getline(playerScoresFile, line))
	{
		// Getting names and scores from txt file
		std::string currentName;
		stringstream decryptedNameSS;

		int currentScore = 0;
		stringstream decryptedScoreSS;

		bool commaReached = false;
		for (char character : line)
		{
			if (commaReached == false)
			{
				if (character != ',')
				{
					char decryptedChar;
					decryptedChar = character - encryptKey;
					decryptedNameSS << decryptedChar;
				}
				else
				{
					commaReached = true;
				}
			}

			else
			{
				char decryptedChar;
				decryptedChar = character ^ encryptKey;
				decryptedScoreSS << decryptedChar;
			}
		}

		decryptedNameSS >> currentName;
		topPlayers.insert(topPlayers.end(), 1, currentName);

		decryptedScoreSS >> currentScore;
		highScores.insert(highScores.end(), 1, currentScore);
	}
	playerScoresFile.close();
}

// sorting the names/scores and encrypting them to be put into the txt file on the hdd
void Game::WriteScoreFile(char encryptKey)
{
	// Sorting the scores in ascending order
	newHighScores.resize(highScores.size());
	newTopPlayers.resize(topPlayers.size());

	for (int i = 0; i < topPlayers.size(); i++)
	{
		bool done = false;
		int j = 0;
		while (done == false)
		{
			if (highScores[i] > newHighScores[j])
			{
				newHighScores.insert(newHighScores.begin() + j, 1, highScores[i]);
				newTopPlayers.insert(newTopPlayers.begin() + j, 1, topPlayers[i]);
				done = true;
			}
			j++;
		}
	}

	// Removing extra "empty" elements in each vector
	newHighScores.erase(newHighScores.begin() + highScores.size(), newHighScores.end());
	newTopPlayers.erase(newTopPlayers.begin() + topPlayers.size(), newTopPlayers.end());

	//Putting encrypted and reordered list back into text file
	playerScoresFile.open("data/HighScores.txt", ios::trunc);
	for (int i = 0; i < newHighScores.size(); i++)
	{
		// Encrypt and store each char for each name in vector
		string encryptedNameChar;
		stringstream nameToEncryptSS;
		nameToEncryptSS << newTopPlayers[i];
		for (int j = 0; j < nameToEncryptSS.str().size(); j++)
		{
			encryptedNameChar.push_back(nameToEncryptSS.str()[j] + encryptKey);
			playerScoresFile << to_string(encryptedNameChar[j]);
		}

		// Encrypt and store each 'char' for each score in vector
		string encryptedScoreChar;
		stringstream scoreToEncryptSS;
		scoreToEncryptSS << newHighScores[i];
		for (int k = 0; k < scoreToEncryptSS.str().size(); k++)
		{
			encryptedScoreChar.push_back(scoreToEncryptSS.str()[k] ^ encryptKey);
			playerScoresFile << to_string(encryptedScoreChar[k]);
		}
	}
	playerScoresFile.close();
}

// putting the name and score vectors into a string to be displayed on the end screen
void Game::CreateEndScreenString()
{
	if (newHighScores.size() > 5)
	{
		for (int i = 0; i < 5; i++)
		{
			scoresSS << newTopPlayers[i] << " - " << newHighScores[i] << endl;
		}
	}
	else
	{
		for (int i = 0; i < newHighScores.size(); i++)
		{
			scoresSS << newTopPlayers[i] << " - " << newHighScores[i] << endl;
		}
	}
}