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

//////////////////////////////////////////////


namespace jop
{
    Color::Color()
        : r(255),
          g(255),
          b(255),
          a(255)
    {}

    Color::Color(const jop::uint8 _r, const jop::uint8 _g, const jop::uint8 _b, const jop::uint8 _a)
        : r(_r),
          g(_g),
          b(_b),
          a(_a)
    {}

    Color::Color(const jop::uint32 colors)
        : r(static_cast<jop::uint8>(colors >> 24)),
          g(static_cast<jop::uint8>(colors >> 16)),
          b(static_cast<jop::uint8>(colors >> 8)),
          a(static_cast<jop::uint8>(colors))
    {}

    Color::Color(const std::string& hexString)
    {
        std::stringstream ss;
        uint32 colors;
        ss << std::hex << hexString;
        ss >> colors;
        *this = Color(colors);
    }

    Color::Color(const float _r, const float _g, const float _b, const float _a)
        : r(static_cast<uint8>(_r * 255)),
          g(static_cast<uint8>(_g * 255)),
          b(static_cast<uint8>(_b * 255)),
          a(static_cast<uint8>(_a * 255))
    {

    }

    //////////////////////////////////////////////


    glm::fvec4 Color::asFloatVector() const
    {
        typedef glm::fvec4::value_type fType;

        return glm::fvec4(static_cast<fType>(r) / 255.f,
                          static_cast<fType>(g) / 255.f,
                          static_cast<fType>(b) / 255.f,
                          static_cast<fType>(a) / 255.f);

    }

    //////////////////////////////////////////////

    unsigned int Color::asInteger() const
    {
        return
        (
            static_cast<unsigned int>(r) << 24 |
            static_cast<unsigned int>(g) << 16 |
            static_cast<unsigned int>(b) << 8  |
            static_cast<unsigned int>(a)
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
        return r == other.r &&
               g == other.g &&
               b == other.b &&
               a == other.a;
    }

    //////////////////////////////////////////////

    bool Color::operator !=(const Color& other) const
    {
        return r != other.r ||
               g != other.g ||
               b != other.b ||
               a != other.a;
    }

} // namespace gp
