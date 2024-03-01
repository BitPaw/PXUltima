#ifndef PXCompilerSettingsINCLUDE
#define PXCompilerSettingsINCLUDE

#include <OS/System/PXOSVersion.h>


#define _PX_FILEPATH_ __FILE__


#define _PX_FUNCTION_ __FUNCTION__
#define _PX_LINE_ __LINE__

#if defined(_DEBUG)
#define _PX_DEBUG 1u
#else 
#define _PX_DEBUG 0u
#endif

#endif