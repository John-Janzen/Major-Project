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
