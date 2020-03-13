#pragma once

#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H

#include "BaseComponent.h"

#include <LinearMath\btVector3.h>

/*
The Camera Component that will create the projection matrix when taken to Render System.
These values are changed when player moves camera in first person.

Inherits the Base Component.
*/
struct CameraComponent :
	public BaseComponent
{
	CameraComponent(const std::uint16_t & id, btVector3 pos = btVector3(0.f, 0.f, 0.f), btVector3 aim = btVector3(0.f, 0.f, -1.f))
		: BaseComponent(id), 
		_aim(aim), 
		_eye(pos)
	{}
	~CameraComponent() {}

	btScalar _fov = 60.f;									// Field of View
	btScalar _near = 0.1f, _far = 1000.f;					// Distance from Camera
	btVector3 _eye, _aim, _up = btVector3(0.f, 1.f, 0.f);	// Essentials of Perspective Matrix
	glm::mat4 _projection;									// Projection Matrix
};

#endif // !_CAMERACOMPONENT_H
