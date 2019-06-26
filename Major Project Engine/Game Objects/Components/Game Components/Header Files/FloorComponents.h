#pragma once

#ifndef _FLOORCOMPONENTS_H
#define _FLOORCOMPONENTS_H

#include "PhysicsComponent.h"
#include "RenderComponent.h"

/*
The Physics Component specific to the Floor Object.
This instantiates with no mass and is unmoving.

Inherits from Physics Component
*/
struct FloorPhysicsComponent :
	public PhysicsComponent
{
	FloorPhysicsComponent(const std::uint16_t & id) 
		: PhysicsComponent(id)
	{
		coll_shape = new btBoxShape(btVector3(50.f, 0.1f, 50.f));
		mass = btScalar(0.f);
		local_inertia = btVector3(0.f, 0.f, 0.f);

		bool dynamic = (mass != 0.f);
		if (dynamic)
			coll_shape->calculateLocalInertia(mass, local_inertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, new btDefaultMotionState(), this->coll_shape, this->local_inertia);
		coll_object = new btRigidBody(rbInfo);
		coll_object->setFriction(btScalar(5.0f));				// Only thing special to floor is a set friction so objects don't keep sliding.
	}

	~FloorPhysicsComponent() {}
};

/*
The Render Component specific to the Floor Object + Wall Object.
This Instantiates with a floor model, test_texture, default vertex and frag shaders.

Inherits from Render Component
*/
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

#endif // !_FLOORCOMPONENTS_H
