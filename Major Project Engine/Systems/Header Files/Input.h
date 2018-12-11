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
	Input();
	~Input();

	JOB_RETURN Load(void* content);
	void Close(void* content);

	void Update(const GLfloat & _dt, const Scene * current_scene);

	void PlayerControls(const GLfloat & _dt, PlayerControllerComponent * pc_cp, btTransform & transform);

	void ChangeInput();
private:

};

#endif // !_INPUT_H
