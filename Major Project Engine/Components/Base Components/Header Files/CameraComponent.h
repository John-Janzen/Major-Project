#pragma once

#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H

#include "BaseComponent.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>

class CameraComponent :
	public BaseComponent
{
public:
	CameraComponent(const std::uint16_t & id);
	~CameraComponent();

private:
	
};

inline CameraComponent::CameraComponent(const std::uint16_t & id)
	: BaseComponent(id)
{
}

inline CameraComponent::~CameraComponent() {}

#endif // !_CAMERACOMPONENT_H
