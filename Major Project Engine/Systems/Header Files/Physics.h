#pragma once

#ifndef _PHYSICS_H
#define _PHYSICS_H
#include "System.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <stdio.h>
#include <exception>
#include <mutex>

/*
The Physics system uses the Bullet library to simulate the physics in the game world.
After the simulation then the transforms are updated.

Inherits System
*/
class Physics :
	public System
{
public:
	Physics(TaskManager & tm, SceneManager & sm);
	~Physics();

	/*
	Load the physics system
	*/
	bool Load();

	/*
	Handle any events that get passed through
	*/
	void HandleEvent(const EventType & e, void * data);

	/*
	Update the physics system
	Bullet::StepSimulation included
	*/
	JOB_RETURN Update(void * ptr);

	/*
	Load all the components in the scene manager into the Bullet dynamic world
	*/
	JOB_RETURN LoadComponents(void * ptr);

	/*
	Close the physics system
	*/
	void Close(void * content);

private:

	std::mutex dworld_lock, coll_detect;

	/*
	Update all objects that have physics object with the transform
	in the bullet stepsimulation
	*/
	JOB_RETURN ComponentUpdate(void * ptr);

	/*
	Read the collision detection from the bullet dynamic world.
	There was supposed to be some rendering done to show what is colliding
	boxes to boxes and boxes to projectiles
	*/
	JOB_RETURN CollisionDetection(void * ptr);

	/*
	When a projectile is loaded from the application
	put it into the dynamic world.
	*/
	JOB_RETURN LoadSingleComponent(void * ptr);

	btDefaultCollisionConfiguration * collisionConfiguration;	// BULLET REQUIRED
	btCollisionDispatcher * dispatcher;							// BULLET REQUIRED
	btBroadphaseInterface * overlappingPairCache;				// BULLET REQUIRED
	btSequentialImpulseConstraintSolver * solver;				// BULLET REQUIRED

	btDiscreteDynamicsWorld * dynamicWorld;						// BULLET REQUIRED

	btAlignedObjectArray<btCollisionShape*> collisionShapes;	// BULLET REQUIRED

	const int p_breakdown = 2;									// breaks down the physics jobs into workable chunks
	int box_on_box_coll = 0;
	int bullet_on_box_coll = 0;
};

#endif // !_PHYSICS_H
