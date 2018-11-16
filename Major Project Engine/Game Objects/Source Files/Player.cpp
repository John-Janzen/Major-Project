#include "Player.h"

Player::Player(const std::string & name, int id)
	: Entity(name, id) 
{

}

Player::~Player() {}

void Player::Load()
{
	ComponentManager::Instance().add_component(this->get_id(), std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, -5.0f)));
	ComponentManager::Instance().add_component(this->get_id(), std::make_shared<PlayerControllerComponent>());
	ComponentManager::Instance().add_component(this->get_id(), std::make_shared<CameraComponent>());
}
