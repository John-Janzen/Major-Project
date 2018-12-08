#pragma once

#ifndef _MODEL_H
#define _MODEL_H

#include <GL\glew.h>

class Model
{
public:
	Model();
	Model(const char * name, const GLfloat * vert, const GLuint * ind, const GLsizei & is, const GLsizei & vs);

	~Model();

	void setVertices(const GLfloat * arr) { _vertices = arr; }
	void setIndices(const GLuint * arr) { _indices = arr; }

	const char * getName() { return _name; }

	const GLfloat * getVertices() const { return _vertices; }
	const GLuint * getIndices() const { return _indices; }
	const GLsizei getVertexSize() const { return VSize; }
	const GLsizei getIndicesSize() const { return ISize; }

private:
	const char * _name;
	const GLfloat * _vertices;
	const GLuint * _indices;
	GLsizei VSize = 0, ISize = 0;
};



inline Model::Model()
{
	_vertices = nullptr;
	_indices = nullptr;
}

inline Model::Model(const char * name, const GLfloat * vert, const GLuint * ind, const GLsizei & is, const GLsizei & vs)
	: _name(name), _vertices{ vert }, _indices{ ind }, VSize(vs), ISize(is)
{
}


inline Model::~Model()
{
	if (_vertices != nullptr) delete _vertices;
	if (_indices != nullptr) delete _indices;
}

#endif // !_MODELS_H



