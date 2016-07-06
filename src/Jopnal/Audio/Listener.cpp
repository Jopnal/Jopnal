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

#include <Jopnal/Audio/Listener.hpp>

#include <Jopnal/Core/DebugHandler.hpp>
#include <Jopnal/Utility/CommandHandler.hpp>

#endif

#include <Jopnal/Audio/AlTry.hpp>

#pragma warning(disable: 4127)

//////////////////////////////////////////////


namespace jop
{
	JOP_REGISTER_COMMAND_HANDLER(Listener)

		JOP_BIND_MEMBER_COMMAND((Listener& (Listener::*)(float))&Listener::setGlobalVolume, "setGlobalVolume");

	JOP_END_COMMAND_HANDLER(Listener)
}

namespace jop
{
    Listener::Listener(Object& object)
		: Component(object, 0),
		m_dopplerEffect(false),
		m_doppler(1.f),
		m_lastPos(NULL)
	{
		jop::AudioDevice::checkError();
	
		glm::vec3 pos = object.getGlobalPosition();
		ALfloat var[] = { pos.x,pos.y,pos.z };
		alTry(alListenerfv(AL_POSITION, var));

		ALfloat speed[] = { 0.f, 0.f, 0.f };
		alTry(alListenerfv(AL_VELOCITY, speed));

		glm::vec3 vec = object.getGlobalUp();
		ALfloat direction[] = { pos.x, pos.y, pos.z, vec.x, vec.y, vec.z };
		alTry(alListenerfv(AL_ORIENTATION, direction));

		jop::AudioDevice::checkError();
	}

    Listener::Listener(const Listener& other, Object& newObj)
		: Component(other, newObj),
		m_dopplerEffect(other.m_dopplerEffect),
		m_doppler(other.m_doppler),
		m_lastPos(other.m_lastPos)
    {
        JOP_DEBUG_WARNING("jop::Listener component is not meant to be copied, sounds will likely not behave correctly");
    }

    //////////////////////////////////////////////

    void Listener::update(const float)
    {

		glm::vec3 pos = getObject()->getGlobalPosition();
		ALfloat var[] = { pos.x, pos.y, pos.z };
		alTry(alListenerfv(AL_POSITION, var));

		if (m_dopplerEffect)
		{
			m_lastPos -= glm::abs(pos);
			ALfloat speed[] = { m_lastPos.x, m_lastPos.y, m_lastPos.z };
			alTry(alListenerfv(AL_VELOCITY, speed));
			m_lastPos = glm::abs(pos);
		}
		
		pos = getObject()->getGlobalFront();
		glm::vec3 up = getObject()->getGlobalUp();
		ALfloat  direction[] = { pos.x, pos.y, pos.z, up.x, up.y, up.z };
		alTry(alListenerfv(AL_ORIENTATION, direction));
    }

    ///////////////////////////////////////

	Listener& Listener::setGlobalVolume(const float volume)
    {
		alTry(alListenerf(AL_GAIN, glm::clamp(volume, 0.f, 100.f) * 0.01f));

		return *this;
    }

    ///////////////////////////////////////

    float Listener::getGlobalVolume() const
    {
		float volume;
		alTry(alGetListenerf(AL_GAIN, &volume));

		return volume* 100.f;
    }

	///////////////////////////////////////

	Listener& Listener::setDopplerEffect(float dop)
	{
		m_doppler = std::min(0.f, dop);
		alTry(alDopplerFactor(m_doppler));

		return *this;
	}

	///////////////////////////////////////

	float Listener::getDopplerEffect()
	{
		return m_doppler;
	}

	///////////////////////////////////////

	Listener& Listener::setDopplerEffectToDefault()
	{
		m_doppler = 1.f;
		alTry(alDopplerFactor(m_doppler));

		return *this;
	}

	///////////////////////////////////////

	Listener& Listener::setSpeedOfSound(float speed)
	{
		float sos = std::max(0.f, speed);
		jop::SoundSource::setSpeedForSound(sos);
		alTry(alSpeedOfSound(sos));

		return *this;
	}

	///////////////////////////////////////

	float Listener::getSpeedOfSound()
	{
		return jop::SoundSource::getSpeedForSound();
	}

	///////////////////////////////////////

	Listener& Listener::useDopplerEffect(bool use)
	{
		m_dopplerEffect = use;
		jop::SoundSource::calculateDopplerEffect(use);

		return *this;
	}

	///////////////////////////////////////

	Listener& Listener::useSpeedOfSound(bool use)
	{
		jop::SoundSource::calculateSpeedOfSound(use);

		return *this;
	}


	///////////////////////////////////////

	bool Listener::isSpeedOfSound()
	{
		return jop::SoundSource::isSpeedOfSound();
	}

	///////////////////////////////////////

	bool Listener::isDopplerEffect()
	{
		return m_dopplerEffect;
	}

	///////////////////////////////////////

	Listener& Listener::setSpeedOfSoundToDefault()
	{
		jop::SoundSource::setSpeedForSound(343.3f);
		alTry(alSpeedOfSound(343.3f));

		return *this;
	}

}