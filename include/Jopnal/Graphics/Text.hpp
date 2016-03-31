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
#include <Jopnal/Graphics/Texture2D.hpp>
#include <Jopnal/Graphics/Mesh.hpp>
#include <Jopnal/Graphics/Material.hpp>

//////////////////////////////////////////////

namespace jop
{
    class JOP_API Text
        :public GenericDrawable, private Mesh
    {
    public:

        /// \brief Constructor
        ///
        /// \param object Parent
        /// \param renderer Renderer
        ///
        Text(Object& object, Renderer& renderer);
        
        /// \brief Set string that is displayed
        ///
        /// \param string text
        ///
        void setString(const std::wstring& string);

        /// \brief Set string that is displayed and calculate vertex positions
        ///
        /// \param string text
        ///
        void setString(const std::string& string);

        /// \brief Set font
        ///
        /// \param font Font
        ///
        void setFont(Font& font);

        /// \brief Set color
        ///
        /// \param color Color
        ///
        void setColor(Color color);

    private:
        WeakReference<Font> m_font;
        Material m_material;
        std::wstring m_string;
        Color m_color = Color::White;
    };
}

#endif

/// \class Text
/// \ingroup graphics
///
/// Class for rendering text
/// 
///