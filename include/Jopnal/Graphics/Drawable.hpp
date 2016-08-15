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
    
        enum Flag : uint32
        {
            ReceiveLights   = 1,
            ReceiveShadows  = 1 << 1,
            CastShadows     = 1 << 2,
            Reflected       = 1 << 3
        };

        struct Attribute
        {
            enum : uint64
            {


                __SkySphere = 1 << 10,
                __SkyBox    = __SkySphere << 1
            };
        };

        struct JOP_API ProjectionInfo
        {
            JOP_DISALLOW_COPY_MOVE(ProjectionInfo);

            ProjectionInfo(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camPos);

            const glm::mat4& viewMatrix;
            const glm::mat4& projectionMatrix;
            const glm::vec3& cameraPosition;
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param renderer Reference to the renderer
        /// \param ID Component identifier
        ///
        Drawable(Object& object, Renderer& renderer, const RenderPass::Pass pass = RenderPass::Pass::BeforePost);

        Drawable(Object& object, RenderPass& pass);

        /// \brief Virtual destructor
        ///
        virtual ~Drawable() override;


        /// \brief Base draw function
        ///
        /// This will use the shader bound to this drawable.
        /// 
        /// \param camera The camera to use
        /// \param lights The light container
        ///
        virtual void draw(const ProjectionInfo& proj, const LightContainer& lights) const;

        /// \brief Get the renderer this drawable is bound to
        ///
        /// \return Reference to the renderer
        ///
        Renderer& getRendrer();

        /// \copydoc getRenderer()
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
        /// \comm setModel
        ///
        /// \return Reference to self
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

        Drawable& setColor(const Color& color);

        const Color& getColor() const;

        const std::pair<glm::vec3, glm::vec3>& getLocalBounds() const;

        const std::pair<glm::vec3, glm::vec3>& getGlobalBounds() const;

        Drawable& setFlags(const uint32 flags);

        bool hasFlag(const uint32 flag) const;

        Drawable& setAttributes(const uint64 attributes);

        Drawable& addAttributes(const uint64 attributes);

        uint64 getAttributes() const;

        bool hasAttribute(const uint64 attribute) const;

        static void getShaderPreprocessorDef(const uint64 attribs, std::string& str);

    protected:

        virtual Message::Result receiveMessage(const Message& message) override;

        ShaderProgram& getShader() const;

    private:

        Color m_color;
        Model m_model;                          ///< The bound model
        mutable WeakReference<ShaderProgram> m_shader;  ///< The bound shader
        uint64 m_attributes;
        Renderer& m_rendererRef;                ///< Reference to the renderer
        const RenderPass::Pass m_pass;
        uint32 m_flags;                         ///< Property flags
        mutable std::pair<glm::vec3, glm::vec3> m_globalBounds;
        uint8 m_renderGroup;                    ///< The render group
        mutable bool m_updateShader;
        mutable bool m_updateBounds;
    };
}

#endif

/// \class Drawable
/// \ingroup graphics
///
/// #TODO Detailed description