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

#ifndef JOPE_OPENGLWINDOW_HPP
#define JOPE_OPENGLWINDOW_HPP

// Headers
#include <nana/gui/wvl.hpp>
#include <nana/gui/timer.hpp>
#include <Jopnal/Graphics/RenderTarget.hpp>

//////////////////////////////////////////////


struct HGLRC__;

namespace jope
{
    class OpenGLWindow final : public nana::nested_form, public jop::RenderTarget
    {
    public:

        OpenGLWindow(nana::window parent, std::unique_ptr<jop::Engine>& eng);

        ~OpenGLWindow() override;

        bool bind() const override;

        glm::uvec2 getSize() const override;

    private:

        static OpenGLWindow* m_instance;

        nana::timer m_timer;
        HGLRC__* m_context;
        unsigned int m_vao;
        std::unique_ptr<jop::Engine>& m_engRef;

    };
}

#endif