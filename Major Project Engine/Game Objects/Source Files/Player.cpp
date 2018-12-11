#include "Player.h"

Player::Player(const std::string & name, int id)
	: Entity(name, id) 
{

}

Player::~Player() {}

void Player::Load(ComponentManager * & c_manager)
{
	//c_manager->AddComponent(this->GetID(), std::make_shared<PlayerControllerComponent>());
	c_manager->AddComponent(this->GetID(), new PlayerRenderComponent());
	c_manager->AddComponent(this->GetID(), new PlayerPhysicsComponent());
	this->_transform = btTransform(btQuaternion(), btVector3(btScalar(0.f), btScalar(0.f), btScalar(-5.f)));
	//this->_transform = new Transform(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(), glm::vec3(1.0f));
}