#pragma once

#ifndef _PLAYERRENDERCOMPONENT_H
#define _PLAYERRENDERCOMPONENT_H

#include "RenderComponent.h"

class PlayerRenderComponent :
	public RenderComponent
{
public:
	PlayerRenderComponent()
		: RenderComponent(
			std::string("Assets/Models/cube.obj"),
			std::string("Assets/Shaders/default_V_shader.glvs"),
			std::string("Assets/Shaders/default_F_shader.glfs"),
			std::string("Assets/Textures/Checkers.png"))
	{
	}
	~PlayerRenderComponent() {}

private:
};

#endif // !_PLAYERRENDERCOMPONENT_H
