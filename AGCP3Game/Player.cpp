#define NOMINMAX

#include "Player.h"
#include "Input.h"
#include "ResourceManager.h"

DirectX::SimpleMath::Vector2 Player::mPos = { 0,0 };

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

    //playerAnim.Init("Player.json", *this);
}

void Player::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
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
    //playerAnim.Update(dTime, *this, animState);

	UpdateInput(dManager, dTime);

    CheckCollision(dManager, rManager, dTime);
    //NewCheckCollision(dManager, rManager, dTime);

    DBOUT("x: " + std::to_string(mPos.x) + " y: " + std::to_string(mPos.y));
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
    kb = dManager->GetKeyboard()->GetState();
    mouse = dManager->GetMouse()->GetState();
    animState = 0;

    //update player core movement
    mPos += currentVel * slowdown_modifier * dTime;        //slowdown modifier by default should be 1

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
        currentVel += (direction * 2500);

        //audioManager.m_shotgun->Play();

        fired = true;
        grounded = false;
        detectMouseClick = false;
        canShotGunJump = false;
        canReloadGemJump = false;
        slowdown_modifier = 1;

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
    }
    //left
    else if (kb.A && !deactivate_A)
    {
        currentVel.x = -PLAYER_SPEED;
        animState = 2;
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
        bool addGravity = false;

        //if the player has slowed down moving upward activate the gravity to take them back down
        if (currentVel.y >= -40 && !isWallSliding)
        {
            addGravity = true;
            if (currentVel.y > GRAVITY) { currentVel.y = GRAVITY; }
            else { currentVel.y += 1.01 * (GRAVITY / 20); }
        }
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

    float collisionPosOffset = 1;               // Value to offset player by when they collide with an object

    for (int i = 0; i < rManager->GetObjects().size(); ++i)
    {
        if (rManager->GetObjects()[i]->GetObjectType() != "Player")
        {
            if (nextPosRect.left < rManager->GetObjects()[i]->GetCollisionBounds().right &&
                nextPosRect.right >= rManager->GetObjects()[i]->GetCollisionBounds().left &&
                nextPosRect.top < rManager->GetObjects()[i]->GetCollisionBounds().bottom &&
                nextPosRect.bottom >= rManager->GetObjects()[i]->GetCollisionBounds().top)
            {
                //gObjects.emplace_back(gObject);

                if (rManager->GetObjects()[i]->GetObjectType() == "Tile")
                {
                    if (collisionBounds.bottom <= rManager->GetObjects()[i]->GetCollisionBounds().top && nextPosRect.bottom > rManager->GetObjects()[i]->GetCollisionBounds().top && !collidedTop)			    // Collided from top, moving down
                    {
                        mPos.y = rManager->GetObjects()[i]->GetCollisionBounds().top - (objSize.y * abs(mScale.y) + collisionPosOffset);							                        // Setting position to just outside obj
                        mPos.x += currentVel.x * dTime;																										        // Only adding velocity on non colliding axis
                        currentVel.y = 0;
                        //(!isWallSliding) ? currentVel.y = 0 : 0;
                        //DBOUT("afafaf");
                        collided = true;
                        collidedTop = true;
                        grounded = true;
                        canShotGunJump = true;
                        fired = false;
                        recordLastCollision = 1;
                        //DBOUT("collidedTop");

                    }
                    else if (collisionBounds.top > rManager->GetObjects()[i]->GetCollisionBounds().bottom && nextPosRect.top <= rManager->GetObjects()[i]->GetCollisionBounds().bottom && !collidedBottom)	    // Collided from bottom, moving up
                    {
                        mPos.y = rManager->GetObjects()[i]->GetCollisionBounds().bottom + collisionPosOffset;																		        // Setting position to just outside obj
                        mPos.x += currentVel.x * dTime;																										        // Only adding velocity on non colliding axis
                        currentVel.y = 0;
                        //(!isWallSliding) ? currentVel.y = 0 : 0;

                        collided = true;
                        collidedBottom = true;
                        recordLastCollision = 2;
                        //DBOUT("collided bottom");
                    }
                    if (collisionBounds.right < rManager->GetObjects()[i]->GetCollisionBounds().left && nextPosRect.right >= rManager->GetObjects()[i]->GetCollisionBounds().left && !collidedLeft)			    // Collided from left, moving right
                    {
                        mPos.x = rManager->GetObjects()[i]->GetCollisionBounds().left - (objSize.x * abs(mScale.x) + collisionPosOffset);							                        // Setting position to just outside obj
                        mPos.y += currentVel.y * dTime;																										        // Only adding velocity on non colliding axis
                        currentVel.x = 0;

                        collided = true;
                        collidedLeft = true;
                        //canCollideRightWall = true;
                        //recordLastCollision = 3;

                        //if (kb.D && !deactivate_D)
                        //{
                        //    isWallSliding = true;
                        //}
                        //else
                        //{
                        //    isWallSliding = false;
                        //}
                        //DBOUT("collidedRight");

                    }
                    else if (collisionBounds.left > rManager->GetObjects()[i]->GetCollisionBounds().right && nextPosRect.left <= rManager->GetObjects()[i]->GetCollisionBounds().right && !collidedRight)		// Collided from right, moving left
                    {
                        mPos.x = rManager->GetObjects()[i]->GetCollisionBounds().right + collisionPosOffset;																		        // Setting position to just outside tile
                        mPos.y += currentVel.y * dTime;																										        // Only adding velocity on non colliding axis
                        currentVel.x = 0;

                        collided = true;
                        collidedRight = true;
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
                    }
                    //if player collided from their top bound
                    else if (collisionBounds.top > rManager->GetObjects()[i]->GetCollisionBounds().bottom && nextPosRect.top <= rManager->GetObjects()[i]->GetCollisionBounds().bottom && !collidedBottom)
                    {
                        currentVel.y = BOUNCE_PAD_JUMP_Y;
                    }
                    //if player collided from their right bound
                    else if (collisionBounds.right < rManager->GetObjects()[i]->GetCollisionBounds().left && nextPosRect.right >= rManager->GetObjects()[i]->GetCollisionBounds().left && !collidedLeft)
                    {
                        currentVel.y = -BOUNCE_PAD_JUMP_X;
                    }
                    //if player collided from their left bound
                    else if (collisionBounds.left > rManager->GetObjects()[i]->GetCollisionBounds().right && nextPosRect.left <= rManager->GetObjects()[i]->GetCollisionBounds().right && !collidedRight)
                    {
                        currentVel.y = BOUNCE_PAD_JUMP_X;
                    }
                }

                if (rManager->GetObjects()[i]->GetObjectType() == "ReloadGem" && rManager->GetObjects()[i]->GetActive())
                {
                    rManager->GetObjects()[i]->SetActive(false);
                    SetVelocity({ 0,0 });
                    fired = false;


                    /*canReloadGemJump = true;
                    slowdown_modifier = 0.1;*/
                }
                //else if (canReloadGemJump)
                //{
                //    gemSlowdownRemaining -= dTime;
                //    if (gemSlowdownRemaining <= 0)
                //    {
                //        canReloadGemJump = false;
                //        slowdown_modifier = 1;
                //    }
                //}
                //else if (!canReloadGemJump)
                //{
                //    //reset the slowdown time remaining
                //    gemSlowdownRemaining = GEM_SLOWDOWN_DURATION;
                //}

                if (rManager->GetObjects()[i]->GetObjectType() == "EndZone")
                {
                    rManager->LoadNextZone(dManager);
                }
            }
        }
    }


    //std::sort(gObjects.begin(), gObjects.end(), CompareDistance);


    //DBOUT(gObjects.size());
    for (GameObject* obj : gObjects)
    {
        
    }
}

