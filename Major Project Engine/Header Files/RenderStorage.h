#pragma once

#ifndef _MODEL_H
#define _MODEL_H

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

	std::string _name;
	const GLfloat * _vertices;
	const GLuint * _indices;
	GLsizei VSize = 0, ISize = 0;
};

struct Texture
{
	Texture(const std::string name,  GLuint * data, const GLuint & imgW, const GLuint & imgH, const GLuint & texW, const GLuint & texH)
		: _name(name), _texture{ data }, imgWidth(imgW), imgHeight(imgH), texWidth(texW), texHeight(texH) {}
	Texture(const std::string name) 
		: _name(name) {}
	~Texture() { if (TextureID != 0) glDeleteTextures(1, _texture); }

	std::string _name;
	const GLuint * _texture;
	GLuint imgWidth, imgHeight, texWidth, texHeight;
	GLuint TextureID;
};

struct Shader
{
	Shader(const std::string name, const GLuint & vert, const GLuint & frag) 
		: _name(name), _shaderID_Vert(vert), _shaderID_Frag(frag) {};
	Shader(const std::string name) 
		: _name(name) {}
	~Shader() {}

	std::string _name;
	GLuint _shaderID_Vert, _shaderID_Frag;
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

	bool HasItem(const std::string & name, T * & model)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		for (auto mod : _objects)
		{
			if (mod != nullptr && name.compare(mod->_name) == 0)
			{
				model = mod;
				return true;
			}
		}
		_objects[num_items] = new T(name);
		model = _objects[num_items];
		num_items++;
		return false;
	}

private:

	static const uint16_t MAX_ITEMS = 16;
	size_t num_items = 0;
	T * _objects[MAX_ITEMS] = { nullptr };
	std::mutex saftey_lock;
};

#endif // !_MODELS_H
