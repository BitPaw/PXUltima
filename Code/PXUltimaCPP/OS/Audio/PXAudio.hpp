#pragma once

#include <OS/Audio/PXAudio.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class Audio : public PXAudio
	{
		public:
		Audio();
		~Audio();

		PX::ActionResult PXAPI Initialize(const PXAudioSystem pxAudioSystem);
		PX::ActionResult PXAPI Release();
	};
}