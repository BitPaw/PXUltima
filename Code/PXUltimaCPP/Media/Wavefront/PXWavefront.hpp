#ifndef PXCPPWavefrontINCLUDE
#define PXCPPWavefrontINCLUDE

#include <Media/Wavefront/PXWavefront.h>
#include <Media/PXModel.hpp>

namespace PX
{
	// [.PXWavefront] Wavefront - 3D model format
	class Wavefront : public PXWavefront
	{
		public:
		Wavefront();
		~Wavefront();

		static PX::ActionResult FileCompile(PX::File& inputStream, PX::File& outputStream);
		static PX::ActionResult ParseToModel(PX::File& inputStream, PX::Model& model);
	};
}

#endif