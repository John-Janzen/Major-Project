#pragma once
#include "BaseComponent.h"
#include "RenderExternals.h"
#include "FileLoader.h"

#include <memory>

class RenderComponent : public BaseComponent
{
public:
	RenderComponent();
	~RenderComponent();

	std::shared_ptr<Model> & get_model() { return current_model; }

	GLuint & get_vertex_buffer() { return vertex_buffer_obj; }
	GLuint & get_vertex_array() { return vertex_array_obj; }
	GLuint & get_element_buffer() { return element_buffer_obj; }
	GLuint & get_shader_program() { return shader_program; }

	void receive(int message) {};

private:
	int test = 5;
	std::shared_ptr<Model> current_model;

	GLuint vertex_buffer_obj = 0;
	GLuint vertex_array_obj = 0;
	GLuint element_buffer_obj = 0;
	GLuint shader_program = 0;



};

