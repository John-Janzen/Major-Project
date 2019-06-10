#pragma once

#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include "BaseComponent.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

struct PhysicsComponent :
	public BaseComponent
{
	PhysicsComponent(const std::uint16_t & id);
	~PhysicsComponent();

	btCollisionShape * coll_shape;
	btCollisionObject * coll_object;

	btScalar mass;
	btVector3 local_inertia;

	//btVector3 linearVelocity;
};


#endif // !_PHYSICSCOMPONENT_H