#pragma once

#ifndef _PLAYERCONTROLLERCOMPONENT_H
#define _PLAYERCONTROLLERCOMPONENT_H

#include "BaseComponent.h"

/*
The Control Component just attaach this to control whatever object.

Inherits from Base Component.
*/
struct PlayerControllerComponent :
	public BaseComponent
{
	PlayerControllerComponent(const std::uint16_t & id) : BaseComponent(id) {}
	~PlayerControllerComponent(){}
};

#endif // !_PLAYERCONTROLLERCOMPONENT_H
