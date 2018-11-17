#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{

}

bool MainScene::Load()
{
	entity_manager->create_entity<Quad>("Quad")->Load(component_manager);
	std::shared_ptr<Player> player_one = entity_manager->create_entity<Player>("Player");
	player_one->Load(component_manager);
	_player1_ID = player_one->get_id();
	player_one.reset();

	return true;
}

bool MainScene::Unload()
{
	return true;
}
