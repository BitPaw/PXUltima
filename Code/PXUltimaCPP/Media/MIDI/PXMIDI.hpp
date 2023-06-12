#pragma once

#include <Media/MIDI/PXMIDI.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class MIDI : public PXMIDI
	{
		public:
		PX::ActionResult Parse(PX::File& file);
		PX::ActionResult Serialize(PX::File& file);
	};
}