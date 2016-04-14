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

#ifndef JOP_THREAD_HPP
#define JOP_THREAD_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <thread>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Thread
    {
    private:

        JOP_DISALLOW_COPY(Thread);

    public:

        struct Priority
        {
            enum
            {
                Lowest,
                Lower,
                Normal,
                Higher,
                Highest
            };
        };

    public:

        Thread();

        template<typename F, typename ... Args>
        explicit Thread(F&& func, Args&&... args);

        Thread(Thread&& other);

        Thread& operator =(Thread&& other);

        ~Thread();


        void join();

        bool isJoinable() const;

        void detach();

        bool setPriority(const unsigned int priority);

        void terminate();

        std::thread::id getId() const;

    private:

        std::thread m_thread;
    };

    // Include the implementation file
    #include <Jopnal/Utility/Inl/Thread.inl>
}

#endif