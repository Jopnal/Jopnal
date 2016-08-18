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


class _JNIEnv;

namespace jop
{
    class JOP_API Thread
    {
    private:

        JOP_DISALLOW_COPY(Thread);

    public:

        /// Thread priority
        ///
        enum class Priority
        {
            Lowest,
            Lower,
            Normal,
            Higher,
            Highest
        };

    public:

        /// \brief Default constructor
        ///
        /// Will not start an actual thread.
        ///
        Thread();

        /// \brief Constructor
        ///
        /// This will start the thread immediately.
        ///
        /// \param func The function to use
        /// \param args Arguments to pass to the function
        ///
        template<typename F, typename ... Args>
        explicit Thread(F&& func, Args&&... args);

        /// \brief Move constructor
        ///
        Thread(Thread&& other);

        /// \brief Move assignment operator
        ///
        Thread& operator =(Thread&& other);

        /// \brief Destructor
        ///
        /// If the thread is running and hasn't been detached, this will
        /// wait until it returns.
        ///
        ~Thread();


        /// \brief Wait for the thread to return
        ///
        /// This will return immediately if the thread is not joinable.
        ///
        void join();

        /// \brief Check if this thread is joinable
        ///
        /// The thread is joinable if it hasn't been detached.
        ///
        /// \return True if joinable
        ///
        bool isJoinable() const;

        /// \brief Detach the thread
        ///
        /// After this call the thread is completely independent. join() will
        /// have no effect.
        ///
        void detach();

        /// \brief Set thread priority
        ///
        /// \param priority The priority to set
        ///
        /// \return True if priority was set successfully
        ///
        bool setPriority(const Priority priority);

        /// \brief Terminate this thread
        ///
        /// This method is unsafe. You should always attempt to make
        /// your threads return by themselves before resorting to
        /// calling this.
        ///
        void terminate();

        /// \brief Get the thread identifier
        ///
        /// \return The thread identifier
        ///
        std::thread::id getId() const;

        /// \brief Attach the calling thread to the Java virtual machine
        ///
        /// A thread needs to be attached if it makes any JNI calls.
        ///
        /// On operating systems besides Android, this function is no-op.
        ///
        /// \param vm Pointer to the Java virtual machine, may be null
        /// \param mainEnv Pointer to the NativeActivity JNIEnv, may be null
        ///
        static void attachJavaThread(void* vm, void* mainEnv);

        /// \brief Detach the calling thread from the Java virtual machine
        ///
        /// On operating systems besides Android, this function is no-op.
        ///
        /// \param vm Pointer to the Java virtual machine, may be null
        ///
        static void detachJavaThread(void* vm);

        /// \brief Get the JNIEnv for the calling thread
        ///
        /// \return Pointer to the JNIEnv
        ///
        static _JNIEnv* getCurrentJavaEnv();

    private:

        std::thread m_thread;   ///< The internal thread
    };

    // Include the implementation file
    #include <Jopnal/Utility/Inl/Thread.inl>
}

/// \class jop::Thread
/// \ingroup utility

#endif