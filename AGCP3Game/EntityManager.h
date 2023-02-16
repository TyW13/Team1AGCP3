#pragma once


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

	void move(float l_x, float l_y);
	void addVelocity(float l_x, float l_y);


};

