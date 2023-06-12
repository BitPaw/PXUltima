#include "PXXML.hpp"

PX::ActionResult PX::XML::FileCompile(PX::File& inputStream, PX::File& outputStream)
{
    return (PX::ActionResult)PXXMLFileCompile(&inputStream, &outputStream);
}