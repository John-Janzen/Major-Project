#pragma once
#include "PhysicsComponent.h"
class MultiPhysicsComponent :
	public PhysicsComponent
{
public:
	MultiPhysicsComponent(const std::uint16_t & id);
	~MultiPhysicsComponent();
	void LoadExtraData() {}
};



inline MultiPhysicsComponent::MultiPhysicsComponent(const std::uint16_t & id)
	: PhysicsComponent(id)
{
	shape = new btBoxShape(btVector3(btScalar(1.f), btScalar(1.f), btScalar(1.f)));
	//shape = new btSphereShape(btScalar(1.f));
	mass = btScalar(5.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		shape->calculateLocalInertia(mass, local_inertia);

}


inline MultiPhysicsComponent::~MultiPhysicsComponent()
{
}
