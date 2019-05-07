#pragma once
#include "RenderComponent.h"
class MultiRenderComponent :
	public RenderComponent
{
public:
	MultiRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/cube.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/Checkers.png");
	
	}
	~MultiRenderComponent() {}
};

