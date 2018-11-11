#include "Quad.h"

Quad::Quad(const std::string & name, int id) : Entity(name, id) {}

Quad::~Quad() {}

void Quad::Load(const std::unique_ptr<ComponentManager>& c_manager)
{
	c_manager->add_component(this->get_id(), std::make_shared<QuadRenderComponent>());
}
