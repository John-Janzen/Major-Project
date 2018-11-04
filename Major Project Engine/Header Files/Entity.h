#pragma once

#include <map>
#include <typeinfo>
#include <typeindex>

class Entity
{
public:
	Entity(const std::string & name, int id) : _name(name), _id(id) {}
	~Entity() {}

	int get_id() const { return _id; }
	std::string get_name() { return _name; }

private:
	std::string _name;
	int _id;
};

