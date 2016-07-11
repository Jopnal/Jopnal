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

#ifndef JOP_AUDIOREADER_HPP
#define JOP_AUDIOREADER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/FileLoader.hpp>

#define JOP_AUDIO_STREAMING_BUFFER_SIZE 1000000

//////////////////////////////////////////////


namespace jop
{
    class SoundBuffer;

    class AudioReader
    {
    public:

        /// \brief Check if data can be decoded and decode it if possible
        ///
        static bool read(const void* ptr, SoundBuffer& soundBuf,uint64 size);

        /// \brief Decode streamed data
        ///
        static bool stream(FileLoader& fileInstance, SoundBuffer& soundBuf, uint64& currentPos);

    private:
        /// \brief Decodes wav file
        ///
        static bool readWav(const void* ptr, SoundBuffer& soundBuf, uint64 size);

        /// \brief Decodes ogg file
        ///
        static bool readVorbis(const void* ptr, SoundBuffer& soundBuf, uint64 size);

        /// \brief Decode streamed vorbis data
        ///
        static bool streamOgg(FileLoader& fileInstance, SoundBuffer& soundBuf, uint64& currentPos);

        /// \brief Checks if data is wav file
        ///
        static bool checkWav(const void* ptr);

        /// \brief Checks if data is ogg file
        ///
        static bool checkVorbis(const void* ptr);
    };
}

#endif

/// \class AudioReader
/// \ingroup Audio
///
/// Parses audio
/// Supports: wav, ogg
