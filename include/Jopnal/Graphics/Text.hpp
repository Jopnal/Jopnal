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

#ifndef JOP_TEXT_HPP
#define JOP_TEXT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/Font.hpp>
#include <Jopnal/Graphics/Texture.hpp>
#include <Jopnal/Graphics/Mesh.hpp>

//////////////////////////////////////////////

namespace jop
{
    class JOP_API Text
        :public Drawable, public Mesh
    {
    public:
        Text(Object& object, const std::string& ID);
        
        void setString(const std::string &string); //generates vertices
        void draw(const Camera&, const LightContainer&)override;


    private:
        WeakReference<Font> m_font;
    };
}

#endif

/// \class Text
/// \ingroup graphics
///
/// Class for rendering text
/// 
///