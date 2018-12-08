#include "Quad.h"

Quad::Quad(const std::string & name, int id) : Entity(name, id) {}

Quad::~Quad() {}

void Quad::Load(ComponentManager * & c_manager)
{
	//c_manager->add_component(this->get_id(), std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f)));
	c_manager->add_component(this->get_id(), new QuadRenderComponent());
	this->_transform = new Transform(glm::vec3(2.0f, 0.0f, -5.0f), glm::vec3(), glm::vec3(1.0f));
}
