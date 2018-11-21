#pragma once

#ifndef _QUADRENDERCOMPONENT_H
#define _QUADRENDERCOMPONENT_H

#include "RenderComponent.h"

class QuadRenderComponent : public RenderComponent
{
public:
	QuadRenderComponent() 
		: RenderComponent(
			std::string("Assets/Models/grid.obj"), 
			std::string("Assets/Shaders/default_V_shader.glvs"), 
			std::string("Assets/Shaders/default_F_shader.glfs")) 
	{}

	~QuadRenderComponent() {}

private:

};

#endif // !_QUADRENDERCOMPONENT_H
