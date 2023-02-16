#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "GameObject.h"

class MainMenu
{
public:
    MainMenu(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, float width, float height);
    ~MainMenu();

    void Update();
    void Render();

private:
    ID3D12Device* m_device;
    ID3D12GraphicsCommandList* m_commandList;
    float m_width;
    float m_height;

    std::vector<std::shared_ptr<GameObject>> m_gameObjects;
    std::shared_ptr<GameObject> m_background;
    std::shared_ptr<GameObject> m_title;
    std::shared_ptr<GameObject> m_startButton;

  /*  std::unique_ptr<DirectX::SoundEffect> m_backgroundMusic;
    std::unique_ptr<DirectX::SoundEffectInstance> m_backgroundMusicInstance;*/
};

