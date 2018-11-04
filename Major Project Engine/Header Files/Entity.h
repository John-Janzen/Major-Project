#pragma once

#include <map>
#include <typeinfo>
#include <typeindex>

class Entity
{
public:
	Entity(const std::string & name, int id) : _name(name), _id(id), death_flag(false) {}
	~Entity() {}

	int get_id() const { return _id; }
	std::string get_name() const { return _name; }
	void set_death() { death_flag = true; }
	bool get_death() { return death_flag; }

private:
	std::string _name;
	int _id;
	bool death_flag;
};

