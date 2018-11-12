#pragma once

#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H

#include "BaseComponent.h"

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>

class CameraComponent :
	public BaseComponent
{
public:
	CameraComponent();
	~CameraComponent();

	void set_project_look(const glm::mat4 & matrix) { projection_look_matrix = projection_matrix * (look_matrix * matrix); }
	GLfloat * get_project_value() { return glm::value_ptr(projection_look_matrix); }

private:
	GLfloat _fov = 60.0f;
	GLfloat _near = 0.1f, _far = 100.0f;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	glm::mat4 projection_look_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 look_matrix;
};

inline CameraComponent::CameraComponent() 
{
	projection_matrix = glm::perspective(glm::radians(_fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, _near, _far);
	look_matrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

inline CameraComponent::~CameraComponent() {}

#endif // !_CAMERACOMPONENT_H
