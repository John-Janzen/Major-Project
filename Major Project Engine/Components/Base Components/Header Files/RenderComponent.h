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
	RenderComponent(const std::string model, const std::string v_shader, const std::string f_shader, const std::string texture);
	RenderComponent();
	~RenderComponent();

	Model * get_model() const { return current_model; }
	Shader * get_shader() const { return current_shader; }
	Texture * get_texture() const { return current_texture; }

	GLuint & get_v_buffer() { return vert_buff_obj; }
	GLuint & get_v_array() { return vert_arr_obj; }
	GLuint & get_e_buffer() { return elem_buff_obj; }

	const GLuint * get_shader_prog() { return &shade_prog; }
	const GLuint * set_shader_prog(const GLuint & prog) { shade_prog = prog; return &shade_prog; }

	const GLint & get_proj_loc() const { return r_project_mat4_loc; }
	void set_proj_loc(const GLint & matrix_loc) { r_project_mat4_loc = matrix_loc; }

	const GLint & get_model_loc() const { return r_model_mat4_loc; }
	void set_model_loc(const GLint & matrix_loc) { r_model_mat4_loc = matrix_loc; }

	const GLint & get_color_loc() const { return r_color_vec4_loc; }
	void set_color_loc(const GLint & matrix_loc) { r_color_vec4_loc = matrix_loc; }

	const glm::vec4 & get_color() const { return _color; }
	void set_color(const glm::vec4 & v4_color) { _color = v4_color; }
	void set_color(const glm::vec3 & v3_color) { _color = glm::vec4(v3_color, 1.0f); }

	const std::string getModelPath() const { return model_path; }
	const std::string getVShaderPath() const { return vertex_shader_path; }
	const std::string getFShaderPath() const { return fragment_shader_path; }
	const std::string getTexturePath() const { return texture_path; }

	GLint r_text_adj_w;
	GLint r_text_adj_h;

	GLint r_text_avail;
	GLint r_text_color;
	GLint r_text_unit;

protected:

	std::string model_path;
	std::string vertex_shader_path;
	std::string fragment_shader_path;
	std::string texture_path;

private:
	Model * current_model;
	Shader * current_shader;
	Texture * current_texture;

	GLint r_project_mat4_loc;
	GLint r_model_mat4_loc;
	GLint r_color_vec4_loc;

	glm::vec4 _color = glm::vec4(1.0f);

	GLuint vert_buff_obj = 0;
	GLuint vert_arr_obj = 0;
	GLuint elem_buff_obj = 0;
	GLuint shade_prog = 0;

};

#endif // !_RENDERCOMPONENT_H
