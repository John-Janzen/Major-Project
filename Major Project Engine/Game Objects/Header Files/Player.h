#pragma once
#include "Entity.h"
class Player :
	public Entity
{
public:
	Player(const std::string & name, int id);
	~Player();

	void Load(const std::unique_ptr<ComponentManager> & c_manager);
};
