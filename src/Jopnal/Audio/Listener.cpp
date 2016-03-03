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
    Listener::Listener(Object& object, const std::string& ID)
        :Component(object, ID)
    {}

    Listener::Listener(const Listener& other)
        : Component(other)
    {}

    //////////////////////////////////////////////

    Listener::~Listener()
    {}

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void Listener::update(const float)
    {
        glm::vec3 var = getObject()->getPosition();
        sf::Listener::setPosition(var.x, var.y, var.z);

        var = getObject()->getGlobalFront();
        sf::Listener::setDirection(var.x, var.y, var.z);
    }

    ///////////////////////////////////////

    void Listener::setGlobalVolume(float vol)
    {
        sf::Listener::setGlobalVolume(vol);
    }

    ///////////////////////////////////////

    float Listener::getGlobalVolume()
    {
        return sf::Listener::getGlobalVolume();
    }

    ///////////////////////////////////////

    void Listener::setPosition(const float& x, const float& y, const float& z)
    {
        sf::Listener::setPosition(x, y, z);
    }

    ///////////////////////////////////////

    void Listener::setPosition(const glm::fvec3& position)
    {
        sf::Listener::setPosition(position.x, position.y, position.z);
    }

    ///////////////////////////////////////

    glm::fvec3 Listener::getPosition()
    {
        return glm::fvec3(sf::Listener::getPosition().x,
            sf::Listener::getPosition().z,
            sf::Listener::getPosition().y);
    }

    ///////////////////////////////////////

    void Listener::setDirection(const float& x, const float& y, const float& z)
    {
        sf::Listener::setDirection(x, y, z);
    }

    ///////////////////////////////////////

    void Listener::setDirection(const glm::fvec3& direction)
    {
        sf::Listener::setDirection(direction.x, direction.y, direction.z);
    }

    ///////////////////////////////////////

    const glm::fvec3 Listener::getDirection()
    {
        return glm::fvec3(sf::Listener::getDirection().x,
            sf::Listener::getDirection().z,
            sf::Listener::getDirection().y);
    }
}