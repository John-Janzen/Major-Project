#include "Player.h"

Player::Player(const std::string & name, int id)
	: Entity(name, id) 
{

}

Player::~Player() {}

void Player::Load(ComponentManager * & c_manager)
{
	//c_manager->add_component(this->get_id(), std::make_shared<PlayerControllerComponent>());
	c_manager->add_component(this->get_id(), new PlayerRenderComponent());
	this->_transform = new Transform(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(), glm::vec3(1.0f));
}