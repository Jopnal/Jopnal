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

	#include <Jopnal/Graphics/Color.hpp>
    
    #include <algorithm>
    #include <sstream>

#endif

//////////////////////////////////////////////


namespace jop
{
    Color::Color()
        : colors    (1.f),
          alpha     (1.f)
    {}

    Color::Color(const glm::vec3& rgb, const float32 a)
        : colors    (rgb),
          alpha     (a)
    {}

    Color::Color(const uint32 colors)
        : colors    (static_cast<float32>(static_cast<uint8>(colors >> 24)) / 255.f,
                    (static_cast<float32>(static_cast<uint8>(colors >> 16)) / 255.f),
                    (static_cast<float32>(static_cast<uint8>(colors >> 8))  / 255.f)),
          alpha     (static_cast<float32>(static_cast<uint8>(colors))       / 255.f)
    {}

    Color::Color(const std::string& hexString)
    {
        std::stringstream ss;
        uint32 col;
        ss << std::hex << hexString;
        ss >> col;
        *this = Color(col);
    }

    Color::Color(const float32 r, const float32 g, const float32 b, const float32 a)
        : colors    (r, g, b),
          alpha     (a)
    {}

    //////////////////////////////////////////////

    glm::vec4 Color::asRGBAVector() const
    {
        return glm::vec4(colors, alpha);
    }

    //////////////////////////////////////////////

    unsigned int Color::asInteger() const
    {
        return
        (
            static_cast<unsigned int>(std::abs(std::min(255.f, colors.r * 255.f))) << 24 |
            static_cast<unsigned int>(std::abs(std::min(255.f, colors.g * 255.f))) << 16 |
            static_cast<unsigned int>(std::abs(std::min(255.f, colors.b * 255.f))) << 8 |
            static_cast<unsigned int>(std::abs(std::min(255.f, alpha    * 255.f)))
        );
    }

    //////////////////////////////////////////////

    // Static member initialization.

    const Color Color::Black        (0x000000FF);
    const Color Color::White        (0xFFFFFFFF);
    const Color Color::Red          (0xFF0000FF);
    const Color Color::Green        (0x00FF00FF);
    const Color Color::Blue         (0x0000FFFF);
    const Color Color::Cyan         (0x00FFFFFF);
    const Color Color::Yellow       (0xFFFF00FF);
    const Color Color::Magenta      (0xFF00FFFF);
    const Color Color::Purple       (0x8000FFFF);
    const Color Color::Orange       (0xFF8000FF);
    const Color Color::Gray         (0x808080FF);
    const Color Color::Brown        (0x806400FF);
    const Color Color::Transparent  (0x00000000);

    //////////////////////////////////////////////

    bool Color::operator ==(const Color& other) const
    {
        return colors == other.colors;
    }

    //////////////////////////////////////////////

    bool Color::operator !=(const Color& other) const
    {
        return !(*this == other);
    }

    //////////////////////////////////////////////

    Color& Color::operator *=(const Color& right)
    {
        colors *= right.colors;
        alpha *= right.alpha;

        return *this;
    }

    //////////////////////////////////////////////

    Color Color::operator *(const float right) const
    {
        return Color(*this) *= right;
    }

    //////////////////////////////////////////////

    Color& Color::operator *=(const float right)
    {
        colors *= right;
        alpha *= right;

        return *this;
    }

    //////////////////////////////////////////////

    Color& Color::operator -=(const Color& right)
    {
        colors -= right.colors;
        alpha -= right.alpha;

        return *this;
    }

    //////////////////////////////////////////////

    Color& Color::operator +=(const Color& right)
    {
        colors += right.colors;
        alpha += right.alpha;

        return *this;
    }

    //////////////////////////////////////////////

    Color Color::operator *(const Color& right) const
    {
        return Color(*this) *= right;
    }

    //////////////////////////////////////////////

    Color Color::operator -(const Color& right) const
    {
        return Color(*this) -= right;
    }

    //////////////////////////////////////////////

    Color Color::operator +(const Color& right) const
    {
        return Color(*this) += right;
    }
}