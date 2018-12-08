#include "Physics.h"

Physics::Physics() 
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;

	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}

Physics::~Physics() 
{
	for (int i = dynamicWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject * obj = dynamicWorld->getCollisionObjectArray()[i];
		btRigidBody * body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete(body->getMotionState());
		}
		dynamicWorld->removeCollisionObject(obj);
		delete(obj);
	}

	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape * shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete(shape);
	}

	delete dynamicWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;

	collisionShapes.clear();
}

void Physics::Update()
{
	//dynamicWorld->stepSimulation(1.f / 60.f, 10);

	//for (int j = dynamicWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	//{
	//	btCollisionObject * obj = dynamicWorld->getCollisionObjectArray()[j];
	//	btRigidBody * body = btRigidBody::upcast(obj);
	//	btTransform trans;
	//	if (body && body->getMotionState())
	//	{
	//		body->getMotionState()->getWorldTransform(trans);
	//	}
	//	else
	//	{
	//		trans = obj->getWorldTransform();
	//	}
	//	//printf("World pos object %d = %f, %f, %f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//}
}

bool Physics::Load(void* content)
{
	dynamicWorld->setGravity(btVector3(0, -10, 0));
	{
		btCollisionShape * groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
		collisionShapes.push_back(groundShape);
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));
		btScalar mass(0.);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		btDefaultMotionState * myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody * body = new btRigidBody(rbInfo);

		dynamicWorld->addRigidBody(body);
	}

	{
		btCollisionShape * colShape = new btSphereShape(btScalar(1.f));
		collisionShapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		btDefaultMotionState * myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody * body = new btRigidBody(rbInfo);

		dynamicWorld->addRigidBody(body);
	}
	return true;
}

void Physics::Close(void* content)
{

}
