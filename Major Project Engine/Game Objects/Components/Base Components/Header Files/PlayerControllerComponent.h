#pragma once

#ifndef _PLAYERCONTROLLERCOMPONENT_H
#define _PLAYERCONTROLLERCOMPONENT_H

#include "BaseComponent.h"
#include "BulletDynamics\Character\btCharacterControllerInterface.h"

enum CONTROL_TYPE {
	NULL_CONTROL,
	MOUSE_KEYBOARD,
	XBOX_CONTROLLER
};

struct PlayerControllerComponent :
	public BaseComponent
{
	PlayerControllerComponent(const std::uint16_t & id) : BaseComponent(id) {}
	~PlayerControllerComponent(){}
	CONTROL_TYPE current_type = MOUSE_KEYBOARD;
};

#endif // !_PLAYERCONTROLLERCOMPONENT_H
