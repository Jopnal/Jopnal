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

#ifndef JOP_WINDOWDESKTOP_HPP
#define JOP_WINDOWDESKTOP_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Window/Window.hpp>
#include <Jopnal/Window/WindowHandle.hpp>
#include <GLFW/glfw3.h>

//////////////////////////////////////////////


namespace jop { namespace detail
{
    class WindowImpl
    {
    private:

        JOP_DISALLOW_COPY_MOVE(WindowImpl);
    
    public:

        explicit WindowImpl(const Window::Settings& settings);

        ~WindowImpl();


        void swapBuffers();

        GLFWwindow* getLibraryHandle();

        WindowHandle getNativeHandle();

        static void pollEvents();

        void setMouseMode(const Mouse::Mode mode);

        void setPosition(const int x, const int y);

        glm::ivec2 getPosition() const;

        void setSize(const int width, const int height);

        glm::ivec2 getSize() const;

    private:

        GLFWwindow* m_window;
        unsigned int m_vertexArray;

    };
}}

#endif