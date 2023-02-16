#include "stdafx.h"
//#include "MainMenu.h"
//#include <xaudio2.h>
//
//
//
#include "SoundCommon.h"
#include "Audio.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MainMenu::MainMenu(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, float width, float height)
    : m_device(device), m_commandList(commandList), m_width(width), m_height(height)
{
    // Load background image
    Microsoft::WRL::ComPtr<ID3D12Resource> backgroundResource;
    HRESULT hr = CreateWICTextureFromFile(device, L"background.png", &backgroundResource, m_background.GetAddressOf());
    if (FAILED(hr))
    {
        throw std::exception("Failed to load background texture.");
    }
    m_gameObjects.push_back(m_background);

    // Load title image
    Microsoft::WRL::ComPtr<ID3D12Resource> titleResource;
    hr = CreateWICTextureFromFile(device, L"title.png", &titleResource, m_title.GetAddressOf());
    if (FAILED(hr))
    {
        throw std::exception("Failed to load title texture.");
    }
    m_title->SetPosition(Vector2(m_width * 0.5f, m_height * 0.25f));
    m_gameObjects.push_back(m_title);

    // Load start button image
    Microsoft::WRL::ComPtr<ID3D12Resource> startButtonResource;
    hr = CreateWICTextureFromFile(device, L"start.png", &startButtonResource, m_startButton.GetAddressOf());
    if (FAILED(hr))
    {
        throw std::exception("Failed to load start button texture.");
    }
    m_startButton->SetPosition(Vector2(m_width * 0.5f, m_height * 0.5f));
    m_gameObjects.push_back(m_startButton);

    // Load background music
    m_backgroundMusic = std::make_unique<SoundEffect>(m_device, L"background_music.wav");
    m_backgroundMusicInstance = m_backgroundMusic->CreateInstance(SoundEffectInstance_Use3D);
    m_backgroundMusicInstance->Play(true);
}
//
//MainMenu::~MainMenu()
//{
//}
//
//void MainMenu::Update()
//{
//    for (auto& gameObject : m_gameObjects)
//    {
//        gameObject->Update(0);
//    }
//}
//
//void MainMenu::Render()
//{
//    for (auto& gameObject : m_gameObjects)
//    {
//        gameObject->Render(m_commandList);
//    }
//}