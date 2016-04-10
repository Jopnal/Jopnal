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
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    SkySphere::SkySphere(Object& obj, Renderer& renderer, const float radius)
        : Drawable(obj, renderer, "skysphere"),
          m_mesh("")
    {
        m_mesh.load(radius, 40, 40, true, true);
    }

    SkySphere::SkySphere(const SkySphere& other, Object& newObj)
        : Drawable(other, newObj),
          m_mesh("")
    {
        
    }

    //////////////////////////////////////////////

    void SkySphere::draw(const Camera* camera, const LightContainer& lights, Shader& shader) const
    {

    }
}