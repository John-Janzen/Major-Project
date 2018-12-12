#include "Quad.h"

Quad::Quad(const std::string & name, int id) : Entity(name, id) {}

Quad::~Quad() {}

void Quad::Load(ComponentManager * & c_manager)
{
	c_manager->AddComponent(this->GetID(), new QuadRenderComponent());
	c_manager->AddComponent(this->GetID(), new Transform(btVector3(btScalar(5.f), btScalar(0.f), btScalar(-5.f))));
}
