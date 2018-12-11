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
	c_manager->AddComponent(this->GetID(), new PlayerControllerComponent());
	c_manager->AddComponent(this->GetID(), new CameraComponent());
	this->_transform = btTransform(btQuaternion(0.f, 0.f, 0.f), btVector3(btScalar(0.f), btScalar(0.f), btScalar(-10.f)));
}

#endif // !_CAMERA_H

