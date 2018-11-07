#pragma once
#include "Entity.h"
#include "QuadRenderComponent.h"

class Quad :
	public Entity
{
public:
	Quad(const std::string & name, int id);
	~Quad();
};

