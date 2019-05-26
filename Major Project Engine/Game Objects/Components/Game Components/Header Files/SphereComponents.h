#pragma once

#ifndef _SPHERECOMPONENTS_H
#define _SPHERECOMPONENTS_H

#include "PhysicsComponent.h"
#include "RenderComponent.h"

struct SpherePhysicsComponent :
	public PhysicsComponent
{
	SpherePhysicsComponent(const std::uint16_t & id);
	~SpherePhysicsComponent() {}
	void LoadExtraData();
};

struct SphereRenderComponent :
	public RenderComponent
{
	SphereRenderComponent(const std::uint16_t & id) : RenderComponent(id)
	{
		model_path = std::string("Assets/Models/sphere.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/Color_Checker.png");
	}

	~SphereRenderComponent() {}
};


inline SpherePhysicsComponent::SpherePhysicsComponent(const std::uint16_t & id)
	: PhysicsComponent(id)
{
	//shape = new btBoxShape(btVector3(btScalar(1.f), btScalar(1.f), btScalar(1.f)));
	shape = new btSphereShape(btScalar(1.f));
	mass = btScalar(3.f);
	local_inertia = btVector3(0.f, 0.f, 0.f);

	bool dynamic = (mass != 0.f);
	if (dynamic)
		shape->calculateLocalInertia(mass, local_inertia);

	linearVelocity = btVector3(0.f, 0.f, -100.f);
}

inline void SpherePhysicsComponent::LoadExtraData()
{
	//rigid_body->setLinearVelocity(btVector3));
	rigid_body->setLinearVelocity(linearVelocity);
}

#endif // !_SPHERECOMPONENTS_H
