#pragma once

#ifndef _SPHERECOMPONENTS_H
#define _SPHERECOMPONENTS_H

#include "PhysicsComponent.h"
#include "RenderComponent.h"

/*
The Physics Component specific to the Sphere Object.
This instantiates with some mass and is moving.

Inherits from Physics Component
*/
struct SpherePhysicsComponent :
	public PhysicsComponent
{
	SpherePhysicsComponent(const std::uint16_t & id)
		: PhysicsComponent(id)
	{
		//shape = new btBoxShape(btVector3(btScalar(1.f), btScalar(1.f), btScalar(1.f)));
		coll_shape = new btSphereShape(btScalar(1.f));
		mass = btScalar(3.f);
		local_inertia = btVector3(0.f, 0.f, 0.f);

		bool dynamic = (mass != 0.f);
		if (dynamic)
			coll_shape->calculateLocalInertia(mass, local_inertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, new btDefaultMotionState(), this->coll_shape, this->local_inertia);
		coll_object = new btRigidBody(rbInfo);

		btRigidBody::upcast(this->coll_object)->setLinearVelocity(btVector3(0.f, 0.f, -100.f));			// Sets a linear velocity on the object do that it actually fires
	}

	~SpherePhysicsComponent() {}
};

/*
The Render Component specific to the Sphere Object.
This Instantiates with a sphere model, Color_Checker, default vertex and frag shaders.

Inherits from Render Component
*/
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

#endif // !_SPHERECOMPONENTS_H
