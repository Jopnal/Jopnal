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

#ifndef JOP_SOUNDSTREAM_HPP
#define JOP_SOUNDSTREAM_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Audio/SoundSource.hpp>
#include <Jopnal/Core/FileLoader.hpp>
#include <Jopnal/Utility/Thread.hpp>
#include <mutex>
#include <vector>
#include <atomic>

//////////////////////////////////////////////


namespace jop
{
    class SoundBuffer;

    class JOP_API SoundStream : public SoundSource
    {
    private:

        struct ParsedStreamingInfo
        {
            ParsedStreamingInfo();

            uint64 sampleCount;         ///< Total number of samples
            uint64 currentPos;          ///< Current reading point in audio data
            std::atomic<float> offset;  ///< Current offset in audio data
        };

        JOP_GENERIC_COMPONENT_CLONE(SoundStream);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        ///
        SoundStream(Object& object);

        /// \brief destructor
        ///
        ~SoundStream();


        /// \brief Update
        ///
        /// Updates offset
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        /// \brief Stream audio from file
        ///
        /// This will not start playing the stream. You must call play() in addition.
        ///
        /// \param path Path to audio file
        ///
        /// \return True if loaded successfully
        ///
        bool setPath(const std::string& path);

        /// \copydoc SoundEffect::play(const bool)
        ///
        SoundStream& play();

        /// \copydoc SoundEffect::play
        ///
        SoundStream& playReset();

        /// \copydoc SoundEffect::stop
        ///
        SoundStream& stop();

        /// \copydoc SoundEffect::pause
        ///
        SoundStream& pause();

        /// \copydoc SoundEffect::setOffset
        ///
        SoundStream& setOffset(const float time);

        /// \copydoc SoundEffect::getOffset
        ///
        float getOffset();

        /// \copydoc SoundEffect::setLoop
        ///
        /// If sound is small enough for one buffer, 
        /// loop might be ignored due to buffer queue ending before loop check.
        ///
        SoundStream& setLoop(const bool loop);

        /// \copydoc SoundEffect::isLooping()
        ///
        bool isLooping() const;

    private:

        /// \brief updateStream
        ///
        /// Loop for stream's thread
        ///
        void updateStream();

        /// \brief fillBuffer
        ///
        /// Reads next chunk of audio data into buffer
        ///
        void fillBuffer();

        /// \brief Private method to change stream's offset
        ///
        void changeOffset();

        /// \brief Private method for opening file
        ///
        bool openFile();

        /// \brief Private method for closing file
        ///
        void closeFile();


        std::recursive_mutex m_mutex;  
        std::string m_path;                         ///< Remembers streaming path for cloning
        bool m_isFileOpen;                          ///< Opens file if it's closed
        bool m_loop;                                ///< If true song start from beginning when finished
        std::atomic<bool> m_keepThread;             ///< While true keeps thread alive
        float m_deltaOffset;                        ///< Updates current offset
        std::atomic<float> m_inputOffset;           ///< Stores input offset
        std::atomic<float> m_rawOffset;             ///< Reader's exact position
        std::vector<std::unique_ptr<SoundBuffer>> m_bufferQueue; ///< SoundBuffer stack for streaming
        ParsedStreamingInfo m_info;                 ///< Critical information for passing between buffers
        FileLoader m_fileInstance;                  ///< FileLoader instance for this stream
        Thread m_streamThread;                      ///< Enables streaming in seperate thread
    };
}
#endif

/// \class SoundStream
/// \ingroup Audio
///
/// Sound streaming straight from file 