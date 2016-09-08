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
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <Jopnal/Graphics/RenderPass.hpp>
#include <Jopnal/Graphics/RenderTexture.hpp>
#include <Jopnal/Graphics/Texture/Cubemap.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <array>
#include <set>

///////////////////////////////////////////


namespace jop
{
    class Object;
    class ShaderProgram;
    class Renderer;
    class RenderTexture;
    class Drawable;

    class JOP_API LightSource final : public Component
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(LightSource);

        friend class Renderer;

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
            Quadratic
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to created object in object class 
        /// \param renderer Reference to the renderer
        /// \param type The light type
        ///
        LightSource(Object& object, Renderer& renderer, const Type type);

        /// \brief Destructor
        ///
        ~LightSource() override;


        /// \brief Set the render mask
        ///
        /// \param mask The new mask to set
        ///
        void setRenderMask(const uint32 mask);

        /// \brief Get the render mask
        ///
        /// \return The render mask
        ///
        uint32 getRenderMask() const;

        /// \brief Get the light type
        ///
        /// \return The light type
        ///
        Type getType() const;

        /// \brief Set shadow casting for this light
        ///
        /// This can be an expensive function, do not call frequently.
        ///
        /// \param castShadows True to cast shadows
        /// \param resolution The shadow map resolution, glm::uvec2(0) to use the default
        ///
        /// \return Reference to self
        ///
        LightSource& setCastShadows(const bool castShadows, const glm::uvec2& resolution = glm::uvec2(0));

        /// \brief Check if this light casts shadows
        ///
        /// \return True if this light casts shadows
        ///
        bool castsShadows() const;

        /// \brief Get a light space matrix
        ///
        /// Faces other than First are only available with point lights.
        /// Trying to get these matrices when the light type is Spot or
        /// Directional will result in overstepping the array
        /// 
        /// \param face The face whose matrix to fetch
        ///
        /// \return Reference to the matrix
        ///
        const glm::mat4& getLightspaceMatrix(const Cubemap::Face face = Cubemap::Face::First) const;

        /// \brief Get the shadow map
        ///
        /// \return Const reference to the shadow map. nullptr if no shadow map exists
        ///
        const Texture* getShadowMap() const;

        /// \brief Set a specific intensity value
        ///
        /// \param intensity The intensity type
        /// \param color The color value to set
        ///
        /// \return Reference to self
        ///
        /// \comm setIntensity
        ///
        LightSource& setIntensity(const Intensity intensity, const Color& color);
        
        /// \brief Set all intensity values
        ///
        /// \param ambient Ambient intensity
        /// \param diffuse Diffuse intensity
        /// \param specular Specular intensity
        ///
        /// \return Reference to self
        ///
        LightSource& setIntensity(const Color& ambient, const Color& diffuse, const Color& specular);

        /// \brief Set all intensity values
        ///
        /// All the intensity values will be set to the given color value.
        ///
        /// \param intensity The intensity to set
        ///
        /// \return Reference to self
        ///
        LightSource& setIntensity(const Color& intensity);

        /// \brief Get an intensity value
        ///
        /// \param intensity The intensity type
        ///
        /// \return The intensity value
        ///
        const Color& getIntensity(const Intensity intensity) const;

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
        ///
        /// \return Reference to self
        ///
        /// \comm setAttenuation
        /// 
        LightSource& setAttenuation(const float constant, const float linear, const float quadratic);

        /// \brief Set the attenuation values using a range
        ///
        /// This roughly estimates the attenuation values for the given range.
        /// This aims for realism, which in some cases might mean that the light
        /// will become too dark.
        ///
        /// \param range The range
        ///
        /// \return Reference to self
        ///
        LightSource& setAttenuation(const float range);

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
        ///
        /// \return A vector with the attenuation values
        ///
        glm::vec3 getAttenuation() const;

        /// \brief Get the approximate maximum range
        ///
        /// The returned value may deviate from what was possibly previously passed
        /// to setAttenuation(). The calculation is a mere approximation but is
        /// to usually enough to determine the range in which the light is at all visible.
        ///
        /// \return The maximum range
        ///
        float getRange() const;

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
        /// \comm setCutoff
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

        /// \brief Calculate cube map view matrices
        ///
        /// \param projection The projection matrix
        /// \param position The view position
        /// \param viewMats Reference to a vector with the matrices. This must have a size of at least 6
        ///
        static void makeCubemapMatrices(const glm::mat4& projection, const glm::vec3& position, std::vector<glm::mat4>& viewMats);
        
    protected:

        /// \copydoc Component::receiveMessage()
        ///
        Message::Result receiveMessage(const Message& message) override;

    private:

        bool drawShadowMap() const;


        mutable RenderTexture m_shadowMap;                      ///< The shadow map
        mutable std::vector<glm::mat4> m_lightSpaceMatrices;    ///< Light space matrices. Used when rendering the shadow map
        const Type m_type;                                      ///< The light type
        std::array<Color, 3> m_intensities;                     ///< The intensities
        glm::vec3 m_attenuation;                                ///< The attenuation values    
        glm::vec2 m_cutoff;                                     ///< Spot light cutoff
        Renderer& m_rendererRef;                                ///< Reference to the renderer
        uint32 m_renderMask;                                    ///< The render mask
    };

    /// \brief Container for lights
    ///
    /// Meant to be passed to Drawable::draw().
    ///
    class JOP_API LightContainer
    {
    public:

        typedef std::vector<const LightSource*> ContainerType;

    public:

        /// \brief Constructor
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
        /// \param drawable The drawable
        /// \param viewMatrix The current view matrix
        ///
        void sendToShader(ShaderProgram& shader, const Drawable& drawable, const glm::mat4& viewMatrix) const;

        /// \brief Access the individual containers for each light type
        ///
        /// \param type The light type
        ///
        /// \return Reference to the container
        ///
        ContainerType& operator [](const LightSource::Type type);

        /// \copydoc operator[]()
        ///
        const ContainerType& operator [](const LightSource::Type type) const;

    private:

        std::array<ContainerType, 3> m_container;   ///< Containers for each type of light
    };
}

/// \class jop::LightSource
/// \ingroup graphics

#endif