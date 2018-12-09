#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

#include "Entity.h"
class Camera :
	public Entity
{
public:
	Camera(const std::string & name, int id);
	~Camera();

	void Load(ComponentManager * & c_manager);
};

inline Camera::Camera(const std::string & name, int id)
	: Entity(name, id)
{}

inline Camera::~Camera() {}

inline void Camera::Load(ComponentManager * & c_manager)
{
	c_manager->add_component(this->get_id(), new PlayerControllerComponent());
	c_manager->add_component(this->get_id(), new CameraComponent());
	this->_transform = new Transform(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(), glm::vec3(1.0f));
}

#endif // !_CAMERA_H

