#include "Input.h"

Input::Input(TaskManager & tm) : System(tm) {}

Input::~Input() {}

bool Input::Load(SceneManager * & sm)
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_EVENTS);
	return true;
}

void Input::Close(void* content)
{

}

void Input::Update
(
	SceneManager * & sm
)
{
	/*for (auto comp : comp_ptr->FindAllTypes<PlayerControllerComponent*>())
	{
		if (comp.second != nullptr)
		{
			PlayerControls(_dt, comp.second, comp_ptr->GetComponent<Transform*>(comp.first)->_transform);
		}
	}*/
}

void Input::PlayerControls(const GLfloat & _dt, PlayerControllerComponent * pc_cp, btTransform & transform)
{
	if (pc_cp->GetType() == CONTROL_TYPE::MOUSE_KEYBOARD)
	{
		const Uint8 * keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_W]) transform.getOrigin() += (btVector3(0.f, 0.f, 10.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_A]) transform.getOrigin() += (btVector3(10.f, 0.f, 0.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_S]) transform.getOrigin() += (btVector3(0.f, 0.f, -10.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_D]) transform.getOrigin() += (btVector3(-10.f, 0.f, 0.f) * btScalar(_dt));
	}
	else if (pc_cp->GetType() == CONTROL_TYPE::XBOX_CONTROLLER)
	{

	}
}

void Input::ChangeInput()
{

}
