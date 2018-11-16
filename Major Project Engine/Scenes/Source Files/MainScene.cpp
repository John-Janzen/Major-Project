#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() 
{

}

bool MainScene::Load()
{
	EntityManager::Instance().create_entity<Quad>("Quad")->Load();
	return true;
}

bool MainScene::Unload()
{
	return true;
}
