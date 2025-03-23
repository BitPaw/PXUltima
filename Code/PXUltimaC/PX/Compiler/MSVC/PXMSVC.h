#ifndef PXJavaIncluded
#define PXJavaIncluded

#include <PX/Media/PXResource.h>

#define PXCompilerTargetExecutable      (1<<0)
#define PXCompilerTargetLibraryStatic   (1<<1)
#define PXCompilerTargetLibraryDynamic  (1<<2)

typedef struct PXCompilerMSVC_
{
    char CompilerPathRoot[260];

    const char* CompilerPathInclude;
    const char* CompilerPathLibrarySearch;
    const char* CompilerPathLibraryFile;
}
PXCompilerMSVC;


PXPublic PXActionResult PXAPI PXCompilerInitializeMSVC(PXCompilerMSVC* const pxCompilerMSVC);
PXPublic PXActionResult PXAPI PXCompilerLinkMSVC(PXCompilerMSVC* const pxCompilerMSVC, int flags, const char* fileName, const char* resultName);

#endif