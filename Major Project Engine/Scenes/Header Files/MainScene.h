#pragma once

#ifndef _MAINSCENE_H
#define _MAINSCENE_H

#include "Scene.h"

#include <iostream>

/*
The Main Scene of the game. This is the only scene of the game and 
Loads many objects into the scene manager.
*/
class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	/*
	Load the scene with the scene manager
	*/
	bool Load(SceneManager & sm);

	/*
	Unload the scene
	currently unused
	*/
	bool Unload();

	static const int NUM_MULTI_OBJECTS = 8; // This can be cahnged to test the number of object that can be rendered on screen
											// be forewarned that 8 seems to be the number that runs best for my computer
											// yours might be lower or higher
											// it calculates out like num x num x num so  8x8x8 = 512 objects

private:
	
	/*
	Load a bullet into the scene.
	This is done to get the model into the game world.
	*/
	bool LoadBullet(SceneManager & sm);

	/*
	Load a wall into the scene
	*/
	bool LoadWalls(SceneManager & sm);

	/*
	Load a quad into the scene
	*/
	bool LoadQuad(SceneManager & sm);

	/*
	Load a floor into the scene
	*/
	bool LoadFloor(SceneManager & sm);

	/*
	Load a player into the scene
	*/
	bool LoadPlayer(SceneManager & sm);

	/*
	Load a multiobject into the scene
	*/
	bool LoadMultiObject(SceneManager & sm, const float x, const float y, const float z);

};

#endif // !
