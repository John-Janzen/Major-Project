#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{

}

bool MainScene::Load(const std::unique_ptr<EntityManager> & e_manager, const std::unique_ptr<ComponentManager> & c_manager)
{
	e_manager->add_entity<Quad>("Quad", c_manager);
	e_manager->create_entity_id("Test_Empty", c_manager);

	return true;
}

bool MainScene::Unload()
{
	return true;
}
