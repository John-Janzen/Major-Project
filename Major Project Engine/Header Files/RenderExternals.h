#pragma once

#ifndef _RENDEREXTERNALS_H
#define _RENDEREXTERNALS_H

#include <glew.h>
#include <cstddef>

struct Model
{
	const GLfloat * _vertices;
	const GLuint * _indices;
	std::size_t VSize = 0, ISize = 0;

	Model() { _vertices = nullptr; _indices = nullptr; }
	Model(const GLfloat * vert, const GLuint * ind, const GLsizei & is, const GLsizei & vs)
		: _vertices{ vert }, _indices{ ind }, VSize(vs), ISize(is) {};

	~Model() { if (_vertices != nullptr) delete(_vertices); if (_indices != nullptr) delete(_indices); }

	void setVertices(const GLfloat * arr) { _vertices = arr; }
	void setIndices(const GLuint * arr) { _indices = arr; }

	const GLfloat * getVertices() { return _vertices; }
	const GLuint * getIndices() { return _indices; }
};

struct Shader
{
	const GLuint _shaderID;
	GLenum _type;

	Shader(const GLuint & data, const GLenum & type) : _shaderID(data), _type(type) {};

	~Shader() {}

	const GLuint getShaderID() { return _shaderID; };
};

#endif // !_RENDEREXTERNALS
