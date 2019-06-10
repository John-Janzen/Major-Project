#include "Physics.h"

Physics::Physics(TaskManager & tm, SceneManager & sm)
	: System(tm, sm)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicWorld->setGravity(btVector3(0, -9.8, 0));

	EventHandler::Instance().SubscribeEvent(EventType::PHYSICS_NEW_OBJECT, this);
	EventHandler::Instance().SubscribeEvent(EventType::STATE_CHANGE, this);
	EventHandler::Instance().SubscribeEvent(EventType::NEW_FRAME, this);
	EventHandler::Instance().SubscribeEvent(EventType::PLAYER_INPUT_TO_PHYSICS, this);

	// Jobs that need to wait on other jobs go here
	{
		m_task.dictionary.emplace(job::JOB_PHYSICS_UPDATE, std::vector<job::JOB_ID>());
		m_task.dictionary[job::JOB_PHYSICS_UPDATE].emplace_back(job::JOB_RENDER_UPDATE);

		m_task.dictionary.emplace(job::JOB_PHYSICS_COMPONENT, std::vector<job::JOB_ID>());
		m_task.dictionary[job::JOB_PHYSICS_COMPONENT].emplace_back(job::JOB_RENDER_UPDATE);
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

bool Physics::Load()
{
	m_task.RegisterJob(new Job(bind_function(&Physics::LoadComponents, this), "Physics_Component_Loader", &m_scene.GetComponents(SceneManager::PHYSICS), job::JOB_PHYSICS_LOAD));
	return true;
}

void Physics::HandleEvent(const EventType & e, void * data) 
{
	switch (e)
	{
	case EventType::PHYSICS_NEW_OBJECT:
		m_task.RegisterJob(new Job(bind_function(&Physics::LoadSingleComponent, this), "Physics_Load_Single", data, job::JOB_PHYSICS_LOAD_SINGLE));
		break;
	case EventType::STATE_CHANGE:
	{
		GAME_STATE gs = *static_cast<GAME_STATE*>(data);
		if (gs == PLAYING || gs == DEBUG_LOAD)
			paused = false;
		else
			paused = true;
		break;
	}
	case EventType::NEW_FRAME:
		if (!paused)
			m_task.RegisterJob(new Job(bind_function(&Physics::Update, this), "Physics_Update", &m_scene.GetComponents(SceneManager::PHYSICS), job::JOB_PHYSICS_UPDATE), true);
		break;
	case EventType::PLAYER_INPUT_TO_PHYSICS:
	{
		auto obj = static_cast<PhysicsComponent*>(m_scene.FindComponent(SceneManager::PHYSICS, static_cast<BaseComponent*>(data)->_id));
		{
			btRigidBody::upcast(obj->coll_object)->getMotionState()->setWorldTransform(static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, static_cast<BaseComponent*>(data)->_id))->_transform);
			btRigidBody::upcast(obj->coll_object)->activate(true);
		}
	}
		break;
	default:
		break;
	}
}

JOB_RETURN Physics::Update(void * ptr)
{	
	std::vector<BaseComponent*> * PVector = static_cast<std::vector<BaseComponent*>*>(ptr);

	{
		std::lock_guard<std::mutex> lock(dworld_lock);
		dynamicWorld->stepSimulation(Timer::Instance().GetDeltaTime(), 0, 0);
	}

	int n_updates_per_job = PVector->size() / p_breakdown;
	int remainder = PVector->size() % p_breakdown;

	int n_collisions_per_job = dynamicWorld->getDispatcher()->getNumManifolds() / p_breakdown;
	int remainder2 = dynamicWorld->getDispatcher()->getNumManifolds() % p_breakdown;

	int x, y;
	int x2, y2;

	if (n_updates_per_job > p_breakdown)
	{
		for (int i = 0; i < p_breakdown; i++)
		{
			x = i * n_updates_per_job;
			y = (i + 1) * n_updates_per_job;

			if (i == p_breakdown - 1)
				y += remainder;

			std::vector<int> * updates = new std::vector<int>{ x, y };
			m_task.RegisterJob(new Job(
				bind_function(&Physics::ComponentUpdate, this),
				"Physics_Component_Update",
				updates,
				job::JOB_PHYSICS_COMPONENT), false);

			
		}
		for (int i = 0; i < p_breakdown; i++) 
		{
			x2 = i * n_collisions_per_job;
			y2 = (i + 1) * n_collisions_per_job;

			if (i == p_breakdown - 1)
				y2 += remainder2;

			std::vector<int> * select = new std::vector<int>{ x2, y2 };
			m_task.RegisterJob(
				new Job(bind_function(&Physics::CollisionDetection, this),
					"Collision_Detection",
					select,
					job::JOB_PHYSICS_COLLISION_DETECTION), false);
		}
	}
	else
	{
		/*m_task.RegisterJob(new Job(
			bind_function(&Physics::ComponentUpdate, this),
			"Physics_Component",
			new std::vector<BaseComponent*>(PVector->begin(), PVector->end()),
			job::JOB_PHYSICS_COMPONENT), false);


		int * selection = (int*)malloc(2 * sizeof(int));
		selection[0] = 0;
		selection[1] = dynamicWorld->getDispatcher()->getNumManifolds();
		m_task.RegisterJob(
			new Job(bind_function(&Physics::CollisionDetection, this),
				"Collision_Detection",
				selection,
				job::JOB_PHYSICS_COLLISION_DETECTION), false);*/
	}
	return JOB_COMPLETED;
}

