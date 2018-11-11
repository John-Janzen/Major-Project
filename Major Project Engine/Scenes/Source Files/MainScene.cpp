#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{

}

bool MainScene::Load(const std::unique_ptr<EntityManager> & e_manager, const std::unique_ptr<ComponentManager> & c_manager)
{
	e_manager->create_entity<Quad>("Quad")->Load(c_manager);
	e_manager->create_entity<Entity>();
	

	return true;
}

bool MainScene::Unload()
{
	return true;
}
