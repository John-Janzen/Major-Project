#pragma once

#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>

/*
The Entity class only holds a name, id and what type of object it is.
Nothing more nothing less.
*/
struct Entity
{
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

	Entity(const std::string & name, const EntityID & id, const EntityType & type = EntityType::DEFAULT) 
		: _name(name), _id(id), entity(type)
	{}
	~Entity() {}

	std::string _name;
	EntityID _id;
	EntityType entity;
};


#endif // !_ENTITY_H
