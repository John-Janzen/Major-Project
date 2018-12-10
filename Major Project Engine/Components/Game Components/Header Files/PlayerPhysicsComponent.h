#pragma once

#ifndef _PLAYER_PHYSICSCOMPONENT_H
#define _PLAYER_PHYSICSCOMPONENT_H

#include "PhysicsComponent.h"

class PlayerPhysicsComponent
	: public PhysicsComponent
{
public:
	PlayerPhysicsComponent();
	~PlayerPhysicsComponent();

private:

};

inline PlayerPhysicsComponent::PlayerPhysicsComponent() 
{
	shape = new btBoxShape(btVector3(1.f, 1.f, 1.f));
	mass = btScalar(1.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		shape->calculateLocalInertia(mass, local_inertia);
}

inline PlayerPhysicsComponent::~PlayerPhysicsComponent() {}

#endif // !_PLAYER_PHYSICSCOMPONENT_H
