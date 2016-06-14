//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//This software is provided 'as-is', without any express or implied
//warranty.In no event will the authors be held liable for any damages
//arising from the use of this software.
//
//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions :
//
//1. The origin of this software must not be misrepresented; you must not
//claim that you wrote the original software.If you use this software
//in a product, an acknowledgement in the product documentation would be
//appreciated but is not required.
//2. Altered source versions must be plainly marked as such, and must not be
//misrepresented as being the original software.
//3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4100)
#include <Jopnal/Graphics/stb/stb_rect_pack.h>
#pragma warning(pop)
#pragma warning(disable: 4505)

//////////////////////////////////////////////


namespace jop
{
    Font::Font(const std::string& name)
        : Resource      (name),
          m_pixelSize   (64)
    {}

    Font::~Font()
    {}

    //////////////////////////////////////////////

    const Texture2D& Font::getTexture()const
    {
        return Texture2D("");
    }

    //////////////////////////////////////////////

    float Font::getPixelSize() const
    {
        return static_cast<float>(m_pixelSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path, const int pixelSize)
    {
        return false;
    }

    //////////////////////////////////////////////

    bool Font::load(const int id, const int pixelSize)
    {
        return false;
    }

    //////////////////////////////////////////////

    bool Font::load(const int pixelSize)
    {
        
        return false;
    }
    
    std::pair<glm::vec2, glm::vec2> Font::getBounds(const int codepoint) const
    {
        
        return std::make_pair(glm::vec2(), glm::vec2());
    }

    //////////////////////////////////////////////

    float Font::getKerning(const int codepoint1, const int codepoint2) const
    {
        return 0.f;
    }

    //////////////////////////////////////////////
    
    void Font::getTextureCoordinates(const int codepoint, int* width, int* height, int* x, int* y) const
    {
        
    }

    //////////////////////////////////////////////

    Font& Font::getDefault()
    {
        static WeakReference<Font> defFont;

        if (defFont.expired())
        {
            defFont = static_ref_cast<Font>(ResourceManager::getEmptyResource<Font>("jop_default_font").getReference());

            //JOP_ASSERT_EVAL(defFont->load(jopr::defaultFont, sizeof(jopr::defaultFont), 32), "Failed to load default font!");

            defFont->setPersistence(0);
        }

        return *defFont;
    }
}