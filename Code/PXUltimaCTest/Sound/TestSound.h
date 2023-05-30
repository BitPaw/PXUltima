#ifndef TestSoundINCLUDE
#define TestSoundINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXImage.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern void TestSoundAll();

    extern void TestSoundWAVWrite();

    extern void TestSoundWAV();
    extern void TestSoundMP3();

#ifdef __cplusplus
}
#endif

#endif