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

    #include <Jopnal/Audio/SoundStream.hpp>

    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(SoundStream)

    JOP_BIND_MEMBER_COMMAND((SoundStream& (SoundStream::*)(const bool reset))&SoundStream::play, "playStream");
    JOP_BIND_MEMBER_COMMAND(&SoundStream::pause, "pauseStream");
    JOP_BIND_MEMBER_COMMAND(&SoundStream::stop, "stopStream");
    JOP_BIND_MEMBER_COMMAND(&SoundStream::setOffset, "setStreamOffset");
    JOP_BIND_MEMBER_COMMAND(&SoundStream::setLoop, "setStreamLoop");

    JOP_END_COMMAND_HANDLER(SoundStream)
}

namespace jop
{
    SoundStream::SoundStream(Object& object)
        : SoundSource       (object, 0),
          m_path            (),
          m_loop            (false),
          m_isFileOpen      (false),
          m_playing         (false),
          m_lastBuffer      (false),
          m_updatebuffer    (false),
          m_updateBoth      (false),
          m_keepThread      (false),
          m_fileInstance    (),
          m_thread          ()
    {
        m_thread.setPriority(Thread::Priority::Lowest);
        alTry(alGenSources(1, &m_source));

        m_info.offset[0] = 0.01f;
        m_info.offset[1] = 0.01f;
    }

    SoundStream::SoundStream(const SoundStream& other, Object& newObj)
        : SoundSource       (other, newObj),
          m_path            (other.m_path),
          m_loop            (other.m_loop),
          m_bufferQueue     (other.m_bufferQueue),
          m_isFileOpen      (false),
          m_playing         (false),
          m_lastBuffer      (false),
          m_updatebuffer    (false),
          m_updateBoth      (false),
          m_keepThread      (true),
          m_fileInstance    (),
          m_thread          ()
    {
        m_thread = Thread(&SoundStream::readBuffer, this);
        m_thread.setPriority(Thread::Priority::Lower);
        alTry(alGenSources(1, &m_source));

        m_info.channelCount = other.m_info.channelCount;
        m_info.currentPos = other.m_info.currentPos;
        m_info.sampleCount = other.m_info.sampleCount;
        m_info.sampleRate = other.m_info.sampleRate;
        m_info.offset[0] = other.m_info.offset[0];
        m_info.offset[1] = other.m_info.offset[1];

        m_bufferQueue.push_back(new SoundBuffer(*other.m_bufferQueue.front(), other.m_bufferQueue.front()->getName()));
        m_bufferQueue.push_back(new SoundBuffer(*other.m_bufferQueue.back(), other.m_bufferQueue.front()->getName()));
    }

    SoundStream::~SoundStream()
    {
        m_keepThread = false;
        alTry(alSourceUnqueueBuffers(m_source, 1, &m_bufferQueue.front()->m_bufferId));
        alTry(alSourcei(m_source, AL_BUFFER, 0));
        alTry(alDeleteSources(1, &m_source));

        for (unsigned int it = 0; it <m_bufferQueue.size(); ++it)
            m_bufferQueue[it]->~SoundBuffer();
    }

    //////////////////////////////////////////////

