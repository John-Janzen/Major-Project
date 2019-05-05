#pragma once

#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include "BaseComponent.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class PhysicsComponent :
	public BaseComponent
{
public:
	PhysicsComponent(const std::uint16_t & id);
	~PhysicsComponent();

	btCollisionShape * GetCollisionShape() { return shape; }
	btMotionState * GetMotionState() { return m_state; }
	btScalar GetMass() const { return mass; }
	btVector3 GetLocalInertia() const { return local_inertia; }

	btRigidBody * GetRigidBody() const { return rigid_body; }
	void SetRigidBody(btRigidBody * rb) { rigid_body = rb; }

protected:
	btCollisionShape * shape;
	btMotionState * m_state;
	btScalar mass;
	btVector3 local_inertia;

	btRigidBody * rigid_body;
};


#endif // !_PHYSICSCOMPONENT_H
