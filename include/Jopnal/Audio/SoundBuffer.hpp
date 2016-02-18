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

#ifndef JOP_SoundBuffer_HPP
#define JOP_SoundBuffer_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <memory>
#include <string>
//////////////////////////////////////////////
namespace sf
{
    class SoundBuffer;
}

namespace jop
{
    class SoundBuffer : public Resource
    {
    public:

        /// \brief Constructor
        ///
        SoundBuffer(const std::string& name);

        /// \brief Virtual destructor
        ///
       ~SoundBuffer();

        /// \brief FileLoader to load new buffer from file
        ///
        /// \param path Name or path for wanted resource
        ///
        bool load(const std::string& path);

        const sf::SoundBuffer& getSfmlBuffer() const;

    private:
        std::unique_ptr<sf::SoundBuffer>m_soundBuf;     ///< Unique audio buffer
    };
}
#endif

/// \class SoundBuffer
/// \ingroup Audio
///
/// Sound data storage