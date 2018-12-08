#pragma once

#ifndef _FILELOADER_H
#define _FILELOADER_H

#include "RenderExternals.h"

#include <IL\il.h>
#include <IL\ilu.h>
#include <unordered_map>
#include <map>
#include <string>
#include <Windows.h>
#include <gl\GL.h>
#include <glm.hpp>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <chrono>
#include <iostream>

typedef std::unordered_map<std::string, Model*> ModelsStorage;
typedef std::unordered_map<std::string, Shader*> ShaderStorage;
typedef std::unordered_map<std::string, Texture*> TextureStorage;

class FileLoader
{
public:
	~FileLoader()
	{

	};

	static FileLoader& Instance()
	{
		static FileLoader inst;
		return inst;
	}

	void Init();

	void Close();

	void add_model(const std::string & path, Model * model) { _models.emplace(path, model); }

	void add_shader(const std::string & path, Shader * shader) { _shaders.emplace(path, shader); }

	void add_texture(const std::string & path, Texture * texture) { _textures.emplace(path, texture); }

	void load_shader(const std::string & path, const GLenum & type, Shader * & shader);

	void load_obj_file(const std::string & path, Model * & model_loc);

	void load_texture(const std::string & path, Texture * & model_loc);

private:
	FileLoader() {}

	template<typename Out>
	Out * mallocSpace(const std::vector<Out> & tooManyVecs)
	{
		Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
		int j = 0;
		for (auto it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
			arr[j] = *it;
		return arr;
	};

	GLuint powerOfTwo(GLuint num)
	{
		if (num != 0)
		{
			num--;
			num |= (num >> 1); //Or first 2 bits
			num |= (num >> 2); //Or next 2 bits
			num |= (num >> 4); //Or next 4 bits
			num |= (num >> 8); //Or next 8 bits
			num |= (num >> 16); //Or next 16 bits
			num++;
		}
		return num;
	}

	ModelsStorage _models;
	ShaderStorage _shaders;
	TextureStorage _textures;

	unsigned int model_count = 0;
	unsigned int texture_count = 0;

	
};

#endif // !_FILELOADER_H
