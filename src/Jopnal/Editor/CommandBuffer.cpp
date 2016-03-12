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
        : jop::Subsystem("Command Buffer")
    {
        JOP_ASSERT(m_instance == nullptr, "Only one CommandBuffer may exist at a time!");

        m_instance = this;
    }

    CommandBuffer::~CommandBuffer()
    {
        m_instance = nullptr;
    }

    //////////////////////////////////////////////

    void CommandBuffer::preUpdate(const float)
    {
        static const unsigned int maxSize = jop::SettingManager::getUint("uMaxCommandBufferSize", 100);

        std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);

        if (!lock.owns_lock())
            return;

        bool newCommands = !m_newCommands.empty();

        while (newCommands)
        {
            m_commands.push_back(std::move(m_newCommands.front()));
            m_newCommands.pop();

            if (m_commands.size() > maxSize)
                m_commands.pop_front();

            m_commands.back()->execute();

            newCommands = !m_newCommands.empty();
        }

        if (newCommands)
            m_currentDepth = m_commands.end();
    }

    //////////////////////////////////////////////

    void CommandBuffer::pushCommand(Command* const command)
    {
        std::lock_guard<std::mutex> lock(m_instance->m_mutex);

        m_instance->m_newCommands.push(std::unique_ptr<Command>(command));
    }

    //////////////////////////////////////////////

    void CommandBuffer::undoLast()
    {
        std::lock_guard<std::mutex> lock(m_instance->m_mutex);

        auto& itr = m_instance->m_currentDepth;

        while (itr != m_instance->m_commands.begin())
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
        std::lock_guard<std::mutex> lock(m_instance->m_mutex);

        auto& itr = m_instance->m_currentDepth;

        if (itr != m_instance->m_commands.end())
            (itr++)->get()->execute();
    }

    //////////////////////////////////////////////

    std::mutex& CommandBuffer::acquireMutex()
    {
        return m_instance->m_mutex;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    CommandBuffer* CommandBuffer::m_instance = nullptr;
}