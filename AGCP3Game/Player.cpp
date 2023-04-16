#include "Player.h"
#include "Input.h"
#include "ResourceManager.h"


void Player::Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	isActive = _active;
	objSize = _objSize;
	objType = _objType;
	isCollidable = _isCollidable;
	objRect = _objRect;
	mPos = _position;
	mScale = _scale;

	dManager->GetResourceUpload()->Begin();																					// Start of texture loading

	DX::ThrowIfFailed(																										// Error check for creation of dds texture
		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), texPath.c_str(),
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),													// Creation of shader resource view?
		dManager->GetResourceDescriptors()->GetCpuHandle(0));

	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
		dManager->GetDeviceResources()->GetCommandQueue());
	uploadResourcesFinished.wait();

    PlayerAnim.Init("Player.json", *this);
}

void Player::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
    collisionBounds.left = mPos.x;
    collisionBounds.top = mPos.y;
    collisionBounds.right = mPos.x + objSize.x * abs(mScale.x);
    collisionBounds.bottom = mPos.y + objSize.y * abs(mScale.y);

    if (!collidedTop)                   // checks if player is on the ground
    {
        grounded = false;
    }

	UpdateInput(dManager, dTime);
    PlayerAnim.Update(dTime, *this, AnimState);    
    CheckCollision(dManager, rManager, dTime);
}

void Player::Render(DeviceManager* dManager)
{
	RECT* sourceRect = &objRect;

	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(0),
		DirectX::GetTextureSize(objTex.Get()),
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, mOrigin, mScale);
}

void Player::UpdateInput(DeviceManager* dManager, float dTime)
{
    //get keyboard stated
    auto kb = dManager->GetKeyboard()->GetState();
    auto mouse = dManager->GetMouse()->GetState();

    //update player core movement
    mPos += currentVel * dTime;

    //--------- mouse
    if (GetAsyncKeyState(VK_LBUTTON) && detectMouseClick && !fired)
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        mousePos.x = cursorPos.x;
        mousePos.y = cursorPos.y;

        //playerToMouseDist = Distance(character.mPos.x, mousePos.x, character.mPos.y, mousePos.y);

        //calculate the direction vector from the player to the mouse click
        direction = mousePos - mPos;

        //reverse the direction vector
        direction *= -1;

        //normalize the direction vector
        direction /= sqrt(pow(direction.x, 2) + pow(direction.y, 2));

        //apply a jump force to the player character
        currentVel = (direction * 1500);

        fired = true;
        grounded = false;
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
        AnimState = 1
    }
    //left
    else if (kb.A && !deactivate_A)
    {
        currentVel.x = -PLAYER_SPEED;
        AnimState = 2;
    }
    //deceleration
    //
    //if on the ground
    if (grounded)
    {
        currentVel.x *= DRAG_X;
    }
    //if in the air
    else if (!grounded)
    {
        currentVel.x *= DRAG_X_IN_AIR;
    }

    //--------- y-axis

    if (grounded)
    {
        //set initial velocity, start timer, record button pressed down during only the first frame
        if (kb.Space)
        {
            detectSpaceKey = true;
            AnimState = 3;
        }

        if (detectSpaceKey)
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
            elapsedtime += dTime;

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

    //get screen dimensions
    int width = GetSystemMetricsForDpi(SM_CXSCREEN, GetDpiForSystem());
    int height = GetSystemMetricsForDpi(SM_CYSCREEN, GetDpiForSystem());

    //bottom
    if (mPos.y >= height - 2 * mOrigin.y)
    {
        mPos.y = height - 2 * mOrigin.y;
    }
    //top
    if (mPos.y < mOrigin.y * 2)
    {
        mPos.y = mOrigin.y * 2;
    }
    //right
    if (mPos.x + mOrigin.x >= width)
    {
        if (kb.D && !deactivate_D)
        {
            isWallSliding = true;
        }
        else
        {
            isWallSliding = false;
        }
        mPos.x = width - mOrigin.x;
    }
    //left
    else if (mPos.x - mOrigin.x <= 0)
    {
        if (kb.A && !deactivate_A)
        {
            isWallSliding = true;
        }
        else
        {
            isWallSliding = false;
        }
        mPos.x = mOrigin.x;
    }
    else
    {
        isWallSliding = false;
    }

    //if the player is on the bottom line (let's say it's the ground for now)
    if (mPos.y == height - 2 * mOrigin.y)
    {
        detectSpaceKey = true;
        grounded = true;
    }
}

