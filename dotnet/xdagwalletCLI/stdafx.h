// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"


#ifdef RUNTIME_EXPORTS
#define NATIVE_LIB_EXPORT __declspec(dllexport)
#else
#define NATIVE_LIB_EXPORT __declspec(dllimport)
#endif



#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
