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

#ifndef JOP_DRAWABLE_HPP
#define JOP_DRAWABLE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <Jopnal/Graphics/Model.hpp>
#include <Jopnal/Graphics/RenderPass.hpp>
#include <Jopnal/Utility/Json.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <utility>

//////////////////////////////////////////////


namespace jop
{
    class ShaderProgram;
    class LightSource;
    class LightContainer;
    class Renderer;

    class JOP_API Drawable : public Component
    {
    protected:

        JOP_GENERIC_COMPONENT_CLONE(Drawable);

    public:
    
        /// Drawable flags
        ///
        enum Flag : uint32
        {
            ReceiveLights   = 1,        ///< Receive lights?
            ReceiveShadows  = 1 << 1,   ///< Receive shadows?
            CastShadows     = 1 << 2,   ///< Cast shadows?
            Reflected       = 1 << 3    ///< Reflect on dynamic environment maps?
        };

        /// Attribute flags
        ///
        struct Attribute
        {
            enum : uint64
            {
                __SkySphere = 1 << 10,
                __SkyBox    = __SkySphere << 1
            };
        };

        /// Projection info
        ///
        struct JOP_API ProjectionInfo
        {
            JOP_DISALLOW_COPY_MOVE(ProjectionInfo);

        public:

            /// \brief Constructor
            ///
            /// \param view The view matrix
            /// \param proj The projection matrix
            /// \param camPos The camera position
            ///
            ProjectionInfo(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camPos);

            const glm::mat4& viewMatrix;        ///< View matrix
            const glm::mat4& projectionMatrix;  ///< Projection matrix
            const glm::vec3& cameraPosition;    ///< Camera position
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param renderer Reference to the renderer
        /// \param pass The render pass
        ///
        Drawable(Object& object, Renderer& renderer, const RenderPass::Pass pass = RenderPass::Pass::BeforePost);

        /// \brief Overloaded constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param pass The render pass to bind this drawable into
        ///
        Drawable(Object& object, RenderPass& pass);

        /// \brief Virtual destructor
        ///
        virtual ~Drawable() override;


        /// \brief Draw function
        /// 
        /// \param proj The projection info
        /// \param lights The light container
        ///
        virtual void draw(const ProjectionInfo& proj, const LightContainer& lights) const;

        /// \brief Get the renderer this drawable is bound to
        ///
        /// \return Reference to the renderer
        ///
        Renderer& getRendrer();

        /// \brief Get the renderer this drawable is bound to
        ///
        /// \return Reference to the renderer
        ///
        const Renderer& getRenderer() const;

        /// \brief Set the render group
        ///
        /// The value will be clamped between 0 and 31.
        ///
        /// \param group The new group to set
        ///
        /// \comm setRenderGroup
        ///
        Drawable& setRenderGroup(const uint8 group);

        /// \brief Get the render group
        ///
        /// \return The render group 
        ///
        uint8 getRenderGroup() const;

        /// \brief Set the model
        ///
        /// The model will be copied.
        ///
        /// \param model Reference to the model
        ///
        /// \return Reference to self
        ///
        /// \comm setModel
        ///
        Drawable& setModel(const Model& model);

        /// \brief Get the model
        ///
        /// \return Reference to the model
        ///
        Model& getModel();

        /// \copydoc getModel()
        ///
        const Model& getModel() const;

        /// \brief Set the color
        ///
        /// This sets the drawable-specific color to be used as a base.
        /// If the bound material has a texture, the color will be used
        /// as a multiplier. The default color is white with an alpha of 1.
        ///
        /// \param color The color to set
        ///
        /// \return Reference to self
        ///
        Drawable& setColor(const Color& color);

        /// \brief Get the color
        ///
        /// \return Reference to the color
        ///
        /// \see setColor()
        ///
        const Color& getColor() const;

        /// \brief Get the local bounds
        ///
        /// This is the same as calling %getModel().%getMesh()->%getBounds().
        ///
        /// \return The local bounds
        ///
        const std::pair<glm::vec3, glm::vec3>& getLocalBounds() const;

        /// \brief Get the global bounds
        ///
        /// This will apply the current transformation to the local bounds and
        /// then return them.
        ///
        /// \return The global bounds
        ///
        std::pair<glm::vec3, glm::vec3> getGlobalBounds() const;

        /// \brief Set flags
        ///
        /// \param flags The flags to set
        ///
        /// \return Reference to self
        ///
        /// \see Flag
        ///
        Drawable& setFlags(const uint32 flags);

        /// \brief Check if this drawable has a flag
        ///
        /// \param flag The flag to check
        ///
        /// \return True if this drawable has the flag
        ///
        /// \see Flag
        ///
        bool hasFlag(const uint32 flag) const;

        /// \brief Set attributes
        ///
        /// \param attributes The attributes to set
        ///
        /// \return Reference to self
        ///
        Drawable& setAttributes(const uint64 attributes);

        /// \brief Add attributes
        ///
        /// \param attributes The attributes to add
        ///
        /// \return Reference to self
        ///
        Drawable& addAttributes(const uint64 attributes);

        /// \brief Get the attribute field
        ///
        /// \return The attribute field
        ///
        uint64 getAttributes() const;

        /// \brief Check if this drawable has a certain attribute
        ///
        /// \param attribute The attribute to check
        ///
        /// \return True if this drawable has the attribute
        ///
        bool hasAttribute(const uint64 attribute) const;

        /// \brief Get a shader pre-processor string
        ///
        /// \param attribs The drawable attributes
        /// \param str The string to write into
        ///
        static void getShaderPreprocessorDef(const uint64 attribs, std::string& str);

    protected:

        /// \copydoc Component::receiveMessage()
        ///
        virtual Message::Result receiveMessage(const Message& message) override;

        /// \brief Get the current shader
        ///
        /// \return Reference to the shader
        ///
        ShaderProgram& getShader() const;

    private:

        Color m_color;                                  ///< Color specific to this drawable
        Model m_model;                                  ///< The bound model
        mutable WeakReference<ShaderProgram> m_shader;  ///< The bound shader
        uint64 m_attributes;                            ///< Attribute flags
        Renderer& m_rendererRef;                        ///< Reference to the renderer
        const RenderPass::Pass m_pass;                  ///< The render pass
        uint32 m_flags;                                 ///< Property flags
        uint8 m_renderGroup;                            ///< The render group
        mutable bool m_updateShader;                    ///< Must the shader be updated?
    };
}

/// \class jop::Drawable
/// \ingroup graphics

#endif