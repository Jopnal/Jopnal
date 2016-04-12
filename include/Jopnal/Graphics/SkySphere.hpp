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

#ifndef JOP_SKYBOX_HPP
#define JOP_SKYBOX_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/SphereMesh.hpp>
#include <Jopnal/Graphics/Material.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API SkySphere : public Drawable
    {
    private:

        SkySphere(const SkySphere& other, Object& newObj);

        JOP_DISALLOW_COPY_MOVE(SkySphere);
        JOP_GENERIC_COMPONENT_CLONE(SkySphere);

    public:

        SkySphere(Object& obj, Renderer& renderer, const float radius = 2.f);


        void draw(const Camera* camera, const LightContainer& lights, Shader& shader) const override;

        void setMap(const Texture2D& map);

        const Texture* getMap() const;

    private:

        SphereMesh m_mesh;
        Material m_material;
    };
}

#endif