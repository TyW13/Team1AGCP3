#pragma once
#include "GameObject.h"

class ResourceManager;

class Player : public GameObject
{
public:
	Player(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, bool isCollidable, RECT objRect = { 0, 0, 0 ,0 })
		: GameObject(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect)
	{
		Init(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect);
	}

	void Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, bool isCollidable, RECT objRect = { 0, 0, 0 ,0 }) override;
	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Render(DeviceManager* dManager) override;

    void UpdateInput(DeviceManager* dManager, float dTime);

	bool GetActive() override { return isActive; }
	DirectX::SimpleMath::Vector2 GetObjectSize() override { return objSize; }
	std::string GetObjectType() override { return objType; }
	bool GetIsCollidable() override { return isCollidable; }
	RECT GetCollisionBounds() override { return collisionBounds; }
	virtual DirectX::SimpleMath::Vector2 GetPosition() { return mPos; }
	virtual DirectX::SimpleMath::Vector2 GetScale() { return mScale; }

	void SetActive(bool _isActive) override;
	void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize) override;
	void SetIsCollidable(bool _isCollidable) override;
	void SetRect(RECT _objRect) override;
	void SetPosition(DirectX::SimpleMath::Vector2 _position) override;
	void SetScale(DirectX::SimpleMath::Vector2 _scale) override;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
	bool isActive;
	DirectX::SimpleMath::Vector2 objSize;
	std::string objType;
	bool isCollidable;
	RECT objRect;
	DirectX::SimpleMath::Vector2 mPos;
	DirectX::SimpleMath::Vector2 mScale;
	DirectX::SimpleMath::Vector2 mOrigin = { 0,0 };

    void CheckCollision(DeviceManager* dManager, ResourceManager* rManager, float dTime);
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
	RECT collisionBounds;
  
    DirectX::Keyboard::State kb;
    DirectX::Mouse::State mouse;

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
    const float GRAVITY = 400;
    const float PLAYER_SPEED = 400;
    const float DRAG_X = 0.82;				            //for deceleration in x-axis on the ground
    const float DRAG_X_IN_AIR = 0.88;			    	//for deceleration in x-axis in air
    const float	DRAG_Y = 0.92;				            //for deceleration in y-axis
    const float HIGH_JUMP_TIME = 0.20;					//how much time it takes to do a higher jump
    const float LOW_JUMP_TIME = HIGH_JUMP_TIME / 2;	    //how much time it takes to do a lower jump
    const float coyoteTimeDuration = 1.2f;              //define the coyote time duration (in seconds)

    float coyoteTimeRemaining = 0.0f;                   //define a variable to track the coyote time remaining

    double elapsed_time = 0;					        //measure how much time has elapsed between starting and ending time counting
    double elapsed_t_bouncepad = 0;

    std::string jumpType;

    bool fired = false;
    bool grounded = false;				                //
    bool timeSpaceClickDetected = false;				//if space button has been released stop measuring time for picking either high or low jump
    bool recordJumpTime = false;				        //start/stop recording jump time
    bool detectSpaceKey = true;				        	//start detecting the space button pressed down
    bool detectMouseClick = true;					    //
    bool canShotGunJump = false;                        //allow shotgun jumping
    bool canCollideRightWall = false;                   //for not letting the player to jump off the same wall twice
    bool canCollideLeftWall = false;                    //for not letting the player to jump off the same wall twice

    //------ simple "collisions"
    double elapsedtime = 0;								//for deactivating A and D buttons after the player has wall jumped
    bool isWallSliding = false;							//true whenever the player is wall sliding
    bool deactivate_A = false;							//deactivate A key input while wall jumping
    bool deactivate_D = false;							//deactivate D key input while wall jumping
    bool hasWallJumped = false;							//detect if wall jumped

    float spaceClickElapsedTime = 0.f;


};

