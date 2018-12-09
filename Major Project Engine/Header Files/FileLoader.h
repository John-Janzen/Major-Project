#pragma once

#ifndef _FILELOADER_H
#define _FILELOADER_H

#include "RenderExternals.h"

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
#include <chrono>
#include <iostream>
#include <algorithm>
#include <mutex>

extern Shader * load_shader(const std::string & vert_path, const std::string & frag_path);

extern Model * load_obj_file(const std::string & path);

extern Texture * load_texture(const std::string & path);

extern std::string openFileRead(const std::string & path);

extern const GLuint compileShader(const std::string shader, const GLenum type);

template<typename Out>
Out * mallocSpace(const std::vector<Out> & tooManyVecs)
{
	Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
	int j = 0;
	for (auto it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
		arr[j] = *it;
	return arr;
};

extern GLuint powerOfTwo(GLuint num);

#endif // !_FILELOADER_H
