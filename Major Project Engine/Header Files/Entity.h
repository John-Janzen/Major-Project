#pragma once
#include "BaseComponent.h"

#include <map>
#include <typeinfo>
#include <typeindex>

class Entity
{
public:
	Entity(const std::string & name, int id) : _name(name), _id(id) {}
	~Entity() {}

private:
	std::string _name;
	int _id;
};

