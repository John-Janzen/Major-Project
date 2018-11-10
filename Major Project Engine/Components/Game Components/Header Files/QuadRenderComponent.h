#pragma once

#ifndef _QUADRENDERCOMPONENT_H
#define _QUADRENDERCOMPONENT_H

#include "RenderComponent.h"

#include <memory>
#include <string>

static const std::string model_to_load = "Assets/quad.obj";

class QuadRenderComponent : public RenderComponent
{
public:
	QuadRenderComponent();
	~QuadRenderComponent();

private:

};

#endif // !_QUADRENDERCOMPONENT_H
