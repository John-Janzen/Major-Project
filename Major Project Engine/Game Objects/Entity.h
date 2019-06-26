#pragma once

#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include <vector>

typedef std::uint16_t EntityID;

enum EntityType
{
	DEFAULT,
	PLAYER,
	MULTIOBJECT,
	WALLSFLOOR,
	BULLET,
	COUNT
};

struct Entity
{
	Entity(const std::string & name, const EntityID & id, const EntityType & type = EntityType::DEFAULT) 
		: _name(name), _id(id), death_flag(false), entity(type)
	{}
	~Entity() {}

	std::string _name;
	EntityID _id;
	EntityType entity;
	bool death_flag;
};


#endif // !_ENTITY_H
