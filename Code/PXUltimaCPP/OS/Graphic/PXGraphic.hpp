#pragma once

#include <OS/Graphic/PXGraphic.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	enum class GraphicSystem
	{
		Invalid,
		OpenGL,
		DirectX,
		Vulcan
	};

	class Graphic : public PXGraphicContext
	{
		public:
		PXDLLExport Graphic();
		PXDLLExport ~Graphic();

		PXDLLExport PX::ActionResult Initialize(const PX::GraphicSystem graphicSystem);


		PXDLLExport PX::ActionResult ViewPortSet(const PXViewPort& pxViewPort);
		PXDLLExport PX::ActionResult ViewPortGet(PXViewPort& pxViewPort);
	};
}