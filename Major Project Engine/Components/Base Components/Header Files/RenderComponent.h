#pragma once

#ifndef _RENDERCOMPONENT_H
#define _RENDERCOMPONENT_H

#include "BaseComponent.h"
#include "FileLoader.h"

#include <memory>
#include <string>

class RenderComponent : public BaseComponent
{
public:
	RenderComponent(const std::string & model, const std::string & v_shader, const std::string f_shader);
	RenderComponent();
	~RenderComponent();

	std::shared_ptr<Model> get_model() const { return current_model; }
	std::shared_ptr<Shader> get_v_shader() const { return current_v_shader; }
	std::shared_ptr<Shader> get_f_shader() const { return current_f_shader; }

	GLuint & get_v_buffer() { return vert_buff_obj; }
	GLuint & get_v_array() { return vert_arr_obj; }
	GLuint & get_e_buffer() { return elem_buff_obj; }

	const GLuint & get_shader_prog() { return shade_prog; }
	void set_shader_prog(const GLuint & prog) { shade_prog = prog; }

	const GLint & get_proj_loc() { return r_project_mat4_loc; }
	void set_proj_loc(const GLint & matrix_loc) { r_project_mat4_loc = matrix_loc; }

	const GLint & get_model_loc() { return r_model_mat4_loc; }
	void set_model_loc(const GLint & matrix_loc) { r_model_mat4_loc = matrix_loc; }

	const GLint & get_color_loc() { return r_color_vec4_loc; }
	void set_color_loc(const GLint & matrix_loc) { r_color_vec4_loc = matrix_loc; }


protected:

private:
	std::shared_ptr<Model> current_model;
	std::shared_ptr<Shader> current_v_shader;
	std::shared_ptr<Shader> current_f_shader;

	GLint r_project_mat4_loc;
	GLint r_model_mat4_loc;
	GLint r_color_vec4_loc;

	GLuint vert_buff_obj = 0;
	GLuint vert_arr_obj = 0;
	GLuint elem_buff_obj = 0;
	GLuint shade_prog = 0;

};

#endif // !_RENDERCOMPONENT_H
