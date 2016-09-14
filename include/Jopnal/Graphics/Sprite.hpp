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

#ifndef JOP_SPRITE_HPP
#define JOP_SPRITE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/Material.hpp>
#include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Texture2D;

    class JOP_API Sprite : public Drawable
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(Sprite);

    public:

        /// \copydoc Drawable::Drawable(Object&,Renderer&,const bool)
        ///
        Sprite(Object& object, Renderer& renderer, const bool cull = true);

        /// \copydoc Drawable::Drawable(Object&,Renderer&,const RenderPass::Pass,const uint32,const bool)
        ///
        Sprite(Object& object, Renderer& renderer, const RenderPass::Pass pass, const uint32 weight, const bool cull = true);


        /// \copydoc Drawable::draw()
        ///
        virtual void draw(const ProjectionInfo& proj, const LightContainer& lights) const override;

        /// \brief Set the texture
        ///
        /// \param texture The texture to bind
        /// \param updateSize Update the size from the texture?
        ///
        /// \return Reference to self
        ///
        Sprite& setTexture(const Texture2D& texture, const bool updateSize);

        /// \brief Get the texture
        ///
        /// \return Reference to the texture
        ///
        const Texture2D& getTexture() const;

        /// \brief Set the size
        ///
        /// \param size The new size
        ///
        /// \return Reference to self
        ///
        Sprite& setSize(const glm::vec2& size);

        /// \brief Get the size
        ///
        /// \return The size of this sprite
        ///
        const glm::vec2& getSize() const;

        /// \brief Set the texture coordinates
        ///
        /// \param min The minimum coordinates
        /// \param max The maximum coordinates
        ///
        /// \return Reference to self
        ///
        Sprite& setTextureCoordinates(const glm::vec2& min, const glm::vec2& max);

        /// \brief Get the texture coordinates
        ///
        /// \return The texture coordinates. First = min, second = max
        ///
        const std::pair<glm::vec2, glm::vec2>& getTextureCoordinates() const;

    private:

        mutable WeakReference<const Texture2D> m_texture;   ///< Texture
        RectangleMesh m_mesh;                               ///< Mesh
        std::pair<glm::vec2, glm::vec2> m_texCoords;        ///< Texture coordinates
    };
}

/// \class jop::Sprite
/// \ingroup graphics

#endif