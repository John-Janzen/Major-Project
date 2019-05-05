#include "Input.h"

Input::Input(TaskManager & tm, SceneManager & sm) : System(tm, sm) {}

Input::~Input() {}

bool Input::Load()
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_EVENTS);

	{
		m_task.dictionary.emplace(Job::JOB_INPUT_UPDATE, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_INPUT_UPDATE].emplace_back(Job::JOB_PHYSICS_UPDATE);
		m_task.dictionary[Job::JOB_INPUT_UPDATE].emplace_back(Job::JOB_RENDER_UPDATE);
	}

	return true;
}

void Input::Close(void* content)
{

}

JOB_RETURN Input::Update (void * ptr)
{
	for (auto comp : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<PlayerControllerComponent*>(comp));
		auto control = static_cast<PlayerControllerComponent*>(comp);
		{
			PlayerControls(Timer::Instance().GetDeltaTime(), control, static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, comp->_id)));
		}
	}
	return JOB_COMPLETED;
}

void Input::PlayerControls(const GLfloat & _dt, PlayerControllerComponent * pc_cp, Transform * transform)
{
	if (pc_cp->GetType() == CONTROL_TYPE::MOUSE_KEYBOARD)
	{
		const Uint8 * keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_W]) transform->_transform.getOrigin() += (btVector3(0.f, 0.f, -10.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_A]) transform->_transform.getOrigin() += (btVector3(-10.f, 0.f, 0.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_S]) transform->_transform.getOrigin() += (btVector3(0.f, 0.f, 10.f) * btScalar(_dt));
		if (keystate[SDL_SCANCODE_D]) transform->_transform.getOrigin() += (btVector3(10.f, 0.f, 0.f) * btScalar(_dt));
	}
	else if (pc_cp->GetType() == CONTROL_TYPE::XBOX_CONTROLLER)
	{

	}
}

void Input::ChangeInput()
{

}
