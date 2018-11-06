#pragma once
#include "BaseComponent.h"
#include "RenderExternals.h"

#include <memory>

class RenderComponent : public BaseComponent
{
public:
	RenderComponent();
	~RenderComponent();

	std::shared_ptr<Model> & get_model()
	{
		return current_model;
	}

	void receive(int message) {};
private:
	int test = 5;
	std::shared_ptr<Model> current_model;
};

