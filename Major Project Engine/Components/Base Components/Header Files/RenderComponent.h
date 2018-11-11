#pragma once

#ifndef _RENDERCOMPONENT_H
#define _RENDERCOMPONENT_H

#include "BaseComponent.h"
#include "FileLoader.h"

#include <memory>

class RenderComponent : public BaseComponent
{
public:
	RenderComponent(const std::string & model, const std::string & v_shader, const std::string f_shader);
	RenderComponent();
	~RenderComponent();

	std::shared_ptr<Model> get_model() const { return current_model; }
	std::shared_ptr<Shader> get_v_shader() const { return current_v_shader; }
	std::shared_ptr<Shader> get_f_shader() const { return current_f_shader; }

	GLuint & get_vertex_buffer() { return vertex_buffer_obj; }
	GLuint & get_vertex_array() { return vertex_array_obj; }
	GLuint & get_element_buffer() { return element_buffer_obj; }
	GLuint & get_shader_program() { return shader_program; }

	GLint & get_render_proj_loc() { return render_projection_matrix_loc; }
	GLint & get_render_model_loc() { return render_model_matrix_loc; }
	GLint & get_render_color_loc() { return render_color_vec; }

private:
	int test = 5;
	std::shared_ptr<Model> current_model;
	std::shared_ptr<Shader> current_v_shader;
	std::shared_ptr<Shader> current_f_shader;

	GLint render_projection_matrix_loc;
	GLint render_model_matrix_loc;
	GLint render_color_vec;

	GLuint vertex_buffer_obj = 0;
	GLuint vertex_array_obj = 0;
	GLuint element_buffer_obj = 0;
	GLuint shader_program = 0;

};

#endif // !_RENDERCOMPONENT_H
