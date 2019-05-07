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
	Physics(TaskManager & tm, SceneManager & sm);
	~Physics();

	bool Load();

	JOB_RETURN PreUpdate(void * ptr);
	JOB_RETURN Update(void * ptr);

	JOB_RETURN LoadComponents(void * ptr);
	void Close(void * content);

private:

	JOB_RETURN ComponentUpdate(void * ptr);

	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btBroadphaseInterface * overlappingPairCache;
	btSequentialImpulseConstraintSolver * solver;

	btDiscreteDynamicsWorld * dynamicWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

