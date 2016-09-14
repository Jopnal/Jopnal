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
    class ShaderProgram;
    class Texture;

    class JOP_API Material final : public Resource
    {
    public:

        JOP_DISALLOW_COPY_MOVE(Material);

        Material(const Material& other, const std::string& newName);

        friend class ShaderAssembler;

    public:

        enum class LightingModel : uint64
        {
            None        = 0ull, ///< No lighting

            /// Vertex-base lighting. Lighting is calculated for each vertex
            /// before interpolating it for each pixel.
            ///
            /// This model is much faster on GPU's where pixel fill rate is
            /// the bottleneck.
            ///
            /// This model is the default for mobile.
            ///
            Gouraud     = 1ull << 63,

            /// Flat lighting, disables normal interpolation between vertices,
            /// which results in a single polygon having a uniform color.
            ///
            /// \warning Not available on GLES 2.0, Gouraud model will be used instead
            ///
            Flat        = Gouraud | Gouraud >> 1,

            /// Fragment-based phong lighting model. The lighting value
            /// is calculated for each pixel individually.
            ///
            /// This is the most expensive lighting model, and thus is not
            /// recommended to be used in mobile environments.
            ///
            /// \see Gouraud
            ///
            Phong       = Gouraud >> 2,

            /// Slightly modified version of Phong. This uses a half-way
            /// normal reflection vector to calculate the specular highlight,
            /// which makes it slightly stronger.
            ///
            /// This model is marginally faster than Phong.
            ///
            /// This model is the default for desktop.
            ///
            BlinnPhong  = Phong | Phong >> 1,

            // To be implemented
            //
            // Toon
            // Fresnel?
            // CookTorrace?
            // OrenNyar?
            // Minnaert?

            /// Default lighting mode
            ///
            /// Gouraud on GLES, BlinnPhong elsewhere.
            ///
            Default     =
            #ifdef JOP_OPENGL_ES
                Gouraud
            #else
                BlinnPhong
            #endif
        };

        /// The reflection attribute
        ///
        /// \note These will only affect rendering when lighting is enabled.
        ///
        enum class Reflection
        {
            Ambient,    ///< Ambient light reflection, unaffected by object orientation or shadows
            Diffuse,    ///< Diffuse reflection
            Specular,   ///< Specular reflection
            Emission    ///< Emissive reflection, which is simply added to the base light value
        };

        /// The map attribute
        ///
        enum class Map : uint64
        {
            /// Essentially the "base" texture for an object
            Diffuse0,

            /// The specular highlight value will be multiplied by this texture's value
            Specular,

            /// The value of this texture will be added to the final fragment value,
            /// after multiplication with the emissive reflection value
            Emission,

            /// Cube map to be used as an environment reflection
            Environment,

            /// To be used with an environment map. The value of this texture will be
            /// multiplied with that of the environment map
            Reflection,

            /// 8-bit alpha map. The opacity of each fragment will be multiplied by this
            /// texture's value
            Opacity,

            /// The value of this map will be multiplied with the shininess value
            ///
            /// Not available when using vertex-based lighting models.
            ///
            Gloss,
            
            /// For internal use. Never use this
            __Last
        };

        /// All the lighting model attributes combined
        ///
        /// For internal use only.
        ///
        static const uint64 LightingAttribs;

        /// All lighting model attributes that are
        /// fragment based.
        ///
        /// For internal use only.
        ///
        static const uint64 FragLightingAttribs;

    private:

        typedef std::array<WeakReference<const Texture>, static_cast<int>(Map::__Last) - 1> MapArray;

    public:

        /// \copydoc Resource::Resource(const std::string&)
        ///
        /// By default the material defines no functionality (only Drawable's
        /// color will be used in rendering).
        ///
        Material(const std::string& name);
        

        /// \brief Send this material to a shader
        ///
        /// \param shader Reference to the shader to send this material to
        ///
        void sendToShader(ShaderProgram& shader) const;

        /// \brief Set the lighting model
        ///
        /// \param model The lighting model to set
        ///
        /// \return Reference to self
        ///
        Material& setLightingModel(const LightingModel model);

        /// \brief Get the lighting model
        ///
        /// \return The lighting model
        ///
        LightingModel getLightingModel() const;

        /// \brief Set a reflection value
        ///
        /// \param reflection The reflection attribute
        /// \param color The reflection color
        ///
        /// \return Reference to self
        ///
        Material& setReflection(const Reflection reflection, const Color& color);

        /// \brief Set the reflection values
        ///
        /// \param ambient The ambient reflection
        /// \param diffuse The diffuse reflection
        /// \param specular The specular reflection
        /// \param emission The emission value
        ///
        /// \return Reference to self
        ///
        Material& setReflection(const Color& ambient, const Color& diffuse, const Color& specular, const Color& emission);
   
        /// \brief Get a reflection value
        ///
        /// \param reflection The reflection attribute
        ///
        /// \return The reflection value
        ///
        const Color& getReflection(const Reflection reflection) const;

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
        /// \param map The map enum
        /// \param tex Reference to the texture
        ///
        /// \return Reference to self
        ///
        Material& setMap(const Map map, const Texture& tex);

        /// \brief Remove a map
        ///
        /// \param map The map to remove
        ///
        /// \return Reference to self
        ///
        Material& removeMap(const Map map);

        /// \brief Get a map
        ///
        /// \param map The map enum
        ///
        /// \return Pointer to the texture. nullptr if none bound
        ///
        const Texture* getMap(const Map map) const;

        /// \brief Check if this material has potential transparency
        ///
        /// \return True if this material has potential transparency
        ///
        bool hasAlpha() const;

        /// \brief Get the shader for this material
        ///
        /// \return Reference to the shader
        ///
        ShaderProgram& getShader() const;

        /// \brief Get the internal attribute field
        ///
        /// This is for internal use only.
        ///
        /// \return The attribute field
        ///
        uint64 getAttributes() const;

        /// \brief Get the default material
        ///
        /// The default material has no attributes, meaning only the
        /// Drawable color will be used.
        ///
        /// \return Reference to the default material
        ///
        static Material& getDefault();

    private:

        static std::string getShaderPreprocessorDef(const uint64 attributes);


        std::array<Color, 4> m_reflection;              ///< The reflection values
        MapArray m_maps;                                ///< An array with the bound maps
        mutable WeakReference<ShaderProgram> m_shader;  ///< Shader
        uint64 m_attributes;                            ///< The attribute bit field
        float m_reflectivity;                           ///< The reflectivity value
        float m_shininess;                              ///< The shininess factor
        mutable bool m_updateShader;                    ///< Does the shader need updating?
    };
}

/// \class jop::Material
/// \ingroup graphics

#endif