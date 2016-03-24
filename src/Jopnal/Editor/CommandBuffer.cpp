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
    CommandBuffer::CommandBuffer()
    {
        JOP_ASSERT(m_instance == nullptr, "Only one CommandBuffer may exist at a time!");
        m_instance = this;
    }

    CommandBuffer::~CommandBuffer()
    {
        m_instance = nullptr;
    }

    //////////////////////////////////////////////

    void CommandBuffer::pushCommand(Command* const command)
    {
        static const unsigned int maxSize = jop::SettingManager::getUint("uMaxCommandBufferSize", 100);

        auto& inst = *m_instance;

        command->execute();
        inst.m_commands.emplace_back(command);

        if (inst.m_commands.size() > maxSize)
            inst.m_commands.pop_front();
    }

    //////////////////////////////////////////////

    void CommandBuffer::undoLast()
    {
        if (m_instance->m_commands.empty())
            return;

        auto& itr = m_instance->m_currentDepth;

        if (itr != m_instance->m_commands.begin())
            (itr--)->get()->undo();
    }

    //////////////////////////////////////////////

    void CommandBuffer::undo(unsigned int steps)
    {
        while (steps--)
            undoLast();
    }

    //////////////////////////////////////////////

    void CommandBuffer::redoCurrent()
    {
        auto& itr = m_instance->m_currentDepth;

        if (itr != m_instance->m_commands.end())
            (itr++)->get()->execute();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    CommandBuffer* CommandBuffer::m_instance = nullptr;
}