#include "Input.h"

Input::Input() {}

Input::~Input() {}

bool Input::Load(const Content * content)
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_EVENTS);
	return true;
}

void Input::Close(const Content * content)
{

}

void Input::Update
(
	const GLfloat & _dt,
	const Scene * current_scene
)
{
	std::shared_ptr<PlayerControllerComponent> pc_cp;
	for (auto & entity : current_scene->get_ent_manager()->retreive_list())
	{
		if ((pc_cp = current_scene->get_comp_manager()->get_component<PlayerControllerComponent>(entity.first)) != nullptr)
		{
			player_controls(_dt, pc_cp->get_type(), entity.second->get_transform());
		}
	}
}

void Input::player_controls(const GLfloat & _dt, const CONTROL_TYPE & control, const std::shared_ptr<Transform> & transform)
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
