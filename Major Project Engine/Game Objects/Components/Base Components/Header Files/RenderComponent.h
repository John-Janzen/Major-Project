#pragma once

#ifndef _RENDERCOMPONENT_H
#define _RENDERCOMPONENT_H

#include "BaseComponent.h"
#include "RenderStorage.h"

#include <memory>
#include <string>
#include <glm.hpp>

struct RenderComponent : public BaseComponent
{
	RenderComponent(const std::uint16_t & id)
		: BaseComponent(id)
	{
		model_id = 0;
		shader_id = 0;
		texture_id = 0;
	}
	~RenderComponent() {}

	GLuint vert_arr_obj = 0;

	std::string model_path;
	std::string vertex_shader_path;
	std::string fragment_shader_path;
	std::string texture_path;

	GLuint model_id;
	GLuint shader_id;
	GLuint texture_id;

	glm::vec4 _color = glm::vec4(1.0f);
};

#endif // !_RENDERCOMPONENT_H
