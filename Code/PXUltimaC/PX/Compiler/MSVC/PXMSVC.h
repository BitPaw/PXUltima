#ifndef PXJavaIncludedd
#define PXJavaIncludedd

#include <PX/Engine/PXResource.h>

#define PXCompilerTargetExecutable      (1<<0)
#define PXCompilerTargetLibraryStatic   (1<<1)
#define PXCompilerTargetLibraryDynamic  (1<<2)

typedef struct PXCompilerMSVC_
{
    char CompilerPathRoot[260];

    const char* CompilerPathIncluded;
    const char* CompilerPathLibrarySearch;
    const char* CompilerPathLibraryFile;
}
PXCompilerMSVC;


PXPublic PXResult PXAPI PXCompilerInitializeMSVC(PXCompilerMSVC* const pxCompilerMSVC);
PXPublic PXResult PXAPI PXCompilerLinkMSVC(PXCompilerMSVC* const pxCompilerMSVC, int flags, const char* fileName, const char* resultName);

#endif