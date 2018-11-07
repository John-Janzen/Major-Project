#include "QuadRenderComponent.h"

QuadRenderComponent::QuadRenderComponent() 
{
	FileLoader::Instance().obj_file_importer(model_to_load, this->get_model());
}

QuadRenderComponent::~QuadRenderComponent() {}
