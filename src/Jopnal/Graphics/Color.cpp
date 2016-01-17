// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
    {
        
    }

    Color::Color(const jop::uint8 _r, const jop::uint8 _g, const jop::uint8 _b, const jop::uint8 _a)
        : r(_r),
          g(_g),
          b(_b),
          a(_a)
    {
    
    }

    Color::Color(const jop::uint32 colors)
        : r(static_cast<jop::uint8>(colors >> 24)),
          g(static_cast<jop::uint8>(colors >> 16)),
          b(static_cast<jop::uint8>(colors >> 8)),
          a(static_cast<jop::uint8>(colors))
    {
        // Nothing more to do here
    }

    Color::Color(const std::string& hexString)
    {
        std::stringstream ss;
        uint32 colors;
        ss << std::hex << hexString;
        ss >> colors;
        *this = Color(colors);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////


    glm::fvec4 Color::asFloatVector() const
    {
        typedef glm::fvec4::value_type fType;

        return glm::fvec4(static_cast<fType>(r) / 255.f,
                          static_cast<fType>(g) / 255.f,
                          static_cast<fType>(b) / 255.f,
                          static_cast<fType>(a) / 255.f);

    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

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

    /////////////////////////////////////////////////////////////////////////////////////////////////

    bool Color::operator ==(const Color& other) const
    {
        return r == other.r &&
               g == other.g &&
               b == other.b &&
               a == other.a;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    bool Color::operator !=(const Color& other) const
    {
        return r != other.r ||
               g != other.g ||
               b != other.b ||
               a != other.a;
    }

} // namespace gp
