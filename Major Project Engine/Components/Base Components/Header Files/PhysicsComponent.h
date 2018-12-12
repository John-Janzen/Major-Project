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

	btCollisionShape * GetCollisionShape() { return shape; }
	btScalar GetMass() { return mass; }
	btVector3 GetLocalInertia() { return local_inertia; }

	btRigidBody * GetRigidBody() { return rigid_body; }
	void SetRigidBody(btRigidBody * rb) { rigid_body = rb; }

protected:
	btCollisionShape * shape;
	btScalar mass;
	btVector3 local_inertia;

	btRigidBody * rigid_body;
};


#endif // !_PHYSICSCOMPONENT_H
