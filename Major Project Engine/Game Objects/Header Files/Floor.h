#pragma once
#include "Entity.h"
class Floor :
	public Entity
{
public:
	Floor(const std::string name, const EntityID);
	~Floor();

	void Load(ComponentManager * & c_manager);
};


inline Floor::Floor(const std::string name, const EntityID id)
	: Entity(name, id)
{
}

inline Floor::~Floor()
{
}

inline void Floor::Load(ComponentManager *& c_manager)
{
	c_manager->AddComponent(this->GetID(), new PlayerRenderComponent());
	c_manager->AddComponent(this->GetID(), new FloorPhysicsComponent());
	c_manager->AddComponent(this->GetID(), new Transform(btVector3(btScalar(0.f), btScalar(2.f), btScalar(0.f))));
}
