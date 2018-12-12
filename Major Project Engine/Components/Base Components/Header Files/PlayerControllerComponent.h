#pragma once

#ifndef _PLAYERCONTROLLERCOMPONENT_H
#define _PLAYERCONTROLLERCOMPONENT_H

#include "BaseComponent.h"

enum CONTROL_TYPE {
	NULL_CONTROL,
	MOUSE_KEYBOARD,
	XBOX_CONTROLLER
};

class PlayerControllerComponent :
	public BaseComponent
{
public:
	PlayerControllerComponent() {}
	~PlayerControllerComponent(){}

	void Receive(int message) {}

	const CONTROL_TYPE & GetType() const { return current_type; }

private:
	CONTROL_TYPE current_type = MOUSE_KEYBOARD;
};

#endif // !_PLAYERCONTROLLERCOMPONENT_H
