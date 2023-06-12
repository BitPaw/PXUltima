#include "PXMIDI.hpp"

PX::ActionResult PX::MIDI::Parse(PX::File& file)
{
	return (PX::ActionResult)PXMIDIParse(this, &file);
}

PX::ActionResult PX::MIDI::Serialize(PX::File& file)
{
	return (PX::ActionResult)PXMIDISerialize(this, &file);
}