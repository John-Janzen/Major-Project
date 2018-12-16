#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{
}

bool MainScene::Load()
{	
	//m_entities->CreateEntity<Quad>("Quad")->Load(m_components);
	this->LoadCamera();
	this->LoadQuad();
	this->LoadFloor();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				this->LoadMultiObject((i - (5 / (float)2)), (j - (5 / (float)2)), (k - (5 / (float)2)));
			}
		}
	}
	return true;
}

bool MainScene::LoadCamera()
{
	Entity * camera = m_entities->CreateEntity<Camera>("Camera", _camera);
	m_components->AddComponent(camera->GetID(), new Transform(btVector3(btScalar(0.f), btScalar(0.f), btScalar(-10.f))));
	m_components->AddComponent(camera->GetID(), new PlayerControllerComponent());
	m_components->AddComponent(camera->GetID(), new CameraComponent());
	return true;
}

bool MainScene::LoadQuad()
{
	Entity * quad = m_entities->CreateEntity<Quad>();
	m_components->AddComponent(quad->GetID(), new QuadRenderComponent());
	m_components->AddComponent(quad->GetID(), new Transform(btVector3(btScalar(5.f), btScalar(0.f), btScalar(-5.f))));
	return true;
}

bool MainScene::LoadFloor()
{
	Entity * floor = m_entities->CreateEntity<Floor>();
	m_components->AddComponent(floor->GetID(), new PlayerRenderComponent());
	m_components->AddComponent(floor->GetID(), new FloorPhysicsComponent());
	m_components->AddComponent(floor->GetID(), new Transform(btVector3(btScalar(0.f), btScalar(-58.f), btScalar(0.f))));
	return true;
}

bool MainScene::LoadPlayer()
{
	Entity * player = m_entities->CreateEntity<Player>("Player", _player1_ID);
	m_components->AddComponent(player->GetID(), new PlayerRenderComponent());
	m_components->AddComponent(player->GetID(), new PlayerPhysicsComponent());
	m_components->AddComponent(player->GetID(), new Transform(btVector3(btScalar(0.f), btScalar(0.f), btScalar(0.f))));
	return true;
}

bool MainScene::LoadMultiObject(const int x, const int y, const int z)
{
	Entity * m_object = m_entities->CreateEntity<MultiObject>(std::string("MultObject#" + (x + y + z)));
	m_components->AddComponent(m_object->GetID(), new PlayerRenderComponent());
	m_components->AddComponent(m_object->GetID(), new PlayerPhysicsComponent());
	m_components->AddComponent(m_object->GetID(), new Transform(btVector3(btScalar(x * 3), btScalar(y * 3), btScalar(z * 3))));
	return true;
}

bool MainScene::Unload()
{
	return true;
}
