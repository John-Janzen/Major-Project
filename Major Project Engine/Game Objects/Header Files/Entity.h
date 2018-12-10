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

	int get_id() const { return _id; }
	std::string get_name() const { return _name; }
	void set_death() { death_flag = true; }
	bool get_death() { return death_flag; }

	btTransform get_transform() const { return _transform; }
	btTransform & get_transform_value() { return _transform; }

private:
	std::string _name;
	EntityID _id;
	bool death_flag;
protected:
	btTransform _transform;
};


#endif // !_ENTITY_H
