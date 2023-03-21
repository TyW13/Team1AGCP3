#include "Player.h"

void Player::Init(HWND window, int width, int height)
{
}

void Player::Update(float elapsedTime, DirectX::Keyboard::State kb, DirectX::Mouse::State mouse)
{
    //update player core movement
    m_screenPos += currentVel * elapsedTime;

    //--------- mouse
    if (GetAsyncKeyState(VK_LBUTTON) && detectMouseClick)
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        mousePos.x = cursorPos.x;
        mousePos.y = cursorPos.y;

        //playerToMouseDist = Distance(character.mPos.x, mousePos.x, character.mPos.y, mousePos.y);

        //calculate the direction vector from the player to the mouse click
        direction = mousePos - m_screenPos;

        //reverse the direction vector
        direction *= -1;

        //normalize the direction vector
        direction /= sqrt(pow(direction.x, 2) + pow(direction.y, 2));

        //apply a jump force to the player character
        currentVel = (direction * 1500);

        detectMouseClick = false;
    }
    if (!mouse.leftButton && !detectMouseClick)
    {
        detectMouseClick = true;
    }

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
}