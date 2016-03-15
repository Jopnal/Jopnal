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
#include <Jopnal/Precompiled.hpp>
#include <Jopnal/Graphics/stb/stb_image.h>

//////////////////////////////////////////////


namespace jop
{
    Cubemap::Cubemap(const std::string& name)
        : Texture(name, gl::TEXTURE_CUBE_MAP),
          m_width(0),
          m_height(0),
          m_bytesPerPixel(0)
    {}

    //////////////////////////////////////////////

    bool Cubemap::load(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front)
    {


        const std::string* const paths[] =
        {
            &right, &left,
            &top, &bottom,
            &back, &front
        };

        for (std::size_t i = 0; i < 6; ++i)
        {
            std::vector<unsigned char> buf;
            if (!FileLoader::read(*paths[i], buf))
            {

                return false;
            }

            unsigned char* pix = stbi_load_from_memory(buf.data(), 0, &m_width, &m_height, &m_bytesPerPixel, 0);

            if (!pix)
            {

                stbi_image_free(pix);
                return false;
            }

            //gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 

            stbi_image_free(pix);
        }

        return true;
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const int /*width*/, const int /*height*/, const int /*bpp*/)
    {
        return true;
    }
}