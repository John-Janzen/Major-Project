#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{
	Scene::~Scene();
}

bool MainScene::Load()
{	
	entity_manager->create_entity<Quad>("Quad")->Load(component_manager);
	entity_manager->create_entity<Player>("Player", _player1_ID)->Load(component_manager);
	return true;
}

bool MainScene::Unload()
{
	return true;
}
