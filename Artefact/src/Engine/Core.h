#pragma once

#ifdef ARTEFACT_PLATFORM_WINDOWS
    #ifdef ARTEFACT_BUILD_DLL
        #define ARTEFACT_API __declspec(dllexport)
    #else
        #define ARTEFACT_API __declspec(dllimport)
    #endif
#else
    #error Artefact only supports Windows!
#endif