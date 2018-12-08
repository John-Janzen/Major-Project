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

extern Shader * load_shader(const std::string & vert_path, const std::string & frag_path);

extern Model * load_obj_file(const std::string & path);

extern Texture * load_texture(const std::string & path);

extern std::string openFileRead(const std::string & path);

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

#endif // !_FILELOADER_H
