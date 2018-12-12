#pragma once
#include "Entity.h"
class MultiObject :
	public Entity
{
public:
	MultiObject(const std::string & name, EntityID id);
	~MultiObject();

	void Load(ComponentManager * & c_manager);
	void SetLocation(Transform * trans, btVector3 pos);
};

inline MultiObject::MultiObject(const std::string & name, EntityID id) 
	: Entity(name, id)
{}

inline MultiObject::~MultiObject() {}

inline void MultiObject::Load(ComponentManager * & c_manager)
{
	c_manager->AddComponent(this->GetID(), new PlayerRenderComponent());
	c_manager->AddComponent(this->GetID(), new PlayerPhysicsComponent());
	c_manager->AddComponent(this->GetID(), new Transform());
}

inline void MultiObject::SetLocation(Transform * trans, btVector3 pos)
{
	
}
