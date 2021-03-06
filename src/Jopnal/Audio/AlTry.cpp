// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#include <Jopnal/Audio/AlTry.hpp>

#ifdef JOP_OPENAL_ERROR_CHECKS

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <AL/al.h>
    #include <AL/alc.h>

#endif

//////////////////////////////////////////////


namespace
{
    std::string ns_deviceErrors[] = ///< OpenAl errors as strings
    {
        "AL_NO_ERROR",
        "AL_INVALID_NAME",
        "AL_INVALID_ENUM",
        "AL_INVALID_VALUE",
        "AL_INVALID_OPERATION",
        "AL_OUT_OF_MEMORY"
    };
}

namespace jop { namespace detail
{
    void openAlCheck(const char* func, const char* file, const unsigned int line)
    {
    #if JOP_CONSOLE_VERBOSITY < 0

        func;
        file;
        line;

    #else

        const ALCenum error = alcGetError(alcGetContextsDevice(alcGetCurrentContext()));
        
        if (error != ALC_NO_ERROR)
            JOP_DEBUG_ERROR("Audio device error " << ns_deviceErrors[error] << " in function " << func << ", in file " << file << ", on line " << line);

    #endif
    }
}}

#endif