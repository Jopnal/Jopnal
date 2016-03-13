// Jopnal Editor C++ Application
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
#include <Jopnal/Editor/Precompiled/Precompiled.hpp>

//////////////////////////////////////////////


namespace jope
{
    Command::Command()
    {}

    Command::~Command()
    {}

    //////////////////////////////////////////////

    CreateObjectCommand::CreateObjectCommand(jop::WeakReference<jop::Object> parent, jop::WeakReference<jop::Object>& ref, const std::string& ID)
        : m_id(ID),
          m_parent(parent),
          m_ref(ref)
    {}

    void CreateObjectCommand::execute()
    {
        if (m_parent.expired())
            m_ref = jop::Engine::getCurrentScene().createChild(m_id);
        else
            m_ref = m_parent->createChild(m_id);

        m_ref->createComponent<jop::GenericDrawable>(jop::Engine::getCurrentScene().getRenderer());
        m_ref->setPosition(0.f, 0.f, -2.f);
    }

    void CreateObjectCommand::undo()
    {
        
    }

    //////////////////////////////////////////////

    ChangeObjectIDCommand::ChangeObjectIDCommand(jop::Object& obj, const std::string& newID)
        : m_newID(newID),
          m_oldID(obj.getID()),
          m_ref(obj)
    {}

    void ChangeObjectIDCommand::execute()
    {
        m_ref->setID(m_newID);
    }

    void ChangeObjectIDCommand::undo()
    {
        m_ref->setID(m_oldID);
    }

    //////////////////////////////////////////////

    SetAciveCommand::SetAciveCommand(jop::Object& obj, const bool active)
        : m_ref(obj),
          m_setActive(active)
    {}

    void SetAciveCommand::execute()
    {
        m_ref->setActive(m_setActive);
    }

    void SetAciveCommand::undo()
    {
        m_ref->setActive(!m_setActive);
    }

    //////////////////////////////////////////////

    SetObjectObjectPositionCommand::SetObjectObjectPositionCommand(jop::Object& obj, const glm::vec3& pos)
        : m_pos(pos),
          m_oldPos(obj.getPosition()),
          m_ref(obj)
    {}

    void SetObjectObjectPositionCommand::execute()
    {
        m_ref->setPosition(m_pos);
    }

    void SetObjectObjectPositionCommand::undo()
    {
        m_ref->setPosition(m_oldPos);
    }

}