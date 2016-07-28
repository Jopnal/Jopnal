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

namespace detail
{
    template<typename Last>
    bool add(TextureAtlas& atlas, const Last& last)
    {
        static_assert(std::is_same<Last, Image>::value || std::is_same<Last, Texture2D>::value || std::is_convertible<Last, std::string>::value, "You can only add Texture2D or Image to Texture Atlas");
        return atlas.addTexture(last) != 0;
    }

    template<typename First, typename ...Rest>
    bool add(TextureAtlas& atlas, const First& first, const Rest&... rest)
    {
        return add(atlas, first) && add(atlas, rest...);
    }
}

template<typename... Args>
bool jop::TextureAtlas::load(const glm::uvec2& atlasSize, const Args&... args)
{
    return load(atlasSize) && detail::add(*this, args...);
}