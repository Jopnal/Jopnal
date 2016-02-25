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


        /// \brief Sets light types to type identifier and returns it
        ///
        /// \param type Light type from Type enum
        ///
        /// \return Reference to self
        ///
        LightSource& setType(const Type type);

        /// \briefGet the light type
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
        
        /// \brief Overload function for setIntensity, returns set intensities for all types from enum
        ///
        /// \param ambient Color value for ambient lighting
        /// \param diffuse Color value for diffuse lighting
        /// \param specular Color value for specular lighting
        ///
        /// \return Reference to self
        ///
        LightSource& setIntensity(const Color ambient, const Color diffuse, const Color specular);

        LightSource& setIntensity(const Color intensity);

        /// \brief Getter returning array of intensity types
        ///
        /// \param intensity Identifier for intensities
        ///
        /// \return The intensity color
        ///
        Color getIntensity(const Intensity intensity) const;


        LightSource& setAttenuation(const Attenuation attenuation, const float value);

        LightSource& setAttenuation(const float constant, const float linear, const float quadratic, const float range);

        float getAttenuation(const Attenuation attenuation) const;

        LightSource& setCutoff(const float inner, const float outer);

        const glm::vec2& getCutoff() const;

        static unsigned int getMaximumLights(const Type type);
        
    private:

        Type m_type;                           ///< Type identifier
        std::array<Color, 3> m_intensities;    ///< Array of colors
        glm::vec4 m_attenuation;
        glm::vec2 m_cutoff;
    };


    class LightContainer
    {
    public:

        typedef std::vector<const LightSource*> ContainerType;

    public:

        LightContainer();


        void clear();

        ContainerType& operator [](const LightSource::Type type);

        const ContainerType& operator [](const LightSource::Type type) const;

    private:

        std::array<ContainerType, 3> m_container;
    };
}

#endif

/// \class LightSource
/// \ingroup Graphics
///
/// Do explanation about the class