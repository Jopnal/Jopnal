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
#include <Jopnal/Core/Resource.hpp>
#include <memory>
#include <array>

//////////////////////////////////////////////


namespace jop
{
    class Shader;
    class Texture;

    class JOP_API Material final : public Resource
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
                // Misc
                AmbientConstant = 1,

                // Maps
                DiffuseMap      = 1 << 2,
                SpecularMap     = 1 << 3,
                EmissionMap     = 1 << 4,
                EnvironmentMap  = 1 << 5,
                ReflectionMap   = 1 << 6,
                //NormalMap
                //ParallaxMap

                // Lighting models
                Phong           = 1 << 12,
                BlinnPhong      = Phong, //
                Gouraud         = Phong, // TODO: Implement these
                Flat            = Phong, //

                // Bundles
                Default         = DiffuseMap,
                DefaultLighting = AmbientConstant | BlinnPhong,

                // For internal functionality, do not use
                RecordEnv       = 1u << 31,
                Lighting        = Phong | BlinnPhong | Gouraud | Flat
            };
        };

        /// The reflection attribute
        ///
        enum class Reflection
        {
            Ambient,
            Diffuse,
            Specular,
            Emission,
            //Normal
            //Parallax

            Solid = Emission
        };

        /// The map attribute
        ///
        enum class Map
        {
            Diffuse = 1,
            Specular,
            Emission,
            Environment,
            Reflection,

            /// For internal use. Never use this
            Last
        };

        /// Predefined material properties
        ///
        enum class Preset
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
        /// \param name Name of this material
        ///
        Material(const std::string& name, const bool autoAttributes = true);

        /// \brief Overloaded constructor for initializing with attributes
        ///
        /// \param name Name of this material
        /// \param attributes The initial attributes
        ///
        Material(const std::string& name, const AttribType attributes);
        

        /// \brief Send this material to a shader
        ///
        /// \param shader Reference to the shader to send this material to
        ///
        void sendToShader(Shader& shader, const Camera* camera) const;

        /// \brief Get the shader
        ///
        /// \return Pointer to the shader
        ///
        Shader* getShader() const;


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
        /// \param emission The emission value
        ///
        /// \return Reference to self
        ///
        Material& setReflection(const Color ambient, const Color diffuse, const Color specular, const Color emission);

        /// \brief Set the reflection values using a preset
        ///
        /// \param preset The preset to use
        ///
        /// \return Reference to this
        ///
        Material& setReflection(const Preset preset);
   
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


        /// \brief Set the reflectivity
        ///
        /// Do not confuse this with reflection. What this defines is how
        /// strongly an environment map is reflected. 0 means there's no
        /// reflection and 1 makes the surface mirror-like. This attribute
        /// has no effect if there's no environment map
        ///
        /// \param reflectivity The reflectivity value
        ///
        /// \return Reference to self
        ///
        Material& setReflectivity(const float reflectivity);

        /// \brief Get the reflectivity value
        ///
        /// \return The reflectivity value
        ///
        float getReflectivity() const;


        /// \brief Set a map
        ///
        /// \param map The map attribute
        /// \param tex Reference to the texture
        ///
        /// \return Reference to self
        ///
        Material& setMap(const Map map, const Texture& tex);

        Material& removeMap(const Map map);

        /// \brief Get a map
        ///
        /// \param map The map attribute
        ///
        /// \return Weak pointer to the texture. Empty if none bound
        ///
        WeakReference<const Texture> getMap(const Map map) const;


        /// \brief Set the attribute bit field
        ///
        /// \param attribs The attributes to set
        ///
        Material& setAttributeField(const AttribType attribs);

        /// \brief Get the attribute bit field
        ///
        /// \return The attribute bit field
        ///
        AttribType getAttributeField() const;

        /// \brief Check if this material has a specific attribute
        ///
        /// \param attrib The attribute to check
        ///
        /// \return True if does have the attribute
        ///
        bool hasAttribute(const AttribType attrib) const;

        bool hasAttributes(const AttribType attribs) const;

        bool compareAttributes(const AttribType attribs) const;

        Material& addAttributes(const AttribType attribs);

        Material& removeAttributes(const AttribType attribs);


        /// \brief Get the default material
        ///
        /// \return Reference to the default material
        ///
        static Material& getDefault();

    private:

        std::array<Color, 4> m_reflection;  ///< The reflection values
        float m_reflectivity;               ///< The reflectivity value
        AttribType m_attributes;            ///< The attribute bit field
        float m_shininess;                  ///< The shininess factor
        std::array
        <
            WeakReference<const Texture>,
            static_cast<int>(Map::Last) - 1
        > m_maps;                               ///< An array with the bound maps
        mutable WeakReference<Shader> m_shader; ///< Shader fitting the attributes
        mutable bool m_attributesChanged;       ///< Have the attributes been changed?
        bool m_autoAttribs;                     ///< Use automatic attributes?
    };
}

#endif