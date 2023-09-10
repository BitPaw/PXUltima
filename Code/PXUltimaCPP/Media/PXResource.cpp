#include "PXResource.hpp"

template<typename T>
PXActionResult PX::Resource::Load(T& resource, const PX::Text& filePath)
{
	return PXResourceLoad(&resource, filePath);
}

template<typename T>
PXActionResult PX::Resource::Load(T& resource, const char* const filePath)
{
	return PXResourceLoadA(&resource, filePath);
}