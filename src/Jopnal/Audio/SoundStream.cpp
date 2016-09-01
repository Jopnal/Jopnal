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

    #include <Jopnal/Audio/AlTry.hpp>
    #include <Jopnal/Audio/AudioReader.hpp>
    #include <Jopnal/Audio/SoundBuffer.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <Jopnal/STL.hpp>
    #include <glm/common.hpp>
    #include <AL/al.h>
    #include <algorithm>

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
    SoundStream::ParsedStreamingInfo::ParsedStreamingInfo()
        : sampleCount   (0),
          currentPos    (0),
          offset        (0.f)
    {}

    ////////////////////////////////////////////

	SoundStream::SoundStream(Object& object)
		: SoundSource         (object, 0),
		  m_path              (),
		  m_loop              (false),
		  m_isFileOpen        (false),
		  m_keepThread        (false),
		  m_deltaOffset       (0.f),
		  m_inputOffset       (-1.f),
		  m_rawOffset         (0.f),
		  m_fileInstance      (),
		  m_streamThread      ()
	{
		m_streamThread.setPriority(Thread::Priority::Lowest);
		alTry(alGenSources(1, &m_source));

		m_info.offset = 0.f;
	}

	SoundStream::SoundStream(const SoundStream& other, Object& newObj)
		: SoundSource         (other, newObj),
		  m_path              (other.m_path),
		  m_loop              (other.m_loop),
		  m_bufferQueue       (),
		  m_isFileOpen        (false),
		  m_keepThread        (true),
		  m_deltaOffset       (0.f),
		  m_inputOffset       (-1.f),
		  m_rawOffset         (0.f),
		  m_fileInstance      (),
		  m_streamThread      ()
	{
		m_streamThread = Thread(&SoundStream::updateStream, this);
		m_streamThread.setPriority(Thread::Priority::Lower);
		alTry(alGenSources(1, &m_source));

		m_info.currentPos = other.m_info.currentPos;
		m_info.sampleCount = other.m_info.sampleCount;

		m_bufferQueue.push_back(std::make_unique<SoundBuffer>(*other.m_bufferQueue.front(), other.m_bufferQueue.front()->getName()));
		m_bufferQueue.push_back(std::make_unique<SoundBuffer>(*other.m_bufferQueue.back(), other.m_bufferQueue.front()->getName()));
	}

	SoundStream::~SoundStream()
	{
		m_keepThread = false;
		alTry(alSourceUnqueueBuffers(m_source, 1, &m_bufferQueue.front()->m_bufferId));
		alTry(alSourcei(m_source, AL_BUFFER, 0));
		alTry(alDeleteSources(1, &m_source));
	}

	////////////////////////////////////////////

	void SoundStream::update(const float)
	{
		if (m_mutex.try_lock())
		{
			float secs = 0.f;
			alTry(alGetSourcef(m_source, AL_SEC_OFFSET, &secs));

			if (getStatus() == Status::Playing)
			{
				if (m_deltaOffset < secs)
					m_deltaOffset += (secs - m_deltaOffset);
				else
					m_deltaOffset = 0.f;
			}

			m_info.offset = m_info.offset + (m_deltaOffset / 100.f);
			m_mutex.unlock();
		}
	}

	////////////////////////////////////////////

	bool SoundStream::setPath(const std::string& path)
	{

		m_bufferQueue.clear();
		m_path = path;

		if (!openFile())
			return false;

		std::vector<uint8> buf;
		buf.resize(JOP_AUDIO_STREAMING_BUFFER_SIZE);

		m_fileInstance.read(buf.data(), JOP_AUDIO_STREAMING_BUFFER_SIZE);

		m_bufferQueue.push_back(std::make_unique<SoundBuffer>("1_stream_" + path));
		AudioReader::read(buf.data(), *m_bufferQueue.front(), buf.size());

		m_info.currentPos = m_bufferQueue.back()->m_info.firstSample;
		m_bufferQueue.back()->m_duration = static_cast<float>((m_bufferQueue.back()->m_info.sampleCount / m_bufferQueue.back()->m_info.sampleRate / m_bufferQueue.back()->m_info.channelCount));

		fillBuffer();

		if (m_bufferQueue.back()->m_info.sampleCount >= JOP_AUDIO_STREAMING_BUFFER_SIZE)
		{
			m_bufferQueue.push_back(std::make_unique<SoundBuffer>("2_stream_" + path));
			m_bufferQueue.back()->m_info.format = m_bufferQueue.front()->m_info.format;
			m_bufferQueue.back()->m_info.firstSample = m_bufferQueue.front()->m_info.firstSample;
			m_bufferQueue.back()->m_info.channelCount = m_bufferQueue.front()->m_info.channelCount;
			m_bufferQueue.back()->m_info.sampleRate = m_bufferQueue.front()->m_info.sampleRate;
			m_bufferQueue.back()->m_duration = m_bufferQueue.front()->m_duration;

			fillBuffer();
		}

		m_keepThread = true;
		m_streamThread = Thread(&SoundStream::updateStream, this);
		m_streamThread.setPriority(Thread::Priority::Lower);

		return true;
	}

	////////////////////////////////////////////

	SoundStream& SoundStream::play()
	{
		if (getStatus() < Status::Playing)
			playReset();

		return *this;
	}

	////////////////////////////////////////////

	SoundStream& SoundStream::playReset()
	{
		if (m_path.empty())
		{
			JOP_DEBUG_INFO(getID() << " has no filepath");
			return *this;
		}

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
		alTry(alSourcePlay(m_source));

		return *this;
	}

	////////////////////////////////////////////

	SoundStream& SoundStream::stop()
	{
		if (m_path.empty())
		{
			JOP_DEBUG_INFO(getID() << " has no filepath");
			return *this;
		}

		alTry(alSourceStop(m_source));
		setOffset(0.f);

		return *this;
	}

	////////////////////////////////////////////

	SoundStream& SoundStream::pause()
	{
		if (m_path.empty())
		{
			JOP_DEBUG_INFO(getID() << " has no filepath");
			return *this;
		}

		alTry(alSourcePause(m_source));

		return *this;
	}

	////////////////////////////////////////////

	SoundStream& SoundStream::setOffset(const float time)
	{
		if (m_path.empty())
		{
			JOP_DEBUG_INFO(getID() << " has no filepath");
			return *this;
		}

		m_inputOffset = glm::max(0.f, time);

		return *this;
	}

	////////////////////////////////////////////

	float SoundStream::getOffset()
	{
		return glm::clamp(static_cast<float>(m_info.offset), 0.f, m_bufferQueue.back()->m_duration);
	}

	////////////////////////////////////////////

	SoundStream& SoundStream::setLoop(const bool loop)
	{
		m_loop = loop;

		return *this;
	}

	////////////////////////////////////////////

	bool SoundStream::isLooping() const
	{
		return m_loop;
	}

	////////////////////////////////////////////

	void SoundStream::updateStream()
	{
		bool buffering = m_bufferQueue.size()>1;

		while (m_keepThread)
		{
			if (getStatus() != Status::Stopped)
			{
				ALint processed;
				alTry(alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed));

				float secs = 0.f;
				alTry(alGetSourcef(m_source, AL_SEC_OFFSET, &secs));

				if (processed > 0)
				{
					if (m_mutex.try_lock())
					{
						if (buffering)
						{
							std::reverse(m_bufferQueue.begin(), m_bufferQueue.end());
							fillBuffer();
						}
						else if (m_loop)
							alTry(alSourcePlay(m_source));

						m_mutex.unlock();
					}
				}
			}
			else
			{
				if (m_isFileOpen)
					closeFile();
			}

			if (m_inputOffset >= 0.f)
			{
				if (m_mutex.try_lock())
				{
					changeOffset();
					m_mutex.unlock();
				}
			}

		}
		if (m_isFileOpen)
			closeFile();
	}

	////////////////////////////////////////////

	void SoundStream::fillBuffer()
	{
		if (!m_isFileOpen)
			openFile();

		if (m_fileInstance.isValid())
		{
			alTry(alSourceUnqueueBuffers(m_source, 1, &m_bufferQueue.back()->m_bufferId));

			if (m_info.currentPos == static_cast<uint64>(m_fileInstance.getSize()) && m_loop)
			{
				m_info.offset = 0.f;
				m_deltaOffset = 0.f;
				m_rawOffset = 0.f;
				m_info.currentPos = m_bufferQueue.back()->m_info.firstSample;
				m_fileInstance.seek(m_info.currentPos);
			}
			else if (m_info.currentPos == static_cast<uint64>(m_fileInstance.getSize()))
				return;

			m_bufferQueue.back()->m_samples.clear();

			if (m_bufferQueue.back()->m_info.format != SoundBuffer::AudioFormat::wav)
			{
			    m_bufferQueue.back()->m_samples.reserve(JOP_AUDIO_STREAMING_BUFFER_SIZE);
				AudioReader::stream(m_fileInstance, *m_bufferQueue.back(), &m_rawOffset);
				m_bufferQueue.back()->m_samples.shrink_to_fit();
				m_bufferQueue.back()->m_info.sampleCount = m_bufferQueue.back()->m_samples.size();
				m_info.currentPos = m_fileInstance.tell();
			}
			else
			{
				int64 bufSize = m_fileInstance.getSize() - m_info.currentPos;
				if (bufSize > JOP_AUDIO_STREAMING_BUFFER_SIZE)
					bufSize = JOP_AUDIO_STREAMING_BUFFER_SIZE;

				else if (bufSize<0)
				{
					m_inputOffset=0.f;
					changeOffset();
					return;
				}

				m_bufferQueue.back()->m_samples.resize(static_cast<std::size_t>(bufSize), 0);

				m_fileInstance.seek(m_info.currentPos);
				m_fileInstance.read(m_bufferQueue.back()->m_samples.data(), bufSize);
				m_bufferQueue.back()->m_info.sampleCount = m_bufferQueue.back()->m_samples.size();
				m_info.currentPos = m_fileInstance.tell();
			}

			m_bufferQueue.back()->refresh();
			alTry(alSourceQueueBuffers(m_source, 1, &m_bufferQueue.back()->m_bufferId));
		}
	}

	////////////////////////////////////////////

	void SoundStream::changeOffset()
	{
		m_info.offset = glm::max(0.f,static_cast<float>(m_inputOffset));
		m_rawOffset = static_cast<float>(m_info.offset);
	    m_info.currentPos = static_cast<uint64>((m_info.offset * m_bufferQueue.back()->m_info.channelCount * m_bufferQueue.back()->m_info.sampleRate) + m_bufferQueue.back()->m_info.firstSample);

		bool reset = getStatus() == Status::Playing;

		alTry(alSourceStop(m_source));

		fillBuffer();
		std::reverse(m_bufferQueue.begin(), m_bufferQueue.end());
		fillBuffer();

		if (reset)
			alTry(alSourcePlay(m_source));

		m_deltaOffset = 0.f;
		m_inputOffset = -1.f;
	}

	////////////////////////////////////////////

    bool SoundStream::openFile()
    {
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
        if (m_isFileOpen)
        {
            m_fileInstance.close();
            m_isFileOpen = false;

            return;
        }

        JOP_DEBUG_INFO(getID() << "'s file is not open");
    }
}