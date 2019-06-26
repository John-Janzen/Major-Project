#pragma once

#ifndef _MODEL_H
#define _MODEL_H

#include <GL\glew.h>
#include <GL\GL.h>
#include <stdint.h>
#include <mutex>
#include <string.h>
#include <array>

/*
Storage for the Models of the game.
Takes vertices and indices.
*/
struct Model
{
	Model() 
		: _vertices{ nullptr }, _indices{ nullptr } {}

	Model(const std::string & name, const GLuint & id)
		: _name(name), _id(id), _vertices{ nullptr }, _indices{ nullptr } {}
	Model(const std::string name, const GLfloat * vert, const GLuint * ind, const GLsizei & is, const GLsizei & vs)
		: _name(name), _vertices{ vert }, _indices{ ind }, VSize(vs), ISize(is) { }

	~Model() { if (_vertices != nullptr) delete _vertices; if (_indices != nullptr) delete _indices; }

	bool operator==(const Model & other)
	{
		return this->_id == other._id;
	}

	bool operator!=(const Model & other)
	{
		return this->_id != other._id;
	}

	GLuint _id = 0;
	std::string _name;
	const GLfloat * _vertices;
	const GLuint * _indices;
	GLsizei VSize = 0, ISize = 0;

	GLuint vert_buff_obj = 0;
	GLuint elem_buff_obj = 0;
};

/*
Storage for the Textures
Has Image height, width and Tex height, width
As well as an ID when it is loaded into OpenGL
*/
struct Texture
{
	Texture(const std::string name, const GLuint & id)
		: _name(name), _id(id) {}
	Texture(const std::string name,  GLuint * data, const GLuint & imgW, const GLuint & imgH, const GLuint & texW, const GLuint & texH)
		: _name(name), _texture{ data }, imgWidth(imgW), imgHeight(imgH), texWidth(texW), texHeight(texH) {}
	~Texture() { if (TextureID != 0) glDeleteTextures(1, _texture); }

	bool operator==(const Texture & other)
	{
		return this->_id == other._id;
	}

	bool operator!=(const Texture & other)
	{
		return this->_id != other._id;
	}

	GLuint _id = 0;
	std::string _name;
	const GLuint * _texture;
	GLuint imgWidth, imgHeight, texWidth, texHeight;
	GLuint TextureID = 0;
};

/*
Storage for the Shaders
This is a combination of both fragment and vertex shaders.
All locations in the shader are to be put here.
*/
struct Shader
{
	Shader(const std::string name, const GLuint & id)
		: _name(name), _id(id) {}
	Shader(const std::string name, const GLuint & vert, const GLuint & frag) 
		: _name(name), _shaderID_Vert(vert), _shaderID_Frag(frag) {};
	~Shader() {}

	bool operator==(const Shader & other)
	{
		return this->_id == other._id;
	}

	bool operator!=(const Shader & other)
	{
		return this->_id != other._id;
	}

	GLuint _id = 0;
	std::string _name;
	GLuint _shaderID_Vert, _shaderID_Frag;
	GLuint shade_prog = 0;

	// Any data required in shader is to be put here
	GLint r_project_mat4_loc;	// Projection Matrix Location
	GLint r_model_mat4_loc;		// Model Matrix Location
	GLint r_color_vec4_loc;		// Color Vector Location

	GLint r_text_color;			// 
	GLint r_text_unit;
};

/*
Storage class for all the rendering obects.
*/
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

	/*
	Add new Item
	*/
	void AddItem(T * && item)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		_objects[num_items] = std::move(item);
		num_items++;
	}

	/*Get Item by name*/
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
		return nullptr;
	}

	/*Get Item by ID*/
	T * GetItem(const GLuint & id)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		for (auto mod : _objects)
		{
			if (mod != nullptr && mod->_id == id)
			{
				return mod;
			}
		}
		return nullptr;
	}

	/*Check if storage has item by name*/
	const bool & HasItem(const std::string name)
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

	/*Check if storage has item by name and get its id*/
	const bool HasItem(const std::string & name, GLuint & id)
	{
		std::lock_guard<std::mutex> lk(saftey_lock);
		for (auto mod : _objects)
		{
			if (mod != nullptr && name.compare(mod->_name) == 0)
			{
				id = mod->_id;
				return true;
			}
		}
		_objects[num_items] = new T(name, item_id++);
		id = _objects[num_items]->_id;
		num_items++;
		return false;
	}

private:

	static const GLuint MAX_ITEMS = 16;
	static std::size_t item_id, num_items;
	std::array<T*, MAX_ITEMS> _objects = { nullptr };
	std::mutex saftey_lock;
};

template<class T>
std::size_t Storage<T>::item_id = 1;

template<class T>
std::size_t Storage<T>::num_items = 0;

#endif // !_MODELS_H