void Player::CheckCollision( DeviceManager* dManager, ResourceManager* rManager, float dTime)
{			
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    float maxVel = 30.f;
    if (currentVel.x * dTime >= maxVel)													// Making sure speed doesnt exceed max speed
    {
        currentVel.x = maxVel / dTime;
    }
    else if (currentVel.x * dTime <= -maxVel)
    {
        currentVel.x = -maxVel / dTime;
    }
    if (currentVel.y * dTime >= maxVel)
    {
        currentVel.y = maxVel / dTime;
    }
    else if (currentVel.y * dTime <= -maxVel)
    {
        currentVel.y = -maxVel / dTime;
    }

    DirectX::SimpleMath::Vector2 nextPos = mPos + currentVel * dTime;															// Player future position after forces applied
    

    RECT nextPosRect = RECT{                                                                                
        static_cast<long> (nextPos.x),
        static_cast<long> (nextPos.y),
        static_cast<long> (nextPos.x + objSize.x * abs(mScale.x)),
        static_cast<long> (nextPos.y + objSize.y * abs(mScale.y)) };

    bool collided = false;                                                                             
    std::vector<GameObject*> gObjects;                                                                 

    for (GameObject* gObject : rManager->GetObjects())
    {
        if (nextPosRect.left <= gObject->GetCollisionBounds().right &&
            nextPosRect.right >= gObject->GetCollisionBounds().left &&
            nextPosRect.top <= gObject->GetCollisionBounds().bottom &&
            nextPosRect.bottom >= gObject->GetCollisionBounds().top)
        {
            gObjects.emplace_back(gObject);
        }
    }

    float collisionPosOffset = 1;               // Value to offset player by when they collide with an object
    for (GameObject* obj : gObjects)
    {
        if (obj->GetObjectType() == "Tile")
        {
            //DBOUT(objType);
            if (collisionBounds.bottom < obj->GetCollisionBounds().top && nextPosRect.bottom >= obj->GetCollisionBounds().top && !collidedTop)			    // Collided from top, moving down
            {
                mPos.y = obj->GetCollisionBounds().top - (objSize.y * abs(mScale.y) + collisionPosOffset);							                        // Setting position to just outside obj
                mPos.x += currentVel.x * dTime;																										        // Only adding velocity on non colliding axis
                currentVel.y = 0;

                collided = true;
                collidedTop = true;
                grounded = true;
                fired = false;
            }
            else if (collisionBounds.top > obj->GetCollisionBounds().bottom && nextPosRect.top <= obj->GetCollisionBounds().bottom && !collidedBottom)	    // Collided from bottom, moving up
            {
                mPos.y = obj->GetCollisionBounds().bottom + collisionPosOffset;																		        // Setting position to just outside obj
                mPos.x += currentVel.x * dTime;																										        // Only adding velocity on non colliding axis
                currentVel.y = 0;

                collided = true;
                collidedBottom = true;
            }
            if (collisionBounds.right < obj->GetCollisionBounds().left && nextPosRect.right >= obj->GetCollisionBounds().left && !collidedLeft)			    // Collided from left, moving right
            {
                mPos.x = obj->GetCollisionBounds().left - (objSize.x * abs(mScale.x) + collisionPosOffset);							                        // Setting position to just outside obj
                mPos.y += currentVel.y * dTime;																										        // Only adding velocity on non colliding axis
                currentVel.x = 0;

                collided = true;
                collidedLeft = true;
            }
            else if (collisionBounds.left > obj->GetCollisionBounds().right && nextPosRect.left <= obj->GetCollisionBounds().right && !collidedRight)		// Collided from right, moving left
            {
                mPos.x = obj->GetCollisionBounds().right + collisionPosOffset;																		        // Setting position to just outside tile
                mPos.y += currentVel.y * dTime;																										        // Only adding velocity on non colliding axis
                currentVel.x = 0;

                collided = true;
                collidedRight = true;
            }
        }

        else if (obj->GetObjectType() == "Damageable")
        {
            rManager->ReloadMap(dManager, rManager->GetCurrentMapNum());               // needs device manager in params
        }
    }
}

void Player::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void Player::SetObjectSize(DirectX::SimpleMath::Vector2 _objSize)
{
	objSize = _objSize;
}

void Player::SetIsCollidable(bool _isCollidable)
{
	isCollidable = _isCollidable;
}

void Player::SetRect(RECT _objRect)
{
	objRect = _objRect;
}

void Player::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	mPos = _position;
}

void Player::SetScale(DirectX::SimpleMath::Vector2 _scale)
{
	mScale = _scale;
}