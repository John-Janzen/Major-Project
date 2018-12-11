#pragma once

#ifndef _ENTITY_H
#define _ENTITY_H

#include "ComponentManager.h"

#include <Bullet3Common\b3Transform.h>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <string>

class Entity
{
public:
	Entity(const std::string & name, EntityID id) : _name(name), _id(id), death_flag(false) {}
	~Entity() {}

	virtual void Load(ComponentManager * & c_manager)
	{
		_transform = btTransform();
		_transform.setIdentity();
	};

	int GetID() const { return _id; }
	std::string GetName() const { return _name; }
	void SetDeathFlag() { death_flag = true; }
	bool GetDeathFlag() { return death_flag; }

	btTransform GetTransform() const { return _transform; }
	btTransform & GetTransformAdd() { return _transform; }

private:
	std::string _name;
	EntityID _id;
	bool death_flag;
protected:
	btTransform _transform;
};


#endif // !_ENTITY_H
