#include "Quad.h"

Quad::Quad(const std::string & name, int id) : Entity(name, id) {}

Quad::~Quad() {}

void Quad::Load(ComponentManager * & c_manager)
{
	c_manager->add_component(this->get_id(), new QuadRenderComponent());
	this->_transform = btTransform();
	_transform.setIdentity();
	_transform.setOrigin(btVector3(btScalar(5.f), btScalar(0.f), btScalar(-5.f)));
}
