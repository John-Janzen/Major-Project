#include "Input.h"

Input::Input(TaskManager & tm, SceneManager & sm) : System(tm, sm) {}

Input::~Input() {}

bool Input::Load()
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_EVENTS);

	EventHandler::Instance().SubscribeEvent(EventType::STATE_CHANGE, this);
	EventHandler::Instance().SubscribeEvent(EventType::NEW_FRAME, this);

	{
		m_task.dictionary.emplace(job::JOB_INPUT_UPDATE, std::vector<job::JOB_ID>());
		m_task.dictionary[job::JOB_INPUT_UPDATE].emplace_back(job::JOB_PHYSICS_UPDATE);
	}

	return true;
}

void Input::Close(void* content)
{

}

void Input::HandleEvent(const EventType & e, void * data)
{
	switch (e)
	{
	case EventType::STATE_CHANGE:
	{
		GAME_STATE gs = *static_cast<GAME_STATE*>(data);
		if (gs == PLAYING || gs == DEBUG_LOAD)
			paused = false;
		else
			paused = true;
		break;
	}
	case EventType::NEW_FRAME:
		if (!paused)
			m_task.RegisterJob(new Job(bind_function(&Input::Update, this), "Input_Update", &m_scene.GetComponents(SceneManager::CONTROLLER), job::JOB_INPUT_UPDATE), false);
		break;
	default:
		break;
	}
}

JOB_RETURN Input::Update (void * ptr)
{
	for (auto comp : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<PlayerControllerComponent*>(comp));
		auto control = static_cast<PlayerControllerComponent*>(comp);
		{
			PlayerControls(control, static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, comp->_id)));
		}
	}
	return JOB_COMPLETED;
}

void Input::PlayerControls(PlayerControllerComponent * pc_cp, Transform * transform)
{
	if (pc_cp->current_type == CONTROL_TYPE::MOUSE_KEYBOARD)
	{
		const float deltaTime = Timer::Instance().GetDeltaTime();
		int x, y;
		Uint32 mouse_state = SDL_GetRelativeMouseState(&x, &y);
		if (mouse_state == SDL_BUTTON_LEFT && !buttonHeld)
		{
			EventHandler::Instance().SendEvent(EventType::LEFT_MOUSE_BUTTON, pc_cp);
			buttonHeld = true;
		}
		if (buttonHeld && mouse_state != SDL_BUTTON_LEFT) buttonHeld = false;


		if (x != 0 || y != 0)
		{
			float deltaX = x * camera_rotation_deg * DEG_TO_RAD;
			float deltaY = -y * camera_rotation_deg * DEG_TO_RAD;

			btMatrix3x3 player_matrix;
			player_matrix.setRotation(btQuaternion(btVector3(0.f, 1.f, 0.f), -deltaX * btScalar(deltaTime)));

			transform->_transform.getBasis() *= player_matrix;

			auto c_component = m_scene.FindComponent(SceneManager::CAMERA, pc_cp->_id);
			if (c_component != nullptr)
			{
				auto camera = static_cast<CameraComponent*>(c_component);
				{
					auto right = camera->_aim.cross(camera->_up).normalize();

					camera->_aim = camera->_aim.rotate(right, deltaY * btScalar(deltaTime));
					camera->_up = camera->_up.rotate(right, deltaY * btScalar(deltaTime));
					
					camera->_aim =camera->_aim.rotate(btVector3(0.f, 1.f, 0.f), -deltaX * btScalar(deltaTime));
					camera->_up = camera->_up.rotate(btVector3(0.f, 1.f, 0.f), -deltaX * btScalar(deltaTime));
				}
			}
		}

		const Uint8 * key_state = SDL_GetKeyboardState(NULL);
		btVector3 distance = btVector3(0.f, 0.f, 0.f);
		if (key_state[SDL_SCANCODE_W]) distance += (btVector3(0.f, 0.f, player_speed) * btScalar(deltaTime));
		if (key_state[SDL_SCANCODE_A]) distance += (btVector3(player_speed, 0.f, 0.f) * btScalar(deltaTime));
		if (key_state[SDL_SCANCODE_S]) distance += (btVector3(0.f, 0.f, -player_speed) * btScalar(deltaTime));
		if (key_state[SDL_SCANCODE_D]) distance += (btVector3(-player_speed, 0.f, 0.f) * btScalar(deltaTime));
		if (key_state[SDL_SCANCODE_T] && !Tbutton) { EventHandler::Instance().SendEvent(EventType::T_BUTTON_PRESSED); Tbutton = true; }
		if (!key_state[SDL_SCANCODE_T] && Tbutton) { Tbutton = false; }
		
		if (!distance.isZero())
		{
			transform->_transform.getOrigin() += transform->_transform.getBasis() * -distance;
			transform->_transform.getOrigin().setY(2.f);
		}
	}
	else if (pc_cp->current_type == CONTROL_TYPE::XBOX_CONTROLLER)
	{

	}
}

void Input::ChangeInput()
{

}
