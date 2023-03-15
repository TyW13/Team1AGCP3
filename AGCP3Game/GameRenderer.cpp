//
// Game.cpp
//
#include "stdafx.h"

#include "pch.h"
#include "GameRenderer.h"
#include "GraphicsMemory.h"
#include "SpriteBatch.h"
#include "ResourceUploadBatch.h"
#include "DDSTextureLoader.h"
#include "Framework.h"
#include "Input.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameRenderer::GameRenderer() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

GameRenderer::~GameRenderer()
{
    delete m_pFramework;

    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void GameRenderer::Initialize(HWND window, int width, int height)
{
    m_pFramework = new Framework;
    m_pFramework->Initialise();

    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    audio.Init();

    //game logic, will move later
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:

 /*   m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);*/

}

#pragma region Frame Update
// Executes the basic game loop.
void GameRenderer::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

// Updates the world.
void GameRenderer::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

    audio.Update(elapsedTime);

    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// This is our game logic all here!
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    m_pFramework->Update(elapsedTime);
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    
    //game logic, will move later
    
    float time = float(timer.GetTotalSeconds());

    //get keyboard stated
    auto kb = m_keyboard->GetState();
    auto mouse = m_mouse->GetState();

    //update player core movement
    m_screenPos += currentVel * elapsedTime;


    //--------- x-axis
    //right
    if (kb.D && !deactivate_D)
    {
        currentVel.x = PLAYER_SPEED;
    }
    //left
    else if (kb.A && !deactivate_A)
    {
        currentVel.x = -PLAYER_SPEED;
    }
    //deceleration
    //
    //if on the ground
    else if (grounded)
    {
        currentVel.x *= DRAG_X;
    }
    //if in the air
    else if (!grounded)
    {
        currentVel.x *= DRAG_X_IN_AIR;
    }

    if (grounded)
    {
        //set initial velocity, start timer, record button pressed down during only the first frame
        if (kb.Space && detectSpaceKey)
        {
            start_time = std::chrono::high_resolution_clock::now();
            currentVel.y = -MAX_JUMP_VEL;	//set initial velocity to max velocity

            detectSpaceKey = false;
            recordJumpTime = true;
            grounded = false;
        }


    }
    if (!grounded)
    {
        //record how much time has been passed since pressing down and releasing space button
        if (recordJumpTime)
        {
            end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_seconds = end_time - start_time;
            elapsed_time = elapsed_seconds.count();

            //if pressed for max time set to high jump straightaway
            if (elapsed_time >= HIGH_JUMP_TIME)
                elapsed_time = HIGH_JUMP_TIME;


        }

        //detect during which of two time frames the space button has been released and then set the jump type based on it 
        if (!kb.Space && !timeSpaceClickDetected || elapsed_time == HIGH_JUMP_TIME)
        {
            std::chrono::duration<double> elapsed_seconds = end_time - start_time;
            spaceClickElapsedTime = elapsed_seconds.count();

            //get the jump type
            if (spaceClickElapsedTime > LOW_JUMP_TIME)
            {
                //spaceClickElapsedTime = HIGH_JUMP_TIME;
                jumpType = "HighJump";

            }
            else
            {
                //spaceClickElapsedTime = LOW_JUMP_TIME;
                jumpType = "LowJump";

            }
            timeSpaceClickDetected = true;


        }

        //set the velocity to minimum jump vel if the space button was clicked during the former time frame 
        //OR 
        //set to maximum jump vel if was clicked during the latter time frame
        if ((!kb.Space && elapsed_time != 0 && elapsed_time <= HIGH_JUMP_TIME && !detectSpaceKey) || elapsed_time == HIGH_JUMP_TIME)
        {
            if (jumpType == "LowJump" && elapsed_time >= LOW_JUMP_TIME)
            {
                jumpType = "";
                elapsed_time = 0;
                currentVel.y = -MIN_JUMP_VEL;
                recordJumpTime = false;

            }
            else if (jumpType == "HighJump" && elapsed_time == HIGH_JUMP_TIME)
            {
                jumpType = "";
                elapsed_time = 0;
                currentVel.y = -MAX_JUMP_VEL;
                recordJumpTime = false;

            }
            else
            {
                recordJumpTime = false;
            }
        }

        //multiply the velocity by drag value to slown down the player moving upward
        if (currentVel.y < 0 && !recordJumpTime)
        {
            currentVel.y *= DRAG_Y;


        }

        //--wall sliding
        //
        //wall jump
        if (isWallSliding && kb.Space && !hasWallJumped)
        {
            //start_time_wall_jump = std::chrono::high_resolution_clock::now();
            currentVel.y = -WALL_JUMP_VEL_Y;
            currentVel.x = -3 * currentVel.x;
            elapsedtime = 0;
            hasWallJumped = true;
            isWallSliding = false;
        }
        if (hasWallJumped)
        {
            elapsedtime += elapsedTime;

            if (elapsedtime < 0.5)
            {

                currentVel *= 0.99;
                deactivate_A = true;
                deactivate_D = true;
            }
            else
            {
                hasWallJumped = false;
                deactivate_A = false;
                deactivate_D = false;
            }
        }
        else
        {
            if (isWallSliding && !kb.W)
            {
                currentVel.y = SLIDE_DOWN_VEL;
            }
        }

        //if the player has slowed down moving upward activate the gravity to take them back down
        if (currentVel.y >= -40 && !isWallSliding)
        {
            if (currentVel.y > GRAVITY) { currentVel.y = GRAVITY; }
            else { currentVel.y += 1.01 * (GRAVITY / 20); }
        }
    }

    //simple collisions
    //for code clarity
    
    //check for player and screen borders collision

    //get screen dimensions
    int width = GetSystemMetricsForDpi(SM_CXSCREEN, GetDpiForSystem());
    int height = GetSystemMetricsForDpi(SM_CYSCREEN, GetDpiForSystem());

    //bottom
    if (m_screenPos.y >= height - 2 * m_origin.y)
    {
        m_screenPos.y = height - 2 * m_origin.y;
    }
    //top
    if (m_screenPos.y < m_origin.y * 2)
    {
        m_screenPos.y = m_origin.y * 2;
    }
    //right
    if (m_screenPos.x + m_origin.x >= width)
    {
        if (kb.D && !deactivate_D)
        {
            isWallSliding = true;
        }
        else
        {
            isWallSliding = false;
        }
        m_screenPos.x = width - m_origin.x;
    }
    //left
    else if (m_screenPos.x - m_origin.x <= 0)
    {
        if (kb.A && !deactivate_A)
        {
            isWallSliding = true;
        }
        else
        {
            isWallSliding = false;
        }
        m_screenPos.x = m_origin.x;
    }
    else
    {
        isWallSliding = false;
    }

    //if the player is on the bottom line (let's say it's the ground for now)
    if (m_screenPos.y == height - 2 * m_origin.y)
    {
        detectSpaceKey = true;
        grounded = true;
    }

    PIXEndEvent();
}
#pragma endregion
/////
#pragma region Frame Render
// Draws the scene.
void GameRenderer::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();

    auto commandList = m_deviceResources->GetCommandList();

    Clear();

    // Add rendering code here.

    float time = float(m_timer.GetTotalSeconds());

   
    ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    m_spriteBatch->Begin(commandList);

    m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Background),
        GetTextureSize(m_background.Get()),
        m_fullscreenRect);

    m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
        GetTextureSize(m_texture.Get()),
        m_screenPos, nullptr, Colors::White, 0.f, m_origin);

    m_spriteBatch->End();

  

    // Show the new frame.
    m_deviceResources->Present();
    m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void GameRenderer::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport = m_deviceResources->GetScreenViewport();
    auto const scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void GameRenderer::OnActivated()
{
    // TODO: Game is becoming active window.
}

