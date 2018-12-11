#include "Physics.h"

Physics::Physics() 
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;

	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicWorld->setGravity(btVector3(0, -10, 0));
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

void Physics::Update(void * ptr)
{
	Scene * scene = static_cast<Scene*>(ptr);
	dynamicWorld->stepSimulation(1.f / 60.f);
	PhysicsComponent * p_cp;
	for (auto & entity : scene->GetEntityManager()->retreive_list())
	{
		if ((p_cp = scene->GetCompManager()->GetComponent<PhysicsComponent*>(entity.second->GetID())) != nullptr)
		{
			btRigidBody * body = p_cp->GetRigidBody();
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(entity.second->GetTransformAdd());
			}
		}
	}
}

JOB_RETURN Physics::Load(void * content)
{
	Scene * scene = static_cast<Scene*>(content);
	PhysicsComponent * p_cp;
	for (auto entity : scene->GetEntityManager()->retreive_list())
	{
		if ((p_cp = scene->GetCompManager()->GetComponent<PhysicsComponent*>(entity.second->GetID())) != nullptr)
		{
			collisionShapes.push_back(p_cp->GetCollisionShape());
			btDefaultMotionState * motion_state = new btDefaultMotionState(entity.second->GetTransform());
			btRigidBody::btRigidBodyConstructionInfo rbInfo(p_cp->GetMass(), motion_state, p_cp->GetCollisionShape(), p_cp->GetLocalInertia());
			p_cp->SetRigidBody(new btRigidBody(rbInfo));
			dynamicWorld->addRigidBody(p_cp->GetRigidBody());
		}
	}
	return JOB_COMPLETED;
}

void Physics::Close(void* content)
{

}
