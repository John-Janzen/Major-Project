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

	void Load(std::unique_ptr<ComponentManager> & c_manager);
};


#endif // !_PLAYER_H
