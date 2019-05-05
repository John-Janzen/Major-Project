#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

#include "Entity.h"
class Camera :
	public Entity
{
public:
	Camera(const std::string & name, int id) :Entity(name, id) {}
	~Camera() {}
};

#endif // !_CAMERA_H

