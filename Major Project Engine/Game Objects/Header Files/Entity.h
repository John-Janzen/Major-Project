#pragma once

#ifndef _ENTITY_H
#define _ENTITY_H

#include "ComponentManager.h"
#include "Transform.h"

#include <map>
#include <typeinfo>
#include <typeindex>
#include <string>

class Entity
{
public:
	Entity(const std::string & name, int id) : _name(name), _id(id), death_flag(false) {}
	~Entity() {}

	int get_id() const { return _id; }
	std::string get_name() const { return _name; }
	void set_death() { death_flag = true; }
	bool get_death() { return death_flag; }

	const Transform * get_transform() const { return _transform; }
	Transform * & get_transform_value() { return _transform; }

private:
	std::string _name;
	int _id;
	bool death_flag;
protected:
	Transform * _transform;
};


#endif // !_ENTITY_H