    void SoundStream::update(const float deltaTime)
    {
        if (isSpeedOfSound())
        {
            SoundSource::update(deltaTime);

            if (m_delayCounter < 0.f && m_delayCounter > -0.5f)
            {
                playReset();
                m_delayCounter = -1.f;
            }
        }

        if (m_playing && getStatus() == Status::Stopped)
        {
            if (this->m_mutex.try_lock())
            {
                m_playing = false;

                m_info.offset[0] += m_info.offset[1];
                m_info.offset[1] += m_bufferQueue.front()->m_samples.size();

                updateOpenAl();
                playReset();
                this->m_mutex.unlock();
                updateBackBuffer();
            }
        }

        if (!m_playing && getStatus() == Status::Stopped)
        {
            if (this->m_mutex.try_lock())
            {
                m_updateBoth = true;
                m_lastBuffer = false;
                fromBegin();
            }
        }

        if (getStatus() == Status::Playing)
            m_playing = true;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setPath(const std::string& path)
    {
        while (!this->m_mutex.try_lock());
        
        m_bufferQueue.clear();

        m_path = path;

        if (!openFile())
        {
            m_bufferQueue.push_back(&SoundBuffer::getDefault());
            return *this;
        }

        std::vector<uint8> buf;
        buf.resize(JOP_AUDIO_STREAMING_BUFFER_SIZE);

        m_fileInstance.read(buf.data(), JOP_AUDIO_STREAMING_BUFFER_SIZE);

        m_bufferQueue.push_back(new SoundBuffer("Stream" + path));
        jop::AudioReader::read(buf.data(), *m_bufferQueue.front(), buf.size());

        m_info.firstSample =  buf.size() - m_bufferQueue.front()->m_samples.size();
        m_info.currentPos = m_info.firstSample;

        m_info.channelCount = m_bufferQueue.front()->m_info.channelCount;
        m_info.sampleCount = (m_fileInstance.getSize() - m_info.firstSample)*sizeof(int16)*m_info.channelCount;

        m_info.sampleRate = m_bufferQueue.front()->m_info.sampleRate;
        m_bufferQueue.front()->m_duration = static_cast<float>((m_bufferQueue.front()->m_info.sampleCount / m_bufferQueue.front()->m_info.sampleRate / m_bufferQueue.front()->m_info.channelCount));
        
        m_keepThread = true;
        m_thread = Thread(&SoundStream::readBuffer, this);
        m_thread.setPriority(Thread::Priority::Lower);

        m_bufferQueue.push_back(new SoundBuffer("Stream" + path));
        m_bufferQueue.back()->m_info.format = m_bufferQueue.front()->m_info.format;
        m_bufferQueue.back()->m_info.channelCount = m_bufferQueue.front()->m_info.channelCount;
        m_bufferQueue.back()->m_info.sampleRate = m_info.sampleRate;
        m_bufferQueue.back()->m_duration = m_bufferQueue.front()->m_duration;

        m_updateBoth = true;
        m_updatebuffer = true;
        this->m_mutex.unlock();
        while (m_updatebuffer);

        closeFile();

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::play()
    {
        if (getStatus() < Status::Playing)
            playReset();

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::playReset()
    {
        if (isSpeedOfSound() && m_bufferQueue.front()->m_info.channelCount == 1)
        {
            if (!m_calculateDelay)
            {
                m_calculateDelay = true;
                return *this;
            }
            else
                m_calculateDelay = false;
        }

        updateOpenAl();
        alTry(alSourcePlay(m_source));
        m_playing = true;

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::stop()
    {
        if (!m_isFileOpen)
            openFile();

        alTry(alSourcePlay(m_source));
        alTry(alSourceStop(m_source));
        alTry(alSourceUnqueueBuffers(m_source, 1, &m_bufferQueue.front()->m_bufferId));

        m_playing = false;

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::pause()
    {
        alTry(alSourcePause(m_source));
        m_playing = false;

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setOffset(const float time)
    {
        while(!this->m_mutex.try_lock());

        alTry(alSourceStop(m_source));
        alTry(alSourceUnqueueBuffers(m_source, 1, &m_bufferQueue.front()->m_bufferId));

        m_info.offset[0] = static_cast<float>(m_info.currentPos - m_info.firstSample);
        m_info.offset[1] = 0.f;
        m_info.currentPos = glm::clamp(static_cast<uint64>(m_info.firstSample + (time * m_info.sampleRate * m_info.channelCount)), m_info.firstSample, m_info.sampleCount);
        m_updateBoth = true;

        this->m_mutex.unlock();
        updateBackBuffer();

        return *this;
    }

    //////////////////////////////////////////////

    float SoundStream::getOffset() const
    {
        ALfloat secs = 0.f;
        alTry(alGetSourcef(m_source, AL_SEC_OFFSET, &secs));

        return secs + m_info.offset[0] / m_info.sampleRate / m_info.channelCount;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setLoop(const bool loop)
    {
        m_loop = loop;

        return *this;
    }

    //////////////////////////////////////////////

    bool SoundStream::isLooping() const
    {
        return m_loop;
    }

    //////////////////////////////////////////////

    void SoundStream::updateBackBuffer()
    {
        while (!this->m_mutex.try_lock());

        if (!m_isFileOpen)
            openFile();

        if (!m_lastBuffer)
        {
            m_updatebuffer = true;
            this->m_mutex.unlock();
        }
        else if (m_loop)
            fromBegin();

        else if (m_bufferQueue.front()->m_duration <= m_info.offset[0]/m_bufferQueue.front()->m_info.sampleRate / m_bufferQueue.front()->m_info.channelCount)
        {
            this->m_mutex.unlock();
            stop();
        }
        else
            this->m_mutex.unlock();
    }

    //////////////////////////////////////////////

    void SoundStream::updateOpenAl()
    {
        ALint processed;
        alTry(alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed));

        if (processed > 0)
        {
            alTry(alSourceUnqueueBuffers(m_source, 1, &m_bufferQueue.front()->m_bufferId));
            std::reverse(m_bufferQueue.begin(), m_bufferQueue.end());
        }
        else
            alTry(alSourceQueueBuffers(m_source, 1, &m_bufferQueue.front()->m_bufferId));
    }

    //////////////////////////////////////////////

    void SoundStream::fromBegin()
    {
        m_lastBuffer = false;

        m_info.offset[0] = 0.01f;
        m_info.offset[1] = 0.01f;
        m_info.currentPos = m_info.firstSample;

        this->m_mutex.unlock();
        updateBackBuffer();
    }

    //////////////////////////////////////////////

    void SoundStream::readBuffer()
    {
        while (m_keepThread)
        {
            if (m_updatebuffer)
            {
                if (this->m_mutex.try_lock())
                {

                    if (m_fileInstance != NULL)
                    {
                        m_bufferQueue.back()->m_samples.clear();

                        if (m_bufferQueue.back()->m_info.format != SoundBuffer::AudioFormat::wav)
                        {
                            if (static_cast<int64>(m_info.currentPos) >= (m_fileInstance.getSize()) && m_playing)
                                m_lastBuffer = true;

                            if (m_info.sampleCount < JOP_AUDIO_STREAMING_BUFFER_SIZE)
                                m_bufferQueue.back()->m_samples.reserve(static_cast<unsigned int>(m_info.sampleCount));
                            else
                                m_bufferQueue.back()->m_samples.reserve(JOP_AUDIO_STREAMING_BUFFER_SIZE);

                            AudioReader::stream(m_fileInstance, *m_bufferQueue.back(), m_info.currentPos);

                        }
                        else
                        {
                            if (m_info.sampleCount < JOP_AUDIO_STREAMING_BUFFER_SIZE)
                                m_bufferQueue.back()->m_samples.resize(static_cast<unsigned int>(m_info.sampleCount), NULL);
                            else
                                m_bufferQueue.back()->m_samples.resize(JOP_AUDIO_STREAMING_BUFFER_SIZE, NULL);

                            if (static_cast<int64>(m_info.currentPos) >= (m_fileInstance.getSize()) && m_playing)
                                m_lastBuffer = true;

                            m_fileInstance.seek(m_info.currentPos);
                            m_fileInstance.read(m_bufferQueue.back()->m_samples.data(), JOP_AUDIO_STREAMING_BUFFER_SIZE);
                            m_bufferQueue.back()->m_info.sampleCount = m_bufferQueue.back()->m_samples.size();
                            m_info.currentPos = m_fileInstance.tell();


                        }
                        m_bufferQueue.back()->refresh();

                        if (m_updateBoth)
                        {
                            std::reverse(m_bufferQueue.begin(), m_bufferQueue.end());
                            if (m_playing)
                                playReset();

                            m_updateBoth = false;
                        }
                        else
                            m_updatebuffer = false;
                    }
                }
                this->m_mutex.unlock();
            }
        }
    }

    //////////////////////////////////////////////

    bool SoundStream::openFile()
    {
        while (m_updatebuffer);
        if (!m_path.empty())
        {
            m_isFileOpen = m_fileInstance.open(m_path);

            return m_isFileOpen;
        }

        JOP_DEBUG_ERROR(getID() << " has no path");

        return false;
    }

    ////////////////////////////////////////////

    void SoundStream::closeFile()
    {
        while (m_updatebuffer);

        if (m_isFileOpen)
        {
            m_fileInstance.close();
            m_isFileOpen = false;

            return;
        }

        JOP_DEBUG_INFO(getID() << "'s file was already closed or was never opened");
    }
}