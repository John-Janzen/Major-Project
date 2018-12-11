#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{
}

bool MainScene::Load()
{	
	//entity_manager->create_entity<Quad>("Quad")->Load(component_manager);
	entity_manager->create_entity<Quad>()->Load(component_manager);
	//entity_manager->create_entity<Player>("Player", _player1_ID)->Load(component_manager);
	entity_manager->create_entity<Floor>()->Load(component_manager);
	MultiObject * m_object;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			entity_manager->create_entity(std::string("MultObject#" + (i + j)), m_object);
			m_object->Load(component_manager);
			m_object->SetLocation(i - (5 / 2), j - (5 / 2));
		}
	}
	return true;
}

bool MainScene::Unload()
{
	return true;
}
