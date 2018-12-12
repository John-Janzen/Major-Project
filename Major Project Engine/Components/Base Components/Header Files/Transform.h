#pragma once
#include "BaseComponent.h"

#include "LinearMath\btTransform.h"

class Transform :
	public BaseComponent
{
public:
	Transform();
	~Transform();

	void Receive(int message)
	{

	}

	btTransform _transform;
};

Transform::Transform() {}

Transform::~Transform() {}