JOB_RETURN Physics::ComponentUpdate(void * ptr)
{
	std::vector<int> * updates = static_cast<std::vector<int>*>(ptr);
	const std::vector<BaseComponent*> & physics = m_scene.GetComponents(SceneManager::PHYSICS);

	for (auto it = physics.begin() + (*updates)[0]; it != physics.begin() + (*updates)[1]; ++it)
	{
		assert(dynamic_cast<PhysicsComponent*>(*it));
		{
			btRigidBody * body;
			if ((body = btRigidBody::upcast(static_cast<PhysicsComponent*>(*it)->coll_object)))
			{
				if (body && body->getMotionState())
				{
					body->getMotionState()->getWorldTransform(static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, (*it)->_id))->_transform);
				}
				else
				{
					static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, (*it)->_id))->_transform = body->getWorldTransform();
				}
			}
		}
	}
	delete updates;
	return JOB_COMPLETED;
}

JOB_RETURN Physics::CollisionDetection(void * ptr)
{
	std::vector<int> * select = static_cast<std::vector<int>*>(ptr);
	//auto asdad = "asda";
	int num = dynamicWorld->getDispatcher()->getNumManifolds();
	for (int i = (*select)[0]; i < (*select)[1]; i++)
	{
		btPersistentManifold * p_manifold = dynamicWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject * object1 = p_manifold->getBody0();
		const btCollisionObject * object2 = p_manifold->getBody1();
		if (object1->getCollisionShape()->isNonMoving() && object2->getCollisionShape()->isNonMoving())
		{

		}
	}
	delete select;
	return JOB_COMPLETED;
}

JOB_RETURN Physics::LoadSingleComponent(void * ptr)
{
	auto obj = static_cast<PhysicsComponent*>(ptr);
	{
		auto trans = static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, obj->_id));
		{
			collisionShapes.push_back(obj->coll_shape);
			btRigidBody * rb = nullptr;
			if ((rb = btRigidBody::upcast(obj->coll_object)))
			{
				rb->setWorldTransform(trans->_transform);
				rb->getMotionState()->setWorldTransform(trans->_transform);
				std::lock_guard<std::mutex> lock(dworld_lock);
				dynamicWorld->addRigidBody(btRigidBody::upcast(obj->coll_object));
			}
		}
	}
	return JOB_COMPLETED;
}

JOB_RETURN Physics::LoadComponents(void * ptr)
{
	for (const auto & physics : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<PhysicsComponent*>(physics));
		auto obj = static_cast<PhysicsComponent*>(physics);
		{
			auto trans = static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, physics->_id));
			{
				collisionShapes.push_back(obj->coll_shape);
				btRigidBody * rb = nullptr;
				if ((rb = btRigidBody::upcast(obj->coll_object)))
				{
					rb->setWorldTransform(trans->_transform);
					rb->getMotionState()->setWorldTransform(trans->_transform);
					dynamicWorld->addRigidBody(rb);
				}
			}
		}
	}
	return JOB_COMPLETED;
}

void Physics::Close(void* content) {}
