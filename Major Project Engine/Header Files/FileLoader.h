#pragma once

#ifndef _FILELOADER_H
#define _FILELOADER_H

#include "RenderStorage.h"

#include <IL\il.h>
#include <IL\ilu.h>
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include <gl\GL.h>
#include <glm.hpp>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <iostream>
#include <algorithm>
#include <mutex>

extern bool LoadShaderFile(const std::string vert_path, const std::string frag_path, Shader * & shader);

extern bool LoadOBJModelFile(const std::string path, Model * & model);

extern bool LoadTextureFile(const std::string path, Texture * & texture);

extern std::string OpenFileRead(const std::string & path);

extern const GLuint CompileShader(const std::string shader, const GLenum type);

template<typename Out>
Out * MallocSpace(const std::vector<Out> & tooManyVecs)
{
	Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
	int j = 0;
	for (auto it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
		arr[j] = *it;
	return arr;
};

extern GLuint PowerOfTwo(GLuint num);

#endif // !_FILELOADER_H
