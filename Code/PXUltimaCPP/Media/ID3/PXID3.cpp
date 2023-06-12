#include "PXID3.hpp"

PX::ActionResult PX::ID3::Parse(PX::File& file)
{
    return (PX::ActionResult)PXID3Parse(this, &file);
}