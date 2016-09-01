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

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Audio/SoundBuffer.hpp>

    #include <Jopnal/Audio/AlTry.hpp>
    #include <Jopnal/Audio/AudioReader.hpp>
    #include <Jopnal/Audio/SoundEffect.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Core/FileLoader.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <AL/al.h>
    #include <vector>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace
{
    ALint ns_format[] =
    {
        AL_FORMAT_MONO16,
        AL_FORMAT_STEREO16,
        alGetEnumValue("AL_FORMAT_QUAD16"),
        alGetEnumValue("AL_FORMAT_51CHN16"),
        alGetEnumValue("AL_FORMAT_61CHN16"),
        alGetEnumValue("AL_FORMAT_71CHN16")
    };
}

namespace jop
{
    SoundBuffer::SoundBuffer(const std::string& name)
        : Resource      (name),
          m_bufferId    (0),
          m_duration    (0.f),
          m_samples     (),
          m_sounds      ()
    {
        alTry(alGenBuffers(1, &m_bufferId));
    }

    SoundBuffer::SoundBuffer(const SoundBuffer& other, const std::string& newName)
        : Resource      (newName),
          m_bufferId    (0),
          m_duration    (other.m_duration),
          m_samples     (other.m_samples),
          m_sounds      (other.m_sounds)
    {
        alTry(alGenBuffers(1, &m_bufferId));
        
        m_info.format = other.m_info.format;
        m_info.channelCount = other.m_info.channelCount;
        m_info.sampleCount = other.m_info.sampleCount;
		m_info.firstSample = other.m_info.firstSample;
        m_info.sampleRate = other.m_info.sampleRate;
    }

    SoundBuffer::~SoundBuffer()
    {
        if (!m_sounds.empty() && !Engine::exiting())
        {
            for (unsigned int it = 0; it < m_sounds.size(); ++it)
                static_cast<SoundEffect*>(m_sounds[it])->setBuffer(getDefault());
        }

        alTry(alDeleteBuffers(1, &m_bufferId));
    }

    //////////////////////////////////////////////

    bool SoundBuffer::load(const std::string& path)
    {
        std::vector<uint8> buf;
        return FileLoader::readBinaryfile(path, buf) && load(buf.data(), buf.size());
    }

    //////////////////////////////////////////////

    bool SoundBuffer::load(const void* ptr, const uint32 size)
    {
        if (ptr && size && AudioReader::read(ptr, *this, size))
        {
            alTry(alBufferData(m_bufferId, ns_format[m_info.channelCount - 1], &m_samples.front(), static_cast<ALsizei>(m_info.sampleCount), m_info.sampleRate));
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    SoundBuffer& SoundBuffer::getDefault()
    {
        static WeakReference<SoundBuffer> defBuf;
        
        if (defBuf.expired())
        {
            defBuf = static_ref_cast<SoundBuffer>(ResourceManager::getEmpty<SoundBuffer>("jop_default_sound").getReference());

            JOP_ASSERT_EVAL(defBuf->load(jopr::defaultSound, sizeof(jopr::defaultSound)), "Failed to load default soundbuffer!");

            defBuf->setPersistence(0);
        }
        defBuf->refresh();

        return *defBuf;
    }

    //////////////////////////////////////////////

    void SoundBuffer::attachSound(SoundSource* sound) const
    {
        m_sounds.push_back(sound);
    }

    //////////////////////////////////////////////

    void SoundBuffer::detachSound(SoundSource* sound) const
    {
        m_sounds.erase(find(m_sounds.begin(), m_sounds.end(), sound));
    }

    //////////////////////////////////////////////

    SoundBuffer& SoundBuffer::refresh()
    {
        alTry(alBufferData(m_bufferId, ns_format[m_info.channelCount - 1], &m_samples.front(), static_cast<ALsizei>(m_info.sampleCount), m_info.sampleRate));

        return *this;
    }
}