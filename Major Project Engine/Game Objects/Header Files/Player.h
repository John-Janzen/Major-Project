#pragma once

#ifndef _PLAYER_H
#define _PLAYER_H

#include "Entity.h"


class Player :
	public Entity
{
public:
	Player(const std::string & name, int id);
	~Player();
	void Load(ComponentManager * & c_manager);
};


#endif // !_PLAYER_H
