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

	JOB_RETURN Update(void * ptr);

	bool Load();
	void Close(void * content);

private:

	void ComponentUpdate(PhysicsComponentContent * PCContent);

	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btBroadphaseInterface * overlappingPairCache;
	btSequentialImpulseConstraintSolver * solver;

	btDiscreteDynamicsWorld * dynamicWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

