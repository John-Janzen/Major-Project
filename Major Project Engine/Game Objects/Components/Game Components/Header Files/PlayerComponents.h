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
	void LoadExtraData();
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
		texture_path = std::string("Assets/Textures/Color_Checker.png");
	}
	~PlayerRenderComponent() {}
};

inline PlayerPhysicsComponent::PlayerPhysicsComponent(const std::uint16_t & id)
	: PhysicsComponent(id)
{
	//shape = new btBoxShape(btVector3(btScalar(1.f), btScalar(1.f), btScalar(1.f)));
	//shape = new btSphereShape(btScalar(1.f));
	shape = new btCapsuleShape(1.f, 4.f);
	mass = btScalar(10.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		shape->calculateLocalInertia(mass, local_inertia);

	//m_state = nullptr;
}

inline void PlayerPhysicsComponent::LoadExtraData()
{
	rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rigid_body->setActivationState(DISABLE_DEACTIVATION);
}

#endif // !_PLAYERCOMPONENTS_H
