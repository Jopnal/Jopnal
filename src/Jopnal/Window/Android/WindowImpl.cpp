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
#include JOP_PRECOMPILED_HEADER_FILE

#include <Jopnal/Window/Android/WindowImpl.hpp>

#ifdef JOP_OS_ANDROID

//////////////////////////////////////////////


namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings, Window& windowPtr)
    {
        
    }

    WindowImpl::~WindowImpl()
    {
        
    }

    //////////////////////////////////////////////

    void WindowImpl::swapBuffers()
    {
       
    }

    //////////////////////////////////////////////

    WindowLibHandle WindowImpl::getLibraryHandle()
    {
        return getNativeHandle();
    }

    //////////////////////////////////////////////

    WindowHandle WindowImpl::getNativeHandle()
    {
        return nullptr;
    }

    //////////////////////////////////////////////

    void WindowImpl::pollEvents()
    {
        
    }

    void WindowImpl::setMouseMode(const Mouse::Mode mode)
    {
        
    }

    //////////////////////////////////////////////

    void WindowImpl::setPosition(const int x, const int y)
    {
        
    }

    //////////////////////////////////////////////

    glm::ivec2 WindowImpl::getPosition() const
    {
        return glm::ivec2();
    }

    //////////////////////////////////////////////

    void WindowImpl::setSize(const int width, const int height)
    {
        
    }

    //////////////////////////////////////////////

    glm::ivec2 WindowImpl::getSize() const
    {
        glm::ivec2 s;

        return s;
    }

    //////////////////////////////////////////////

    Window* WindowImpl::getCurrentContextWindow()
    {
        
        return nullptr;
    }
}}

#endif