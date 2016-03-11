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


namespace
{
    static const nana::appearance ns_oglAppearance
    (
        /* Decoration  */ true,
        /* Task bar    */ true,
        /* Floating    */ false,
        /* No activate */ false,
        /* Minimize    */ true,
        /* Maximize    */ false,
        /* Resizeable  */ true
    );
}

namespace jope
{
    OpenGLWindow::OpenGLWindow(nana::window parent)
        : nana::nested_form (parent),
          m_context         (nullptr)
    {
        HWND hwnd = reinterpret_cast<HWND>(this->native_handle());

        // Create context
        {
            HDC hdc = GetDC(hwnd);

            // Create glfw window to be able to load extensions
            jop::Window::Settings ws(false);
            jop::Window w(ws);

            wgl::sys::LoadFunctions(hdc);
            gl::sys::LoadFunctions();

            JOP_ASSERT(wgl::exts::var_ARB_create_context && wgl::exts::var_ARB_pixel_format, "Failed to load context creation functions!");

            const int pixelFormatAttribList[] =
            {
                wgl::DRAW_TO_WINDOW_ARB, 1,
                wgl::SUPPORT_OPENGL_ARB, 1,
                wgl::DOUBLE_BUFFER_ARB, 1,
                wgl::PIXEL_TYPE_ARB, wgl::TYPE_RGBA_ARB,
                wgl::COLOR_BITS_ARB, 32,
                wgl::DEPTH_BITS_ARB, 24,
                wgl::STENCIL_BITS_ARB, 8,
                0 // End of attributes list
            };
            int contextAttribs[] =
            {
                wgl::CONTEXT_MAJOR_VERSION_ARB, 3,
                wgl::CONTEXT_MINOR_VERSION_ARB, 3,
                wgl::CONTEXT_FLAGS_ARB, wgl::CONTEXT_CORE_PROFILE_BIT_ARB,
                0 // End of attributes list
            };

            int pixelFormat;
            UINT numFormats;
            wgl::ChoosePixelFormatARB(hdc, pixelFormatAttribList, NULL, 1, &pixelFormat, &numFormats);

            PIXELFORMATDESCRIPTOR pfd;
            SetPixelFormat(hdc, pixelFormat, &pfd);

            m_context = wgl::CreateContextAttribsARB(hdc, 0, contextAttribs);

            JOP_ASSERT(m_context != nullptr, "Couldn't create context!");
        }

        // Set the draw-through callback
        this->draw_through([this, hwnd]
        {
            static const glm::vec4 col = jop::Color(jop::SettingManager::getString("uDefaultWindowClearColor", "222222FF")).asRGBAFloatVector();

            makeCurrent();

            if (jop::Engine::hasCurrentScene())
                jop::Engine::getCurrentScene().updateTransformTree(nullptr, false);

            jop::Engine::getSharedScene().updateTransformTree(nullptr, false);

            RECT r;
            GetClientRect(hwnd, &r);

            gl::Viewport(0, 0, r.right - r.left, r.bottom - r.top);

            gl::ClearColor(col.r, col.g, col.b, col.a);
            gl::ClearDepth(1.f);
            gl::ClearStencil(0);
            gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);

            jop::Engine::renderFrame();

            SwapBuffers(GetDC(hwnd));
            gl::Flush();
        });

        // Disable the close button
        EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_GRAYED);

        // Add the updater sub system
        jop::Engine::createSubsystem<jope::WindowUpdater>(*this);
    }

    OpenGLWindow::~OpenGLWindow()
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_context);

        HWND w = reinterpret_cast<HWND>(this->native_handle());
        ReleaseDC(w, GetDC(w));
    }

    //////////////////////////////////////////////

    void OpenGLWindow::makeCurrent() const
    {
        if (!wglMakeCurrent(GetDC(reinterpret_cast<HWND>(this->native_handle())), m_context))
            JOP_DEBUG_ERROR("Failed to set context current");
    }

    //////////////////////////////////////////////

    void OpenGLWindow::draw() const
    {
        HWND w = reinterpret_cast<HWND>(this->native_handle());

        RECT r;
        GetClientRect(w, &r);
        InvalidateRect(w, &r, FALSE);
    }
}