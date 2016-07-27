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

#ifndef JOP_TEXTUREATLAS_HPP
#define JOP_TEXTUREATLAS_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Graphics/Image.hpp>
#include <Jopnal/Graphics/Texture/Texture2D.hpp>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        struct AtlasPacker;
    }

    class JOP_API TextureAtlas : public Resource
    {
    public:

        enum class LoadMode
        {
            Separate,
            Sheet,
            TextureOnly
        };

    public:
        
        /// \brief Constructor
        ///
        /// \param name Name for the resource
        ///
        TextureAtlas(const std::string& name);

        /// \brief Destructor
        ///
        ~TextureAtlas();

        /// \brief Load atlas
        ///
        /// \param atlasSize Size of the atlas to be created.
        ///
        bool load(const glm::uvec2& atlasSize);

        /// \brief Load atlas
        ///
        /// Loads atlas of given size and with variable amount of given Images, Texture2D's or filepath's
        /// using the addTexture method for each argument
        ///
        /// \param atlasSize Size of the atlas to be created.
        /// \param args Variable amount of Texture2D's, Image's or file paths
        ///
        template<typename... Args>
        bool load(const glm::uvec2& atlasSize, const Args&... args);

        /// \brief Load from JSON file
        ///
        /// \param mode Load mode - if you have a complete sheet use LoadMode::Sheet, otherwise use LoadMode::Seperate (which is on by default)
        ///
        bool load(const std::string& path, const LoadMode mode = LoadMode::Separate);

        /// \brief Add texture from image to the atlas
        ///
        /// \param image Image to be added
        ///
        unsigned int addTexture(const Image& image);

        /// \brief Add texture to the atlas
        ///
        /// \param texture Texture to be added
        ///
        unsigned int addTexture(const Texture2D& texture);

        /// \brief Add texture from file to atlas
        ///
        /// \param texturePath Path to the texture in file
        ///
        unsigned int addTexture(const std::string& texturePath);

        /// \brief Define a texture
        ///
        /// Adds a texture to memory with given dimension
        ///
        unsigned int defineTexture(const glm::vec2& start, const glm::vec2& end);

        /// \brief Get texture location inside atlas
        ///
        /// \param index Index of the texture
        ///
        std::pair<glm::vec2, glm::vec2> getCoordinates(const unsigned int index) const;

        /// \brief Get amount of textures inside atlas
        ///
        unsigned int getTextureAmount() const;

        /// \brief Get the texture of atlas
        ///
        /// If no textures have been added to atlas default texture will be returned
        ///
        const Texture2D& getTexture() const;

    private:

        std::vector<std::pair<glm::vec2, glm::vec2>> m_textures;    ///< Coordinates for each texture inside atlas
        Texture2D m_texture;                                        ///< Texture of the atlas     
        std::unique_ptr<detail::AtlasPacker> m_packer;              ///< TextureAtlas packer
        bool m_isSheet;                                             ///< Is atlas a sheet?
    };
#include <Jopnal/Graphics/Inl/TextureAtlas.inl>
}

#endif