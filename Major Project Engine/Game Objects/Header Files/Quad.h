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
};


#endif // !_QUAD_H
