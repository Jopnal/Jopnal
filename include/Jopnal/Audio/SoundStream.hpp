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

//////////////////////////////////////////////


namespace jop
{
    class SoundBuffer;

    class JOP_API SoundStream : public SoundSource
    {
    private:

        struct ParsedStreamingInfo
        {
            uint64 sampleCount  = 0; ///< Total number of samples
            uint64 currentPos   = 0; ///< Current point in audio data
            uint64 firstSample  = 0; ///< First audio sample for looping
            float offset[2];            ///< Track offset of current buffer
            int channelCount    = 0; ///< Number of channels
            int sampleRate      = 0; ///< Samples per second
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
        /// Keeps tabs on offset and handling buffers.
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
        float getOffset() const;

        /// \copydoc SoundEffect::setLoop
        ///
        SoundStream& setLoop(const bool loop);

        /// \copydoc SoundEffect::isLooping
        ///
        bool isLooping() const;

    private:

        /// \brief Private method handling updating buffers
        ///
        void updateBackBuffer();

        /// \brief Private method handling openAlQueue
        ///
        void updateOpenAl();

        /// \brief Private method to start song from beginning
        ///
        void fromBegin();

        /// \brief Private method to read samples into free buffer
        ///
        void readBuffer();

        /// \brief Private method for opening file
        ///
        bool openFile();

        /// \brief Private method for closing file
        ///
        void closeFile();

        std::mutex m_mutex;  
        std::string m_path;                         ///< Remembers streaming path for cloning
        bool m_isFileOpen;                          ///< Opens file if it's closed
        bool m_loop;                                ///< If true song start from beginning when finished
        bool m_playing;                             ///< For maintaining buffer updates
        bool m_lastBuffer;                          ///< True when chunk of data is in use
        std::atomic<bool> m_updatebuffer;           ///< Check to able reader thread to update audio data
        std::atomic<bool> m_updateBoth;             ///< Check reader thread will update audio data for both buffers
        std::atomic<bool> m_keepThread;             ///< While true keeps thread alive
        std::vector<std::unique_ptr<SoundBuffer>> m_bufferQueue; ///< SoundBuffer stack for streaming
        ParsedStreamingInfo m_info;                 ///< Critical information for passing between buffers
        FileLoader m_fileInstance;                  ///< FileLoader instance for this stream
        Thread m_thread;                            ///< Own thread for reading files to buffer
    };
}
#endif

/// \class SoundStream
/// \ingroup Audio
///
/// Sound streaming straight from file 
/// Uses SFML Music.hpp streaming instead of FileLoader.hpp