void GameRenderer::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void GameRenderer::OnSuspending()
{
    audio.OnSuspending();

    // TODO: Game is being power-suspended (or minimized).
}

void GameRenderer::OnResuming()
{
    m_timer.ResetElapsedTime();

    audio.OnResuming();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void GameRenderer::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void GameRenderer::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void GameRenderer::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void GameRenderer::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void GameRenderer::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();



    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);


//m_graphicsMemory:



    // Tiling a Sprite
    m_states = std::make_unique<CommonStates>(device);

    m_resourceDescriptors = std::make_unique<DescriptorHeap>(device,
        Descriptors::Count);

    ResourceUploadBatch resourceUpload(device);

    resourceUpload.Begin();


    // WIC

    //DX::ThrowIfFailed(
    //    CreateWICTextureFromFile(device, resourceUpload, L"cat.png",
    //        m_texture.ReleaseAndGetAddressOf()));

    // DDS

    // CAT

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, resourceUpload, L"Data/cat.dds",
            m_texture.ReleaseAndGetAddressOf()));


    CreateShaderResourceView(device, m_texture.Get(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Cat));

    DX::ThrowIfFailed(
        CreateWICTextureFromFile(device, resourceUpload, L"Data/sunset.jpg",
            m_background.ReleaseAndGetAddressOf()));

    // BACKGROUND

    CreateShaderResourceView(device, m_background.Get(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Background));

    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
        m_deviceResources->GetDepthBufferFormat());

    //SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);
    //m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

    auto sampler = m_states->LinearWrap();
    SpriteBatchPipelineStateDescription pd(
        rtState, nullptr, nullptr, nullptr, &sampler);
    m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

    XMUINT2 catSize = GetTextureSize(m_texture.Get());

    m_origin.x = float(catSize.x / 2);
    m_origin.y = float(catSize.y / 2);

    //m_origin.x = float(catSize.x * 2);
    //m_origin.y = float(catSize.y * 2);

    m_tileRect.left = catSize.x * 2;
    m_tileRect.right = catSize.x * 6;
    m_tileRect.top = catSize.y * 2;
    m_tileRect.bottom = catSize.y * 6;

    auto uploadResourcesFinished = resourceUpload.End(
        m_deviceResources->GetCommandQueue());

    uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void GameRenderer::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    m_fullscreenRect = m_deviceResources->GetOutputSize();

    auto viewport = m_deviceResources->GetScreenViewport();
    m_spriteBatch->SetViewport(viewport);

    auto size = m_deviceResources->GetOutputSize();
    m_screenPos.x = float(size.right) / 2.f;
    m_screenPos.y = float(size.bottom) / 2.f;

    // Stretch Sprite
    m_stretchRect.left = size.right / 4;
    m_stretchRect.top = size.bottom / 4;
    m_stretchRect.right = m_stretchRect.left + size.right / 2;
    m_stretchRect.bottom = m_stretchRect.top + size.bottom / 2;
}

void GameRenderer::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory.reset();
    m_texture.Reset();
    m_resourceDescriptors.reset();
    m_spriteBatch.reset();
    m_states.reset();
    m_background.Reset();
}

void GameRenderer::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion