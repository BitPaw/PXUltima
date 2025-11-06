#pragma once

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


PXPublic PXResult PXAPI PXCompilerInitializeMSVC(PXCompilerMSVC PXREF pxCompilerMSVC);
PXPublic PXResult PXAPI PXCompilerLinkMSVC(PXCompilerMSVC PXREF pxCompilerMSVC, int flags, const char* fileName, const char* resultName);

#endif