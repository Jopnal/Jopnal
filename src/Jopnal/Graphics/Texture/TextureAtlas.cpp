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

#include <Jopnal/Graphics/Texture/TextureAtlas.hpp>

#endif

#include <STB/stb_rect_pack.h>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        struct AtlasPacker
        {
            stbrp_context           context;
            std::vector<stbrp_node> nodes;
            glm::uvec2              origin;
        };
    }

    //////////////////////////////////////////////

    TextureAtlas::TextureAtlas(const std::string& name)
        : Resource(name),
          m_texture("AtlasTexture"),
          m_textures(),
          m_packer(std::make_unique<detail::AtlasPacker>()),
          m_isSheet(false)
    {}

    TextureAtlas::~TextureAtlas()
    {}

    //////////////////////////////////////////////

    bool TextureAtlas::load(const glm::uvec2& atlasSize)
    {
        // Check if size is ok and load texture
        if (16 <= atlasSize.x && 16 <= atlasSize.y && m_texture.load(atlasSize, 4, true, false))
        {
            // Initialize packer
            m_packer->nodes.resize(atlasSize.x);
            m_packer->origin = glm::uvec2(0);
            // Initialize target - NOTE: num_nodes >= width
            stbrp_init_target(&m_packer->context, atlasSize.x, atlasSize.y, m_packer->nodes.data(), m_packer->nodes.size());

            Image defaultImg;

            // Create default glyph (2x2 white square in the corner)
            std::array<unsigned char, 16> data;
            data.fill(255);

            defaultImg.load(glm::uvec2(2), 4, data.data());

            return addTexture(defaultImg) == 0;
        }

        return false;
    }

    //////////////////////////////////////////////

    bool TextureAtlas::load(const std::string& path, LoadMode mode)
    {
        if (mode == LoadMode::TextureOnly)
            return m_texture.load(path, true, true, false);

        std::string file;

        if (!FileLoader::readTextfile(path, file))
            return false;

        json::Document doc;
        doc.ParseInsitu(&file[0]);

        if (!json::checkParseError(doc))
            return false;

        if (!doc.IsObject())
        {
            JOP_DEBUG_ERROR("Atlas root must be a JSON object");
            return false;
        }

        unsigned int slashPos = path.find_last_of("\\/");
        const std::string rootPath(slashPos == std::string::npos ? "" : path.substr(0, slashPos));

        m_isSheet = false;

        if (mode == LoadMode::Sheet)
        {
            if (doc.HasMember("path") && doc["path"].IsString() && doc.HasMember("textures") && doc["textures"].IsArray())
            {
                if (!m_texture.load(rootPath + doc["path"].GetString(), true, false))
                {
                    JOP_DEBUG_ERROR("Texture being loaded into atlas has invalid path");
                    return false;
                }

                for (auto& i : doc["textures"])
                {
                    if (!i.IsObject() || !i.HasMember("start") || !i.HasMember("end"))
                    {
                        JOP_DEBUG_WARNING("Texture in JSON file is not a JSON object or start/end is missing");
                        continue;
                    }

                    m_textures.emplace_back(glm::vec2(i["start"][0u].GetUint(), i["start"][1u].GetUint()) / glm::vec2(m_texture.getSize()),
                        glm::vec2(i["end"][0u].GetUint(), i["end"][1u].GetUint()) / glm::vec2(m_texture.getSize()));
                }

                m_isSheet = true;
            }
        }
        else
        {
            if (doc.HasMember("sizeX") && doc["sizeX"].IsUint() && doc.HasMember("sizeY") && doc["sizeY"].IsUint() &&
                doc.HasMember("textures") && doc["textures"].IsArray())
            {
                load(glm::uvec2(doc["sizeX"].GetUint(), doc["sizeY"].GetUint()));

                for (auto& i : doc["textures"])
                {
                    if (!i.IsString() || i.GetStringLength() == 0)
                    {
                        JOP_DEBUG_WARNING("Texture in JSON file missing a string or length is zero");
                        continue;
                    }

                    Image img;
                    if (!img.load(rootPath + i.GetString(), false))
                    {
                        JOP_DEBUG_WARNING("Failed to load atlas texture \"" << i.GetString() << "\"");
                        continue;
                    }

                    addTexture(img);
                }
            }
            else
            {                
                JOP_DEBUG_ERROR("Texture Atlas invalid size or no textures");
                return false;
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    unsigned int TextureAtlas::addTexture(const Image& image)
    {
        if (m_isSheet)
        {
            JOP_DEBUG_WARNING("Texture being packed into atlas is a sheet not a single image");
            return 0;
        }

        // Find an empty spot in the texture
        // Add padding to avoid pixel overflow (1 empty pixel)
        stbrp_rect rectangle = { 0, static_cast<stbrp_coord>(image.getSize().x + 1), static_cast<stbrp_coord>(image.getSize().y + 1) };
        stbrp_pack_rects(&m_packer->context, &rectangle, 1);
        rectangle.w -= 1;
        rectangle.h -= 1;

        if (rectangle.was_packed)
        {
            // Pass pixel data to texture
            if (image.getPixels())
            {
                m_texture.setPixels(glm::uvec2(rectangle.x, rectangle.y), image);

                m_textures.emplace_back(glm::vec2(rectangle.x, rectangle.y) / glm::vec2(m_texture.getSize()),
                    (glm::vec2(rectangle.x, rectangle.y) + glm::vec2(image.getSize())) / glm::vec2(m_texture.getSize()));

                return m_textures.size() - 1;
            }
            else
            {
                JOP_DEBUG_ERROR("Texture being packed into atlas has no data");
                return 0;
            }
        }
        else
        {
            JOP_DEBUG_ERROR("Failed to pack texture into atlas: Atlas full or texture too large");
            return 0;
        }

    }

    //////////////////////////////////////////////

    unsigned int TextureAtlas::addTexture(const Texture2D& texture)
    {
        return addTexture(texture.getImage());
    }

    //////////////////////////////////////////////

    unsigned int TextureAtlas::addTexture(const std::string& texturePath)
    {
        Image img;
        return img.load(texturePath, false) && addTexture(img) != 0;
    }

    unsigned int TextureAtlas::defineTexture(const glm::vec2& start, const glm::vec2& end)
    {
        m_textures.emplace_back(start / glm::vec2(m_texture.getSize()),end / glm::vec2(m_texture.getSize()));
        return m_textures.size() - 1;
    }

    //////////////////////////////////////////////

    std::pair<glm::vec2, glm::vec2> TextureAtlas::getCoordinates(const unsigned int index) const
    {
        if (m_textures.size() >= index)
            return m_textures[index];

        if (m_textures.empty())
        {
            JOP_DEBUG_ERROR("Invalid index given while getting coordinates from texture atlas");
            return std::make_pair(glm::vec2(0.f), glm::vec2(1.f));
        }

        return m_textures[0];
    }

    //////////////////////////////////////////////

    unsigned int TextureAtlas::getTextureAmount() const
    {
            return m_textures.size();
    }

    //////////////////////////////////////////////

    const jop::Texture2D& TextureAtlas::getTexture() const
    {
        if (m_texture.isValid())
            return m_texture;

        JOP_DEBUG_ERROR("Error texture: No texture found for texture atlas");
        return m_texture.getError();
    }

}