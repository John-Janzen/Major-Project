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

JOB_RETURN Physics::Update(void * ptr)
{
	ComponentManager * comp_ptr = static_cast<ComponentManager*>(ptr);
	dynamicWorld->stepSimulation(Timer::Instance().GetDeltaTime());
	for (auto comp_it : comp_ptr->FindAllTypes<PhysicsComponent*>())
	{
		if (comp_it.second != nullptr)
		{
			TaskManager::Instance().RegisterJob(
				new Job(bind_function(&Physics::ComponentUpdate, this),
					"Physics Component Update",
					new PhysicsComponentContent(comp_it.second, comp_ptr->GetComponent<Transform*>(comp_it.first)), job::JOB_PHYSICS_COMPONENT));
		}
	}
	
	return JOB_COMPLETED;
}

JOB_RETURN Physics::ComponentUpdate(void * ptr)
{
	PhysicsComponentContent * PCContent = static_cast<PhysicsComponentContent*>(ptr);

	if (PCContent->p_cp != nullptr)
	{
		btRigidBody * body = PCContent->p_cp->GetRigidBody();
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(PCContent->trans->_transform);
		}
	}
	delete(PCContent);
	return JOB_COMPLETED;
}

JOB_RETURN Physics::Load(void * content)
{
	Scene * scene = static_cast<Scene*>(content);
	ComponentManager * comp_ptr = scene->GetCompManager();
	for (auto comp_it : comp_ptr->FindAllTypes<PhysicsComponent*>())
	{
		if (comp_it.second != nullptr)
		{
			collisionShapes.push_back(comp_it.second->GetCollisionShape());
			btDefaultMotionState * motion_state = new btDefaultMotionState(comp_ptr->GetComponent<Transform*>(comp_it.first)->_transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(comp_it.second->GetMass(), motion_state, comp_it.second->GetCollisionShape(), comp_it.second->GetLocalInertia());
			comp_it.second->SetRigidBody(new btRigidBody(rbInfo));
			dynamicWorld->addRigidBody(comp_it.second->GetRigidBody());
		}
	}
	return JOB_COMPLETED;
}

void Physics::Close(void* content)
{

}
