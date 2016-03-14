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

#ifndef JOP_ENVIRONMENTRECORDER_HPP
#define JOP_ENVIRONMENTRECORDER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Graphics/Cubemap.hpp>
#include <memory>
#include <set>

//////////////////////////////////////////////


namespace jop
{
    class Drawable;

    class JOP_API EnvironmentRecorder final : public Component
    {
    private:

        EnvironmentRecorder(const EnvironmentRecorder& other, Object& newObj);

        JOP_DISALLOW_COPY_MOVE(EnvironmentRecorder);
        JOP_GENERIC_COMPONENT_CLONE(EnvironmentRecorder);

    public:

        EnvironmentRecorder(Object& obj);


        void record(const std::set<const Drawable*>& drawables, const std::set<const LightSource*>& lights);


    private:

        Cubemap m_map;

    };
}

#endif