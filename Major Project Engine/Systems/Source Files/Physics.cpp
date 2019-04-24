#include "Physics.h"

Physics::Physics(TaskManager & tm, SceneManager & sm)
	: System(tm, sm)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;

	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicWorld->setGravity(btVector3(0, -9.8, 0));

	// Jobs that need to wait on other jobs go here
	{
		m_task.dictionary.emplace(Job::JOB_PHYSICS_UPDATE, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_PHYSICS_UPDATE].emplace_back(Job::JOB_RENDER_UPDATE);

		m_task.dictionary.emplace(Job::JOB_PHYSICS_PREUPDATE, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_PHYSICS_PREUPDATE].emplace_back(Job::JOB_PHYSICS_UPDATE);

		m_task.dictionary.emplace(Job::JOB_PHYSICS_COMPONENT, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_PHYSICS_COMPONENT].emplace_back(Job::JOB_RENDER_UPDATE);
	}
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
	std::vector<BaseComponent*> * PVector = static_cast<std::vector<BaseComponent*>*>(ptr);
	const int Breakdown = 10;

	int num = PVector->size() / Breakdown;
	int remainder = PVector->size() % Breakdown;

	int x, y;

	for (int i = 0; i < Breakdown; i++)
	{
		x = (i * num); y = (i + 1) * num;

		if (i == Breakdown - 1)
			y += remainder;

		m_task.RegisterJob(new Job(
			bind_function(&Physics::ComponentUpdate, this),
			"Physics_Update",
			new std::vector<BaseComponent*>(PVector->begin() + x, PVector->begin() + y),
			Job::JOB_PHYSICS_COMPONENT), true);
	}

	m_task.RegisterJob(new Job(bind_function(&Physics::PreUpdate, this), "Physics_Pre_Update", nullptr, Job::JOB_PHYSICS_PREUPDATE), false);
	return JOB_COMPLETED;
}

JOB_RETURN Physics::ComponentUpdate(void * ptr)
{
	std::vector<BaseComponent*> * PVector = static_cast<std::vector<BaseComponent*>*>(ptr);
	//Timer::Instance().Start();
	for (auto & physics : *PVector)
	{
		assert(dynamic_cast<PhysicsComponent*>(physics));
		auto obj = static_cast<PhysicsComponent*>(physics);
		{
			auto trans = m_scene.FindComponent(SceneManager::TRANSFORM, obj->_id);
			assert(dynamic_cast<Transform*>(trans));
			{
				btRigidBody * body = obj->GetRigidBody();
				if (body && body->getMotionState())
				{
					body->getMotionState()->getWorldTransform(static_cast<Transform*>(trans)->_transform);
				}
			}
		}
	}
	delete PVector;
	return JOB_COMPLETED;
}

bool Physics::Load()
{
	//std::vector<BaseComponent*> * objects = static_cast<std::vector<BaseComponent*>*>(content);
	//ComponentManager * comp_ptr = scene->GetCompManager();

	for (const auto & physics : m_scene.GetComponents(SceneManager::PHYSICS))
	{
		assert(dynamic_cast<PhysicsComponent*>(physics));
		auto obj = static_cast<PhysicsComponent*>(physics);
		{
			auto trans = m_scene.FindComponent(SceneManager::TRANSFORM, physics->_id);
			assert(dynamic_cast<Transform*>(trans));
			{
				collisionShapes.push_back(obj->GetCollisionShape());
				btDefaultMotionState * motion_state = new btDefaultMotionState(static_cast<Transform*>(trans)->_transform);
				btRigidBody::btRigidBodyConstructionInfo rbInfo(obj->GetMass(), motion_state, obj->GetCollisionShape(), obj->GetLocalInertia());
				obj->SetRigidBody(new btRigidBody(rbInfo));
				dynamicWorld->addRigidBody(obj->GetRigidBody());
			}
		}
	}
	return true;
}

void Physics::Close(void* content)
{

}
