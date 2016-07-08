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

    #include <Jopnal/Audio/AudioReader.hpp>

    #include <Jopnal/Audio/SoundBuffer.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

#define DR_WAV_IMPLEMENTATION
#include <Mackron/dr_wav.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

//////////////////////////////////////////////


namespace jop
{
    class InputStream
    {
    public:
        InputStream(const void* buffer, int64 size);
        InputStream(FileLoader& fileInstance);

        void open(const void* data, std::size_t sizeInBytes);
        int64 read(void* data, int64 size);
        int64 seek(int64 position);
        int64 tell();
        int64 getSize();

    private:

        FileLoader* m_fileInstance;
        const char* m_data;
        int64 m_size;
        int64 m_offset;
    };
}

namespace
{
    size_t read(void* ptr, size_t size, size_t nmemb, void* data)
    {
        jop::InputStream* stream = static_cast<jop::InputStream*>(data);
        return static_cast<std::size_t>(stream->read(ptr, size * nmemb));
    }

    int seek(void* data, ogg_int64_t offset, int whence)
    {
        jop::InputStream* stream = static_cast<jop::InputStream*>(data);
        switch (whence)
        {
        case SEEK_SET:
            break;
        case SEEK_CUR:
            offset += stream->tell();
            break;
        case SEEK_END:
            offset = stream->getSize() - offset;
        }
        return static_cast<int>(stream->seek(offset));
    }

    long tell(void* data)
    {
        jop::InputStream* stream = static_cast<jop::InputStream*>(data);
        return static_cast<long>(stream->tell());
    }

    static ov_callbacks callbacks = { &read, &seek, NULL, &tell };
}

namespace jop
{
    InputStream::InputStream(const void* buffer, int64 size)
        : m_fileInstance    (NULL),
          m_data            (static_cast<const char*>(buffer)),
          m_size            (size),
          m_offset          (0)
    {}

    InputStream::InputStream(FileLoader& fileInstance)
        : m_fileInstance    (&fileInstance),
          m_data            (NULL),
          m_size            (fileInstance.getSize()),
          m_offset          (0)
    {}

    //////////////////////////////////////////////

    void InputStream::open(const void* data, std::size_t sizeInBytes)
    {
         m_data = static_cast<const char*>(data);
         m_size = sizeInBytes;
         m_offset = 0;
    }

    //////////////////////////////////////////////

    int64 InputStream::read(void* data, int64 size)
    {
        if (m_fileInstance)
        {
            std::vector<char>buf;
            buf.reserve(static_cast<unsigned int>(size));
            m_fileInstance->read(buf.data(), size);
            m_data = buf.data();

            int64 endPosition = m_offset + size;
            int64 count = endPosition <= m_size ? size : m_size - m_offset;
            if (count > 0)
            {
                std::memcpy(data, m_data, static_cast<std::size_t>(count));
                m_offset += count;
            }

            return count;
        }

        if (!m_data)
            return -1;

        int64 endPosition = m_offset + size;
        int64 count = endPosition <= m_size ? size : m_size - m_offset;

        if (count > 0)
        {
            std::memcpy(data, m_data + m_offset, static_cast<std::size_t>(count));
            m_offset += count;
        }

        return count;
    }

    //////////////////////////////////////////////

    int64 InputStream::seek(int64 position)
    {
        if (!m_data && !m_fileInstance)
            return -1;

        m_offset = position < m_size ? position : m_size;
    
        if (m_fileInstance)
            m_fileInstance->seek(m_offset);

        return m_offset;
    }

    //////////////////////////////////////////////

    int64 InputStream::tell()
    {
        if (!m_data && !m_fileInstance)
            return -1;

        return m_offset;
    }

    //////////////////////////////////////////////

    int64 InputStream::getSize()
    {
        if (!m_data)
            return -1;

        return m_size;
    }

    //////////////////////////////////////////////

    bool AudioReader::read(const void* ptr, SoundBuffer& soundBuf, uint64 size)
    {
        if (ptr)
        {
            if (checkWav(ptr))
                return readWav(ptr, soundBuf,size);

            else if (checkVorbis(ptr))
                return readVorbis(ptr, soundBuf, size);

            else
                JOP_DEBUG_ERROR("Tried to read unsupported audio file");
        }
        else
            JOP_DEBUG_ERROR("Tried to read empty audio buffer");

        return false;
    }

    //////////////////////////////////////////////

    bool AudioReader::stream(FileLoader& fileInstance, SoundBuffer& soundBuf, uint64& currentPos)
    {
        if (soundBuf.m_info.format == SoundBuffer::AudioFormat::ogg)
            return streamOgg(fileInstance, soundBuf, currentPos);

        return false;
    }

    //////////////////////////////////////////////

