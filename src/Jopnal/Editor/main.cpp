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


int main(int argc, char* argv[])
{

    jop::Engine e("Jopnal Editor", argc, argv);
    e.createSubsystem<jop::SettingManager>();
    e.createSubsystem<jop::FileLoader>(argv[0]);
    e.createSubsystem<jop::ResourceManager>();
    e.createSubsystem<jop::ShaderManager>();
    e.createSubsystem<jope::CommandBuffer>();
    e.setPaused(true);
    e.setRenderingFrozen(true);

    jope::MainWindow form;

    e.createScene<jop::Scene>("New Scene");
    e.getCurrentScene().createChild("Default Camera")
    ->createComponent<jop::Camera>(e.getCurrentScene().getRenderer(), jop::Camera::Projection::Perspective);

    //e.getCurrentScene().createChild("Obj")
    //    ->createComponent<jop::GenericDrawable>(e.getCurrentScene().getRenderer());

    form.m_objWindow.buildObjectTree();

    wglMakeCurrent(NULL, NULL);

    std::thread t([]{JOP_MAIN_LOOP;});
    
    nana::exec();
    t.join();

    return EXIT_SUCCESS;
}