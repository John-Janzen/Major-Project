#pragma once

#ifndef _RENDEREXTERNALS_H
#define _RENDEREXTERNALS_H

#include <GL\glew.h>
#include <cstddef>

struct Model
{
	const GLfloat * _vertices;
	const GLuint * _indices;
	GLsizei VSize = 0, ISize = 0;

	Model() { _vertices = nullptr; _indices = nullptr; }
	Model(const GLfloat * vert, const GLuint * ind, const GLsizei & is, const GLsizei & vs)
		: _vertices{ vert }, _indices{ ind }, VSize(vs), ISize(is) {};

	~Model() 
	{
		if (_vertices != nullptr) delete(_vertices); 
		if (_indices != nullptr) delete(_indices); 
	}

	void setVertices(const GLfloat * arr) { _vertices = arr; }
	void setIndices(const GLuint * arr) { _indices = arr; }

	const GLfloat * getVertices() { return _vertices; }
	const GLuint * getIndices() { return _indices; }
};

struct Texture
{
	const GLuint * _texture;
	GLuint imgWidth, imgHeight, texWidth, texHeight;
	GLuint TextureID;

	Texture(const GLuint * data, const GLuint & imgW, const GLuint & imgH, const GLuint & texW, const GLuint & texH)
		: _texture{ data }, imgWidth(imgW), imgHeight(imgH), texWidth(texW), texHeight(texH) {}

	~Texture() { if (TextureID != 0) glDeleteTextures(1, _texture); }

	void set_data(const GLuint * arr) { _texture = arr; }
	const GLuint * get_data() { return _texture; }
};

struct Shader
{
	const GLuint _shaderID_Vert, _shaderID_Frag;

	Shader(const GLuint & vert, const GLuint & frag) : _shaderID_Vert(vert), _shaderID_Frag(frag) {};

	~Shader() {}

	const GLuint getVertexShader() { return _shaderID_Vert; };
	const GLuint getFragmentShader() { return _shaderID_Frag; };
};

#endif // !_RENDEREXTERNALS
