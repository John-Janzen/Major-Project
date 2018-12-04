#pragma once
#include "System.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <stdio.h>

class Physics :
	public System
{
public:
	Physics();
	~Physics();

	void Update();

	bool Load();
	void Close();

private:
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btBroadphaseInterface * overlappingPairCache;
	btSequentialImpulseConstraintSolver * solver;

	btDiscreteDynamicsWorld * dynamicWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

