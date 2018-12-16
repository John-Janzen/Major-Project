#pragma once

#ifndef _RENDERCOMPONENT_H
#define _RENDERCOMPONENT_H

#include "BaseComponent.h"
#include "RenderStorage.h"

#include <memory>
#include <string>
#include <glm.hpp>

class RenderComponent : public BaseComponent
{
public:
	RenderComponent(const std::string model, const std::string v_shader, const std::string f_shader, const std::string texture);
	RenderComponent();
	~RenderComponent();

	void SetModel(Model * model) { current_model = model; }
	Model * GetModel() const { return current_model; }
	Model * & GetModelAdd() { return current_model; }

	void SetShader(Shader * shader) { current_shader = shader; }
	Shader * GetShader() const { return current_shader; }
	Shader * & GetShaderAdd() { return current_shader; }

	void SetTexture(Texture * texture) { current_texture = texture; }
	Texture * GetTexture() const { return current_texture; }
	Texture * & GetTextureAdd() { return current_texture; }

	//GLuint & GetVertexBuffer() { return vert_buff_obj; }
	GLuint & GetVertexArray() { return vert_arr_obj; }
	//GLuint & GetElementBuffer() { return elem_buff_obj; }

	const GLint & GetProjectionMatrixLoc() const { return r_project_mat4_loc; }
	void SetProjectionMatrixLoc(const GLint & matrix_loc) { r_project_mat4_loc = matrix_loc; }

	const GLint & GetModelMatrixLoc() const { return r_model_mat4_loc; }
	void SetModelMatrixLoc(const GLint & matrix_loc) { r_model_mat4_loc = matrix_loc; }

	const GLint & GetColorShaderLoc() const { return r_color_vec4_loc; }
	void SetColorShaderLoc(const GLint & matrix_loc) { r_color_vec4_loc = matrix_loc; }

	const glm::vec4 & GetColor() const { return _color; }
	void SetColor(const glm::vec4 & v4_color) { _color = v4_color; }
	void SetColor(const glm::vec3 & v3_color) { _color = glm::vec4(v3_color, 1.0f); }

	const std::string GetModelPath() const { return model_path; }
	const std::string GetVShaderPath() const { return vertex_shader_path; }
	const std::string GetFShaderPath() const { return fragment_shader_path; }
	const std::string GetTexturePath() const { return texture_path; }

	const std::string GetShaderPath() const { return std::string(vertex_shader_path + fragment_shader_path); }

	GLint r_text_adj_w;
	GLint r_text_adj_h;

	GLint r_text_avail;
	GLint r_text_color;
	GLint r_text_unit;

	GLuint vert_arr_obj = 0;

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
};

#endif // !_RENDERCOMPONENT_H
