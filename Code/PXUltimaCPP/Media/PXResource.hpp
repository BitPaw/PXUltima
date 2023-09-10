#pragma once

#include <Media/PXResource.h>
#include "PXText.hpp"
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class Resource
	{
		public:
		template<typename T>
		PXDLLExport PX::ActionResult Load(T& resource, const PX::Text& filePath);

		template<typename T>
		PXDLLExport PX::ActionResult Load(T& resource, const char* const filePath);
	};
}