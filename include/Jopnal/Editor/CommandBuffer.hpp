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

#ifndef JOPE_COMMANDBUFFER_HPP
#define JOPE_COMMANDBUFFER_HPP

// Headers
#include <Jopnal/Core/SubSystem.hpp>
#include <queue>
#include <memory>
#include <mutex>

//////////////////////////////////////////////


namespace jope
{
    class Command;

    class CommandBuffer final : public jop::Subsystem
    {
    private:

        typedef std::deque<std::unique_ptr<Command>> CommandQueue;

    public:

        CommandBuffer();

        ~CommandBuffer() override;

        void preUpdate(const float deltaTime) override;


        static void pushCommand(Command* const command);

        static void undoLast();

        static void undo(unsigned int steps);

        static void redoCurrent();

        static std::mutex& acquireMutex();


    private:

        static CommandBuffer* m_instance;

        std::mutex m_mutex;
        CommandQueue m_commands;
        std::queue<std::unique_ptr<Command>> m_newCommands;
        CommandQueue::iterator m_currentDepth;

    };
};

#endif