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
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/place.hpp>

//////////////////////////////////////////////


namespace
{
    static const nana::appearance ns_mainAppearance
    (
        /* Decoration  */ true,  
        /* Task bar    */ true,
        /* Floating    */ false,
        /* No activate */ false,
        /* Minimize    */ true,
        /* Maximize    */ true,
        /* Resizeable  */ true
    );

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

    HGLRC ns_context = nullptr;

    void enableOGL(nana::nested_form& form)
    {
        HDC hdc = GetDC(reinterpret_cast<HWND>(form.native_handle()));

        // Create glfw window to be able to load extensions
        jop::Window::Settings ws(false);
        jop::Window w(ws);

        wgl::sys::LoadFunctions(hdc);
        gl::sys::LoadFunctions();
        
        JOP_ASSERT(wgl::exts::var_ARB_create_context && wgl::exts::var_ARB_pixel_format, "Failed to load context creation functions!");

        const int iPixelFormatAttribList[] =
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
        int iContextAttribs[] =
        {
            wgl::CONTEXT_MAJOR_VERSION_ARB, 3,
            wgl::CONTEXT_MINOR_VERSION_ARB, 3,
            wgl::CONTEXT_FLAGS_ARB, wgl::CONTEXT_CORE_PROFILE_BIT_ARB,
            0 // End of attributes list
        };

        int iPixelFormat, iNumFormats;
        wgl::ChoosePixelFormatARB(hdc, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

        PIXELFORMATDESCRIPTOR pfd;
        SetPixelFormat(hdc, iPixelFormat, &pfd);

        ns_context = wgl::CreateContextAttribsARB(hdc, 0, iContextAttribs);

        JOP_ASSERT(ns_context != nullptr, "Couldn't create context!");
    }

    void destroyOGL(nana::nested_form& form)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(ns_context);

        HWND w = reinterpret_cast<HWND>(form.native_handle());
        ReleaseDC(w, GetDC(w));
    }
}

namespace jope
{
    MainWindow::MainWindow()
        : nana::form(nana::API::make_center(1280, 720), ns_mainAppearance),
          m_layout(*this),
          m_oglWindow(*this, nana::rectangle(300, 300, 800, 600), ns_oglAppearance),
          m_objWindow(*this)
    {
        this->caption("Jopnal Editor");
        this->bgcolor(nana::color(nana::color_rgb(0x222222)));
        
        this->events().unload([this](const nana::arg_unload& u)
        {
            bool close = false;

            m_oglWindow.hide();

            nana::nested_form dialog(*this, nana::API::make_center(*this, 200, 40), nana::appearance(true, false, true, false, false, false, false));
            dialog.z_order(nullptr, nana::z_order_action::topmost);
            dialog.caption("Exit without saving?");
            dialog.bgcolor(this->bgcolor());
            
            nana::place pl(dialog);
            pl.div("<ab margin=[6,16] gap=10>");

            nana::button yes(dialog, "Yes");
            yes.events().click([&dialog, &close]
            {
                close = true;
                dialog.close();
            });
            
            nana::button no(dialog, "No");
            no.events().click([&dialog]
            {
                dialog.close();
            });
            
            pl.field("ab") << yes << no;

            pl.collocate();
            dialog.show();

            nana::API::wait_for(dialog);
            
            if (close)
                jop::Engine::exit();
            else
            {
                u.cancel = true;
                m_oglWindow.show();
            }
        });
        
        m_layout.div("<obj weight=10% margin=[0,0,48]><ogl margin=[0, 18, 48, 20]>");
        m_layout.field("obj") << m_objWindow;
        m_layout.field("ogl") << m_oglWindow;
        
        // Keep last
        this->zoom(true);
        this->show();

        m_oglWindow.show();
        EnableMenuItem(GetSystemMenu(reinterpret_cast<HWND>(m_oglWindow.native_handle()), FALSE), SC_CLOSE, MF_GRAYED);

        enableOGL(m_oglWindow);

        m_oglWindow.draw_through([this]
        {
            static const glm::vec4 col = jop::Color(jop::SettingManager::getString("uDefaultWindowClearColor", "222222FF")).asRGBAFloatVector();

            makeContextCurrent();

            if (jop::Engine::hasCurrentScene())
                jop::Engine::getCurrentScene().updateTransformTree();

            jop::Engine::getSharedScene().updateTransformTree();

            RECT r;
            GetClientRect(reinterpret_cast<HWND>(m_oglWindow.native_handle()), &r);

            gl::Viewport(0, 0, r.right - r.left, r.bottom - r.top);

            gl::ClearColor(col.r, col.g, col.b, col.a);
            gl::ClearDepth(1.f);
            gl::ClearStencil(0);
            gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);

            jop::Engine::renderFrame();

            SwapBuffers(GetDC(reinterpret_cast<HWND>(m_oglWindow.native_handle())));
            gl::Flush();
        });
    }

    MainWindow::~MainWindow()
    {
        destroyOGL(m_oglWindow);
    }

    //////////////////////////////////////////////

    void MainWindow::makeContextCurrent()
    {
        if (!wglMakeCurrent(GetDC(reinterpret_cast<HWND>(m_oglWindow.native_handle())), ns_context))
            JOP_DEBUG_ERROR("Failed to set context current");
    }

    //////////////////////////////////////////////

    void MainWindow::draw()
    {
        HWND w = reinterpret_cast<HWND>(m_oglWindow.native_handle());
        
        RECT r;
        GetClientRect(w, &r);
        InvalidateRect(w, &r, FALSE);
    }
}