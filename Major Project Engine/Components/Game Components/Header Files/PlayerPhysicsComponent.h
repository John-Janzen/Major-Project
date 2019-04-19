#pragma once

#ifndef _PLAYER_PHYSICSCOMPONENT_H
#define _PLAYER_PHYSICSCOMPONENT_H

#include "PhysicsComponent.h"

class PlayerPhysicsComponent
	: public PhysicsComponent
{
public:
	PlayerPhysicsComponent(const std::uint16_t & id);
	~PlayerPhysicsComponent();

private:

};

inline PlayerPhysicsComponent::PlayerPhysicsComponent(const std::uint16_t & id)
	: PhysicsComponent(id)
{
	//shape = new btBoxShape(btVector3(btScalar(1.f), btScalar(1.f), btScalar(1.f)));
	shape = new btSphereShape(btScalar(1.f));
	mass = btScalar(1.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		shape->calculateLocalInertia(mass, local_inertia);
}

inline PlayerPhysicsComponent::~PlayerPhysicsComponent() {}

#endif // !_PLAYER_PHYSICSCOMPONENT_H
