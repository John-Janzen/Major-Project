#pragma once
#include "BaseComponent.h"
#include "RenderExternals.h"

class RenderComponent : public BaseComponent
{
public:
	RenderComponent();
	~RenderComponent();

	void receive(int message) {};
private:
	int test = 5;
};

