#pragma once

#ifndef _FLOORCOMPONENTS_H
#define _FLOORCOMPONENTS_H

#include "PhysicsComponent.h"
#include "RenderComponent.h"

struct FloorPhysicsComponent :
	public PhysicsComponent
{
	FloorPhysicsComponent(const std::uint16_t & id);
	~FloorPhysicsComponent() {}
	void LoadExtraData();
};

struct FloorRenderComponent :
	public RenderComponent
{
	FloorRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/grid.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/test_texture.png");
	}

	~FloorRenderComponent() {}

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
}

inline void FloorPhysicsComponent::LoadExtraData()
{
	rigid_body->setFriction(btScalar(5.0f));
}

#endif // !_FLOORCOMPONENTS_H
