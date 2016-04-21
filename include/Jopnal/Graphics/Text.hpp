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
#include <Jopnal/Graphics/GenericDrawable.hpp>
#include <Jopnal/Graphics/Mesh.hpp>
#include <Jopnal/Graphics/Material.hpp>
#include <Jopnal/Graphics/Color.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Font;

    class JOP_API Text : public GenericDrawable
    {
    private:

        Text(const Text& other, Object& newObj);

        JOP_DISALLOW_COPY_MOVE(Text);
        JOP_GENERIC_COMPONENT_CLONE(Text);

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
        Text& setString(const std::wstring& string);

        /// \brief Set string that is displayed and calculate vertex positions
        ///
        /// \param string text
        ///
        Text& setString(const std::string& string);

        /// \brief Get the string
        ///
        /// \return Reference to the internal string
        ///
        const std::wstring& getString() const;

        /// \brief Set font
        ///
        /// \param font Font
        ///
        Text& setFont(const Font& font);

        /// \brief Set color
        ///
        /// \param color Color
        ///
        Text& setColor(const Color color);

        /// \brief Get the color
        ///
        /// \return The color
        ///
        Color getColor() const;

        void draw(const Camera* camera, const LightContainer& lights, Shader& shader) const override;

    private:

        WeakReference<const Font> m_font;
        Mesh m_mesh;
        Material m_material;
        std::wstring m_string;
    };
}

#endif

/// \class Text
/// \ingroup graphics
///
/// Class for rendering text
/// 
///