#pragma once

#ifndef _QUAD_H
#define _QUAD_H

#include "Entity.h"
#include "QuadRenderComponent.h"

class Quad :
	public Entity
{
public:
	Quad(const std::string & name, int id);
	~Quad();
	void Load(std::unique_ptr<ComponentManager> & c_manager);
};


#endif // !_QUAD_H
