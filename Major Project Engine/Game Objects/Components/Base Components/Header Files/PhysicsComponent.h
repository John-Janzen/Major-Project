#pragma once

#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include "BaseComponent.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

/*
The Physics Component holds all information for the bullet collision system.

Inherits from Base Component
*/
struct PhysicsComponent :
	public BaseComponent
{
	PhysicsComponent(const std::uint16_t & id)
		: BaseComponent(id) {}

	~PhysicsComponent() {}

	btCollisionShape * coll_shape;			// Bullet Collision Shape (Capsule)
	btCollisionObject * coll_object;		// Bullet Collision Object (RigidBody)

	btScalar mass;							// Mass of object
	btVector3 local_inertia;				// Starting inertia
};


#endif // !_PHYSICSCOMPONENT_H
