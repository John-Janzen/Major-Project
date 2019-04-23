#pragma once

#ifndef _FLOORRENDERCOMPONENT_H
#define _FLOORRENDERCOMPONENT_H

#include "RenderComponent.h"

class FloorRenderComponent :
	public RenderComponent
{
public:
	FloorRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/grid.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/test_texture.png");
	}
	~FloorRenderComponent() {}

private:
};

#endif