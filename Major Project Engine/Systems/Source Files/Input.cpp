#include "Input.h"

Input::Input() {}

Input::~Input() {}

JOB_RETURN Input::Load(void* content)
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_EVENTS);
	return JOB_COMPLETED;
}

void Input::Close(void* content)
{

}

void Input::Update
(
	const GLfloat & _dt,
	const Scene * current_scene
)
{
	PlayerControllerComponent * pc_cp;
	for (auto & entity : current_scene->get_ent_manager()->retreive_list())
	{
		if ((pc_cp = current_scene->get_comp_manager()->get_component<PlayerControllerComponent*>(entity.first)) != nullptr)
		{
			player_controls(_dt, pc_cp, entity.second->get_transform_value());
		}
	}
}

void Input::player_controls(const GLfloat & _dt, PlayerControllerComponent * pc_cp, btTransform & transform)
{
	if (pc_cp->get_type() == CONTROL_TYPE::MOUSE_KEYBOARD)
	{
		const Uint8 * keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_W]) transform.getOrigin() += (btVector3(0.f, 0.f, 5.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_A]) transform.getOrigin() += (btVector3(5.f, 0.f, 0.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_S]) transform.getOrigin() += (btVector3(0.f, 0.f, -5.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_D]) transform.getOrigin() += (btVector3(-5.f, 0.f, 0.f) * btScalar(_dt));
	}
	else if (pc_cp->get_type() == CONTROL_TYPE::XBOX_CONTROLLER)
	{

	}
}

void Input::change_input()
{

}
