#pragma once
#include "System.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <stdio.h>
#include <exception>
#include <mutex>

class Physics :
	public System
{
public:
	Physics(TaskManager & tm, SceneManager & sm);
	~Physics();

	bool Load();
	void HandleEvent(const EventType & e, void * data);

	JOB_RETURN PreUpdate(void * ptr);
	JOB_RETURN Update(void * ptr);

	JOB_RETURN LoadComponents(void * ptr);
	void Close(void * content);

private:

	std::mutex dworld_lock;
	JOB_RETURN ComponentUpdate(void * ptr);

	JOB_RETURN LoadSingleComponent(void * ptr);

	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btBroadphaseInterface * overlappingPairCache;
	btSequentialImpulseConstraintSolver * solver;

	btDiscreteDynamicsWorld * dynamicWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

