#pragma once
#include "PhysicsComponent.h"
class WallPhysicsComponent :
	public PhysicsComponent
{
public:
	WallPhysicsComponent(const std::uint16_t & id);
	~WallPhysicsComponent();
};

inline WallPhysicsComponent::WallPhysicsComponent(const std::uint16_t & id)
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

inline WallPhysicsComponent::~WallPhysicsComponent() {}
