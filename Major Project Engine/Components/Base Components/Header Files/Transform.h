#pragma once
#include "BaseComponent.h"

#include "LinearMath\btTransform.h"

class Transform :
	public BaseComponent
{
public:
	Transform();
	Transform(btVector3 position);
	~Transform();

	btTransform _transform;
};

inline Transform::Transform()
{
	_transform = btTransform();
	_transform.setIdentity();
}

inline Transform::Transform(btVector3 position) 
{
	_transform = btTransform();
	_transform.getIdentity();
	_transform.setOrigin(position);
}

inline Transform::~Transform() {}
