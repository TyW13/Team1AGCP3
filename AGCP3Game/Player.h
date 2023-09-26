#pragma once
#include "AudioManager.h"
#include "PlayerAnimation.h"

class ResourceManager;

class Player : public GameObject
{
public:
	Player(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, int collisionDirection, RECT objRect = { 0, 0, 0 ,0 })
		: GameObject(dManager, texPath, position, scale, active, objSize, objType, collisionDirection, objRect)
	{
		Init(dManager, texPath, position, scale, active, objSize, objType, collisionDirection, objRect);
	}

	void Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, int collisionDirection, RECT objRect = { 0, 0, 0 ,0 }) override;
	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Render(DeviceManager* dManager) override;

    void UpdateInput(DeviceManager* dManager, float dTime);

    bool GetGrounded() { return grounded; }
    bool GetFired() { return fired; }
	bool GetActive() override { return isActive; }
	DirectX::SimpleMath::Vector2 GetObjectSize() override { return objSize; }
	std::string GetObjectType() override { return objType; }
    int GetCollisionDirection() override { return collisionDirection; }
	RECT GetCollisionBounds() override { return collisionBounds; }
	virtual DirectX::SimpleMath::Vector2 GetPosition() { return mPos; }
	virtual DirectX::SimpleMath::Vector2 GetScale() { return mScale; }

	void SetActive(bool _isActive) override;
	void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize) override;
    void SetCollisionDirection(int _direction);
	void SetRect(RECT _objRect) override;
	void SetPosition(DirectX::SimpleMath::Vector2 _position) override;
	void SetScale(DirectX::SimpleMath::Vector2 _scale) override;

    void SetVelocity(DirectX::SimpleMath::Vector2 _newVel) { currentVel = _newVel; }

private:
    AudioManager audioManager;
    PlayerAnimation playerAnim;
    PlayerAnimation shotgunAnim;
    int animState = 0;
    int shotgunState = 0;

	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
	bool isActive;
	DirectX::SimpleMath::Vector2 objSize;
	std::string objType;
    int collisionDirection;
	RECT objRect;
	static DirectX::SimpleMath::Vector2 mPos;
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

    //------ movement variables ----------------------------------------------------------------//
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_wall_jump;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time_wall_jump;

    DirectX::SimpleMath::Vector2 currentVel = currentVel.Zero;
    DirectX::SimpleMath::Vector2 mousePos = mousePos.Zero;
    DirectX::SimpleMath::Vector2 direction = direction.Zero;

    //const variables
    const float MAX_JUMP_VEL = 400;
    const float MIN_JUMP_VEL = MAX_JUMP_VEL / 2;
    const float WALL_JUMP_VEL_Y = 1500;
    const float WALL_JUMP_VEL_X = 1500;
    const float SLIDE_DOWN_VEL = 80;					//sliding down velocity
    const float GRAVITY = 500;
    const float PLAYER_SPEED = 350;
    const float DRAG_X = 0.82;				            //for deceleration in x-axis on the ground
    const float DRAG_X_IN_AIR = 0.88;			    	//for deceleration in x-axis in air
    const float	DRAG_Y = 0.92;				            //for deceleration in y-axis
    const float HIGH_JUMP_TIME = 0.20;					//how much time it takes to do a higher jump
    const float LOW_JUMP_TIME = HIGH_JUMP_TIME / 2;	    //how much time it takes to do a lower jump
    const float BOUNCE_PAD_JUMP_X = 3600;               //bounce pad force in x axis
    const float BOUNCE_PAD_JUMP_Y = 4500;               //bounce pad force in y axis
    const float COYOTE_TIME_DURATION = 0.15;            //define the coyote time duration (in seconds)
    const float GEM_SLOWDOWN_DURATION = 1.3;            //define the coyote time duration (in seconds)

    

    double elapsed_time = 0;					        //measure how much time has elapsed between starting and ending time counting
    std::string jumpType;                               //set type of the jump (low/ high jump)

    //------ mouse
    bool detectMouseClick = true;					    //start detecting the mouse button

    //------ keyboard
    bool timeSpaceClickDetected = false;				//if space button has been released stop measuring time for picking either high or low jump

    //------ basic movement
    float spaceClickElapsedTime = 0.f;                  //check how much time has passed since pressing space button, uses chrono
    bool fired = false;
    bool grounded = false;				                //keep track if the player is grounded or not
    bool recordJumpTime = false;				        //start/stop recording jump time
    bool detectSpaceKey = true;				        	//start detecting the space button pressed down
    
    //------ reloadable gem
    float slowdown_modifier = 1;
    float gemSlowdownRemaining = 0.0f;                  //define a variable to track the reload jump slow down remaining
    bool canReloadGemJump = false;

    //------ coyote time
    int recordLastCollision = 0;                        //keep track of the last collision, can do coyote time only when collided with ground
    float coyoteTimeRemaining = 0.0f;                   //define a variable to track the coyote time remaining

    //------ shotgun jump
    bool canShotGunJump = false;                        //allow shotgun jumping

    //------ bounce pad
    double elapsed_t_bouncepad = 0;

    //------ wall jump
    double elapsedtime = 0;								//for deactivating A and D buttons after the player has wall jumped
    bool canCollideRightWall = false;                   //for not letting the player to jump off the same wall twice
    bool canCollideLeftWall = false;                    //for not letting the player to jump off the same wall twice
    bool isWallSliding = false;							//true whenever the player is wall sliding
    bool deactivate_A = false;							//deactivate A key input while wall jumping
    bool deactivate_D = false;							//deactivate D key input while wall jumping
    bool hasWallJumped = false;							//detect if wall jumped

    //------ audio
    bool jumpSound = false;                             //for jumping only, to prevent repeated jump sound

};

