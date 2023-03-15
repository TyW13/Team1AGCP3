//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SpriteBatch.h"
#include <SpriteFont.h>
#include "AudioManager.h"
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

class Framework;
class Input;
// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class GameRenderer final : public DX::IDeviceNotify
{
public:

    GameRenderer() noexcept(false);
    ~GameRenderer();

    GameRenderer(GameRenderer&&) = default;
    GameRenderer& operator= (GameRenderer&&) = default;

    GameRenderer(GameRenderer const&) = delete;
    GameRenderer& operator= (GameRenderer const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

private:
    Framework* m_pFramework;

    DirectX::SpriteBatch* mpSB = nullptr;
    DirectX::SpriteFont* mpSF = nullptr;

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    using VertexType = DirectX::VertexPositionColor;

    std::unique_ptr<DirectX::BasicEffect> m_effect;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    RECT m_tileRect;
    std::unique_ptr<DirectX::CommonStates> m_states;

    RECT m_stretchRect;

    RECT m_fullscreenRect;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_background;

    AudioManager audio;

    //game logic, will move later
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;

    //------ movement variables
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_wall_jump;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time_wall_jump;

    DirectX::SimpleMath::Vector2 currentVel = currentVel.Zero;
    DirectX::SimpleMath::Vector2 mousePos = mousePos.Zero;
    DirectX::SimpleMath::Vector2 direction = direction.Zero;

    const float MAX_JUMP_VEL = 400;
    const float MIN_JUMP_VEL = MAX_JUMP_VEL / 2;
    const float WALL_JUMP_VEL_Y = 1500;
    const float CLIMB_VEL = 170;					    //player climbing velocity
    const float SLIDE_DOWN_VEL = 80;					//sliding down velocity
    const float GRAVITY = 500;
    const float PLAYER_SPEED = 400;
    const float DRAG_X = 0.82;				            //for deceleration in x-axis on the ground
    const float DRAG_X_IN_AIR = 0.88;			    	//for deceleration in x-axis in air
    const float	DRAG_Y = 0.92;				            //for deceleration in y-axis
    const float HIGH_JUMP_TIME = 0.20;					//how much time it takes to do a higher jump
    const float LOW_JUMP_TIME = HIGH_JUMP_TIME / 2;	    //how much time it takes to do a lower jump

    double elapsed_time = 0;					        //measure how much time has elapsed between starting and ending time counting
    double elapsed_t_bouncepad = 0;

    std::string jumpType;

    bool grounded = false;				                //
    bool timeSpaceClickDetected = false;				//if space button has been released stop measuring time for picking either high or low jump
    bool recordJumpTime = false;				        //start/stop recording jump time
    bool detectSpaceKey = true;				        	//start detecting the space button pressed down
    bool detectMouseClick = true;					    //

    //------ simple "collisions"
    double elapsedtime = 0;								//for deactivating A and D buttons after the player has wall jumped
    bool isWallSliding = false;							//true whenever the player is wall sliding
    bool deactivate_A = false;							//deactivate A key input while wall jumping
    bool deactivate_D = false;							//deactivate D key input while wall jumping
    bool hasWallJumped = false;							//detect if wall jumped

    float spaceClickElapsedTime = 0.f;

    enum Descriptors
    {
        Cat,
        Background,
        Count
    };
};

class Player
{
public:

private:

};