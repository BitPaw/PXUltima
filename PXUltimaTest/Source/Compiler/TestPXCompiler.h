#ifndef TestPXCompilerDEFINE
#define TestPXCompilerDEFINE

#include <stddef.h>

#include <OS/Error/PXActionResult.h>
#include <Format/Image.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern void TestPXCompilerAll();

    extern void TestPXCompilerOBJ();
    extern void TestPXCompilerYAML();
    extern void TestPXCompilerXML();

#ifdef __cplusplus
}
#endif

#endif
