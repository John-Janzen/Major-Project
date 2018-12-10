#pragma once

#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include "BaseComponent.h"

#include <btBulletCollisionCommon.h>

class PhysicsComponent :
	public BaseComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	btCollisionShape * getCollisionShape() { return shape; }
	btScalar getMass() { return mass; }
	btVector3 getLocalInertia() { return local_inertia; }
	btRigidBody * getRigidBody() { return rigid_body; }

	void setRigidBody(btRigidBody * rb) { rigid_body = rb; }

protected:
	btCollisionShape * shape;
	btScalar mass;
	btVector3 local_inertia;

	btRigidBody * rigid_body;
};


#endif // !_PHYSICSCOMPONENT_H
