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

#ifndef JOP_SOUNDBUFFER_HPP
#define JOP_SOUNDBUFFER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class SoundSource;
    class SoundStream;

    class JOP_API SoundBuffer : public Resource
    {    
    public:

        enum class AudioFormat
        {
            undefined,
            wav,
            ogg
        };

    private:

        struct parsedAudioInfo
        {
            uint64 sampleCount = 0;                      ///< Total number of samples
            int channelCount   = 0;                      ///< Number of channels
            int sampleRate     = 0;                      ///< Samples per second
            AudioFormat format = AudioFormat::undefined;    ///< Format of audio for decoding
        };

        friend class AudioReader;
        friend class SoundEffect;
        friend class SoundStream;

        JOP_DISALLOW_COPY_MOVE(SoundBuffer);

    public:

        /// \brief Constructor
        ///
        /// Initializes the internal buffer to be empty.
        ///
        /// \param name Name of this resource
        ///
        SoundBuffer(const std::string& name);

        /// \brief Copy constructor
        ///
        /// \param other The other buffer to be copied
        /// \param newName New name of this resource
        ///
        SoundBuffer(const SoundBuffer& other, const std::string& newName);

        /// \brief Destructor
        ///
        /// \warning Destroying a sound buffer while it's being used by a SoundEffect
        ///          instance, will most likely lead to a crash.
        ///
        ~SoundBuffer() override;


        /// \brief Load a new buffer from file
        ///
        /// \param path Path for wanted resource
        ///
        /// \return True if successful
        ///
        bool load(const std::string& path);

        /// \brief Load a new buffer from memory
        ///
        /// \param Pointer to array and size
        ///
        /// \return True if successful
        ///
        bool load(const void* ptr, const uint32 size);

        /// \brief Get default sound buffer
        ///
        /// \return Reference to the buffer
        ///
        static SoundBuffer& getDefault();

    private:

        /// \brief Update OpenAl
        ///
        SoundBuffer& refresh();

        /// \brief Private method to link SoundSource and SoundBuffer
        ///
        /// param SoundSource to get attached
        ///
        void attachSound(SoundSource* sound) const;

        /// \brief Private method to unlink SoundSource and SoundBuffer
        ///
        /// param SoundSource to get detached
        ///
        void detachSound(SoundSource* sound) const;
        
        unsigned int m_bufferId;                    ///< Identifier for openAl buffer
        float m_duration;                           ///< Duration as seconds
        std::vector<uint8> m_samples;               ///< Samples
        mutable std::vector<SoundSource*> m_sounds; ///< SoundSources that use this buffer
        parsedAudioInfo m_info;                     ///< Info about sound's structure
    };
}
#endif

/// \class SoundBuffer
/// \ingroup Audio
///
/// Sound data storage