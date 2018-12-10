#pragma once
#include "Entity.h"
class MultiObject :
	public Entity
{
public:
	MultiObject(const std::string & name, EntityID id);
	~MultiObject();

	void Load(ComponentManager * & c_manager);
	void SetLocation(const int x, const int y);
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

inline void MultiObject::SetLocation(const int x, const int y)
{
	_transform = btTransform(btQuaternion(), btVector3(btScalar(x), btScalar(0.f), btScalar(y)));
}
