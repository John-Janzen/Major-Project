#include "MainScene.h"
#include "SceneManager.h"

MainScene::MainScene() {}

MainScene::~MainScene() {}

bool MainScene::Load(SceneManager & sm)
{	
	//m_entities->CreateEntity<Quad>("Quad")->Load(m_components);
	//this->LoadCamera();
	std::cout << "Number of objects being created: " << NUM_MULTI_OBJECTS * NUM_MULTI_OBJECTS * NUM_MULTI_OBJECTS << std::endl;
	this->LoadQuad(sm);
	this->LoadFloor(sm);
	//this->LoadPlayer(sm);
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
	m_components->AddComponent(camera->GetID(), new Transform(
		btQuaternion(btScalar(0.f), btScalar(0.5f), btScalar(0.f)), 
		btVector3(btScalar(0.f), btScalar(0.f), btScalar(-20.f))));
	m_components->AddComponent(camera->GetID(), new PlayerControllerComponent());
	m_components->AddComponent(camera->GetID(), new CameraComponent());*/
	return true;
}

bool MainScene::LoadQuad(SceneManager & sm)
{
	//Entity * quad = m_entities->CreateEntity<Quad>();
	Entity * & quad = sm.CreateEntity("Default");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(quad->GetID(), btVector3(btScalar(0.f), btScalar(0.f), btScalar(0.f))));
	sm.AddComponent(SceneManager::RENDER, new QuadRenderComponent(quad->GetID()));
	return true;
}

bool MainScene::LoadFloor(SceneManager & sm)
{
	//Entity * floor = m_entities->CreateEntity<Floor>();
	Entity * & floor = sm.CreateEntity("Floor");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(floor->GetID(), btVector3(btScalar(0.f), btScalar(-10.f), btScalar(0.f))));
	sm.AddComponent(SceneManager::PHYSICS, new FloorPhysicsComponent(floor->GetID()));
	sm.AddComponent(SceneManager::RENDER, new FloorRenderComponent(floor->GetID()));
	return true;
}

bool MainScene::LoadPlayer(SceneManager & sm)
{
	//Entity * player = m_entities->CreateEntity<Player>("Player", _player1_ID);
	Entity * & player = sm.CreateEntity("Player");
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(player->GetID(), btVector3(btScalar(0.f), btScalar(2.f), btScalar(0.f))));
	sm.AddComponent(SceneManager::PHYSICS, new PlayerPhysicsComponent(player->GetID()));
	sm.AddComponent(SceneManager::RENDER, new PlayerRenderComponent(player->GetID()));
	return true;
}

bool MainScene::LoadMultiObject(SceneManager & sm, const float x, const float y, const float z)
{
	//Entity * m_object = m_entities->CreateEntity<MultiObject>(std::string("MultObject#" + static_cast<int>(x + y + z)));
	Entity * & m_object = sm.CreateEntity(std::string("MultiObject"));
	sm.AddComponent(SceneManager::TRANSFORM, new Transform(m_object->GetID(), btQuaternion(btScalar(0.f), btScalar(0.f), btScalar(1.f)), btVector3(x, y + 20.f, z)));
	sm.AddComponent(SceneManager::PHYSICS, new PlayerPhysicsComponent(m_object->GetID()));
	sm.AddComponent(SceneManager::RENDER, new PlayerRenderComponent(m_object->GetID()));
	return true;
}

bool MainScene::Unload()
{
	return true;
}
