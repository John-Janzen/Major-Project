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

	bool Load();
	void Close();

	void Update(const GLfloat & _dt, 
		const SDL_Event & sdl_event, 
		const std::shared_ptr<PlayerControllerComponent> & control, 
		const std::shared_ptr<Transform> & transform);

	void change_input();
private:

};

#endif // !_INPUT_H
