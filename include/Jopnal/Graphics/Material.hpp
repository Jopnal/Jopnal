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

#ifndef JOP_MATERIAL_HPP
#define JOP_MATERIAL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <memory>
#include <array>

//////////////////////////////////////////////


namespace jop
{
    class Shader;
    class Texture;

    class JOP_API Material
    {
    public:

        /// Attribute data type
        ///
        typedef uint32 AttribType;

        /// Bit values to describe a material's attributes
        ///
        struct Attribute
        {
            enum : AttribType
            {
                AmbientConstant = 1,
                Material        = 1 << 1,
                Diffusemap      = 1 << 2,
                Specularmap     = 1 << 3,
                Emissionmap     = 1 << 4,
                Phong           = 1 << 5
            };
        };

        static const AttribType DefaultAttributes;

        /// The reflection attribute
        ///
        enum class Reflection
        {
            Ambient,
            Diffuse,
            Specular,
            Emission,
            Solid = Emission
        };

        /// The map attribute
        ///
        enum class Map
        {
            Diffuse,
            Specular,
            Emission
        };

        /// Predefined material properties
        ///
        enum class Property
        {
            Emerald,
            Jade,
            Obsidian,
            Pearl,
            Ruby,
            Turquoise,
            Brass,
            Bronze,
            Chrome,
            Copper,
            Gold,
            Silver,
            BlackPlastic,
            CyanPlastic,
            GreenPlastic,
            RedPlastic,
            WhitePlastic,
            YellowPlastic,
            BlackRubber,
            CyanRubber,
            GreenRubber,
            RedRubber,
            WhiteRubber,
            YellowRubber
        };

    public:

        /// \brief Default constructor
        ///
        Material();

        Material(const AttribType attributes);
        

        /// \brief Send this material to a shader
        ///
        /// \param shader Reference to the shader to send this material to
        ///
        void sendToShader(Shader& shader) const;


        /// \brief Set a reflection value
        ///
        /// \param reflection The reflection attribute
        /// \param color The reflection color
        ///
        /// \return Reference to self
        ///
        Material& setReflection(const Reflection reflection, const Color color);

        /// \brief Set the reflection values
        ///
        /// \param ambient The ambient reflection
        /// \param diffuse The diffuse reflection
        /// \param specular The specular reflection
        ///
        /// \return Reference to self
        ///
        Material& setReflection(const Color ambient, const Color diffuse, const Color specular, const Color emission);

        /// \brief Get a reflection value
        ///
        /// \param reflection The reflection attribute
        ///
        /// \return The reflection value
        ///
        Color getReflection(const Reflection reflection) const;

        /// \brief Set the shininess value
        ///
        /// \param value The shininess value
        ///
        /// \return Reference to self
        ///
        Material& setShininess(const float value);

        /// \brief Get the shininess value
        ///
        /// \return The shininess value
        ///
        float getShininess() const;


        /// \brief Set a map
        ///
        /// \param map The map attribute
        /// \param tex Reference to the texture
        ///
        /// \return Reference to self
        ///
        Material& setMap(const Map map, const Texture& tex);

        /// \brief Get a map
        ///
        /// \param map The map attribute
        ///
        /// \return Weak pointer to the texture. Empty if none bound
        ///
        WeakReference<const Texture> getMap(const Map map) const;


        void setAttributeField(const AttribType attribs);


        bool hasAttribute(const AttribType attrib) const;


        /// \brief Get the default material
        ///
        /// \return Reference to the default material
        ///
        static const Material& getDefault();

    private:

        std::array<Color, 4> m_reflection;                  ///< The reflection values
        AttribType m_attributes;                            
        float m_shininess;                                  ///< The shininess factor
        std::array<WeakReference<const Texture>, 3> m_maps; ///< An array with the bound maps

    };
}

#endif