    bool AudioReader::readWav(const void* ptr, SoundBuffer& soundBuf,uint64 size)
    {
        drwav wavData;

        if (!drwav_init_memory(&wavData, ptr, static_cast<size_t>(size)))
            return false;

        soundBuf.m_info.format = SoundBuffer::AudioFormat::wav;
        soundBuf.m_info.channelCount = wavData.channels;
        soundBuf.m_info.sampleCount = wavData.memoryStream.dataSize - wavData.memoryStream.currentReadPos;
        soundBuf.m_info.sampleRate = wavData.sampleRate;
        soundBuf.m_duration = static_cast<float>(soundBuf.m_info.sampleCount / soundBuf.m_info.sampleRate / soundBuf.m_info.channelCount);

        if (soundBuf.m_duration < 0.01f)
        {
            --soundBuf.m_info.channelCount;
            soundBuf.m_duration = static_cast<float>(soundBuf.m_info.sampleCount / soundBuf.m_info.sampleRate / soundBuf.m_info.channelCount);
        }

        soundBuf.m_samples.reserve(wavData.memoryStream.dataSize - wavData.memoryStream.currentReadPos);

        for (size_t it = wavData.memoryStream.currentReadPos; it < wavData.memoryStream.dataSize; ++it)
            soundBuf.m_samples.push_back(wavData.memoryStream.data[it]);

        return true;
    }

    //////////////////////////////////////////////

    bool AudioReader::readVorbis(const void* ptr, SoundBuffer& soundBuf, uint64 size)
    {
        soundBuf.m_info.format = SoundBuffer::AudioFormat::ogg;

        InputStream input(ptr, size);
        OggVorbis_File oggData = {NULL};

        if (ov_open_callbacks(&input, &oggData, NULL, 0, callbacks) < 0)
        {
            ov_clear(&oggData);
            JOP_DEBUG_ERROR("Vorbis file " << soundBuf.getName() << " could not be parsed")
                return false;
        }

        vorbis_info*    oggInfo = ov_info(&oggData, -1);

        soundBuf.m_info.channelCount = oggInfo->channels;
        soundBuf.m_info.sampleRate = oggInfo->rate;
        soundBuf.m_info.sampleCount = static_cast<std::size_t>(ov_pcm_total(&oggData, -1) * oggInfo->channels);
        soundBuf.m_duration = static_cast<float>(soundBuf.m_info.sampleCount / soundBuf.m_info.sampleRate / soundBuf.m_info.channelCount);

        std::string streamTest = soundBuf.getName();
        if (streamTest[0] == 'S' &&streamTest[1] == 't' && streamTest[2] == 'r' && streamTest[3] == 'e')
        {
            soundBuf.m_samples.resize(JOP_AUDIO_STREAMING_BUFFER_SIZE - static_cast<unsigned int>(oggData.offset));
            return true;
        }

        soundBuf.m_samples.reserve(static_cast<unsigned int>(soundBuf.m_info.sampleCount));

        uint64 count = 0;
        std::vector<char> samplesReaded;
        samplesReaded.resize(4096);

        while (count < soundBuf.m_info.sampleCount)
        {
            int bytesToRead = static_cast<int>(soundBuf.m_info.sampleCount - count) * sizeof(int16);
            long bytesRead = ov_read(&oggData, reinterpret_cast<char*>(samplesReaded.data()), bytesToRead, 0, 2, 1, NULL);

            if (bytesRead > 0)
            {
                count += bytesRead / sizeof(int16);
                soundBuf.m_samples.insert(soundBuf.m_samples.end(), samplesReaded.begin(), samplesReaded.begin() + bytesRead);
            }
            else
                break;
        }

        ov_clear(&oggData);

        return true;
    }

    //////////////////////////////////////////////

    bool AudioReader::streamOgg(FileLoader& fileInstance, SoundBuffer& soundBuf, uint64& currentPos)
    {
        InputStream input(fileInstance);
        
        OggVorbis_File oggData = {NULL};

        if (ov_open_callbacks(&input, &oggData, NULL, 0, callbacks) < 0)
        {
            ov_clear(&oggData);
            JOP_DEBUG_ERROR("Vorbis file " << soundBuf.getName() << " could not be parsed");
            
            return false;
        }
        if (ov_pcm_seek(&oggData, currentPos) != 0)
            currentPos = input.tell();

        uint64 count = 0;
        std::vector<char> samplesReaded;
        samplesReaded.resize(4096);
        while (count < JOP_AUDIO_STREAMING_BUFFER_SIZE)
        {
            int bytesToRead = static_cast<int>(JOP_AUDIO_STREAMING_BUFFER_SIZE - count) * sizeof(int16);
            long bytesRead = ov_read(&oggData, reinterpret_cast<char*>(samplesReaded.data()), bytesToRead, 0, 2, 1, NULL);
            if (bytesRead > 0)
            {
                count += bytesRead / sizeof(int16);
                soundBuf.m_samples.insert(soundBuf.m_samples.end(), samplesReaded.begin(), samplesReaded.begin() + bytesRead);
            }
            else
            {
                JOP_DEBUG_ERROR("Decoding vorbis file " << soundBuf.getName() << " failed")
                return false;
            }

        }
        currentPos = ov_pcm_tell(&oggData);
        soundBuf.m_info.sampleCount = soundBuf.m_samples.size();
        ov_clear(&oggData);

        return true;
    }

    //////////////////////////////////////////////

    bool AudioReader::checkWav(const void* ptr)
    {
        auto buf = static_cast<const char*>(ptr);

        return (buf[0] == 'R') && (buf[1] == 'I') && (buf[2] == 'F') && (buf[3] == 'F')
            && (buf[8] == 'W') && (buf[9] == 'A') && (buf[10] == 'V') && (buf[11] == 'E');
    }

    //////////////////////////////////////////////

    bool AudioReader::checkVorbis(const void* ptr)
    {
        auto buf = static_cast<const char*>(ptr);

        return (buf[0] == 'O' && buf[1] == 'g' && buf[2] == 'g' && buf[3] == 'S');
    }
}