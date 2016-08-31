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

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/Sprite.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Sprite::Sprite(Object& object, Renderer& renderer, const RenderPass::Pass pass)
        : Drawable      (object, renderer, pass),
          m_material    ("", true),
          m_mesh        ("")
    {
        m_material.setMap(Material::Map::Diffuse, Texture2D::getDefault());
        setModel(Model(m_mesh, m_material));
    }

    Sprite::Sprite(Object& object, RenderPass& pass)
        : Drawable      (object, pass),
          m_material    ("", true),
          m_mesh        ("")
    {
        m_material.setMap(Material::Map::Diffuse, Texture2D::getDefault());
        setModel(Model(m_mesh, m_material));
    }

    Sprite::Sprite(const Sprite& other, Object& newObj)
        : Drawable      (other, newObj),
          m_material    ("", true),
          m_mesh        ("")
    {
        if (other.getTexture())
            m_material.setMap(Material::Map::Diffuse, *static_cast<const Texture2D*>(other.getTexture()));

        setModel(Model(m_mesh, m_material));
    }

    //////////////////////////////////////////////

    Sprite& Sprite::setTexture(const Texture2D& texture, const bool updateSize)
    {
        m_material.setMap(Material::Map::Diffuse, texture);

        if (updateSize)
            return setSize(texture.getSize());

        return *this;
    }

    //////////////////////////////////////////////

    const Texture* Sprite::getTexture() const
    {
        return m_material.getMap(Material::Map::Diffuse);
    }

    //////////////////////////////////////////////

    Sprite& Sprite::setSize(const glm::vec2& size)
    {
        m_mesh.load(size);
        return *this;
    }

    //////////////////////////////////////////////

    const glm::vec2& Sprite::getSize() const
    {
        return m_mesh.getSize();
    }
}