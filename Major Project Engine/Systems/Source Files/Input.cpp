#include "Input.h"

Input::Input() {}

Input::~Input() {}

bool Input::Load()
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_EVENTS);
	return true;
}

void Input::Close()
{

}

void Input::Update
(
	const GLfloat & _dt,
	const SDL_Event & sdl_event,
	const CONTROL_TYPE & control,
	const std::shared_ptr<Transform> & transform
)
{
	if (control == CONTROL_TYPE::MOUSE_KEYBOARD)
	{
		const Uint8 * keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_W]) transform->add_z_pos(5.0f * _dt);
		if (keystate[SDL_SCANCODE_A]) transform->add_x_pos(5.0f * _dt);
		if (keystate[SDL_SCANCODE_S]) transform->add_z_pos(-5.0f * _dt);
		if (keystate[SDL_SCANCODE_D]) transform->add_x_pos(-5.0f * _dt);
	}
	else if (control == CONTROL_TYPE::XBOX_CONTROLLER)
	{

	}
}

void Input::change_input()
{

}
