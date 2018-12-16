#pragma once
#include "BaseComponent.h"

#include "LinearMath\btTransform.h"

class Transform :
	public BaseComponent
{
public:
	Transform();
	Transform(const btVector3 position);
	~Transform();

	bool _physics = false, _input = true, _gamelogic = true;

	btTransform _transform;
};

inline Transform::Transform()
{
	_transform = btTransform();
	_transform.setIdentity();
}

inline Transform::Transform(const btVector3 position) 
{
	_transform = btTransform();
	_transform.setIdentity();
	_transform.setOrigin(position);
}

inline Transform::~Transform() {}
