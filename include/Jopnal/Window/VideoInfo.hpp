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

#ifndef JOP_VIDEOINFO_HPP
#define JOP_VIDEOINFO_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API VideoInfo
    {
    public:
    
        /// \brief Get all the supported full screen resolutions
        ///
        /// \return Reference to an internal array with the resolutions
        ///
        static const std::vector<VideoInfo>& getSupportedModes();

        /// \brief Get the primary monitor desktop resolution
        ///
        /// \return Primary monitor desktop resolution
        /// 
        static VideoInfo getDesktopMode();

    public:

        glm::uvec2 resolution;
        unsigned int refreshRate;
    };
}

/// \class jop::VideoInfo
/// \ingroup window

#endif