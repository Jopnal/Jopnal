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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Listener::Listener(Object& object)
        : Component(object, "listener")
    {}

    Listener::Listener(const Listener& other, Object& newObj)
        : Component(other, newObj)
    {
        JOP_DEBUG_WARNING("jop::Listener component is not meant to be copied, sounds will likely not behave correctly");
    }

    //////////////////////////////////////////////

    void Listener::update(const float)
    {
        glm::vec3 var = getObject()->getGlobalPosition();
        sf::Listener::setPosition(var.x, var.y, var.z);

        var = getObject()->getGlobalFront();
        sf::Listener::setDirection(var.x, var.y, var.z);
    }

    ///////////////////////////////////////

    void Listener::setGlobalVolume(const float volume)
    {
        sf::Listener::setGlobalVolume(glm::clamp(volume, 0.f, 100.f));
    }

    ///////////////////////////////////////

    float Listener::getGlobalVolume() const
    {
        return sf::Listener::getGlobalVolume();
    }
}