#include "PXResource.hpp"

template<typename T>
PX::ActionResult PX::Resource::Load(T& resource, const PX::Text& filePath)
{
	return (PX::ActionResult)PXResourceLoad(&resource, filePath);
}

template<typename T>
PX::ActionResult PX::Resource::Load(T& resource, const char* const filePath)
{
	return (PX::ActionResult)PXResourceLoadA(&resource, filePath);
}