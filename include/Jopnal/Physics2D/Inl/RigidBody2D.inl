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


template<typename T>
T* RigidBody2D::getJoint(unsigned int id)
{
    for (auto& i : m_joints)
    {
        if (typeid(*i) == typeid(T) && id == i->getID())
            return static_cast<T*>(i.get());
    }
    return nullptr;
}

//////////////////////////////////////////////

template<typename T>
bool RigidBody2D::breakJoint(RigidBody2D& other, unsigned int IDthis, unsigned int IDother)
{
    unsigned int result = 0;
    
    if ((!m_joints.empty()) && (!other.m_joints.empty()))
    {
        //this
        for (auto itr = m_joints.begin(); itr != m_joints.end(); ++itr)
        {
            if (typeid(*(*itr)) == typeid(T) && IDthis == (*itr)->getID())
            {
                m_joints.erase(itr);
                ++result;
                break;
            }
        }

        //other
        for (auto itr = other.m_joints.begin(); itr != other.m_joints.end(); ++itr)
        {
            if (typeid(*(*itr)) == typeid(T) && IDother == (*itr)->getID())
            {
                other.m_joints.erase(itr);
                return ++result == 2;
            }
        }
    }

    return false;
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& RigidBody2D::link(RigidBody2D& other, Args&&... args)
{
    auto joint = std::make_shared<T>(m_worldRef2D, *this, other, std::forward<Args>(args)...);

    m_joints.emplace(joint);
    other.m_joints.emplace(joint);

    return static_cast<T&>(*joint);
}