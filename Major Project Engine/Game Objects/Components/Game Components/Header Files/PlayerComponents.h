#pragma once

#ifndef _PLAYERCOMPONENTS_H
#define _PLAYERCOMPONENTS_H

#include "PhysicsComponent.h"
#include "RenderComponent.h"

struct PlayerPhysicsComponent
	: public PhysicsComponent
{
	PlayerPhysicsComponent(const std::uint16_t & id);
	~PlayerPhysicsComponent() {}
};

struct PlayerRenderComponent :
	public RenderComponent
{
	PlayerRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/capsule.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/Test_Checker.png");
	}
	~PlayerRenderComponent() {}
};

inline PlayerPhysicsComponent::PlayerPhysicsComponent(const std::uint16_t & id)
	: PhysicsComponent(id)
{
	coll_shape = new btCapsuleShape(1.f, 3.f);
	mass = btScalar(10.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		coll_shape->calculateLocalInertia(mass, local_inertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, new btDefaultMotionState(), this->coll_shape, this->local_inertia);
	coll_object = new btRigidBody(rbInfo);

	btRigidBody::upcast(coll_object)->setCollisionFlags(coll_object->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	btRigidBody::upcast(coll_object)->setActivationState(DISABLE_DEACTIVATION);
}

#endif // !_PLAYERCOMPONENTS_H
