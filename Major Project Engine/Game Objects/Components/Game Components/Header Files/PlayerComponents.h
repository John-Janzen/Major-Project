#pragma once

#ifndef _PLAYERCOMPONENTS_H
#define _PLAYERCOMPONENTS_H

#include "PhysicsComponent.h"
#include "RenderComponent.h"

/*
The Physics Component specific to the Player Object.
This instantiates with some mass and is  player moving.

Inherits from Physics Component
*/
struct PlayerPhysicsComponent
	: public PhysicsComponent
{
	PlayerPhysicsComponent(const std::uint16_t & id)
		: PhysicsComponent(id)
	{
		coll_shape = new btCapsuleShape(1.f, 3.f);		// The Player does have there own collision shape
		mass = btScalar(10.f);
		local_inertia = btVector3(0.f, 0.f, 0.f);

		bool dynamic = (mass != 0.f);
		if (dynamic)
			coll_shape->calculateLocalInertia(mass, local_inertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, new btDefaultMotionState(), this->coll_shape, this->local_inertia);
		coll_object = new btRigidBody(rbInfo);

		btRigidBody::upcast(coll_object)->setCollisionFlags(coll_object->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);		// Set Player to interact with world but doesn't interact with him
		btRigidBody::upcast(coll_object)->setActivationState(DISABLE_DEACTIVATION);															// No deactivation when calculating physics. Object never sleeps.
	}
	~PlayerPhysicsComponent() {}
};

/*
The Render Component specific to the Player Object.
This Instantiates with a Capsule model, Test_Checker, default vertex and frag shaders.

Inherits from Render Component
*/
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

#endif // !_PLAYERCOMPONENTS_H
