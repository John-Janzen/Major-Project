#pragma once
#include "BaseComponent.h"

#include "LinearMath\btTransform.h"

/*
The Transform Component holds Bullets transform object.
This class was a toss up between in entity or a seperate component.
Went with seperate component.

Inherits Base Component
*/
struct Transform :
	public BaseComponent
{
	/*
	Contructor
	Empty Transform
	*/
	Transform(const std::uint16_t & id)
		: BaseComponent(id)
	{
		_transform = btTransform();
		_transform.setIdentity();
	}

	/*
	Constructor
	Tranform with only position
	*/
	Transform(const std::uint16_t & id, const btVector3 & position) 
		: BaseComponent(id)
	{
		_transform = btTransform();
		_transform.setIdentity();
		_transform.setOrigin(position);
	}

	/*
	Constructor
	Transform with position and rotation
	*/
	Transform(const std::uint16_t & id, const btQuaternion & quat, const btVector3 & pos)
		: BaseComponent(id)
	{
		_transform = btTransform(quat, pos);
	}

	~Transform() {}

	btTransform _transform;		// Bullet Provided Transform
};
