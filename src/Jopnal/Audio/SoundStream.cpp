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
    SoundStream::SoundStream(Object& object, const std::string& ID)
        :Component(object, ID),
        m_stream(std::make_unique<sf::Music>())
    {}

    SoundStream::SoundStream(const SoundStream& other)
        : Component(other),
        m_stream()
    {
        other.clone()->clone(*m_stream);
    }
    //////////////////////////////////////////////

    SoundSource::~SoundSource()
    {}

    //////////////////////////////////////////////
    //////////////////////////////////////////////


    void SoundStream::setStream(const std::string& path)
    {
        if (m_stream->openFromFile(path))
        {
            m_path = path;
            m_stream->play();
        }
        else
            JOP_DEBUG_ERROR("Error in SoundStream::setStream: "<<path)
    }

    //////////////////////////////////////////////

    void SoundStream::setStream(const std::string& path, const float& x, const float& y, const float& z)
    {
        if (m_stream->openFromFile(path))
        {
            m_path = path;
            m_stream->setRelativeToListener(true);
            m_stream->setPosition(x, y, z);
            m_stream->play();
        }
        else
            JOP_DEBUG_ERROR("Error in SoundStream::setStream: " << path)
    }

    //////////////////////////////////////////////

    void SoundStream::toggleStreamLoop(bool loop)
    {
        m_stream->setLoop(loop);
    }

    //////////////////////////////////////////////

    void SoundStream::toggleStreamLoop(bool loop, const float& x, const float& y, const float& z)
    {
        m_stream->setPosition(x, y, z);
        m_stream->setLoop(loop);
    }

    //////////////////////////////////////////////

    void SoundStream::toggleStreamListener(bool toggle)
    {
        m_stream->setRelativeToListener(toggle);
    }

    //////////////////////////////////////////////

    void SoundStream::clone(sf::Music& copy)
    {
        copy.setPlayingOffset(m_stream->getPlayingOffset());
       
        if(bool listener = m_stream->isRelativeToListener())
        {
           copy.setRelativeToListener(listener);
           copy.setPosition( m_stream->getPosition());
        }

        copy.openFromFile(m_path);
        copy.play();
        if(m_stream->getStatus()!=sf::Music::Playing);
        copy.pause();
    }


}