#pragma once
#include "PhysicsComponent.h"
class FloorPhysicsComponent :
	public PhysicsComponent
{
public:
	FloorPhysicsComponent();
	~FloorPhysicsComponent();

private:

};

inline FloorPhysicsComponent::FloorPhysicsComponent() 
{
	shape = new btBoxShape(btVector3(1.f, 1.f, 1.f));
	mass = btScalar(0.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		shape->calculateLocalInertia(mass, local_inertia);
}

inline FloorPhysicsComponent::~FloorPhysicsComponent() {}
