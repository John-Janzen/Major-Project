#include "Player.h"

Player::Player(const std::string & name, int id)
	: Entity(name, id) 
{

}

Player::~Player() {}

void Player::Load(ComponentManager * & c_manager)
{
	c_manager->AddComponent(this->GetID(), new PlayerRenderComponent());
	c_manager->AddComponent(this->GetID(), new PlayerPhysicsComponent());
	c_manager->AddComponent(this->GetID(), new Transform(btVector3(btScalar(0.f), btScalar(0.f), btScalar(-5.f))));
}