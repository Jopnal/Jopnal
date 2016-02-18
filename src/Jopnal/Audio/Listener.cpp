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
      //  m_listener(std::make_unique<sf::Listener>())
    {
        if (&m_listener != nullptr)
        {
            JOP_DEBUG_WARNING("WARNING!: There is already declarated listener!")
        }
       m_camera.operator=( object.getComponent<Camera>());
    }
    
    ///////////////////////////////////////

    Listener::~Listener()
    {}

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void Listener::update()
    {     
            glm::mat4 modelViewT = glm::transpose(m_camera.lock()->getViewMatrix());            
           
            // Get plane normals 
            glm::vec3 n1(modelViewT[0]);
            glm::vec3 n2(modelViewT[1]);
            glm::vec3 n3(modelViewT[2]);

            sf::Vector3f sfPos = { n3.x,n3.y,n3.z };
            sf::Listener::setDirection(sfPos);

            // Get plane distances
            float d1(modelViewT[0].w);
            float d2(modelViewT[1].w);
            float d3(modelViewT[2].w);

            // Get the intersection of these 3 planes
            glm::vec3 n2n3 = cross(n2, n3);
            glm::vec3 n3n1 = cross(n3, n1);
            glm::vec3 n1n2 = cross(n1, n2);

            glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
            float denom = dot(n1, n2n3);

            glm::vec3 pos = top / -denom;
            sfPos= { pos.x, pos.y, pos.z };
            sf::Listener::setPosition(sfPos);
            
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
        sf::Vector3f pos = { position.x, position.y, position.z };
        sf::Listener::setPosition(pos);
    }

    ///////////////////////////////////////

    glm::fvec3 Listener::getPosition()
    {
        return glm::fvec3(sf::Listener::getPosition().x, sf::Listener::getPosition().z, sf::Listener::getPosition().y);
    }

    ///////////////////////////////////////

    void Listener::setDirection(const float& x, const float& y, const float& z)
    {
        sf::Listener::setDirection(x, y, z);
    }

    ///////////////////////////////////////

    void Listener::setDirection(const glm::fvec3& direction)
    {
        sf::Vector3f dir = { direction.x, direction.y, direction.z };
        sf::Listener::setPosition(dir);
    }

    ///////////////////////////////////////

    const glm::fvec3 Listener::getDirection()
    {
        return glm::fvec3(sf::Listener::getDirection().x, sf::Listener::getDirection().z, sf::Listener::getDirection().y);
    }

}