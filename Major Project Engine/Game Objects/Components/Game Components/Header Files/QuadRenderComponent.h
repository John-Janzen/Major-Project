#pragma once

#ifndef _QUADRENDERCOMPONENT_H
#define _QUADRENDERCOMPONENT_H

#include "RenderComponent.h"

/*
The Render Component specific to the Quad Object.
This Instantiates with a Quad model, test_texture, default vertex and frag shaders.

Inherits from Render Component
*/
struct QuadRenderComponent : public RenderComponent
{
	QuadRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/quad.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/test_texture.png");
	}

	~QuadRenderComponent() {}

};

#endif // !_QUADRENDERCOMPONENT_H
