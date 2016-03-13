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

#ifndef JOPE_COMMAND_HPP
#define JOPE_COMMAND_HPP

// Headers
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class Object;
}

namespace jope
{
    class Command
    {
    private:

        friend class CommandBuffer;

    protected:

        Command();

    public:

        virtual ~Command() = 0;

    private:

        virtual void execute() = 0;

        virtual void undo() = 0;

    };

    //////////////////////////////////////////////

    struct CreateObjectCommand : Command
    {
        CreateObjectCommand(jop::WeakReference<jop::Object> parent, jop::WeakReference<jop::Object>& ref, const std::string& ID);

    private:

        void execute() override;
        void undo() override;

        std::string m_id;
        jop::WeakReference<jop::Object> m_parent;
        jop::WeakReference<jop::Object>& m_ref;

    };

    //////////////////////////////////////////////

    struct RemoveObjectCommand : Command
    {
        RemoveObjectCommand();

    private:

        void execute() override;
        void undo() override;


    };

    //////////////////////////////////////////////

    struct ChangeObjectIDCommand : Command
    {
        ChangeObjectIDCommand(jop::Object& obj, const std::string& newID);

        void execute() override;

        void undo() override;

    private:

        std::string m_newID;
        std::string m_oldID;
        jop::WeakReference<jop::Object> m_ref;
    };

    //////////////////////////////////////////////

    struct SetAciveCommand : Command
    {
        SetAciveCommand(jop::Object& obj, const bool active);

        virtual void execute() override;

        virtual void undo() override;

    private:

        jop::WeakReference<jop::Object> m_ref;
        bool m_setActive;
    };

    //////////////////////////////////////////////

    struct SetObjectObjectPositionCommand : Command
    {
        SetObjectObjectPositionCommand(jop::Object& obj, const glm::vec3& pos);

        virtual void execute() override;

        virtual void undo() override;

    private:

        glm::vec3 m_pos;
        glm::vec3 m_oldPos;
        jop::WeakReference<jop::Object> m_ref;
    };

    //////////////////////////////////////////////

    struct SetObjectScaleCommand : Command
    {

    };

    //////////////////////////////////////////////

    struct SetObjectRotCommand : Command
    {

    };
}

#endif