#pragma once

#ifndef _MULTIOBJCOMPONENTS_H
#define _MULTIOBJCOMPONENTS_H

#include "PhysicsComponent.h"
#include "RenderComponent.h"

/*
The Physics Component specific to the Cube Object.
This instantiates with some mass and is moving.

Inherits from Physics Component
*/
struct MultiPhysicsComponent :
	public PhysicsComponent
{
	MultiPhysicsComponent(const std::uint16_t & id)
		: PhysicsComponent(id)
	{
		coll_shape = new btBoxShape(btVector3(btScalar(1.f), btScalar(1.f), btScalar(1.f)));
		//shape = new btSphereShape(btScalar(1.f));
		mass = btScalar(1.f);
		local_inertia = btVector3(0.f, 0.f, 0.f);

		bool dynamic = (mass != 0.f);
		if (dynamic)
			coll_shape->calculateLocalInertia(mass, local_inertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, new btDefaultMotionState(), this->coll_shape, this->local_inertia);
		coll_object = new btRigidBody(rbInfo);
	}
	~MultiPhysicsComponent() {}
};

/*
The Render Component specific to the Cube Object.
This Instantiates with a cube model, Checkers, default vertex and frag shaders.

Inherits from Render Component
*/
struct MultiRenderComponent :
	public RenderComponent
{
	MultiRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/cube.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/Checkers.png");
	}
	~MultiRenderComponent() {}
};

#endif // !_MULTIOBJCOMPONENTS_H
