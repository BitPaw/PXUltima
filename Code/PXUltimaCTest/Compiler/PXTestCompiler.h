#ifndef TestPXCompilerDEFINE
#define TestPXCompilerDEFINE

#include <PX/Media/PXResource.h>

PXPublic void PXAPI PXTestCompilerAll();

PXPrivate void PXAPI PXTestCompilerCToLanguages();
PXPrivate void PXAPI PXTestCompilerOBJ(char* filePath);
PXPrivate void PXAPI PXTestCompilerYAML();
PXPrivate void PXAPI PXTestCompilerYAML_2();
PXPrivate void PXAPI PXTestCompilerXML();

#endif