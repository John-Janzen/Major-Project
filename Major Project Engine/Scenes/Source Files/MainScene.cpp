#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{

}

bool MainScene::Load(const std::unique_ptr<EntityManager> & e_manager, const std::unique_ptr<ComponentManager> & c_manager)
{
	int entity_id1 = e_manager->add_entity<Quad>("Quad");
	int entity_id2 = e_manager->create_entity_id("Test_Empty");

	c_manager->add_component(entity_id1, std::make_shared<QuadRenderComponent>());
	c_manager->add_component(entity_id2, std::make_shared<RenderComponent>());
	return true;
}

bool MainScene::Unload()
{
	return true;
}
