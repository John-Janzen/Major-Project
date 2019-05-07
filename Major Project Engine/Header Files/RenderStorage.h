#pragma once

#ifndef _MODEL_H
#define _MODEL_H

#include "Job.h"

#include <GL\glew.h>
#include <stdint.h>
#include <mutex>
#include <string.h>

struct Model
{
	Model() 
		: _vertices{ nullptr }, _indices{ nullptr } {}

	Model(const std::string name) 
		: _name(name), _vertices{ nullptr }, _indices{ nullptr } {}
	Model(const std::string name, const GLfloat * vert, const GLuint * ind, const GLsizei & is, const GLsizei & vs)
		: _name(name), _vertices{ vert }, _indices{ ind }, VSize(vs), ISize(is) { }

	~Model() { if (_vertices != nullptr) delete _vertices; if (_indices != nullptr) delete _indices; }

	JOB_RETURN CheckDoneLoad(void * ptr) { return (_vertices != nullptr && _indices != nullptr) ? JOB_COMPLETED : JOB_RETRY; }

	std::string _name;
	const GLfloat * _vertices;
	const GLuint * _indices;
	GLsizei VSize = 0, ISize = 0;

	GLuint vert_buff_obj = 0;
	GLuint elem_buff_obj = 0;
};

struct Texture
{
	Texture(const std::string name,  GLuint * data, const GLuint & imgW, const GLuint & imgH, const GLuint & texW, const GLuint & texH)
		: _name(name), _texture{ data }, imgWidth(imgW), imgHeight(imgH), texWidth(texW), texHeight(texH) {}
	Texture(const std::string name) 
		: _name(name) {}
	~Texture() { if (TextureID != 0) glDeleteTextures(1, _texture); }

	JOB_RETURN CheckDoneLoad(void * ptr) 
	{
		return (_texture != nullptr && TextureID != 0) ? JOB_COMPLETED : JOB_RETRY; 
	}

	std::string _name;
	const GLuint * _texture;
	GLuint imgWidth, imgHeight, texWidth, texHeight;
	GLuint TextureID = 0;
};

struct Shader
{
	Shader(const std::string name, const GLuint & vert, const GLuint & frag) 
		: _name(name), _shaderID_Vert(vert), _shaderID_Frag(frag) {};
	Shader(const std::string name) 
		: _name(name) {}
	~Shader() {}

	JOB_RETURN CheckDoneLoad(void * ptr) { return (_shaderID_Vert != 0 && _shaderID_Frag != 0) ? JOB_COMPLETED : JOB_RETRY; }

	std::string _name;
	GLuint _shaderID_Vert, _shaderID_Frag;
	GLuint shade_prog = 0;

	// Any data required in shader is to be put here
	GLint r_project_mat4_loc;
	GLint r_model_mat4_loc;
	GLint r_color_vec4_loc;

	GLint r_text_adj_w;
	GLint r_text_adj_h;

	GLint r_text_avail;
	GLint r_text_color;
	GLint r_text_unit;
};

enum LOAD {
	CURRENT_LOAD,
	WAIT_LOAD,
	DONE_LOAD
};

template <class T>
class Storage
{
public:
	
	Storage() {}
	~Storage() 
	{
		for (int i = 0; i < MAX_ITEMS; i++)
		{
			if (_objects[i] != nullptr)
			{
				delete _objects[i];
				_objects[i] = nullptr;
			}
		}
	}

	void AddItem(T * model)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		_objects[num_items] = model;
		num_items++;
	}

	T * GetItem(const std::string name)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		for (auto mod : _objects)
		{
			if (mod != nullptr && name.compare(mod->_name) == 0)
			{
				return mod;
			}
		}
	}

	bool HasItem(const std::string name)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		for (auto mod : _objects)
		{
			if (mod != nullptr && name.compare(mod->_name) == 0)
			{
				return true;
			}
		}
		return false;
	}

	LOAD HasItem(const std::string & name, T * & model)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		for (auto mod : _objects)
		{
			if (mod != nullptr && name.compare(mod->_name) == 0)
			{
				model = mod;
				if (mod->CheckDoneLoad(nullptr) == JOB_COMPLETED)
					return DONE_LOAD;
				else
					return WAIT_LOAD;
			}
		}
		_objects[num_items] = new T(name);
		model = _objects[num_items];
		num_items++;
		return CURRENT_LOAD;
	}

private:

	static const uint16_t MAX_ITEMS = 16;
	size_t num_items = 0;
	std::array<T*, MAX_ITEMS> _objects = { nullptr };
	std::mutex saftey_lock;
};

#endif // !_MODELS_H
