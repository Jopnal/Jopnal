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

    #include <Jopnal/Audio/AudioDevice.hpp>

    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

//////////////////////////////////////////////


namespace
{
    ALCdevice_struct* ns_device = nullptr;    ///< Audio's output device
    ALCcontext_struct* ns_context = nullptr;  ///< Audio's context
}

namespace jop
{
    AudioDevice::AudioDevice()
        : Subsystem(0)
    {
        ns_device = alcOpenDevice(NULL);

        if (ns_device)
            ns_context = alcCreateContext(ns_device, NULL);
        else
            JOP_DEBUG_ERROR("Could not initialize context to audio device");
        
        if (!alcIsExtensionPresent(ns_device, "AL_EXT_BFORMAT"))
            JOP_DEBUG_INFO("Missing AL_EXT_BFORMAT extension for OpenAl");

        if (!alcIsExtensionPresent(ns_device, "AL_EXT_MULAW_BFORMAT"))
            JOP_DEBUG_INFO("Missing AL_EXT_MULAW_BFORMAT extension for OpenAl");

        if (!alcIsExtensionPresent(ns_device, "ALC_SOFT_HRTF"))
            JOP_DEBUG_INFO("Missing AL_EXT_MULAW_BFORMAT extension for OpenAl");

        if (!alcMakeContextCurrent(ns_context))
            JOP_DEBUG_ERROR("Could not set audio's context active");

        if (!alIsExtensionPresent("AL_SOFT_source_length"))
            JOP_DEBUG_ERROR("Audio's required AL_SOFT_source_length not present");

        if (!alIsExtensionPresent("AL_SOFT_source_latency"))
            JOP_DEBUG_INFO("Audio's AL_SOFT_source_latency not present, audio may strutter");
    }

    AudioDevice::~AudioDevice()
    {
        ns_context = alcGetCurrentContext();
        ns_device = alcGetContextsDevice(ns_context);

        alcMakeContextCurrent(NULL);
        alcDestroyContext(ns_context);
        alcCloseDevice(ns_device);
    }

    //////////////////////////////////////////////

    void AudioDevice::setDevice(const std::string& device)
    {
        ns_context = alcGetCurrentContext();
        ns_device = alcGetContextsDevice(ns_context);

        alcMakeContextCurrent(NULL);
        alcDestroyContext(ns_context);
        alcCloseDevice(ns_device);

        ns_device = alcOpenDevice(device.c_str());

        if (ns_device)
            ns_context = alcCreateContext(ns_device, NULL);
        else
            JOP_DEBUG_ERROR("Could not initialize context to audio device");
    }

    //////////////////////////////////////////////

    std::string AudioDevice::getDeviceName()
    {
        auto str = alTry(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));

        return std::string(str);
    }

    //////////////////////////////////////////////

    std::string AudioDevice::getAllDeviceNames()
    {
        auto str = alTry(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

        return std::string(str);
    }

    //////////////////////////////////////////////

    ALCdevice_struct& AudioDevice::getDevice()
    {
        return *ns_device;
    }

    //////////////////////////////////////////////

    ALCcontext_struct& AudioDevice::getContext()
    {
        return *ns_context;
    }
}