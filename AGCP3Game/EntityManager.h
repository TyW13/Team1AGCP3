#pragma once


#include <vector>
#include <memory>
#include "Entity.h"



class EntityManager {
public:
	EntityManager() {}
	virtual ~EntityManager() {}

	void AddEntity(std::unique_ptr<Entity>&& entity);
	void UpdateEntities(float delta);
	void DrawEntities(ID3D12GraphicsCommandList* commandList);

private:
	std::vector<std::unique_ptr<Entity>> entities_;
};


// For any potential future sprite sheets

enum class EntityType {Base, Enemy, Player};
enum class EntityState {Idle, Walking, Jumping, Attacking, Hurt, Dying};


class EntityManager;
class EntityBase
{
	friend class EntityManager;

public :
	EntityBase(EntityManager* l_entityMgr);

	virtual ~EntityBase();

	// Getters and Setters

	void Move(float l_x, float l_y);
	void AddVelocity(float l_x, float l_y);
	void Accelerate(float l_x, float l_y);
	void SetAcceleration(float l_x, float l_y);
	void ApplyFriction(float l_x, float l_y);

	virtual void Update(float l_dT);
	virtual void Draw();


protected:

	// Methods

	void UpdateAABB();
	void CheckCollisions();
	void ResolveCollisions();

	// Method for what THIS Entity does to the l_collider entity
};

