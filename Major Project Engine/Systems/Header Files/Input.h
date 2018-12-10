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
	Input();
	~Input();

	bool Load(void* content);
	void Close(void* content);

	void Update(const GLfloat & _dt, const Scene * current_scene);

	void player_controls(const GLfloat & _dt, PlayerControllerComponent * pc_cp, btTransform & transform);

	void change_input();
private:

};

#endif // !_INPUT_H
