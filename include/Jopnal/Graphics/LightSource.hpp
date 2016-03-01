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

///////////////////////////////////////////

#ifndef JOP_LIGHTSOURCE_HPP
#define JOP_LIGHTSOURCE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <array>

///////////////////////////////////////////


namespace jop
{
    class Object;

    class JOP_API LightSource final : public Drawable
    {
    public:
        
        /// The light type
        ///
        enum class Type
        {
            Point,
            Spot,
            Directional
        };

        /// The intensity attribute
        ///
        enum class Intensity
        {
            Ambient,
            Diffuse,
            Specular
        };

        /// The attenuation attribute
        ///
        enum class Attenuation
        {
            Constant,
            Linear,
            Quadratic,
            Range
        };

        /// Some good predefined attenuation values
        ///
        enum class AttenuationPreset
        {
            _7, _13, _20, _32, _50, _65, _100, _160, _200, _320, _600, _3250
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to created object in object class 
        /// \param ID Unique string id for created light source as object
        ///
        LightSource(Object& object, const std::string& ID);

        JOP_GENERIC_CLONE(LightSource);


        /// \brief Overridden draw function
        ///
        /// Doesn't do anything.
        ///
        void draw(const Camera&, const LightContainer&) override;


        /// \brief Sets light type
        ///
        /// \param type The light type
        ///
        /// \return Reference to self
        ///
        LightSource& setType(const Type type);

        /// \brief Get the light type
        ///
        /// \return The light type
        ///
        Type getType() const;


        /// \brief Sets m_intensities array to color
        ///
        /// \param intensity Intensity type from enum
        /// \param color Color type as RGB vector
        ///
        /// \return Reference to self
        ///
        LightSource& setIntensity(const Intensity intensity, const Color color);
        
        /// \brief Overload function for setIntensity
        ///
        /// \param ambient Color value for ambient lighting
        /// \param diffuse Color value for diffuse lighting
        /// \param specular Color value for specular lighting
        ///
        /// \return Reference to self
        ///
        LightSource& setIntensity(const Color ambient, const Color diffuse, const Color specular);

        /// \brief Set an uniform intensity
        ///
        /// All the intensity values will be set to the given color value
        ///
        /// \param intensity The intensity to set
        ///
        /// \return Reference to self
        ///
        LightSource& setIntensity(const Color intensity);

        /// \brief Get an intensity value
        ///
        /// \param intensity The intensity type
        ///
        /// \return The intensity color
        ///
        Color getIntensity(const Intensity intensity) const;


        /// \brief Set a single attenuation value
        ///
        /// \param attenuation The attenuation type
        /// \param value The value to set
        ///
        /// \return Reference to self
        /// 
        LightSource& setAttenuation(const Attenuation attenuation, const float value);

        /// \brief Set the attenuation values
        ///
        /// \param constant The constant attenuation
        /// \param linear The linear attenuation
        /// \param quadratic The quadratic attenuation
        /// \param range Range of this light. Only affects culling
        ///
        /// \return Reference to self
        /// 
        LightSource& setAttenuation(const float constant, const float linear, const float quadratic, const float range);

        /// \brief Set the attenuation values using a preset
        ///
        /// \param preset The preset
        ///
        /// \return Reference to self
        ///
        LightSource& setAttenuation(const AttenuationPreset preset);

        /// \brief Get an attenuation value
        ///
        /// \param attenuation The attenuation type
        ///
        /// \return The attenuation value
        ///
        float getAttenuation(const Attenuation attenuation) const;

        /// \brief Get the attenuation values as a vector
        ///
        /// x = constant
        /// y = linear
        /// z = quadratic
        /// Doesn't include the range parameter
        ///
        /// \return A vector with the attenuation values
        ///
        glm::vec3 getAttenuationVec() const;


        /// \brief Set the cutoff
        ///
        /// This only affects spot lights.
        /// The inner and outer cutoff control the dimming near
        /// the edges of the spot light.
        ///
        /// \param inner The inner cutoff in radians. The light will be at its
        ///              maximum intensity inside this angle
        /// \param outer The outer cutoff in radians
        ///
        /// \return Reference to self
        ///
        LightSource& setCutoff(const float inner, const float outer);

        /// \brief Get the cutoff values
        ///
        /// x = inner
        /// y = outer
        ///
        /// \return Vector with the cutoff values
        ///
        const glm::vec2& getCutoff() const;


        /// \brief Get the setting for the maximum number of lights
        ///
        /// \param type The light type
        ///
        /// \return The maximum number of lights
        ///
        static unsigned int getMaximumLights(const Type type);
        
    private:

        Type m_type;                        ///< The light type
        std::array<Color, 3> m_intensities; ///< The intensities
        glm::vec4 m_attenuation;            ///< The attenuation values    
        glm::vec2 m_cutoff;                 ///< Spot light cutoff
    };

    /// \brief Container for lights
    ///
    /// Meant to be passed to drawable
    ///
    class LightContainer
    {
    public:

        typedef std::vector<const LightSource*> ContainerType;

    public:

        /// \brief Default constructor
        ///
        /// Reserves storage according to the maximum light amounts
        ///
        LightContainer();


        /// \brief Check is this container is empty
        ///
        /// \return True if empty
        ///
        bool empty() const;

        /// \brief Clear this container
        ///
        void clear();

        /// \brief Send the contained lights to shader
        ///
        /// \param shader The shader to send the lights to
        /// \param camera The camera used currently
        ///
        void sendToShader(Shader& shader, const Camera& camera) const;

        /// \brief Access the individual containers for each light type
        ///
        /// \param type The light type
        ///
        /// \return Reference to the container
        ///
        ContainerType& operator [](const LightSource::Type type);

        /// \copydoc operator[]
        ///
        const ContainerType& operator [](const LightSource::Type type) const;

    private:

        std::array<ContainerType, 3> m_container;   ///< Containers for each type of light
    };
}

#endif

/// \class LightSource
/// \ingroup Graphics
///
/// Do explanation about the class