#pragma once

#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H

#include "BaseComponent.h"

#include <LinearMath\btVector3.h>

struct CameraComponent :
	public BaseComponent
{
	CameraComponent(const std::uint16_t & id, btVector3 pos = btVector3(0.f, 0.f, 0.f), btVector3 aim = btVector3(0.f, 0.f, -1.f))
		: BaseComponent(id), 
		_aim(aim), 
		_eye(pos)
	{}
	~CameraComponent() {}

	btScalar _fov = 60.f;
	btScalar _near = 0.1f, _far = 1000.f;
	btVector3 _eye, _aim, _up = btVector3(0.f, 1.f, 0.f);
	glm::mat4 projection;
};

#endif // !_CAMERACOMPONENT_H
