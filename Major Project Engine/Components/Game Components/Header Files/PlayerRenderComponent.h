#pragma once

#ifndef _PLAYERRENDERCOMPONENT_H
#define _PLAYERRENDERCOMPONENT_H

#include "RenderComponent.h"

class PlayerRenderComponent :
	public RenderComponent
{
public:
	PlayerRenderComponent();
	~PlayerRenderComponent();

private:
};

inline PlayerRenderComponent::PlayerRenderComponent()
	: RenderComponent(
		std::string("Assets/Models/cube.obj"), 
		std::string("Assets/Shaders/default_V_shader.glvs"), 
		std::string("Assets/Shaders/default_F_shader.glfs"))
{
}


inline PlayerRenderComponent::~PlayerRenderComponent() {}


#endif // !_PLAYERRENDERCOMPONENT_H
