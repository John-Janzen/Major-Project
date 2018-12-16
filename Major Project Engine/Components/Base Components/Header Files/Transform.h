#pragma once
#include "BaseComponent.h"

#include "LinearMath\btTransform.h"

class Transform :
	public BaseComponent
{
public:
	Transform();
	Transform(const btVector3 & position);
	Transform(const btQuaternion & quat, const btVector3 & pos);
	~Transform();

	bool _physics = false, _input = true, _gamelogic = true;

	btTransform _transform;
};

inline Transform::Transform()
{
	_transform = btTransform();
	_transform.setIdentity();
}

inline Transform::Transform(const btVector3 & position) 
{
	_transform = btTransform();
	_transform.setIdentity();
	_transform.setOrigin(position);
}

inline Transform::Transform(const btQuaternion & quat, const btVector3 & pos)
{
	_transform = btTransform(quat, pos);
}

inline Transform::~Transform() {}
