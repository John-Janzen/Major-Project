#pragma once

#ifndef _WALLPHYSICSCOMPONENT_H
#define _WALLPHYSICSCOMPONENT_H

#include "PhysicsComponent.h"

/*
The Physics Component specific to the Wall Object.
This instantiates with no mass and is unmoving.

Inherits from Physics Component
*/
struct WallPhysicsComponent :
	public PhysicsComponent
{
	WallPhysicsComponent(const std::uint16_t & id)
		: PhysicsComponent(id)
	{
		coll_shape = new btBoxShape(btVector3(50.f, 1.0f, 50.f));
		mass = btScalar(0.f);
		local_inertia = btVector3(0.f, 0.f, 0.f);

		bool dynamic = (mass != 0.f);
		if (dynamic)
			coll_shape->calculateLocalInertia(mass, local_inertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, new btDefaultMotionState(), this->coll_shape, this->local_inertia);
		coll_object = new btRigidBody(rbInfo);
		btRigidBody::upcast(coll_object)->setFriction(btScalar(5.0f));
	}
	~WallPhysicsComponent() {}
};

#endif // !_WALLPHYSICSCOMPONENT_H
