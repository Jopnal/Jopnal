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

#ifndef JOPE_MAINWINDOW_HPP
#define JOPE_MAINWINDOW_HPP

// Headers
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <Jopnal/Editor/Forms/ObjectWindow.hpp>
#include <Jopnal/Editor/Forms/OpenGLWindow.hpp>
#include <Jopnal/Editor/Forms/ToolWindow.hpp>
#include <Jopnal/Editor/Forms/PropertyWindow.hpp>

//////////////////////////////////////////////


namespace jope
{
    class MainWindow : public nana::form
    {
    public:

        MainWindow();



        nana::place m_layout;
        PropertyWindow m_propertyWindow;
        ObjectWindow m_objWindow;
        OpenGLWindow m_oglWindow;
        ToolWindow m_toolWindow;

    };
}

#endif