void Player::NewCheckCollision(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
    DirectX::SimpleMath::Vector4 broadphaseBox = GetSweptBroadphaseBox(this);

    for (GameObject* obj : rManager->GetObjects())
    {
        if (AABBCheck(broadphaseBox, obj))
        {
            float normalX, normalY;
            float collisionTime = SweptAABB(this, obj, normalX, normalY, dTime);

            mPos.x += currentVel.x * collisionTime * dTime;
            mPos.y += currentVel.y * collisionTime * dTime;
            float remainingTime = 1.0f - collisionTime;
           
            if (collisionTime < 1.0f)
            {
                //// Slide
                float dotprod = (currentVel.x * normalY + currentVel.y * normalX) * remainingTime;
                currentVel.x = dotprod * normalY * dTime;
                currentVel.y = dotprod * normalX * dTime;
            }
        }



        //DBOUT("x: " + std::to_string(mPos.x) + " y: " + std::to_string(mPos.y));
    }
}

DirectX::SimpleMath::Vector4 Player::GetSweptBroadphaseBox(Player* obj)
{
    DirectX::SimpleMath::Vector2 broadphaseRect;
    DirectX::SimpleMath::Vector2 rectPos;
    DirectX::SimpleMath::Vector2 rectDims;

    rectPos.x = obj->currentVel.x > 0 ? obj->mPos.x : obj->mPos.x + obj->currentVel.x;
    rectPos.y = obj->currentVel.y > 0 ? obj->mPos.y : obj->mPos.y + obj->currentVel.y;
    rectDims.x = obj->currentVel.x > 0 ? obj->currentVel.x + (obj->GetObjectSize().x * obj->GetScale().x) : (obj->GetObjectSize().x * obj->GetScale().x) - obj->currentVel.x;
    rectDims.y = obj->currentVel.y > 0 ? obj->currentVel.y + (obj->GetObjectSize().y * obj->GetScale().y) : (obj->GetObjectSize().y * obj->GetScale().y) - obj->currentVel.y;

    return DirectX::SimpleMath::Vector4(rectPos.x, rectPos.y, rectDims.x, rectDims.y);
}

