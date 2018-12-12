#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{
}

bool MainScene::Load()
{	
	//entity_manager->CreateEntity<Quad>("Quad")->Load(component_manager);
	entity_manager->CreateEntity<Quad>()->Load(component_manager);
	//entity_manager->CreateEntity<Player>("Player", _player1_ID)->Load(component_manager);
	entity_manager->CreateEntity<Floor>()->Load(component_manager);
	MultiObject * m_object;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				entity_manager->CreateEntity(std::string("MultObject#" + (i + j)), m_object);
				m_object->Load(component_manager);
				m_object->SetLocation
				(
					component_manager->GetComponent<Transform*>(m_object->GetID()), 
					btVector3(btScalar(i - (5 / (float)2)), btScalar(j - (5 / (float)2)), btScalar(k - (5 / (float)2)))
				);
			}
		}
	}
	return true;
}

bool MainScene::Unload()
{
	return true;
}
