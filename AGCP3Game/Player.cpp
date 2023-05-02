#define NOMINMAX

#include "Player.h"
#include "Input.h"
#include "ResourceManager.h"

DirectX::SimpleMath::Vector2 Player::mPos = { 0,0 };

void Player::Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, int _collisionDirection, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	isActive = _active;
	objSize = _objSize;
	objType = _objType;
    collisionDirection = _collisionDirection;
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

    playerAnim.Init("Player.json", *this);
    playerAnim.Init("Player.json", *this);
    audioManager.Init();
}
void Player::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
    //SmPos = DirectX::SimpleMath::Vector2{ mPos.x + 5, mPos.y };
    collisionBounds.left = mPos.x;
    collisionBounds.top = mPos.y;
    collisionBounds.right = mPos.x + objSize.x * mScale.x;
    collisionBounds.bottom = mPos.y + objSize.y * mScale.y;

    if (!collidedTop)                   // checks if player is on the ground
    {
        grounded = false;
    }

    /*if (canCollideRightWall)
    {
        canCollideLeftWall = false;
    }
    if (canCollideLeftWall)
    {
        canCollideRightWall = false;
    }*/

    // Check if the player is on the ground
    if (grounded)
    {
        //reset the coyote time remaining
        coyoteTimeRemaining = COYOTE_TIME_DURATION;
    }
    else
    {
        //decrease the coyote time remaining
        coyoteTimeRemaining -= dTime;
    }

	// Player Animations update goes in here
    playerAnim.Update(dTime, *this, animState);

	UpdateInput(dManager, dTime);

    CheckCollision(dManager, rManager, dTime);
    playerAnim.Update(dTime, *this, animState);
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
    animState = 0;
    //get keyboard stated
    kb = dManager->GetKeyboard()->GetState();
    mouse = dManager->GetMouse()->GetState();
    //update player core movement
    mPos += currentVel * slowdown_modifier * dTime;        //slowdown modifier by default should be 1

    //--------- mouse
    if (GetAsyncKeyState(VK_LBUTTON) && detectMouseClick && !fired)
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        mousePos.x = cursorPos.x;
        mousePos.y = cursorPos.y;

        //calculate the direction vector from the player to the mouse click
        direction = mousePos - mPos;

        //reverse the direction vector
        direction *= -1;

        //normalize the direction vector
        direction /= sqrt(pow(direction.x, 2) + pow(direction.y, 2));

        //apply a jump force to the player character
        currentVel += (direction * 2500);

        //audioManager.m_shotgun->Play();

        fired = true;
        grounded = false;
        detectMouseClick = false;
        canShotGunJump = false;
        canReloadGemJump = false;
        slowdown_modifier = 1;

        canShotGunJump = false;
        canReloadGemJump = false;
        slowdown_modifier = 1;
        audioManager.PlayShotgun();
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
        animState = 1;
    
        if (grounded)
        {
            audioManager.Playfootstep(0.8);
        }
    }
    //left
    else if (kb.A && !deactivate_A)
    {
        currentVel.x = -PLAYER_SPEED;
        animState = 2;
        if (grounded)
        {
            audioManager.Playfootstep(0.8);
        }
    }
    else
    {
        audioManager.Stopfootstep();
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

    if (grounded || coyoteTimeRemaining >= 0.0f && recordLastCollision == 1 && currentVel.x != 0)
    if (grounded)
    {
        jumpSound = true;
    }

    if (grounded || coyoteTimeRemaining >= 0.0f && recordLastCollision == 1 && currentVel.x != 0)
    {
        //set initial velocity, start timer, record button pressed down during only the first frame
        if (kb.Space)
        {
            detectSpaceKey = true;
        }

        if (detectSpaceKey)
        {
            animState = 3;
            start_time = std::chrono::high_resolution_clock::now();
            currentVel.y = -MAX_JUMP_VEL;	//set initial velocity to max velocity
            detectSpaceKey = false;
            recordJumpTime = true;
            grounded = false;
        }

        if (jumpSound && kb.Space)
        {
            audioManager.PlayJump();
            jumpSound = false;
        }

    }
    else
    {
        jumpSound = false;
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

        //wall jump
        if (isWallSliding && kb.Space && !hasWallJumped)
        {
            //start_time_wall_jump = std::chrono::high_resolution_clock::now();
            currentVel.y = -WALL_JUMP_VEL_Y;
            (currentVel.x > 0) ? currentVel.x = -WALL_JUMP_VEL_X : (currentVel.x < 0) ? currentVel.x = WALL_JUMP_VEL_X : 0 ;
            elapsedtime = 0;
            hasWallJumped = true;
            isWallSliding = false;
        }
        if (hasWallJumped)
        {
            elapsedtime += dTime;

            if (elapsedtime < 0.5)
            {

                currentVel.x *= 0.95;
                currentVel.y *= 0.99;
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
        //wall slide
        else
        {
            if (isWallSliding && !kb.W)
            {
                currentVel.y = SLIDE_DOWN_VEL;
            }
        }
        //bool addGravity = false;

        //DBOUT(std::to_string(currentVel.y) + "\n");
        //if the player has slowed down moving upward activate the gravity to take them back down
        if (currentVel.y >= -40 && !isWallSliding)
        {
            //addGravity = true;
            if (currentVel.y > GRAVITY) { currentVel.y = GRAVITY; }
            else { currentVel.y += 1.02 * (GRAVITY / 10); }
        }

        //addGravity = true;
        //if (currentVel.y > GRAVITY) { currentVel.y = GRAVITY; }
        //else { currentVel.y += 1.02 * (GRAVITY / 10); }
        //DBOUT(addGravity);
    }
}

void Player::CheckCollision(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
    //get keyboard stated
    kb = dManager->GetKeyboard()->GetState();
    mouse = dManager->GetMouse()->GetState();

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    //float maxVel = 30.f;
    //if (currentVel.x * dTime >= maxVel)													// Making sure speed doesnt exceed max speed
    //{
    //    currentVel.x = maxVel / dTime;
    //}
    //else if (currentVel.x * dTime <= -maxVel)
    //{
    //    currentVel.x = -maxVel / dTime;
    //}
    //if (currentVel.y * dTime >= maxVel)
    //{
    //    currentVel.y = maxVel / dTime;
    //}
    //else if (currentVel.y * dTime <= -maxVel)
    //{
    //    currentVel.y = -maxVel / dTime;
    //}

    mPos.x = std::ceil(mPos.x);
    mPos.y = std::ceil(mPos.y);

    DirectX::SimpleMath::Vector2 nextPos = mPos + currentVel * dTime;															// Player future position after forces applied


    RECT nextPosRect = RECT{
        static_cast<long> (nextPos.x),
        static_cast<long> (nextPos.y),
        static_cast<long> (nextPos.x + objSize.x * mScale.x),
        static_cast<long> (nextPos.y + objSize.y * abs(mScale.y)) };

    auto tempCollisionBoundsLeft = collisionBounds.left;
    auto tempCollisionBoundsRight = collisionBounds.right;

    if (mScale.x <= 0)
    {
        collisionBounds.left = tempCollisionBoundsRight;
        collisionBounds.right = tempCollisionBoundsLeft;
    }

    for (int i = 0; i < rManager->GetObjects().size(); ++i)
    {
        if (rManager->GetObjects()[i]->GetObjectType() != "Player")
        {
			// Keeping for now in merge, dont forget to compare
            if (animState == 2)
            {
                LONG TempStorage = collisionBounds.left;
                LONG TempStorage2 = collisionBounds.right;
                collisionBounds.left = TempStorage2;
                collisionBounds.right = TempStorage;
                TempStorage = nextPosRect.left;
                TempStorage2 = nextPosRect.right;
                nextPosRect.right = TempStorage;
                nextPosRect.left = TempStorage2;
            }
			///////////////////////////////////////////////////////
			
            if (nextPosRect.left <= rManager->GetObjects()[i]->GetCollisionBounds().right &&
                nextPosRect.right >= rManager->GetObjects()[i]->GetCollisionBounds().left &&
                nextPosRect.top <= rManager->GetObjects()[i]->GetCollisionBounds().bottom &&
                nextPosRect.bottom >= rManager->GetObjects()[i]->GetCollisionBounds().top)
            {
                float collisionPosOffset = 1;               // Value to offset player by when they collide with an object

                if (rManager->GetObjects()[i]->GetObjectType() == "Tile")
                {
                    int yDiff = 0;
                    int xDiff = 0;

                    switch (rManager->GetObjects()[i]->GetCollisionDirection())
                    {
                    case(1):                                                                                        // Top
                        //(!isWallSliding) ? currentVel.y = 0 : 0;
                        collidedTop = true;
                        grounded = true;
                        canShotGunJump = true;
                        fired = false;
                        recordLastCollision = 1;

                        if (currentVel.y > 0)
                        {
                            currentVel.y = 0;
                        }

                        break;

                    case(2):                                                                                        // Top Right
                        yDiff = abs(nextPosRect.bottom - rManager->GetObjects()[i]->GetCollisionBounds().top);
                        xDiff = abs(nextPosRect.left - rManager->GetObjects()[i]->GetCollisionBounds().right);

                        if (currentVel.y > 0 && yDiff <= xDiff)                                                               // Colliding from top
                        {
                            currentVel.y = 0;

                            collidedTop = true;
                            grounded = true;
                            canShotGunJump = true;
                            fired = false;
                        }

                        if (currentVel.x < 0 && xDiff < yDiff)
                        {
                            currentVel.x = 0;

                            collidedRight = true;
                        }
                        break;
                    case(3):                                                                                        // Right
                        //canCollideLeftWall = true;
                        //recordLastCollision = 4;

                        //if (kb.A && !deactivate_A)
                        //{
                        //    isWallSliding = true;
                        //}
                        //else
                        //{
                        //    isWallSliding = false;
                        //}
                        //DBOUT("collidedLeft");

                        if (currentVel.x < 0)
                        {
                            currentVel.x = 0;

                            collidedRight = true;
                        }

                        break;
                    case(4):
                        yDiff = abs(nextPosRect.top - rManager->GetObjects()[i]->GetCollisionBounds().bottom);
                        xDiff = abs(nextPosRect.left - rManager->GetObjects()[i]->GetCollisionBounds().right);

                        if (currentVel.x < 0 && xDiff <= yDiff)
                        {
                            currentVel.x = 0;

                            collidedRight = true;
                        }
                        if (currentVel.y < 0 && yDiff < xDiff)
                        {
                            currentVel.y = 0;

                            collidedBottom = true;
                        }

                        break;
                    case(5):
                        //(!isWallSliding) ? currentVel.y = 0 : 0;


                        //DBOUT("collided bottom");

                        if (currentVel.y < 0)
                        {
                            currentVel.y = 0;

                            collidedBottom = true;
                            recordLastCollision = 2;
                        }

                        break;
                    case(6):
                        yDiff = abs(nextPosRect.top - rManager->GetObjects()[i]->GetCollisionBounds().bottom);
                        xDiff = abs(nextPosRect.right - rManager->GetObjects()[i]->GetCollisionBounds().left);

                        if (currentVel.y < 0 && yDiff < xDiff)
                        {
                            currentVel.y = 0;

                            collidedBottom = true;
                        }
                        if (currentVel.x > 0 && xDiff <= yDiff)
                        {
                            currentVel.x = 0;

                            collidedLeft = true;
                        }

                        break;
                    case(7):
                        if (currentVel.x > 0)
                        {
                            currentVel.x = 0;

                            collidedLeft = true;
                        }
						
                        break;
						
                    case(8):
                        yDiff = abs(nextPosRect.bottom - rManager->GetObjects()[i]->GetCollisionBounds().top);
                        xDiff = abs(nextPosRect.right - rManager->GetObjects()[i]->GetCollisionBounds().left);

                        if (currentVel.x > 0 && xDiff < yDiff)
                        {
                            currentVel.x = 0;

                            collidedLeft = true;
                        }
                        if (currentVel.y > 0 && yDiff <= xDiff)
                        {
                            currentVel.y = 0;

                            collidedTop = true;
                            grounded = true;
                            canShotGunJump = true;
                            fired = false;
                        }

                        break;
                    }
                }

				if (rManager->GetObjects()[i]->GetObjectType() == "Damageable")
				{
					rManager->ReloadMap(dManager, rManager->GetCurrentMapNum());
				}
				
                if (rManager->GetObjects()[i]->GetObjectType() == "BouncePad")
                {
                    //if player collided from their bottom bound
                    if (collisionBounds.bottom < rManager->GetObjects()[i]->GetCollisionBounds().top && nextPosRect.bottom >= rManager->GetObjects()[i]->GetCollisionBounds().top && !collidedTop)
                    {
                        currentVel.y = -BOUNCE_PAD_JUMP_Y;

                        fired = false;
                    }
                    //if player collided from their top bound
                    else if (collisionBounds.top > rManager->GetObjects()[i]->GetCollisionBounds().bottom && nextPosRect.top <= rManager->GetObjects()[i]->GetCollisionBounds().bottom && !collidedBottom)
                    {
                        currentVel.y = BOUNCE_PAD_JUMP_Y;

                        fired = false;
                    }
                    //if player collided from their right bound
                    else if (collisionBounds.right < rManager->GetObjects()[i]->GetCollisionBounds().left && nextPosRect.right >= rManager->GetObjects()[i]->GetCollisionBounds().left && !collidedLeft)
                    {
                        currentVel.y = -BOUNCE_PAD_JUMP_X;

                        fired = false;
                    }
                    //if player collided from their left bound
                    else if (collisionBounds.left > rManager->GetObjects()[i]->GetCollisionBounds().right && nextPosRect.left <= rManager->GetObjects()[i]->GetCollisionBounds().right && !collidedRight)
                    {
                        currentVel.y = BOUNCE_PAD_JUMP_X;

                        fired = false;
                    }
                }


				// For simpler version, only use first 3 lines
				 if (rManager->GetObjects()[i]->GetObjectType() == "ReloadGem" && rManager->GetObjects()[i]->GetActive())
				{
					rManager->GetObjects()[i]->SetActive(false);
					SetVelocity({ 0,0 });
					fired = false;
					////////////////////////////////////////////////////////////////////

					canReloadGemJump = true;
					slowdown_modifier = 0.1;
				}
				else if (canReloadGemJump)
				{
					gemSlowdownRemaining -= dTime;
					if (gemSlowdownRemaining <= 0)
					{
						canReloadGemJump = false;
						slowdown_modifier = 1;
					}
				}
				else if (!canReloadGemJump)
				{
					//reset the slowdown time remaining
					gemSlowdownRemaining = GEM_SLOWDOWN_DURATION;
				}

				if (rManager->GetObjects()[i]->GetObjectType() == "EndZone" || kb.Q)
				{
					rManager->LoadNextZone(dManager);
				}
            }
        }
    }
	
	collisionBounds.left = tempCollisionBoundsLeft;
    collisionBounds.right = tempCollisionBoundsRight;
}

void Player::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void Player::SetObjectSize(DirectX::SimpleMath::Vector2 _objSize)
{
	objSize = _objSize;
}

void Player::SetCollisionDirection(int _direction)
{
    collisionDirection = _direction;
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