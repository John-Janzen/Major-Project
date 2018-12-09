#pragma once

#ifndef _QUAD_H
#define _QUAD_H

#include "Entity.h"

class Quad :
	public Entity
{
public:
	Quad(const std::string & name, int id);
	~Quad();
	void Load(ComponentManager * & c_manager);
	
};


#endif // !_QUAD_H