bool Player::AABBCheck(DirectX::SimpleMath::Vector4 obj1, GameObject* obj2)
{
    DirectX::SimpleMath::Vector2 obj1Pos = { obj1.x, obj1.y };
    DirectX::SimpleMath::Vector2 obj1Size = { obj1.z, obj1.w };

    return !(obj1Pos.x + obj1Size.x < obj2->GetPosition().x || 
        obj1Pos.x > obj2->GetPosition().x + (obj2->GetObjectSize().x * obj2->GetScale().x) || 
        obj1Pos.y + obj1Size.y < obj2->GetPosition().y || 
        obj1Pos.y > obj2->GetPosition().y + (obj2->GetObjectSize().y * obj2->GetScale().y));
}

float Player::SweptAABB(Player* obj1, GameObject* obj2, float& normalX, float& normalY, float dTime)
{
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    // Find distance between objects on near and far sides for x and y
    if (obj1->currentVel.x > 0.0f)
    {
        xInvEntry = obj2->GetPosition().x - (obj1->mPos.x + (obj1->GetObjectSize().x * obj1->GetScale().x));
        xInvExit = (obj2->GetPosition().x + (obj2->GetObjectSize().x * obj2->GetScale().x)) - obj1->GetPosition().x;
    }
    else
    {
        xInvEntry = (obj2->GetPosition().x + (obj2->GetObjectSize().x * obj2->GetScale().x)) - obj1->GetPosition().x;
        xInvExit = obj2->GetPosition().x - (obj1->mPos.x + (obj1->GetObjectSize().x * obj1->GetScale().x));
    }
    if (obj1->currentVel.y > 0.0f)
    {
        yInvEntry = obj2->GetPosition().y - (obj1->mPos.y + (obj1->GetObjectSize().y * obj1->GetScale().y));
        yInvExit = (obj2->GetPosition().y + (obj2->GetObjectSize().y * obj2->GetScale().y)) - obj1->GetPosition().y;
    }
    else
    {
        yInvEntry = (obj2->GetPosition().y + (obj2->GetObjectSize().y * obj2->GetScale().y)) - obj1->GetPosition().y;
        yInvExit = obj2->GetPosition().y - (obj1->mPos.y + (obj1->GetObjectSize().y * obj1->GetScale().y));
    }

    float xEntry, yEntry;
    float xExit, yExit;

    if (obj1->currentVel.x == 0.0f)
    {
        xEntry = -std::numeric_limits<float>::infinity();
        xExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        xEntry = xInvEntry / obj1->currentVel.x;
        xExit = xInvExit / obj1->currentVel.x;
    }

    if (obj1->currentVel.y == 0.0f)
    {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        yEntry = xInvEntry / obj1->currentVel.y;
        yExit = xInvExit / obj1->currentVel.y;
    }

    float entryTime, exitTime;

    entryTime = std::max(xEntry, yEntry);
    exitTime = std::min(xExit, yExit);

    if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f)
    {
        normalX = 0.0f;
        normalY = 0.0f;
        return 1.0f;
    }
    else // if there was a collision 
    {
        // calculate normal of collided surface
        if (xEntry > yEntry)
        {
            if (xInvEntry < 0.0f)
            {
                normalX = 1.0f;
                normalY = 0.0f;
            }
            else
            {
                normalX = -1.0f;
                normalY = 0.0f;
            }
        }
        else
        {
            if (yInvEntry < 0.0f)
            {
                normalX = 0.0f;
                normalY = 1.0f;
            }
            else
            {
                grounded = true;
                canShotGunJump = true;
                fired = false;
                normalX = 0.0f;
                normalY = -1.0f;
            }
        } // return the time of collisionreturn entryTime; 
    }
}

bool Player::CompareDistance(GameObject* a, GameObject* b)
{
    float aDist = sqrt((pow(a->GetPosition().x, 2) - pow(mPos.x, 2)) + (pow(a->GetPosition().y, 2) - pow(mPos.y, 2)));
    float bDist = sqrt((pow(b->GetPosition().x, 2) - pow(mPos.x, 2)) + (pow(b->GetPosition().y, 2) - pow(mPos.y, 2)));

    //DBOUT("A dist: " + std::to_string(aDist) + " B dist: " + std::to_string(bDist) + "\n");

    return fabs(aDist) < fabs(bDist);
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