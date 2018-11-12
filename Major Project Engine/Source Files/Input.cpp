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

void Input::Update(const SDL_Event & sdl_event, const std::shared_ptr<PlayerControllerComponent>& control, const std::shared_ptr<Transform> & transform)
{

	if (control->get_type() == CONTROL_TYPE::MOUSE_KEYBOARD)
	{
		const Uint8 * keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_W]) transform->add_z_pos(0.01f);
		if (keystate[SDL_SCANCODE_A]) transform->add_x_pos(0.01f);
		if (keystate[SDL_SCANCODE_S]) transform->add_z_pos(-0.01f);
		if (keystate[SDL_SCANCODE_D]) transform->add_x_pos(-0.01f);
	}
	else if (control->get_type() == CONTROL_TYPE::XBOX_CONTROLLER)
	{

	}
}

void Input::change_input()
{

}
