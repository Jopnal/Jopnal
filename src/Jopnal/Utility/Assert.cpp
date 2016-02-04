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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    void assertion(const bool expression, const std::string& file, unsigned int line, const std::string& message)
    {
        if (!expression)
        {
        #if defined(JOP_OS_WINDOWS)

            auto slashPos = file.find_last_of("/\\");

            std::string newStr = "File: " + file.substr(slashPos == std::string::npos ? 0 : slashPos + 1);
            newStr += "\nLine: " + std::to_string(line);
            newStr += "\n\n" + message;

            MessageBoxA(GetDesktopWindow(), newStr.c_str(), "Assertion failed!", MB_ICONERROR | MB_OK | MB_SETFOREGROUND);

        #endif

        #if defined(JOP_DEBUG_MODE) && defined(JOP_ENABLE_EXCEPTIONS)
            throw std::runtime_error(newStr);
        #else
            std::exit(EXIT_FAILURE);
        #endif
        }
    }
}