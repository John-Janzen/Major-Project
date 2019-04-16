#pragma once
#include "System.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <stdio.h>
#include <exception>

class Physics :
	public System
{
public:
	Physics(TaskManager & tm);
	~Physics();

	JOB_RETURN Update(void * ptr);

	JOB_RETURN ComponentUpdate(void * ptr);

	JOB_RETURN Load(void* content);
	void Close(void* content);

private:
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btBroadphaseInterface * overlappingPairCache;
	btSequentialImpulseConstraintSolver * solver;

	btDiscreteDynamicsWorld * dynamicWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

