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

#ifndef JOP_AUDIODEVICE_HPP
#define JOP_AUDIODEVICE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <string>

//////////////////////////////////////////////


struct ALCdevice_struct;
struct ALCcontext_struct;

namespace jop
{
    class JOP_API AudioDevice final : public Subsystem
    {
    public:

        /// \brief Default constructor
        ///
        /// Initializes preferred device for output audio
        ///
        AudioDevice();

        /// \brief Destructor
        ///
        ~AudioDevice() override;

        
        /// \brief Set new device for audio output
        ///
        /// \param device Audio device's name
        ///
        static void setDevice(const std::string& device);
        
        /// \return Default audio device's name
        ///
        static std::string getDeviceName();

        /// \return All audio device names
        ///
        static std::string getAllDeviceNames();

        /// \return Audio device
        ///
        static ALCdevice_struct& getDevice();

        /// \return Audio context
        ///
        static ALCcontext_struct& getContext();
    };
}

/// \class jop::AudioDevice
/// \ingroup audio
///
/// Handle to operating system's audio management application

#endif