#pragma once

#ifndef _RENDERCOMPONENT_H
#define _RENDERCOMPONENT_H

#include "BaseComponent.h"

#include <string>
#include <GL\glew.h>
#include <glm.hpp>

/*
The Render Component controls the model, texture and shaders.
This class takes paths for all then is loaded in the Render System.

Inherits Base Component
*/
struct RenderComponent : public BaseComponent
{
	RenderComponent(const std::uint16_t & id)
		: BaseComponent(id), model_id(0), shader_id(0), texture_id(0)
	{}
	~RenderComponent() {}

	GLuint vert_arr_obj = 0;			// Every Render object has to have a independant VAO (Vertex Array Object)
										// Something OpenGL enforces.

	std::string model_path;				// Path to the model
	std::string vertex_shader_path;		// Path to the Vertex Shader
	std::string fragment_shader_path;	// Path to the Frag Shader
	std::string texture_path;			// Path to the Texture

	GLuint model_id;					// ID of the model (Storage class)
	GLuint shader_id;					// ID of the shader (Storage class)
	GLuint texture_id;					// ID of the texture (Storage class)

	glm::vec4 _color = glm::vec4(1.0f);	// Basic color vertex
};

#endif // !_RENDERCOMPONENT_H
