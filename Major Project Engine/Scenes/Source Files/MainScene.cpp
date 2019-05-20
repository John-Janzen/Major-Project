#include "MainScene.h"
#include "SceneManager.h"

MainScene::MainScene() {}

MainScene::~MainScene() {}

bool MainScene::Load(SceneManager & sm)
{	
	//m_entities->CreateEntity<Quad>("Quad")->Load(m_components);
	//this->LoadCamera();
	std::cout << "Number of Multi-Objects being created: " << NUM_MULTI_OBJECTS * NUM_MULTI_OBJECTS * NUM_MULTI_OBJECTS << std::endl;
	//this->LoadQuad(sm);
	this->LoadFloor(sm);
	this->LoadWalls(sm);
	this->LoadPlayer(sm);
	for (int i = 0; i < NUM_MULTI_OBJECTS; i++)
	{
		for (int j = 0; j < NUM_MULTI_OBJECTS; j++)
		{
			for (int k = 0; k < NUM_MULTI_OBJECTS; k++)
			{
				this->LoadMultiObject(sm, ((i * 3) - (NUM_MULTI_OBJECTS - 1) * 1.5), ((j * 3) - (NUM_MULTI_OBJECTS - 1) * 1.5), ((k * 3) - (NUM_MULTI_OBJECTS - 1) * 1.5));
			}
		}
	}
	return true;
}

bool MainScene::LoadCamera(SceneManager & sm)
{
	/*Entity * camera = m_entities->CreateEntity<Camera>("Camera", _camera);
	m_components->AddComponent(camera->_id, new Transform(
		btQuaternion(btScalar(0.f), btScalar(0.5f), btScalar(0.f)), 
		btVector3(btScalar(0.f), btScalar(0.f), btScalar(-20.f))));
	m_components->AddComponent(camera->_id, new PlayerControllerComponent());
	m_components->AddComponent(camera->_id, new CameraComponent());*/
	return true;
}

bool MainScene::LoadWalls(SceneManager & sm) 
{
	Entity * & w_wall = sm.CreateEntity("Wall_West");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(w_wall->_id, btQuaternion(0.f, 0.f, 1.57f), btVector3(btScalar(50.f), btScalar(25.f), btScalar(0.f))));
	sm.AddComponent(SceneManager::PHYSICS, new WallPhysicsComponent(w_wall->_id));
	sm.AddComponent(SceneManager::RENDER, new FloorRenderComponent(w_wall->_id));

	Entity * & e_wall = sm.CreateEntity("Wall_east");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(e_wall->_id, btQuaternion(0.f, 0.f, -1.57f), btVector3(btScalar(-50.f), btScalar(25.f), btScalar(0.f))));
	sm.AddComponent(SceneManager::PHYSICS, new WallPhysicsComponent(e_wall->_id));
	sm.AddComponent(SceneManager::RENDER, new FloorRenderComponent(e_wall->_id));

	Entity * & n_wall = sm.CreateEntity("Wall_north");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(n_wall->_id, btQuaternion(0.f, -1.57f, 0.f), btVector3(btScalar(0.f), btScalar(25.f), btScalar(50.f))));
	sm.AddComponent(SceneManager::PHYSICS, new WallPhysicsComponent(n_wall->_id));
	sm.AddComponent(SceneManager::RENDER, new FloorRenderComponent(n_wall->_id));

	/*Entity * & s_wall = sm.CreateEntity("Wall_south");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(s_wall->_id, btQuaternion(0.f, 1.57f, 0.f), btVector3(btScalar(0.f), btScalar(25.f), btScalar(-50.f))));
	sm.AddComponent(SceneManager::PHYSICS, new WallPhysicsComponent(s_wall->_id));
	sm.AddComponent(SceneManager::RENDER, new FloorRenderComponent(s_wall->_id));*/
	return true;
}

bool MainScene::LoadQuad(SceneManager & sm)
{
	//Entity * quad = m_entities->CreateEntity<Quad>();
	Entity * & quad = sm.CreateEntity("Default");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(quad->_id, btVector3(btScalar(0.f), btScalar(0.f), btScalar(0.f))));
	sm.AddComponent(SceneManager::RENDER, new QuadRenderComponent(quad->_id));
	return true;
}

bool MainScene::LoadFloor(SceneManager & sm)
{
	//Entity * floor = m_entities->CreateEntity<Floor>();
	Entity * & floor = sm.CreateEntity("Floor");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(floor->_id, btVector3(btScalar(0.f), btScalar(0.f), btScalar(0.f))));
	sm.AddComponent(SceneManager::PHYSICS, new FloorPhysicsComponent(floor->_id));
	sm.AddComponent(SceneManager::RENDER, new FloorRenderComponent(floor->_id));
	return true;
}

bool MainScene::LoadPlayer(SceneManager & sm)
{
	//Entity * player = m_entities->CreateEntity<Player>("Player", _player1_ID);
	Entity * & player = sm.CreateEntity("Player");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(player->_id, btVector3(btScalar(0.f), btScalar(2.f), btScalar(30.f))));
	sm.AddComponent(SceneManager::PHYSICS, new PlayerPhysicsComponent(player->_id));
	sm.AddComponent(SceneManager::RENDER, new PlayerRenderComponent(player->_id));
	sm.AddComponent(SceneManager::CONTROLLER, new PlayerControllerComponent(player->_id));
	return true;
}

bool MainScene::LoadMultiObject(SceneManager & sm, const float x, const float y, const float z)
{
	//Entity * m_object = m_entities->CreateEntity<MultiObject>(std::string("MultObject#" + static_cast<int>(x + y + z)));
	Entity * & m_object = sm.CreateEntity(std::string("MultiObject"));
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(m_object->_id, btQuaternion(btScalar(0.f), btScalar(0.f), btScalar(1.f)), btVector3(x, y + 1 + ((NUM_MULTI_OBJECTS - 1) * 1.5f), z)));
	sm.AddComponent(SceneManager::PHYSICS, new MultiPhysicsComponent(m_object->_id));
	sm.AddComponent(SceneManager::RENDER, new MultiRenderComponent(m_object->_id));
	return true;
}

bool MainScene::Unload()
{
	return true;
}
