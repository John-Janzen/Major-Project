#pragma once

#ifndef _INPUT_H
#define _INPUT_H

#include "System.h"
#include "ComponentHeaders.h"

#include <SDL.h>

class Input :
	public System
{
public:

	float player_speed = 10;
	Input(TaskManager & tm, SceneManager & sm);
	~Input();

	bool Load();
	void Close(void* content);

	JOB_RETURN Update(void * ptr);

	void PlayerControls(const GLfloat & _dt, PlayerControllerComponent * pc_cp, Transform * transform);

	void ChangeInput();
private:

};

#endif // !_INPUT_H
