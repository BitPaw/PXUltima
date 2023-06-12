#include "PXMP3.hpp"

PX::ActionResult PX::MP3::Parse(PX::File& file)
{
	return (PX::ActionResult)PXMP3Parse(this, &file);
}