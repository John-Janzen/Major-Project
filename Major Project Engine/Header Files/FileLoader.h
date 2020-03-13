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

/*
Load a shader from an external file
Shader will go to provided shader pointer
*/
extern bool LoadShaderFile(const std::string & vert_path, const std::string & frag_path, Shader * shader);

/*
Load a model from external file
Model will go to provided model pointer
*/
extern bool LoadOBJModelFile(const std::string & path, Model * model);

/*
Load a texture from external file
Texture will go to provided texture pointer
*/
extern bool LoadTextureFile(const std::string & path, Texture * texture);

/*
Keep threads at bay with this thread-safe io call function.
This should be called if opening a file.
*/
extern std::string OpenFileRead(const std::string & path);

/*
Compile the shader and by the type
Fragment or Vertex
*/
extern const GLuint CompileShader(const std::string & shader, const GLenum type);

/*
This reduces the number of vectors to just a single pointer object.
Mainly for the model loader.
*/
template<typename Out>
Out * MallocSpace(const std::vector<Out> & tooManyVecs)
{
	Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
	int j = 0;
	for (auto it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
		arr[j] = *it;
	return arr;
};

/*
Move number to the next power of two.
*/
extern GLuint PowerOfTwo(GLuint num);

#endif // !_FILELOADER_H
