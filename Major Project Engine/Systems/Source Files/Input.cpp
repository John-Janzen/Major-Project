#include "Input.h"

Input::Input(TaskManager & tm, SceneManager & sm, EventHandler & eh) : System(tm, sm, eh) {}

Input::~Input() {}

bool Input::Load()
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_EVENTS);

	{
		m_task.dictionary.emplace(Job::JOB_INPUT_UPDATE, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_INPUT_UPDATE].emplace_back(Job::JOB_PHYSICS_PREUPDATE);
	}

	return true;
}

void Input::Close(void* content)
{

}

void Input::HandleEvent(const EventType & e, void * data)
{
}

JOB_RETURN Input::WindowControls(void * ptr)
{
	SDL_Event * sdl_event = static_cast<SDL_Event*>(ptr);			// Polls events for SDL (Mouse, Keyboard, window, etc.)
	switch (sdl_event->type)
	{
	case SDL_QUIT:
		h_event.SendEvent(EventType::GAME_CLOSED);
		break;
	case SDL_WINDOWEVENT:
		if (sdl_event->window.event == SDL_WINDOWEVENT_CLOSE)
		{
			h_event.SendEvent(EventType::GAME_CLOSED);
			break;
		}
		break;
	case SDL_KEYDOWN:
	{
		switch (sdl_event->key.keysym.scancode)
		{
		case SDL_SCANCODE_ESCAPE:
			h_event.SendEvent(EventType::GAME_CLOSED);
			break;
		case SDL_SCANCODE_T:
			h_event.SendEvent(EventType::OPEN_DEBUGGER);
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
	delete sdl_event;
	return JOB_COMPLETED;
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
