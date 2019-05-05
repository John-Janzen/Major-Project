#pragma once
#include "PhysicsComponent.h"
class FloorPhysicsComponent :
	public PhysicsComponent
{
public:
	FloorPhysicsComponent(const std::uint16_t & id);
	~FloorPhysicsComponent();

private:

};

inline FloorPhysicsComponent::FloorPhysicsComponent(const std::uint16_t & id)
	: PhysicsComponent(id)

{
	//shape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
	shape = new btBoxShape(btVector3(50.f, 0.1f, 50.f));
	mass = btScalar(0.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		shape->calculateLocalInertia(mass, local_inertia);

	m_state = new btDefaultMotionState();

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_state, shape, local_inertia);
	SetRigidBody(new btRigidBody(rbInfo));
	rigid_body->setFriction(btScalar(5.0f));

}

inline FloorPhysicsComponent::~FloorPhysicsComponent() {}
