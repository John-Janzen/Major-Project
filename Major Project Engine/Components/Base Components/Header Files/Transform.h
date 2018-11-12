#pragma once
#include "BaseComponent.h"
class Transform :
	public BaseComponent
{
public:
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	~Transform();

	const glm::vec3 & get_pos() { return _position; }
	const glm::vec3 & get_rot() { return _rotation; }
	const glm::vec3 & get_scale() { return _scale; }

	void add_x_pos(const GLfloat & n) { _position.x += n; }
	void add_y_pos(const GLfloat & n) { _position.y += n; }
	void add_z_pos(const GLfloat & n) { _position.z += n; }

	void add_vec3_pos(const glm::vec3 & vec) { _position += vec; }

	void add_x_rot(const GLfloat & n) { _rotation.x += n; }
	void add_y_rot(const GLfloat & n) { _rotation.y += n; }
	void add_z_rot(const GLfloat & n) { _rotation.z += n; }

	void add_vec3_rot(const glm::vec3 & vec) { _rotation += vec; }

	void add_x_scale(const GLfloat & n) { _scale.x += n; }
	void add_y_scale(const GLfloat & n) { _scale.y += n; }
	void add_z_scale(const GLfloat & n) { _scale.z += n; }

	void add_vec3_scale(const glm::vec3 & vec) { _scale += vec; }

private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
};

inline Transform::Transform(glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3(1.0f))
	: _position(pos), _rotation(rot), _scale(scale)
{

}

inline Transform::~Transform() {}
