#pragma once
#include "Entity.h"
class MultiObject :
	public Entity
{
public:
	MultiObject(const std::string & name, EntityID id);
	~MultiObject();

	void Load(ComponentManager * & c_manager);
	void SetLocation(const float x, const float y, const float z);
};

inline MultiObject::MultiObject(const std::string & name, EntityID id) 
	: Entity(name, id)
{}

inline MultiObject::~MultiObject() {}

inline void MultiObject::Load(ComponentManager * & c_manager)
{
	c_manager->add_component(this->get_id(), new PlayerRenderComponent());
	c_manager->add_component(this->get_id(), new PlayerPhysicsComponent());
}

inline void MultiObject::SetLocation(const float x, const float y, const float z)
{
	_transform = btTransform();
	_transform.setIdentity();
	_transform.setOrigin(btVector3(btScalar(x * 3), btScalar(z * 3), btScalar(y * 3)));
}
