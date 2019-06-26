#pragma once
#include "BaseComponent.h"

#include "LinearMath\btTransform.h"

class Transform :
	public BaseComponent
{
public:
	Transform(const std::uint16_t & id);
	Transform(const std::uint16_t & id, const btVector3 & position);
	Transform(const std::uint16_t & id, const btQuaternion & quat, const btVector3 & pos);
	~Transform();

	btTransform _transform;
};

inline Transform::Transform(const std::uint16_t & id)
	: BaseComponent(id)
{
	_transform = btTransform();
	_transform.setIdentity();
}

inline Transform::Transform(const std::uint16_t & id, const btVector3 & position)
	: BaseComponent(id)
{
	_transform = btTransform();
	_transform.setIdentity();
	_transform.setOrigin(position);
}

inline Transform::Transform(const std::uint16_t & id, const btQuaternion & quat, const btVector3 & pos)
	: BaseComponent(id)
{
	_transform = btTransform(quat, pos);
}

inline Transform::~Transform() {}
