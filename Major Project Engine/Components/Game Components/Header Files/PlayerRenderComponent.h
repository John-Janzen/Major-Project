#pragma once

#ifndef _PLAYERRENDERCOMPONENT_H
#define _PLAYERRENDERCOMPONENT_H

#include "RenderComponent.h"

class PlayerRenderComponent :
	public RenderComponent
{
public:
	PlayerRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/cube.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/Checkers.png");
	}
	~PlayerRenderComponent() {}

private:
};

#endif // !_PLAYERRENDERCOMPONENT_H
